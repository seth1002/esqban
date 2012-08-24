#!/usr/bin/env python

import os
import sys

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




rootdir = sys.argv[1]




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

def find_movie(name):
    try:
        str_query = "SELECT * FROM movies WHERE name like \"%" + name + "%\";"
        #print str_query
        #con.query(str_query)
        #result = con.use_result()
        cur = con.cursor()
        cur.execute(str_query)
        
        #print result.fetch_row()
        row = cur.fetchone ()
        if row is not None:
            print "* Found " + name
            #print row
        else:
            print "- " + name + " not found"
        cur.close()

    #except _mysql.Error, e:
    except MySQLdb.Error, e:
        print str_query
        print "Error %d: %s" % (e.args[0], e.args[1])
        #sys.exit(1)



def process_folder(path):
#    with open(os.path.join(folder,'python-outfile.txt'), 'w') as dest:
    for folder, subs, files in os.walk(path):
        for filename in files:
            if filename.lower().endswith(".avi"):
                find_movie( os.path.splitext(filename)[0] )
                #print filename
                #print_info(os.path.join(folder, filename))
            #with open(os.path.join(folder, filename), 'r') as src:
            #    #dest.write(src.read())

path = "/media/Video2"
process_folder(rootdir)
#process_folder(path)


if con:
    con.close()


