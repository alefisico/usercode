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

trigger = '4jet80_6jet60' #str ( sys.argv[1] )
#logscale = int ( sys.argv[4] )
sample = [ 350, 450, 550, 650, 750]

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
		'avgMassHiggsCandidate_cutDiagStop2bbjj0':'step3plots1D',
		'massHiggsCandidate_cutDiagStop2bbjj0':'step3plots1D',
		'massStop1Candidate_cutDiagStop2bbjj0':'step3plots1D',
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

#input3 = "/uscms_data/d3/algomez/files/Stops/Results/HT250-500_"+trigger+"_plots.root"
#input2 = "/uscms_data/d3/algomez/files/Stops/Results/HT500-1000_"+trigger+"_plots.root"
#input1 = "/uscms_data/d3/algomez/files/Stops/Results/HT1000-Inf_"+trigger+"_plots.root"
#input4 = "/uscms_data/d3/algomez/files/Stops/Results/data_"+trigger+"_plots_v1.root"
#input5 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_450_200_"+trigger+"_plots.root"
#input6 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_550_200_"+trigger+"_plots.root"
#input7 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_650_200_"+trigger+"_plots.root"
#input8 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_750_200_"+trigger+"_plots.root"

outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

for st2mass in sample:

	input0 = "/uscms_data/d3/algomez/files/Stops/Results/data_"+trigger+"_plots.root"
	input1 = "/uscms/home/algomez/nobackup/files/Stops/Results/st2_h_bb_st1_jj_"+str(st2mass)+"_100_"+trigger+"_plots.root"
	input2 = "/uscms/home/algomez/nobackup/files/Stops/Results/st2_h_bb_st1_jj_"+str(st2mass)+"_200_"+trigger+"_plots.root"
	input3 = "/uscms/home/algomez/nobackup/files/Stops/Results/st2_h_bb_st1_jj_"+str(st2mass)+"_300_"+trigger+"_plots.root"
	input4 = "/uscms/home/algomez/nobackup/files/Stops/Results/st2_h_bb_st1_jj_"+str(st2mass)+"_400_"+trigger+"_plots.root"
	input5 = "/uscms/home/algomez/nobackup/files/Stops/Results/st2_h_bb_st1_jj_"+str(st2mass)+"_500_"+trigger+"_plots.root"

	f0 = TFile(input0)
	f1 = TFile(input1)
	if ( st2mass > 300 ): f2 = TFile(input2)
	if ( st2mass > 400 ): f3 = TFile(input3)
	if ( st2mass > 500 ): f4 = TFile(input4)
	if ( st2mass > 600 ): f5 = TFile(input5)
	
	for hist1, folder in histos.iteritems():
		h0 = f0.Get(folder+'/' + hist1)
		h1 = f1.Get(folder+'/' + hist1)
		if ( st2mass > 300 ): h2 = f2.Get(folder+'/' + hist1)
		if ( st2mass > 400 ): h3 = f3.Get(folder+'/' + hist1)
		if ( st2mass > 500 ): h4 = f4.Get(folder+'/' + hist1)
		if ( st2mass > 600 ): h5 = f5.Get(folder+'/' + hist1)
		if not h1: continue
	
		h0clone = h0.Clone("h0")
		h1clone = h1.Clone("h1")
		if ( st2mass > 300 ): h2clone = h2.Clone("h2")
		if ( st2mass > 400 ): h3clone = h3.Clone("h3")
		if ( st2mass > 500 ): h4clone = h4.Clone("h4")
		if ( st2mass > 600 ): h5clone = h5.Clone("h5")
		
		h0clone.SetLineColor(1)
		h1clone.SetLineColor(7)
		if ( st2mass > 300 ): h2clone.SetLineColor(6)
		if ( st2mass > 400 ): h3clone.SetLineColor(5)
		if ( st2mass > 500 ): h4clone.SetLineColor(4)
		if ( st2mass > 600 ): h5clone.SetLineColor(3)
		h0clone.SetLineWidth(2)
		
		
		c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
		#c.SetLogy()
		h1clone.SetTitle("")
		if 'pt' in hist1: h1clone.GetXaxis().SetTitle("p_T [GeV]")
		elif 'num' in hist1: h1clone.GetXaxis().SetTitle("Number of Jets")
		elif "avgMass" in hist1: h1clone.GetXaxis().SetTitle("Average of Higgs Candidates Mass [GeV]")
		elif "massHiggs" in hist1: h1clone.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
		elif "massRecoBjets" in hist1: h1clone.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
		elif "massStop1" in hist1: h1clone.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
		elif "massdijet" in hist1: h1clone.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
		elif "massStop1" in hist1: h1clone.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
		elif "massRecoDiBjet" in hist1: h1clone.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
		else: h1clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
		h1clone.GetYaxis().SetTitle("Normalized")
		h1clone.GetYaxis().SetTitleOffset(1.2)
		if 'avg' in hist1: h1clone.SetMaximum( h0clone.GetMaximum()*10)
		h1clone.DrawNormalized("hist")
		h0clone.DrawNormalized("histsame")
		if ( st2mass > 300 ): h2clone.DrawNormalized("histsame")
		if ( st2mass > 400 ): h3clone.DrawNormalized("histsame")
		if ( st2mass > 500 ): h4clone.DrawNormalized("histsame")
		if ( st2mass > 600 ): h5clone.DrawNormalized("histsame")
		
		legend=TLegend(0.70,0.7,0.89,0.89)
		legend.SetFillColor(0);
		legend.SetBorderSize(0);
		legend.AddEntry(h0clone,"DATA", "l")
		legend.AddEntry(h1clone, "jj_"+str(st2mass)+"_100", "l")
		if ( st2mass > 300 ): legend.AddEntry(h2clone, "jj_"+str(st2mass)+"_200", "l")
		if ( st2mass > 400 ): legend.AddEntry(h3clone, "jj_"+str(st2mass)+"_300", "l")
		if ( st2mass > 500 ): legend.AddEntry(h4clone, "jj_"+str(st2mass)+"_400", "l")
		if ( st2mass > 600 ): legend.AddEntry(h5clone, "jj_"+str(st2mass)+"_500", "l")
		legend.Draw()
		  
		textBox=TLatex()
		textBox.SetNDC()
		textBox.SetTextSize(0.05) 
		textBox.SetTextColor(50)
		textBox.DrawText(0.10,0.91,"CMS Preliminary")
	
		textBox3=TLatex()
		textBox3.SetNDC()
		textBox3.SetTextSize(0.04) 
		textBox3.DrawLatex(0.72,0.65,"4^{th} jet > 80 GeV ")
		
		textBox4=TLatex()
		textBox4.SetNDC()
		textBox4.SetTextSize(0.04) 
		textBox4.DrawLatex(0.72,0.60,"6^{th} jet > 60 GeV ")
	
		textBox4=TLatex()
		textBox4.SetNDC()
		textBox4.SetTextSize(0.04) 
		textBox4.DrawLatex(0.72,0.55,"#geq 4 bjets")
	
		if "cutDiag" in hist1:
			textBox5=TLatex()
			textBox5.SetNDC()
			textBox5.SetTextSize(0.04) 
			textBox5.DrawLatex(0.72,0.50,"#Delta = 0 GeV")

		c.SaveAs(outputDir + hist1 + "_DATA_QCD_Signal_"+str(st2mass)+"_DiffStop1_"+trigger+"_Normalized.pdf")
		del c
	
	for hist2, folder in scaledHistos.iteritems():
		h0 = f0.Get(folder.replace('scaleS','s')+'/' + hist2)
		h1 = f1.Get(folder+'/' + hist2)
		if ( st2mass > 300 ): h2 = f2.Get(folder+'/' + hist2)
		if ( st2mass > 400 ): h3 = f3.Get(folder+'/' + hist2)
		if ( st2mass > 500 ): h4 = f4.Get(folder+'/' + hist2)
		if ( st2mass > 600 ): h5 = f5.Get(folder+'/' + hist2)
		if not h1: continue
	
		h0clone = h0.Clone("h0")
		h1clone = h1.Clone("h1")
		if ( st2mass > 300 ): h2clone = h2.Clone("h2")
		if ( st2mass > 400 ): h3clone = h3.Clone("h3")
		if ( st2mass > 500 ): h4clone = h4.Clone("h4")
		if ( st2mass > 600 ): h5clone = h5.Clone("h5")
		
		h0clone.SetLineColor(1)
		h1clone.SetLineColor(7)
		if ( st2mass > 300 ): h2clone.SetLineColor(6)
		if ( st2mass > 400 ): h3clone.SetLineColor(5)
		if ( st2mass > 500 ): h4clone.SetLineColor(4)
		if ( st2mass > 600 ): h5clone.SetLineColor(3)
		h0clone.SetLineWidth(2)
		
		
		c = TCanvas('c_' + hist2, 'c_' + hist2,  10, 10, 750, 500 )
		#c.SetLogy()
		h0clone.SetTitle("")
		if 'pt' in hist2: h0clone.GetXaxis().SetTitle("p_T [GeV]")
		elif 'num' in hist2: h0clone.GetXaxis().SetTitle("Number of Jets")
		elif "avgMass" in hist2: h0clone.GetXaxis().SetTitle("Average of Higgs Candidates Mass [GeV]")
		elif "massHiggs" in hist2: h0clone.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
		elif "massRecoBjets" in hist2: h0clone.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
		elif "massStop1" in hist2: h0clone.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
		elif "massdijet" in hist2: h0clone.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
		elif "massStop1" in hist2: h0clone.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
		elif "massRecoDiBjet" in hist2: h0clone.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
		else: h0clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
		if "avg" in hist2: h0clone.GetYaxis().SetTitle("Events / 10 GeV")
		elif "DiJet" in hist2: h0clone.GetYaxis().SetTitle("QuadJet / 10 GeV")
		elif "pt" in hist2: h0clone.GetYaxis().SetTitle("Events / 10 GeV")
		elif "num" in hist2: h0clone.GetYaxis().SetTitle("Events / 10 GeV")
		else: h0clone.GetYaxis().SetTitle("Dijet / 10 GeV")
		h0clone.GetYaxis().SetTitleOffset(1.2)
		h0clone.Draw("hist")
		h1clone.Draw("histsame")
		if ( st2mass > 300 ): h2clone.Draw("histsame")
		if ( st2mass > 400 ): h3clone.Draw("histsame")
		if ( st2mass > 500 ): h4clone.Draw("histsame")
		if ( st2mass > 600 ): h5clone.Draw("histsame")
		
		legend=TLegend(0.70,0.7,0.89,0.89)
		legend.SetFillColor(0);
		legend.SetBorderSize(0);
		legend.AddEntry(h0clone,"DATA", "l")
		legend.AddEntry(h1clone, "jj_"+str(st2mass)+"_100", "l")
		if ( st2mass > 300 ): legend.AddEntry(h2clone, "jj_"+str(st2mass)+"_200", "l")
		if ( st2mass > 400 ): legend.AddEntry(h3clone, "jj_"+str(st2mass)+"_300", "l")
		if ( st2mass > 500 ): legend.AddEntry(h4clone, "jj_"+str(st2mass)+"_400", "l")
		if ( st2mass > 600 ): legend.AddEntry(h5clone, "jj_"+str(st2mass)+"_500", "l")
		legend.Draw()
		  
		textBox=TLatex()
		textBox.SetNDC()
		textBox.SetTextSize(0.05) 
		textBox.SetTextColor(50)
		textBox.DrawLatex(0.10,0.91,"CMS Preliminary 19.5 fb^{-1} at #sqrt{s} = 8 TeV")
	
		textBox2=TLatex()
		textBox2.SetNDC()
		textBox2.SetTextSize(0.04) 
		textBox2.SetTextColor(50)
		textBox2.DrawLatex(0.30,0.85,"QCD and Signal Scale to 19.5 fb^{-1}")

		textBox3=TLatex()
		textBox3.SetNDC()
		textBox3.SetTextSize(0.04) 
		textBox3.DrawLatex(0.72,0.65,"4^{th} jet > 80 GeV ")
		
		textBox4=TLatex()
		textBox4.SetNDC()
		textBox4.SetTextSize(0.04) 
		textBox4.DrawLatex(0.72,0.60,"6^{th} jet > 60 GeV ")
	
		textBox4=TLatex()
		textBox4.SetNDC()
		textBox4.SetTextSize(0.04) 
		textBox4.DrawLatex(0.72,0.55,"#geq 4 bjets")
	
		if "cutDiag" in hist2:
			textBox5=TLatex()
			textBox5.SetNDC()
			textBox5.SetTextSize(0.04) 
			textBox5.DrawLatex(0.72,0.50,"#Delta = 0 GeV")
		
		c.SaveAs(outputDir + hist2 + "_DATA_QCD_Signal_"+str(st2mass)+"_DiffStop1_"+trigger+".pdf")
		del c
	
