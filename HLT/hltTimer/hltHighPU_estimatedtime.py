#!/usr/bin/env python
############################################################################
#
# hltHighPU_estimatedtime.py
# python script created by Alejandro Gomez
# email: alejandro.gomez@cern.ch
#
# This script can perform two differents types of estimated High PU time
# Please, uncomment/comment in the main function for one of those.
# **** This script now takes some paths from pathlist, but is
#                  easy to modify for the whole menu.****
#
######### For the Second Estimated Time
# 1. Takes the number of entries for the high PU and Nominal skims.
# 2. To weight the number of events in HPU, takes the number of entries in
#    the module hltPre+path (i.e. hltPreMu40) for both skims.
# 3. Takes the mean time per module per path in the high PU skim.
# 4. Calculates the estimated time as:
#        mean time * nentries Nominal * nentries in hltPre HPU
#                    nentries HPU       nentries in hltPre Nominal
# 5. Plots the difference in the module times per path between HPU, Nominal
#    and estimated.
#
#
######### For the First Estimated Time
# 1. Estimate the time per module in each path for two different root files,
#    (this time was made for high pile up events). 
# 2. Takes two root files: one with nominal events (2) and the other with high
#    pile up events (1).
# 3. From (1) takes paths-modules and mean time for each.
# 4. From (2) takes number events, max number events and time path.
# 5. Matches paths and for each path weight = (number of events)/(max events).
# 6. Multiplies weight*mean time from (1) = estimated time.
# 7. Returns two repos (dictionaties) with pathnames: mean time and
#    pathnames:estimatedtime
# 8. Plots the difference between nominal time and estimated time.
# 9. Plots the difference in the module times per path between HPU, Nominal
#    and estimated.
#
###########################################################################

"""
   usage: %prog <file1> <file2>
 
"""


import os,sys
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
    print 'Usage: '+sys.argv[0]+' <High Pile Up file> <Nominal file>'
    print 'e.g.:  '+sys.argv[0]+' outputTiming1.root outputTiming2.root\n'


###############################################################
def get_histos_info_mean(infile):
	'''Return mean time of modules for an specific path'''
# This function is for High Pile Up skims
###############################################################
	repo_means = {}
	repo_means_sum = {}
	repo_nentries = {}
	repo_nentries_hltPre = {}  # for hltPre+Pathname
	f = TFile(infile,"READ")
	dir = f.GetListOfKeys()
	for k in dir:
		h = k.ReadObj()
		allnames = h.GetName()
		nentries = h.GetEntries()
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
			##### For mean time per module in each path
                        if not pathname in repo_means:
                                repo_means[pathname] = {}
                        repo_means[pathname][modname] = themean
			##### For number of entries per module in each path
			if not pathname in repo_nentries:
				repo_nentries[pathname] = {}
			repo_nentries[pathname][modname] = nentries
			##### For number of entries in hltPre+path module per path
			if "HLT" in pathname:
				path = ''.join(pathname.split('_')[1:-1])
				if modname == 'hltPre'+path:
					hltPrepath = modname
					if not pathname in repo_nentries_hltPre:
						repo_nentries_hltPre[pathname] = {}
					repo_nentries_hltPre[pathname][hltPrepath] = nentries

	########## To obtain the nentries for hltPre+path module per path 
	########## Actually this part does the same as the former part 
#		for pathname2 in repo_means:
#			for modname2, mean2 in repo_means[pathname2].iteritems():
#				if "HLT" in pathname2:
#					path = ''.join(pathname2.split('_')[1:-1])
#					if modname2 == 'hltPre'+path:
#						hltPrepath = modname2
#						if not pathname2 in repo_nentries_hltPre:
#							repo_nentries_hltPre[pathname2] = {}
#						repo_nentries_hltPre[pathname2][hltPrepath] = nentries
#	print repon_entries
	#######################################################################################


	########## To obtain the path time as a sum of module time
	for pathname_means_sum, modname_means_sum in repo_means.iteritems():
		repo_means_sum[pathname_means_sum] = sum(modname_means_sum.values())
#	print repo_means_sum
	#######################################################################
	return repo_means, repo_nentries, repo_nentries_hltPre, repo_means_sum


            
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
                ishist = allnames.split("_").count("moduleInPathScaledTime")

		#### For nentries for each module
		if ishist > 0:
			fullname = allnames.split("_")
			fullname.remove("moduleInPathScaledTime")
			for pathname in repo_means:
				for modname2, mean in repo_means[pathname].iteritems():
					for nitem, modname1 in enumerate(fullname):
		 				if modname1 == modname2:
							if not pathname in repo_nentries:
								repo_nentries[pathname] = {}
 							repo_nentries[pathname][modname1] = nentries
		#####################################################################################

		#### For total number of events
		if allnames == "totalTime":
                        maxentries = nentries
		#################################

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
		###############################################################
		
