import re
import csv
import time
import datetime
from datetime import date

from scrapy.contrib.spiders import CrawlSpider, Rule
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor

from scrapy.selector import Selector
from scrapy.http import Request
from scrapy.http import FormRequest
from scrapy.conf import settings
#from scrapy.shell import inspect_response
from scrapy import log
from tase.items import WeatherItem

import urllib
from urlparse import urlparse
from urlparse import parse_qs
#from urlparse import urljoin

import tase.common

class WeatherSpider(CrawlSpider):
	month_dict = {"Jan":1,"Feb":2,"Mar":3,"Apr":4, "May":5, "Jun":6, "Jul":7,"Aug":8,"Sep":9,"Oct":10,"Nov":11,"Dec":12}
	name = 'weather'
	allowed_domains = ['wunderground.com']
	start_urls = []

	rules = (
		Rule(SgmlLinkExtractor(allow=('CustomHistory\.html',)), callback='parse_year'),
	)

	def __init__(self):
		super(WeatherSpider,self).__init__()
		self.start_urls = self.get_start_urls()
		
	def get_start_urls(self):
		year = str(date.today().year)
		yield 'http://www.wunderground.com/history/airport/LLBG/'+year+'/1/1/CustomHistory.html?dayend=31&monthend=12&yearend='+year+'&req_city=NA&req_state=NA&req_statename=NA&MR=1'
	
#	def __init__(self):
#		self.headers = get_headers() 

	def parse_year(self, response):
		log.msg("parse_year", level=log.WARNING)
#		self.log2("parse_year: " + response.url)
		year = 0
		month = ""
		sel = Selector(response)
		tables = sel.xpath("//div[@id='observations_details']/table[@id='obsTable']/tbody")
		for table in tables:
			row = table.xpath("tr")
			clas = tase.common.get_text(row, 'td/@class')
			if len(clas) > 0:
				year = tase.common.to_int(tase.common.get_text(table, "preceding-sibling::thead[1]/tr/th[1]/text()"))
				month = tase.common.get_text(table, "tr/td[1]/text()")
			else:
				cols = row.xpath("td")
				day = tase.common.to_int(tase.common.get_text(cols[0], 'a/text()'))
#				log.msg(str(year) + " " + month + " " + str(day), level=log.WARNING)
				item = WeatherItem()
				item["date_"] = datetime.date(year, self.month_dict[month], day)
				item["temp_high"] = tase.common.to_int(tase.common.get_text(cols[1], 'span/text()'))
				item["temp_avg"] = tase.common.to_int(tase.common.get_text(cols[2], 'span/text()'))
				item["temp_low"] = tase.common.to_int(tase.common.get_text(cols[3], 'span/text()'))
				item["dew_high"] = tase.common.to_int(tase.common.get_text(cols[4], 'span/text()'))
				item["dew_avg"] = tase.common.to_int(tase.common.get_text(cols[5], 'span/text()'))
				item["dew_low"] = tase.common.to_int(tase.common.get_text(cols[6], 'span/text()'))
				item["hum_high"] = tase.common.to_int(tase.common.get_text(cols[7], 'span/text()'))
				item["hum_avg"] = tase.common.to_int(tase.common.get_text(cols[8], 'span/text()'))
				item["hum_low"] = tase.common.to_int(tase.common.get_text(cols[9], 'span/text()'))
				item["pres_high"] = tase.common.to_int(tase.common.get_text(cols[10], 'span/text()'))
				item["pres_avg"] = tase.common.to_int(tase.common.get_text(cols[11], 'span/text()'))
				item["pres_low"] = tase.common.to_int(tase.common.get_text(cols[12], 'span/text()'))
				item["vis_high"] = tase.common.to_int(tase.common.get_text(cols[13], 'span/text()'))
				item["vis_avg"] = tase.common.to_int(tase.common.get_text(cols[14], 'span/text()'))
				item["vis_low"] = tase.common.to_int(tase.common.get_text(cols[15], 'span/text()'))
				item["wind_high"] = tase.common.to_int(tase.common.get_text(cols[16], 'span/text()'))
				item["wind_avg"] = tase.common.to_int(tase.common.get_text(cols[17], 'span/text()'))
				item["wind_low"] = tase.common.to_int(tase.common.get_text(cols[18], 'span/text()'))
				item["precip"] = tase.common.to_float(tase.common.get_text(cols[19], 'span/text()'))
				item["events"] = tase.common.get_text(cols[20], 'span/text()')
				yield item


