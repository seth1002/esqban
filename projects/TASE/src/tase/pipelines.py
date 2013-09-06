from scrapy import log
#from scrapy.exceptions import DropItem
from twisted.enterprise import adbapi
from scrapy.conf import settings
from scrapy.contrib.pipeline.images import ImagesPipeline
from scrapy.exceptions import DropItem
from scrapy.http import Request

import os
import time
import datetime

import MySQLdb.cursors

import pagerank

import getterms
from tase.items import TaseItem
from tase.items import MarketItem
from tase.items import NewsArticle
from tase.items import FinancialStatement
from tase.items import WeatherItem

#import hashlib
import zlib
#import urllib

import tase.common


category_comp = settings.get('CATEGORY_COMP')
category_bond = settings.get('CATEGORY_BOND')
category_fund = settings.get('CATEGORY_FUND')

PROCESS_HISTORY = settings.getbool('PROCESS_HISTORY', False)

global_time = time.time()
global_date = datetime.datetime.now()


def clean_name(s):
	#s = s.replace("\\r", "")
	#s = s.replace("\\n", "")
	#return s.strip()
	return s.lstrip('\r\n').strip()

class CleanupPipeline(object):

    def process_item(self, item, spider):
        if not isinstance(item, TaseItem):
            return item
        item['name'] = clean_name( item['name'] )
        item['symbol'] = clean_name( item['symbol'] )
        item['CompanyID'] = clean_name( item['CompanyID'] )
        item['ShareID'] = clean_name( item['ShareID'] )
        return item


class BaseDB(object):

    def __init__(self):
        self.dbpool = adbapi.ConnectionPool('MySQLdb',
                host=settings['DATABASE_HOST'],
                db=settings['DATABASE_SCHEMA'],
                user=settings['DATABASE_USER'],
                passwd=settings['DATABASE_PASSWORD'],
                cursorclass=MySQLdb.cursors.DictCursor,
                charset='utf8',
                use_unicode=True
            )
    
    def open_spider(self, spider):
        self.conn = self.dbpool.connect()
        self.cur = self.conn.cursor()


class MarketDataPipeline(BaseDB):

    def process_item(self, item, spider):
        # run db query in thread pool
        query = self.dbpool.runInteraction(self._conditional_insert, item)
        query.addErrback(self.handle_error)
        return item

    def handle_error(self, e):
        log.err(e)

    def _conditional_insert(self, tx, item):
        if not isinstance(item, MarketItem):
            return item
        tx.execute("select * from market_data where name = %s", item['name'])
        result = tx.fetchone()
        if result is None:
            try:
                tx.execute(\
                    "insert into markets (sessionid, name) "
                    "values (%s, %s)", ( global_time, item['name'] )
                )
            except MySQLdb.IntegrityError, e:
                #print 'SQL integrity error: %s' % e
                log.msg('SQL integrity error: %s' % e)
        market_name = item['name']
        for symbol in item['symbols']:
            try:
                tx.execute(\
                    "insert into market_shares (sessionid, market_name, symbol) "
                    "values (%s, %s, %s)", ( global_time, market_name, symbol )
                )
            except MySQLdb.IntegrityError, e:
                #print 'SQL integrity error: %s' % e
                log.msg('SQL integrity error: %s' % e)
        try:
            tx.execute(\
                "insert into market_data (sessionid, date_, name, base_price, opening_price, closing_price, high, low, market_cap) "
                "values (%s, %s, %s, %s, %s, %s, %s, %s, %s)",
                (
                global_time,
                item['date_'],
                item['name'],
                item['base_price'],
                item['opening_price'],
                item['closing_price'],
                item['high'],
                item['low'],
                item['market_cap']
                )
            )
        except MySQLdb.IntegrityError, e:
            #print 'SQL integrity error: %s' % e
            log.msg('SQL integrity error: %s' % e)


class PageRankPipeline(BaseDB):

    def process_item(self, item, spider):
        if not isinstance(item, TaseItem):
            return item
        if item['url']:
            rank = pagerank.get_pagerank(item['url'])
            try:
                pagerank = float(rank)

                self.cur.execute(\
                    "insert into pagerank (sessionid, date_, symbol, pagerank) "
                    "values (%s, %s, %s, %s)",
                    (
                    global_time,
                    global_date.isoformat(),
                    item['symbol'],
                    pagerank
                    )
                )
            except ValueError:
                pass
            except MySQLdb.IntegrityError, e:
                #print 'SQL integrity error: %s' % e
                log.msg('SQL integrity error: %s' % e)
        return item


