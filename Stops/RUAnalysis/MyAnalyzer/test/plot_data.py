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

#trigger = str ( sys.argv[1] )
trigger = '4jet80_6jet60'
#logscale = int ( sys.argv[4] )
histos = { 'massRecoBjetsCSVM':'step1plots1D', 
		'avgMassRecoBjetsCSVM':'step1plots1D', 
		'massdijetWORecoBjetsCSVM':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj0':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj10':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj20':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj30':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj40':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj50':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj60':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj70':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj80':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj90':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj100':'step2plots1D',
#		#'massdijetWORecoBjetsCSVM_cutDiagStop1jj150':'step2plots1D',
#		'massRecoDiBjetDiJet':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj0':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj10':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj20':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj30':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj40':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj50':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj60':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj70':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj80':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj90':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj100':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj0':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj10':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj20':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj30':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj40':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj50':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj60':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj70':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj80':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj90':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj100':'step3plots1D',
		#'massRecoDiBjetDiJetSmallestDeltaM':'step4plots1D'
		}

outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

input1 = "/uscms_data/d3/algomez/files/Stops/Results/data_"+trigger+"_plots.root"

f1 = TFile(input1)

for hist1, folder in histos.iteritems():
	h1 = f1.Get(folder+'/' + hist1)
	h1clone = h1.Clone(hist1)

	c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
	#if (logscale ==1): c.SetLogy()
	#c.SetGrid()
	h1clone.Draw()
	h1clone.SetTitle("")
	h1clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
	if "avg" in hist1: h1clone.GetYaxis().SetTitle("Events / 10 GeV")
	elif "DiJet" in hist1: h1clone.GetYaxis().SetTitle("QuadJet / 10 GeV")
	else: h1clone.GetYaxis().SetTitle("Dijet / 10 GeV")
	h1clone.GetYaxis().SetTitleOffset(1.2)
	
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
	
	c.SaveAs(outputDir + hist1 + "_data_"+trigger+".pdf")
	del c
