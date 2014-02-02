import re
import urllib

from scrapy.contrib.spiders import CrawlSpider, Rule
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor

from scrapy.selector import Selector
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
	start_urls = ['http://www.tase.co.il']

	rules = (
		Rule(SgmlLinkExtractor(allow=['Homepage\.aspx']), callback='parse_markets'),
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
#		self.log("parse_markets: " + response.url)
		sel = Selector(response)
		links = sel.xpath("//a[@class='IndexesLink1']/@href")
		for link in links:
			url = urllib.unquote(link.extract())
			full_url = 'http://www.' + self.allowed_domains[0] + url
			yield Request(full_url, callback=self.parse_market)

	def parse_market(self, response):
		self.log("parse_market: " + response.url)
		sel = Selector(response)
		market = MarketItem()
		rows = sel.xpath('//table[@id="ctl00_SPWebPartManager1_g_24df47e6_1ef9_484c_8cef_5524d650e226_ctl00_gridShareByIndex_DataGrid1"]/tr')
		market['symbols'] = []
		row_index = 0
		for row in rows:
			if row_index > 0:
				col = tase.common.get_text(row, 'td[2]/text()')
				market['symbols'].append(col)
			row_index += 1
		links = sel.xpath("//input[@class='RegularButton' and @value='Historical Prices']/@onclick")
		for link in links:
			m = re.search("javascript:gotoUrl\('(.*?)'", link.extract())
			if m:
				url = urllib.unquote(m.group(1))
				full_url = 'http://www.' + self.allowed_domains[0] + url
				yield Request(full_url, callback=self.get_market_history_data, meta={'market': market})

#	def get_base_url(self, sel):
#		base_url = sel.xpath('//base/@href')[0].extract()
#		o = urlparse(base_url)
#		res = urlunparse(o)
##		log.msg("get_base_url: " + res, level=log.WARNING)
#		return res

	def get_market_history_data(self, response):
#		self.log("get_history_data: " + response.url)
		#inspect_response(response)
		sel = Selector(response)
		market = response.request.meta['market']
		viewstate = sel.xpath('//input[@name="__VIEWSTATE"]/@value').extract()[0]
		fd = {
			'__VIEWSTATE':viewstate,
			'ctl00$SPWebPartManager1$g_a97a61ce_3baf_4f18_b714_6cbcc326fa71$ctl00$HistoryData1$hiddenID':'0',
			'ctl00$SPWebPartManager1$g_a97a61ce_3baf_4f18_b714_6cbcc326fa71$ctl00$HistoryData1$rbPeriod' : 'rbPeriod2',
			'ctl00$SPWebPartManager1$g_a97a61ce_3baf_4f18_b714_6cbcc326fa71$ctl00$HistoryData1$RBExtraTypeList' : '2',
			'ctl00$SPWebPartManager1$g_a97a61ce_3baf_4f18_b714_6cbcc326fa71$ctl00$HistoryData1$RBExtraPeriodList' : 'lastMONTH',
			'ctl00$SPWebPartManager1$g_a97a61ce_3baf_4f18_b714_6cbcc326fa71$ctl00$HistoryData1$rbPeriodOTC' : 'rbPeriodOTC2',
			'ctl00$SPWebPartManager1$g_b2f63986_2b4a_438d_b1b1_fb08c9e1c862$ctl00$HistoryData1$rbFrequency':'rbFrequency1',
			#'ctl00$SPWebPartManager1$g_b2f63986_2b4a_438d_b1b1_fb08c9e1c862$ctl00$HistoryData1$RBCoordinatedList': 'AdjustmentRate',
			'ctl00$SPWebPartManager1$g_a97a61ce_3baf_4f18_b714_6cbcc326fa71$ctl00$HistoryData1$rbExtraFrequency' : 'rbExtraMonth',
		}
		fd['ctl00$SPWebPartManager1$g_b2f63986_2b4a_438d_b1b1_fb08c9e1c862$ctl00$HistoryData1$rbPeriod'] = "rbPeriod{period}".format(period=HISTORY_PERIOD)
		for i in range(6):
			name = "ctl00$SPWebPartManager1$g_b2f63986_2b4a_438d_b1b1_fb08c9e1c862$ctl00$HistoryData1$CBDailyDFiledsList${index}".format(index=i)
			fd[name] = 'on'
		for i in range(2):
			name = "ctl00$SPWebPartManager1$g_b2f63986_2b4a_438d_b1b1_fb08c9e1c862$ctl00$HistoryData1$CBInnerDFiledsList${index}".format(index=i)
			fd[name] = 'on'
		#base_url = self.get_base_url(sel)
		#response = response.replace(url=base_url)
		req = FormRequest.from_response(response, formdata=fd, formname='Form1', callback=self.parse_history_data, meta={'market': market})
		self.log( req )
		return req;

	def parse_history_data(self, response):
		self.log("parse_history_data: " + response.url)
		orig_market = response.request.meta['market']
		sel = Selector(response)
		symbol = tase.common.get_text(sel, "//td[@class='BigTitleInner']/text()")
		symbol = tase.common.unescape(symbol);
		table = sel.xpath('//table[@id="HistoryData1_gridHistoryData_DataGrid1"]')
		rows = table.xpath('tr')#[@class != "gridHeader"]')
		row_index = 0
		for row in rows:
			columns = row.xpath('td')
			if len(self.header) <= len(columns) and row_index > 0:
				item = MarketItem(orig_market)
				item['name'] = symbol
				col_index = 0
				for column in columns:
					if col_index < len(self.header):
						name = self.header[col_index][0]
						func = self.header[col_index][1]
						s = column.xpath('.//text()').extract()
						item[name] = func(s)
					col_index += 1
				yield item
			row_index += 1
