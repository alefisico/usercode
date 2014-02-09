#!/usr/bin/env python

'''
File: plot_CompareSignals_JES.py
Author: Alejandro Gomez Espinosa
Email: gomez@physics.rutgers.edu
Description: Draw comparative plots for Jet Energy Scale Uncertainties.
'''

from ROOT import *
import glob,sys

gROOT.Reset()
gStyle.SetOptStat(0)

textBox=TLatex(0.10,0.91,"CMS Preliminary Simulation")
textBox.SetNDC()
textBox.SetTextSize(0.05) 
textBox.SetTextColor(50)

###############################################################
### Draw comparative plots for JES                        #####
###############################################################
def plotJEScomp( st2mass, dir, outputDir, histos ):
	"""docstring for plotJEScomp"""

	for mass in st2mass:
	
		nominalNameFile = dir+"st2_h_bb_st1_jj_"+str(mass)+"_200_4jet80_6jet60_plots.root"
		jesupNameFile = dir+"st2_h_bb_st1_jj_"+str(mass)+"_200_jesup_4jet80_6jet60_plots.root"
		jesdownNameFile = dir+"st2_h_bb_st1_jj_"+str(mass)+"_200_jesdown_4jet80_6jet60_plots.root"
	
		nominalFile = TFile(nominalNameFile)
		jesupFile = TFile(jesupNameFile)
		jesdownFile = TFile(jesdownNameFile)
		
		for histo, folder in histos.iteritems():
			h_RawNominal = nominalFile.Get(folder+'/' + histo)
			h_RawJesup = jesupFile.Get(folder+'/' + histo)
			h_RawJesdown = jesdownFile.Get(folder+'/' + histo)
		
			h_Nominal = axisLabels( h_RawNominal.Clone(), histo )
			h_JESUP = h_RawJesup.Clone()
			h_JESDOWN = h_RawJesdown.Clone()
	
			h_Nominal.SetLineColor(1)
			h_JESUP.SetLineColor(2)
			h_JESDOWN.SetLineColor(4)

			#### Calculate Ratio
			#h_Ratio_UP, h_Ratio_DOWN = calcRatioCorrUnc( h_Nominal, h_JESUP, h_JESDOWN )	### fancy way to do it
			h_Ratio_UP = h_JESUP.Clone()
			h_Ratio_DOWN = h_JESDOWN.Clone()
			h_Ratio_UP.Divide( h_Nominal )
			h_Ratio_DOWN.Divide( h_Nominal )
			####################################################

			c = TCanvas('c_' + histo, 'c_' + histo,  10, 10, 750, 750 )
			pad1 = TPad("pad1", "Diff",0,0.30,1.00,1.00,-1)
			pad2 = TPad("pad2", "Ratio",0,0,1.00,0.33,-1);
			pad2.Draw()
			pad1.Draw()
	
			pad1.cd()
			#pad1.SetLogy()
			#pad1.SetLogx()
			#h_Nominal.SetMaximum( totalMax*1.2 )
			h_Nominal.Draw("histe")
			h_JESUP.Draw("histesame")
			h_JESDOWN.Draw("histesame")

			textBox.Draw()
			
			legend=TLegend(0.70,0.7,0.90,0.9)
			legend.SetFillColor(0);
			legend.AddEntry(h_Nominal, "Nominal", "le")
			legend.AddEntry(h_JESUP, "JES up", "le")
			legend.AddEntry(h_JESDOWN, "JES down", "le")
			legend.Draw()
			
			DrawLabels( histo, mass )
	
			pad2.cd()
			#pad2.SetLogx()
			h_Ratio_UP.SetTitle("")
			h_Ratio_UP.GetYaxis().SetTitle("Ratio Systematic #frac{Up or Down}{Nominal}")
			h_Ratio_UP.GetYaxis().SetLabelSize(0.06)
			h_Ratio_UP.GetXaxis().SetLabelSize(0.08)
			h_Ratio_UP.GetYaxis().SetTitleSize(0.06)
			h_Ratio_UP.GetYaxis().SetTitleOffset(0.6)
			h_Ratio_UP.SetMinimum(0.5)
			h_Ratio_UP.SetMaximum(1.5)
			h_Ratio_UP.SetLineColor(2)
			h_Ratio_DOWN.SetLineColor(4)
			h_Ratio_UP.Sumw2()
			h_Ratio_UP.Draw("e")
			h_Ratio_DOWN.Draw("esame")
			
			c.SaveAs(outputDir + histo + "_jj_" +str(mass)+ "_200_JES.pdf")
			c.SaveAs(outputDir + histo + "_jj_" +str(mass)+ "_200_JES.png")
			del c
	
