import os
import webapp2
from google.appengine.ext.webapp.util import run_wsgi_app
from models import Marina
from google.appengine.ext import db

class MainHandler(webapp2.RequestHandler):
	def get(self):
		self.get_from_db()
		
	def get_from_file(self):
		path = os.path.join(os.path.split(__file__)[0], 'marinas2.json')
		ret = file(path,'r').read()
		self.response.write( ret )

	def get_from_db(self):
		a = self.request.get('a')
		b = self.request.get('b')
		c = self.request.get('c')
		d = self.request.get('d')
		self.response.headers['Content-Type'] = 'application/json'
		self.response.out.write('{"markers":[\n')
#		marinas = db.GqlQuery("SELECT *  FROM Marina WHERE latitude < " + str(a) + " AND latitude > " + str(c) + " AND longitude < " + str(b) + " AND longitude > " + str(d) + " LIMIT 100")
		marinas = db.GqlQuery("SELECT *  FROM Marina WHERE longitude < " + str(b) + " AND longitude > " + str(d))
		keys = dict()
		for marina in marinas:
			keys[marina.key()] = 1
		marinas = db.GqlQuery("SELECT *  FROM Marina WHERE latitude < " + str(a) + " AND latitude > " + str(c))
#		marinas = Marina.all()
#		marinas.filter("latitude <", a)
#		marinas.filter("latitude >", c)
#		marinas.filter("longitude >", b)
#		marinas.filter("longitude <", d)
		start = True
		for marina in marinas:
			if marina.key() in keys:
				if start:
					start = False
				else:
					self.response.out.write(",\n")
				s = '{"latitude": "' + str(marina.latitude) + '", "name": "' + marina.name + '", "longitude": "' + str(marina.longitude) + '"}'
				self.response.out.write(s)
		self.response.out.write('\n]}')

application = webapp2.WSGIApplication([('/markers', MainHandler)], debug=True)

def main():
    run_wsgi_app(application)

if __name__ == "__main__":
    main()

