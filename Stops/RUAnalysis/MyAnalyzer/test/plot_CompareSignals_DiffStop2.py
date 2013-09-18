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
norm = int ( sys.argv[1] )

histos = { #'massRecoBjetsCSVM':'step1plots1D', 
#		'avgMassRecoBjetsCSVM':'step1plots1D', 
#		'massdijetWORecoBjetsCSVM':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj50':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj100':'step2plots1D',
#		'massdijetWORecoBjetsCSVM_cutDiagStop1jj150':'step2plots1D',
		'massRecoDiBjetDiJet':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj50':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj100':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj50':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj100':'step3plots1D',
#		'massRecoDiBjetDiJetSmallestDeltaM':'step4plots1D'
		}

samples = [ 100, 200, 300, 400, 500 ]
decay1 = 'jj'
#print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

if ( norm == 0 ):
	weightSignal250 = 1
	weightSignal350 = 1
	weightSignal450 = 1
	weightSignal550 = 1
	weightSignal650 = 1
	weightSignal750 = 1
else:
	weightSignal250 = 19500 * 5.57596/100000
	weightSignal350 = 19500 * 0.807323/100000
	weightSignal450 = 19500 * 0.169668/100000
	weightSignal550 = 19500 * 0.0452067/100000
	weightSignal650 = 19500 * 0.0139566/100000
	weightSignal750 = 19500 * 0.00480639/100000


