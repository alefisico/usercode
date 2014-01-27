#!/usr/bin/env python

###################
### Make Fitting
###################

from ROOT import *
import glob,sys

gROOT.Reset()
gStyle.SetOptFit()
gStyle.SetStatY(0.9)
gStyle.SetStatX(0.4)
gStyle.SetStatW(0.15)
gStyle.SetStatH(0.15) 
TVirtualFitter.SetMaxIterations(50000000)		######### Trick to increase number of iterations

###### Input parameters
st2samples = [ 350, 450, 550, 650, 750]
st1mass = 200
decay = 'jj'
histos = {
#		'massRecoBjetsCSVM':'step1plots1D', 
#		'avgMassRecoBjetsCSVM':'step1plots1D', 
#		'massdijetWORecoBjetsCSVM':'step2plots1D',
		'massRecoDiBjetDiJet':'step3plots1D',
#		'massRecoDiBjetDiJet_cutDiagStop2bbjj0':'step3plots1D',
#		'avgMassHiggsCandidate_cutDiagStop2bbjj0':'step3plots1D',
#		'massHiggsCandidate_cutDiagStop2bbjj0':'step3plots1D',
#		'massStop1Candidate_cutDiagStop2bbjj0':'step3plots1D',
		}
#print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]


####### Input files
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

