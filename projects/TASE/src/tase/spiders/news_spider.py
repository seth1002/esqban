import re
import csv
import time
import datetime

from twisted.enterprise import adbapi
from scrapy.conf import settings

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

class NewsSpider(CrawlSpider):
	name = 'news'
	allowed_domains = ['globes.co.il']
	start_urls = []
	
	rules = (
		Rule(SgmlLinkExtractor(allow=('search\.aspx',)), callback='parse_article_list'),
		Rule(SgmlLinkExtractor(allow=('\/en\/article-',)), callback='parse_article'),
	)
	
	def __init__(self):
		super(NewsSpider,self).__init__()
		self.start_urls = get_start_urls()
		rules = (
			Rule(SgmlLinkExtractor(allow=('CustomHistory\.html',)), callback='parse_year'),
		)
		
	def get_start_urls():
		dbpool = adbapi.ConnectionPool('MySQLdb',
		        host=settings['DATABASE_HOST'],
		        db=settings['DATABASE_SCHEMA'],
		        user=settings['DATABASE_USER'],
		        passwd=settings['DATABASE_PASSWORD'],
		        cursorclass=MySQLdb.cursors.DictCursor,
		        charset='utf8',
		        use_unicode=True
		    )
		conn = dbpool.connect()
		cur = conn.cursor()
		rows = dbpool.runQuery('SELECT symbol FROM companies')
		for row in rows:
			strURL = "http://www.globes.co.il/en/searchajax.aspx?page=1&searchType=all&searchQuery=%s" % row[0]
			yield strURL
	
	def parse_article_list(self, response):
		self.log(response.url)
		item = response.request.meta['item']
		sel = Selector(response)
		links = sel.xpath("//b[@class='searchElement']/a")
		for link in links:
			article = NewsArticle()
			article['symbol'] = item['symbol']
			a = link.xpath("a")
			if len(a) == 0:
				continue
			article['url'] = tase.common.get_string(a.xpath("@href").extract())
			article['headline'] = tase.common.get_string(a.xpath('text()').extract())
			p = link.xpath("p")
			if len(p) < 2:
				continue
			summary = tase.common.get_string(p[0].xpath('text()').extract())
			if len(summary) < 10:
				return
			article['summary'] = summary
			try:
				strdate = p[1].xpath("span[@class='date']/text()")[0].extract()
				article['date_'] = tase.common.get_date2(strdate)
				if PROCESS_NEWS_CONTENT:
					yield Request(article['url'], callback=self.parse_company_news_content, dont_filter=True, meta={'article': article})
				else:
					yield article
			except ValueError:
				self.log('Error reading date on page %s' % response.url)
				tase.common.log2('Error reading date on page %s' % response.url)
		if PROCESS_NEWS_HISTORY:
			a = sel.xpath("//a[@onclick='GetMoreResults()']")
			if len(a) > 0:
				href = tase.common.get_string(a[0].xpath("@href").extract())
				url = "http://www.globes.co.il/en/searchajax.aspx?page=2&searchType=all&searchQuery=%s" % item['symbol']
				yield Request(url, callback=self.parse_company_news, meta={'item': item})

	def parse_article(self, response):
		self.log(response.url)
		article = response.request.meta['article']
		sel = Selector(response)
		try:
			#content = html2text(tase.common.get_string(sel.xpath("//p[@id='content']").extract()))
			content = html2text(tase.common.get_string(sel.xpath("//div[@id='EN_main_Content']").extract()))
		except:
			content = ''
			tase.common.log2('Error reading content on page %s' % response.url)
		article['content'] = content
		return article
