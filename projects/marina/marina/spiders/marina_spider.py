from scrapy.spider import BaseSpider
from scrapy.http import Request
from scrapy.selector import HtmlXPathSelector

from marina.items import MarinaItem

class MarinaSpider(BaseSpider):
	name = "marinas"
	allowed_domains = ["marinas.com"]
	start_urls = [
		"http://marinas.com/browse/marina/",
	]

	def parse_marina(self, response):
		hxs = HtmlXPathSelector(response)
		marina = MarinaItem()
		tmp = hxs.select('//tr/td[contains(., "Latitude")]/following-sibling::td/text()')
		if len(tmp) > 0:
			marina["latitude"] = tmp[0].extract().strip()
		tmp = hxs.select('//tr/td[contains(., "Longitude")]/following-sibling::td/text()')
		if len(tmp) > 0:
			marina["longitude"] = tmp[0].extract().strip()
		tmp = hxs.select('//tr/td[contains(., "Web")]/following-sibling::td/a/text()')
		if len(tmp) > 0:
			marina["site"] = tmp[0].extract().strip()
		return marina

	def parse_country(self, response):
		hxs = HtmlXPathSelector(response)
		marinas = hxs.select('//a[@href[contains(., "http://marinas.com/view/marina/")]]/@href')
		for marina in marinas:
			yield Request(marina.extract(), callback=self.parse_marina)

	def parse(self, response):
		hxs = HtmlXPathSelector(response)
		#hxs.select('//a[contains(@href, "http://marinas.com/browse/marina/")]')
		#hxs.select('//a[contains(./img/@src, "flags")]')
		#hxs.select('//a[./img/@src[contains(., "flags")]]')
		countries = hxs.select('//a[img/@src[contains(., "flags")]]/@href')
		for country in countries:
			yield Request(country.extract(), callback=self.parse_country)
