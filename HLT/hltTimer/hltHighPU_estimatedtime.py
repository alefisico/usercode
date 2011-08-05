#!/usr/bin/env python
############################################################################
#
###########################################################################

"""
   usage: %prog <file1> <file2>
 
"""


import os,sys
import subprocess
import string, re
import fileinput
import commands
from time import gmtime, localtime, strftime
from ROOT import *

#just a debug flag
#DEBUG = False
#threshold for the ratio of the running modules
#THRATIO = 10.0
#threshold for the difference of the running modules, used
#when one instance is zero
#THDIFF = 100

gROOT.SetStyle("Plain")
gStyle.SetOptStat(0)

###############################################################
def usage():
###############################################################

    if (DEBUG):
        print "This is the usage function"
        
    print '\n'
    print 'Usage: '+sys.argv[0]+' <file1> <file2>'
    print 'e.g.:  '+sys.argv[0]+' outputTiming1.root outputTiming2.root\n'



###############################################################
def get_histos_info_mean(infile,path):
	'''Return mean time of modules for an specific path'''
###############################################################
	repo = {}
	f = TFile(infile,"READ")
	dir = f.GetListOfKeys()
	for k in dir:
		h = k.ReadObj()
		allnames = h.GetName()
		themean = h.GetMean()
		ishist = allnames.split("_").count("moduleInPathScaledTime")
		if ishist > 0: 						#get only the running modules in path
			fullname = allnames.split("_")
			fullname.remove("moduleInPathScaledTime")
			pathname = ""                           	#form path name
			modname = fullname[len(fullname)-1]    		#get module name
			for j in range(len(fullname)-1):
				pathname = pathname + fullname[j]
				if j is not (len(fullname)-2):
					pathname = pathname + "_"
					if pathname == path :           #store information paths and mods
						if themean != 0:
							repo[modname] = themean
#	print repo
	return repo;


            
###############################################################
def get_histos_info_nentries(infile,repo1):
        '''Return number of entries of modules for an specific path'''
###############################################################
        repo = {}
        f = TFile(infile,"READ")
        dir = f.GetListOfKeys()
        for k in dir:
                h = k.ReadObj()
                allnames = h.GetName()
                nentries = h.GetEntries()
                ishist = allnames.split("_").count("moduleScaledTime")
		if ishist > 0:
			fullname = allnames.split("_")
			fullname.remove("moduleScaledTime")
			for a, b in enumerate(fullname):
 				for c, d in repo1.iteritems():
	 				if b == c:
						repo[c] = nentries
#	print repo
	return repo



###############################################################
def get_highPU_time(repo1,repo2):
        '''Return a new estimated time for high pile up events'''
###############################################################
	repo = {}
	events = 12365
	for a, b in repo2.iteritems():
		freq = b/events
		for c,d in repo1.iteritems():
			if a == c:
				newtime = freq*d 
				repo[a] = newtime
				print a, b, c, d ,freq, newtime
#	print repo
	for i,j in repo.iteritems():
		print 'module = ',i,', average time =', d, ', estimated time =', j
	return repo


###############################################################
def main():
###############################################################
    #check the number of parameter
    #numarg = len(sys.argv)
    #if numarg < 2:
    #    usage()
    #    return 1

    infile1 = sys.argv[1]
    Path1 = "HLT_Ele32_WP70_PFMT50_" #HLT_Mu40_"
    #infile2 = sys.argv[2]

    #check if input files exist
    #if  not(os.path.isfile(infile1)):
    #    print infile1+" does not exist. Please check."
    #    sys.exit(1)
    #if  not(os.path.isfile(infile2)):
    #    print infile2+" does not exist. Please check."
    #    sys.exit(1)


    #get the histos info in containers 
    repo1 = get_histos_info_mean(infile1,Path1)
    repo2 = get_histos_info_nentries(infile1,repo1)
    freq1 = get_highPU_time(repo1,repo2)
    #plot results
    #plot_results(repo1,repo2,infile1,infile2)

#######################################################
if __name__ =='__main__':
#######################################################
    sys.exit(main())
