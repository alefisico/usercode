#!/usr/bin/env python

###################
# plot_Compare2Histos.py
# description: compares two histograms from one root file
# instructions:
#  python -i plot_Compare2Histos.py
###################

from ROOT import *
import glob,sys

gROOT.Reset()
gStyle.SetOptStat(0)

hist1 = str ( sys.argv[1] )
folder = str ( sys.argv[2] )
norm = int ( sys.argv[3] )
decay1 = sys.argv[4]
#print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]

input1 = "/uscms_data/d3/algomez/files/Stops/Results/HT250-500_4jet80_plots.root"
input2 = "/uscms_data/d3/algomez/files/Stops/Results/HT500-1000_4jet80_plots.root"
input3 = "/uscms_data/d3/algomez/files/Stops/Results/HT1000-Inf_4jet80_plots.root"
input4 = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_plots.root"
input5 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay1+"_450_200_plots.root"
input6 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay1+"_750_200_plots.root"

outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

#print "Input files: ", input1

if ( norm == 0 ):
	weightHT250_500 = 1 
	weightHT500_1000 = 1
	weightHT1000_Inf = 1
	weightSignal450 = 1
	weightSignal750 = 1
else:
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
		if "st2XS450" == line.split()[0]:
			signal450XS = float(line.split()[1])
		if "st2XS750" == line.split()[0]:
			signal750XS = float(line.split()[1])

	weightHT250_500 = 20000 * HT250_500XS/HT250_500nEvents
	weightHT500_1000 = 20000 * HT500_1000XS/HT500_1000nEvents
	weightHT1000_Inf = 20000 * HT1000_InfXS/HT1000_InfnEvents
	weightSignal450 = 20000 * signal450XS/100000
	weightSignal750 = 20000 * signal750XS/100000


f1 = TFile(input1)
f2 = TFile(input2)
f3 = TFile(input3)
f4 = TFile(input4)
f5 = TFile(input5)
f6 = TFile(input6)

h1 = f1.Get(folder+'/' + hist1)
h2 = f2.Get(folder+'/' + hist1)
h3 = f3.Get(folder+'/' + hist1)
h4 = f4.Get(folder+'/' + hist1)
h5 = f5.Get(folder+'/' + hist1)
h6 = f6.Get(folder+'/' + hist1)

h1clone = h1.Clone("h1")
h2clone = h2.Clone("h2")
h3clone = h3.Clone("h3")
h4clone = h4.Clone("h4")
h5clone = h5.Clone("h5")
h6clone = h6.Clone("h6")

  
h1clone.Scale( weightHT250_500 )
h2clone.Scale( weightHT500_1000 )
h3clone.Scale( weightHT1000_Inf )
h5clone.Scale( weightSignal450 )
h6clone.Scale( weightSignal750 )

h1clone.Add( h2clone )
h1clone.Add( h3clone )

h1clone.SetLineColor(1)
h4clone.SetLineColor(2)
h5clone.SetLineColor(3)
h6clone.SetLineColor(4)
#h1clone.SetFillColor(10)
#h4clone.SetFillColor(2)
h1clone.SetLineWidth(2)
h4clone.SetLineWidth(2)

c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
#c.SetLogy()
if ( norm == 0 ) :
	h4clone.GetXaxis().SetTitle("Mass [GeV]")
	h4clone.GetYaxis().SetTitle("Normalized")
	h4clone.DrawNormalized("hist")
	h1clone.DrawNormalized("histsame")
	h5clone.DrawNormalized("histsame")
	h6clone.DrawNormalized("histsame")
else: 
	h4clone.GetXaxis().SetTitle("Mass [GeV]")
	h4clone.GetYaxis().SetTitle("Events/10 GeV")
	h4clone.Draw()
	h1clone.Draw("same")
	h5clone.Draw("same")
	h6clone.Draw("same")

legend=TLegend(0.70,0.7,0.90,0.9)
legend.SetFillColor(0);
legend.AddEntry(h1clone,"QCD", "l")
legend.AddEntry(h4clone,"DATA", "l")
legend.AddEntry(h5clone,decay1+"_450_200", "l")
legend.AddEntry(h6clone,decay1+"_750_200", "l")
legend.Draw()
  
if ( norm == 0 ) : c.SaveAs(outputDir + hist1 + "_DATA_QCD_Signal_Normalized.pdf")
else: c.SaveAs(outputDir + hist1 + "_DATA_QCD_Signal.pdf")
