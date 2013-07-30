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
folder = str ( sys.argv[5] )
#hist2 = str ( sys.argv[5] )
#print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]

input1 = "/cms/gomez/Stops/Results/st2_h_bb_st1_"+decay1+"_"+str(st2mass1)+"_"+str(st1mass1)+"_plots.root"

outputDir = "/cms/gomez/Stops/Results/Plots/"

#print "Input files: ", input1

canvs = []
outhists = []
stacks = []
leg = []

f1 = TFile(input1)


mcScaleFile = open("MCScaleFactors.txt")
for line in mcScaleFile:
	line = line.rstrip()
	if "st2XS"+str(st2mass1) == line.split()[0]:
		#print line.split()[1]
		XSsample = float(line.split()[1])
	if decay1+"_"+str(st2mass1)+"_"+str(st1mass1) == line.split()[0]:
		#print line.split()[1]
		nEvents = float(line.split()[1])

#weight = nEvents/XSsample
weight = 20000 * XSsample/nEvents
print "ScaleFactor for st2_h_bb_st1_"+decay1+"_"+str(st2mass1)+"_"+str(st1mass1) , weight

###########################
# PLOTTING FUNCTION
#   plot("histogram1","histogram2","Title;XaxisLabel;YaxisLabel","histo1legendEntry","histo2legendEntry","OutputFileName.png")

def plot_cuts(folder,hist1,label1) :

	gROOT.Reset()
	gStyle.SetPadRightMargin(0.2);

	#h1 = f1.Get(options.dir + hist1)
	h1 = f1.Get(folder+'/' + hist1)

	h1clone = h1.Clone("h1")

  	hs = THStack( h1.GetName(), h1.GetTitle() )

	h1clone.Scale( weight )

	hs.SetTitle(hist1)
	hs.Add( h1clone, 'hist' )

	c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
	#c.SetLogy()
	hs.Draw('nostack')
	#hs.GetXaxis().SetTitle("Mass [GeV]")

	legend=TLegend(0.83,0.4,0.98,0.8)
	legend.SetFillColor(0);
	legend.AddEntry(h1clone,label1, "l")
	legend.Draw()
  
	outhists.append( [h1clone] )
	stacks.append( hs )
	leg.append( legend )
	canvs.append(c)

	c.SaveAs(outputDir + hist1 + "_" + decay1 + "_" + str(st2mass1) + "_" +str(st1mass1) + ".pdf")
	return


plot_cuts( folder, hist1, decay1 + "_" + str(st2mass1) + "_" +str(st1mass1) )
