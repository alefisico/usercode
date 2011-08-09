#!/usr/bin/env python
############################################################################

# hltHighPU_estimatedtime.py
# python script created by Alejandro Gomez
# email: alejandro.gomez@cern.ch
#
# 1. Estimate the time per module in each path for two different root files,
#    (this time was made for high pile up events). 
# 2. Takes two root files: one with nominal events (1) and the other with high
#    pile up events (2).
# 3. From (2) takes paths-modules and mean time for each.
# 4. From (1) takes number events, max number events and time path.
# 5. Matches paths and for each path freq = (number of events)/(max events).
# 6. Multiplies freq*mean time from (2) = estimated time.
# 7. Returns two repos (dictionaties) with pathnames:meantime and pathnames:estimatedtime
# 8. This script now takes some paths from pathlist, but is easy to modify for the whole menu.
# 9. Plots the difference between nominal time and estimated time.

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
# This function is for High Pile Up skims
###############################################################
	repo_means = {}
	f = TFile(infile,"READ")
	dir = f.GetListOfKeys()
	for k in dir:
		h = k.ReadObj()
		allnames = h.GetName()
		themean = h.GetMean()
		ishist = allnames.split("_").count("moduleInPathScaledTime")	#get only the running modules in path
		if ishist > 0: 
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
#	print repo_means
	return repo_means;


            
###############################################################
def get_histos_info_nominal(infile,repo_means):
        '''Return number of entries of modules for an specific path
	   and the max number of events'''
# This is for a nominal skim
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
                if ishist_path > 0:
                        fullname_path = allnames.split("_")
                        fullname_path.remove("pathTime")
                        pathname_path = "" 
                        for j in range(len(fullname_path)):
                                pathname_path = pathname_path + fullname_path[j]
                                if j is not (len(fullname_path)-1):
                                        pathname_path = pathname_path + "_"
                        if not pathname_path in repo_pathtime:
                                repo_pathtime[pathname_path] = mean

#       print maxentries
#	print repo
	return repo_nentries, repo_pathtime, maxentries


###############################################################
def get_highPU_time(repo_means,repo_nentries,maxEvents):
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
#	print repo_newpathtime
#	print repo_newtime
#	for path in repo_newtime:
#		for mod, time in repo_newtime[path].iteritems():
#			print 'path = ', path, 'module = ',mod,', average time =', mean, ', estimated time =', time
	return repo_newtime, repo_newpathtime

###############################################################
def plot_results(repo_PathTime, repo_estimatedPathTime, pathlist):#, file1, file2):
###############################################################

#	fname1 = file1.strip(".root")
#	fname2 = file2.strip(".root")
	h1 = TH1F("h1","Estimated Path Time for High Pile Up Events", len(pathlist),0,len(pathlist))
	h2 = TH1F("h2", " ", len(pathlist),0,len(pathlist))

	maxmean = 0
	for path in pathlist:
		for pathname1, time1 in repo_PathTime.iteritems():
			if path == pathname1:
				h1.Fill(path, time1)
#				print path, time1
		for pathname2, time2 in repo_estimatedPathTime.iteritems():
			if path == pathname2:
				h2.Fill(path, time2) 
				if time2 > maxmean:
					maxmean = time2
#				print path, time2

	#draw the two histos and save the plot
	can = TCanvas("can","",800,600);
	can.cd();
	h1.Draw();
	h1.SetLineWidth(2)
	h1.SetMaximum(maxmean+3)
	h1.GetYaxis().SetTitle("msec")
	h1.GetXaxis().SetLabelSize(0.03)
	h2.SetLineColor(2);
	h2.Draw("same")
	h2.SetLineWidth(2)
	can.SetBottomMargin(0.25)
#	can.SetLogy()
	can.SetGridx()
	can.SetGridy()
	lg = TLegend(0.79, 0.89, 1.0, 0.99);
	lg.AddEntry(h1,"Nominal" ,"L");
	lg.AddEntry(h2,"High Pile Up","L");
	lg.SetTextSize(0.025);
	lg.SetBorderSize(0);
	lg.SetFillColor(0);
	lg.Draw("same");
	can.SaveAs("hltHighPU_estimatedtime.png")
	del can


###############################################################
def main():
###############################################################
	#check the number of parameter
	numarg = len(sys.argv)
	if numarg < 2:
        	usage()
	        return 1

	print "\nPython script for estimated High Pile Up time"
	print "For more info, please contact:"
	print "Alejandro Gomez"
	print "email: alejandro.gomez@cern.ch\n"

	infile1 = sys.argv[1]
	print infile1+ " is the High Pile Up Skim file "
	infile2 = sys.argv[2]
	print  infile2+ " is the Nominal Skim file\n"

	#check if input files exist
	if  not(os.path.isfile(infile1)):
		print infile1+" does not exist. Please check."
		sys.exit(1)
	if  not(os.path.isfile(infile2)):
		print infile2+" does not exist. Please check."
		sys.exit(1)

	#get the histos info in containers 
	pathlist = ['HLT_Ele32_WP70_PFMT50_v3',
		    'HLT_Mu40_v5',
		    'HLT_Jet300_v5',
		    'HLT_Photon26_IsoVL_Photon18_IsoVL_v7',
		    'HLT_HT300_MHT80_v2']
	
	print "This process takes a while.... please be patient\n"

	repo1 = get_histos_info_mean(infile1)
	repo2, repo3, nentries = get_histos_info_nominal(infile2,repo1)
	time1, repo4 = get_highPU_time(repo1,repo2, nentries)

	#plot results
	plot_results(repo3,repo4, pathlist)
	print "Final plot is hltHighPU_estimatedtime.png\n"

#######################################################
if __name__ =='__main__':
#######################################################
	sys.exit(main())
