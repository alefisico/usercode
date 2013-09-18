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
gStyle.SetOptStat()
gStyle.SetStatY(0.9)
gStyle.SetStatX(0.9)
gStyle.SetStatW(0.20)
gStyle.SetStatH(0.20) 

trigger = str ( sys.argv[1] )
scale = int ( sys.argv[2] )
#logscale = int ( sys.argv[4] )
histos = { 'massRecoBjetsCSVM':'step1plots1D', 
		'avgMassRecoBjetsCSVM':'step1plots1D', 
		'massdijetWORecoBjetsCSVM':'step2plots1D',
		'massdijetWORecoBjetsCSVM_cutDiagStop1jj50':'step2plots1D',
		'massdijetWORecoBjetsCSVM_cutDiagStop1jj100':'step2plots1D',
		'massdijetWORecoBjetsCSVM_cutDiagStop1jj150':'step2plots1D',
		'massRecoDiBjetDiJet':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop1jj50':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop1jj100':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj50':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj100':'step3plots1D',
		'massRecoDiBjetDiJetSmallestDeltaM':'step4plots1D'
		}

outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

input3 = "/uscms_data/d3/algomez/files/Stops/Results/HT250-500_"+trigger+"_plots.root"
input2 = "/uscms_data/d3/algomez/files/Stops/Results/HT500-1000_"+trigger+"_plots.root"
input1 = "/uscms_data/d3/algomez/files/Stops/Results/HT1000-Inf_"+trigger+"_plots.root"

#print "Input files: ", input1

f1 = TFile(input1)
f2 = TFile(input2)
f3 = TFile(input3)

mcScaleFile = open("MCScaleFactors.txt")
for line in mcScaleFile:
	line = line.rstrip()
	if "HT250-500XS" == line.split()[0]:
		HT250_500XS = float(line.split()[1])
	if "HT500-1000XS" == line.split()[0]: 
		HT500_1000XS = float(line.split()[1])
	if "HT1000-InfXS" == line.split()[0]: 
		HT1000_InfXS = float(line.split()[1])
	if "HT250-500nEvents" == line.split()[0]: 
		HT250_500nEvents = float(line.split()[1])
	if "HT500-1000nEvents" == line.split()[0]: 
		HT500_1000nEvents = float(line.split()[1])
	if "HT1000-InfnEvents" == line.split()[0]: 
		HT1000_InfnEvents = float(line.split()[1])

if ( scale == 1 ):
	weightHT250_500 = 1
	weightHT500_1000 = 1
	weightHT1000_Inf = 1
else:
	weightHT250_500 = 19500*HT250_500XS/HT250_500nEvents
	weightHT500_1000 = 19500*HT500_1000XS/HT500_1000nEvents
	weightHT1000_Inf = 19500*HT1000_InfXS/HT1000_InfnEvents
#print "ScaleFactor for HT250_500: ", weightHT250_500
#print "ScaleFactor for HT500-1000: ", weightHT500_1000
#print "ScaleFactor for HT1000-Inf: ", weightHT1000_Inf

for hist1, folder in histos.iteritems():
	h1 = f1.Get(folder+'/' + hist1)
	h2 = f2.Get(folder+'/' + hist1)
	h3 = f3.Get(folder+'/' + hist1)
	
	h1clone = h1.Clone(hist1)
	h2clone = h2.Clone(hist1)
	#h3clone = h3.Clone(hist1)
	
	#h3clone.Scale( weightHT250_500 )
	h2clone.Scale( weightHT500_1000 )
	h1clone.Scale( weightHT1000_Inf )
	
	h1clone.Add( h2clone )
	#h1clone.Add( h3clone )
	
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
	textBox1.DrawText(0.58,0.85,"QCD MC")
	
	if ( scale!=1 ) :
		textBox2=TLatex()
		textBox2.SetNDC()
		textBox2.SetTextSize(0.04) 
		textBox2.DrawText(0.51,0.75,"Scale to 19.5/fb")
	
	if ( scale == 1 ): c.SaveAs(outputDir + hist1 + "_QCD_"+trigger+".pdf")
	else: c.SaveAs(outputDir + hist1 + "_QCD_"+trigger+"_scale.pdf")

