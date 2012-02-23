
import os
import sys
import MySQLdb
#import time
#import datetime
#import subprocess


db = MySQLdb.connect(host="localhost", user="root", passwd="", db="tase")



def normalize_filename(s):
	s = s.replace("&", "AND")
	s = s.replace(" ", "_")
	return s


def plot_graph(cursor, sector, subsector, symbol):
	datafilename = "tmp/{0}-{1}-{2}-data".format(sector, subsector, symbol)
	imagefilename = "graphs/{0}/{1}/{2}.png".format(sector, subsector, symbol)

	datafile = open(datafilename, 'w+r')
	numrows = int(cursor.rowcount)
	if numrows == 0:
		return False
	for i in range(0,numrows):
		row = cursor.fetchone()
		#print row[0], row[1]
		datafile.write(str(row[0]) + "|" + str(row[1]) + "\n")
	datafile.close()


	#imagefilename = "graphs/{0}/{1}/0ALL.png".format(sector_name, subsector_name)

	conffilename = "tmp/{0}-{1}-{2}-plot-load.conf".format(sector, subsector, symbol)
	configfile = open(conffilename, 'w+r')
	configfile.write('set terminal png size 1200,800' + "\n")
	configfile.write('set xdata time' + "\n")
	#configfile.write('set timefmt "%b-%d-%H:%M:%S"' + "\n")
	configfile.write("set output '%s'" % imagefilename + "\n")
	#configfile.write('time range must be in same format as data file' + "\n")
	#configfile.write('set xrange ["2005-00-00":"2012-00-00"]' + "\n")
	#configfile.write('set yrange [0:100]' + "\n")
	configfile.write('set grid' + "\n")
	configfile.write('set xlabel "Date"' + "\n")
	configfile.write('set ylabel "Price"' + "\n")
	configfile.write('set title "Price Averages"' + "\n")
	configfile.write('set key left box' + "\n")
	configfile.write('set style data lines' + "\n")
	#configfile.write('set output "escan_graph.png"' + "\n")
	configfile.write('set datafile separator "|"' + "\n")
	#configfile.write('set xdata time' + "\n")
	configfile.write('set timefmt "%Y-%m-%d"' + "\n")
	#configfile.write('set data style lines' + "\n")
	#configfile.write('set xrange ["2005-00-00":"2012-00-00"]' + "\n")
	#configfile.write('set format x "%Y-%m-%d"' + "\n")
	#configfile.write('set xlabel "Time"' + "\n")
	#configfile.write('set ylabel "Price"' + "\n")
	configfile.write("plot '{0}' using 1:2 index 0 title '{1}' with lines".format(datafilename, symbol))
	configfile.close()

	#subprocess.call(['cat plot-load.conf | gnuplot'])
	print 'cat %s | gnuplot' % conffilename
	os.system('cat %s | gnuplot' % conffilename);

	return True



# Plot companies
cursor1 = db.cursor()

if (len(sys.argv) > 1):
	symbol = sys.argv[1]
	cursor1.execute("SELECT symbol, sector, subsector FROM companies WHERE symbol='%s'" % symbol)
else:
	cursor1.execute("SELECT symbol, sector, subsector FROM companies")

numcomp = int(cursor1.rowcount)
for c in range(0,numcomp):
	row = cursor1.fetchone()
	symbol = str(row[0])
	sector = str(row[1])
	subsector = str(row[2])
	#print symbol

	sector_name = normalize_filename(sector)
	subsector_name = normalize_filename(subsector)

	imagepath = "graphs/{0}/{1}".format(sector_name, subsector_name)
	if not os.path.isdir(imagepath):
		os.makedirs(imagepath)

	cursor2 = db.cursor()
	cursor2.execute("SELECT p.date_, p.price, c.sector, c.subsector FROM prices p inner join companies c on p.symbol=c.symbol where c.symbol='%s'" % symbol)
	plot_graph(cursor2, sector_name, subsector_name, symbol)





# Plot sectors
cursor1.execute("select distinct sector from companies")

numsectors = int(cursor1.rowcount)
for c in range(0,numsectors):
	sectorrow = cursor1.fetchone()
	sector = sectorrow[0]

	# Plot subsectors
	cursor2.execute("select distinct subsector from companies")

	numsectors = int(cursor2.rowcount)
	for c in range(0,numsectors):
		subsectorrow = cursor2.fetchone()
		subsector = subsectorrow[0]
		#print subsector

		sector_name = normalize_filename(sector)
		subsector_name = normalize_filename(subsector)

		cursor3 = db.cursor()
		cursor3.execute("select date_, total from sector_sums_by_date where sector='{0}' and subsector='{1}'".format(sector, subsector))
		plot_graph(cursor3, sector_name, subsector_name, "0ALL")


