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
#gStyle.SetStatY(0.9)
#gStyle.SetStatX(0.9)
#gStyle.SetStatW(0.20)
#gStyle.SetStatH(0.20) 

#trigger = str ( sys.argv[1] )
trigger = '4jet80_6jet60'
#logscale = int ( sys.argv[4] )
histos = { #'massRecoBjetsCSVM':'step1plots1D', 
		#'avgMassRecoBjetsCSVM':'step1plots1D', 
		#'massdijetWORecoBjetsCSVM':'step2plots1D',
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
		'avgMassHiggsCandidate_cutDiagStop2bbjj0':'step3plots1D',
		'avgMassHiggsCandidate_cutDiagStop2bbjj0_resoBasedBin':'step3plots1D',
		'massHiggsCandidate_cutDiagStop2bbjj0':'step3plots1D',
		'massStop1Candidate_cutDiagStop2bbjj0':'step3plots1D',
		'massStop1Candidate_cutDiagStop2bbjj0_resoBasedBin':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj0':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj0_resoBasedBin':'step3plots1D',
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
	if "avgMass" in hist1: 
		h1clone.GetXaxis().SetTitle("Average of Higgs Candidates Mass [GeV]")
		h1clone.GetYaxis().SetTitle("Events / 10 GeV")
		if "reso" in hist1: h1clone.GetYaxis().SetTitle("dN/dM_{bb} [GeV^{-1}]")
	elif "massHiggs" in hist1: h1clone.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
	elif "massRecoBjets" in hist1: h1clone.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
	elif "massStop1" in hist1: 
		h1clone.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
		h1clone.GetYaxis().SetTitle("Dijets / 10 GeV")
		if "reso" in hist1: h1clone.GetYaxis().SetTitle("dN/dM_{jj} [GeV^{-1}]")
	elif "massdijet" in hist1: h1clone.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
	elif "massStop1" in hist1: h1clone.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
	elif "massRecoDiBjet" in hist1: 
		h1clone.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
		h1clone.GetYaxis().SetTitle("Quadjets / 10 GeV")
		if "reso" in hist1: h1clone.GetYaxis().SetTitle("dN/dM_{bbjj} [GeV^{-1}]")
	else: h1clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
	h1clone.GetYaxis().SetTitleOffset(1.2)
	
	textBox=TLatex()
	textBox.SetNDC()
	textBox.SetTextSize(0.05) 
	textBox.SetTextColor(50)
	textBox.DrawLatex(0.10,0.91,"CMS Preliminary 19.5 fb^{-1} at #sqrt{s} = 8 TeV")
	
	textBox1=TLatex()
	textBox1.SetNDC()
	textBox1.SetTextSize(0.04) 
	textBox1.DrawText(0.70,0.85,"DATA")
	
	textBox3=TLatex()
	textBox3.SetNDC()
	textBox3.SetTextSize(0.04) 
	textBox3.DrawLatex(0.70,0.80,"4^{th} jet > 80 GeV ")
	
	textBox4=TLatex()
	textBox4.SetNDC()
	textBox4.SetTextSize(0.04) 
	textBox4.DrawLatex(0.70,0.75,"6^{th} jet > 60 GeV ")

	textBox4=TLatex()
	textBox4.SetNDC()
	textBox4.SetTextSize(0.04) 
	textBox4.DrawLatex(0.70,0.70,"#geq 4 bjets")

	if "cutDiag" in hist1:
		textBox2=TLatex()
		textBox2.SetNDC()
		textBox2.SetTextSize(0.04) 
		textBox2.DrawLatex(0.70,0.65,"#Delta = 0 GeV")
	
	c.SaveAs(outputDir + hist1 + "_data_"+trigger+".pdf")
	c.SaveAs(outputDir + hist1 + "_data_"+trigger+".eps")
	del c
