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
from tase.items import FinancialStatement
from tase.items import NewsArticle

import urllib
from urlparse import urlparse
from urlparse import parse_qs
#from urlparse import urljoin

import tase.common

from html2text import html2text

#import string
#import random


PROCESS_HISTORY = settings.getbool('PROCESS_HISTORY', False)
HISTORY_PERIOD = settings.getint('HISTORY_PERIOD', 2) # 1 month
category_comp = settings.get('CATEGORY_COMP')
PROCESS_NEWS  = settings.getbool('PROCESS_NEWS', False)
PROCESS_NEWS_HISTORY  = settings.getbool('PROCESS_NEWS_HISTORY', False)
PROCESS_NEWS_CONTENT  = settings.getbool('PROCESS_NEWS_CONTENT', True)
PROCESS_FINANCIAL_STATEMENTS = settings.getbool('PROCESS_FINANCIAL_STATEMENTS', False)


class StockSpider(HistorySpider):
	name = 'stocks'
	allowed_domains = ['tase.co.il', 'globes.co.il']
	start_urls = ['http://www.tase.co.il/TASEEng/MarketData/Stocks/MarketData/']

	rules = (
		Rule(SgmlLinkExtractor(allow=('MarketData\.htm',)), callback='parse_company_list'),
		Rule(SgmlLinkExtractor(allow=('companyMainData\.aspx',)), callback='parse_company'),
		Rule(SgmlLinkExtractor(allow=('companyhistorydata\.aspx',)), callback='get_history_data'),
		Rule(SgmlLinkExtractor(allow=('companyDetails\.htm',)), callback='parse_company_details'),
#		Rule(SgmlLinkExtractor(allow=('searchresults\.asp',)), callback='parse_company_news'),
#		Rule(SgmlLinkExtractor(allow=('_page\d\.html',)), callback='parse_company_news'),
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
		'Dividends' : ('dividends', tase.common.to_float),
		'Net cash flow generated by operating activities' : ('net_cash_flow_generated_by_operating_activities', tase.common.to_long),
		#'Financial Ratios' : ('', tase.common.to_long),
		'Market to book value' : ('market_to_book_value', tase.common.to_float),
		'Price-Earning Ratio' : ('price_earning_ratio', tase.common.to_float),
		'Equity-Assets Ratio' : ('equity_assets_ratio', tase.common.to_float),
		'Return on Equity' : ('return_on_equity', tase.common.to_float)
	}

	details_url = "http://www.tase.co.il/TASEEng/General/Company/companyDetails.htm?subDataType=0&companyID={companyID}&shareID={shareID}"
	history_url = "http://www.tase.co.il/TASEEng/General/Company/companyHistoryData.htm?subDataType=0&companyID={companyID}&shareID={shareID}&intPeriod={period}&intFrequency1=0&IsYield=False&IsDollar=False"

	# Main companies list, with paging
	def parse_company_list(self, response):
		hxs = HtmlXPathSelector(response)
		links = hxs.select("//tr[@class='pagerText']/td/a")
		for link in links:
			m = re.search("javascript:__doPostBack\('(.*?)'", link.extract())
			if m:
				url = urllib.unquote(m.group(1))
				yield FormRequest(self.start_urls[0], method='POST', formdata={'__EVENTTARGET': url, '__EVENTARGUMENT': ''})

	def parse_company(self, response):
		hxs = HtmlXPathSelector(response)
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
			item['name'] = hxs.select("//td[@class='BigBlue']/text()").extract()[0]
		except IndexError:
			item['name'] = ""
		lst = hxs.select("//td[contains(child::text(), 'Symbol:')]/following-sibling::td[1]/table/tr/td[1]/text()").extract()
		if len(lst) > 0:
			item['symbol'] = lst[0]
		else:
			try:
				item['symbol'] = hxs.select("//td[contains(., 'Symbol:')]/following-sibling::td[1]/text()").extract()[0]
			except IndexError:
				item['symbol'] = item['ShareID']
		href = hxs.select('//tr[1]/td[1]/a[@target="_blank"]/@href').extract()
		url = href[0]
		o = urlparse(url)
		if len(o.netloc) > 0:
			item['url'] = url
		else:
			item['url'] = ''
		try:
			href = hxs.select("//tr/td[@class='subtitle']/text()").extract()
			item['sector'] = tase.common.unescape(urllib.unquote(href[4].strip()))
			item['subsector'] = tase.common.unescape(urllib.unquote(href[3].strip()))
		except IndexError:
			item['sector'] = ""
			item['subsector'] = ""
		if PROCESS_FINANCIAL_STATEMENTS:
			yield self.get_company_details(item)
		#url = "http://archive.globes.co.il/searchgl/%s" % item['symbol']
		url = "http://www.globes.co.il/serveen/globes/searchresults.asp?exact=%s" % item['symbol']
		if PROCESS_NEWS:
			yield Request(url, callback=self.parse_company_news, meta={'item': item})
		yield self.process_history(item)
	
	def get_company_details(self, item):
		url = self.details_url.format(shareID=item['ShareID'], companyID=item['CompanyID'])
		return Request(url, callback=self.parse_company_details, meta={'item': item})

	def parse_company_details(self, response):
		item = response.request.meta['item']
		hxs = HtmlXPathSelector(response)
		item['financial_statements'] = []
		for i in range(3):
			fs = self.process_company_statement(hxs, i)
			if not fs is None:
				item['financial_statements'].append(fs)
		return self.process_history(item)

	def process_company_statement(self, hxs, index):
		table = hxs.select('//table[@id="gridFinanceReport_DataGrid1"]')
		rows = table.select('tr')#[@class != "gridHeader"]')
		fs = FinancialStatement()
		start = True
		for row in rows:
			if start:
				columns = row.select('td[@class="titleGridReg"]/text()')
				if index >= len(columns):
					return None
				fs['period'] = columns[index].extract()
				start = False
			else:
				name = row.select('td/text()')[0].extract().strip()
				values = row.select('td/div/text()')
				if len(values) > 0:
					value = values[index].extract().strip()
					if not self.header2.get(name) is None:
						key = self.header2[name][0]
						func = self.header2[name][1]
						fs[key] = func(value)
		return fs

	def parse_company_news(self, response):
		self.log(response.url)
		item = response.request.meta['item']
		hxs = HtmlXPathSelector(response)
		#links = hxs.select("//tr[@class='RowPrint']")
		links = hxs.select("//div[@class='views_content']")
		for link in links:
			article = NewsArticle()
			article['symbol'] = item['symbol']
			a = link.select("a")
			if len(a) == 0:
				continue
			article['url'] = 'http://www.globes.co.il' + tase.common.get_string(a.select("@href").extract())
			#article['random_string'] = ''.join(random.choice(string.ascii_uppercase + string.digits) for x in range(10))
			#next = link.select("following-sibling::tr[*]/td[@class='SubHeader']/a")[0]
			#summary1 = tase.common.get_string(next.select("text()").extract())
			#next = link.select("following-sibling::tr[*]/td[@class='excerpt']/doc")[0]
			#article['summary'] = summary1 + ' ' + html2text(tase.common.get_string(next.extract()))
			#next = link.select("following-sibling::tr[*]/td[@class='DateIndex']/a")[0]
			#strdate = next.select("text()").extract()
			article['headline'] = tase.common.get_string(a.select('text()').extract())
			p = link.select("p")
			if len(p) < 2:
				continue
			summary = tase.common.get_string(p[0].select('text()').extract())
			if len(summary) < 10:
				return
			article['summary'] = summary
			try:
				strdate = p[1].select("span/text()")[0].extract()
				article['date_'] = tase.common.get_date2(strdate)
				if PROCESS_NEWS_CONTENT:
					yield Request(article['url'], callback=self.parse_company_news_content, dont_filter=True, meta={'article': article})
				else:
					yield article
			except ValueError:
				self.log('Error reading date on page %s' % response.url)
				tase.common.log2('Error reading date on page %s' % response.url)
		if PROCESS_NEWS_HISTORY:
			a = hxs.select("//a[contains(., '>>')]")
			if len(a) > 0:
				href = tase.common.get_string(a[0].select("@href").extract())
				url = 'http://www.globes.co.il' + href
				yield Request(url, callback=self.parse_company_news, meta={'item': item})

	def parse_company_news_content(self, response):
		self.log(response.url)
		article = response.request.meta['article']
		hxs = HtmlXPathSelector(response)
		try:
			#content = html2text(tase.common.get_string(hxs.select("//p[@id='content']").extract()))
			content = html2text(tase.common.get_string(hxs.select("//div[@id='EN_main_Content']").extract()))
		except:
			content = ''
			tase.common.log2('Error reading content on page %s' % response.url)
		article['content'] = content
		return article
