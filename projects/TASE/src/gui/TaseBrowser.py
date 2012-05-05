#!/usr/bin/env python
"""
Example of embedding matplotlib in an application and interacting with
a treeview to store data.  Double click on an entry to update plot
data

"""
import pygtk
pygtk.require('2.0')
import gtk
#from gtk import gdk

import MySQLdb

#import time, datetime

from MainView import MainView
import CompanyView


class TaseBrowser(gtk.Window):

    def __init__(self):
        gtk.Window.__init__(self)

        self.set_default_size(1024, 768)
        self.connect('destroy', self.on_destroy)
        self.set_title('TASE')
        self.set_border_width(8)

        self.db = MySQLdb.connect(host="localhost", user="sqba", db="tase")
        
        self.main_view = MainView(self.select_symbol)
        self.add(self.main_view)
        self.main_view.show()

    def select_symbol(self, treeview):
        self.main_view.clear()
        path = treeview.get_cursor()
        if path == None:
            return
        selection = treeview.get_selection()
        (model, pathlist) = selection.get_selected_rows()
        iters = [model.get_iter(path) for path in pathlist]
        for treeiter in iters:
            type = model.get_value(treeiter, CompanyView.col_type)
            symbol = model.get_value(treeiter, CompanyView.col_sector)
            date = model.get_value(treeiter, CompanyView.col_buy_date)
            if type == "Portfolio":
                self.main_view.load(symbol, date)
            else:
                self.main_view.load(symbol)

    def on_destroy(self, widget):
        gtk.main_quit()


browser = TaseBrowser()
browser.show_all()
gtk.main()

