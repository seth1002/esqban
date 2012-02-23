#!/usr/bin/env python

import os
os.environ.setdefault('SCRAPY_SETTINGS_MODULE', 'tase.settings')

from scrapy.cmdline import execute
execute()

