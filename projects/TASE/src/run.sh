#/bin/sh

sh import_currency.sh

./scrapy-ctl.py crawl markets stocks bonds funds weather

#./scrapy-ctl.py crawl markets weather
#./scrapy-ctl.py crawl stocks
#./scrapy-ctl.py crawl funds
#./scrapy-ctl.py crawl bonds


#./scrapy-ctl.py crawl stocks & tail -f scrapy.log
#scrapy crawl stocks --set FEED_URI=items.json --set FEED_FORMAT=json
#scrapy crawl stocks

