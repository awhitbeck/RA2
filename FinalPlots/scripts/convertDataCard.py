#!/usr/bin/env python

import sys, getopt

# Read command-line arguments
try:
    opts, args = getopt.getopt(sys.argv[1:],"i:")
except getopt.GetoptError:
    print 'ERROR'
    print 'usage: convertDataCard.py -i <inputfile>'
    sys.exit(2)

inputFile = ""

# Parse command-line arguments
for opt, arg in opts:
    if opt == "-i" :
        inputFile = arg

file = open(inputFile,"r")
lines = file.readlines()
file.close()

nbins = 0
sample = ""
for line in lines:
    keys = line.split()
    if len(keys) > 2:
        if keys[0] == "channels":
            nbins = int(keys[2])
        if keys[0] == "sample":
            sample = keys[2]

print "Converting datacard for sample '"+sample+"' with "+str(nbins)+" search bins"

yields = []
stat = []
systDn = []
systUP = []
for line in lines:
    keys = line.split()
    if len(keys) == nbins+2:
        if keys[0] == sample+"_events":
            yields = keys
        elif keys[0] == sample+"_uncertainty_1":
            stat = keys
        elif keys[0] == sample+"_uncertaintyDN_2":
            systDn = keys
        elif keys[0] == sample+"_uncertaintyUP_2":
            systUp = keys

outputFile = sample+"_searchBinPredictions.txt"
file = open(outputFile,"w")
file.write("# <yield> <stat> <systdn> <systup>\n")
for i in range(2,len(yields)):
    file.write(yields[i]+"   "+stat[i]+"   "+systDn[i]+"   "+systUp[i]+"\n")
file.close()

print "Done. Wrote file '"+outputFile+"'"

