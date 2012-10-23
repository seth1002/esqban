import re
import csv
import time
import datetime

from scrapy.contrib.spiders import CrawlSpider, Rule
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor

from scrapy.selector import HtmlXPathSelector
from scrapy.http import Request
from scrapy.http import FormRequest
from scrapy.conf import settings
#from scrapy.shell import inspect_response

from tase.items import TaseItem

import urllib
from urlparse import urlparse
from urlparse import parse_qs
#from urlparse import urljoin


USE_CSV = settings.getbool('USE_CSV', False)
PROCESS_HISTORY = settings.getbool('PROCESS_HISTORY', False)
HISTORY_PERIOD = settings.getint('HISTORY_PERIOD', 2)
PROCESS_GRAPH = False # Not working yet
PROCESS_BONDS_AND_FUNDS = settings.getbool('PROCESS_BONDS_AND_FUNDS', True)

category_comp = settings.get('CATEGORY_COMP')
category_bond = settings.get('CATEGORY_BOND')
category_fund = settings.get('CATEGORY_FUND')

#Match date
#([0]?[1-9]|[1|2][0-9]|[3][0|1])[./-]([0]?[1-9]|[1][0-2])[./-]([0-9]{4}|[0-9]{2})

class TaseSpider(CrawlSpider):
	name = 'tase.co.il'
	allowed_domains = ['tase.co.il']
	start_urls = [
		#'http://www.tase.co.il/TASEEng/MarketData/',
		'http://www.tase.co.il/TASEEng/MarketData/Stocks/MarketData/',
		'http://www.tase.co.il/TASEEng/MarketData/T-bills/',
		'http://www.tase.co.il/TASEEng/MarketData/Bonds/GovernmentBonds/',
		'http://www.tase.co.il/TASEEng/MarketData/Bonds/CorporateBonds/',
		'http://www.tase.co.il/TASEEng/MarketData/MutualFunds/',
	]

	rules = (
		Rule(SgmlLinkExtractor(allow=(r'ErrorHandler.aspx',)), callback='process_error'),
		# Companies
		Rule(SgmlLinkExtractor(allow=('MarketData\.htm',)), callback='parse_company_list'),
		Rule(SgmlLinkExtractor(allow=('companyMainData\.htm',)), callback='parse_company'),
		Rule(SgmlLinkExtractor(allow=('CompanyHistory\.aspx',)), callback='get_history_data'),
		# Bonds
		Rule(SgmlLinkExtractor(allow=('BondsMainData\.htm',)), callback='parse_bond'),
		# Funds
		Rule(SgmlLinkExtractor(allow=[r'BuildCmb_6_1.js']), callback='parse_fund_list'),
		Rule(SgmlLinkExtractor(allow=('FundMainData\.htm',)), callback='parse_fund'),
		Rule(SgmlLinkExtractor(allow=(r'MutualFunds',)), callback='parse_fund_search'),

		#Rule(SgmlLinkExtractor(allow=('Export\.aspx',)), callback='parse_history'),
	)
	
#	def __init__(self):
#		self.headers = get_headers() 
	
	def process_error(self, response):
		self.log2("process_error: " + response)

	# Main companies list, with paging
	def parse_company_list(self, response):
		self.log2("parse_company_list: " + response.url)
		hxs = HtmlXPathSelector(response)
		links = hxs.select("//tr[@class='pagerText']/td/a")
		for link in links:
			m = re.search("javascript:__doPostBack\('(.*?)'", link.extract())
			if m:
				url = urllib.unquote(m.group(1))
				yield FormRequest(self.start_urls[0], method='POST', formdata={'__EVENTTARGET': url, '__EVENTARGUMENT': ''})

	def parse_company(self, response):
		self.log2("parse_company: " + response.url)
		#inspect_response(response)
		hxs = HtmlXPathSelector(response)
		item = TaseItem()
		item['category'] = category_comp
		item['tase_url'] = response.url
		item['date_'] = ''
		query = parse_qs(urlparse(response.url)[4]) # query
		item['CompanyID'] = query['CompanyID'][0]
		item['ShareID'] = query['ShareID'][0]
		item['name'] = hxs.select("//td[@class='BigBlue']/text()").extract()[0]
		lst = hxs.select("//td[contains(child::text(), 'Symbol:')]/following-sibling::td[1]/table/tr/td[1]/text()").extract()
		if len(lst) > 0:
			item['symbol'] = lst[0]
		else:
			item['symbol'] = hxs.select("//td[contains(., 'Symbol:')]/following-sibling::td[1]/text()").extract()[0]
		href = hxs.select('//tr[1]/td[1]/a[@target="_blank"]/@href').extract()
		url = href[0]
		o = urlparse(url)
		if len(o.netloc) > 0:
			item['url'] = url
		else:
			item['url'] = ''
		href = hxs.select("//tr/td[@class='subtitle']/text()").extract()
		item['sector'] = unescape(urllib.unquote(href[4].strip()))
		item['subsector'] = unescape(urllib.unquote(href[3].strip()))
		return self.process_history(item)

	def parse_bond(self, response):
		self.log2("parse_bond: " + response.url)
		#inspect_response(response)
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
		item['sector'] = unescape(urllib.unquote(lst[0].strip()))
		if len(lst) > 1:
			item['subsector'] = unescape(urllib.unquote(lst[1].strip()))
		else:
			item['subsector'] = ""
		return self.process_history(item)

	def parse_fund_search(self, response):
		self.log2("parse_fund_search: " + response.url)
		return Request('http://www.tase.co.il/TASE/SearchJSFiles/BuildCmb_6_1.js', callback=self.parse_fund_list)

	def parse_fund_list(self, response):
		self.log2("parse_fund_list: " + response.url)
		matches = re.findall("[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]", response.body)
		for m in matches:
			item = TaseItem()
			item['category'] = category_fund
			item['symbol'] = m
			item['CompanyID'] = m
			item['ShareID'] = m
			url = "http://www.tase.co.il/TASEEng/General/trustfund/fundmaindata.htm?FundID={fundID}".format(fundID=m)
			yield Request(url, callback=self.parse_fund, meta={'item': item})

	def parse_fund(self, response):
		self.log2("parse_fund: " + response.url)
