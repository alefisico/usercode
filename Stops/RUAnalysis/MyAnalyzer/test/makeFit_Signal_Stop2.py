#!/usr/bin/env python

###################
### Make Fitting
###################

from ROOT import *
import glob,sys

gROOT.Reset()
gStyle.SetOptStat(0)
gStyle.SetOptFit()
gStyle.SetStatY(0.9)
gStyle.SetStatX(0.9)
gStyle.SetStatW(0.12)
gStyle.SetStatH(0.12) 
TVirtualFitter.SetMaxIterations(5000000000000)		######### Trick to increase number of iterations

###### Input parameters
st1mass = int (sys.argv[1])
st2mass = int (sys.argv[2])
histo = str ( sys.argv[3] )
jes = str ( sys.argv[4] )
#folder = str ( sys.argv[5] )
#print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]

decay = 'jj'

####### Input files
if jes == 'no' :
	#input1 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_preNtuplesPlots.root"
	input2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_plots.root"
	output = histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60"
else:
	input2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_jes"+jes+"_4jet80_6jet60_plots.root"
	output = histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_jes"+jes+"_4jet80_6jet60"
#inputFile1 = TFile(input1)
inputFile2 = TFile(input2)
#print "Input files: ", input2
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

binSize = 10
massBins = [0, 30, 60, 90, 120, 150, 180, 210, 250, 290, 330, 370, 410, 460, 510, 560, 610, 670, 730, 790, 860, 930, 1000, 1080, 1160, 1240, 1330, 1420, 1520, 1620, 1730, 1840, 2000]
eventsGenerated = 100000

minBinValue = min(enumerate(massBins), key=lambda x: abs(x[1]-st2mass))
minBinValueBin = (minBinValue[1] + massBins[minBinValue[0]+1])/2
######## Fit Functions
P4PreFit = TF1("P4PreFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",300,1200);
GausPreFit = TF1("GausPreFit", "gaus",0,2000);
P4GausFit= TF1("P4GausFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))+gaus(4)",300,1200);

######## Get Histograms 
tmp_h_Reco = inputFile2.Get('step3plots1D/'+histo)
h_PreFitP4 = tmp_h_Reco.Clone(histo)
h_P4GausFit = tmp_h_Reco.Clone(histo)
h_PreFitGauss = tmp_h_Reco.Clone(histo)
#tmp_h_Match = inputFile1.Get('plots/AnalysisPlots/massMatchDiBjetDiJet')
#h_PreFitGauss = tmp_h_Match.Clone("tmp_Stop2_Match")

############## Pre-Fitting combinations
if "reso" in histo:
	if ( st2mass == 450 ): PreFitStart= h_PreFitP4.GetMaximumBin()*binSize+50  ### BEST for 450_200
	elif ( st2mass == 350 ): PreFitStart= h_PreFitP4.GetMaximumBin()*binSize+50   #### BEST for 350_200
	elif ( st2mass == 550 ): PreFitStart= h_PreFitP4.GetMaximumBin()*binSize+30   #### BEST for 550_200
	elif ( st2mass == 650 ): PreFitStart= h_PreFitP4.GetMaximumBin()*binSize+30   #### BEST for 650_200
	else: PreFitStart= 400
	PreFitEnd  = h_PreFitP4.GetMaximumBin()*binSize+800
	h_PreFitGauss.Fit(GausPreFit,"MRI","",st2mass-50,st2mass+50)
	#PreFitEnd  = 1200
	#P4PreFit.SetParameter(0,1)
else:
	if ( st2mass == 550 ): 
		PreFitStart = massBins[minBinValue[0]-3]		#### BEST for 650_200
		for i in range(-2,1):
			h_PreFitP4.SetBinContent(minBinValue[0]+i, 0)
			h_PreFitP4.SetBinError(minBinValue[0]+i, 0)
		h_PreFitGauss.Fit(GausPreFit,"MRI","", massBins[minBinValue[0]-2], massBins[minBinValue[0]+2])
	if ( st2mass == 650 ): 
		PreFitStart = massBins[minBinValue[0]-7]		#### BEST for 650_200
		#print massBins[minBinValue[0]-3]
		for i in range(-10,10):
			h_PreFitP4.SetBinContent(minBinValue[0]+i, 0)
			h_PreFitP4.SetBinError(minBinValue[0]+i, 0)
		#h_PreFitGauss.Fit(GausPreFit,"MRI","", massBins[minBinValue[0]-2], massBins[minBinValue[0]+2])
		h_PreFitGauss.Fit(GausPreFit,"MRI","",st2mass-30,st2mass+30)
	elif ( st2mass == 450 ): 
		PreFitStart =  massBins[minBinValue[0]]  	#### BEST for 650_200
		h_PreFitGauss.Fit(GausPreFit,"MRI","", massBins[minBinValue[0]-2], massBins[minBinValue[0]+1])
	else: 
		PreFitStart = 400 #massBins[minBinValue[0]-3] 
		for i in range(-15,10):
			h_PreFitP4.SetBinContent(minBinValue[0]+i, 0)
			h_PreFitP4.SetBinError(minBinValue[0]+i, 0)
		#h_PreFitGauss.Fit(GausPreFit,"MRI","",st2mass-50,st2mass+50)
		#print massBins[minBinValue[0]-3], massBins[minBinValue[0]+2]
		#h_PreFitGauss.Fit(GausPreFit,"MRI","", massBins[minBinValue[0]-2], massBins[minBinValue[0]+2])
		h_PreFitGauss.Fit(GausPreFit,"MRI","",st2mass-50,st2mass+50)

