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

sample = [ 100, 200, 300, 400, 500 ]

trigger = '4jet80_6jet60' #str ( sys.argv[1] )
#logscale = int ( sys.argv[4] )

histos = {# 'recoBjets_num':'basicPlots',
#		'recoJets_pt':'basicPlots',
#		'recoJets_1pt':'basicPlots',
#		'recoJets_2pt':'basicPlots',
#		'recoJets_HT':'basicPlots',
#		'recoBjets_num_Step1':'basicPlots',
#		'massRecoBjetsCSVM':'step1plots1D', 
		'avgMassRecoBjetsCSVM':'step1plots1D', 
		'massdijetWORecoBjetsCSVM':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj50':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj100':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj150':'step2plots1D',
#		'massRecoDiBjetDiJet':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj50':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj100':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj0':'step3plots1D',
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
		'avgMassRecoBjetsCSVM':'scaleStep1plots1D', 
		'massdijetWORecoBjetsCSVM':'scaleStep2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj50':'scaleStep2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj100':'scaleStep2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj150':'scaleStep2plots1D',
#		'massRecoDiBjetDiJet':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj50':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj100':'scaleStep3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj0':'scaleStep3plots1D',
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
	input5 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_750_"+str(st1mass)+"_"+trigger+"_plots.root"
	input6 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_650_"+str(st1mass)+"_"+trigger+"_plots.root"
	input7 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_550_"+str(st1mass)+"_"+trigger+"_plots.root"
	input8 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_450_"+str(st1mass)+"_"+trigger+"_plots.root"
	input9 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_350_"+str(st1mass)+"_"+trigger+"_plots.root"


	f5 = TFile(input5)
	f6 = TFile(input6)
	if ( st1mass < 500 ): f7 = TFile(input7)
	if ( st1mass < 400 ): f8 = TFile(input8)
	if ( st1mass < 300 ): f9 = TFile(input9)
	
	for hist1, folder in histos.iteritems():
		h5 = f5.Get(folder+'/' + hist1)
		h6 = f6.Get(folder+'/' + hist1)
		if ( st1mass < 500 ): h7 = f7.Get(folder+'/' + hist1)
		if ( st1mass < 400 ): h8 = f8.Get(folder+'/' + hist1)
		if ( st1mass < 300 ): h9 = f9.Get(folder+'/' + hist1)
	
		h5clone = h5.Clone("h5")
		h6clone = h6.Clone("h6")
		if ( st1mass < 500 ): h7clone = h7.Clone("h7")
		if ( st1mass < 400 ): h8clone = h8.Clone("h8")
		if ( st1mass < 300 ): h9clone = h9.Clone("h9")
		
		h5clone.SetLineColor(7)
		h6clone.SetLineColor(6)
		if ( st1mass < 500 ): h7clone.SetLineColor(5)
		if ( st1mass < 400 ): h8clone.SetLineColor(4)
		if ( st1mass < 300 ): h9clone.SetLineColor(3)
		
		h5max = h5.GetMaximum()
		h6max = h6.GetMaximum()
		h7max = 0
		h8max = 0
		h9max = 0
		if ( st1mass < 500 ): h7max = h7.GetMaximum()
		if ( st1mass < 400 ): h8max = h8.GetMaximum()
		if ( st1mass < 300 ): h9max = h9.GetMaximum()
		totalMax = max( h5max, h6max, h7max, h8max, h9max )
		
		c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
		#c.SetLogy()
		h5clone.SetTitle("")
		if 'pt' in hist1: h5clone.GetXaxis().SetTitle("p_T [GeV]")
		elif 'num' in hist1: h5clone.GetXaxis().SetTitle("Number of Jets")
		else: h5clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
		h5clone.GetYaxis().SetTitle("Normalized")
		h5clone.SetMaximum( totalMax*1.2 )
		h5clone.DrawNormalized("hist")
		h6clone.DrawNormalized("histsame")
		if ( st1mass < 500 ): h7clone.DrawNormalized("histsame")
		if ( st1mass < 400 ): h8clone.DrawNormalized("histsame")
		if ( st1mass < 300 ): h9clone.DrawNormalized("histsame")

		legend=TLegend(0.70,0.7,0.90,0.9)
		legend.SetFillColor(0);
		legend.AddEntry(h5clone,"jj_750_"+str(st1mass), "l")
		legend.AddEntry(h6clone,"jj_650_"+str(st1mass), "l")
		if ( st1mass < 500 ): legend.AddEntry(h7clone,"jj_550_"+str(st1mass), "l")
		if ( st1mass < 400 ): legend.AddEntry(h8clone,"jj_450_"+str(st1mass), "l")
		if ( st1mass < 300 ): legend.AddEntry(h9clone,"jj_350_"+str(st1mass), "l")
		legend.Draw()
		  
		textBox=TLatex()
		textBox.SetNDC()
		textBox.SetTextSize(0.05) 
		textBox.SetTextColor(50)
		textBox.DrawText(0.10,0.91,"CMS Preliminary Simulation")
	
		c.SaveAs(outputDir + hist1 + "_Signals_"+str(st1mass)+"_DiffStop2_"+trigger+"_Normalized.pdf")
		del c
	
	for hist2, folder in scaledHistos.iteritems():
		h5 = f5.Get(folder+'/' + hist2)
		h6 = f6.Get(folder+'/' + hist2)
		if ( st1mass < 500 ): h7 = f7.Get(folder+'/' + hist2)
		if ( st1mass < 400 ): h8 = f8.Get(folder+'/' + hist2)
		if ( st1mass < 300 ): h9 = f9.Get(folder+'/' + hist2)
	
		h5clone = h5.Clone("h5")
		h6clone = h6.Clone("h6")
		if ( st1mass < 500 ): h7clone = h7.Clone("h7")
		if ( st1mass < 400 ): h8clone = h8.Clone("h8")
		if ( st1mass < 300 ): h9clone = h9.Clone("h9")
		
		h5clone.SetLineColor(7)
		h6clone.SetLineColor(6)
		if ( st1mass < 500 ): h7clone.SetLineColor(5)
		if ( st1mass < 400 ): h8clone.SetLineColor(4)
		if ( st1mass < 300 ): h9clone.SetLineColor(3)
		
		h5max = h5.GetMaximum()
		h6max = h6.GetMaximum()
		h7max = 0
		h8max = 0
		h9max = 0
		if ( st1mass < 500 ): h7max = h7.GetMaximum()
		if ( st1mass < 400 ): h8max = h8.GetMaximum()
		if ( st1mass < 300 ): h9max = h9.GetMaximum()
		totalMax = max( h5max, h6max, h7max, h8max, h9max )
		
		c = TCanvas('c_' + hist2, 'c_' + hist2,  10, 10, 750, 500 )
		#c.SetLogy()
		h5clone.SetTitle("")
		if 'pt' in hist2: h5clone.GetXaxis().SetTitle("p_T [GeV]")
		elif 'num' in hist2: h5clone.GetXaxis().SetTitle("Number of Jets")
		else: h5clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
		if "avg" in hist2: h5clone.GetYaxis().SetTitle("Events / 10 GeV")
		elif "DiJet" in hist2: h5clone.GetYaxis().SetTitle("QuadJet / 10 GeV")
		elif "pt" in hist2: h5clone.GetYaxis().SetTitle("Events / 10 GeV")
		elif "num" in hist2: h5clone.GetYaxis().SetTitle("Events / 10 GeV")
		else: h5clone.GetYaxis().SetTitle("Dijet / 10 GeV")
		h5clone.GetYaxis().SetTitleOffset(1.2)
		h5clone.SetMaximum( totalMax*1.2 )
		h5clone.Draw("hist")
		h6clone.Draw("histsame")
		if ( st1mass < 500 ): h7clone.Draw("histsame")
		if ( st1mass < 400 ): h8clone.Draw("histsame")
		if ( st1mass < 300 ): h9clone.Draw("histsame")
		
		legend=TLegend(0.70,0.7,0.90,0.9)
		legend.SetFillColor(0);
		legend.AddEntry(h5clone,"jj_750_"+str(st1mass), "l")
		legend.AddEntry(h6clone,"jj_650_"+str(st1mass), "l")
		if ( st1mass < 500 ): legend.AddEntry(h7clone,"jj_550_"+str(st1mass), "l")
		if ( st1mass < 400 ): legend.AddEntry(h8clone,"jj_450_"+str(st1mass), "l")
		if ( st1mass < 300 ): legend.AddEntry(h9clone,"jj_350_"+str(st1mass), "l")
		legend.Draw()
		  
		textBox=TLatex()
		textBox.SetNDC()
		textBox.SetTextSize(0.05) 
		textBox.SetTextColor(50)
		textBox.DrawText(0.10,0.91,"CMS Preliminary Simulation")
	
		textBox2=TLatex()
		textBox2.SetNDC()
		textBox2.SetTextSize(0.04) 
		textBox2.SetTextColor(50)
		textBox2.DrawText(0.50,0.65,"QCD and Signal Scale to 19.5/fb")
	
		c.SaveAs(outputDir + hist2 + "_Signals_"+str(st1mass)+"_DiffStop2_"+trigger+".pdf")
		del c
	
