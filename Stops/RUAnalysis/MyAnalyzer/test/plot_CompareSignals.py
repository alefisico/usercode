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
st1mass1 = int (sys.argv[4])
decay1 = sys.argv[5]
#print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]

input1 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay1+"_750_"+str(st1mass1)+"_plots.root"
input2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay1+"_650_"+str(st1mass1)+"_plots.root"
input3 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay1+"_550_"+str(st1mass1)+"_plots.root"
input4 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay1+"_450_"+str(st1mass1)+"_plots.root"
input5 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay1+"_350_"+str(st1mass1)+"_plots.root"
input6 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay1+"_250_"+str(st1mass1)+"_plots.root"

outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

#print "Input files: ", input7

if ( norm == 0 ):
	weightSignal250 = 1
	weightSignal350 = 1
	weightSignal450 = 1
	weightSignal550 = 1
	weightSignal650 = 1
	weightSignal750 = 1
else:
	mcScaleFile = open("MCScaleFactors.txt")
	for line in mcScaleFile:
		line = line.rstrip()
		if "st2XS250" == line.split()[0]:
			signal250XS = float(line.split()[1])
		if "st2XS350" == line.split()[0]:
			signal350XS = float(line.split()[1])
		if "st2XS450" == line.split()[0]:
			signal450XS = float(line.split()[1])
		if "st2XS550" == line.split()[0]:
			signal550XS = float(line.split()[1])
		if "st2XS650" == line.split()[0]:
			signal650XS = float(line.split()[1])
		if "st2XS750" == line.split()[0]:
			signal750XS = float(line.split()[1])

	weightSignal250 = 20000 * signal250XS/100000
	weightSignal350 = 20000 * signal350XS/100000
	weightSignal450 = 20000 * signal450XS/100000
	weightSignal550 = 20000 * signal550XS/100000
	weightSignal650 = 20000 * signal650XS/100000
	weightSignal750 = 20000 * signal750XS/100000


f1 = TFile(input1)
f2 = TFile(input2)
if ( st1mass1 < 500 ): f3 = TFile(input3)
if ( st1mass1 < 400 ): f4 = TFile(input4)
if ( st1mass1 < 300 ): f5 = TFile(input5)
if ( st1mass1 < 200 ): f6 = TFile(input6)

h1 = f1.Get(folder+'/' + hist1)
h2 = f2.Get(folder+'/' + hist1)
if ( st1mass1 < 500 ): h3 = f3.Get(folder+'/' + hist1)
if ( st1mass1 < 400 ): h4 = f4.Get(folder+'/' + hist1)
if ( st1mass1 < 300 ): h5 = f5.Get(folder+'/' + hist1)
if ( st1mass1 < 200 ): h6 = f6.Get(folder+'/' + hist1)

h1clone = h1.Clone("h1")
h2clone = h2.Clone("h2")
if ( st1mass1 < 500 ): h3clone = h3.Clone("h3")
if ( st1mass1 < 400 ): h4clone = h4.Clone("h4")
if ( st1mass1 < 300 ): h5clone = h5.Clone("h5")
if ( st1mass1 < 200 ): h6clone = h6.Clone("h6")

h1clone.Scale( weightSignal250 )
h2clone.Scale( weightSignal350 )
if ( st1mass1 < 500 ): h3clone.Scale( weightSignal450 )
if ( st1mass1 < 400 ): h4clone.Scale( weightSignal550 )
if ( st1mass1 < 300 ): h5clone.Scale( weightSignal650 )
if ( st1mass1 < 200 ): h6clone.Scale( weightSignal750 )

h1clone.SetLineColor(1)
h2clone.SetLineColor(2)
if ( st1mass1 < 500 ): h3clone.SetLineColor(3)
if ( st1mass1 < 400 ): h4clone.SetLineColor(4)
if ( st1mass1 < 300 ): h5clone.SetLineColor(5)
if ( st1mass1 < 200 ): h6clone.SetLineColor(6)
h1clone.SetLineWidth(2)
h2clone.SetLineWidth(2)
if ( st1mass1 < 500 ): h3clone.SetLineWidth(2)
if ( st1mass1 < 400 ): h4clone.SetLineWidth(2)
if ( st1mass1 < 300 ): h5clone.SetLineWidth(2)
if ( st1mass1 < 200 ): h6clone.SetLineWidth(2)

c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
#c.SetLogy()
if ( norm == 0 ) :
	h2clone.GetXaxis().SetTitle("Mass [GeV]")
	h2clone.GetYaxis().SetTitle("Normalized")
	h2clone.GetYaxis().SetTitleOffset(1.2)
	h2clone.SetMaximum(h1clone.GetMaximum()*1.4)
	h2clone.DrawNormalized("hist")
	h1clone.DrawNormalized("histsame")
	if ( st1mass1 < 500 ): h3clone.DrawNormalized("histsame")
	if ( st1mass1 < 400 ): h4clone.DrawNormalized("histsame")
	if ( st1mass1 < 300 ): h5clone.DrawNormalized("histsame")
	if ( st1mass1 < 200 ): h6clone.DrawNormalized("histsame")
else: 
	h1clone.GetXaxis().SetTitle("Mass [GeV]")
	h1clone.GetYaxis().SetTitle("Events/10 GeV")
	h1clone.Draw()
	h2clone.Draw("same")
	if ( st1mass1 < 500 ): h3clone.Draw("same")
	if ( st1mass1 < 400 ): h4clone.Draw("same")
	if ( st1mass1 < 300 ): h5clone.Draw("same")
	if ( st1mass1 < 200 ): h6clone.Draw("same")

legend=TLegend(0.70,0.7,0.90,0.9)
legend.SetFillColor(0);
legend.AddEntry(h1clone, decay1+"_750_"+str(st1mass1), "l")
legend.AddEntry(h2clone, decay1+"_650_"+str(st1mass1), "l")
if ( st1mass1 < 500 ): legend.AddEntry(h3clone, decay1+"_550_"+str(st1mass1), "l")
if ( st1mass1 < 400 ): legend.AddEntry(h4clone, decay1+"_450_"+str(st1mass1), "l")
if ( st1mass1 < 300 ): legend.AddEntry(h5clone, decay1+"_350_"+str(st1mass1), "l")
if ( st1mass1 < 200 ): legend.AddEntry(h6clone, decay1+"_250_"+str(st1mass1), "l")
legend.Draw()
  
if ( norm == 0 ) : c.SaveAs(outputDir + hist1 + "_DiffSignals_"+decay1+"_"+str(st1mass1)+"_Normalized.pdf")
else: c.SaveAs(outputDir + hist1 + "_"+decay1+"_"+str(st1mass1)+"_DiffSignals.pdf")
