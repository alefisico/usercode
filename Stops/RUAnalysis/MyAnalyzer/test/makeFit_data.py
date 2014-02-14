#!/usr/bin/env python

'''
File: makeFit_data.py
Author: Alejandro Gomez Espinosa
Email: gomez@physics.rutgers.edu
Description: Fit data with P4 using simple Root, draw histograms for bin 10 and resoBasedBin,
		create Pull and Residual distributions for both
'''
from ROOT import *
import glob,sys
from array import array

gROOT.Reset()
TVirtualFitter.SetMaxIterations(5000000000)		######### Trick to increase number of iterations

###############################################################
###  Some common labels and utilities                     #####
###############################################################
binSize = 10
massBins = [0, 30, 60, 90, 120, 150, 180, 210, 250, 290, 330, 370, 410, 460, 510, 560, 610, 670, 730, 790, 860, 930, 1000, 1080, 1160, 1240, 1330, 1420, 1520, 1620, 1730, 1840, 2000]

xline = array('d', [0,2000])
yline = array('d', [0,0])
line = TGraph(2, xline, yline)
line.SetLineColor(kRed)
	  
textBox=TLatex(0.10,0.91,"CMS Preliminary 19.5 fb^{-1} at #sqrt{s} = 8 TeV")
textBox.SetNDC()
textBox.SetTextSize(0.05) 
textBox.SetTextColor(kRed)
	
textBox3=TLatex(0.70,0.60,"4^{th} jet > 80 GeV ")
textBox3.SetNDC()
textBox3.SetTextSize(0.04) 
	
textBox4=TLatex(0.70,0.55,"6^{th} jet > 60 GeV ")
textBox4.SetNDC()
textBox4.SetTextSize(0.04) 
	
textBox5=TLatex(0.70,0.50,"#geq 4 bjets")
textBox5.SetNDC()
textBox5.SetTextSize(0.04) 
	
textBox2=TLatex(0.70,0.45,"#Delta = 0 GeV")
textBox2.SetNDC()
textBox2.SetTextSize(0.04) 

