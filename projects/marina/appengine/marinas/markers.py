import os
import webapp2
from google.appengine.ext.webapp.util import run_wsgi_app
import models

class MainHandler(webapp2.RequestHandler):
	def get(self):
		#ret = {"markers":{
		##"record": {"name": "bob", ...}
		#	{"latitude": "33.972499847412", "site": "Theharboratarchstone.com", "longitude": "-118.452003479"},
		#}
		path = os.path.join(os.path.split(__file__)[0], 'marinas.json')
		ret = file(path,'r').read()
		self.response.write( ret )

application = webapp2.WSGIApplication([('/markers', MainHandler)], debug=True)

def main():
    run_wsgi_app(application)

if __name__ == "__main__":
    main()

