import re

from scrapy.contrib.spiders import Rule
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor

from scrapy.selector import Selector
from scrapy.http import Request
from scrapy.http import FormRequest
from scrapy.conf import settings
#from scrapy.shell import inspect_response

from scrapy.utils.response import get_base_url

from tase.HistorySpider import HistorySpider
from tase.items import TaseItem
from tase.items import FinancialStatement

import urllib
from urlparse import urlparse
from urlparse import parse_qs
from urlparse import urljoin

import tase.common

from html2text import html2text

#import string
#import random


PROCESS_HISTORY = settings.getbool('PROCESS_HISTORY', False)
HISTORY_PERIOD = settings.getint('HISTORY_PERIOD', 2) # 1 month
category_comp = settings.get('CATEGORY_COMP')
PROCESS_FINANCIAL_STATEMENTS = settings.getbool('PROCESS_FINANCIAL_STATEMENTS', False)


class StockSpider(HistorySpider):
	name = 'stocks'
	allowed_domains = ['tase.co.il']
	start_urls = ['http://www.tase.co.il/eng/marketdata/stocks/marketdata/Pages/MarketData.aspx']

	rules = (
		Rule(SgmlLinkExtractor(allow=('MarketData\.aspx',)), callback='parse_company_list'),
		Rule(SgmlLinkExtractor(allow=('companyMainData\.aspx',)), callback='parse_company'),
		Rule(SgmlLinkExtractor(allow=('companyhistorydata\.aspx',)), callback='get_history_data'),
		Rule(SgmlLinkExtractor(allow=('companyDetails\.htm',)), callback='parse_company_details'),
	)
	
	header = (
		('date_', tase.common.get_date),
		('adjusted_closing_price', tase.common.to_float),
		('closing_price', tase.common.to_float),
		('change_', tase.common.to_float),
		('opening_price', tase.common.to_float),
		('base_price', tase.common.to_float),
		('high', tase.common.to_float),
		('low', tase.common.to_float),
		('capital_listed_for_trading', tase.common.to_long),
		('market_cap', tase.common.to_long),
		('turnover', tase.common.to_long),
		('volume', tase.common.to_long),
		('trans', tase.common.to_long)
	)

	header2 = {
		'Total Assets' : ('total_assets', tase.common.to_long),
		'Current Assets' : ('current_assets', tase.common.to_long),
		'Non Current Assets' : ('non_current_assets', tase.common.to_long),
		'Shareholders Equity' : ('shareholders_equity', tase.common.to_long),
		'Of which Minority Interest' : ('of_which_minority_interest', tase.common.to_long),
		'Current Liabilities' : ('current_liabilities', tase.common.to_long),
		'Long-Term Liabilities' : ('long_term_liabilities', tase.common.to_long),
		#'Profit & Loss Statement' : ('', tase.common.to_long),
		'Revenues' : ('revenues', tase.common.to_long),
		'Gross Profit' : ('gross_profit', tase.common.to_long),
		'Operating Income' : ('operating_income', tase.common.to_long),
		'Income Before Tax' : ('income_before_tax', tase.common.to_long),
		'Net Income' : ('net_income', tase.common.to_long),
		'Income Attributable to Shareholders' : ('income_atributable_to_shareholders', tase.common.to_long),
		'Earnings per Share' : ('earnings_per_share', tase.common.to_float),
		#'Additional Data' : ('', tase.common.to_long),
		'Dividends' : ('dividends', tase.common.to_long),
		'Net cash flow generated by operating activities' : ('net_cash_flow_generated_by_operating_activities', tase.common.to_long),
		#'Financial Ratios' : ('', tase.common.to_long),
		'Market to book value' : ('market_to_book_value', tase.common.to_float),
		'Price-Earning Ratio' : ('price_earning_ratio', tase.common.to_float),
		'Equity-Assets Ratio' : ('equity_assets_ratio', tase.common.to_float),
		'Return on Equity' : ('return_on_equity', tase.common.to_float)
	}

	details_url = "http://www.tase.co.il/Eng/General/Company/Pages/companyDetails.aspx?subDataType=0&companyID={companyID}&shareID={shareID}"
	history_url = "http://www.tase.co.il/Eng/General/Company/Pages/companyHistoryData.aspx?subDataType=0&companyID={companyID}&shareID={shareID}&intPeriod={period}&intFrequency1=0&IsYield=False&IsDollar=False"

	def get_control_id(self):
		return "g_301c6a3d_c058_41d6_8169_6d26c5d97050"

	# Main companies list, with paging
	def parse_company_list(self, response):
		sel = Selector(response)
		fd = dict()
		inputs = sel.xpath("//input[@type='hidden']")
		for inpt in inputs:
			name = tase.common.get_string(inpt.xpath("@name").extract())
			value = tase.common.get_string(inpt.xpath("@value").extract())
			fd[name] = value
		#print fd
		#req_digest = sel.xpath("//input[@id='__REQUESTDIGEST']/@value").extract()
		#ev_val = sel.xpath("//input[@id='__EVENTVALIDATION']/@value").extract()
		links = sel.xpath("//tr[@class='pagerText']/td/a")
		for link in links:
			m = re.search("javascript:__doPostBack\('(.*?)'", link.extract())
			if m:
				url = urllib.unquote(m.group(1))
				fd['__EVENTTARGET'] = url
				#yield FormRequest(self.start_urls[0], method='POST', formdata={'__EVENTTARGET': url, '__EVENTARGUMENT': '', '__REQUESTDIGEST': req_digest, '__EVENTVALIDATION': ev_val})
				#print "url: " + self.start_urls[0]
				yield FormRequest(self.start_urls[0], method='POST', formdata=fd)

	def parse_company(self, response):
		sel = Selector(response)
		item = TaseItem()
		item['category'] = category_comp
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
			item['name'] = sel.xpath("//td[@class='BigBlue']/text()").extract()[0]
		except IndexError:
			item['name'] = ""
		try:
			base_url = get_base_url(response)
			relative_url = sel.xpath("//td[@rowspan='4']/img/@src").extract()[0]
			item['image_url'] = urljoin(base_url, relative_url)
		except IndexError:
			item['image_url'] = ""
		lst = sel.xpath("//td[contains(child::text(), 'Symbol:')]/following-sibling::td[1]/table/tr/td[1]/text()").extract()
		if len(lst) > 0:
			item['symbol'] = lst[0]
		else:
			try:
				item['symbol'] = sel.xpath("//td[contains(., 'Symbol:')]/following-sibling::td[1]/text()").extract()[0]
			except IndexError:
				item['symbol'] = item['ShareID']
		href = sel.xpath('//tr[1]/td[1]/a[@target="_blank"]/@href').extract()
		url = href[0]
		o = urlparse(url)
		if len(o.netloc) > 0:
			item['url'] = url
		else:
			item['url'] = ''
		try:
			href = sel.xpath("//tr/td[@class='subtitle']/text()").extract()
			item['sector'] = tase.common.unescape(urllib.unquote(href[4].strip()))
			item['subsector'] = tase.common.unescape(urllib.unquote(href[3].strip()))
		except IndexError:
			item['sector'] = ""
			item['subsector'] = ""
		item['sector_int'] = 0
		item['subsector_int'] = 0
		if PROCESS_FINANCIAL_STATEMENTS:
			yield self.get_company_details(item)
		yield self.process_history(item)
	
	def get_company_details(self, item):
		url = self.details_url.format(shareID=item['ShareID'], companyID=item['CompanyID'])
		return Request(url, callback=self.parse_company_details, meta={'item': item})

	def parse_company_details(self, response):
		item = response.request.meta['item']
		sel = Selector(response)
		item['financial_statements'] = []
		for i in range(3):
			fs = self.process_company_statement(sel, i)
			if not fs is None:
				item['financial_statements'].append(fs)
		return self.process_history(item)

	def process_company_statement(self, sel, index):
		table = sel.xpath('//table[@id="ctl00_SPWebPartManager1_g_8e3d9f18_75c6_43cc_bc21_c3e7170427ca_ctl00_gridFinanceReport_DataGrid1"]')
		rows = table.xpath('tr')#[@class != "gridHeader"]')
		fs = FinancialStatement()
		start = True
		for row in rows:
			if start:
				columns = row.xpath('td[@class="titleGridReg"]/text()')
				if index >= len(columns):
					return None
				fs['period'] = columns[index].extract()
				start = False
			else:
				name = row.xpath('td/text()')[0].extract().strip()
				values = row.xpath('td/div/text()')
				if len(values) > 0:
					value = values[index].extract().strip()
					if not self.header2.get(name) is None:
						key = self.header2[name][0]
						func = self.header2[name][1]
						val = func(value)
						#self.log('Value: %s' % val)
						fs[key] = val
		return fs

