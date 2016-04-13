#! /usr/bin/python

fp = open('input.txt','r')
linenumber = 0
while True:
    line = fp.readline()
    if not line:
        break
    try:
        value = int(line)
    except:
        print 'convert to int error'
    if value==0:
        print '0 found at line %d' %linenumber
        break
#    print value
    linenumber+=1

