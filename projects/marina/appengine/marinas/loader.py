# appengine/loader.py

from google.appengine.ext import bulkload

class MarinaLoader(bulkload.Loader):
	def __init__(self):
		fields = [
			("name", str),
			("quantity", int)
		]

		bulkload.Loader.__init__(self, "Marinas", fields)

if __name__ == "__main__":
	bulkload.main(MarinaLoader())
