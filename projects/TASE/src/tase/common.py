#!/usr/bin/env python

import re
import datetime


def log2(msg):
    open("0log.txt", 'ab').write(msg + '\n')

def unescape(s):
    s = get_string(s)
    s = s.replace("&lt;", "<")
    s = s.replace("&gt;", ">")
    # this has to be last:
    s = s.replace("&amp;", "AND")
    s = s.replace(" AND ", " & ")
    s = s.replace(u'\xa0', u'')
    return s

def normalize_number(s):
    s = get_string(s)
    s = s.replace(u'\xa0', u'')
    s = s.replace("\"", "")
    s = s.replace("%", "")
    s = s.replace(",", "")
    s = s.replace(" ", "")
    s = s.replace("---", "")
    return s

def to_float(s):
    s = normalize_number(s)
    try:
        return float(s)
    except ValueError:
        return 0

def to_int(s):
    s = normalize_number(s)
    try:
        return int(s)
    except ValueError:
        return 0

def to_long(s):
    s = normalize_number(s)
    try:
        return long(s)
    except ValueError:
        return 0

def get_text(hxs, path):
    l = hxs.xpath(path).extract()
    return get_string(l)

def get_date(s):
    s = get_string(s)
    regex = "([0]?[1-9]|[1|2][0-9]|[3][0|1])[./-]([0]?[1-9]|[1][0-2])[./-]([0-9]{4}|[0-9]{2})"
    m = re.search(regex, s)
    if not m is None:
        yy = int(m.group(3))
        if yy < 100:
            yy += 2000
        mm = int(m.group(2))
        dd = int(m.group(1))
        res = datetime.date(yy, mm, dd)
    else:
        res = datetime.date(0, 0, 0)
    return res.isoformat()

def get_date2(s):
    s = get_string(s)
    res = datetime.datetime.strptime(s, "%d %b %y")
    return res.isoformat()

def get_date3(s):
    s = get_string(s)
    res = datetime.datetime.strptime(s, "%d/%m/%Y %H:%M")
    return res.isoformat()

def is_sequence(arg):
    return (not hasattr(arg, "strip") and
            hasattr(arg, "__getitem__") or
            hasattr(arg, "__iter__"))

def get_string(s):
    if is_sequence(s):
        if len(s) > 0:
            return s[0].strip()
        else:
            return ''
    else:
        return s.strip()

