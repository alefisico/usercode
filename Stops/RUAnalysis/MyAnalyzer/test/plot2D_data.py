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
gStyle.SetOptStat()
gStyle.SetStatY(0.9)
gStyle.SetStatX(0.9)
gStyle.SetStatW(0.20)
gStyle.SetStatH(0.20) 

trigger = str ( sys.argv[1] )

histos = { 'recoDiBjetDiJet_masspt':'step3plots2D',
		'recoDiBjetDiJet_cutDiagStop1jj50_masspt':'step3plots2D',
		'recoDiBjetDiJet_cutDiagStop1jj100_masspt':'step3plots2D',
		'recoDiBjetDiJet_cutDiagStop2bbjj50_masspt':'step3plots2D',
		'recoDiBjetDiJet_cutDiagStop2bbjj100_masspt':'step3plots2D',
		'dijetWORecoBjetsCSVM_masspt':'step2plots2D',
		'dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj50':'step2plots2D'
		}

input1 = "/uscms_data/d3/algomez/files/Stops/Results/data_"+trigger+"_plots.root"
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"


f1 = TFile(input1)

for hist1, folder in histos.iteritems():
	
	h1 = f1.Get(folder+'/' + hist1)
		
	c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
	c.SetLogz()
		
	h1.Draw("colz")
	h1.SetTitle("")
	h1.GetXaxis().SetTitle('scalar #sum Pt [GeV]')
	h1.GetYaxis().SetTitle('Invariant Mass [GeV]')
	h1.GetYaxis().SetTitleOffset(1.2)
	h1.GetXaxis().SetTitleOffset(1.2)
			
	textBox=TLatex()
	textBox.SetNDC()
	textBox.SetTextSize(0.05) 
	textBox.SetTextColor(50)
	textBox.DrawLatex(0.10,0.91,"CMS Preliminary 19.5 fb^{-1} at #sqrt{s} = 8 TeV")
		
	textBox1=TLatex()
	textBox1.SetNDC()
	textBox1.SetTextSize(0.04) 
	textBox1.SetTextColor(50)
	textBox1.DrawText(0.60,0.85,"DATA")

	c.SaveAs(outputDir + hist1 + "_data_" + trigger + ".pdf")
	del c
