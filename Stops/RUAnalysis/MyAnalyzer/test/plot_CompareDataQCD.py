#!/usr/bin/env python
from ROOT import *
import glob,sys

gROOT.Reset()
gStyle.SetOptStat(0)

trigger = "4jet80_6jet60"
#scale = "1"
#logscale = int ( sys.argv[4] )
histos = { #'recoBjets_num':'basicPlots',
#		'recoJets_pt':'basicPlots',
#		'recoJets_1pt':'basicPlots',
#		'recoJets_2pt':'basicPlots',
#		'recoJets_HT':'basicPlots',
#		'recoBjets_num_Step1':'basicPlots',
#		'massRecoBjetsCSVM':'step1plots1D', 
#		'avgMassRecoBjetsCSVM':'step1plots1D', 
#		'massdijetWORecoBjetsCSVM':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj50':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj100':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj150':'step2plots1D',
#		'massRecoDiBjetDiJet':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj50':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj100':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj0':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj10':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj20':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj30':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj40':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj50':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj100':'step3plots1D',
#		'massRecoDiBjetDiJetSmallestDeltaM':'step4plots1D'
		'avgMassHiggsCandidate_cutDiagStop2bbjj0':'step3plots1D',
		'avgMassHiggsCandidate_cutDiagStop2bbjj0_resoBasedBin':'step3plots1D',
		'massHiggsCandidate_cutDiagStop2bbjj0':'step3plots1D',
		'massStop1Candidate_cutDiagStop2bbjj0':'step3plots1D',
		'massStop1Candidate_cutDiagStop2bbjj0_resoBasedBin':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj0':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj0_resoBasedBin':'step3plots1D',
		}

input3 = "/uscms_data/d3/algomez/files/Stops/Results/HT250-500_4jet80_6jet60_plots.root"
input2 = "/uscms_data/d3/algomez/files/Stops/Results/HT500-1000_4jet80_6jet60_plots.root"
input1 = "/uscms_data/d3/algomez/files/Stops/Results/HT1000-Inf_4jet80_6jet60_plots.root"
input4 = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_6jet60_plots.root"

outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

#print "Input files: ", input1

#HT250-500XS 276000
#HT500-1000XS 8426
#HT1000-InfXS 240
#HT250-500nEvents  17021377
#HT500-1000nEvents  28122500
#HT1000-InfnEvents  13795394
#mcScaleFile = open("MCScaleFactors.txt")
#for line in mcScaleFile:
#	line = line.rstrip()
#	if "HT250-500XS" == line.split()[0]:
#		HT250_500XS = float(line.split()[1])
#	if "HT500-1000XS" == line.split()[0]: 
#		HT500_1000XS = float(line.split()[1])
#	if "HT1000-InfXS" == line.split()[0]: 
#		HT1000_InfXS = float(line.split()[1])
#	if "HT250-500nEvents" == line.split()[0]: 
#		HT250_500nEvents = float(line.split()[1])
#	if "HT500-1000nEvents" == line.split()[0]: 
#		HT500_1000nEvents = float(line.split()[1])
#	if "HT1000-InfnEvents" == line.split()[0]: 
#		HT1000_InfnEvents = float(line.split()[1])
#weightHT250_500 = 19500 * HT250_500XS/HT250_500nEvents
#weightHT500_1000 = 19500 * HT500_1000XS/HT500_1000nEvents
#weightHT1000_Inf = 19500 * HT1000_InfXS/HT1000_InfnEvents

#kfactor = 1.1  ### 1.2 WOHT250
kfactor = 2.5
weightHT250_500 = kfactor * 19500 * 276000/17021377
weightHT500_1000 = kfactor * 19500 * 8426/28122500
weightHT1000_Inf = kfactor * 19500 * 204/13795394

f1 = TFile(input1)
f2 = TFile(input2)
f3 = TFile(input3)
f4 = TFile(input4)

