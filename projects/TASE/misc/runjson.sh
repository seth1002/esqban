#/bin/sh

date > 0log.txt
date > scrapy.log

#mysql -uroot TASE < db/mysql.sql

#./scrapy-ctl.py crawl tase.co.il

./scrapy-ctl.py crawl tase.co.il --set FEED_URI=items.json --set FEED_FORMAT=json
#scrapy crawl tase.co.il

