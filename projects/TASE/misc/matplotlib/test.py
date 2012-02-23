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

        vbox = gtk.VBox(False, 8)
        #self.add(vbox)

        vpan = gtk.VPaned()
        self.add(vpan)

        sw = gtk.ScrolledWindow()
        sw.set_shadow_type(gtk.SHADOW_ETCHED_IN)
        sw.set_policy(gtk.POLICY_NEVER, gtk.POLICY_AUTOMATIC)
        vbox.pack_start(sw, True, True)

        button_execute = gtk.Button(label='Execute', stock=None)
        button_execute.connect('clicked', self.execute_sql, "execute sql")
        vbox.pack_start(button_execute, False, False)

        self.buff = gtk.TextBuffer()
        query = open("query.txt", 'r').read()
        if len(query) > 0:
            self.buff.set_text(query)
        textview = gtk.TextView(self.buff)
        sw.add(textview)

        vpan.pack1(vbox, shrink=False)

        # matplotlib stuff
        self.fig = Figure(figsize=(6,4))
        self.canvas = FigureCanvas(self.fig)  # a gtk.DrawingArea
        #vbox.pack_start(self.canvas, True, True)
        vpan.pack2(self.canvas, shrink=False)

        vpan.show()

    def on_destroy(self, widget):
        start, end = self.buff.get_bounds()
        query = self.buff.get_text(start, end)
        if len(query) > 0:
            open("query.txt", 'w').write(query + '\n')
        gtk.main_quit()

    def execute_sql(self, widget, data=None):
        self.fig.clear()
        start, end = self.buff.get_bounds()
        query = self.buff.get_text(start, end)
        for sql in query.split(';'):
            sql = sql.strip()
            if len(sql) > 0:
                cursor = self.db.cursor()
                try:
                    cursor.execute(sql)
                    self.process_results(cursor, sql)
                except MySQLdb.ProgrammingError, e:
                    print 'SQL error: %s' % e

    def process_results(self, cursor, sql):
        x = []
        y = []
        for row in cursor:
            x.append(row[0])
            y.append(row[1])
        ax = self.fig.add_subplot(111)
        line = ax.plot(x, y, label=sql)
        ax.legend(loc='best', prop = FontProperties(size='9'))
        self.canvas.draw()

manager = DataManager()
manager.show_all()
gtk.main()