###############################################################
###  Fit P4 to data                                       #####
###############################################################
def fit_SimpleRoot( namehisto, folder, file, outputDir ):
	"""docstring for fit_SimpleRoot"""

	inputFile = TFile(file)
	initHisto = inputFile.Get(folder+'/' + namehisto)
	hFit = initHisto.Clone(namehisto)
	
	######## Fit Functions
	print '################################################# Fitting...'
	P4Fit = TF1("P4Fit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",0,2000);
	
	FitStart= 350 #hFit.GetMaximumBin()*binSize-100
	FitEnd  = 1400 #hFit.GetMaximumBin()*binSize+800
	P4Fit.SetParameter(1,10)
	resultFit = hFit.Fit(P4Fit,"MRI","",FitStart,FitEnd)
	dummy = int(resultFit)
	while dummy != 0: resultFit = hFit.Fit(P4Fit,"MRI","",FitStart,FitEnd)
	print '###################### Result Fit:', dummy
	
	#print FitStart, FitEnd
	#P4Fit.SetParameter(0,1)
	#P4Fit.SetParameter(1,-100)
	#P4Fit.SetParameter(2,16)
	#	P4Fit.SetParameter(3, 1)
	#hFit.Fit(P4Fit,"MRI","",FitStart,FitEnd)
	#hFit.Fit(P4Fit,"MRI","",FitStart,FitEnd)
	
	############################### Pull and Residual Distro
	print '############################################################### Calculating Pull and Residual...'
	#alpha = 1 - 0.6827
	
	hPull = initHisto.Clone()
	hResidual = initHisto.Clone()
	
	for bin in range(0,  hFit.GetNbinsX()):
		hPull.SetBinContent(bin, 0.)
		hPull.SetBinError(bin, 0.)
		hResidual.SetBinContent(bin, 0.)
		hResidual.SetBinError(bin, 0.)
	
	for ibin in range(0, hFit.GetNbinsX()):
	
		binCont = initHisto.GetBinContent(ibin)
		binErr = initHisto.GetBinError(ibin)
		valIntegral = P4Fit.Eval( initHisto.GetBinCenter(ibin) ) ### +5 because binSize is 10
		#print binCont, binErr, valIntegral 
	
		diff = (binCont - valIntegral)/ valIntegral
		errDiff = diff * TMath.Sqrt( TMath.Power( P4Fit.GetParError(0) / P4Fit.GetParameter(0),2 ) + TMath.Power( P4Fit.GetParError(1)/ P4Fit.GetParameter(1), 2 )  + TMath.Power( P4Fit.GetParError(2)/ P4Fit.GetParameter(2), 2 )  + TMath.Power( P4Fit.GetParError(3)/ P4Fit.GetParameter(3), 2 ) )
		if (( ibin >= FitStart/binSize) and (binCont != 0) and (ibin <= FitEnd/binSize)):
			pull = (binCont - valIntegral)/ binErr
			#print pull
			hPull.SetBinContent(ibin, pull)
			hPull.SetBinError(ibin, 1.0)
	
			hResidual.SetBinContent(ibin, diff)
			#hResidual.SetBinError(ibin, binErr/valIntegral )
			hResidual.SetBinError(ibin, errDiff )#/valIntegral)
	############################################################################### 
	
	##### Histo to save
	print '############################################################### Drawing 10GeV Plots...'
	h_data = hFit.Clone()
	h_QCDP4 = P4Fit.Clone()    
			
	hFit.SetTitle("")
	hFit.GetXaxis().SetTitle("Heavy stop reconstruction invariant mass [GeV]")
	hFit.GetYaxis().SetTitle("Quadruplets / 10 GeV")
	hFit.GetYaxis().SetTitleOffset(1.2);

	hPull.SetTitle("Pull Distribution")
	hPull.GetXaxis().SetTitle("Heavy stop reconstruction invariant mass [GeV]")
	hPull.GetYaxis().SetTitle("Pulls")
	#hPull.GetYaxis().SetTitleSize(0.1)
	#hPull.GetYaxis().SetTitleOffset(1.2);
	hPull.SetMarkerStyle(7)

	hResidual.SetTitle("Residual Distribution")
	hResidual.GetXaxis().SetTitle("Heavy stop reconstruction invariant mass [GeV]")
	hResidual.GetYaxis().SetTitle("Residuals")
	#hResidual.GetYaxis().SetTitleSize(0.1)
	#hResidual.GetYaxis().SetTitleOffset(1.2);
	hResidual.SetMarkerStyle(7)
	#hResidual.SetMaximum(2)

	legend=TLegend(0.25,0.15,0.45,0.25)
	legend.SetFillColor(0);
	legend.SetBorderSize(0);
	legend.AddEntry(hFit,"Data", "l")
	legend.AddEntry(P4Fit,"P4 parameter fit", "l")    

	######### Plotting Histograms
	c1 = TCanvas('c1', 'c1',  10, 10, 750, 1000 )
	pad1 = TPad("pad1", "Fit",0,0.50,1.00,1.00,-1)
	pad2 = TPad("pad2", "Pull",0,0.25,1.00,0.50,-1);
	pad3 = TPad("pad3", "Residual",0,0,1.00,0.25,-1);
	pad1.Draw()
	pad2.Draw()
	pad3.Draw()
	
	pad1.cd()
	pad1.SetLogy()
	gStyle.SetOptFit()
	gStyle.SetStatY(0.9)
	gStyle.SetStatX(0.9)
	gStyle.SetStatW(0.15)
	gStyle.SetStatH(0.15) 
	hFit.Sumw2()
	hFit.Draw()
	legend.Draw()
	textBox.Draw()
	textBox3.Draw()
	textBox4.Draw()
	textBox5.Draw()
	textBox2.Draw()
	  
	
	pad2.cd()
	gStyle.SetOptStat(0)
	hPull.Sumw2()
	hPull.Draw("e")
	line.Draw("same")
	
	pad3.cd()
	gStyle.SetOptStat(0)
	#hResidual.Sumw2()
	hResidual.Draw("e")
	line.Draw("same")
	
	c1.SaveAs(outputDir+namehisto+"_data_FitP4Gauss_simpleRoot.pdf")
	c1.SaveAs(outputDir+namehisto+"_data_FitP4Gauss_simpleRoot.png")
	del c1
	
	c2 = TCanvas('c2', 'c2',  10, 10, 750, 500 )
	gStyle.SetOptStat(0)
	hPull.Sumw2()
	hPull.Draw("e")
	line.Draw("same")
	
	c2.SaveAs(outputDir+namehisto+"_data_FitP4Gauss_Pull_simpleRoot.pdf")
	c2.SaveAs(outputDir+namehisto+"_data_FitP4Gauss_Pull_simpleRoot.png")
	del c2
	
	c3 = TCanvas('c3', 'c3',  10, 10, 750, 500 )
	
	gStyle.SetOptStat(0)
	hResidual.GetYaxis().SetLimits(-5,5)
	hResidual.SetMaximum(1)
	#hResidual.Sumw2()
	hResidual.Draw("e")
	line.Draw("same")
	
	c3.SaveAs(outputDir+namehisto+"_data_FitP4Gauss_Residual_simpleRoot.pdf")
	c3.SaveAs(outputDir+namehisto+"_data_FitP4Gauss_Residual_simpleRoot.png")
	del c3
	
	c4 = TCanvas('c4', 'c4',  10, 10, 750, 500 )
	gStyle.SetOptFit()
	gStyle.SetStatY(0.9)
	gStyle.SetStatX(0.9)
	gStyle.SetStatW(0.15)
	gStyle.SetStatH(0.15) 
	c4.SetLogy()
	hFit.Sumw2()
	hFit.Draw()
	legend.Draw()
	textBox.Draw()
	textBox3.Draw()
	textBox4.Draw()
	textBox5.Draw()
	textBox2.Draw()
	
	
	c4.SaveAs(outputDir+namehisto+"_data_FitP4Gauss_FitOnly_simpleRoot.pdf")
	c4.SaveAs(outputDir+namehisto+"_data_FitP4Gauss_FitOnly_simpleRoot.png")
	del c4

	plotResoBasedBin( file, namehisto, folder, P4Fit.GetParameter(1), P4Fit.GetParameter(2), P4Fit.GetParameter(3), P4Fit.GetParError(1), P4Fit.GetParError(2), P4Fit.GetParError(3) )

