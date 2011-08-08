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
THRATIO = 10.0
#threshold for the difference of the running modules, used
#when one instance is zero
THDIFF = 100

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
	repo_means = {}
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
			if not pathname in repo_means:
				repo_means[pathname] = {} 
			repo_means[pathname][modname] = themean
#					if pathname == path :           #store information paths and mods
#						if themean != 0:
#							repo[modname] = themean
#	print repo
	return repo_means;


            
###############################################################
def get_histos_info_nominal(infile,repo_means):
        '''Return number of entries of modules for an specific path
	   and the max number of events'''
###############################################################
        repo_nentries = {}
        repo_pathtime= {}
        f = TFile(infile,"READ")
        dir = f.GetListOfKeys()
        for k in dir:
                h = k.ReadObj()
                allnames = h.GetName()
                nentries = h.GetEntries()
		mean = h.GetMean()
                ishist = allnames.split("_").count("moduleScaledTime")
		if ishist > 0:
			fullname = allnames.split("_")
			fullname.remove("moduleScaledTime")
			for pathname in repo_means:
				for modname2, mean in repo_means[pathname].iteritems():
					for nitem, modname1 in enumerate(fullname):
		 				if modname1 == modname2:
							if not pathname in repo_nentries:
								repo_nentries[pathname] = {}
 							repo_nentries[pathname][modname1] = nentries

		#### For total number of events
		if allnames == "totalTime":
                        maxentries = nentries

		#### For mean path time 
		ishist_path = allnames.split("_").count("pathTime")
                if ishist_path > 0:                                          #get only the running modules in path
                        fullname_path = allnames.split("_")
                        fullname_path.remove("pathTime")
                        pathname_path = ""                                   #form path name
                        for j in range(len(fullname_path)):
                                pathname_path = pathname_path + fullname_path[j]
                                if j is not (len(fullname_path)-1):
                                        pathname_path = pathname_path + "_"
                        if not pathname_path in repo_pathtime:
                                repo_pathtime[pathname_path] = mean

        #print maxentries
	print repo_pathtime
	return repo_nentries, repo_pathtime, maxentries


###############################################################
def get_highPU_time(infile,repo_means,repo_nentries,maxEvents):
        '''Return a new estimated time for high pile up events'''
###############################################################
	repo_newtime = {}
	repo_newpathtime = {}
	for pathname_nentries in repo_nentries:
		for modname_nentries, nentries in repo_nentries[pathname_nentries].iteritems():
			freq = nentries/maxEvents
			for pathname_means in repo_means:
				for modname_means, mean in repo_means[pathname_nentries].iteritems():
					if (pathname_nentries == pathname_means) and (modname_means == modname_nentries):
						newtime = freq*mean 
						if not pathname_nentries in repo_newtime:
							repo_newtime[pathname_nentries] = {}
						repo_newtime[pathname_nentries][modname_nentries] = newtime

	## Add new estimated time per modules for each path
	for pathname_newtime, modname_newtime in repo_newtime.iteritems():
 		repo_newpathtime[pathname_newtime] = sum(modname_newtime.values())
	print repo_newpathtime
#	print repo
#	for path in repo:
#		for mod, time in repo[path].iteritems():
#			print 'path = ', path, 'module = ',mod,', average time =', mean, ', estimated time =', time
	return repo_newtime, repo_newpathtime


###############################################################
def main():
###############################################################
	#check the number of parameter
	#numarg = len(sys.argv)
	#if numarg < 2:
        ##	usage()
	#        return 1

	infile1 = sys.argv[1]
	#infile2 = sys.argv[2]

	#check if input files exist
	#if  not(os.path.isfile(infile1)):
	#	print infile1+" does not exist. Please check."
#		sys.exit(1)
#	if  not(os.path.isfile(infile2)):
#		print infile2+" does not exist. Please check."
#		sys.exit(1)

	#get the histos info in containers 
	repo1 = get_histos_info_mean(infile1)
	repo2, repo3, nentries = get_histos_info_nominal(infile1,repo1)
	time1, repo4 = get_highPU_time(infile1,repo1,repo2, nentries)
	#repo3 = get_pathtime_mean(infile1)

	#plot results
	#plot_results(repo1,repo2,infile1,infile1)
	#plot_results(repo3,repo4)

#######################################################
if __name__ =='__main__':
#######################################################
	sys.exit(main())