class SessionPipeline(BaseDB):
    
    def open_spider(self, spider):
        BaseDB.open_spider(self, spider)
        try:
            self.cur.execute("insert into sessions (sessionid, date_) values (%s, %s);",
                    ( global_time, global_date.isoformat() )
            )
            self.conn.commit()
        except MySQLdb.IntegrityError, e:
            #print 'SQL integrity error: %s' % e
            log.msg('SQL integrity error: %s' % e)

    def process_item(self, item, spider):
        return item


class MultiplierPipeline(object):

    item_header = {
        'closing_price' : 1.0/100.0,
        'adjusted_closing_price' : 1.0/100.0,
        'opening_price' : 1.0/100.0,
        'base_price' : 1.0/100.0,
        'high' : 1.0/100.0,
        'low' : 1.0/100.0,
        'market_cap' : 1000,
        }
    
    financial_statements_header = {
        'total_assets' : 1000,
        'current_assets' : 1000,
        'non_current_assets' : 1000,
        'shareholders_equity' : 1000,
        'of_which_minority_interest' : 1000,
        'current_liabilities' : 1000,
        'long_term_liabilities' : 1000,
        'revenues' : 1000,
        'gross_profit' : 1000,
        'operating_income' : 1000,
        'income_before_tax' : 1000,
        'net_income' : 1000,
        'income_atributable_to_shareholders' : 1000,
        #'earnings_per_share' : 1000,
        'dividends' : 1000,
        'net_cash_flow_generated_by_operating_activities' : 1000
        }
    
    def process_object(self, obj):
        header = []
        if isinstance(obj, TaseItem):
            header = self.item_header
        elif isinstance(obj, FinancialStatement):
            header = self.financial_statements_header
        for key, val in header.iteritems():
            try:
                if obj.get(key):
                    obj[key] *= val
            except:
                print('Error processing %s' % key)

    def process_item(self, item, spider):
        if not isinstance(item, TaseItem):
            return item
        if item['category'] == category_fund:
            return item
        if item['category'] == category_comp:
            # Financial statements
            if not item.get('financial_statements') is None:
                for fs in item['financial_statements']:
                    self.process_object(fs)
        #elif item['category'] == category_bond:
        self.process_object(item)
        return item

class KeywordPipeline(BaseDB):

    def process_item(self, item, spider):
        if not isinstance(item, TaseItem):
            return item
        if item['url']:
            url = item['url']
            for t in getterms.get_terms(url):
                try:
                    self.cur.execute("select id from keywords where keyword = %s", t[0])
                    result = self.cur.fetchone()
                    if result is None:
                        self.cur.execute(\
                            "insert into keywords (sessionid, keyword) "
                            "values (%s, %s)", ( global_time, t[0] )
                        )
                        keyword_id = self.conn.insert_id()
                        #keyword_id = cur.lastrowid
                    else:
                        keyword_id = result['id']
     
                    self.cur.execute(\
                        "insert into company_keywords (sessionid, symbol, keyword_id, x, y) "
                        "values (%s, %s, %s, %s, %s)",
                        ( global_time, item['symbol'], keyword_id, t[1], t[2] )
                    )
                except MySQLdb.IntegrityError, e:
                    #print 'SQL integrity error: %s' % e
                    log.msg('SQL integrity error: %s' % e)
            self.conn.commit()


class FinancialStatementsPipeline(BaseDB):
    
    def process_item(self, item, spider):
        if not isinstance(item, TaseItem):
            return item
        if not item.get('financial_statements'):
            return item
        # run db query in thread pool
        query = self.dbpool.runInteraction(self._conditional_insert, item)
        query.addErrback(self.handle_error)
        return item

    def _conditional_insert(self, tx, item):
        for fs in item['financial_statements']:
            keys = fs.keys()
            items = fs.items()
            values = [itm[1] for itm in items]
            values.insert(0, item['symbol'])
            values.insert(0, global_time)
            names = ','.join(map(str, keys))
            vals = ", ".join(["%s"]*(len(items)+2))
            #tmp = " "
            #if len(keys) > 0:
            #    tmp = ", "
            sql_query = "insert into financial_statements (sessionid, symbol, " + names + ") values (" + vals + ")"
            #log.err('Full SQL statement: %s' % sql_query)
            try:
                tx.execute(sql_query, values)
            except MySQLdb.IntegrityError, e:
                #print 'SQL integrity error: %s' % e
                log.msg('SQL integrity error: %s' % e)
            except MySQLdb.OperationalError, e:
                #print 'Full SQL statement: %s' % sql_query
                log.err('Full SQL statement: %s' % sql_query)
                log.err('Values: %s' % values)
            #except MySQLdb.ProgrammingError, e:
                #print 'Full SQL statement: %s' % sql_query

    def handle_error(self, e):
        log.err(e)


