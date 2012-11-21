from google.appengine.ext import db
#from google.appengine.tools import bulkloader
from google.appengine.ext import bulkload
from google.appengine.api import datastore_types
#from geo.geomodel import GeoModel

class Marina(db.Model):
	name = db.StringProperty()
	latitude = db.FloatProperty()
	longitude = db.FloatProperty()
	#point = db.GeoPtProperty()
	marina_type = db.IntegerProperty()

#class Marina2(GeoModel):
#	name = db.StringProperty()
#	latitude = db.FloatProperty()
#	longitude = db.FloatProperty()
#	#point = db.GeoPtProperty()
#	marina_type = db.IntegerProperty()

class MarinaLoader(bulkload.Loader):
	def __init__(self):
		bulkload.Loader.__init__(self, 'Marina',
								[("name", lambda x: x.decode('utf-8')),
								("latitude", float),
								("longitude", float),
								("marina_type", int)
								])

#loaders = [MarinaLoader]

if __name__=='__main__':
	loader = MarinaLoader()
	bulkload.main(loader)
