#!/usr/bin/env python

from ROOT import *
import glob,sys

gROOT.Reset()
gStyle.SetOptStat()
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
		'jj_450_200'
		]

histos = { 'massRecoBjetsCSVM':'step1plots1D', 
		'avgMassRecoBjetsCSVM':'step1plots1D', 
		'massdijetWORecoBjetsCSVM':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj50':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj100':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj150':'step2plots1D',
		'massRecoDiBjetDiJet':'step3plots1D',
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


outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

for masspoint in samples:
	
	if '250' in masspoint: XSample = 5.57596 
	if '350' in masspoint: XSample = 0.807323
	if '450' in masspoint: XSample = 0.169668
	if '550' in masspoint: XSample = 0.0452067
	if '650' in masspoint: XSample = 0.0139566
	if '750' in masspoint: XSample = 0.00480639
	signalweight = 19500 * XSample/100000
	diffscales = [ 1 , signalweight ]
	#print "ScaleFactor for st2_h_bb_st1_"+decay1+"_"+str(st2mass1)+"_"+str(st1mass1) , weight

	input1 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+masspoint+"_"+trigger+"_plots.root"
	f1 = TFile(input1)

	for hist1, folder in histos.iteritems():
		for weight in diffscales:
			h1 = f1.Get(folder+'/' + hist1)
			if not h1: continue
			h1clone = h1.Clone(hist1)
			h1clone.Scale( weight )

			c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
			#if (logscale ==1): c.SetLogy()
			#c.SetGrid()
			h1clone.Draw()
			h1clone.SetTitle("")
			h1clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
			if "avg" in hist1: h1clone.GetYaxis().SetTitle("Events / 10 GeV")
			elif "DiJet" in hist1: h1clone.GetYaxis().SetTitle("QuadJet / 10 GeV")
			else: h1clone.GetYaxis().SetTitle("Dijet / 10 GeV")
			h1clone.GetYaxis().SetTitleOffset(1.2)
	
			textBox=TLatex()
			textBox.SetNDC()
			textBox.SetTextSize(0.05) 
			textBox.SetTextColor(50)
			textBox.DrawText(0.10,0.91,"CMS Preliminary Simulation")
	
			textBox1=TLatex()
			textBox1.SetNDC()
			textBox1.SetTextSize(0.04) 
			textBox1.SetTextColor(50)
			textBox1.DrawText(0.49,0.85,"Signal "+masspoint)
	
			if ( weight!=1 ) :
				textBox2=TLatex()
				textBox2.SetNDC()
				textBox2.SetTextSize(0.04) 
				textBox2.SetTextColor(50)
				textBox2.DrawText(0.51,0.80,"Scale to 19.5/fb")
	
			if ( weight == 1 ): c.SaveAs(outputDir + hist1 + "_" + masspoint + "_" + trigger +".pdf")
			else: c.SaveAs(outputDir + hist1 + "_" + masspoint + "_" + trigger +"_scale.pdf")
			del c
	