for hist1, folder in histos.iteritems():
	h1 = f1.Get(folder+'/' + hist1)
	h2 = f2.Get(folder+'/' + hist1)
	#h3 = f3.Get(folder+'/' + hist1)
	h4 = f4.Get(folder+'/' + hist1)
	
	h1clone = h1.Clone("h1")
	h2clone = h2.Clone("h2")
	#h3clone = h3.Clone("h3")
	h4clone = h4.Clone("h4")
	
	  
	#h3clone.Scale( weightHT250_500 )
	h2clone.Scale( weightHT500_1000 )
	h1clone.Scale( weightHT1000_Inf )
	
	#h1clone.Add( h3clone )
	h1clone.Add( h2clone )
	
	#h1clone.Scale(1000/h4clone.Integral())
	
	h1clone.SetLineColor(1)
	h4clone.SetLineColor(2)
	#h1clone.SetFillColor(10)
	#h4clone.SetFillColor(2)
	h1clone.SetLineWidth(2)
	h4clone.SetLineWidth(2)
	
	c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
	if 'num' in hist1: c.SetLogy()
	h4clone.SetTitle("")
	if "avgMass" in hist1: 
		h4clone.GetXaxis().SetTitle("Average of Higgs Candidates Mass [GeV]")
		h4clone.GetYaxis().SetTitle("Events / 10 GeV")
		if "reso" in hist1: h4clone.GetYaxis().SetTitle("dN/dM_{bb} [GeV^{-1}]")
	elif "massHiggs" in hist1: h4clone.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
	elif "massRecoBjets" in hist1: h4clone.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
	elif "massStop1" in hist1: 
		h4clone.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
		h4clone.GetYaxis().SetTitle("Dijets / 10 GeV")
		if "reso" in hist1: h4clone.GetYaxis().SetTitle("dN/dM_{jj} [GeV^{-1}]")
	elif "massdijet" in hist1: h4clone.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
	elif "massStop1" in hist1: h4clone.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
	elif "massRecoDiBjet" in hist1: 
		h4clone.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
		h4clone.GetYaxis().SetTitle("Quadjets / 10 GeV")
		if "reso" in hist1: h4clone.GetYaxis().SetTitle("dN/dM_{bbjj} [GeV^{-1}]")
	else: h4clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
	h4clone.GetYaxis().SetTitleOffset(1.2)
#	if 'pt' in hist1: h4clone.GetXaxis().SetTitle("p_T [GeV]")
#	elif 'num' in hist1: h4clone.GetXaxis().SetTitle("Number of Jets")
#	else: h4clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
#	if "avg" in hist1: h4clone.GetYaxis().SetTitle("Events / 10 GeV")
#	elif "DiJet" in hist1: h4clone.GetYaxis().SetTitle("QuadJet / 10 GeV")
#	elif "pt" in hist1: h4clone.GetYaxis().SetTitle("Events / 10 GeV")
#	elif "num" in hist1: h4clone.GetYaxis().SetTitle("Events / 10 GeV")
#	else: h4clone.GetYaxis().SetTitle("Dijet / 10 GeV")
#	h4clone.GetYaxis().SetTitleOffset(1.2)
	h4clone.Draw("hist")
	h1clone.Draw("histsame")
	
	legend=TLegend(0.70,0.8,0.90,0.9)
	legend.SetFillColor(0);
	legend.AddEntry(h1clone,"QCD Simulation", "l")
	legend.AddEntry(h4clone,"DATA", "l")
	legend.Draw()

	textBox=TLatex()
	textBox.SetNDC()
	textBox.SetTextSize(0.05) 
	textBox.SetTextColor(50)
	textBox.DrawLatex(0.10,0.91,"CMS Preliminary 19.5 fb^{-1} at #sqrt{s} = 8 TeV")
	
	textBox1=TLatex()
	textBox1.SetNDC()
	textBox1.SetTextSize(0.04) 
	textBox1.DrawText(0.65,0.75,"QCD scaled to DATA")
	
	textBox3=TLatex()
	textBox3.SetNDC()
	textBox3.SetTextSize(0.04) 
	textBox3.DrawLatex(0.70,0.70,"4^{th} jet > 80 GeV ")
	
	textBox4=TLatex()
	textBox4.SetNDC()
	textBox4.SetTextSize(0.04) 
	textBox4.DrawLatex(0.70,0.65,"6^{th} jet > 60 GeV ")

	textBox4=TLatex()
	textBox4.SetNDC()
	textBox4.SetTextSize(0.04) 
	textBox4.DrawLatex(0.70,0.60,"#geq 4 bjets")

	if "cutDiag" in hist1:
		textBox2=TLatex()
		textBox2.SetNDC()
		textBox2.SetTextSize(0.04) 
		textBox2.DrawLatex(0.70,0.55,"#Delta = 0 GeV")
	
	c.SaveAs(outputDir + hist1 + "_DATA_QCD_"+trigger+".pdf")

