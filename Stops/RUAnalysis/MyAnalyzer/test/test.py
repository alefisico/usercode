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
TVirtualFitter.SetMaxIterations(5000000000000)		######### Trick to increase number of iterations

###### Input parameters
st1mass = int (sys.argv[1])
st2mass = int (sys.argv[2])
histo = str ( sys.argv[3] )
#folder = str ( sys.argv[5] )
#print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]

decay = 'jj'

####### Input files
input1 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_preNtuplesPlots.root"
input2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_plots.root"
inputFile1 = TFile(input1)
inputFile2 = TFile(input2)
#print "Input files: ", input2
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

binSize = 10
massBins = [0, 30, 60, 90, 120, 150, 180, 210, 250, 290, 330, 370, 410, 460, 510, 560, 610, 670, 730, 790, 860, 930, 1000, 1080, 1160, 1240, 1330, 1420, 1520, 1620, 1730, 1840, 2000]
eventsGenerated = 100000

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
if not "reso" in histo:
	if ( st2mass == 450 ): PreFitStart= h_PreFitP4.GetMaximumBin()*binSize+50  ### BEST for 450_200
	elif ( st2mass == 350 ): PreFitStart= h_PreFitP4.GetMaximumBin()*binSize+50   #### BEST for 350_200
	elif ( st2mass == 550 ): PreFitStart= h_PreFitP4.GetMaximumBin()*binSize+30   #### BEST for 550_200
	elif ( st2mass == 650 ): PreFitStart= h_PreFitP4.GetMaximumBin()*binSize+50   #### BEST for 650_200
	else: PreFitStart= 400
	PreFitEnd  = h_PreFitP4.GetMaximumBin()*binSize+800
	#PreFitEnd  = 1200
	#P4PreFit.SetParameter(0,1)
else:
	if ( st2mass == 450 ): PreFitStart= 500  ### BEST for 450_200
	elif ( st2mass == 550 ): PreFitStart= 500   #### BEST for 550_200
	elif ( st2mass == 650 ): PreFitStart= 600   #### BEST for 650_200
	else: PreFitStart= 400
	PreFitEnd  = 1500
	#PreFitEnd  = 1200
	#P4PreFit.SetParameter(0,1)
P4PreFit.SetParameter(1,1)

h_PreFitP4.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)
h_PreFitP4.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)


################# Gaussian Fit Match 
if not "reso" in histo:
	h_PreFitGauss.Fit(GausPreFit,"MRI","",st2mass-50,st2mass+50)
else:
	maxBin = h_PreFitGauss.GetMaximumBin()
	maxStop2 = massBins[maxBin]
	#h_PreFitGauss.Fit(GausPreFit,"MRI","",maxStop2-50,maxStop2+100)
	h_PreFitGauss.Fit(GausPreFit,"MRI","",370, 510)
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
P4GausFit.SetParLimits(5, 370, 560)
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
if (st2mass == 350):
	FitStart= 250
	FitEnd = 1400
else:
	FitStart= 300
	FitEnd = 2000
h_P4GausFit.Fit(P4GausFit,"MRI","",FitStart,FitEnd);
h_P4GausFit.Fit(P4GausFit,"MRI","",FitStart,FitEnd);
#print 'Standard Deviation of Fit: ', round((P4GausFit.GetChisquare()-P4GausFit.GetNDF())/sqrt(2*P4GausFit.GetNDF()),3)
#
############ Calculate Acceptance
GausOnly = TF1("GausOnly","gaus", 0, 1000)
P4Only = TF1("P4Only","[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))", 0, 1000)
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
#SigP4 = GausOnly.Integral(0,1000)
#FullSigP4 = h_P4GausFit.Integral(0,1000)
#GaussIntegral = SigP4/binSize/eventsGenerated 
#FullIntegral = FullSigP4/eventsGenerated
Acceptance = sqrt(2*3.14)*GausOnly.GetParameter(2)*GausOnly.GetParameter(0)/binSize/eventsGenerated 
#GaussYield = sqrt(2*3.14)*GausOnly.GetParameter(2)*GausOnly.GetParameter(0)/binSize
##print 'Sample: ', decay+"_"+str(st2mass)+"_"+str(st1mass)
#print 'Gaus Integral: ', round(GaussIntegral,5)
print 'Gaus Acceptance: ', round(Acceptance, 5)
#, 'Gaus Yield: ', round(GaussYield, 2), ' Full Acceptance: ', round(FullIntegral, 5)
##print GausOnly.GetParameter(0) , 1/GausOnly.GetParameter(0), GausOnly.GetParameter(2), 1/(sqrt(2*3.14)*GausOnly.GetParameter(2))

