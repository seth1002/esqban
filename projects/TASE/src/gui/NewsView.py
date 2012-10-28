#!/usr/bin/env python

import pygtk
pygtk.require('2.0')
import gtk

import webbrowser

import MySQLdb

import dbconn


class NewsList(gtk.ScrolledWindow):
    def __init__(self, content):
        gtk.ScrolledWindow.__init__(self)
        self.content = content
        self.liststore = gtk.ListStore(str, str, str, str)
        self.tree = gtk.TreeView(self.liststore)
        for idx, column in enumerate(['Date', 'Headline']):
            col = gtk.TreeViewColumn(column, gtk.CellRendererText(), text=idx)
            self.tree.append_column(col)
        self.tree.connect('row_activated', self.row_activated)
        self.tree.connect('cursor-changed', self.set_cursor)
        treeselection = self.tree.get_selection()
        treeselection.set_mode(gtk.SELECTION_SINGLE)
        self.set_shadow_type(gtk.SHADOW_ETCHED_IN)
        self.set_policy(gtk.POLICY_NEVER, gtk.POLICY_AUTOMATIC)
        self.add(self.tree)

        self.db = MySQLdb.connect(host=dbconn.DB_HOST, user=dbconn.DB_USERNAME, passwd=dbconn.DB_PASSWORD, db=dbconn.DB_SCHEMA)

    def row_activated(self, treeview, path, column):
        treeiter = self.liststore.get_iter(path)
        url = self.liststore.get_value(treeiter, 2)
        webbrowser.open_new_tab(url)

    def set_cursor(self, treeview):
        path = treeview.get_cursor()[0]
        if path == None:
            return
        treeiter = self.liststore.get_iter(path)
        url = self.liststore.get_value(treeiter, 3)
        self.content.set_text(url)

    def load(self, symbol, date=None):
        cursor = self.db.cursor()
        if not date is None:
            sql = "select date_, headline, url, content from company_news where symbol='%s' and date_>DATE('%s') order by date_ desc" % (symbol, date)
        else:
            sql = "select date_, headline, url, content from company_news where symbol='%s' order by date_ desc" % symbol
        try:
            cursor.execute(sql)
            self.load_cursor(cursor)
        except MySQLdb.ProgrammingError, e:
            print 'SQL error: %s' % e

    def load_cursor(self, cursor):
        for row in cursor:
            self.liststore.append([row[0], row[1], row[2], row[3]])

    def clear(self):
        self.liststore.clear()


class NewsContents(gtk.ScrolledWindow):
    def __init__(self):
        gtk.ScrolledWindow.__init__(self)
        self.buffer = gtk.TextBuffer()
        self.textview = gtk.TextView(self.buffer)
        self.add(self.textview)

    def clear(self):
        start, end = self.buffer.get_bounds()
        self.buffer.delete(start, end)

    def set_text(self, text):
        self.clear()
        #self.buffer = gtk.TextBuffer()
        #self.buffer.set_text(text.encode("utf-8"))
        self.buffer.set_text(text)


class NewsView(gtk.VPaned):
    def __init__(self):
        gtk.VPaned.__init__(self)

        self.content = NewsContents()
        self.list = NewsList(self.content)

        self.pack1(self.list, shrink=True)
        self.pack2(self.content, shrink=True)
        
        self.set_position(200)

    def load(self, symbols, dates=None):
        self.list.load(symbols, dates)

    def clear(self):
        self.list.clear()
        self.content.clear()