#PreFitEnd  = 1200
#P4PreFit.SetParameter(0,1)
PreFitEnd  = 1500
if ( st2mass >= 650): P4PreFit.SetParameter(0,1)
h_PreFitP4.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)
h_PreFitP4.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)


################# Gaussian Fit Match 
#print GausPreFit.GetChisquare(), GausPreFit.GetNDF() 
#print round((GausPreFit.GetChisquare()-GausPreFit.GetNDF())/sqrt(2*GausPreFit.GetNDF()),3), ' standards deviation away from the mean.'
#
#
################## Real Fitting
P4GausFit.SetParameter(0,P4PreFit.GetParameter(0));				
P4GausFit.SetParameter(1,P4PreFit.GetParameter(1));
P4GausFit.SetParameter(2,P4PreFit.GetParameter(2));
P4GausFit.SetParameter(3,P4PreFit.GetParameter(3));
P4GausFit.SetParameter(4,GausPreFit.GetParameter(0));
P4GausFit.SetParameter(5,GausPreFit.GetParameter(1));
#P4GausFit.SetParLimits(5,GausPreFit.GetParameter(1)-30,GausPreFit.GetParameter(1)+30);
if (st2mass == 650): P4GausFit.SetParLimits(5, massBins[minBinValue[0]-1]-30, massBins[minBinValue[0]]-10)
elif (st2mass == 550): P4GausFit.SetParLimits(5, massBins[minBinValue[0]-1]-30, massBins[minBinValue[0]]+10)
#print massBins[minBinValue[0]], massBins[minBinValue[0]]-30, massBins[minBinValue[0]]+10
#P4GausFit.SetParameter(5,st2mass);
#
P4GausFit.SetParameter(6,GausPreFit.GetParameter(2));
##if not "cut" in histo:
##	P4GausFit.SetParLimits(5,st1mass-20,st1mass+30);
##	P4GausFit.SetParLimits(6,GausPreFit.GetParameter(2)-30,GausPreFit.GetParameter(2)+30);
##else:
##	P4GausFit.SetParLimits(5,st1mass-20,st1mass+30);
##	P4GausFit.SetParLimits(6,GausPreFit.GetParameter(2)-10,GausPreFit.GetParameter(2)+10);
#
#
##if ( h_P4GausFit.GetMaximumBin() > 150 ):
##	FitStart= h_P4GausFit.GetMaximumBin()*binSize-40
##else:
#if (st2mass == 350):
#	FitStart= 250
#	FitEnd = 1400
#elif (st2mass == 450):
#	FitStart= 290
#	FitEnd = 1620
#elif (st2mass == 550):
#	FitStart= 410
#	FitEnd = 1400
#elif (st2mass == 650):
#	FitStart= 400
#	FitEnd = 1400
#else:
#	FitStart= 510
#	FitStart= 510
FitStart = 350
FitEnd = 2000
h_P4GausFit.Fit(P4GausFit,"MRI","",FitStart,FitEnd);
h_P4GausFit.Fit(P4GausFit,"MRI","",FitStart,FitEnd);
#print 'Standard Deviation of Fit: ', round((P4GausFit.GetChisquare()-P4GausFit.GetNDF())/sqrt(2*P4GausFit.GetNDF()),3)
#

##### Histo to save
h_signal = h_P4GausFit.Clone()

