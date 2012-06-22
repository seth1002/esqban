
from tase.spiders.stock_spider import StockSpider



class PortfolioSpider(StockSpider):
	name = 'portfolio'

	start_urls = [l.strip() for l in open('portfolio.txt').readlines()]

	def parse(self, response):
		return StockSpider.parse_company(self, response);

