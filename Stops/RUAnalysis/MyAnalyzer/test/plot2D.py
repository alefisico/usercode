#!/usr/bin/env python

###################
# plot_Compare2Histos.py
# description: compares two histograms from one root file
# instructions:
#  python -i plot_Compare2Histos.py
###################

from ROOT import *
import glob,sys

st1mass1 = int (sys.argv[1])
st2mass1 = int (sys.argv[2])
decay1 = sys.argv[3]
hist1 = str ( sys.argv[4] )
#hist2 = str ( sys.argv[5] )
#print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]

#input1 = "/cms/gomez/Stops/Results/st2_h_bb_st1_"+decay1+"_"+str(st2mass1)+"_"+str(st1mass1)+"_plots_v7.root"
input1 = "/cms/gomez/Stops/Results/st2_h_bb_st1_"+decay1+"_"+str(st2mass1)+"_"+str(st1mass1)+"_plots_v7_120-130.root"
outputDir = "/cms/gomez/Stops/Results/Plots/"


f1 = TFile(input1)

###########################
# PLOTTING FUNCTION
#   plot("histogram1","histogram2","Title;XaxisLabel;YaxisLabel","histo1legendEntry","histo2legendEntry","OutputFileName.png")

def plot_cuts(hist1) :

	gROOT.Reset()
	#gStyle.SetPadRightMargin(0.2);

	h1 = f1.Get('plots/AnalysisPlots/' + hist1)

	c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
	c.SetLogz()

	h1.Draw("colz")
	h1.GetXaxis().SetTitle('scalar #sum Pt [GeV]')
	h1.GetYaxis().SetTitle('Mass [GeV]')
	

	c.SaveAs(outputDir + hist1 + "_120-130_" + decay1 + "_" + str(st2mass1) + "_" +str(st1mass1) + ".pdf")
	return


plot_cuts( hist1 )
#plot( hist1, hist2 )
