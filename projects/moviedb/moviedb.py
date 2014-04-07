#!/usr/bin/env python

import os
import sys
import re
from sys import argv, stderr, exit
import MySQLdb
from os.path import basename


con = MySQLdb.connect(host='127.0.0.1', db='imdb', user='imdb', passwd='12345') #user, passwd


def log2(msg, level=0):
	print msg
	if level == 0:
		open("log.txt", 'ab').write(msg + '\n')


def log_csv(name, path):
	msg = "'" + name + "';'" + path + "'"
	open("log.csv", 'ab').write(msg + '\n')

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
		log2(str_query)
		log2("Error %d: %s" % (e.args[0], e.args[1]))
	return result


def insert_movie(movie_id, movie_path):
	try:
		cur = con.cursor()
		cur.execute("insert into my_movies (id, path) values (%s, %s);", (movie_id, movie_path) )
		con.commit()
		log2("+ " + movie_path + " added successfully", 2)
	except MySQLdb.IntegrityError, e:
		cur = con.cursor()
		cur.execute("update my_movies set path=%s where id=%s;", (movie_path, movie_id) )
		con.commit()
		log2("+ " + movie_path + " updated successfully", 2)
		#log2('SQL integrity error: %s' % e, 1)


def movie_not_found_in_imdb(name, path):
	log2("- " + name + " not found")
	log_csv(name, path)


def process_name(name):
	name = re.sub(r'^[\d.]*\d+\.', "", name)

	#pattern1 = re.compile(r'DVDRIP', re.IGNORECASE)
	#pattern1.sub(" ", name)

	name = re.sub(r'DVDRIP', "", name, flags=re.I)
	#name = name.replace(r'DvDrip', " ")
	name = name.replace(r'-aXXo', " ")
	name = name.replace(r'Jaybob', " ")
	name = re.sub(r'KLAXXON', "", name, flags=re.I)
	name = re.sub(r'YIFY', "", name, flags=re.I)
	name = re.sub(r'bluray', "", name, flags=re.I)
	name = re.sub(r'x264', "", name, flags=re.I)
	name = re.sub(r'720p', "", name, flags=re.I)
	name = re.sub(r'1080p', "", name, flags=re.I)
	name = re.sub(r'PROPER', "", name, flags=re.I)
	name = re.sub(r'HDTV', "", name, flags=re.I)
	name = re.sub(r'trailer', "", name, flags=re.I)
	name = re.sub(r'XVID', "", name, flags=re.I)
	name = re.sub(r'divx', "", name, flags=re.I)
	name = re.sub(r'HDDVD', "", name, flags=re.I)
	name = re.sub(r'xnHD', "", name, flags=re.I)
	name = re.sub(r'NhaNc3', "", name, flags=re.I)

	name = re.sub(r'\[(.*?)\]', "", name)
	name = re.sub(r'\((.*?)\)', "", name)
	name = re.sub(r'\{(.*?)\}', "", name)
	name = re.sub(r'^The ', "", name)
	name = re.sub(r'^A ', "", name)
	#name = re.sub(r'S^[0-9]{2}$E^[0-9]{2}$', "", name)
	name = re.sub(r'S\d+E\d+', "", name)

	#name = re.sub(r'\'', "\'\'", name)
	name = re.sub(r'\'', "%", name)

	name = re.sub(r' -', "%", name)
	#name = name.replace(r'-', ":")
	#s = name.split('-', 2)
	#if len(s) > 1:
	#    name = s[0]

	name = name.replace(r'-', " ")
	name = name.replace(r'.', " ")

	name = name.strip()

	#print name

	return name


def process_folder(path):
	count_total = 0
	counter_found = 0
	counter_not_found = 0
#	with open(os.path.join(folder,'python-outfile.txt'), 'w') as dest:
	for folder, subs, files in os.walk(path):
		for filename in files:
			if filename.lower().endswith(".avi") or filename.lower().endswith(".mkv") or filename.lower().endswith(".mp4") or filename.lower().endswith(".divx"):
				full_path = os.path.join(folder, filename)
				name = os.path.splitext(filename)[0]
				name = process_name( name )
				if name:
					count_total += 1
					movie_id = find_movie( name )
					if( movie_id > -1 ):
						counter_found += 1
						insert_movie( movie_id, full_path )
					else:
						counter_not_found += 1
						movie_not_found_in_imdb(name, full_path)
					#print_info(os.path.join(folder, filename))
			#with open(os.path.join(folder, filename), 'r') as src:
			#	dest.write(src.read())
	percent_found = 100 - counter_not_found*100/count_total
	log2("Found " + str(counter_found) + " out of " + str(count_total) + " (" + str(percent_found) + "%)", 2)


#rootdir = sys.argv[1]
#process_folder(rootdir)


path = "/media/"
process_folder(path)


if con:
    con.close()


