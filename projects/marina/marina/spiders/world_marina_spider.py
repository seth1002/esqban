from scrapy.spider import BaseSpider
from scrapy.http import Request
from scrapy.selector import HtmlXPathSelector
import urlparse

from marina.items import MarinaItem

import re
#import geolucidate.parser


def ConvertDMSToDD(minutes, seconds, direction):
	dd = minutes/60 + seconds/(60*60);

	if direction == "S" or direction == "W":
		dd = dd * -1;
	# Don't do anything for N or E
	return dd

def ParseDMS(strinput):
	#S 22 58.123 W 021 53.2
	#([SN])\s(\d+)\s(\d+(?:\.\d+)?)\s([EW])\s(\d+)\s(\d+(?:\.\d*)?)

	#22 58.123 S 021 53.2 W
	#(\d+)\s(\d+(?:\.\d+)?)\s([SN])\s(\d+)\s(\d+(?:\.\d*)?)\s([EW])

	regex_str=r"(\d+).*\s(\d+(?:\.\d+)?)\'\s*([SN])\s*(\d+).*\s(\d+(?:\.\d+)?)\'\s*([EW])"

	#parts = strinput.split('/[^\d\w]+/')
	#lat = ConvertDMSToDD(parts[0], parts[1], parts[2], parts[3])
	#lng = ConvertDMSToDD(parts[4], parts[5], parts[6], parts[7])
	prog = re.compile(regex_str)
	result = prog.match(strinput)
	lat = ConvertDMSToDD(result.group(1), result.group(2), result.group(3))
	lng = ConvertDMSToDD(result.group(4), result.group(5), result.group(6))
	return lat, lng

class MarinaSpider(BaseSpider):
	name = "worldmarinaguide"
	allowed_domains = ["worldmarineguide.com"]
	start_urls = [
		"http://www.worldmarineguide.com/country/list",
	]

	def parse_marina(self, response):
		hxs = HtmlXPathSelector(response)

		tmp = hxs.select("//div[@id='splash']/h2/b/text()")
		if len(tmp) == 0:
			return

		marina = MarinaItem()

		latlong = tmp[0].extract().strip().replace(u'\u02da', "deg")
		lat, lng = ParseDMS(latlong)
		marina["latitude"] = lat
		marina["longitude"] = lng

		tmp = hxs.select("//h2[@class='boxtop']/text()")
		if len(tmp) > 0:
			marina["name"] = tmp[0].extract().strip()

		#"//div[@id='leftcol']/div[@class='boxthin']/h2[@class='boxtop']/text()"

		tmp = hxs.select("//div[@class='boxthin']/following-sibling::td/text()")
		if len(tmp) > 0:
			marina["name"] = tmp[0].extract().strip()

		return marina

	def parse_country(self, response):
		hxs = HtmlXPathSelector(response)
		marinas = hxs.select('//small/a/@href')
		for marina in marinas:
			url = urlparse.urljoin(response.url, marina.extract())
			yield Request(url, callback=self.parse_marina)

	def parse(self, response):
		hxs = HtmlXPathSelector(response)
		#hxs.select('//a[contains(@href, "http://marinas.com/browse/marina/")]')
		#hxs.select('//a[contains(./img/@src, "flags")]')
		#hxs.select('//a[./img/@src[contains(., "flags")]]')
		countries = hxs.select('//li/a/@href')
		for country in countries:
			url = urlparse.urljoin(response.url, country.extract())
			yield Request(url, callback=self.parse_country)