################## Plotting Histograms
#### Histo without Gaussian bins
c1 = TCanvas('c1', 'c1',  10, 10, 750, 500 )
h_PreFitP4.SetTitle("")
h_PreFitP4.Draw()
h_PreFitP4.GetXaxis().SetTitle("Invariant Mass [GeV]")
h_PreFitP4.GetYaxis().SetTitle("Quadjets / 10 GeV")
textBox=TLatex()
textBox.SetNDC()
textBox.SetTextSize(0.05) 
textBox.SetTextColor(50)
textBox.DrawLatex(0.10,0.91,"CMS Preliminary Simulation")
textBox1=TLatex()
textBox1.SetNDC()
textBox1.SetTextColor(50)
textBox1.SetTextSize(0.04) 
textBox1.DrawText(0.50,0.85,decay+"_"+str(st2mass)+"_"+str(st1mass))
c1.SaveAs(outputDir+histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_WOStop2_FitP40.pdf")
del c1

####### Gaussian from Match
c2 = TCanvas('c2', 'c2',  10, 10, 750, 500 )
h_PreFitGauss.SetTitle("")
h_PreFitGauss.Draw()
h_PreFitGauss.GetXaxis().SetTitle("Invariant Mass [GeV]")
h_PreFitGauss.GetYaxis().SetTitle("Quadjets / 10 GeV")
textBox=TLatex()
textBox.SetNDC()
textBox.SetTextSize(0.05) 
textBox.SetTextColor(50)
textBox.DrawLatex(0.10,0.91,"CMS Preliminary Simulation")
textBox1=TLatex()
textBox1.SetNDC()
textBox1.SetTextColor(50)
textBox1.SetTextSize(0.04) 
textBox1.DrawText(0.50,0.85,decay+"_"+str(st2mass)+"_"+str(st1mass))
#c2.SaveAs(outputDir+"MatchStop1"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_FitGaus.pdf")
c2.SaveAs(outputDir+"MatchStop2"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_FitGaus.pdf")
#c2.SaveAs(outputDir+"MatchStop1"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_FitGaus.pdf")
del c2

########### Real Fit
c3 = TCanvas('c3', 'c3',  10, 10, 750, 500 )
h_P4GausFit.GetXaxis().SetTitle("Invariant Mass [GeV]")
if not "resoBased" in histo: h_P4GausFit.GetYaxis().SetTitle("Quadjets / 10 GeV")
else: h_P4GausFit.GetYaxis().SetTitle("Quadjets")
h_P4GausFit.GetYaxis().SetTitleOffset(1.2);
h_P4GausFit.SetTitle("")
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
textBox1=TLatex()
textBox1.SetNDC()
textBox1.SetTextColor(50)
textBox1.SetTextSize(0.04) 
textBox1.DrawText(0.50,0.85,decay+"_"+str(st2mass)+"_"+str(st1mass))

#c3.SaveAs(outputDir+histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_FitP4Gauss.pdf")
#c3.SaveAs(outputDir+histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_FitP4Gauss.pdf")
c3.SaveAs(outputDir+histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_FitP4Gauss.pdf")
del c3

