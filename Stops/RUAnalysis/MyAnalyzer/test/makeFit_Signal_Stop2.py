#!/usr/bin/env python

'''
File: makeFit_Signal_Stop2.py
Author: Alejandro Gomez Espinosa
Email: gomez@physics.rutgers.edu
Description: Fit 10 GeV bin histogram for Signal with root 
'''

from ROOT import *
import glob,sys, datetime

gROOT.Reset()
gStyle.SetOptStat(0)
gStyle.SetOptFit()
gStyle.SetStatY(0.9)
gStyle.SetStatX(0.9)
gStyle.SetStatW(0.12)
gStyle.SetStatH(0.12) 
TVirtualFitter.SetMaxIterations(5000000000000)		######### Trick to increase number of iterations


###############################################################
### Some initial parameters                               #####
###############################################################
binSize = 10
#massBins = [0, 30, 60, 90, 120, 150, 180, 210, 250, 290, 330, 370, 410, 460, 510, 560, 610, 670, 730, 790, 860, 930, 1000, 1080, 1160, 1240, 1330, 1420, 1520, 1620, 1730, 1840, 2000]
eventsGenerated = 100000

###############################################################
### Main Fit                                              #####
###############################################################
def fitP4Signal( input, histo, mass, XS, output, outputDir ):
	"""docstring for fitP4Signal"""

	inputFile = TFile(input)

	######## Fit Functions
	P4PreFit = TF1("P4PreFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",300,1200);
	GausPreFit = TF1("GausPreFit", "gaus",0,2000);
	P4GausFit= TF1("P4GausFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))+gaus(4)",300,1200);
	
	######## Get Histograms 
	tmp_h_Reco = inputFile.Get('step3plots1D/'+histo)
	h_PreFitP4 = tmp_h_Reco.Clone(histo)
	h_P4GausFit = tmp_h_Reco.Clone(histo)
	h_PreFitGauss = tmp_h_Reco.Clone(histo)
	
	############## Pre-Fitting combinations for Gausian and P4 separated
	if ( mass == 450 ): 
		PreFitStart = 500
		h_PreFitGauss.Fit(GausPreFit,"MRI","", 400, 500)
	elif ( mass == 550 ): 
		PreFitStart = 600
		h_PreFitGauss.Fit(GausPreFit,"MRI","", 500, 600 )
	elif ( mass == 650 ): 
		PreFitStart = 300
		h_PreFitGauss.Fit(GausPreFit,"MRI","", 600, 700)
	else: 
		PreFitStart = 400
		h_PreFitGauss.Fit(GausPreFit,"MRI","", 600, 700)
	
	PreFitEnd  = 1500
	P4PreFit.SetParameter(0,1)
	P4PreFit.SetParameter(1,10)
	h_PreFitP4.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)
	h_PreFitP4.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)
	
	
	################# Gaussian Fit Match 
	#print GausPreFit.GetChisquare(), GausPreFit.GetNDF() 
	#print round((GausPreFit.GetChisquare()-GausPreFit.GetNDF())/sqrt(2*GausPreFit.GetNDF()),3), ' standards deviation away from the mean.'
	
	
	################## Real Fitting
	P4GausFit.SetParameter(0,P4PreFit.GetParameter(0));				
	P4GausFit.SetParameter(1,P4PreFit.GetParameter(1));
	P4GausFit.SetParameter(2,P4PreFit.GetParameter(2));
	P4GausFit.SetParameter(3,P4PreFit.GetParameter(3));
	P4GausFit.SetParameter(4,GausPreFit.GetParameter(0));
	P4GausFit.SetParameter(5,GausPreFit.GetParameter(1));
	#P4GausFit.SetParLimits(5,GausPreFit.GetParameter(1)-30,GausPreFit.GetParameter(1)+30);
	#if (mass == 650): P4GausFit.SetParLimits(5, mass-30, massBins[minBinValue[0]]-10)
	#if (mass == 550): P4GausFit.SetParLimits(5, 520, 580)
	#if (mass == 450): P4GausFit.SetParLimits(5, 420, 480)
	#print massBins[minBinValue[0]], massBins[minBinValue[0]]-30, massBins[minBinValue[0]]+10
	#P4GausFit.SetParameter(5,mass);
	#
	P4GausFit.SetParameter(6,GausPreFit.GetParameter(2));
	FitStart = 300
	FitEnd = 2000
	statfit = h_P4GausFit.Fit(P4GausFit,"MRI","",FitStart,FitEnd);
	#while statfit != 0: h_P4GausFit.Fit(P4GausFit,"MRI","",FitStart,FitEnd);
	#print 'Standard Deviation of Fit: ', round((P4GausFit.GetChisquare()-P4GausFit.GetNDF())/sqrt(2*P4GausFit.GetNDF()),3)
	#
	
	##### Histo to save
	h_signal = h_P4GausFit.Clone()
	
	############ Calculate Acceptance
	GausOnly = TF1("GausOnly","gaus", 300, 1000)
	P4Only = TF1("P4Only","[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))", 300, 2000 )
	#		
	P4Only.SetParameter(0,P4GausFit.GetParameter(0))
	P4Only.SetParameter(1,P4GausFit.GetParameter(1))
	P4Only.SetParameter(2,P4GausFit.GetParameter(2))
	P4Only.SetParameter(3,P4GausFit.GetParameter(3))
	#		
	GausOnly.SetParameter(0,P4GausFit.GetParameter(4))
	GausOnly.SetParameter(1,P4GausFit.GetParameter(5))
	GausOnly.SetParameter(2,P4GausFit.GetParameter(6))
	#		
	GausOnly.SetParError(0,P4GausFit.GetParError(4))
	GausOnly.SetParError(1,P4GausFit.GetParError(5))
	GausOnly.SetParError(2,P4GausFit.GetParError(6))
	#		
	SigP4 = GausOnly.Integral(0,1000)
	#FullSigP4 = h_P4GausFit.Integral(0,1000)
	GaussIntegral = SigP4/binSize/eventsGenerated 
	#FullIntegral = FullSigP4/eventsGenerated
	Acceptance = abs(sqrt(2*3.14)*GausOnly.GetParameter(2)*GausOnly.GetParameter(0)/binSize/eventsGenerated)
	#GaussYield = sqrt(2*3.14)*GausOnly.GetParameter(2)*GausOnly.GetParameter(0)/binSize
	print 'Sample: ', output
	print SigP4, GaussIntegral, binSize
	print 'Gaus Integral: ', round(GaussIntegral,5)
	print 'Gaus Acceptance: ', round(Acceptance, 5)
	print 'Gaus Mean: ', round(GausOnly.GetParameter(1),5)
	print 'Gaus Width: ', round(GausOnly.GetParameter(2),5)
	#, 'Gaus Yield: ', round(GaussYield, 2), ' Full Acceptance: ', round(FullIntegral, 5)
	##print GausOnly.GetParameter(0) , 1/GausOnly.GetParameter(0), GausOnly.GetParameter(2), 1/(sqrt(2*3.14)*GausOnly.GetParameter(2))
	saveInfo( '/uscms_data/d3/algomez/files/Stops/Limits/', mass, XS, Acceptance, GausOnly.GetParameter(1), abs(GausOnly.GetParameter(2))  )
	
	########### Draw Histos
	c3 = TCanvas('c3', 'c3',  10, 10, 750, 500 )
	h_P4GausFit.GetXaxis().SetTitle("Heavier Stop Reconstruction Invariant Mass [GeV]")
	if not "resoBased" in histo: h_P4GausFit.GetYaxis().SetTitle("Quadruplets / 10 GeV")
	else: h_P4GausFit.GetYaxis().SetTitle("Quadruplets")
	h_P4GausFit.GetYaxis().SetTitleOffset(1.2);
	h_P4GausFit.SetTitle("")
	#if ( mass == 450): h_P4GausFit.SetMaximum(1300)
	#if ( mass == 550): h_P4GausFit.SetMaximum(2000)
	#if ( mass == 650): h_P4GausFit.SetMaximum(2000)
	h_P4GausFit.Sumw2()
	h_P4GausFit.Draw()
	GausOnly.SetLineColor(3)
	GausOnly.Draw("same")
	P4Only.SetLineColor(1)
	P4Only.Draw("same")
	
	textBox=TLatex()
	textBox.SetNDC()
	textBox.SetTextSize(0.05) 
	textBox.SetTextColor(50)
	textBox.DrawLatex(0.10,0.91,"CMS Preliminary Simulation")
	
	legend=TLegend(0.69,0.43,0.89,0.60)
	legend.SetFillColor(0);
	legend.SetBorderSize(0);
	legend.AddEntry(h_P4GausFit, "jj_"+str(mass)+"_200", "le")
	legend.AddEntry(P4GausFit, "P4 + Gauss", "l")
	legend.AddEntry(GausOnly, "Gauss Only", "l")
	legend.AddEntry(P4Only, "P4 Only", "l")
	legend.Draw()
	
	textBox3=TLatex()
	textBox3.SetNDC()
	textBox3.SetTextSize(0.04) 
	textBox3.DrawLatex(0.70,0.30,"4^{th} jet > 80 GeV ")
	
	textBox4=TLatex()
	textBox4.SetNDC()
	textBox4.SetTextSize(0.04) 
	textBox4.DrawLatex(0.70,0.25,"6^{th} jet > 60 GeV ")
		
	textBox4=TLatex()
	textBox4.SetNDC()
	textBox4.SetTextSize(0.04) 
	textBox4.DrawLatex(0.70,0.20,"#geq 4 bjets")
		
	if "cutDiag" in histo:
		textBox2=TLatex()
		textBox2.SetNDC()
		textBox2.SetTextSize(0.04) 
		textBox2.DrawLatex(0.70,0.15,"#Delta = 0 GeV")
	
	c3.SaveAs(outputDir+output+"_FitP4Gauss_simpleRoot.pdf")
	c3.SaveAs(outputDir+output+"_FitP4Gauss_simpleRoot.png")
	del c3