for st1mass in samples:
	input1 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay1+"_750_"+str(st1mass)+"_"+trigger+"_plots.root"
	input2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay1+"_650_"+str(st1mass)+"_"+trigger+"_plots.root"
	input3 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay1+"_550_"+str(st1mass)+"_"+trigger+"_plots.root"
	input4 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay1+"_450_"+str(st1mass)+"_"+trigger+"_plots.root"
	input5 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay1+"_350_"+str(st1mass)+"_"+trigger+"_plots.root"
	input6 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay1+"_250_"+str(st1mass)+"_"+trigger+"_plots.root"
	
	
	#print "Input files: ", input7
	
	f1 = TFile(input1)
	f2 = TFile(input2)
	if ( st1mass < 500 ): f3 = TFile(input3)
	if ( st1mass < 400 ): f4 = TFile(input4)
	if ( st1mass < 300 ): f5 = TFile(input5)
	if ( st1mass < 200 ): f6 = TFile(input6)
	
	
	for hist1, folder in histos.iteritems():
		h1 = f1.Get(folder+'/' + hist1)
		h2 = f2.Get(folder+'/' + hist1)
		if ( st1mass < 500 ): h3 = f3.Get(folder+'/' + hist1)
		if ( st1mass < 400 ): h4 = f4.Get(folder+'/' + hist1)
		if ( st1mass < 300 ): h5 = f5.Get(folder+'/' + hist1)
		if ( st1mass < 200 ): h6 = f6.Get(folder+'/' + hist1)
	
		h1clone = h1.Clone("h1")
		h2clone = h2.Clone("h2")
		if ( st1mass < 500 ): h3clone = h3.Clone("h3")
		if ( st1mass < 400 ): h4clone = h4.Clone("h4")
		if ( st1mass < 300 ): h5clone = h5.Clone("h5")
		if ( st1mass < 200 ): h6clone = h6.Clone("h6")
	
		h1clone.Scale( weightSignal250 )
		h2clone.Scale( weightSignal350 )
		if ( st1mass < 500 ): h3clone.Scale( weightSignal450 )
		if ( st1mass < 400 ): h4clone.Scale( weightSignal550 )
		if ( st1mass < 300 ): h5clone.Scale( weightSignal650 )
		if ( st1mass < 200 ): h6clone.Scale( weightSignal750 )
		
		h1clone.SetLineColor(1)
		h2clone.SetLineColor(2)
		if ( st1mass < 500 ): h3clone.SetLineColor(3)
		if ( st1mass < 400 ): h4clone.SetLineColor(4)
		if ( st1mass < 300 ): h5clone.SetLineColor(5)
		if ( st1mass < 200 ): h6clone.SetLineColor(6)
		h1clone.SetLineWidth(2)
		h2clone.SetLineWidth(2)
		if ( st1mass < 500 ): h3clone.SetLineWidth(2)
		if ( st1mass < 400 ): h4clone.SetLineWidth(2)
		if ( st1mass < 300 ): h5clone.SetLineWidth(2)
		if ( st1mass < 200 ): h6clone.SetLineWidth(2)
	
		maxh1 = h1clone.GetMaximum( )
		maxh2 = h2clone.GetMaximum( )
		maxh3 = 0
		maxh4 = 0
		maxh5 = 0
		if ( st1mass < 500 ): maxh3 = h3clone.GetMaximum( )
		if ( st1mass < 400 ): maxh4 = h4clone.GetMaximum( )
		if ( st1mass < 300 ): maxh5 = h5clone.GetMaximum( )
		if ( st1mass < 200 ): maxh6 = h6clone.GetMaximum( )
		TotalMax = max( maxh1, maxh2, maxh3, maxh4, maxh5, maxh6 )
		TotalMin = min( maxh1, maxh2, maxh3, maxh4, maxh5, maxh6 )
		
		c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
		#c.SetLogy()
		if ( norm == 0 ) :
			h1clone.SetTitle("")
			h1clone.GetXaxis().SetTitle("Mass [GeV]")
			h1clone.GetYaxis().SetTitle("Normalized")
			h1clone.GetYaxis().SetTitleOffset(1.2)
			h1clone.SetMaximum(h1clone.GetMaximum()*2)
			#h1clone.SetMaximum( 1 )
			h1clone.DrawNormalized("hist")
			h2clone.DrawNormalized("histsame")
			if ( st1mass < 500 ): h3clone.DrawNormalized("histsame")
			if ( st1mass < 400 ): h4clone.DrawNormalized("histsame")
			if ( st1mass < 300 ): h5clone.DrawNormalized("histsame")
			if ( st1mass < 200 ): h6clone.DrawNormalized("histsame")
		else: 
			h1clone.SetTitle("")
			h1clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
			if "avg" in hist1: h1clone.GetYaxis().SetTitle("Events / 10 GeV")
			elif "DiJet" in hist1: h1clone.GetYaxis().SetTitle("QuadJet / 10 GeV")
			else: h1clone.GetYaxis().SetTitle("Dijet / 10 GeV")
			h1clone.SetMaximum( TotalMax *1.2)
			h1clone.GetYaxis().SetTitleOffset(1.2)
			h1clone.Draw("hist")
			h2clone.Draw("histsame")
			if ( st1mass < 500 ): h3clone.Draw("histsame")
			if ( st1mass < 400 ): h4clone.Draw("histsame")
			if ( st1mass < 300 ): h5clone.Draw("histsame")
			if ( st1mass < 200 ): h6clone.Draw("histsame")
		
		textBox=TLatex()
		textBox.SetNDC()
		textBox.SetTextSize(0.05) 
		textBox.SetTextColor(50)
		textBox.DrawText(0.10,0.91,"CMS Preliminary Simulation")
		
		legend=TLegend(0.70,0.7,0.90,0.9)
		legend.SetFillColor(0);
		legend.AddEntry(h1clone, decay1+"_750_"+str(st1mass), "l")
		legend.AddEntry(h2clone, decay1+"_650_"+str(st1mass), "l")
		if ( st1mass < 500 ): legend.AddEntry(h3clone, decay1+"_550_"+str(st1mass), "l")
		if ( st1mass < 400 ): legend.AddEntry(h4clone, decay1+"_450_"+str(st1mass), "l")
		if ( st1mass < 300 ): legend.AddEntry(h5clone, decay1+"_350_"+str(st1mass), "l")
		if ( st1mass < 200 ): legend.AddEntry(h6clone, decay1+"_250_"+str(st1mass), "l")
		legend.Draw()
		  
		if ( norm != 0 ) :
			textBox2=TLatex()
			textBox2.SetNDC()
			textBox2.SetTextSize(0.04) 
			textBox2.SetTextColor(50)
			textBox2.DrawText(0.70,0.65,"Scale to 19.5/fb")
		
		if ( norm == 0 ) : c.SaveAs(outputDir + hist1+ "_" +decay1+"_"+str(st1mass)+"_"+ trigger +"_DiffSignals_Normalized.pdf")
		else: c.SaveAs(outputDir + hist1 + "_"+decay1+"_"+str(st1mass)+"_"+ trigger +"_DiffSignals.pdf")
		del c