sigma = []
sigmaError = []
for st2mass in st2samples:
	input1 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_preNtuplesPlots.root"
	#input2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_plots.root"
	inputFile1 = TFile(input1)

	for histo, folder in histos.iteritems():
		#if "massdijet" in histo: h1 = inputFile1.Get("plots/BasicPlots/matchJetsStop1_mass")
		h1 = inputFile1.Get("plots/BasicPlots/matchJetsStop2_mass")
		#h2 = inputFile2.Get(folder+'/' + histo)
		
		h1clone = h1.Clone("h1")
		#h2clone = h2.Clone("h2")
		
		GausPreFit = TF1("GausPreFit", "gaus",0,1000);
		maxValue = h1clone.GetMaximumBin()*10
		if (st2mass == 350): h1clone.Fit(GausPreFit,"MRI","",maxValue-50,maxValue+40)

		############ best fit Stop1
		#elif (st1mass == 200): h1clone.Fit(GausPreFit,"MRI","",maxValue-30,maxValue+20)
		#elif (st1mass == 300): h1clone.Fit(GausPreFit,"MRI","",maxValue-20,maxValue+30)
		#elif (st1mass == 400): h1clone.Fit(GausPreFit,"MRI","",maxValue-30,maxValue+30)
		#elif (st1mass == 500): h1clone.Fit(GausPreFit,"MRI","",maxValue-50,maxValue+30)

		elif (st2mass == 450): h1clone.Fit(GausPreFit,"MRI","",maxValue-50,maxValue+40)
		elif (st2mass == 550): h1clone.Fit(GausPreFit,"MRI","",maxValue-60,maxValue+50)
		elif (st2mass == 650): h1clone.Fit(GausPreFit,"MRI","",maxValue-70,maxValue+60)
		elif (st2mass == 750): h1clone.Fit(GausPreFit,"MRI","",maxValue-70,maxValue+60)

		GausSigma = GausPreFit.GetParameter(2)
		GausSigmaError = GausPreFit.GetParError(2)
		sigma.append( GausSigma )
		sigmaError.append( GausSigmaError )
	
		######### Plotting Histograms
		c1 = TCanvas('c1', 'c1',  10, 10, 750, 500 )
		if "massdijet" in histo: 
			h1clone.SetTitle("Resolution of Stop1 Mass Reconstruction")
			h1clone.GetYaxis().SetTitle("Dijet/10.GeV")
			h1clone.GetXaxis().SetTitle("Lighter Stop Reconstruction Invariant Mass [GeV]")
		else: 
			h1clone.SetTitle("Resolution of Stop2 Mass Reconstruction")
			h1clone.GetYaxis().SetTitle("Quadjet/10.GeV")
			h1clone.GetXaxis().SetTitle("Heavier Stop Reconstruction Invariant Mass [GeV]")
		h1clone.Sumw2()
		h1clone.Draw()
	
		textBox2=TLatex()
		textBox2.SetNDC()
		textBox2.SetTextSize(0.04) 
		textBox2.DrawLatex(0.70,0.45,decay+"_"+str(st2mass)+"_"+str(st1mass))
	
		textBox3=TLatex()
		textBox3.SetNDC()
		textBox3.SetTextSize(0.04) 
		textBox3.DrawLatex(0.70,0.40,"4^{th} jet > 80 GeV ")
		
		textBox4=TLatex()
		textBox4.SetNDC()
		textBox4.SetTextSize(0.04) 
		textBox4.DrawLatex(0.70,0.35,"6^{th} jet > 60 GeV ")
	
		textBox4=TLatex()
		textBox4.SetNDC()
		textBox4.SetTextSize(0.04) 
		textBox4.DrawLatex(0.70,0.30,"#geq 4 bjets")
		c1.SaveAs(outputDir+histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_Resolution.pdf")
		del c1

resolution350 = sigma[0]/350
resolution450 = sigma[1]/450
resolution550 = sigma[2]/550
resolution650 = sigma[3]/650
resolution750 = sigma[4]/750
print sigma
print resolution350, resolution450, resolution550, resolution650, resolution750  
hResolution = TH1D("reso", "reso", 5, 300, 800 )
hResolution.SetBinContent(1, sigma[0])
hResolution.SetBinContent(2, sigma[1])
hResolution.SetBinContent(3, sigma[2])
hResolution.SetBinContent(4, sigma[3])
hResolution.SetBinContent(5, sigma[4])

hResolution.SetBinError(1, sigmaError[0])
hResolution.SetBinError(2, sigmaError[1])
hResolution.SetBinError(3, sigmaError[2])
hResolution.SetBinError(4, sigmaError[3])
hResolution.SetBinError(5, sigmaError[4])

LinearFit = TF1("resol", "pol1",300,800);
hResolution.Fit(LinearFit,"MRI","",300,800)

c3 = TCanvas('c3', 'c3',  10, 10, 750, 500 )
gStyle.SetOptFit()
gStyle.SetStatY(0.9)
gStyle.SetStatX(0.4)
gStyle.SetStatW(0.15)
gStyle.SetStatH(0.15) 
hResolution.GetXaxis().SetTitle("Heavier Stop Invariant Mass [GeV]")
hResolution.GetYaxis().SetTitle("Resolution")
hResolution.Sumw2()
hResolution.Draw()
c3.SaveAs(outputDir+histo+decay+"_"+str(st2mass)+"_4jet80_6jet60_Resolution.pdf")
c3.Close()
	
#	c2 = TCanvas('c2', 'c2',  10, 10, 750, 500 )
#	gStyle.SetOptStat(0)
#	h2clone.SetTitle("Difference between MCGen and Reco")
#	if "massdijet" in histo: 
#		h2clone.GetYaxis().SetTitle("Dijet/10 GeV")
#		h2clone.GetXaxis().SetTitle("Lighter Stop Candidates Invariant Mass")
#	else: 
#		h2clone.GetYaxis().SetTitle("Quadjet/10 GeV")
#		h2clone.GetXaxis().SetTitle("Heavier Stop Candidates Invariant Mass")
#	h2clone.SetLineColor(1)
#	h1clone.SetLineColor(2)
#	h2clone.Draw("hist")
#	h1clone.Draw("histsame")
#	
#	legend=TLegend(0.70,0.7,0.89,0.89)
#	legend.SetFillColor(0);
#	legend.SetBorderSize(0);
#	legend.AddEntry(h1clone,"MCGen", "l")
#	legend.AddEntry(h2clone,"Reco", "l")
#	legend.Draw()
#	
#	textBox3=TLatex()
#	textBox3.SetNDC()
#	textBox3.SetTextSize(0.04) 
#	textBox3.DrawLatex(0.70,0.65,"4^{th} jet > 80 GeV ")
#		
#	textBox4=TLatex()
#	textBox4.SetNDC()
#	textBox4.SetTextSize(0.04) 
#	textBox4.DrawLatex(0.70,0.60,"6^{th} jet > 60 GeV ")
#	
#	textBox4=TLatex()
#	textBox4.SetNDC()
#	textBox4.SetTextSize(0.04) 
#	textBox4.DrawLatex(0.70,0.55,"#geq 4 bjets")
#	c2.SaveAs(outputDir+histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_ResolutionDiff.pdf")
#	del c2
#	
	
