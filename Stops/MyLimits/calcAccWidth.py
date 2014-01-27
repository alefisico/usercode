#!/usr/bin/env python

from ROOT import *
import glob,sys
from array import array

gROOT.Reset()
gStyle.SetOptStat(0)
gStyle.SetOptFit()
gStyle.SetStatY(0.9)
gStyle.SetStatX(0.4)
gStyle.SetStatW(0.12)
gStyle.SetStatH(0.12)

listMass = []
listAcceptance = []
listWidth = []

for line in open("signalInfo.txt"):
	columns = line.split()
       	listMass.append( float(columns[0]))
       	listAcceptance.append( float(columns[2]))
       	listWidth.append( float(columns[3]))

Mass = array('d', listMass)
Acceptance = array('d', listAcceptance)
Width = array('d', listWidth)

Acc = TGraph(len(Mass), Mass, Acceptance)
Acc.Fit("pol3")
Wid = TGraph(len(Mass), Mass, Width)
Wid.Fit("pol3")

c = TCanvas('c1', 'c1',  10, 10, 750, 500 )
#c.SetLogy()
#c.SetLogx()
Acc.SetTitle('Acceptance')
Acc.GetYaxis().SetTitle("Stop2 Mass [GeV]")
Acc.GetXaxis().SetTitle("Acceptance")
Acc.SetMarkerStyle(2)
Acc.Draw("ap")
c.SaveAs("Acceptance.pdf")
del c

c = TCanvas('c1', 'c1',  10, 10, 750, 500 )
#c.SetLogy()
#c.SetLogx()
Wid.SetTitle('Gaussian Width')
Wid.GetYaxis().SetTitle("Stop2 Mass [GeV]")
Wid.GetXaxis().SetTitle("Gaussian Width [GeV]")
Wid.SetMarkerStyle(2)
Wid.Draw("ap")
c.SaveAs("Width.pdf")
