#!/usr/bin/env python

import pygtk
pygtk.require('2.0')
import gtk

import webbrowser

import MySQLdb

import dbconn


class InfoView(gtk.ScrolledWindow):
    def __init__(self):
        gtk.ScrolledWindow.__init__(self)
        self.set_shadow_type(gtk.SHADOW_ETCHED_IN)
        self.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
        self.set_shadow_type(gtk.SHADOW_ETCHED_IN)
        self.liststore = gtk.ListStore(str, str)
        self.tree = gtk.TreeView(self.liststore)
        for idx, column in enumerate(['Name', 'Value']):
            col = gtk.TreeViewColumn(column, gtk.CellRendererText(), text=idx)
            self.tree.append_column(col)
#        try:
#            cursor = self.db.cursor()
#            sql = "select * from companies limit 1"
#            cursor.execute(sql)
#            for idx, column in enumerate(cursor.description):
#                col = gtk.TreeViewColumn(column[0], gtk.CellRendererText(), text=0)
#                self.listview.append_column(col)
#        except MySQLdb.ProgrammingError, e:
#            print 'SQL error: %s' % e
        self.tree.connect('row_activated', self.row_activated)
        treeselection = self.tree.get_selection()
        treeselection.set_mode(gtk.SELECTION_SINGLE)
        self.add(self.tree)

        self.db = MySQLdb.connect(host=dbconn.DB_HOST, user=dbconn.DB_USERNAME, passwd=dbconn.DB_PASSWORD, db=dbconn.DB_SCHEMA)

    def load(self, symbol, date=None):
        cursor = self.db.cursor()
        sql = "select * from symbol_data where symbol='%s'" % symbol
        try:
            cursor.execute(sql)
            self.load_cursor(cursor)
        except MySQLdb.ProgrammingError, e:
            print 'SQL error: %s' % e

    def load_cursor(self, cursor):
        for row in cursor:
            for idx, column in enumerate(cursor.description):
                name = column[0]
                value = "%s"%row[idx]
                self.liststore.append([name, value])
        #self.get_keywords(symbol)
        #self.liststore.append(['', ''])
    
    def row_activated(self, treeview, path, column):
        treeiter = self.liststore.get_iter(path)
        url = self.liststore.get_value(treeiter, 1)
        webbrowser.open_new_tab(url)

    def clear(self):
        self.liststore.clear()
