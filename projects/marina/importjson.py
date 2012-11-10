#!/usr/bin/env python

#import json
#from pprint import pprint
#json_data=open('anchorageworld.txt')
#data = json.load(json_data)
#pprint(data)
#json_data.close()


import urllib
import xml.etree.ElementTree as xml
#from twisted.enterprise import adbapi
import MySQLdb.cursors
#import _mysql
import MySQLdb

#dbpool = adbapi.ConnectionPool('MySQLdb',
#                db='marinas',
#                user=settings['DATABASE_USER'],
#                passwd=settings['DATABASE_PASSWORD'],
#                cursorclass=MySQLdb.cursors.DictCursor,
#                charset='utf8',
#                use_unicode=True
#            )

#con = _mysql.connect('192.168.2.107', 'sqba', 'crl2688', 'marinas')
conn = MySQLdb.connect(host='192.168.2.107',user='sqba', passwd='crl2688', db = "marinas")


def get_country_id(name):
	cursor = conn.cursor()
	country_id = -1
	cursor.execute ('SELECT id FROM countries WHERE name="' + name + '"')
	row = cursor.fetchone ()
	if None != row:
		country_id = row[0]
	else:
		cursor.execute('INSERT INTO countries(name) VALUES("' + name + '")')
		country_id = cursor.lastrowid
	return country_id

def get_region_id(name, country):
	country_id = get_country_id(name)
	cursor = conn.cursor()
	region_id = -1
	cursor.execute ('SELECT id FROM regions WHERE name="' + name + '" AND country = "' + str(country_id) + '"')
	row = cursor.fetchone ()
	if None != row:
		region_id = row[0]
	else:
		cursor.execute('INSERT INTO regions(name, country) VALUES("' + name + '", ' + str(country_id) + ')')
		region_id = cursor.lastrowid
	return region_id

def get_marina_type_id(name):
	cursor = conn.cursor()
	type_id = -1
	cursor.execute ('SELECT id FROM marina_types WHERE name="' + name + '"')
	row = cursor.fetchone ()
	if None != row:
		type_id = row[0]
	else:
		cursor.execute('INSERT INTO marina_types(name) VALUES("' + name + '")')
		type_id = cursor.lastrowid
	return type_id

#print get_country_id("Italy")
#exit()

def process_name(name):
	return name.strip().replace(u'\u017e', u'z').replace(u'\u0160', u's').replace(u'\u0161', u's').replace(u'\u2019', u'')
	
tree = xml.parse(urllib.urlopen('data/portbooker.xml'))
rootElement = tree.getroot()
count=0
cursor = conn.cursor()
for a in rootElement.findall('marker'):
	country = process_name(a.get('pais_nombre'))
	region = process_name(a.get('zona_nombre'))
	marina_name = process_name(a.get('puerto_nombre'))
	marina_type = a.get('puerto_type_marina').strip()
	marina_lat = a.get('lat').strip()
	marina_long = a.get('lng').strip()
	#print marina_name
	print country + ": " + str(get_country_id(country))
	region_id = get_region_id(region, country)
	print region + ": " + str(region_id)
	marina_type_id = get_marina_type_id(marina_type)
	try:
		cursor.execute('INSERT INTO marinas(name, region, type, latitude, longitude) VALUES("' + marina_name + '", ' + str(region_id) + ', ' + str(marina_type_id) + ', ' + str(marina_lat) + ', ' + str(marina_long) + ')')
	except MySQLdb.IntegrityError, e:
		print 'SQL integrity error: %s' % e
	#cur.execute("INSERT INTO regions(name) VALUES('" + region + "')")
	count+=1
	print '==========================='
print count
conn.commit()
cursor.close()
    
