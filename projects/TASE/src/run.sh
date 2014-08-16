#/bin/sh

rm tase.log

sh import_currency.sh

#./scrapy-ctl.py crawl portfolio

#./scrapy-ctl.py crawl markets
./scrapy-ctl.py crawl weather
./scrapy-ctl.py crawl stocks
./scrapy-ctl.py crawl funds
./scrapy-ctl.py crawl bonds
./scrapy-ctl.py crawl news

#./scrapy-ctl.py crawl markets stocks bonds funds weather

#./scrapy-ctl.py crawl stocks & tail -f scrapy.log
#scrapy crawl stocks --set FEED_URI=items.json --set FEED_FORMAT=json
#scrapy crawl stocks

