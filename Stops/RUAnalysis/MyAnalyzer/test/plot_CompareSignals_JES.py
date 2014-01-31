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
st1mass = 200
#logscale = int ( sys.argv[4] )
sample = [ 450]#, 550, 650, 750]

histos = {
		'recoBjets_num':'basicPlots',
		'recoJets_pt':'basicPlots',
#		'recoJets_eta':'basicPlots',
		'recoJets_1pt':'basicPlots',
		'recoJets_4pt':'basicPlots',
		'recoJets_6pt':'basicPlots',
#		'recoJets_HT':'basicPlots',
		'recoJets_num':'basicPlots',
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

#		'avgMassHiggsCandidate_cutDiagStop2bbjj0':'step3plots1D',
#		'avgMassHiggsCandidate_cutDiagStop2bbjj0_resoBasedBin':'step3plots1D',
#		'massHiggsCandidate_cutDiagStop2bbjj0':'step3plots1D',
#		'massStop1Candidate_cutDiagStop2bbjj0':'step3plots1D',
#		'massStop1Candidate_cutDiagStop2bbjj0_resoBasedBin':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj0':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj0_resoBasedBin':'step3plots1D',

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
#		'massRecoDiBjetDiJet_cutDiagStop1jj50':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj100':'scaleStep3plots1D',
		'avgMassHiggsCandidate_cutDiagStop2bbjj0':'scaleStep3plots1D',
		'avgMassHiggsCandidate_cutDiagStop2bbjj0_resoBasedBin':'scaleStep3plots1D',
		'massHiggsCandidate_cutDiagStop2bbjj0':'scaleStep3plots1D',
		'massStop1Candidate_cutDiagStop2bbjj0':'scaleStep3plots1D',
		'massStop1Candidate_cutDiagStop2bbjj0_resoBasedBin':'scaleStep3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj0':'scaleStep3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj0_resoBasedBin':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj10':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj20':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj30':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj40':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj50':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj100':'scaleStep3plots1D',
#		'massRecoDiBjetDiJetSmallestDeltaM':'scaleStep4plots1D'
		}


outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

for st2mass in sample:

	input1 = "/uscms/home/algomez/nobackup/files/Stops/Results/st2_h_bb_st1_jj_"+str(st2mass)+"_"+str(st1mass)+"_"+trigger+"_plots.root"
	input2 = "/uscms/home/algomez/nobackup/files/Stops/Results/st2_h_bb_st1_jj_"+str(st2mass)+"_"+str(st1mass)+"_jesup_"+trigger+"_plots.root"
	input3 = "/uscms/home/algomez/nobackup/files/Stops/Results/st2_h_bb_st1_jj_"+str(st2mass)+"_"+str(st1mass)+"_jesdown_"+trigger+"_plots.root"

	f1 = TFile(input1)
	f2 = TFile(input2)
	f3 = TFile(input3)
	
	for hist1, folder in histos.iteritems():
		h1 = f1.Get(folder+'/' + hist1)
		h2 = f2.Get(folder+'/' + hist1)
		h3 = f3.Get(folder+'/' + hist1)
	
		h_Nominal = h1.Clone("h1")
		h_JESUP = h2.Clone("h2")
		h_JESDOWN = h3.Clone("h3")

		h_Nominal.SetLineColor(1)
		h_JESUP.SetLineColor(2)
		h_JESDOWN.SetLineColor(4)
		