#		hxs = HtmlXPathSelector(response)
		item = response.request.meta['item']
		item['tase_url'] = response.url
		item['name'] = item['symbol']
		item['sector'] = ""
		item['subsector'] = ""
		item['url'] = ""
		return self.process_history(item)

	def process_history(self, item):
		if PROCESS_HISTORY is False:
			return item
		else:
			url = ''
			if item['category'] == category_comp:
				url = "http://www.tase.co.il/TASE/Templates/Company/CompanyHistory.aspx?subDataType=0&companyID={companyID}&shareID={shareID}&intPeriod={period}&intFrequency1=0&IsYield=False&IsDollar=False"
			if item['category'] == category_bond:
				url = "http://www.tase.co.il/TASE/Templates/Bonds/BondsHistory.aspx?bondType=4&subDataType=5&companyID={companyID}&shareID={shareID}&intPeriod={period}&intFrequency1=0&IsYield=False&IsDollar=False"
			if item['category'] == category_fund:
				url = "http://www.tase.co.il/TASE/Templates/TrustFund/FundHistory.aspx?Action=&FundID={shareID}"
			url = url.format(shareID=item['ShareID'], companyID=item['CompanyID'], period=HISTORY_PERIOD)
			self.log(url)
			return Request(url, callback=self.get_history_data, meta={'item': item})

	def get_header(self, item):
		header = []
		if item['category'] == category_comp:
			header.append(('date_', get_date))
			header.append(('adjusted_closing_price', to_float))
			header.append(('closing_price', to_float))
			header.append(('change_', to_float))
			header.append(('opening_price', to_float))
			header.append(('base_price', to_float))
			header.append(('high', to_float))
			header.append(('low', to_float))
			header.append(('capital_listed_for_trading', to_int))
			header.append(('market_cap', to_int))
			header.append(('turnover', to_int))
			header.append(('volume', to_int))
			header.append(('trans', to_int))
		if item['category'] == category_bond:
			header.append(('date_', get_date))
			header.append(('adjusted_closing_price', to_float))
			header.append(('closing_price', to_float))
			header.append(('change_', to_float))
			header.append(('gross_yield_to_maturity', to_float))
			header.append(('opening_price', to_float))
			header.append(('base_price', to_float))
			header.append(('high', to_float))
			header.append(('low', to_float))
			header.append(('capital_listed_for_trading', to_int))
			header.append(('market_cap', to_int))
			header.append(('turnover', to_int))
			header.append(('volume', to_int))
			header.append(('trans', to_int))
		if item['category'] == category_fund:
			header.append(('date_', get_date))
			header.append(('purchase_price', to_float))
			header.append(('redemption_price', to_float))
			header.append(('change_', to_float))
			header.append(('sales_load', to_float))
			header.append(('management_fee', to_float))
			header.append(('asset_value', to_int))
			header.append(('trustee_fee', to_float))
		return header

	def get_base_url(self, hxs):
		base_url = hxs.select('//base/@href')
		o = urlparse(base_url)
		o.params=''
		o.query=''
		o.fragment=''
		res = urlparse.urlunparse(o)
		self.log2("get_base_url: " + res)
		return res

	def get_history_data(self, response):
		self.log2("get_history_data: " + response.url)
		#self.log2("get_history_data " + response.body)
		item = response.request.meta['item'] 
		hxs = HtmlXPathSelector(response)
		viewstate = hxs.select('//input[@name="__VIEWSTATE"]/@value').extract()[0]
		fd = {
			'__VIEWSTATE':viewstate,
			'HistoryData1$hiddenID':'0',
			'HistoryData1$rbFrequency':'rbFrequency1',
			'HistoryData1$RBCoordinatedList': 'AdjustmentRate',
		}
		fd['HistoryData1$rbPeriod'] = "rbPeriod{period}".format(period=HISTORY_PERIOD)
		for i in range(20):
			name = "HistoryData1$CBDailyDFiledsList${index}".format(index=i)
			fd[name] = 'on'
		base_url = self.get_base_url(hxs)
		req = FormRequest.from_response(response, url=base_url, formdata=fd, formname='Form1', callback=self.parse_history_data, meta={'item': item})
		self.log( req )
		return req

	def parse_history_data(self, response):
		self.log2("parse_history_data: " + response.url)
		#self.log2("parse_history_data " + response.body)
		
		item = response.request.meta['item']

