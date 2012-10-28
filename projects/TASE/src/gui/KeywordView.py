#!/usr/bin/env python

import pygtk
pygtk.require('2.0')
import gtk

import MySQLdb

import dbconn


class KeywordView(gtk.ScrolledWindow):
    def __init__(self):
        gtk.ScrolledWindow.__init__(self)
        self.liststore = gtk.ListStore(str, int, int)
        self.tree = gtk.TreeView(self.liststore)
        for idx, column in enumerate(['Name', 'x', 'y']):
            col = gtk.TreeViewColumn(column, gtk.CellRendererText(), text=idx)
            self.tree.append_column(col)
        self.tree.connect('row_activated', self.row_activated)
        treeselection = self.tree.get_selection()
        treeselection.set_mode(gtk.SELECTION_SINGLE)
        self.set_shadow_type(gtk.SHADOW_ETCHED_IN)
        self.set_policy(gtk.POLICY_NEVER, gtk.POLICY_AUTOMATIC)
        self.add(self.tree)

        self.db = MySQLdb.connect(host=dbconn.DB_HOST, user=dbconn.DB_USERNAME, passwd=dbconn.DB_PASSWORD, db=dbconn.DB_SCHEMA)

    def row_activated(self, treeview, path, column):
        pass

    def load(self, symbol, date=None):
        cursor = self.db.cursor()
        sql = "select k.keyword, c.x, c.y from keywords k inner join company_keywords c on c.keyword_id=k.id where c.symbol='%s'" % symbol
        try:
            cursor.execute(sql)
            self.load_cursor(cursor)
        except MySQLdb.ProgrammingError, e:
            print 'SQL error: %s' % e

    def load_cursor(self, cursor):
        for row in cursor:
            self.liststore.append([row[0], row[1], row[2]])
        self.liststore.append(['', 0, 0])

    def clear(self):
        self.liststore.clear()
