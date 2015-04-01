#!/usr/bin/python

res = {}
lines = open("data").readlines()
for line in lines:
    data = int(line.strip())
    if res.has_key(data):
        res[data] += 1
    else:
        res[data] = 1

n = len(lines)
for k in res.keys():
    ratio = res[k]/float(n)
    if ratio >= 0.028:
        print k, res[k]/float(n)
    