###############################################################
### Save info into a file                                 #####
###############################################################
def saveInfo( Dir, st2mass , XS, Acceptance, GausMean, GausSigma ):
	
	try:
		listName = [ '#Mass', 'XS', 'Acceptance', 'GausMean', 'GausSigma' ]
		list = [ str(st2mass), str(XS), str(Acceptance), str(GausMean), str(GausSigma) ]

		if (st2mass == 450): 
			with open( Dir+"signalInfo_simpleRoot.txt", "w") as myfile:
				myfile.write('#####################################  ')
				myfile.write(datetime.datetime.now().ctime()+'\n') 	### fancy way to keep track of the date
				myfile.write("\t".join(listName)+ '\n')
				myfile.write("\t".join(list)+ '\n')
		else:
			with open( Dir+"signalInfo_simpleRoot.txt", "a") as myfile:
				myfile.write("\t".join(list)+ '\n')
			
	except IOError:
		print 'File ', filename, 'does not exist.'

###########################################################
if __name__ == '__main__':

	###### Input parameters
	st2mass = [ 450, 550, 650, 750 ]
	listXS  = [ 0.169668, 0.0452067, 0.0139566, 0.00480639 ]	
	histo = 'massRecoDiBjetDiJet_cutDiagStop2bbjj0'
	jes = str ( sys.argv[1] )
	outputDir = "/uscms_data/d3/algomez/files/Stops/FitPlots/"
	
	####### Input files
	k = 0 #dummy
	for mass in st2mass:
		if jes:
			input = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_"+str(mass)+"_200_4jet80_6jet60_plots.root"
			output = histo+"_jj_"+str(mass)+"_200_4jet80_6jet60"
		else:
			input = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_"+str(mass)+"_200_jes"+jes+"_4jet80_6jet60_plots.root"
			output = histo+"_jj_"+str(mass)+"_200_jes"+jes+"_4jet80_6jet60"

		fitP4Signal( input, histo, mass, listXS[k], output, outputDir )
		k+=1
