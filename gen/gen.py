#! /usr/bin/python

import random

CASENUM = 300000000
MAX_VALUE = 50000000
for i in xrange(CASENUM):
    value = random.randint(0,MAX_VALUE)
    if random.randint(0,1)==0:
        value=-value
    print value
