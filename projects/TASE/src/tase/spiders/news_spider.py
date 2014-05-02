import re
import csv
import time
import datetime

from scrapy.conf import settings
import MySQLdb.cursors

from scrapy.contrib.spiders import CrawlSpider, Rule
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor

from scrapy.selector import Selector
from scrapy.http import Request
from scrapy.http import FormRequest
from scrapy.conf import settings
#from scrapy.shell import inspect_response
from scrapy import log
from tase.items import NewsArticle

import urllib
from urlparse import urlparse
from urlparse import parse_qs
#from urlparse import urljoin

import tase.common

PROCESS_NEWS  = settings.getbool('PROCESS_NEWS', False)
PROCESS_NEWS_HISTORY  = settings.getbool('PROCESS_NEWS_HISTORY', False)
PROCESS_NEWS_CONTENT  = settings.getbool('PROCESS_NEWS_CONTENT', True)

class NewsSpider(CrawlSpider):
	name = 'news'
	allowed_domains = ['globes.co.il']
	start_urls = []
	
	rules = (
		#Rule(SgmlLinkExtractor(allow=(r'searchajax\.aspx\?',)), callback='parse_article_list'),
		Rule(SgmlLinkExtractor(allow=('searchajax\.aspx',)), callback='parse_article_list'),
		#Rule(SgmlLinkExtractor(allow=('\/en\/article-',)), callback='parse_article'),
	)
	
	def __init__(self):
		super(NewsSpider,self).__init__()
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
		cur.execute('SELECT symbol FROM companies WHERE category=%s', settings['CATEGORY_COMP'])
		#cur.execute('SELECT symbol FROM portfolio')
		for row in cur.fetchall():
			strURL = "http://www.globes.co.il/en/searchajax.aspx?page=1&searchType=all&searchQuery=%s" % row[0]
			yield strURL
	
	def parse(self, response):
		#self.log("parse_article_list: " + response.url)
		#inspect_response(response, self)
		#self.log(response.url)
		query = parse_qs(urlparse(response.url)[4]) # query
		symbol = query['searchQuery'][0]
		self.log("symbol: " + symbol)
		sel = Selector(response)
		links = sel.xpath("//article[@class='search']")
		for link in links:
			article = NewsArticle()
			article['symbol'] = symbol
			article['url'] = tase.common.get_string(link.xpath("b/a/@href").extract())
			article['headline'] = tase.common.get_string(link.xpath('b/a/text()').extract())
			article['summary'] = article['headline']
			article['content'] = ""
			try:
				strdate = link.xpath("span[@class='date']/text()")[0].extract()
				article['date_'] = tase.common.get_date3(strdate)
				if PROCESS_NEWS_CONTENT:
					yield Request(article['url'], callback=self.parse_company_news_content, dont_filter=True, meta={'article': article})
				else:
					yield article
			except ValueError:
				self.log('Error reading date on page %s' % response.url)
				self.log(link.xpath("span[@class='date']/text()")[0].extract())
				tase.common.log2('Error reading date on page %s' % response.url)
		if PROCESS_NEWS_HISTORY:
			a = sel.xpath("//a[@onclick='GetMoreResults()']")
			if len(a) > 0:
				href = tase.common.get_string(a[0].xpath("@href").extract())
				url = "http://www.globes.co.il/en/searchajax.aspx?page=2&searchType=all&searchQuery=%s" % item['symbol']
				yield Request(url, callback=self.parse_company_news, meta={'article': article})

	def parse_article(self, response):
		self.log(response.url)
		article = NewsArticle()
		article['url'] = tase.common.get_string(a.xpath("@href").extract())
		article['symbol'] = response.response.headers.get('searchQuery', None)
		sel = Selector(response)
		try:
			#content = html2text(tase.common.get_string(sel.xpath("//p[@id='content']").extract()))
			content = html2text(tase.common.get_string(sel.xpath("//div[@id='EN_main_Content']").extract()))
		except:
			content = ''
			tase.common.log2('Error reading content on page %s' % response.url)
		article['content'] = content
		return article
