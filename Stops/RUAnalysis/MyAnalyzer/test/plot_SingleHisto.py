#!/usr/bin/env python

from ROOT import *
import glob,sys

gROOT.Reset()
gStyle.SetOptStat(0)
gStyle.SetStatY(0.9)
gStyle.SetStatX(0.9)
gStyle.SetStatW(0.20)
gStyle.SetStatH(0.20) 

#logscale = int ( sys.argv[7] )
#trigger = str ( sys.argv[1] )
trigger = '4jet80_6jet60'

samples = [ #'jj_250_100',
#		'jj_350_100', 'jj_350_200', 
#		'jj_450_100', 'jj_450_200', 'jj_450_300',
#		'jj_550_100', 'jj_550_200', 'jj_550_300', 'jj_550_400',
#		'jj_650_100', 'jj_650_200', 'jj_650_300', 'jj_650_400', 'jj_650_500',
#		'jj_750_100', 'jj_750_200', 'jj_750_300', 'jj_750_400', 'jj_750_500'
#		'jj_450_200', 'jj_550_200','jj_650_200','jj_750_200',
		'jj_550_200'
		]

histos = { #'massRecoBjetsCSVM':'step1plots1D', 
#		'avgMassRecoBjetsCSVM':'step1plots1D', 
#		'massdijetWORecoBjetsCSVM':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_resoBasedBin':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj50':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj100':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj150':'step2plots1D',
#		'massRecoDiBjetDiJet':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj50':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj100':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj0':'step3plots1D',
#		'massRecoDiBjetDiJet_resoBasedBin':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj0_resoBasedBin':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj10':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj20':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj30':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj40':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj50':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj100':'step3plots1D',
#		'massRecoDiBjetDiJetSmallestDeltaM':'step4plots1D'
		}

scaledHistos = { #'massRecoBjetsCSVM':'scaleStep1plots1D', 
#		'avgMassRecoBjetsCSVM':'scaleStep1plots1D', 
#		'massdijetWORecoBjetsCSVM':'scaleStep2plots1D',
#		'massdijetWORecoBjetsCSVM_resoBasedBin':'scaleStep2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj50':'scaleStep2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj100':'scaleStep2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj150':'scaleStep2plots1D',
#		'massRecoDiBjetDiJet':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj50':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj100':'scaleStep3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj0':'scaleStep3plots1D',
		'massRecoDiBjetDiJet_resoBasedBin':'scaleStep3plots1D',
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

for masspoint in samples:
	
	input1 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+masspoint+"_"+trigger+"_plots.root"
	f1 = TFile(input1)

	for hist1, folder in histos.iteritems():
		h1 = f1.Get(folder+'/' + hist1)
		if not h1: continue
		h1clone = h1.Clone(hist1)

		c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
		#if (logscale ==1): c.SetLogy()
		#c.SetGrid()
		h1clone.SetTitle("")
		h1clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
		if "avg" in hist1: h1clone.GetYaxis().SetTitle("Events / 10 GeV")
		#elif "DiJet" in hist1: h1clone.GetYaxis().SetTitle("QuadJet / 10 GeV")
		elif "DiJet_reso" in hist1: h1clone.GetYaxis().SetTitle("DiJet")
		elif "CSVM_reso" in hist1: h1clone.GetYaxis().SetTitle("QuadJet")
		else: h1clone.GetYaxis().SetTitle("Dijet / 10 GeV")
		h1clone.GetYaxis().SetTitleOffset(1.2)
		h1clone.Sumw2()
		h1clone.Draw()

		textBox=TLatex()
		textBox.SetNDC()
		textBox.SetTextSize(0.05) 
		textBox.SetTextColor(50)
		textBox.DrawText(0.10,0.91,"CMS Preliminary Simulation")

		textBox1=TLatex()
		textBox1.SetNDC()
		textBox1.SetTextSize(0.04) 
		textBox1.SetTextColor(50)
		textBox1.DrawText(0.70,0.85,"Signal "+masspoint)

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

		c.SaveAs(outputDir + hist1 + "_" + masspoint + "_" + trigger +".pdf")
		del c
	
	for hist2, folder in scaledHistos.iteritems():
		h2 = f1.Get(folder+'/' + hist2)
		if not h2: continue
		h2clone = h2.Clone(hist2)

		c = TCanvas('c_' + hist2, 'c_' + hist2,  10, 10, 750, 500 )
		#if (logscale ==1): c.SetLogy()
		#c.SetGrid()
		h2clone.SetTitle("")
		if "avgMass" in hist2: 
			h2clone.GetXaxis().SetTitle("Average of Higgs Candidates Mass [GeV]")
			h2clone.GetYaxis().SetTitle("Events / 10 GeV")
			if "reso" in hist2: h2clone.GetYaxis().SetTitle("dN/dM_{bb} [GeV^{-1}]")
		elif "massHiggs" in hist2: h2clone.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
		elif "massRecoBjets" in hist2: h2clone.GetXaxis().SetTitle("Higgs Candidates Mass (diBjet) [GeV]")
		elif "massStop1" in hist2: 
			h2clone.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
			h2clone.GetYaxis().SetTitle("Dijets / 10 GeV")
			if "reso" in hist2: h2clone.GetYaxis().SetTitle("dN/dM_{jj} [GeV^{-1}]")
		elif "massdijet" in hist2: h2clone.GetXaxis().SetTitle("Lighter Stop Candidates Mass (dijet) [GeV]")
		elif "massStop1" in hist2: h2clone.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
		elif "massRecoDiBjet" in hist2: 
			h2clone.GetXaxis().SetTitle("Heavier Stop Candidates Mass (quadjet) [GeV]")
			h2clone.GetYaxis().SetTitle("Quadjets / 10 GeV")
			if "reso" in hist2: h2clone.GetYaxis().SetTitle("dN/dM_{bbjj} [GeV^{-1}]")
		else: h2clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
		h2clone.GetYaxis().SetTitleOffset(1.2)
		h2clone.Sumw2()
		h2clone.Draw("hist")

		textBox=TLatex()
		textBox.SetNDC()
		textBox.SetTextSize(0.05) 
		textBox.SetTextColor(50)
		textBox.DrawText(0.10,0.91,"CMS Preliminary Simulation")

		textBox1=TLatex()
		textBox1.SetNDC()
		textBox1.SetTextSize(0.04) 
		textBox1.DrawText(0.68,0.85,"RPV Stop 550_200")

		textBox2=TLatex()
		textBox2.SetNDC()
		textBox2.SetTextSize(0.04) 
		textBox2.DrawLatex(0.68,0.80,"Scaled to 19.5 fb^{-1}")

		textBox3=TLatex()
		textBox3.SetNDC()
		textBox3.SetTextSize(0.04) 
		textBox3.DrawLatex(0.68,0.75,"4^{th} jet > 80 GeV ")
		
		textBox4=TLatex()
		textBox4.SetNDC()
		textBox4.SetTextSize(0.04) 
		textBox4.DrawLatex(0.68,0.70,"6^{th} jet > 60 GeV ")
	
		textBox4=TLatex()
		textBox4.SetNDC()
		textBox4.SetTextSize(0.04) 
		textBox4.DrawLatex(0.68,0.65,"#geq 4 bjets")
	
		if "cutDiag" in hist2:
			textBox2=TLatex()
			textBox2.SetNDC()
			textBox2.SetTextSize(0.04) 
			textBox2.DrawLatex(0.68,0.60,"#Delta = 0 GeV")

		c.SaveAs(outputDir + hist2 + "_" + masspoint + "_" + trigger +"_scale.pdf")
		del c