class SectorPipeline(BaseDB):

	def insert_new_sector(self, name):
		try:
			tx.execute(\
				"insert into sectors (name) "
				"values (%s)", ( name )
			)
			return self.conn.insert_id()
		except MySQLdb.IntegrityError, e:
			#print 'SQL integrity error: %s' % e
			log.msg('SQL integrity error: %s' % e)
			
	def get_sector_id(self, name, sub=False):
		name = item['subsector'] if sub else item['sector']
        	tx.execute("select id from sectors where name = %s", item['sector'])
        	result = tx.fetchone()
        	if result is None:
			return insert_new_sector( item['sector'] )
		else:
			return result['id']

	def _conditional_insert(self, tx, item):
        	if isinstance(item, MarketItem):
        		item['sector_int'] = get_sector_id(name, false)
        		item['subsector_int'] = get_sector_id(name, true)
        	return item


class MySQLStorePipeline(BaseDB):

    def process_item(self, item, spider):
        if not isinstance(item, TaseItem):
            return item
        # run db query in thread pool
        query = self.dbpool.runInteraction(self._conditional_insert, item)
        query.addErrback(self.handle_error)
        return item

    def _conditional_insert(self, tx, item):
        # create record if doesn't exist.
        # all this block run on it's own thread
        tx.execute("select * from companies where symbol = %s", item['symbol'])
        result = tx.fetchone()
        try:
            if result is None:
                tx.execute(\
                    "insert into companies (sessionid, category, symbol, name, sector, subsector, url, tase_url) "
                    "values (%s, %s, %s, %s, %s, %s, %s, %s)",
                    (
                    global_time,
                    item['category'],
                    item['symbol'],
                    item['name'],
                    item['sector_int'],
                    item['subsector_int'],
                    item['url'],
                    item['tase_url']
                    )
                )
            else:
                tx.execute(\
                    "UPDATE companies SET sessionid=%s, category=%s, name=%s, sector=%s, subsector=%s, url=%s, tase_url=%s WHERE symbol=%s",
                    (
                    global_time,
                    item['category'],
                    item['name'],
                    item['sector_int'],
                    item['subsector_int'],
                    item['url'],
                    item['tase_url'],
                    item['symbol']
                    )
                )
        except MySQLdb.IntegrityError, e:
            #print 'SQL integrity error: %s' % e
            log.msg('SQL integrity error: %s' % e)
            
        if PROCESS_HISTORY is False:
            return

        try:
            if item['category'] == category_comp:
                tx.execute(\
                        "insert into company_prices (sessionid, date_, symbol, adjusted_closing_price, closing_price, change_, opening_price, base_price, high, low, capital_listed_for_trading, market_cap, turnover, volume, trans) "
                        "values (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)",
                        (
                        global_time,
                        item['date_'],
                        item['symbol'],
                        item['adjusted_closing_price'],
                        item['closing_price'],
                        item['change_'],
                        item['opening_price'],
                        item['base_price'],
                        item['high'],
                        item['low'],
                        item['capital_listed_for_trading'],
                        item['market_cap'],
                        item['turnover'],
                        item['volume'],
                        item['trans']
                        )
                )
            elif item['category'] == category_bond:
                tx.execute(\
                        "insert into bond_prices (sessionid, date_, symbol, adjusted_closing_price, closing_price, change_, gross_yield_to_maturity, opening_price, base_price, high, low, capital_listed_for_trading, market_cap, turnover, volume, trans) "
                        "values (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)",
                        (
                        global_time,
                        item['date_'],
                        item['symbol'],
                        item['adjusted_closing_price'],
                        item['closing_price'],
                        item['change_'],
                        item['gross_yield_to_maturity'],
                        item['opening_price'],
                        item['base_price'],
                        item['high'],
                        item['low'],
                        item['capital_listed_for_trading'],
                        item['market_cap'],
                        item['turnover'],
                        item['volume'],
                        item['trans']
                        )
                )
            elif item['category'] == category_fund:
                tx.execute(\
                        "insert into fund_prices (sessionid, date_, symbol, purchase_price, redemption_price, change_, sales_load, management_fee, asset_value, trustee_fee) "
                        "values (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s)",
                        (
                        global_time,
                        item['date_'],
                        item['symbol'],
                        item['purchase_price'],
                        item['redemption_price'],
                        item['change_'],
                        item['sales_load'],
                        item['management_fee'],
                        item['asset_value'],
                        item['trustee_fee']
                        )
                )
        except MySQLdb.IntegrityError, e:
            #print 'SQL integrity error: %s' % e
            log.msg('SQL integrity error: %s' % e)

    def handle_error(self, e):
        log.err(e)




