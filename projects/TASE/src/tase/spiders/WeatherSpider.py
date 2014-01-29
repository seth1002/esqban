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
	start_urls = [
#		'http://www.wunderground.com/history/airport/LLBG/2005/1/1/CustomHistory.html?dayend=2&monthend=1&yearend=2006&req_city=NA&req_state=NA&req_statename=NA&MR=1',
#		'http://www.wunderground.com/history/airport/LLBG/2006/1/1/CustomHistory.html?dayend=2&monthend=1&yearend=2007&req_city=NA&req_state=NA&req_statename=NA&MR=1',
#		'http://www.wunderground.com/history/airport/LLBG/2007/1/1/CustomHistory.html?dayend=2&monthend=1&yearend=2008&req_city=NA&req_state=NA&req_statename=NA&MR=1',
#		'http://www.wunderground.com/history/airport/LLBG/2008/1/1/CustomHistory.html?dayend=2&monthend=1&yearend=2009&req_city=NA&req_state=NA&req_statename=NA&MR=1',
#		'http://www.wunderground.com/history/airport/LLBG/2009/1/1/CustomHistory.html?dayend=2&monthend=1&yearend=2010&req_city=NA&req_state=NA&req_statename=NA&MR=1',
#		'http://www.wunderground.com/history/airport/LLBG/2010/1/1/CustomHistory.html?dayend=2&monthend=1&yearend=2011&req_city=NA&req_state=NA&req_statename=NA&MR=1',
#		'http://www.wunderground.com/history/airport/LLBG/2012/1/1/CustomHistory.html?dayend=31&monthend=12&yearend=2012&req_city=NA&req_state=NA&req_statename=NA&MR=1',
		'http://www.wunderground.com/history/airport/LLBG/2013/1/1/CustomHistory.html?dayend=31&monthend=12&yearend=2013&req_city=NA&req_state=NA&req_statename=NA&MR=1',
		'http://www.wunderground.com/history/airport/LLBG/2014/1/1/CustomHistory.html?dayend=31&monthend=12&yearend=2014&req_city=NA&req_state=NA&req_statename=NA&MR=1',
	]

	rules = (
		Rule(SgmlLinkExtractor(allow=('CustomHistory\.html',)), callback='parse_year'),
	)
	
#	def __init__(self):
#		self.headers = get_headers() 

	def parse_year(self, response):
		log.msg("parse_year", level=log.WARNING)
#		self.log2("parse_year: " + response.url)
		year = 0
		month = ""
		hxs = HtmlXPathSelector(response)
		tables = hxs.select("//div[@id='observations_details']/table[@id='obsTable']/tbody")
		for table in tables:
			row = table.select("tr")
			clas = tase.common.get_text(row, 'td/@class')
			if len(clas) > 0:
				year = tase.common.to_int(tase.common.get_text(table, "preceding-sibling::thead[1]/tr/th[1]/text()"))
				month = tase.common.get_text(table, "tr/td[1]/text()")
			else:
				cols = row.select("td")
				day = tase.common.to_int(tase.common.get_text(cols[0], 'a/text()'))
#				log.msg(str(year) + " " + month + " " + str(day), level=log.WARNING)
				item = WeatherItem()
				item["date_"] = datetime.date(year, self.month_dict[month], day)
				item["temp_high"] = tase.common.to_int(tase.common.get_text(cols[1], 'text()'))
				item["temp_avg"] = tase.common.to_int(tase.common.get_text(cols[2], 'text()'))
				item["temp_low"] = tase.common.to_int(tase.common.get_text(cols[3], 'text()'))
				item["dew_high"] = tase.common.to_int(tase.common.get_text(cols[4], 'text()'))
				item["dew_avg"] = tase.common.to_int(tase.common.get_text(cols[5], 'text()'))
				item["dew_low"] = tase.common.to_int(tase.common.get_text(cols[6], 'text()'))
				item["hum_high"] = tase.common.to_int(tase.common.get_text(cols[7], 'text()'))
				item["hum_avg"] = tase.common.to_int(tase.common.get_text(cols[8], 'text()'))
				item["hum_low"] = tase.common.to_int(tase.common.get_text(cols[9], 'text()'))
				item["pres_high"] = tase.common.to_int(tase.common.get_text(cols[10], 'text()'))
				item["pres_avg"] = tase.common.to_int(tase.common.get_text(cols[11], 'text()'))
				item["pres_low"] = tase.common.to_int(tase.common.get_text(cols[12], 'text()'))
				item["vis_high"] = tase.common.to_int(tase.common.get_text(cols[13], 'text()'))
				item["vis_avg"] = tase.common.to_int(tase.common.get_text(cols[14], 'text()'))
				item["vis_low"] = tase.common.to_int(tase.common.get_text(cols[15], 'text()'))
				item["wind_high"] = tase.common.to_int(tase.common.get_text(cols[16], 'text()'))
				item["wind_avg"] = tase.common.to_int(tase.common.get_text(cols[17], 'text()'))
				item["wind_low"] = tase.common.to_int(tase.common.get_text(cols[18], 'text()'))
				item["precip"] = tase.common.to_float(tase.common.get_text(cols[19], 'text()'))
				item["events"] = tase.common.get_text(cols[20], 'text()')
				yield item


