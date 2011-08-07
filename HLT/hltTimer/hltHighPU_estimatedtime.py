#!/usr/bin/env python
############################################################################
# hltHighPU_estimatedtime.py
# python script created by Alejandro Gomez
# email: alejandro.gomez@cern.ch
#
# Estimate the time per module in each path for two different root files,
# (this time was made of high pile up events). 
# Takes infile1 and creates a dict as [pathname][modname] = meantime
# Takes infile2 and creates a dict as [pathname][modname] = numberofentries
# Takes infile2 and gets max number of entries
# Matches pathname and modname of infile1 and infile2, calculate the frequency
# of each module as numberofentries/maxentries, multiply by meantime to get
# an estimated time per each module and path and creates a new dict as
# [pathname][module] = estimatedtime. 
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
DEBUG = False
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
def get_histos_info_mean(infile):
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
			if not pathname in repo:
				repo[pathname] = {} 
			repo[pathname][modname] = themean
#					if pathname == path :           #store information paths and mods
#						if themean != 0:
#							repo[modname] = themean
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
			for pathname in repo1:
				for modname2, mean in repo1[pathname].iteritems():
					for nitem, modname1 in enumerate(fullname):
		 				if modname1 == modname2:
							if not pathname in repo:
								repo[pathname] = {}
 							repo[pathname][modname1] = nentries
#	print repo
	return repo

###############################################################
def get_maxEvents(infile):
        '''Return max number of events''' 
###############################################################
	f = TFile(infile,"READ")
	dir = f.GetListOfKeys()
	for k in dir:
		h = k.ReadObj()
		name = h.GetName()
		entries = h.GetEntries()
		if name == "totalTime":
			maxentries = entries
 	#print maxentries
	return maxentries 


###############################################################
def get_highPU_time(infile,repo1,repo2):
        '''Return a new estimated time for high pile up events'''
###############################################################
	repo = {}
	events = get_maxEvents(infile)
	for pathname2 in repo2:
		for modname2, nentries in repo2[pathname2].iteritems():
			freq = nentries/events
			for pathname1 in repo1:
				for modname1, mean in repo1[pathname1].iteritems():
					if (pathname1 == pathname2) and (modname1 == modname2):
						newtime = freq*mean 
						if not pathname1 in repo:
							repo[pathname1] = {}
						repo[pathname1][modname1] = newtime
#						print pathname1, modname1, pathname2, modname2 ,freq, newtime
#	print repo
#	for path in repo:
#		for mod, time in repo[path].iteritems():
#			print 'path = ', path, 'module = ',mod,', average time =', mean, ', estimated time =', time
	return repo


###############################################################
def main():
###############################################################
	#check the number of parameter
	numarg = len(sys.argv)
	if numarg < 2:
        	usage()
	        return 1

	infile1 = sys.argv[1]
	infile2 = sys.argv[2]

	#check if input files exist
	if  not(os.path.isfile(infile1)):
		print infile1+" does not exist. Please check."
		sys.exit(1)
	if  not(os.path.isfile(infile2)):
		print infile2+" does not exist. Please check."
		sys.exit(1)

	#get the histos info in containers 
	repo1 = get_histos_info_mean(infile1)
	repo2 = get_histos_info_nentries(infile2,repo1)
	time1 = get_highPU_time(infile2,repo1,repo2)

	#plot results
	#plot_results(repo1,repo2,infile1,infile2)

#######################################################
if __name__ =='__main__':
#######################################################
	sys.exit(main())
