#!/usr/bin/env python
"""
Example of embedding matplotlib in an application and interacting with
a treeview to store data.  Double click on an entry to update plot
data

"""
import pygtk
pygtk.require('2.0')
import gtk
from gtk import gdk

import matplotlib
matplotlib.use('GTKAgg')  # or 'GTK'
from matplotlib.backends.backend_gtk import FigureCanvasGTK as FigureCanvas

from numpy.random import random
from matplotlib.figure import Figure
from matplotlib.font_manager import FontProperties

import matplotlib.lines as lines

import os
import sys
import MySQLdb


class DataManager(gtk.Window):

    def __init__(self):
        self.db = MySQLdb.connect(host="localhost", user="root", passwd="", db="tase")

        gtk.Window.__init__(self)
        self.set_default_size(600, 600)
        self.connect('destroy', self.on_destroy)

        self.set_title('TASE')
        self.set_border_width(8)

#        hbox = gtk.HBox(True, 8)
#        self.add(hbox)

        sw = gtk.ScrolledWindow()
        sw.set_shadow_type(gtk.SHADOW_ETCHED_IN)
        sw.set_policy(gtk.POLICY_NEVER, gtk.POLICY_AUTOMATIC)
#        hbox.pack_start(sw, True, True)

        self.treestore = gtk.TreeStore(str)
        self.treeview = gtk.TreeView(self.treestore)
        col = gtk.TreeViewColumn('Sector', gtk.CellRendererText(), text=0)
        self.treeview.append_column(col)
#        col = gtk.TreeViewColumn('Subsector', gtk.CellRendererText(), text=1)
#        self.treeview.append_column(col)
#        col = gtk.TreeViewColumn('Symbol', gtk.CellRendererText(), text=2)
#        self.treeview.append_column(col)
        #self.cell = gtk.CellRendererText()
        #self.tvcolumn.pack_start(self.cell, True)
        #self.tvcolumn.add_attribute(self.cell, 'text', 0)
        #self.treeview.set_search_column(0)
        #self.treeview.connect('row-activated', self.row_activated)
        self.treeview.connect('cursor-changed', self.set_cursor)
        sw.add(self.treeview)
        #self.treeview.show()

        self.load_sectors()

        # matplotlib stuff
        self.fig = Figure(figsize=(6,4))
        self.canvas = FigureCanvas(self.fig)  # a gtk.DrawingArea
#        hbox.pack_start(self.canvas, True, True)

        hpan = gtk.HPaned()
        self.add(hpan)
        hpan.show()
        hpan.pack1(sw, shrink=False)
        hpan.pack2(self.canvas, shrink=False)

    def load_sectors(self):
        cursor = self.db.cursor()
        sql = "select distinct sector from companies where category='company'"
        try:
            cursor.execute(sql)
            for row in cursor:
                sector = self.treestore.append(None, [row[0]])
                self.load_subsectors(sector, row[0])
        except MySQLdb.ProgrammingError, e:
            print 'SQL error: %s' % e
        #self.treeview.expand_all()

    def load_subsectors(self, parent, sector):
        cursor = self.db.cursor()
        sql = "select distinct subsector from companies where sector='%s' and category='company'" % sector
        try:
            cursor.execute(sql)
            for row in cursor:
                subsector = self.treestore.append(parent, [row[0]])
                self.load_companies(subsector, sector, row[0])
        except MySQLdb.ProgrammingError, e:
            print 'SQL error: %s' % e
        #self.treeview.expand_all()

    def load_companies(self, parent, sector, subsector):
        cursor = self.db.cursor()
        sql = "select symbol from companies where sector='{0}' and subsector='{1}' and category='company'".format(sector, subsector)
        try:
            cursor.execute(sql)
            for row in cursor:
                self.treestore.append(parent, [row[0]])
        except MySQLdb.ProgrammingError, e:
            print 'SQL error: %s' % e
        #self.treeview.expand_all()

    #def row_activated(self, treeview, path, column):
    def set_cursor(self, treeview):
        #print path
        #print column.get_title()
        #value = self.treestore.get_value(iter, column)
        path, col = self.treeview.get_cursor ()
        if path == None:
            return
        model = self.treeview.get_model ()
        treeiter = model.get_iter (path)
        self.fig.clear()
#        model = self.treeview.get_model()
#        try:
#            treeiter = model.get_iter(path)
#        except ValueError:
#            return
        value = model.get_value(treeiter, 0)
        #print value
        cursor = self.db.cursor()
        sql = "select date_, adjusted_closing_price/100 from company_prices where symbol='%s'"%value
        try:
            cursor.execute(sql)
            self.process_results(cursor, value)
        except MySQLdb.ProgrammingError, e:
            print 'SQL error: %s' % e

    def process_results(self, cursor, name):
        x = []
        y = []
        for row in cursor:
            x.append(row[0])
            y.append(row[1])
        ax = self.fig.add_subplot(111)
        line = ax.plot(x, y, label=name)
        ax.legend(loc='best', prop = FontProperties(size='9'))
        self.canvas.draw()

    def on_destroy(self, widget):
        gtk.main_quit()

manager = DataManager()
manager.show_all()
gtk.main()

