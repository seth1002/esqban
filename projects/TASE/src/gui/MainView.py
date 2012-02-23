#!/usr/bin/env python

import pygtk
pygtk.require('2.0')
import gtk

from CompanyView import CompanyView
from DataView import DataView

class MainView(gtk.HPaned):
    def __init__(self, callback):
        gtk.HPaned.__init__(self)

        self.company_view = CompanyView(callback);
        self.company_view.show()
        self.pack1(self.company_view, shrink=False)
        
        self.data_view = DataView()
        self.data_view.show()
        self.pack2(self.data_view, shrink=False)
        
        self.set_position(300)

    def clear(self):
        self.data_view.clear()

    def load(self, symbols, dates=None):
        self.data_view.load(symbols, dates)
