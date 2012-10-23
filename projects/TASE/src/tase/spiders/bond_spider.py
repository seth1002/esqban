import re

from scrapy.contrib.spiders import Rule
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor

from scrapy.selector import HtmlXPathSelector
from scrapy.http import Request
from scrapy.http import FormRequest
from scrapy.conf import settings
#from scrapy.shell import inspect_response

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
		'http://www.tase.co.il/TASEEng/MarketData/T-bills/',
		'http://www.tase.co.il/TASEEng/MarketData/Bonds/GovernmentBonds/',
		'http://www.tase.co.il/TASEEng/MarketData/Bonds/CorporateBonds/',
	]

	rules = (
		Rule(SgmlLinkExtractor(allow=(r'ErrorHandler.aspx',)), callback='process_error'),
		Rule(SgmlLinkExtractor(allow=('CorporateBonds',)), callback='parse_bond_list'),
		Rule(SgmlLinkExtractor(allow=('BondsMainData\.htm',)), callback='parse_bond'),
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

	# Main companies list, with paging
	def parse_bond_list(self, response):
		hxs = HtmlXPathSelector(response)
		links = hxs.select("//tr[@class='pagerText']/td/a")
		for link in links:
			m = re.search("javascript:__doPostBack\('(.*?)'", link.extract())
			if m:
				url = urllib.unquote(m.group(1))
				yield FormRequest(self.start_urls[0], method='POST', formdata={'__EVENTTARGET': url, '__EVENTARGUMENT': ''})

	def parse_bond(self, response):
		hxs = HtmlXPathSelector(response)
		item = TaseItem()
		item['category'] = category_bond
		item['tase_url'] = response.url
		query = parse_qs(urlparse(response.url)[4]) # query
		item['CompanyID'] = query['CompanyID'][0]
		item['ShareID'] = query['ShareID'][0]
		item['name'] = hxs.select("//td[@class='BigTitleInner']/text()").extract()[0]
		lst = hxs.select("//td[contains(child::text(), 'Symbol:')]/following-sibling::td[1]/table/tr/td[1]/text()").extract()
		if len(lst) > 0:
			item['symbol'] = lst[0]
		else:
			item['symbol'] = hxs.select("//td[contains(., 'Symbol:')]/following-sibling::td[1]/text()").extract()[0]
		item['url'] = ""
		lst = hxs.select("//tr[@class='subtitleText']/following-sibling::tr[1]/td/table/tr/td[@class='subtitle']/text()").extract()
		item['sector'] = tase.common.unescape(urllib.unquote(lst[0].strip()))
		if len(lst) > 1:
			item['subsector'] = tase.common.unescape(urllib.unquote(lst[1].strip()))
		else:
			item['subsector'] = ""
		return self.process_history(item)