#########################################################
#####  Plot P4 function in Resolution Based Binning  ####
#########################################################
def plotResoBasedBin(file, h, folder, p1, p2, p3, p1Err, p2Err, p3Err):

	print '############################################################### Drawing resoBasedBin Plots...'
	histo = h + '_resoBasedBin'
	inputFile = TFile(file)
	h_Initial = inputFile.Get(folder+'/' + histo )
	hFit = h_Initial.Clone(histo)
	
	################################ Pull and Residual Distro
	hPull = h_Initial.Clone()
	hResidual = h_Initial.Clone()

	#listCorrectP4 = []
	for ibin in range(0, len(massBins)-1):
		hPull.SetBinContent(ibin, 0.)
		hPull.SetBinError(ibin, 0.)
		hResidual.SetBinContent(ibin+1, 0.)
		hResidual.SetBinError(ibin+1, 0.)
		
		resoBasedBinSize = massBins[ibin + 1] - massBins[ibin]
		hFit.SetBinContent( ibin, h_Initial.GetBinContent(ibin)/ resoBasedBinSize )
		hFit.SetBinError( ibin, h_Initial.GetBinError(ibin)/ resoBasedBinSize )

	######### Fit Functions
	P4Fit = TF1("P4Fit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",350,1400)
	FitStart = 350
	FitEnd = 1400
	P4Fit.FixParameter(1,p1)
	P4Fit.FixParameter(2,p2)
	P4Fit.FixParameter(3,p3)
	hFit.Fit(P4Fit,'B',"",FitStart,FitEnd)
		
	#for bin in range(11, len(massBins)-1):
	for bin in range(0, hFit.GetNbinsX()):
		binCont = hFit.GetBinContent(bin)
		binErr = hFit.GetBinError(bin)
		binMiddlepoint = hFit.GetBinCenter(bin)
		valIntegral = (P4Fit.Eval(binMiddlepoint))
		##print binMiddlepoint, valIntegral, binCont

		#errDiff = sqrt(binCont)
		diff = (binCont - valIntegral)/ valIntegral
		errDiff = diff * TMath.Sqrt( TMath.Power( P4Fit.GetParError(0) / P4Fit.GetParameter(0),2 ) + TMath.Power( p1Err/ p1, 2 )  + TMath.Power( p2Err/ p2, 2 )  + TMath.Power( p3Err/ p3, 2 ) )
		if (( massBins[bin] >= FitStart) and (binCont != 0) and ( massBins[bin] <= FitEnd) ):
			pull = (binCont - valIntegral)/ binErr #errDiff
			#print bin*massBins[bin], binCont, valIntegral, errDiff, pull
			hPull.SetBinContent(bin, pull)
			hPull.SetBinError(bin, 1.0)
	
			hResidual.SetBinContent(bin, diff)
			hResidual.SetBinError(bin, errDiff ) #hFit.GetBinError(bin)/valIntegral )#resError )#errDiff/valIntegral)
	############################################################################### 

	###### Formatting Plots 
	hFit.SetTitle("")
	hFit.GetXaxis().SetTitle("Heavy stop reconstruction invariant mass [GeV]")
	#hFit.GetYaxis().SetTitle("Dijets/10 GeV")
	hFit.GetYaxis().SetTitle("dN/dM_{bbjj} [GeV^{-1}]")
	hFit.GetYaxis().SetTitleOffset(1.2)

	hPull.SetTitle("")
	hPull.GetXaxis().SetTitle("Heavy stop reconstruction invariant mass [GeV]")
	hPull.GetYaxis().SetTitle("Pull Distribution")
	#hPull.GetYaxis().SetTitleSize(0.1)
	#hPull.GetYaxis().SetTitleOffset(1.2)
	hPull.SetMarkerStyle(7)

	hResidual.SetTitle("")
	hResidual.GetXaxis().SetTitle("Heavy stop reconstruction invariant mass [GeV]")
	hResidual.GetYaxis().SetTitle("Residuals")
	#hResidual.GetYaxis().SetTitleSize(0.1)
	#hResidual.GetYaxis().SetTitleOffset(1.2)
	hResidual.SetMarkerStyle(7)
	#hResidual.GetYaxis().SetLimits(-5,5)
	hResidual.SetMaximum(1)

	######## Legends and text
	legend=TLegend(0.25,0.15,0.45,0.25)
	legend.SetFillColor(0)
	legend.SetBorderSize(0)
	legend.AddEntry(hFit,"Data", "l")
	legend.AddEntry(P4Fit,"P4 parameter fit", "l")    

	hPull.GetXaxis().SetTitle("")
	#hPull.Sumw2()

	hResidual.GetXaxis().SetTitle("")
	#hResidual.SetMaximum(3)
	#hResidual.SetMinimum(-2)
	#hResidual.Sumw2()

	######### Plotting Histograms
	
	c2 = TCanvas('c2', 'c2',  10, 10, 750, 500 )
	gStyle.SetOptStat(0)
	hPull.Draw("e")
	line.Draw("L")
	textBox.Draw("same")
	c2.SaveAs(outputDir+histo+"_data_FitP4_Pull_simpleRoot.pdf")
	c2.SaveAs(outputDir+histo+"_data_FitP4_Pull_simpleRoot.png")
	del c2
	
	c3 = TCanvas('c3', 'c3',  10, 10, 750, 500 )
	gStyle.SetOptStat(0)
	hResidual.Draw("e")
	line.Draw("L")
	textBox.Draw("same")
	c3.SaveAs(outputDir+histo+"_data_FitP4_Residual_simpleRoot.pdf")
	c3.SaveAs(outputDir+histo+"_data_FitP4_Residual_simpleRoot.png")
	del c3

	c1 = TCanvas('c1', 'c1',  10, 10, 750, 1000 )
	pad1 = TPad("pad1", "Fit",0,0.50,1.00,1.00,-1)
	pad2 = TPad("pad2", "Pull",0,0.25,1.00,0.50,-1)
	pad3 = TPad("pad3", "Residual",0,0,1.00,0.25,-1)
	pad1.Draw()
	pad2.Draw()
	pad3.Draw()
	
	pad1.cd()
	pad1.SetLogy()
	gStyle.SetOptFit()
	gStyle.SetStatY(0.9)
	gStyle.SetStatX(0.9)
	gStyle.SetStatW(0.15)
	gStyle.SetStatH(0.13) 
	hFit.Sumw2()
	hFit.Draw()
	P4Fit.Draw("lsame")
	legend.Draw()
	textBox.Draw()
	textBox3.Draw()
	textBox4.Draw()
	textBox5.Draw()
	textBox2.Draw()
	
	pad2.cd()
	gStyle.SetOptStat(0)
	hPull.GetYaxis().SetLabelSize(0.08)
	hPull.GetXaxis().SetLabelSize(0.08)
	hPull.GetYaxis().SetTitleSize(0.06)
	hPull.GetYaxis().SetTitleOffset(0.6)
	hPull.Draw("E")
	line.Draw("L")
	
	pad3.cd()
	gStyle.SetOptStat(0)
	hResidual.GetYaxis().SetLabelSize(0.08)
	hResidual.GetXaxis().SetLabelSize(0.08)
	hResidual.GetYaxis().SetTitleSize(0.06)
	hResidual.GetYaxis().SetTitleOffset(0.6)
	hResidual.Draw("e")
	line.Draw("L")
	
	c1.SaveAs(outputDir+histo+"_data_FitP4_simpleRoot.pdf")
	c1.SaveAs(outputDir+histo+"_data_FitP4_simpleRoot.png")
	del c1
	
	c4 = TCanvas('c4', 'c4',  10, 10, 750, 500 )
	gStyle.SetOptFit()
	gStyle.SetStatY(0.9)
	gStyle.SetStatX(0.9)
	gStyle.SetStatW(0.15)
	gStyle.SetStatH(0.15) 
	c4.SetLogy()
	hFit.Sumw2()
	hFit.Draw()
	textBox.Draw("same")
	legend.Draw()
	textBox.Draw()
	textBox2.Draw()
	textBox3.Draw()
	textBox4.Draw()
	textBox5.Draw()
	
	c4.SaveAs(outputDir+histo+"_data_FitP4_FitOnly_simpleRoot.pdf")
	c4.SaveAs(outputDir+histo+"_data_FitP4_FitOnly_simpleRoot.png")
	del c4


#########################################################
if __name__ == '__main__':

	###### Input parameters
	namehisto = str ( sys.argv[1] )
	folder = 'step3plots1D' #str ( sys.argv[2] )
	
	####### Input files
	inputFile = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_6jet60_plots.root"
	outputDir = "/uscms_data/d3/algomez/files/Stops/FitPlots/"

	if not sys.argv[1]: 'You need to give the name of the plot, e.g.: ./makeFit_data.py massRecoDiBjetDiJet_cutDiagStop2bbjj0 -b'
	fit_SimpleRoot( namehisto, folder, inputFile, outputDir )
	print 'Well done :)'
