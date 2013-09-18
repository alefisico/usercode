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
scale = int ( sys.argv[1] )
#logscale = int ( sys.argv[4] )
sample = [ 450, 550, 650, 750]

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

	if ( scale == 0 ): weightSignal = 1
	else:
		if ( st2mass == 250 ): weightSignal = 19500 * 5.57596/100000
		elif ( st2mass == 350 ): weightSignal = 19500 * 0.807323/100000
		elif ( st2mass == 450 ): weightSignal = 19500 * 0.169668/100000
		elif ( st2mass == 550 ): weightSignal = 19500 * 0.0452067/100000
		elif ( st2mass == 650 ): weightSignal = 19500 * 0.0139566/100000
		elif ( st2mass == 750 ): weightSignal = 19500 * 0.00480639/100000

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
		
		h1clone.Scale( weightSignal )
		if ( st2mass > 300 ): h2clone.Scale( weightSignal )
		if ( st2mass > 400 ): h3clone.Scale( weightSignal )
		if ( st2mass > 500 ): h4clone.Scale( weightSignal )
		if ( st2mass > 600 ): h5clone.Scale( weightSignal )
		
		h1clone.SetLineColor(1)
		if ( st2mass > 300 ): h2clone.SetLineColor(2)
		if ( st2mass > 400 ): h3clone.SetLineColor(3)
		if ( st2mass > 500 ): h4clone.SetLineColor(4)
		if ( st2mass > 600 ): h5clone.SetLineColor(5)
		h1clone.SetLineWidth(2)
		if ( st2mass > 300 ): h2clone.SetLineWidth(2)
		if ( st2mass > 400 ): h3clone.SetLineWidth(2)
		if ( st2mass > 500 ): h4clone.SetLineWidth(2)
		if ( st2mass > 600 ): h5clone.SetLineWidth(2)
		
		
		
		c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
		#c.SetLogy()
		if ( scale == 0 ) :
			h1clone.SetTitle("")
			if 'pt' in hist1: h1clone.GetXaxis().SetTitle("p_T [GeV]")
			elif 'num' in hist1: h1clone.GetXaxis().SetTitle("Number of Jets")
			else: h1clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
			h1clone.GetYaxis().SetTitle("Normalized")
			h1clone.DrawNormalized("hist")
			h0clone.DrawNormalized("histsame")
			if ( st2mass > 300 ): h2clone.DrawNormalized("histsame")
			if ( st2mass > 400 ): h3clone.DrawNormalized("histsame")
			if ( st2mass > 500 ): h4clone.DrawNormalized("histsame")
			if ( st2mass > 600 ): h5clone.DrawNormalized("histsame")
		else: 
			h0clone.SetTitle("")
			if 'pt' in hist1: h0clone.GetXaxis().SetTitle("p_T [GeV]")
			elif 'num' in hist1: h0clone.GetXaxis().SetTitle("Number of Jets")
			else: h0clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
			if "avg" in hist1: h0clone.GetYaxis().SetTitle("Events / 10 GeV")
			elif "DiJet" in hist1: h0clone.GetYaxis().SetTitle("QuadJet / 10 GeV")
			elif "pt" in hist1: h0clone.GetYaxis().SetTitle("Events / 10 GeV")
			elif "num" in hist1: h0clone.GetYaxis().SetTitle("Events / 10 GeV")
			else: h0clone.GetYaxis().SetTitle("Dijet / 10 GeV")
			h0clone.GetYaxis().SetTitleOffset(1.2)
			h0clone.Draw("hist")
			h1clone.Draw("histsame")
			if ( st2mass > 300 ): h2clone.Draw("histsame")
			if ( st2mass > 400 ): h3clone.Draw("histsame")
			if ( st2mass > 500 ): h4clone.Draw("histsame")
			if ( st2mass > 600 ): h5clone.Draw("histsame")
		
		legend=TLegend(0.70,0.7,0.90,0.9)
		legend.SetFillColor(0);
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
	
		if ( scale!=0 ) :
			textBox2=TLatex()
			textBox2.SetNDC()
			textBox2.SetTextSize(0.04) 
			textBox2.SetTextColor(50)
			textBox2.DrawText(0.50,0.65,"QCD and Signal Scale to 19.5/fb")
	
		if ( scale == 0 ) : c.SaveAs(outputDir + hist1 + "_DATA_QCD_Signal_"+str(st2mass)+"_DiffStop1_"+trigger+"_Normalized.pdf")
		else: c.SaveAs(outputDir + hist1 + "_DATA_QCD_Signal_"+str(st2mass)+"_DiffStop1_"+trigger+".pdf")
		del c
	