######################## calculate max of several histos (in progress)
#		h_RawNominalmax = h_RawNominal.GetMaximum()
#		h_RawJesupmax = 0
#		h_RawJesdownmax = 0
#		h4max = 0
#		h5max = 0
#		if ( st2mass > 300 ): h_RawJesupmax = h_RawJesup.GetMaximum()
#		if ( st2mass > 400 ): h_RawJesdownmax = h_RawJesdown.GetMaximum()
#		if ( st2mass > 500 ): h4max = h4.GetMaximum()
#		if ( st2mass > 600 ): h5max = h5.GetMaximum()
#		totalMax = max( h_RawNominalmax, h_RawJesupmax, h_RawJesdownmax, h4max, h5max )
			

###############################################################
### Draw Axis Info                                        #####
###############################################################
def axisLabels( histo, histoName ):
	"""docstring for axisLabels"""

	histo.SetTitle("")
	histo.GetYaxis().SetTitleOffset(1.3)
	binSize = histo.GetXaxis().GetBinWidth(10)
	if "avgMass" in histoName: 
		histo.GetXaxis().SetTitle("Average of Higgs Candidates Mass [GeV]")
		histo.GetYaxis().SetTitle("Events / "+str(binSize)+" GeV")
		if "reso" in histoName: histo.GetYaxis().SetTitle("dN/dM_{bb} [GeV^{-1}]")
	elif "massHiggs" in histoName: histo.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
	elif "massRecoBjets" in histoName: histo.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
	elif "massStop1" in histoName: 
		histo.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
		histo.GetYaxis().SetTitle("Dijets / "+str(binSize)+" GeV")
		if "reso" in histoName: histo.GetYaxis().SetTitle("dN/dM_{jj} [GeV^{-1}]")
	elif "massdijet" in histoName: histo.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
	elif "massRecoDiBjet" in histoName: 
		histo.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadruplets) [GeV]")
		histo.GetYaxis().SetTitle("Quadruplets / "+str(binSize)+" GeV")
		if "reso" in histoName: histo.GetYaxis().SetTitle("dN/dM_{bbjj} [GeV^{-1}]")
	elif "Jets_num" in histoName: 
		histo.GetXaxis().SetTitle("Number of Jets")
		histo.GetXaxis().SetRange(2,15)
		histo.GetYaxis().SetTitle("Events")
	elif "Bjets_num" in histoName: 
		histo.GetXaxis().SetTitle("Number of BJets")
		histo.GetXaxis().SetRange(2,15)
		histo.GetYaxis().SetTitle("Events")
	elif "pt" in histoName: 
		histo.GetXaxis().SetTitle("p_{T} [GeV]")
		histo.GetYaxis().SetTitle("Events / "+str(binSize)+" GeV")
	elif "eta" in histoName: 
		histo.GetXaxis().SetTitle("#eta")
		histo.GetYaxis().SetTitle("Events / "+str(binSize))
	else: 
		histo.GetXaxis().SetTitle("Invariant Mass [GeV]")

	return histo



###############################################################
### Draw Labels                                           #####
###############################################################
def DrawLabels( histo, mass ):
	"""docstring for DrawLabels"""

	textBox1=TLatex()
	textBox1.SetNDC()
	textBox1.SetTextSize(0.04) 
	textBox1.DrawText(0.70,0.65,"jj_"+str(mass)+"_200")
	
	if not 'checkJES' in histo:
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
	
		if "cutDiag" in histo:
			textBox2=TLatex()
			textBox2.SetNDC()
			textBox2.SetTextSize(0.04) 
			textBox2.DrawLatex(0.70,0.45,"#Delta = 0 GeV")
	else:
		textBox3=TLatex()
		textBox3.SetNDC()
		textBox3.SetTextSize(0.04) 
		textBox3.DrawLatex(0.70,0.60,"JES verification")

		textBox4=TLatex()
		textBox4.SetNDC()
		textBox4.SetTextSize(0.04) 
		if 'pt' in histo: textBox4.DrawLatex(0.70,0.55,"|#eta_{jet}| < 0.1")
		elif 'eta' in histo: 
			textBox4.DrawLatex(0.70,0.55,"90 GeV < p_{T}^{jet}| < 110 GeV")
			

