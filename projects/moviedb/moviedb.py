#!/usr/bin/env python

import os
import sys
import re

from hachoir_core.error import HachoirError
from hachoir_core.cmd_line import unicodeFilename
from hachoir_parser import createParser
from hachoir_core.tools import makePrintable
from hachoir_metadata import extractMetadata
from hachoir_core.i18n import getTerminalCharset
from sys import argv, stderr, exit

#import _mysql
import MySQLdb

from os.path import basename

from imdb import IMDb
ia = IMDb()




def print_info(filename):
    filename, realname = unicodeFilename(filename), filename
    parser = createParser(filename, realname)
    if not parser:
        print >>stderr, "Unable to parse file"
        #exit(1)
    try:
        metadata = extractMetadata(parser)
    except HachoirError, err:
        print "Metadata extraction error: %s" % unicode(err)
        metadata = None
    if not metadata:
        print "Unable to extract metadata"
        #exit(1)

    text = metadata.exportPlaintext()
    charset = getTerminalCharset()
    for line in text:
        print makePrintable(line, charset)

    print metadata.get('duration')
    print metadata.get('width')
    print metadata.get('height')
    print metadata.get('frame_rate')
    #print metadata.get('aspect_ratio')
    #print metadata.get('aspect_ratio')
    #for data in metadata:
    #    print data.key

#con = _mysql.connect(host='localhost', db='imdb') #user, passwd
con = MySQLdb.connect(host='localhost', db='imdb') #user, passwd

def find_movie_in_db(name):
    found = False
    try:
        str_query = "SELECT * FROM movies WHERE name like \"%" + name + "%\";"
        #str_query = "SELECT * FROM movies WHERE name like \"" + name + "\";"
        #print str_query
        #con.query(str_query)
        #result = con.use_result()
        cur = con.cursor()
        cur.execute(str_query)
        
        #print result.fetch_row()
        row = cur.fetchone ()
        if row is not None:
            print "* Found " + name
            found = True
            #print row
#        else:
#            print "- " + full_path + " not found"
#            print "- " + name + " not found, searching online"
#            results = ia.search_movie(name)
#            for movie in results:
#                print movie#['title']
        cur.close()

    #except _mysql.Error, e:
    except MySQLdb.Error, e:
        print str_query
        print "Error %d: %s" % (e.args[0], e.args[1])
        #sys.exit(1)
    return found

def process_name(name):
    #[\d.]*\d+
    name = re.sub(r'^[\d.]*\d+\.', "", name)

    #name = name.replace('.', " ")
    name = name.replace(r'DvDrip-aXXo', " ")

    #\[(.*?)\]      \((.*?)\)      \{(.*?)\}
    name = re.sub(r'\[(.*?)\]', "", name)
    name = re.sub(r'\((.*?)\)', "", name)
    name = re.sub(r'\{(.*?)\}', "", name)
    name = re.sub(r'^The', "", name)

    #s = re.sub('-', '', s)
    name = name.replace(r'-', ":")
    #s = name.split('-', 2)
    #if len(s) > 1:
    #    name = s[0]

    name = name.strip()

    #print name

    return name


def find_movie(name, full_path):

    name = process_name(name)

    result = find_movie_in_db(name)

    if not result:
        print "- " + full_path + " not found"

    return result


def process_folder(path):
    counter_found = 0
    counter_not_found = 0
#    with open(os.path.join(folder,'python-outfile.txt'), 'w') as dest:
    for folder, subs, files in os.walk(path):
        for filename in files:
            if filename.lower().endswith(".avi") or filename.lower().endswith(".mkv") or filename.lower().endswith(".mp4"):
                full_path = os.path.join(folder, filename)
                if( find_movie( os.path.splitext(filename)[0], full_path ) ):
                    counter_found += 1
                else:
                    counter_not_found += 1
                #print filename
                #print_info(os.path.join(folder, filename))
            #with open(os.path.join(folder, filename), 'r') as src:
            #    #dest.write(src.read())
    print "Found: " + str(counter_found)
    print "Not found: " + str(counter_not_found)


#rootdir = sys.argv[1]
#process_folder(rootdir)


path = "/media/Video2"
process_folder(path)


if con:
    con.close()


