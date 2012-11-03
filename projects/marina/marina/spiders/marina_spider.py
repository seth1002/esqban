from scrapy.spider import BaseSpider

class MarinaSpider(BaseSpider):
    name = "marinas"
    allowed_domains = ["marinas.com"]
    start_urls = [
        "http://marinas.com/browse/marina/",
    ]

    def parse(self, response):
        filename = response.url.split("/")[-2]
        open(filename, 'wb').write(response.body)
        #hxs.select('//a[contains(@href, "http://marinas.com/browse/marina/")]')
        #hxs.select('//a[contains(./img/@src, "flags")]')
		#hxs.select('//a[./img/@src[contains(., "flags")]]')
		countries = hxs.select('//a[img/@src[contains(., "flags")]]/@href')
		marinas = hxs.select('//a[@href[contains(., "http://marinas.com/view/marina/")]]/@href')
		latitude = hxs.select('//tr/td[contains(., "Latitude")]/following-sibling::td/text()')
		hxs.select('//tr/td[contains(., "Longitude")]/following-sibling::td/text()')
		site = hxs.select('//tr/td[contains(., "Web")]/following-sibling::td/a/text()')[0].extract()
