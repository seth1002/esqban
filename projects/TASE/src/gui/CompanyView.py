#!/usr/bin/env python

import pygtk
pygtk.require('2.0')
import gtk

import MySQLdb

import dbconn

col_type = 0
col_sector = 1
col_buy_date = 2
col_buy_price = 3

class CompanyView(gtk.ScrolledWindow):
    def __init__(self, callback):
        gtk.ScrolledWindow.__init__(self)
        self.set_shadow_type(gtk.SHADOW_ETCHED_IN)
        self.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)

        self.column_count = 4
        self.treestore = gtk.TreeStore(str, str, str, str)
        self.tree = gtk.TreeView(self.treestore)
        self.add(self.tree)
        self.tree.connect('cursor-changed', callback)

        treeselection = self.tree.get_selection()
        treeselection.set_mode(gtk.SELECTION_MULTIPLE)

        self.add_column('Sector', col_sector)
        self.add_column('Buy date', col_buy_date)
        self.add_column('Buy price', col_buy_price)

        self.db = MySQLdb.connect(host=dbconn.DB_HOST, user=dbconn.DB_USERNAME, passwd=dbconn.DB_PASSWORD, db=dbconn.DB_SCHEMA)
        
        self.load()

    def clear(self):
        self.treestore.clear()

    def add_column(self, name, index):
        cell = gtk.CellRendererText()
        #cell.set_property('cell-background', 'gray')
        col = gtk.TreeViewColumn(name, cell, text=index)
        self.tree.append_column(col)

    def add_company(self, parent, name, type=''):
        lst = list(name)
        lst.insert(0, type)
        l = len(lst)
        max = self.column_count
        if l > max:
            lst = lst[:max] # Trim
        for i in range(l,max):
            lst.append('')
        try:
            return self.treestore.append(parent, lst)
        except ValueError:
            return None;

    def load(self):
        pairs = (
                 ("Portfolio", "select symbol, date_, purchase_price from portfolio"),
                 ("Watching", "select symbol from watching"),
                 ("NCAV", "select symbol, ncav, percent from ncav where percent between 0.1 and 67 order by percent desc"),
                 ("New Portfolio", "select symbol from new_portfolio"),
                 ("Bonds", "select symbol from companies where category='bond'"),
                 ("Funds", "select symbol from companies where category='fund'"),
                 ("Earnings", "SELECT symbol FROM financial_statements where period='March 2011' order by earnings_per_share desc")
        )
        for pair in pairs:
            self.load_companies_sql(None, pair[0], pair[1])
        self.load_sectors()

    def load_companies_sql(self, parent, name, sql):
        node = self.add_company(parent, [name, '', ''])
        cursor = self.db.cursor()
        try:
            cursor.execute(sql)
            for row in cursor:
                self.add_company(node, row, name)
        except MySQLdb.ProgrammingError, e:
            print 'SQL error: %s' % e

    def load_sectors(self):
        cursor = self.db.cursor()
        sql = "select distinct sector from companies where category='company'"
        try:
            cursor.execute(sql)
            for row in cursor:
                node = self.add_company(None, row)
                self.load_subsectors(node, row[0])
        except MySQLdb.ProgrammingError, e:
            print 'SQL error: %s' % e

    def load_subsectors(self, parent, sector):
        cursor = self.db.cursor()
        sql = "select distinct subsector from companies where sector='%s' and category='company'" % sector
        try:
            cursor.execute(sql)
            for row in cursor:
                node = self.add_company(parent, row)
                self.load_companies(node, sector, row[0])
        except MySQLdb.ProgrammingError, e:
            print 'SQL error: %s' % e

    def load_companies(self, parent, sector, subsector):
        cursor = self.db.cursor()
        sql = "select symbol from companies where sector='{0}' and subsector='{1}' and category='company'".format(sector, subsector)
        try:
            cursor.execute(sql)
            for row in cursor:
                self.add_company(parent, row)
        except MySQLdb.ProgrammingError, e:
            print 'SQL error: %s' % e
