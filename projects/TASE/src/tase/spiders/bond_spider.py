import re

from scrapy.contrib.spiders import Rule
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor

from scrapy.selector import HtmlXPathSelector
from scrapy.http import Request
from scrapy.http import FormRequest
from scrapy.conf import settings
#from scrapy.shell import inspect_response

from scrapy.utils.response import get_base_url

from tase.HistorySpider import HistorySpider
from tase.items import TaseItem
#from tase.items import FinancialStatement

import urllib
from urlparse import urlparse
from urlparse import parse_qs
#from urlparse import urljoin

import tase.common


PROCESS_HISTORY = settings.getbool('PROCESS_HISTORY', False)
HISTORY_PERIOD = settings.getint('HISTORY_PERIOD', 2)
category_bond = settings.get('CATEGORY_BOND')


class BondSpider(HistorySpider):
	name = 'bonds'
	allowed_domains = ['tase.co.il']
	start_urls = [
		'http://www.tase.co.il/eng/marketdata/t-bills/Pages/ShortTermLoan.aspx',
		'http://www.tase.co.il/eng/marketdata/bonds/governmentbonds/Pages/BondsGov.aspx',
		'http://www.tase.co.il/eng/marketdata/bonds/corporatebonds/Pages/BondsByCuts.aspx',
	]

	rules = (
		Rule(SgmlLinkExtractor(allow=(r'ErrorHandler.aspx',)), callback='process_error'),
		Rule(SgmlLinkExtractor(allow=('ShortTermLoan\.aspx',)), callback='parse_bond_list'),
		Rule(SgmlLinkExtractor(allow=('BondsGov\.aspx',)), callback='parse_bond_list'),
		Rule(SgmlLinkExtractor(allow=('BondsByCuts\.aspx',)), callback='parse_bond_list'),
		Rule(SgmlLinkExtractor(allow=('BondsMainData\.aspx',)), callback='parse_bond'),
		Rule(SgmlLinkExtractor(allow=('companyMainData\.aspx',)), callback='parse_bond'),
	)

	header = (
		('date_', tase.common.get_date),
		('adjusted_closing_price', tase.common.to_float),
		('closing_price', tase.common.to_float),
		('change_', tase.common.to_float),
		('gross_yield_to_maturity', tase.common.to_float),
		('opening_price', tase.common.to_float),
		('base_price', tase.common.to_float),
		('high', tase.common.to_float),
		('low', tase.common.to_float),
		('capital_listed_for_trading', tase.common.to_int),
		('market_cap', tase.common.to_int),
		('turnover', tase.common.to_int),
		('volume', tase.common.to_int),
		('trans', tase.common.to_int)
	)

	history_url = "http://www.tase.co.il/TASEEng/General/BONDs/bondsHistoryData.htm?bondType=4&subDataType=5&companyID={companyID}&shareID={shareID}&intPeriod={period}&intFrequency1=0&IsYield=False&IsDollar=False"

	def get_control_id(self):
		return "g_ed8af170_7f0e_440a_85fe_19d9352a2a86"

	# Main companies list, with paging
	def parse_bond_list(self, response):
		hxs = HtmlXPathSelector(response)
		fd = dict()
		inputs = hxs.select("//input[@type='hidden']")
		for inpt in inputs:
			name = tase.common.get_string(inpt.select("@name").extract())
			value = tase.common.get_string(inpt.select("@value").extract())
			fd[name] = value
		links = hxs.select("//tr[@class='pagerText']/td/a")
		for link in links:
			m = re.search("javascript:__doPostBack\('(.*?)'", link.extract())
			if m:
				url = urllib.unquote(m.group(1))
				fd['__EVENTTARGET'] = url
				#print self.start_urls[2]
				#print fd
				#yield FormRequest(self.start_urls[2], method='POST', formdata={'__EVENTTARGET': url, '__EVENTARGUMENT': ''})
				yield FormRequest(self.start_urls[2], method='POST', formdata=fd)

	# almost same as parse_company
	def parse_bond(self, response):
		hxs = HtmlXPathSelector(response)
		item = TaseItem()
		item['category'] = category_bond
		item['tase_url'] = response.url
		item['date_'] = ''
		query = parse_qs(urlparse(response.url)[4]) # query
		try:
			item['CompanyID'] = query['CompanyID'][0]
		except KeyError:
			item['CompanyID'] = query['FundID'][0]
		try:
			item['ShareID'] = query['ShareID'][0]
		except KeyError:
			item['ShareID'] = query['FundID'][0]
		try:
			item['name'] = hxs.select("//td[@class='BigBlue']/text()").extract()[0]
		except IndexError:
			item['name'] = ""
		try:
			base_url = get_base_url(response)
			relative_url = hxs.select("//td[@rowspan='4']/img/@src").extract()[0]
			item['image_url'] = urljoin(base_url, relative_url)
		except IndexError:
			item['image_url'] = ""
		lst = hxs.select("//td[contains(child::text(), 'Symbol:')]/following-sibling::td[1]/table/tr/td[1]/text()").extract()
		if len(lst) > 0:
			item['symbol'] = lst[0]
		else:
			try:
				item['symbol'] = hxs.select("//td[contains(., 'Symbol:')]/following-sibling::td[1]/text()").extract()[0]
			except IndexError:
				item['symbol'] = item['ShareID']
		href = hxs.select('//tr[1]/td[1]/a[@target="_blank"]/@href').extract()
		if len(href) > 0:
			url = href[0]
			o = urlparse(url)
			if len(o.netloc) > 0:
				item['url'] = url
			else:
				item['url'] = ''
		else:
			item['url'] = ''
		try:
			href = hxs.select("//tr/td[@class='subtitle']/text()").extract()
			item['sector'] = tase.common.unescape(urllib.unquote(href[4].strip()))
			item['subsector'] = tase.common.unescape(urllib.unquote(href[3].strip()))
		except IndexError:
			item['sector'] = ""
			item['subsector'] = ""
		item['sector_int'] = 0
		item['subsector_int'] = 0
		#url = "http://archive.globes.co.il/searchgl/%s" % item['symbol']
		url = "http://www.globes.co.il/serveen/globes/searchresults.asp?exact=%s" % item['symbol']
		yield self.process_history(item)
