
echo %DATE% > 0log.txt
echo %DATE% > scrapy.log


scrapy crawl tase.co.il --set FEED_URI=items.json --set FEED_FORMAT=json


rem scrapy shell http://tase.co.il --nolog