###############################################################
### Calculate the ratio between Up or Down/Nominal        #####
### here, the uncertainties are correlated                #####
### the actual formula to calculate this I didn't do it   #####
###############################################################
def calcRatioCorrUnc( hNominal, hUp, hDown ):
	"""docstring for calcRatioCorrUnc"""

	h_Ratio_UP = hNominal.Clone()
	h_Ratio_DOWN = hNominal.Clone()
	
	for bin in range(0, 200):
		h_Ratio_UP.SetBinContent(bin, 0.)
		h_Ratio_UP.SetBinError(bin, 0.)
		h_Ratio_DOWN.SetBinContent(bin, 0.)
		h_Ratio_DOWN.SetBinError(bin, 0.)

	for ibin in range(1, hNominal.GetNbinsX()):

		nominalBin = hNominal.GetBinContent(ibin)
		nominalBinErr = hNominal.GetBinError(ibin)
		jesupBin = hUp.GetBinContent(ibin)
		jesupBinErr = hUp.GetBinError(ibin)
		jesdownBin = hDown.GetBinContent(ibin)
		jesdownBinErr = hDown.GetBinError(ibin)
		#print nominalBin, jesupBin, jesdownBin

		if (nominalBin != 0 ):

			ratio_UP_Nom = jesupBin / nominalBin
			ratio_DOWN_Nom = jesdownBin / nominalBin
			h_Ratio_UP.SetBinContent(ibin, ratio_UP_Nom)
			h_Ratio_DOWN.SetBinContent(ibin, ratio_DOWN_Nom)

			ErrNominal = nominalBinErr/nominalBin
			if ( jesupBin !=0 ):
				errSumUp = nominalBin + jesupBin
				if (errSumUp > 0):
					nominalProd = nominalBin * jesupBinErr
					jesupProd = nominalBinErr * jesupBin
					sum =  (nominalProd * nominalProd) + (jesupProd * jesupProd) - (1.8 * nominalProd * jesupProd)  #assume 90% correlation
					if sum > 0: ErrJesUp = 4.0 * sqrt(sum) / (errSumUp * errSumUp);
				h_Ratio_UP.SetBinError(ibin, ErrJesUp)
			if (jesdownBin !=0 ):
				errSumDown = nominalBin + jesupBin
				if (errSumDown > 0):
					nominalProd = nominalBin * jesdownBinErr
					jesdownProd = nominalBinErr * jesdownBin
					sum =  (nominalProd * nominalProd) + (jesdownProd * jesdownProd) - (1.8 * nominalProd * jesdownProd)  #assume 90% correlation
					if sum > 0: ErrJesDown = 4.0 * sqrt(sum) / (errSumDown * errSumDown);
				h_Ratio_DOWN.SetBinError(ibin, ErrJesDown)

	return h_Ratio_UP, h_Ratio_DOWN
			

##################################################################
if __name__ == '__main__':

	st2mass = [ 450 ]#, 550, 650, 750]
	inputDir = "/uscms_data/d3/algomez/files/Stops/Results/"
	outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"
	
	histos = { }
	#histos['checkJES_recoJets_pt'] = 'basicPlots'
	#histos['checkJES_recoJets_eta'] = 'basicPlots'
	histos['recoBjets_num'] = 'basicPlots'
	histos['recoJets_pt'] = 'basicPlots'
	histos['recoJets_eta'] = 'basicPlots'
	histos['recoJets_1pt'] = 'basicPlots'
	histos['recoJets_4pt'] = 'basicPlots'
	histos['recoJets_6pt'] = 'basicPlots'
	histos['recoJets_HT'] = 'basicPlots'
	histos['recoJets_num'] = 'basicPlots'
	histos['avgMassHiggsCandidate_cutDiagStop2bbjj0'] = 'step3plots1D'
	histos['avgMassHiggsCandidate_cutDiagStop2bbjj0_resoBasedBin'] = 'step3plots1D'
	histos['massHiggsCandidate_cutDiagStop2bbjj0'] = 'step3plots1D'
	histos['massStop1Candidate_cutDiagStop2bbjj0'] = 'step3plots1D'
	histos['massStop1Candidate_cutDiagStop2bbjj0_resoBasedBin'] = 'step3plots1D'
	histos['massRecoDiBjetDiJet_cutDiagStop2bbjj0'] = 'step3plots1D'
	histos['massRecoDiBjetDiJet_cutDiagStop2bbjj0_resoBasedBin'] = 'step3plots1D'
	
	
	plotJEScomp( st2mass, inputDir, outputDir, histos )

################# Add Scale Plots
#		'recoJets_pt':'scaleBasicPlots',
#		'recoJets_1pt':'scaleBasicPlots',
#		'recoJets_2pt':'scaleBasicPlots',
	
