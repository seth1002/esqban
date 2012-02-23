#!/usr/bin/env python

from topia.termextract import extract
import urllib2
from html2text import html2text


MIN_TERM_LENGTH = 5
MAX_TERM_LENGTH = 20

def extract_terms(text):
    extractor = extract.TermExtractor()
    extractor.filter = extract.permissiveFilter
    terms = extractor(text)
    #return terms
    return [t[0].lower() for t in terms if t[2] == 1 and MIN_TERM_LENGTH <= len(t[0]) <= MAX_TERM_LENGTH]

"""
f = urllib2.urlopen("http://www.betterplace.com/")
encoding = f.headers.getparam('charset')
if encoding is not None:
	html = f.read().decode(encoding)
else:
	html = f.read()

#text = html.encode('UTF8')
text = html2text(html)

#print text.encode('UTF8')
open("text.txt", 'w').write(text.encode('UTF8'))
"""

#text = open("html.txt", 'r').read()

import os
stream = os.popen("wget %s -O- | html2text" % 'www.betterplace.com')
text = stream.read()

terms = extract_terms(text)
for t in terms:
    print t




