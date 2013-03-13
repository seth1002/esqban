#!/usr/bin/env python

import os
import sys
import re

from sys import argv, stderr, exit

import MySQLdb

from os.path import basename

con = MySQLdb.connect(host='192.168.2.106', db='imdb', user='sqba', passwd='crl2688') #user, passwd

def find_movie(name):
    result = -1
    str_query = "SELECT id FROM movies WHERE name like \"%" + name + "%\" ORDER BY rank DESC;"
    try:
        cur = con.cursor()
        cur.execute(str_query)
        row = cur.fetchone ()
        if not row is None:
			result = row[0]
        cur.close()
    except MySQLdb.Error, e:
        print str_query
        print "Error %d: %s" % (e.args[0], e.args[1])
    return result

def insert_movie(movie_id, movie_path):
	try:
		cur = con.cursor()
		cur.execute("insert into my_movies (id, path) values (%s, %s);",	(movie_id, movie_path) )
		con.commit()
	except MySQLdb.IntegrityError, e:
		print 'SQL integrity error: %s' % e

def process_name(name):
    name = re.sub(r'^[\d.]*\d+\.', "", name)

    name = name.replace(r'DvDrip-aXXo', " ")

    name = re.sub(r'\[(.*?)\]', "", name)
    name = re.sub(r'\((.*?)\)', "", name)
    name = re.sub(r'\{(.*?)\}', "", name)
    name = re.sub(r'^The ', "", name)
    name = re.sub(r'^A ', "", name)

    #name = re.sub(r'\'', "\'\'", name)
    name = re.sub(r'\'', "%", name)

    name = re.sub(r' -', "%", name)
    #name = name.replace(r'-', ":")
    #s = name.split('-', 2)
    #if len(s) > 1:
    #    name = s[0]

    name = name.strip()

    #print name

    return name


def process_folder(path):
    count_total = 0
    counter_found = 0
    counter_not_found = 0
#    with open(os.path.join(folder,'python-outfile.txt'), 'w') as dest:
    for folder, subs, files in os.walk(path):
        for filename in files:
            if filename.lower().endswith(".avi") or filename.lower().endswith(".mkv") or filename.lower().endswith(".mp4") or filename.lower().endswith(".divx"):
                full_path = os.path.join(folder, filename)
                name = os.path.splitext(filename)[0]
                name = process_name( name )
                count_total += 1
                movie_id = find_movie( name )
                if( movie_id > -1 ):
                    counter_found += 1
                    insert_movie( movie_id, full_path )
#                    print "* Found " + name
                else:
                    counter_not_found += 1
                    print "- " + full_path + " not found"
#                    print "- '" + name + "' not found"
                #print filename
                #print_info(os.path.join(folder, filename))
            #with open(os.path.join(folder, filename), 'r') as src:
            #    #dest.write(src.read())
    percent_found = 100 - counter_not_found*100/count_total
    print "Found " + str(counter_found) + " out of " + str(count_total) + " (" + str(percent_found) + "%)"

#rootdir = sys.argv[1]
#process_folder(rootdir)


path = "/media/Video2/common"
process_folder(path)


if con:
    con.close()


