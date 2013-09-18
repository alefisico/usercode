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

input3 = "/uscms_data/d3/algomez/files/Stops/Results/HT250-500_"+trigger+"_plots.root"
input2 = "/uscms_data/d3/algomez/files/Stops/Results/HT500-1000_"+trigger+"_plots.root"
input1 = "/uscms_data/d3/algomez/files/Stops/Results/HT1000-Inf_"+trigger+"_plots.root"
input4 = "/uscms_data/d3/algomez/files/Stops/Results/data_"+trigger+"_plots_v1.root"
input5 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_450_200_"+trigger+"_plots.root"
input6 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_550_200_"+trigger+"_plots.root"
input7 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_650_200_"+trigger+"_plots.root"
input8 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_750_200_"+trigger+"_plots.root"

outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

#print "Input files: ", input1

kfactor = 1.3  ### 1.2 WOHT250
weightSignal250 = 19500 * 5.57596/100000
weightSignal350 = 19500 * 0.807323/100000
weightSignal450 = 19500 * 0.169668/100000
weightSignal550 = 19500 * 0.0452067/100000
weightSignal650 = 19500 * 0.0139566/100000
weightSignal750 = 19500 * 0.00480639/100000
weightHT250_500 = kfactor * 19500 * 276000/17021377
weightHT500_1000 = kfactor * 19500 * 8426/28122500
weightHT1000_Inf = kfactor * 19500 * 204/13795394

f1 = TFile(input1)
f2 = TFile(input2)
f3 = TFile(input3)
f4 = TFile(input4)
f5 = TFile(input5)
f6 = TFile(input6)
f7 = TFile(input7)
f8 = TFile(input8)

for hist1, folder in histos.iteritems():
	h1 = f1.Get(folder+'/' + hist1)
	h2 = f2.Get(folder+'/' + hist1)
	h3 = f3.Get(folder+'/' + hist1)
	h4 = f4.Get(folder+'/' + hist1)
	h5 = f5.Get(folder+'/' + hist1)
	h6 = f6.Get(folder+'/' + hist1)
	h7 = f7.Get(folder+'/' + hist1)
	h8 = f8.Get(folder+'/' + hist1)

	h1clone = h1.Clone("h1")
	h2clone = h2.Clone("h2")
	#h3clone = h3.Clone("h3")
	h4clone = h4.Clone("h4")
	h5clone = h5.Clone("h5")
	h6clone = h6.Clone("h6")
	h7clone = h7.Clone("h7")
	h8clone = h8.Clone("h8")
	
	if ( scale == 0 ):  
	#31clone.Scale( weightHT250_500 )
		h2clone.Scale( weightHT500_1000 )
		h1clone.Scale( weightHT1000_Inf )
	h5clone.Scale( weightSignal450 )
	h6clone.Scale( weightSignal550 )
	h7clone.Scale( weightSignal650 )
	h8clone.Scale( weightSignal750 )
	
	h1clone.Add( h2clone )
	#h1clone.Add( h3clone )
	
	h1clone.SetLineColor(1)
	h4clone.SetLineColor(2)
	h5clone.SetLineColor(3)
	h6clone.SetLineColor(4)
	h7clone.SetLineColor(5)
	h8clone.SetLineColor(6)
	#h1clone.SetFillColor(10)
	#h4clone.SetFillColor(2)
	h1clone.SetLineWidth(2)
	h4clone.SetLineWidth(2)
	
	c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
	#c.SetLogy()
	if ( scale == 1 ) :
		h4clone.SetTitle("")
		if 'pt' in hist1: h4clone.GetXaxis().SetTitle("p_T [GeV]")
		elif 'num' in hist1: h4clone.GetXaxis().SetTitle("Number of Jets")
		else: h4clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
		h4clone.GetYaxis().SetTitle("Normalized")
		h4clone.DrawNormalized("hist")
		#h1clone.DrawNormalized("histsame")
		h5clone.DrawNormalized("histsame")
		h6clone.DrawNormalized("histsame")
		h7clone.DrawNormalized("histsame")
		h8clone.DrawNormalized("histsame")
	else: 
		h4clone.SetTitle("")
		if 'pt' in hist1: h4clone.GetXaxis().SetTitle("p_T [GeV]")
		elif 'num' in hist1: h4clone.GetXaxis().SetTitle("Number of Jets")
		else: h4clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
		if "avg" in hist1: h4clone.GetYaxis().SetTitle("Events / 10 GeV")
		elif "DiJet" in hist1: h4clone.GetYaxis().SetTitle("QuadJet / 10 GeV")
		elif "pt" in hist1: h4clone.GetYaxis().SetTitle("Events / 10 GeV")
		elif "num" in hist1: h4clone.GetYaxis().SetTitle("Events / 10 GeV")
		else: h4clone.GetYaxis().SetTitle("Dijet / 10 GeV")
		h4clone.GetYaxis().SetTitleOffset(1.2)
		h4clone.Draw("hist")
		#h1clone.Draw("histsame")
		h5clone.Draw("histsame")
		h6clone.Draw("histsame")
		h7clone.Draw("histsame")
		h8clone.Draw("histsame")
	
	legend=TLegend(0.70,0.7,0.90,0.9)
	legend.SetFillColor(0);
	#legend.AddEntry(h1clone,"QCD", "l")
	legend.AddEntry(h4clone,"DATA", "l")
	legend.AddEntry(h5clone,"jj_450_200", "l")
	legend.AddEntry(h6clone,"jj_550_200", "l")
	legend.AddEntry(h7clone,"jj_650_200", "l")
	legend.AddEntry(h8clone,"jj_750_200", "l")
	legend.Draw()
	  
	textBox=TLatex()
	textBox.SetNDC()
	textBox.SetTextSize(0.05) 
	textBox.SetTextColor(50)
	textBox.DrawText(0.10,0.91,"CMS Preliminary")

	if ( scale!=1 ) :
		textBox2=TLatex()
		textBox2.SetNDC()
		textBox2.SetTextSize(0.04) 
		textBox2.SetTextColor(50)
		textBox2.DrawText(0.50,0.65,"QCD and Signal Scale to 19.5/fb")

	if ( scale == 1 ) : c.SaveAs(outputDir + hist1 + "_DATA_QCD_Signal_DiffStop2_"+trigger+"_Normalized.pdf")
	else: c.SaveAs(outputDir + hist1 + "_DATA_QCD_Signal_DiffStop2_"+trigger+".pdf")

