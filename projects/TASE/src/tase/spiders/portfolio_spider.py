
from scrapy.conf import settings
import MySQLdb.cursors
from tase.spiders.stock_spider import StockSpider



class PortfolioSpider(StockSpider):
	name = 'portfolio'

	def __init__(self):
		super(PortfolioSpider,self).__init__()
		self.start_urls = self.get_start_urls()

	def get_start_urls(self):
		db = MySQLdb.connect(
		        host=settings['DATABASE_HOST'],
		        db=settings['DATABASE_SCHEMA'],
		        user=settings['DATABASE_USER'],
		        passwd=settings['DATABASE_PASSWORD'],
		        charset='utf8',
		        use_unicode=True
		    )
		cur = db.cursor()
		cur.execute('select p.symbol, c.tase_url from portfolio p inner join companies c on p.symbol=c.symbol')
		for row in cur.fetchall():
			yield row[1]

	def parse(self, response):
		return StockSpider.parse_company(self, response);