#       #print maxentries
#	print repo_pathtime
	return repo_nentries, repo_pathtime, maxentries


###############################################################
def get_highPU_time(repo_means,repo_nentries,maxEvents):
        '''Return a new estimated time for high pile up events'''
#  This is the first try to get a HPU estimated time
###############################################################
	repo_newtime = {}
	repo_newpathtime = {}
	for pathname_nentries in repo_nentries:
		for modname_nentries, nentries in repo_nentries[pathname_nentries].iteritems():
			weight = nentries/maxEvents
			for pathname_means in repo_means:
				for modname_means, mean in repo_means[pathname_means].iteritems():
					if (pathname_nentries == pathname_means) and (modname_means == modname_nentries):
						newtime = weight*mean 
						if not pathname_nentries in repo_newtime:
							repo_newtime[pathname_nentries] = {}
						repo_newtime[pathname_nentries][modname_nentries] = newtime

	################# Add new estimated time per modules for each path
	for pathname_newtime, modname_newtime in repo_newtime.iteritems():
 		repo_newpathtime[pathname_newtime] = sum(modname_newtime.values())
	##########################################################################
#	#print repo_newpathtime
#	#print repo_newtime
#	for path in repo_newtime:
#		for mod, time in repo_newtime[path].iteritems():
#			#print 'path = ', path, 'module = ',mod,', average time =', mean, ', estimated time =', time
	return repo_newtime, repo_newpathtime


##############################################################
def get_HPU_time(repo_means1,repo_nentries1, repo_nentries_hltPre1, repo_nentries2, repo_nentries_hltPre2):
	''' New way of estimated the time event '''
#############################################################
	repo_HPU = {}
	repo_mod_ratio = {}
	repo_hltPre_ratio = {}
	for pathname_mean in repo_means1:
		hltPre_ratio = 0
		mod_ratio = 0
		estimated_time = 0

		############# Calculates the ratio between number of entries for Nominal and HPU
		if (pathname_mean in repo_nentries1) and (pathname_mean in repo_nentries2):
			for modname1, nentries1 in repo_nentries1[pathname_mean].iteritems():
				for modname2, nentries2 in repo_nentries2[pathname_mean].iteritems():
					if (nentries1 == 0) or (nentries2 ==0):
						mod_ratio = 1
					else:
						mod_ratio = nentries2/nentries1
				if not pathname_mean in repo_mod_ratio:
					repo_mod_ratio[pathname_mean] = {}
				repo_mod_ratio[pathname_mean][modname1] = mod_ratio
		###################################################################################

		############# Calculates a ratio between nentries of hltPre+path module of HPU and Nominal
		if (pathname_mean in repo_nentries_hltPre1) and (pathname_mean in repo_nentries_hltPre2):
			for modname1, nentries1 in repo_nentries1[pathname_mean].iteritems():
				for modname2, nentries2 in repo_nentries2[pathname_mean].iteritems():
					if (nentries1 == 0) or (nentries2 ==0):
						hltPre_ratio = 1
					else:   
						hltPre_ratio = nentries1/nentries2
				if not pathname_mean in repo_hltPre_ratio:
					repo_hltPre_ratio[pathname_mean] = {}
				repo_hltPre_ratio[pathname_mean][modname1] = hltPre_ratio
		#######################################################################################

		############# Calculates the estimated HPU time
                for modname_mean, mean in repo_means1[pathname_mean].iteritems():
			estimatedtime = mean*mod_ratio*hltPre_ratio
			if not pathname_mean in repo_HPU:
				repo_HPU[pathname_mean] = {}
			repo_HPU[pathname_mean][modname_mean] = estimatedtime
		#################################################################

	#print repo_HPU
	return repo_HPU

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
				#print path, time1
		for pathname2, time2 in repo_estimatedPathTime.iteritems():
			if path == pathname2:
				h2.Fill(path, time2) 
				if time2 > maxmean:
					maxmean = time2
				#print path, time2

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


##############################################################
def plot_results_permodule(repoNominalPathTime, repoHighPUPathTime, repoEstimatedPathTime, pathlist):
	''' Plot module time per Path '''
