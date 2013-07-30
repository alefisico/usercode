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
#print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]

input1 = "/cms/gomez/Stops/Results/HT250-500_plots.root"
input2 = "/cms/gomez/Stops/Results/HT500-1000_plots.root"
input3 = "/cms/gomez/Stops/Results/HT1000-Inf_plots.root"
input4 = "/cms/gomez/Stops/Results/st2_h_bb_st1_"+decay1+"_"+str(st2mass1)+"_"+str(st1mass1)+"_plots.root"

outputDir = "/cms/gomez/Stops/Results/Plots/"

#print "Input files: ", input1

canvs = []
outhists = []
stacks = []
leg = []

f1 = TFile(input1)
f2 = TFile(input2)
f3 = TFile(input3)
f4 = TFile(input4)


mcScaleFile = open("MCScaleFactors.txt")
for line in mcScaleFile:
	line = line.rstrip()
	if "HT250-500XS" == line.split()[0]:
		HT250_500XS = float(line.split()[1])
	if "HT500-1000XS" == line.split()[0]: 
		HT500_1000XS = float(line.split()[1])
	if "HT1000-InfXS" == line.split()[0]: 
		HT1000_InfXS = float(line.split()[1])
	if "HT250-500nEvents" == line.split()[0]: 
		HT250_500nEvents = float(line.split()[1])
	if "HT500-1000nEvents" == line.split()[0]: 
		HT500_1000nEvents = float(line.split()[1])
	if "HT1000-InfnEvents" == line.split()[0]: 
		HT1000_InfnEvents = float(line.split()[1])
	if "st2XS"+str(st2mass1) == line.split()[0]:
		signalXS = float(line.split()[1])
	if decay1+"_"+str(st2mass1)+"_"+str(st1mass1) == line.split()[0]: 
		signalnEvents = float(line.split()[1])


weightSignal = 20000 * signalXS/signalnEvents
weightHT250_500 = 20000 * HT250_500XS/HT250_500nEvents
weightHT500_1000 = 20000 * HT500_1000XS/HT500_1000nEvents
weightHT1000_Inf = 20000 * HT1000_InfXS/HT1000_InfnEvents
#weightHT250_500 = 1 
#weightHT500_1000 = 1
#weightHT1000_Inf = 1

#print "ScaleFactor for st2_h_bb_st1_"+decay1+"_"+str(st2mass1)+"_"+str(st1mass1) , weight

###########################
# PLOTTING FUNCTION
#   plot("histogram1","histogram2","Title;XaxisLabel;YaxisLabel","histo1legendEntry","histo2legendEntry","OutputFileName.png")

def plot_cuts(folder,hist1,label1, label2) :

	gROOT.Reset()
	gStyle.SetPadRightMargin(0.2);

	#h1 = f1.Get(options.dir + hist1)
	h1 = f1.Get(folder+'/' + hist1)
	h2 = f2.Get(folder+'/' + hist1)
	h3 = f3.Get(folder+'/' + hist1)
	h4 = f4.Get(folder+'/' + hist1)

	h1clone = h1.Clone("h1")
	h2clone = h2.Clone("h2")
	h3clone = h3.Clone("h3")
	h4clone = h4.Clone("h3")

  	hs = THStack( h1.GetName()+h2.GetName()+h3.GetName()+h4.GetName(), h1.GetTitle() )

  
	h1clone.Scale( weightHT250_500 )
	h2clone.Scale( weightHT500_1000 )
	h3clone.Scale( weightHT1000_Inf )
	h4clone.Scale( weightSignal )

	h4clone.Add( h1clone )
	h4clone.Add( h2clone )
	h4clone.Add( h3clone )

	h1clone.Add( h2clone )
	h1clone.Add( h3clone )

  	h1clone.SetLineColor(1)
  	h4clone.SetLineColor(1)
	h1clone.SetFillColor(10)
	h4clone.SetFillColor(2)
  	#h1clone.SetLineWidth(2)
  	#h4clone.SetLineWidth(2)

	hs.SetTitle(hist1)
	hs.Add( h4clone, 'hist' )
	hs.Add( h1clone, 'hist' )

	c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
	#c.SetLogy()
	hs.Draw('nostack')
	#hs.GetXaxis().SetTitle("Mass [GeV]")

	legend=TLegend(0.83,0.4,0.98,0.8)
	legend.SetFillColor(0);
	legend.AddEntry(h1clone,label1, "f")
	legend.AddEntry(h4clone,label2, "f")
	legend.Draw()
  
	outhists.append( [h1clone,h4clone] )
	stacks.append( hs )
	leg.append( legend )
	canvs.append(c)

	c.SaveAs(outputDir + hist1 + "_QCD_"+decay1+"_"+str(st2mass1)+"_"+str(st1mass1)+".pdf")
	return


plot_cuts( folder, hist1, "QCD", "QCD + "+decay1+"_"+str(st2mass1)+"_"+str(st1mass1)  )
