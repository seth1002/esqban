#!/usr/bin/env python

import pygtk
pygtk.require('2.0')
import gtk

import matplotlib
matplotlib.use('GTKAgg')  # or 'GTK'
from matplotlib.backends.backend_gtk import FigureCanvasGTK as FigureCanvas
from matplotlib.dates import YearLocator, MonthLocator, DateFormatter
from matplotlib.backends.backend_gtkagg import NavigationToolbar2GTKAgg as NavigationToolbar

#from numpy.random import random
from matplotlib.figure import Figure
from matplotlib.font_manager import FontProperties

import time, datetime

import MySQLdb

import dbconn

#from pylab import *

class Graph(gtk.Frame):
    def __init__(self, label):
        self.plots = dict()
        gtk.Frame.__init__(self)
        self.set_label(label)
        self.fig = Figure(figsize=(6,4))
        self.canvas = FigureCanvas(self.fig)  # a gtk.DrawingArea
        #self.add(self.canvas)
        vbox = gtk.VBox()
        vbox.pack_start(self.canvas)
        toolbar = NavigationToolbar(self.canvas, self)
        vbox.pack_start(toolbar, False, False)
        self.add(vbox)
    def clear(self):
        self.fig.clear()
        self.plots.clear()
    def draw(self, symbol, dates, values):
        years    = YearLocator()   # every year
        months   = MonthLocator()  # every month
        yearsFmt = DateFormatter('%Y')
        #monthFmt = DateFormatter('%m')

#        coeffs = polyfit(dates, values, 3)
#        besty = polyval(coeffs, dates)
        
        ax = self.fig.add_subplot(111)
        ax.plot(dates, values, label=symbol)
        ax.legend(loc='best', prop = FontProperties(size='9'))
        ax.grid(True)
        ax.xaxis.set_major_locator(years)
        ax.xaxis.set_major_formatter(yearsFmt)
        #ax.xaxis.set_minor_formatter(monthFmt)
        ax.xaxis.set_minor_locator(months)
        ax.autoscale_view()
        self.plots[symbol] = ax
        #self.fig1.autofmt_xdate()
        self.canvas.draw()
    def insert_annotations(self, symbol, cursor):
        ax = self.plots[symbol]
        for row in cursor:
            ax.annotate(row[0], xy=(row[1], row[2]))
        self.canvas.draw()
#        ax.annotate('local max', xy=(an[0], an[1]), xytext=(3, 1.5), arrowprops=dict(facecolor='black', shrink=0.05))

class GraphView(gtk.VPaned):
    def __init__(self):
        gtk.VPaned.__init__(self)
        
        self.price_graph = Graph("Price")
        self.pack1(self.price_graph, shrink=True)

        self.volume_graph = Graph("Volume")
        self.pack2(self.volume_graph, shrink=True)
        
        self.set_position(500)

        self.db = MySQLdb.connect(host=dbconn.DB_HOST, user=dbconn.DB_USERNAME, db=dbconn.DB_SCHEMA)

    def clear(self):
        self.price_graph.clear()
        self.volume_graph.clear()

    def load(self, symbol, date=None):
        cursor = self.db.cursor()
        if not date is None:
            sql = "select date_, price, volume from all_prices where symbol='%s' and date_>DATE('%s')" % (symbol, date)
        else:
            sql = "select date_, price, volume from all_prices where symbol='%s'" % symbol
        try:
            cursor.execute(sql)
            self.draw(cursor, symbol)
        except MySQLdb.ProgrammingError, e:
            print 'SQL error: %s' % e

    def draw_news(self, symbol):
        cursor = self.db.cursor()
        sql = "select n.headline, n.date_, p.closing_price from company_news n inner join company_prices p on p.symbol=n.symbol and p.date_=n.date_ where n.symbol='%s'" % symbol
        try:
            cursor.execute(sql)
            self.price_graph.insert_annotations(symbol, cursor)
        except MySQLdb.ProgrammingError, e:
            print 'SQL error: %s' % e

    def draw(self, cursor, symbol):
        date = []
        price = []
        volume = []
        count = 0
        time_format = "%Y-%m-%d"
        for row in cursor:
            d = datetime.datetime.fromtimestamp(time.mktime(time.strptime(row[0], time_format)))
            date.append(d)
            price.append(row[1])
            volume.append(row[2])
            count += 1
        if count == 0:
            self.clear()
            return

        self.price_graph.draw(symbol, date, price)
        self.volume_graph.draw(symbol, date, volume)
        self.draw_news(symbol)
