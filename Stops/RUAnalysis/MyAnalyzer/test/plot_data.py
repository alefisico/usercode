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

input1 = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_plots.root"
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

f1 = TFile(input1)

h1 = f1.Get(folder+'/' + hist)
h1clone = h1.Clone(hist)

c = TCanvas('c_' + hist, 'c_' + hist,  10, 10, 750, 500 )
c.SetLogy()
c.SetGrid()
h1clone.Draw()
#h1clone.GetXaxis().SetTitle("Mass [GeV]")
#h1clone.GetYaxis().SetTitle("Dijet/10 GeV")
#h1clone.GetYaxis().SetTitle("Events/10 GeV")
h1clone.GetYaxis().SetTitle("Events")
h1clone.GetYaxis().SetTitleOffset(1);

c.SaveAs(outputDir + hist + "_data.pdf")