#		h1max = h1.GetMaximum()
#		h2max = 0
#		h3max = 0
#		h4max = 0
#		h5max = 0
#		if ( st2mass > 300 ): h2max = h2.GetMaximum()
#		if ( st2mass > 400 ): h3max = h3.GetMaximum()
#		if ( st2mass > 500 ): h4max = h4.GetMaximum()
#		if ( st2mass > 600 ): h5max = h5.GetMaximum()
#		totalMax = max( h1max, h2max, h3max, h4max, h5max )
		
		##################### Calculate Ratio
		h_Ratio_UP = h1.Clone("h1")
		h_Ratio_DOWN = h1.Clone("h1")
		
		for bin in range(0, 200):
			h_Ratio_UP.SetBinContent(bin, 0.)
			h_Ratio_UP.SetBinError(bin, 0.)
			h_Ratio_DOWN.SetBinContent(bin, 0.)
			h_Ratio_DOWN.SetBinError(bin, 0.)

		for ibin in range(1, h_Nominal.GetNbinsX()):

			nominalBin = h_Nominal.GetBinContent(ibin)
			nominalBinErr = h_Nominal.GetBinError(ibin)
			jesupBin = h_JESUP.GetBinContent(ibin)
			jesupBinErr = h_JESUP.GetBinError(ibin)
			jesdownBin = h_JESDOWN.GetBinContent(ibin)
			jesdownBinErr = h_JESDOWN.GetBinError(ibin)
			#print nominalBin, jesupBin, jesdownBin

			if (nominalBin != 0 ):
				ratio_UP_Nom = jesupBin / nominalBin
				ratio_DOWN_Nom = jesdownBin / nominalBin
				h_Ratio_UP.SetBinContent(ibin, ratio_UP_Nom)
				h_Ratio_DOWN.SetBinContent(ibin, ratio_DOWN_Nom)

				### Errors (%Err = err1/val1 + err2/val2 )
				if ( jesupBin !=0 ):
					ErrPercen = (nominalBinErr/nominalBin) + (jesupBinErr/jesupBin)
					ErrJesUp = ratio_UP_Nom*ErrPercen/100.0
					h_Ratio_UP.SetBinError(ibin, ErrJesUp)
				if (jesdownBin !=0 ):
					ErrPercen = (nominalBinErr/nominalBin) + (jesdownBinErr/jesdownBin)
					ErrJesDown = ratio_DOWN_Nom*ErrPercen/100.0
					h_Ratio_DOWN.SetBinError(ibin, ErrJesDown)
				#print ErrPercen, ErrJesUp


		
		c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 750 )
		pad1 = TPad("pad1", "Diff",0,0.30,1.00,1.00,-1)
		pad2 = TPad("pad2", "Ratio",0,0,1.00,0.33,-1);
		pad2.Draw()
		pad1.Draw()

		pad1.cd()
		#c.SetLogy()
		h_Nominal.SetTitle("")
		if "avgMass" in hist1: 
			h_Nominal.GetXaxis().SetTitle("Average of Higgs Candidates Mass [GeV]")
			h_Nominal.GetYaxis().SetTitle("Events / 10 GeV")
			if "reso" in hist1: h_Nominal.GetYaxis().SetTitle("dN/dM_{bb} [GeV^{-1}]")
		elif "massHiggs" in hist1: h_Nominal.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
		elif "massRecoBjets" in hist1: h_Nominal.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
		elif "massStop1" in hist1: 
			h_Nominal.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
			h_Nominal.GetYaxis().SetTitle("Dijets / 10 GeV")
			if "reso" in hist1: h_Nominal.GetYaxis().SetTitle("dN/dM_{jj} [GeV^{-1}]")
		elif "massdijet" in hist1: h_Nominal.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
		elif "massStop1" in hist1: h_Nominal.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
		elif "massRecoDiBjet" in hist1: 
			h_Nominal.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
			h_Nominal.GetYaxis().SetTitle("Quadjets / 10 GeV")
			if "reso" in hist1: h_Nominal.GetYaxis().SetTitle("dN/dM_{bbjj} [GeV^{-1}]")
		else: h_Nominal.GetXaxis().SetTitle("Invariant Mass [GeV]")
		#h_Nominal.SetMaximum( totalMax*1.2 )
		h_Nominal.Draw("histe")
		h_JESUP.Draw("esame")
		h_JESDOWN.Draw("esame")
		
		legend=TLegend(0.70,0.7,0.90,0.9)
		legend.SetFillColor(0);
		legend.AddEntry(h_Nominal, "Nominal", "le")
		legend.AddEntry(h_JESUP, "JES up", "le")
		legend.AddEntry(h_JESDOWN, "JES down", "le")
		legend.Draw()
		  
		textBox=TLatex()
		textBox.SetNDC()
		textBox.SetTextSize(0.05) 
		textBox.SetTextColor(50)
		textBox.DrawLatex(0.10,0.91,"CMS Preliminary Simulation")
		
		textBox1=TLatex()
		textBox1.SetNDC()
		textBox1.SetTextSize(0.04) 
		textBox1.DrawText(0.70,0.65,"jj_"+str(st2mass)+"_"+str(st1mass))
		
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

		pad2.cd()
		h_Ratio_UP.SetTitle("")
#		if "avgMass" in hist1: 
#			h_Ratio_UP.GetXaxis().SetTitle("Average of Higgs Candidates Mass [GeV]")
#		elif "massHiggs" in hist1: h_Ratio_UP.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
#		elif "massRecoBjets" in hist1: h_Ratio_UP.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
#		elif "massStop1" in hist1: 
#			h_Ratio_UP.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
#		elif "massdijet" in hist1: h_Ratio_UP.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
#		elif "massStop1" in hist1: h_Ratio_UP.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
#		elif "massRecoDiBjet" in hist1: 
#			h_Ratio_UP.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
#		else: h_Ratio_UP.GetXaxis().SetTitle("Invariant Mass [GeV]")
		h_Ratio_UP.GetYaxis().SetTitle("Ratio Systematic #frac{Up or Down}{Nominal}")
		h_Ratio_UP.GetYaxis().SetLabelSize(0.08)
		h_Ratio_UP.GetXaxis().SetLabelSize(0.08)
		h_Ratio_UP.GetYaxis().SetTitleSize(0.06)
		h_Ratio_UP.GetYaxis().SetTitleOffset(0.6)
		#h_Ratio_UP.SetMarkerStyle(2)
		#h_Ratio_DOWN.SetMarkerStyle(2)
		h_Ratio_UP.SetLineColor(2)
		h_Ratio_DOWN.SetLineColor(4)
		#h_Ratio_UP.Sumw2()
		h_Ratio_UP.Draw("e")
		h_Ratio_DOWN.Draw("esame")
		
		c.SaveAs(outputDir + hist1 + "_jj_" +str(st2mass)+ "_" + str(st1mass)+ "_JES.pdf")
		c.SaveAs(outputDir + hist1 + "_jj_" +str(st2mass)+ "_" + str(st1mass)+ "_JES.png")
		del c
	
