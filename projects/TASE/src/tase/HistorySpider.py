#!/usr/bin/env python

from scrapy.contrib.spiders import CrawlSpider

from scrapy.selector import Selector
from scrapy.http import Request
from scrapy.http import FormRequest
from scrapy.conf import settings

import urllib
from urlparse import urlparse, urlunparse
#from urlparse import parse_qs
#from urlparse import urljoin
#from scrapy import log

from tase.items import TaseItem

PROCESS_HISTORY = settings.getbool('PROCESS_HISTORY', False)
HISTORY_PERIOD = settings.getint('HISTORY_PERIOD', 2)

class HistorySpider(CrawlSpider):

    history_url = ""
    header = ()
    
    def process_history(self, item):
        if PROCESS_HISTORY is False:
            return item
        else:
            url = self.history_url.format(shareID=item['ShareID'], companyID=item['CompanyID'], period=HISTORY_PERIOD)
            return Request(url, callback=self.get_history_data, meta={'item': item})
    
    def get_base_url(self, response):
        sel = Selector(response)
        base_url = response.url
        #base_url = sel.xpath('//base/@href')[0].extract()
        o = urlparse(base_url)
        res = urlunparse(o)
#        log.msg("get_base_url: " + res, level=log.WARNING)
        return res

    def get_history_data(self, response):
        item = response.request.meta['item'] 
        sel = Selector(response)
        viewstate = sel.xpath('//input[@name="__VIEWSTATE"]/@value').extract()[0]
        fd = {
            '__VIEWSTATE':viewstate,
            'ctl00$SPWebPartManager1$' + self.get_control_id() + '$ctl00$HistoryData1$hiddenID':'0',
            'ctl00$SPWebPartManager1$' + self.get_control_id() + '$ctl00$HistoryData1$rbFrequency':'rbFrequency1',
            'ctl00$SPWebPartManager1$' + self.get_control_id() + '$ctl00$HistoryData1$RBCoordinatedList': 'AdjustmentRate',
        }
        fd['ctl00$SPWebPartManager1$' + self.get_control_id() + '$ctl00$HistoryData1$rbPeriod'] = "rbPeriod{period}".format(period=HISTORY_PERIOD)
        for i in range(20):
            name = "ctl00$SPWebPartManager1$" + self.get_control_id() + "$ctl00$HistoryData1$CBDailyDFiledsList${index}".format(index=i)
            fd[name] = 'on'
        #base_url = self.get_base_url(response)
        #response = response.replace(url=base_url)
        req = FormRequest.from_response(response, formdata=fd, formname='aspnetForm', callback=self.parse_history_data, meta={'item': item})
        self.log( req )
        return req;

    def parse_history_data(self, response):
        item = response.request.meta['item']
        sel = Selector(response)
        table = sel.xpath('//table[@id="ctl00_SPWebPartManager1_' + self.get_control_id() + '_ctl00_HistoryData1_gridHistoryData_DataGrid1"]')
        rows = table.xpath('tr')#[@class != "gridHeader"]')
        row_index = 0
        for row in rows:
            columns = row.xpath('td')
            if len(self.header) <= len(columns) and row_index > 0:
                item2 = TaseItem(item)
                col_index = 0
                for column in columns:
                    if col_index < len(self.header):
                        name = self.header[col_index][0]
                        func = self.header[col_index][1]
                        s = column.xpath('.//text()').extract()
                        item2[name] = func(s)
                    col_index += 1
                yield item2
            row_index += 1
