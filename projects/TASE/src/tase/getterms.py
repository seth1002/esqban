#!/usr/bin/env python

from topia.termextract import extract
from HTMLParser import HTMLParser, HTMLParseError


#from BeautifulSoup import BeautifulSoup

"""
from re import sub
from sys import stderr
from traceback import print_exc
"""
#import codecs




"""
class _DeHTMLParser(HTMLParser):
    def __init__(self):
        HTMLParser.__init__(self)
        self.__text = []

    def handle_data(self, data):
        text = data.strip()
        if len(text) > 0:
            text = sub('[ \t\r\n]+', ' ', text)
            self.__text.append(text + ' ')

    def handle_starttag(self, tag, attrs):
        if tag == 'p':
            self.__text.append('\n\n')
        elif tag == 'br':
            self.__text.append('\n')

    def handle_startendtag(self, tag, attrs):
        if tag == 'br':
            self.__text.append('\n\n')

    def text(self):
        return ''.join(self.__text).strip()


def dehtml(text):
    try:
        parser = _DeHTMLParser()
        parser.feed(text)
        parser.close()
        return parser.text()
    except:
        print_exc(file=stderr)
        return text
"""


"""
import htmllib
import formatter
from formatter import AbstractFormatter

class Paragraph:
    def __init__(self):
        self.text = ''
        self.bytes = 0
        self.density = 0.0
 
class LineWriter(formatter.AbstractWriter):
    def __init__(self, *args):
        self.last_index = 0
        self.lines = [Paragraph()]
        formatter.AbstractWriter.__init__(self)
 
    def send_flowing_data(self, data):
        # Work out the length of this text chunk.
        t = len(data)
        # We've parsed more text, so increment index.
        self.index += t
        # Calculate the number of bytes since last time.
        b = self.index - self.last_index
        self.last_index = self.index
        # Accumulate this information in current line.
        l = self.lines[-1]
        l.text += data
        l.bytes += b
 
    def send_paragraph(self, blankline):
        #Create a new paragraph if necessary.
        if self.lines[-1].text == '':
            return
        self.lines[-1].text += 'n' * (blankline+1)
        self.lines[-1].bytes += 2 * (blankline+1)
        self.lines.append(Writer.Paragraph())
 
    def send_literal_data(self, data):
        self.send_flowing_data(data)
 
    def send_line_break(self):
        self.send_paragraph(0)


class TrackingParser(htmllib.HTMLParser):
    #Try to keep accurate pointer of parsing location.
    def __init__(self, writer, *args):
        htmllib.HTMLParser.__init__(self, *args)
        self.writer = writer
    def parse_starttag(self, i):
        index = htmllib.HTMLParser.parse_starttag(self, i)
        self.writer.index = index
        return index
    def parse_endtag(self, i):
        self.writer.index = i
        return htmllib.HTMLParser.parse_endtag(self, i)

def extract_text(html):
    # Derive from formatter.AbstractWriter to store paragraphs.
    writer = LineWriter()
    # Default formatter sends commands to our writer.
    formatter = AbstractFormatter(writer)
    # Derive from htmllib.HTMLParser to track parsed bytes.
    parser = TrackingParser(writer, formatter)
    # Give the parser the raw HTML data.
    parser.feed(html)
    parser.close()
    # Filter the paragraphs stored and output them.
    return writer.output()
"""


MIN_TERM_LENGTH = 5
MAX_TERM_LENGTH = 20

title_trans=''.join(chr(c) if chr(c).isupper() or chr(c).islower() else '_' for c in range(256))

def get_text_shell(url):
    import os
    # Timeout 10sec, 5 retries
    #stream = os.popen("wget %s -T10 -t5 -O- | html2text" % url)
    stream = os.popen("lynx -dump %s" % url)
    text = stream.read()
    stream.close()
    return text

def get_text_python(url):
    import urllib2
    from html2text import html2text
    f = urllib2.urlopen(url)
    encoding = f.headers.getparam('charset')
    if not encoding is None:
        print 'Encoding: ' + encoding
        html = f.read().decode(encoding)
    else:
        #html = codecs.utf_8_decode(f.read())
        try:
            html = f.read().decode('utf8')
            #html = f.read().decode('1255')
            #html = unicode(f.read())
        except UnicodeDecodeError, e:
            print "Error: " + str(e)
            html = ""
    f.close()
    #html = html.encode('ascii')
    #html = html.replace('<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"', '')
    try:
        text = html2text(html)
	#text = extract_text(html)
	#text = dehtml(html)
	#soup = BeautifulSoup(html)
	#if not soup is None:
	#	result = soup.body(text=True)
	#	if not result is None:
	#		text = str(result)
	#		text = text.replace(b'\n', ' ')
	#		print text
    except HTMLParseError, e:
        print "HTMLParseError: " + str(e)
        text = ""
    except TypeError, e:
        print "TypeError: " + str(e)
        text = ""
    return text

def get_text(url):
    #print 'get_text(%s)'%url
    return get_text_shell(url)
    #return get_text_python(url)

def get_terms(url):
    text = get_text(url)
    extractor = extract.TermExtractor()
    extractor.filter = extract.permissiveFilter
    #extractor.filter = extract.DefaultFilter(singleStrengthMinOccur=2)
    terms = extractor(text)
    for t in terms:
        if MIN_TERM_LENGTH <= len(t[0]) <= MAX_TERM_LENGTH:
            txt = t[0]
            txt = txt.translate(title_trans)
            txt = txt.replace('_', ' ')
            txt = txt.strip().lower()
            if len(txt) > 2:
                t2 = [txt, t[1], t[2]]
                yield t2

