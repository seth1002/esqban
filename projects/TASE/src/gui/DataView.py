#!/usr/bin/env python

import pygtk
pygtk.require('2.0')
import gtk

from GraphView import GraphView
from InfoView import InfoView
from KeywordView import KeywordView
from NewsView import NewsView


class DataView(gtk.Notebook):
    def __init__(self):
        gtk.Notebook.__init__(self)

        self.set_tab_pos(gtk.POS_TOP)
        
        self.graph_view = GraphView()
        self.graph_view.show()
        self.add('Graphs', self.graph_view)

        self.info_view = InfoView()
        self.info_view.show()
        self.add('Info', self.info_view)
        
        self.keyword_view = KeywordView()
        self.keyword_view.show()
        self.add('Keywords', self.keyword_view)
        
        self.news_view = NewsView()
        self.news_view.show()
        self.add('News', self.news_view)
        
        #self.connect('select-page', self.selectPage)
        self.connect('switch-page', self.switchPage)
        
        self.symbol = None
        self.date = None

        #self.show()
        
    #def selectPage(self, notebook, page, pageNum):
    #    index = self.current_page()

        
    def switchPage(self, notebook, page, pageNum):
        #index = self.current_page()
        if self.symbol is None:
            return
        real_page = self.get_nth_page(pageNum)
        real_page.clear()
        real_page.load(self.symbol, self.date)


    def clear(self):
        self.graph_view.clear()
        self.info_view.clear()
        self.keyword_view.clear()
        self.news_view.clear()

    def add(self, txt, ctrl):
        label = gtk.Label(txt)
        self.append_page(ctrl, label)

    def load(self, symbol, date=None):
        self.symbol = symbol
        self.date = date
        #self.info_view.load(symbols, dates)
        #self.keyword_view.load(symbols, dates)
        #self.news_view.load(symbols, dates)
        index = self.current_page()
        real_page = self.get_nth_page(index)
        real_page.load(symbol, date)

