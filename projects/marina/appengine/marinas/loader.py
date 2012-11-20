#from google.appengine.ext import bulkload
from google.appengine.tools import bulkloader
from google.appengine.api import datastore_types

class MarinaLoader(bulkloader.Loader):
	def __init__(self):
		bulkloader.Loader.__init__(self, 'Marina',
								[("name", str),
								("latitude", float),
								("longitude", float),
								("marina_type", int)
								])

loaders = [AlbumLoader]
    
