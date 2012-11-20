# marinas/models.py

from google.appengine.ext import db

class Marina(db.Model):
	name = db.StringProperty()
	latitude = db.FloatProperty()
	longitude = db.FloatProperty()
	marina_type = db.ReferenceProperty(MarinaType)
