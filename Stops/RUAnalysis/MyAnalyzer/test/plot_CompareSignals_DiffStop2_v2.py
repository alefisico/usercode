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

sample = [ 200,  ]

trigger = '4jet80_6jet60' #str ( sys.argv[1] )
#logscale = int ( sys.argv[4] )

histos = {# 'recoBjets_num':'basicPlots',
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
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj100':'step3plots1D',
#		'massRecoDiBjetDiJetSmallestDeltaM':'step4plots1D'
		}

scaledHistos = {# 'recoBjets_num':'scaleBasicPlots',
#		'recoJets_pt':'scaleBasicPlots',
#		'recoJets_1pt':'scaleBasicPlots',
#		'recoJets_2pt':'scaleBasicPlots',
#		'recoJets_HT':'scaleBasicPlots',
#		'recoBjets_num_Step1':'scaleBasicPlots',
#		'massRecoBjetsCSVM':'scaleStep1plots1D', 
#		'avgMassRecoBjetsCSVM':'scaleStep1plots1D', 
#		'massdijetWORecoBjetsCSVM':'scaleStep2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj50':'scaleStep2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj100':'scaleStep2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj150':'scaleStep2plots1D',
#		'massRecoDiBjetDiJet':'scaleStep3plots1D',
		'avgMassHiggsCandidate_cutDiagStop2bbjj0':'scaleStep3plots1D',
		'avgMassHiggsCandidate_cutDiagStop2bbjj0_resoBasedBin':'scaleStep3plots1D',
		'massHiggsCandidate_cutDiagStop2bbjj0':'scaleStep3plots1D',
		'massStop1Candidate_cutDiagStop2bbjj0':'scaleStep3plots1D',
		'massStop1Candidate_cutDiagStop2bbjj0_resoBasedBin':'scaleStep3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj0':'scaleStep3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj0_resoBasedBin':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj50':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj100':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj10':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj20':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj30':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj40':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj50':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj100':'scaleStep3plots1D',
#		'massRecoDiBjetDiJetSmallestDeltaM':'scaleStep4plots1D'
		}

outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"
kfactor = 1.3  ### 1.2 WOHT300

for st1mass in sample:
	input6 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_650_"+str(st1mass)+"_"+trigger+"_plots.root"
	input8 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_450_"+str(st1mass)+"_"+trigger+"_plots.root"


	f6 = TFile(input6)
	if ( st1mass < 400 ): f8 = TFile(input8)
	
	for hist1, folder in histos.iteritems():
		h6 = f6.Get(folder+'/' + hist1)
		if ( st1mass < 400 ): h8 = f8.Get(folder+'/' + hist1)
	
		h6clone = h6.Clone("h6")
		if ( st1mass < 400 ): h8clone = h8.Clone("h8")
		
		h6clone.SetLineColor(6)
		if ( st1mass < 400 ): h8clone.SetLineColor(4)
		
		h6max = h6.GetMaximum()
		h8max = 0
		if ( st1mass < 400 ): h8max = h8.GetMaximum()
		totalMax = max( h6max, h8max)
		
		c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
		#c.SetLogy()
		h6clone.SetTitle("")
		if 'pt' in hist1: h6clone.GetXaxis().SetTitle("p_T [GeV]")
		elif 'num' in hist1: h6clone.GetXaxis().SetTitle("Number of Jets")
		else: h6clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
		h6clone.GetYaxis().SetTitle("Normalized")
		h6clone.SetMaximum( totalMax*1.2 )
		h6clone.DrawNormalized("hist")
		if ( st1mass < 400 ): h8clone.DrawNormalized("histsame")

		legend=TLegend(0.70,0.7,0.90,0.9)
		legend.SetFillColor(0);
		legend.AddEntry(h6clone,"RPV Stop 650", "l")
		if ( st1mass < 400 ): legend.AddEntry(h8clone,"RPV Stop 450", "l")
		legend.Draw()
		  
		textBox=TLatex()
		textBox.SetNDC()
		textBox.SetTextSize(0.05) 
		textBox.SetTextColor(50)
		textBox.DrawText(0.10,0.91,"CMS Preliminary Simulation")
	
		c.SaveAs(outputDir + hist1 + "_Signals_"+str(st1mass)+"_DiffStop2_"+trigger+"_Normalized.pdf")
		del c
	
	for hist2, folder in scaledHistos.iteritems():
		h6 = f6.Get(folder+'/' + hist2)
		if ( st1mass < 400 ): h8 = f8.Get(folder+'/' + hist2)
	
		h6clone = h6.Clone("h6")
		if ( st1mass < 400 ): h8clone = h8.Clone("h8")
		
		h6clone.SetLineColor(3)
		if ( st1mass < 400 ): h8clone.SetLineColor(4)
		
		h6max = h6.GetMaximum()
		h8max = 0
		if ( st1mass < 400 ): h8max = h8.GetMaximum()
		totalMax = max( h6max, h8max)
		
		c = TCanvas('c_' + hist2, 'c_' + hist2,  10, 10, 750, 500 )
		#c.SetLogy()
		h6clone.SetTitle("")
