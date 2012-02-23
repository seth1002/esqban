#!/usr/bin/env python

import tase.getterms as getterms

import MySQLdb

import time
import datetime


db = MySQLdb.connect(host="localhost", user="root", passwd="", db="tase")
cursor = db.cursor()

global_time = time.time()
global_date = datetime.datetime.now()

cursor.execute("insert into sessions (sessionid, date_) values (%s, %s);",
        (
        global_time,
        global_date.isoformat()
        )
)
db.commit()

def process_url(url, symbol):
        print "Processing %s" % url
        for t in getterms.get_terms(url):
		try:
		    cursor.execute("select id from keywords where keyword = %s", t[0])
		    result = cursor.fetchone()
		    if result is None:
		        cursor.execute(\
		            "insert into keywords (sessionid, keyword) "
		            "values (%s, %s)",
		            (
		            global_time,
		            t[0]
		            )
		        )
		        #keyword_id = self.conn.insert_id()
		        keyword_id = cursor.lastrowid
		    else:
		        keyword_id = result[0]

		    cursor.execute(\
		        "insert into company_keywords (sessionid, symbol, keyword_id, x, y) "
		        "values (%s, %s, %s, %s, %s)",
		        (
		        global_time,
		        symbol,
		        keyword_id,
		        t[1],
		        t[2]
		        )
		    )
		except MySQLdb.IntegrityError, e:
		    print 'SQL integrity error: %s' % e
		except UnicodeEncodeError, e:
		    print 'Encoding error: %s' % e
	db.commit()


sql = "select url, symbol from companies where url!=''"
try:
    cursor.execute(sql)
    for row in cursor:
        url = row[0]
	symbol = row[1]
	process_url(url, symbol)
except MySQLdb.ProgrammingError, e:
    print 'SQL error: %s' % e





#http://diveintopython.org/html_processing/extracting_data.html










