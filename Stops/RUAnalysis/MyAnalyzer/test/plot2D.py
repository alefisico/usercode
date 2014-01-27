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
gStyle.SetStatY(0.9)
gStyle.SetStatX(0.9)
gStyle.SetStatW(0.20)
gStyle.SetStatH(0.20) 

trigger = "4jet80_6jet60"

samples = [ 'jj_550_200',
#		'jj_350_100', 'jj_350_200', 
#		'jj_450_100', 'jj_450_200', 'jj_450_300',
#		'jj_550_100', 'jj_550_200', 'jj_550_300', 'jj_550_400',
#		'jj_650_100', 'jj_650_200', 'jj_650_300', 'jj_650_400', 'jj_650_500',
#		'jj_750_100', 'jj_750_200', 'jj_750_300', 'jj_750_400', 'jj_750_500'
		]

histos = { 'recoDiBjetDiJet_masspt':'step3plots2D',
#		'recoDiBjetDiJet_cutDiagStop1jj50_masspt':'step3plots2D',
#		'recoDiBjetDiJet_cutDiagStop1jj100_masspt':'step3plots2D',
#		'recoDiBjetDiJet_cutDiagStop2bbjj50_masspt':'step3plots2D',
#		'recoDiBjetDiJet_cutDiagStop2bbjj100_masspt':'step3plots2D',
#		'dijetWORecoBjetsCSVM_masspt':'step2plots2D',
#		'dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj50':'step2plots2D'
		}


outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

for masspoint in samples:
	
	if '250' in masspoint: XSample = 5.57596 
	if '350' in masspoint: XSample = 0.807323
	if '450' in masspoint: XSample = 0.169668
	if '550' in masspoint: XSample = 0.0452067
	if '650' in masspoint: XSample = 0.0139566
	if '750' in masspoint: XSample = 0.00480639
	signalweight = 19500 * XSample/10000

	input1 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+masspoint+"_"+trigger+"_plots.root"

	f1 = TFile(input1)
	
	for hist1, folder in histos.iteritems():
	
		h1 = f1.Get(folder+'/' + hist1)
		
		c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
		c.SetLogz()
		
		h1.Draw("colz")
		h1.SetTitle("")
		h1.GetXaxis().SetTitle('Quadjet scalar p_{T} [GeV]')
		h1.GetYaxis().SetTitle('Quadjet Invariant Mass [GeV]')
		h1.GetYaxis().SetTitleOffset(1.2)
			
		textBox=TLatex()
		textBox.SetNDC()
		textBox.SetTextSize(0.05) 
		textBox.SetTextColor(50)
		textBox.DrawText(0.10,0.91,"CMS Preliminary Simulation")

		textBox2=TLatex()
		textBox2.SetNDC()
		textBox2.SetTextSize(0.04) 
		textBox2.DrawLatex(0.68,0.30,"RPV Stop 550_200")

		textBox3=TLatex()
		textBox3.SetNDC()
		textBox3.SetTextSize(0.04) 
		textBox3.DrawLatex(0.72,0.25,"4^{th} jet > 80 GeV ")
		
		textBox4=TLatex()
		textBox4.SetNDC()
		textBox4.SetTextSize(0.04) 
		textBox4.DrawLatex(0.72,0.20,"6^{th} jet > 60 GeV ")
	
		textBox5=TLatex()
		textBox5.SetNDC()
		textBox5.SetTextSize(0.04) 
		textBox5.DrawLatex(0.72,0.15,"#geq 4 bjets")
		
		line2 = TLine(0,0,2000,2000)
		line2.SetLineColor(kBlack)
		line2.SetLineStyle(10)
		line2.Draw("same")

#		textBox1=TLatex()
#		textBox1.SetNDC()
#		textBox1.SetTextSize(0.04) 
#		textBox1.SetTextColor(50)
#		textBox1.DrawText(0.49,0.85,"Signal "+masspoint)
		
		c.SaveAs(outputDir + hist1 + "_" + masspoint + "_" + trigger + ".pdf")
		del c
