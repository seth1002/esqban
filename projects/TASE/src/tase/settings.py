# - Scrapy settings for tase project

#import tase

BOT_NAME = 'tase'
#BOT_VERSION = '1.0'

SPIDER_MODULES = ['tase.spiders']
NEWSPIDER_MODULE = 'tase.spiders'
DEFAULT_ITEM_CLASS = 'scrapy.item.Item'
#USER_AGENT = '%s/%s' % (BOT_NAME, BOT_VERSION)
USER_AGENT = 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17'

DEFAULT_REQUEST_HEADERS = {
 'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
 'Accept-Language': 'en-US,en;q=0.8,sr;q=0.6',
}

#CONCURRENT_ITEMS = 1 #Default: 100
#CONCURRENT_REQUESTS_PER_DOMAIN = 1 #Default: 8
#CONCURRENT_SPIDERS = 1 #Default: 8
#DOWNLOAD_TIMEOUT = 360

MEMUSAGE_ENABLED = True
MEMUSAGE_LIMIT_MB = 1000

DOWNLOAD_DELAY = 1
RANDOMIZE_DOWNLOAD_DELAY = True

#LOG_FILE = 'tase.log'
LOG_STDOUT = True
#LOG_LEVEL = 'WARNING'
#LOG_LEVEL = 'ERROR'

DATABASE_HOST = '127.0.0.1'
DATABASE_SCHEMA = 'tase'
DATABASE_USER = 'tase'
DATABASE_PASSWORD = '12345'

PROCESS_HISTORY = True
#HISTORY_PERIOD = 1 # Last Trading Day
HISTORY_PERIOD = 2 # Last Month
#HISTORY_PERIOD = 3 # 3 Months
#HISTORY_PERIOD = 4 # 6 Months
#HISTORY_PERIOD = 5 # 1 Year
#HISTORY_PERIOD = 6 # 3 Years
#HISTORY_PERIOD = 7	# 5 Years
PROCESS_NEWS = True
PROCESS_NEWS_CONTENT = True
PROCESS_NEWS_HISTORY = True
#PROCESS_FINANCIAL_STATEMENTS = True

# for the database
CATEGORY_COMP = 'company' # rename some day to stock
CATEGORY_BOND = 'bond'
CATEGORY_FUND = 'fund'

ITEM_PIPELINES = (
	'tase.pipelines.CleanupPipeline',
	'tase.pipelines.SessionPipeline',
	'tase.pipelines.MultiplierPipeline',
	'tase.pipelines.MarketDataPipeline',
#	'tase.pipelines.PageRankPipeline',
#	'tase.pipelines.KeywordPipeline',
	'tase.pipelines.FinancialStatementsPipeline',
	'tase.pipelines.NewsPipeline',
	'tase.pipelines.WeatherPipeline',
	'tase.pipelines.MySQLStorePipeline',
)