############ Calculate Acceptance
GausOnly = TF1("GausOnly","gaus", massBins[minBinValue[0]-5], massBins[minBinValue[0]+5])
P4Only = TF1("P4Only","[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))", massBins[minBinValue[0]-4], massBins[minBinValue[0]+4])
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
##GausOnly.SetLineColor(6)
##GausOnly.SetName(("Gaus_P4"+to_string(mass[m])+"_"+to_string(pT6V)).c_str())
##P4Only.SetLineColor(1)
##P4Only.SetName(("P4_"+to_string(mass[m])+"_"+to_string(pT6V)).c_str())
##string bfolder
##fnew1.cd()
##h_CombFitP4.Write()
##GausOnly.Write()
##P4Only.Write()
#		
SigP4 = GausOnly.Integral(0,1000)
#FullSigP4 = h_P4GausFit.Integral(0,1000)
GaussIntegral = SigP4/binSize/eventsGenerated 
#FullIntegral = FullSigP4/eventsGenerated
Acceptance = sqrt(2*3.14)*GausOnly.GetParameter(2)*GausOnly.GetParameter(0)/binSize/eventsGenerated 
#GaussYield = sqrt(2*3.14)*GausOnly.GetParameter(2)*GausOnly.GetParameter(0)/binSize
print 'Sample: ', output
print SigP4, GaussIntegral, binSize
print 'Gaus Integral: ', round(GaussIntegral,5)
print 'Gaus Acceptance: ', round(Acceptance, 5)
print 'Gaus Mean: ', round(GausOnly.GetParameter(1),5)
print 'Gaus Width: ', round(GausOnly.GetParameter(2),5)
#, 'Gaus Yield: ', round(GaussYield, 2), ' Full Acceptance: ', round(FullIntegral, 5)
##print GausOnly.GetParameter(0) , 1/GausOnly.GetParameter(0), GausOnly.GetParameter(2), 1/(sqrt(2*3.14)*GausOnly.GetParameter(2))

################## Plotting Histograms
#### Histo without Gaussian bins
#c1 = TCanvas('c1', 'c1',  10, 10, 750, 500 )
#h_PreFitP4.SetTitle("")
#h_PreFitP4.Draw()
#h_PreFitP4.GetXaxis().SetTitle("Heavier Stop Reconstruction Invariant Mass [GeV]")
#h_PreFitP4.GetYaxis().SetTitle("Quadruplets")
#textBox=TLatex()
#textBox.SetNDC()
#textBox.SetTextSize(0.05) 
#textBox.SetTextColor(50)
#textBox.DrawLatex(0.10,0.91,"CMS Preliminary Simulation")
#textBox1=TLatex()
#textBox1.SetNDC()
#textBox1.SetTextColor(50)
#textBox1.SetTextSize(0.04) 
#textBox1.DrawText(0.50,0.85,decay+"_"+str(st2mass)+"_"+str(st1mass))
#c1.SaveAs(outputDir+output+"_WOStop2_FitP40.pdf")
#del c1
#
######## Gaussian from Match
#c2 = TCanvas('c2', 'c2',  10, 10, 750, 500 )
#h_PreFitGauss.SetTitle("")
#h_PreFitGauss.Draw()
#h_PreFitGauss.GetXaxis().SetTitle("Invariant Mass [GeV]")
#h_PreFitGauss.GetYaxis().SetTitle("Quadruplets / 10 GeV")
#textBox=TLatex()
#textBox.SetNDC()
#textBox.SetTextSize(0.05) 
#textBox.SetTextColor(50)
#textBox.DrawLatex(0.10,0.91,"CMS Preliminary Simulation")
#textBox1=TLatex()
#textBox1.SetNDC()
#textBox1.SetTextColor(50)
#textBox1.SetTextSize(0.04) 
#textBox1.DrawText(0.50,0.85,decay+"_"+str(st2mass)+"_"+str(st1mass))
##c2.SaveAs(outputDir+"MatchStop1"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_FitGaus.pdf")
#c2.SaveAs(outputDir+"MatchStop2"+output+"_FitGaus.pdf")
##c2.SaveAs(outputDir+"MatchStop1"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_FitGaus.pdf")
#del c2

########### Real Fit
c3 = TCanvas('c3', 'c3',  10, 10, 750, 500 )
h_P4GausFit.GetXaxis().SetTitle("Heavier Stop Reconstruction Invariant Mass [GeV]")
if not "resoBased" in histo: h_P4GausFit.GetYaxis().SetTitle("Quadruplets / 10 GeV")
else: h_P4GausFit.GetYaxis().SetTitle("Quadruplets")
h_P4GausFit.GetYaxis().SetTitleOffset(1.2);
h_P4GausFit.SetTitle("")
#if ( st2mass == 450): h_P4GausFit.SetMaximum(1300)
#if ( st2mass == 550): h_P4GausFit.SetMaximum(2000)
#if ( st2mass == 650): h_P4GausFit.SetMaximum(2000)
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
legend.AddEntry(h_P4GausFit, decay+"_"+str(st2mass)+"_"+str(st1mass), "le")
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

#c3.SaveAs(outputDir+histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_FitP4Gauss.pdf")
#c3.SaveAs(outputDir+histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_FitP4Gauss.pdf")
c3.SaveAs(outputDir+output+"_FitP4Gauss_simpleRoot.pdf")
c3.SaveAs(outputDir+output+"_FitP4Gauss_simpleRoot.png")
del c3