#		filename = "html/" + item['symbol'] + ".html"
#		open(filename, 'wb').write(response.body)
			
		if USE_CSV is True:
#			return self.get_csv(item)
			yield self.get_csv(item)
		else:
			#open(item['symbol'] + '.html', 'ab').write(response.body)
			
			self.log2('Processing history for ' + item['symbol'])
			hxs = HtmlXPathSelector(response)
	
			header = self.get_header(item)
			#columns = hxs.select('//table[@id="HistoryData1_gridHistoryData_DataGrid1"]/tr[@class="gridHeader"]/td[@class="titleGridReg"]//text()')
			#for col in columns:
			#	header.append(col.extract())
			
#			items = []
			table = hxs.select('//table[@id="HistoryData1_gridHistoryData_DataGrid1"]')
			rows = table.select('tr')#[@class != "gridHeader"]')
			row_index = 0
			for row in rows:
				columns = row.select('td')
				if len(header) <= len(columns) and row_index > 0:
					item2 = TaseItem(item)
					col_index = 0
					for column in columns:
						if col_index < len(header):
							name = header[col_index][0]
							func = header[col_index][1]
							s = column.select('.//text()').extract()
							item2[name] = func(s)
						col_index += 1
					yield item2
#					items.append(item2)
				row_index += 1
#			msg = "Processed {0} history items for {1}".format(len(items), item['symbol'])
#			self.log2(msg)
#			return items

	def get_csv(self, item):
		self.log2("get_csv: " + item['symbol'])
		url = "http://www.tase.co.il/TASE/Pages/Export.aspx?tbl=0&Columns=AddColColumnsHistory&Titles=AddColTitlesHistory&sn=dsHistory&enumTblType=GridHistorydaily&ExportType=3"
		return Request(url, callback=self.save_csv, meta={'item': item})

	def save_csv(self, response):
		self.log2("save_csv: " + response.url)
		item = response.request.meta['item'] 
		if len(response.body) > 0:
			filename = "csv/" + item['symbol'] + ".csv"
			open(filename, 'wb').write(response.body)
			return self.parse_csv(filename, item)
		else:
			self.log2('failed parse_history: %s' % response.url)
			return item

	def parse_csv(self, filename, item):
		self.log2("parse_csv: " + filename)
		items = []
		ifile  = open(filename, "r")
		reader = csv.reader(ifile)
		rownum = 0
		header = self.get_header(item)
		for row in reader:
			if rownum > 3 and len(row) >= len(header):
				column_index = 0
				for column in header:
					item2 = TaseItem(item)
					name = column[0]
					func = column[1]
					item2[name] = func(row[column_index])
					column_index += 1
				items.append(item2)
			rownum += 1
		ifile.close()
		if rownum < 5:
			self.log2("Failed reading %s" % filename)
			return item
		return items



	def log2(self, msg):
		self.log(msg)
		open("0log.txt", 'ab').write(msg + '\n')

def unescape(s):
	s = get_string(s)
	s = s.replace("&lt;", "<")
	s = s.replace("&gt;", ">")
	# this has to be last:
	s = s.replace("&amp;", "AND")
	s = s.replace(" AND ", " & ")
	s = s.replace(u'\xa0', u'')
	return s

def normalize_number(s):
	s = get_string(s)
	s = s.replace(u'\xa0', u'')
	s = s.replace("\"", "")
	s = s.replace("%", "")
	s = s.replace(",", "")
	s = s.replace(" ", "")
	return s

def to_float(s):
	s = get_string(s)
	s = normalize_number(s)
	try:
		return float(s)
	except ValueError:
		return 0

def to_int(s):
	s = get_string(s)
	s = normalize_number(s)
	try:
		return float(s)
	except ValueError:
		return 0

def get_text(hxs, path):
	l = hxs.select(path).extract()
	return get_string(l)

def get_date(s):
	s = get_string(s)
	regex = "([0]?[1-9]|[1|2][0-9]|[3][0|1])[./-]([0]?[1-9]|[1][0-2])[./-]([0-9]{4}|[0-9]{2})"
	m = re.search(regex, s)
	d = datetime.date(int(m.group(3)), int(m.group(2)), int(m.group(1)))
	#p = re.compile(regex)
	#m = p.findall(s)
	#d = datetime.date(int(m[0][2]), int(m[0][1]), int(m[0][0]))
	return d.isoformat()

def is_sequence(arg):
	return (not hasattr(arg, "strip") and
            hasattr(arg, "__getitem__") or
            hasattr(arg, "__iter__"))

def get_string(s):
	if is_sequence(s):
		if len(s) > 0:
			return s[0]
		else:
			return ''
	else:
		return s






#import types
#assert isinstance(x, types.ListType)
#assert not isinstance(x, types.StringTypes)

#isinstance(var, (list, tuple))

