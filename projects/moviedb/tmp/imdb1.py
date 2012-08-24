#import time, datetime

import sys, os
import urllib, urllib2

# Global variables
name = 'Watch movie details on IMDB'
_list = ['[InternetShortcut]', '']

def writeToFile(fulldir, name, _list):
    # Creates a file and saves it to .url,
    # creating an url shortcut to the webpage in _list
    f = open(fulldir + '\\' + name + '.url', 'w')
    for item in _list:
        f.write(item + '\n')
    f.close

def getURL(search):
    # Does a search using google feeling lucky, and returns the url of the hit
    query = 'http://www.google.com/search?btnI=745&q='
    # Header to spoof an old mozilla browser
    user_agent = 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)'
    headers = { 'User-Agent' : user_agent }
    # Create the actual url and request to homepage from google
    url = query + search
    req = urllib2.Request(url, None, headers)
    response = urllib2.urlopen(req)
    the_page = response.read()
    if '.imdb.com' in the_page:
        #the_header = response.info()
        if '<link rel="canonical" href="' in the_page:
            result = the_page[the_page.find('<link rel="canonical" href="')+28:\
                              the_page.find('<link rel="canonical" href="')+80]
            result = result[:result.rfind('\"')]
            return result
        else:
            print 'IMDB site found, but could\'nt track URL'
    else:
        print 'The site returned was not IMDB.',
        print 'The link will not be saved for:', search
    #return result

def main():
    # Current Working Directory (sys.path[0] also works)
    #folder = os.getcwd() # Change this to do it in another folder
    folder = '\\\\DISKSTATION\\video'
    # Runs through all subfolders of current and prints their subfolders
    for curdir, direntries, fileentries in os.walk(folder):
        # Strips to only the folder name
        fulldir = curdir[:]
        curdir = curdir[curdir.rfind('\\')+1:]
        curdir = curdir.replace(' ', '%20')
        url = getURL(curdir)
        try:
            _list[1] = 'URL=' + url
            writeToFile(fulldir, name, _list)
            print "Success:", curdir.replace('%20', ' '), "(", _list[1], ")"
        except TypeError, e:
            print "Error concatenating list:", e
    print "Done!"

if __name__ == "__main__":
    main()
    