class NewsPipeline(BaseDB):

    def process_item(self, item, spider):
        if not isinstance(item, NewsArticle):
            return item
        if item['category'] != category_comp:
            return item
        symbol = item['symbol']
        #url_hash = hashlib.sha224(item['url']).hexdigest()
        url = item['url']
        url = url.encode("utf-8")
        #url = unicode(url)
        #url = urllib.quote(url)
        #url = url.replace(u'\u2019', u'\'')
        #url = url.replace(u'\u201c', u'')
        #url = url.replace(u'\u20ac', u'')
        #url = url.replace(u'\u05e0', u'')
        #url = unicode(url, errors='replace')
        try:
            url_hash = zlib.adler32(url)
        except:
            tase.common.log2(url)
#            print 'UnicodeEncodeError: %s' % item['url']
            return
        if len(item['summary']) < 10:
            return
#        url_hash = item['url_hash']
        self.cur.execute("select id from news_articles where url_hash = %s", url_hash)
        result = self.cur.fetchone()
        if result is None:
            self.cur.execute(\
                "insert into news_articles (sessionid, date_, url, url_hash, headline, summary, content) "
                "values (%s, %s, %s, %s, %s, %s, %s)",
                ( global_time, item['date_'], item['url'], url_hash, item['headline'], item['summary'], item['content'] )
            )
            article_id = self.conn.insert_id()
        else:
            article_id = result['id']
        try:
            self.cur.execute(\
                "insert into symbol_articles (sessionid, symbol, article_id) "
                "values (%s, %s, %s)",
                ( global_time, symbol, article_id )
            )
        except MySQLdb.IntegrityError, e:
            #print 'SQL integrity error: %s' % e
            log.msg('SQL integrity error: %s' % e)
        self.conn.commit()


class WeatherPipeline(BaseDB):

    def process_item(self, item, spider):
        if not isinstance(item, WeatherItem):
            return item
        try:
            self.cur.execute(\
                "insert into weather (sessionid, date_, temp_high, temp_avg, temp_low, dew_high, dew_avg, dew_low, hum_high, hum_avg, hum_low, pres_high, pres_avg, pres_low, vis_high, vis_avg, vis_low, wind_high, wind_avg, wind_low, precip, events) "
                "values (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)",
                ( global_time, item["date_"], item["temp_high"], item["temp_avg"], item["temp_low"], item["dew_high"], item["dew_avg"], item["dew_low"], item["hum_high"], item["hum_avg"], item["hum_low"], item["pres_high"], item["pres_avg"], item["pres_low"], item["vis_high"], item["vis_avg"], item["vis_low"], item["wind_high"], item["wind_avg"], item["wind_low"], item["precip"], item["events"] )
            )
        except MySQLdb.IntegrityError, e:
            #print 'SQL integrity error: %s' % e
            log.msg('SQL integrity error: %s' % e)
        self.conn.commit()


class MyImagesPipeline(ImagesPipeline):

    def get_media_requests(self, item, info):
        if isinstance(item, TaseItem):
		image_url = item['image_url']
		if image_url:
			return Request(image_url)

    def item_completed(self, results, item, info):
        image_paths = [x['path'] for ok, x in results if ok]
        if not image_paths:
            raise DropItem("Item contains no images")
        base_path = settings.get('IMAGES_STORE')
        item['image_path'] = os.path.join(base_path, image_paths[0])
        return item
