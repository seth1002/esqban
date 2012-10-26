import re
import urllib

from scrapy.contrib.spiders import CrawlSpider, Rule
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor

from scrapy.selector import HtmlXPathSelector
from scrapy.http import Request
from scrapy.http import FormRequest
from scrapy.conf import settings
#from scrapy.shell import inspect_response

from urlparse import urlparse, urlunparse

from tase.items import MarketItem
import tase.common

HISTORY_PERIOD = settings.getint('HISTORY_PERIOD', 2)


class MarketSpider(CrawlSpider):
	name = 'markets'
	allowed_domains = ['tase.co.il']
	start_urls = ['http://www.tase.co.il/TASEEng']

	rules = (
		Rule(SgmlLinkExtractor(allow=['homepage.htm']), callback='parse_markets'),
	)

	header = (
		('date_', tase.common.get_date),
		('base_price', tase.common.to_float),
		('opening_price', tase.common.to_float),
		('closing_price', tase.common.to_float),
		('high', tase.common.to_float),
		('low', tase.common.to_float),
		('market_cap', tase.common.to_int)
	)

	def parse_markets(self, response):
		self.log("parse_markets: " + response.url)
		hxs = HtmlXPathSelector(response)
		links = hxs.select("//a[@class='IndexesLink1']/@href")
		for link in links:
			url = urllib.unquote(link.extract())
			full_url = 'http://www.' + self.allowed_domains[0] + url
			yield Request(full_url, callback=self.parse_market)

	def parse_market(self, response):
		self.log("parse_market: " + response.url)
		hxs = HtmlXPathSelector(response)
		market = MarketItem()
		rows = hxs.select('//table[@id="gridShareByIndex_DataGrid1"]/tr')
		market['symbols'] = []
		row_index = 0
		for row in rows:
			if row_index > 0:
				col = tase.common.get_text(row, 'td[2]/text()')
				market['symbols'].append(col)
			row_index += 1
		links = hxs.select("//input[@class='RegularButton' and @value='Historical Prices']/@onclick")
		for link in links:
			m = re.search("javascript:gotoUrl\('(.*?)'", link.extract())
			if m:
				url = urllib.unquote(m.group(1))
				full_url = 'http://www.' + self.allowed_domains[0] + url
				yield Request(full_url, callback=self.get_market_history_data, meta={'market': market})

	def get_base_url(self, hxs):
		base_url = hxs.select('//base/@href')[0].extract()
		o = urlparse(base_url)
		res = urlunparse(o)
#		log.msg("get_base_url: " + res, level=log.WARNING)
		return res

	def get_market_history_data(self, response):
#		self.log("get_history_data: " + response.url)
		#inspect_response(response)
		hxs = HtmlXPathSelector(response)
		market = response.request.meta['market']
		viewstate = hxs.select('//input[@name="__VIEWSTATE"]/@value').extract()[0]
		fd = {
			'__VIEWSTATE':viewstate,
			'HistoryData1$hiddenID':'0',
			'HistoryData1$rbFrequency':'rbFrequency1',
			'HistoryData1$RBCoordinatedList': 'AdjustmentRate',
		}
		fd['HistoryData1$rbPeriod'] = "rbPeriod{period}".format(period=HISTORY_PERIOD)
		for i in range(3):
			name = "HistoryData1$CBDailyDFiledsList${index}".format(index=i)
			fd[name] = 'on'
		for i in range(2):
			name = "HistoryData1$CBInnerDFiledsList${index}".format(index=i)
			fd[name] = 'on'
		base_url = self.get_base_url(hxs)
		response = response.replace(url=base_url)
		req = FormRequest.from_response(response, formdata=fd, formname='Form1', callback=self.parse_history_data, meta={'market': market})
		self.log( req )
		return req;

	def parse_history_data(self, response):
		self.log("parse_history_data: " + response.url)
		orig_market = response.request.meta['market']
		hxs = HtmlXPathSelector(response)
		symbol = tase.common.get_text(hxs, "//td[@class='BigTitleInner']/text()")
		symbol = tase.common.unescape(symbol);
		table = hxs.select('//table[@id="HistoryData1_gridHistoryData_DataGrid1"]')
		rows = table.select('tr')#[@class != "gridHeader"]')
		row_index = 0
		for row in rows:
			columns = row.select('td')
			if len(self.header) <= len(columns) and row_index > 0:
				item = MarketItem(orig_market)
				item['name'] = symbol
				col_index = 0
				for column in columns:
					if col_index < len(self.header):
						name = self.header[col_index][0]
						func = self.header[col_index][1]
						s = column.select('.//text()').extract()
						item[name] = func(s)
					col_index += 1
				yield item
			row_index += 1
