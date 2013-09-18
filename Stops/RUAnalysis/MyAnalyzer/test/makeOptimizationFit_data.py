#!/usr/bin/env python

###################
### Make Fitting
###################

from ROOT import *
import glob,sys

gROOT.Reset()
gStyle.SetOptFit()
gStyle.SetStatY(0.9)
gStyle.SetStatX(0.9)
gStyle.SetStatW(0.15)
gStyle.SetStatH(0.15) 
TVirtualFitter.SetMaxIterations(50000000)		######### Trick to increase number of iterations

###### Input parameters

histos = { #'massRecoDiBjetDiJet':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj0':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj10':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj20':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj30':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj40':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj50':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj60':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj70':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj80':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj90':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop1jj100':'step3plots1D',
#
		'massRecoDiBjetDiJet_cutDiagStop2bbjj0':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj10':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj20':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj30':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj40':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj50':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj60':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj70':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj80':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj90':'step3plots1D',
		'massRecoDiBjetDiJet_cutDiagStop2bbjj100':'step3plots1D'
		}

####### Input files
input1 = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_6jet60_plots.root"
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

binSize = 10
#print "Input files: ", input1

f1 = TFile(input1)

for hist1, folder in histos.iteritems():
	h1 = f1.Get(folder+'/' + hist1)
	
	h1clone = h1.Clone(hist1)
	
	######## Fit Functions
	#P4PreFit = TF1("P4PreFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",0,1000);
	LandauFit = TF1("LandauFit", "landau",350,950);
	
	#print h1clone.GetMaximumBin(), PreFitStart, PreFitEnd
	
	#if not "cutDiag" in hist1:
	#	PreFitStart= h1clone.GetMaximumBin()*binSize-10
	#	PreFitEnd  = h1clone.GetMaximumBin()*binSize+1000
	#	P4PreFit.SetParameter(0, 0.0002)
	#	P4PreFit.SetParameter(1, -1.44)
	#	P4PreFit.SetParameter(2, 7)
	#	P4PreFit.SetParameter(3, 0.7)
	#else:
	#	PreFitStart= h1clone.GetMaximumBin()*binSize-30
	#	PreFitEnd  = h1clone.GetMaximumBin()*binSize+1000
	#	P4PreFit.SetParameter(0,1)
	#	P4PreFit.SetParameter(1,50)
	#	P4PreFit.SetParameter(2, 6)
	#	P4PreFit.SetParameter(3, 1)

	if "Stop2" in hist1:
		PreFitStart= 250
		PreFitEnd  = 600
	else:
		PreFitStart= 350
		PreFitEnd  = 700

	h1clone.Fit(LandauFit,"MRI","",PreFitStart,PreFitEnd)
	
			
	######### Plotting Histograms
	c1 = TCanvas('c1', 'c1',  10, 10, 750, 500 )
	h1clone.SetTitle("")
	h1clone.GetXaxis().SetTitle("Invariant Mass [GeV]")
	h1clone.GetYaxis().SetTitle("Quadjet / 10 GeV")
	h1clone.GetYaxis().SetTitleOffset(1.2);
	h1clone.Draw()
	textBox=TLatex()
	textBox.SetNDC()
	textBox.SetTextSize(0.05) 
	textBox.SetTextColor(50)
	textBox.DrawLatex(0.10,0.91,"CMS Preliminary 19.5 fb^{-1} at #sqrt{s} = 8 TeV")
	
	textBox1=TLatex()
	textBox1.SetNDC()
	textBox1.SetTextSize(0.05) 
	textBox1.SetTextColor(50)
	textBox1.DrawText(0.53,0.85,"DATA")
	
	c1.SaveAs(outputDir+hist1+"_data_4jet80_6jet60_FitLandau.pdf")
	c1.Close()
	del c1

