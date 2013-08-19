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

hist = str ( sys.argv[1] )
folder = str ( sys.argv[2] )
#hist2 = str ( sys.argv[5] )
#print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]

input1 = "/uscms_data/d3/algomez/files/Stops/Results/HT250-500_4jet80_plots.root"
input2 = "/uscms_data/d3/algomez/files/Stops/Results/HT500-1000_4jet80_plots.root"
input3 = "/uscms_data/d3/algomez/files/Stops/Results/HT1000-Inf_4jet80_plots.root"
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

#print "Input files: ", input1

f1 = TFile(input1)
f2 = TFile(input2)
f3 = TFile(input3)

mcScaleFile = open("MCScaleFactors.txt")
for line in mcScaleFile:
	line = line.rstrip()
	if "HT250-500XS" == line.split()[0]:
		HT250_500XS = float(line.split()[1])
		print line
	if "HT500-1000XS" == line.split()[0]: 
		HT500_1000XS = float(line.split()[1])
	if "HT1000-InfXS" == line.split()[0]: 
		HT1000_InfXS = float(line.split()[1])
	if "HT250-500nEvents" == line.split()[0]: 
		HT250_500nEvents = float(line.split()[1])
		print line
	if "HT500-1000nEvents" == line.split()[0]: 
		HT500_1000nEvents = float(line.split()[1])
	if "HT1000-InfnEvents" == line.split()[0]: 
		HT1000_InfnEvents = float(line.split()[1])

weightHT250_500 = 20000*HT250_500XS/HT250_500nEvents
weightHT500_1000 = 20000*HT500_1000XS/HT500_1000nEvents
weightHT1000_Inf = 20000*HT1000_InfXS/HT1000_InfnEvents
#weightHT250_500 = 1 
#weightHT500_1000 = 1
#weightHT1000_Inf = 1

print "ScaleFactor for HT250_500: ", weightHT250_500
print "ScaleFactor for HT500-1000: ", weightHT500_1000
print "ScaleFactor for HT1000-Inf: ", weightHT1000_Inf

h1 = f1.Get(folder+'/' + hist)
h2 = f2.Get(folder+'/' + hist)
h3 = f3.Get(folder+'/' + hist)

h1clone = h1.Clone(hist)
h2clone = h2.Clone(hist)
h3clone = h3.Clone(hist)

h1clone.Scale( weightHT250_500 )
h2clone.Scale( weightHT500_1000 )
h3clone.Scale( weightHT1000_Inf )

h1clone.Add( h2clone )
h1clone.Add( h3clone )

c = TCanvas('c_' + hist, 'c_' + hist,  10, 10, 750, 500 )
c.SetLogz()

h1clone.Draw("colz")
h1clone.GetXaxis().SetTitle('scalar #sum Pt [GeV]')
h1clone.GetYaxis().SetTitle('Mass [GeV]')
	

c.SaveAs(outputDir + hist + "_QCD.pdf")