##############################################################             
        maxmean = 0
        for path in pathlist:
                can = TCanvas("can","",1500,800);
                can.cd();
                for pathname1 in repoNominalPathTime:
                        if path == pathname1:
                		h1 = TH1F("h1",path, len(repoNominalPathTime[pathname1]),0,len(repoNominalPathTime[pathname1]))
                                for modname1, time1 in repoNominalPathTime[pathname1].iteritems():
                                        h1.Fill(modname1, time1)
#                                        #print pathname1, modname1, time1

                for pathname2 in repoHighPUPathTime:
                        if path == pathname2:
                		h2 = TH1F("h2","", len(repoHighPUPathTime[pathname2]),0,len(repoHighPUPathTime[pathname2]))
                                for modname2, time2 in repoHighPUPathTime[pathname2].iteritems():
                                        h2.Fill(modname2, time2)
                                        if time2 > maxmean:
                                                 maxmean = time2
#                                        #print pathname2, modname2, time2

                for pathname3 in repoEstimatedPathTime:
                        if path == pathname3:
                		h3 = TH1F("h3","", len(repoEstimatedPathTime[pathname3]),0,len(repoEstimatedPathTime[pathname3]))
                                for modname3, time3 in repoEstimatedPathTime[pathname3].iteritems():
                                        h3.Fill(modname3, time3)
#                                        #print pathname3, modname3, time3

	h1.Draw()
	h1.SetLineWidth(2)
	h1.SetMaximum(maxmean+3)
	h1.GetYaxis().SetTitle("msec")
	h1.GetXaxis().SetLabelSize(0.03)
	h2.SetLineColor(2)
	h2.Draw("same")
	h2.SetLineWidth(2)
	h3.SetLineColor(3)
	h3.Draw("same")
	h3.SetLineWidth(2)
	can.SetBottomMargin(0.35)
	can.SetGridx()
	can.SetGridy()
	lg = TLegend(0.79, 0.89, 1.0, 0.99);
	lg.AddEntry(h1,"Nominal","L");
	lg.AddEntry(h2,"High Pile Up","L");
	lg.AddEntry(h3,"Estimated","L");
	lg.SetTextSize(0.025);
	lg.SetBorderSize(0);
	lg.SetFillColor(0);
	lg.Draw("same");
	can.SaveAs(path+".png")
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

	######### For plot_results_module (Please uncomment the path you want to plot)
#	pathlist = ['HLT_Mu40_v5']
#	pathlist = ['HLT_Ele32_WP70_PFMT50_v3']
#	pathlist = ['HLT_Jet300_v5']
#	pathlist = ['HLT_60Jet10_v1']
#	pathlist = ['HLT_Photon26_IsoVL_Photon18_IsoVL_v7']
	pathlist = ['HLT_HT300_MHT80_v2']
	###################################################

	########## For plot_results
#	pathlist = ['HLT_Mu40_v5',
#		'HLT_Ele32_WP70_PFMT50_v3',
#		'HLT_Jet300_v5',
#		'HLT_Photon26_IsoVL_Photon18_IsoVL_v7',
#		'HLT_60Jet10_v1',
#		'HLT_HT300_MHT80_v2']
	###################################################

	
	print "This process takes a while.... please be patient\n"

	############### For the second estimated HPU time
	repo_means1, repo_nentries1, repo_nentries_hltPre1, repo_means_sum1 = get_histos_info_mean(infile1)
	repo_means2, repo_nentries2, repo_nentries_hltPre2, repo_means_sum2 = get_histos_info_mean(infile2)
	HPUtime = get_HPU_time(repo_means1,repo_nentries1, repo_nentries_hltPre1, repo_nentries2, repo_nentries_hltPre2)
	plot_results_permodule(repo_means2,repo_means1, HPUtime, pathlist)
	print "Final plot is pathname.png where pathname is the specific path name\n"
	###############################################################################################################


	############### For the first estimated HPU time
	#repo5, repo7 = get_histos_info_mean(infile2)		#for test porpouses
	#repo2, repo3, nentries = get_histos_info_nominal(infile2,repo1)
	#time1, repo4 = get_highPU_time(repo1,repo2, nentries)
	#plot_results(repo3,repo4, pathlist)
	#print "Final plot is hltHighPU_estimatedtime.png\n"
	#plot_results_permodule(repo5,repo1, time1, pathlist)
	#print "Final plot is pathname.png where pathname is the specific path name\n"
	###############################################################################


#######################################################
if __name__ =='__main__':
#######################################################
	sys.exit(main())
