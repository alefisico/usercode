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

histos = { 'massRecoBjetsCSVM':'step1plots1D', 
		'avgMassRecoBjetsCSVM':'step1plots1D', 
		'massdijetWORecoBjetsCSVM':'step2plots1D',
		'massdijetWORecoBjetsCSVM_cutDiagStop1jj50':'step2plots1D',
		'massdijetWORecoBjetsCSVM_cutDiagStop1jj100':'step2plots1D',
		'massdijetWORecoBjetsCSVM_cutDiagStop1jj150':'step2plots1D',
		'massRecoDiBjetDiJet':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop1jj50':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop1jj100':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj50':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj100':'step3plots1D',
		'massRecoDiBjetDiJetSmallestDeltaM':'step4plots1D'
		}

#print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"


input2 = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet120_plots.root"
input3 = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_plots.root"
input1 = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet60_plots.root"
input4 = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_6jet60_plots.root"
	
	
f1 = TFile(input1)
f2 = TFile(input2)
f3 = TFile(input3)
f4 = TFile(input4)
	
for hist1, folder in histos.iteritems():
	h1 = f1.Get(folder+'/' + hist1)
	h2 = f2.Get(folder+'/' + hist1)
	h3 = f3.Get(folder+'/' + hist1)
	h4 = f4.Get(folder+'/' + hist1)

	h1clone = h1.Clone("h1")
	h2clone = h2.Clone("h2")
	h3clone = h3.Clone("h3")
	h4clone = h4.Clone("h4")

	h1clone.SetLineColor(1)
	h2clone.SetLineColor(2)
	h3clone.SetLineColor(3)
	h4clone.SetLineColor(4)
	h1clone.SetLineWidth(2)
	h2clone.SetLineWidth(2)
	h3clone.SetLineWidth(2)
	h4clone.SetLineWidth(2)

	c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
	#c.SetLogy()
	h1clone.SetTitle("")
	h1clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
	if "avg" in hist1: h1clone.GetYaxis().SetTitle("Events / 10 GeV")
	elif "DiJet" in hist1: h1clone.GetYaxis().SetTitle("QuadJet / 10 GeV")
	else: h1clone.GetYaxis().SetTitle("Dijet / 10 GeV")
	h1clone.GetYaxis().SetTitleOffset(1.2)
	h1clone.Draw("hist")
	h2clone.Draw("histsame")
	h3clone.Draw("histsame")
	h4clone.Draw("histsame")
	
	textBox=TLatex()
	textBox.SetNDC()
	textBox.SetTextSize(0.05) 
	textBox.SetTextColor(50)
	textBox.DrawText(0.10,0.91,"CMS Preliminary")
	
	legend=TLegend(0.70,0.7,0.90,0.9)
	legend.SetFillColor(0);
	legend.AddEntry(h2clone, "4jet120", "l")
	legend.AddEntry(h3clone, "4jet80", "l")
	legend.AddEntry(h1clone, "4jet60", "l")
	legend.AddEntry(h4clone, "4jet80_6jet60", "l")
	legend.Draw()
	  
	
	c.SaveAs(outputDir + hist1 + "_data_DiffAllTriggers.pdf")
	del c