#		if 'pt' in hist2: h6clone.GetXaxis().SetTitle("p_T [GeV]")
#		elif 'num' in hist2: h6clone.GetXaxis().SetTitle("Number of Jets")
#		else: h6clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
#		if "avg" in hist2: h6clone.GetYaxis().SetTitle("Events / 10 GeV")
#		elif "DiJet" in hist2: h6clone.GetYaxis().SetTitle("QuadJet / 10 GeV")
#		elif "pt" in hist2: h6clone.GetYaxis().SetTitle("Events / 10 GeV")
#		elif "num" in hist2: h6clone.GetYaxis().SetTitle("Events / 10 GeV")
#		else: h6clone.GetYaxis().SetTitle("Dijet / 10 GeV")
		if "avgMass" in hist2: 
			h6clone.GetXaxis().SetTitle("Average of Higgs Candidates Mass [GeV]")
			h6clone.GetYaxis().SetTitle("Events / 10 GeV")
			if "reso" in hist2: h6clone.GetYaxis().SetTitle("dN/dM_{bb} [GeV^{-1}]")
		elif "massHiggs" in hist2: h6clone.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
		elif "massRecoBjets" in hist2: h6clone.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
		elif "massStop1" in hist2: 
			h6clone.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
			h6clone.GetYaxis().SetTitle("Dijets / 10 GeV")
			if "reso" in hist2: h6clone.GetYaxis().SetTitle("dN/dM_{jj} [GeV^{-1}]")
		elif "massdijet" in hist2: h6clone.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
		elif "massStop1" in hist2: h6clone.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
		elif "massRecoDiBjet" in hist2: 
			h6clone.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
			h6clone.GetYaxis().SetTitle("Quadjets / 10 GeV")
			if "reso" in hist2: h6clone.GetYaxis().SetTitle("dN/dM_{bbjj} [GeV^{-1}]")
		else: h6clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
		h6clone.GetYaxis().SetTitleOffset(1.2)
		h6clone.SetMaximum( totalMax*1.2 )
		h6clone.Draw("hist")
		if ( st1mass < 400 ): h8clone.Draw("histsame")
		
		legend=TLegend(0.70,0.7,0.90,0.9)
		legend.SetFillColor(0);
		legend.AddEntry(h6clone,"RPV Stop 650_200", "l")
		if ( st1mass < 400 ): legend.AddEntry(h8clone,"RPV Stop 450_200", "l")
		legend.Draw()
		  
		textBox=TLatex()
		textBox.SetNDC()
		textBox.SetTextSize(0.05) 
		textBox.SetTextColor(50)
		textBox.DrawText(0.10,0.91,"CMS Preliminary Simulation")
	
		textBox1=TLatex()
		textBox1.SetNDC()
		textBox1.SetTextSize(0.04) 
		textBox1.DrawLatex(0.63,0.65,"Signal scaled to 19.5 fb^{-1}")
		
		textBox3=TLatex()
		textBox3.SetNDC()
		textBox3.SetTextSize(0.04) 
		textBox3.DrawLatex(0.70,0.60,"4^{th} jet > 80 GeV ")
		
		textBox4=TLatex()
		textBox4.SetNDC()
		textBox4.SetTextSize(0.04) 
		textBox4.DrawLatex(0.70,0.55,"6^{th} jet > 60 GeV ")
	
		textBox4=TLatex()
		textBox4.SetNDC()
		textBox4.SetTextSize(0.04) 
		textBox4.DrawLatex(0.70,0.50,"#geq 4 bjets")
	
		if "cutDiag" in hist1:
			textBox2=TLatex()
			textBox2.SetNDC()
			textBox2.SetTextSize(0.04) 
			textBox2.DrawLatex(0.70,0.45,"#Delta = 0 GeV")
	
		c.SaveAs(outputDir + hist2 + "_Signals_"+str(st1mass)+"_DiffStop2_"+trigger+".pdf")
		c.SaveAs(outputDir + hist2 + "_Signals_"+str(st1mass)+"_DiffStop2_"+trigger+".eps")
		del c
	
