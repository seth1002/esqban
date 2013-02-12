import re

from scrapy.contrib.spiders import Rule
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor

#from scrapy.selector import HtmlXPathSelector
from scrapy.http import Request
#from scrapy.http import FormRequest
from scrapy.conf import settings
#from scrapy.shell import inspect_response

from tase.HistorySpider import HistorySpider
from tase.items import TaseItem

import tase.common


PROCESS_HISTORY = settings.getbool('PROCESS_HISTORY', False)
HISTORY_PERIOD = settings.getint('HISTORY_PERIOD', 2)
category_fund = settings.get('CATEGORY_FUND')


class FundSpider(HistorySpider):
	name = 'funds'
	allowed_domains = ['tase.co.il']
	start_urls = ['http://www.tase.co.il/TASEEng/MarketData/MutualFunds/']

	rules = (
		Rule(SgmlLinkExtractor(allow=[r'BuildCmb_6_1.js']), callback='parse_fund_list'),
		Rule(SgmlLinkExtractor(allow=('FundMainData\.htm',)), callback='parse_fund'),
		Rule(SgmlLinkExtractor(allow=(r'MutualFunds',)), callback='parse_fund_search'),
		Rule(SgmlLinkExtractor(allow=('fundHistory\.aspx',)), callback='get_history_data'),
	)

	header = (
		('date_', tase.common.get_date),
		('purchase_price', tase.common.to_float),
		('redemption_price', tase.common.to_float),
		('change_', tase.common.to_float),
		('sales_load', tase.common.to_float),
		('management_fee', tase.common.to_float),
		('asset_value', tase.common.to_int),
		('trustee_fee', tase.common.to_float)
	)

	list_url = 'http://www.tase.co.il/_layouts/Tase/Scripts/SearchJSFiles/BuildCmb_6_1.js'
	main_url = "http://www.tase.co.il/eng/general/trustfund/pages/fundmaindata.aspx?FundID={fundID}"
	history_url = "http://www.tase.co.il/Eng/General/trustfund/Pages/fundHistory.aspx?Action=&FundID={shareID}"

	def parse_fund_search(self, response):
		return Request(self.list_url, callback=self.parse_fund_list)

	def parse_fund_list(self, response):
		matches = re.findall("[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]", response.body)
		for m in matches:
			item = TaseItem()
			item['category'] = category_fund
			item['symbol'] = m
			item['CompanyID'] = m
			item['ShareID'] = m
			url = self.main_url.format(fundID=m)
			yield Request(url, callback=self.parse_fund, meta={'item': item})

	def parse_fund(self, response):
		item = response.request.meta['item']
		item['tase_url'] = response.url
		item['name'] = item['symbol']
		item['sector'] = ""
		item['subsector'] = ""
		item['url'] = ""
		return self.process_history(item)
