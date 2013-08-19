#!/usr/bin/env python

###################
### Make Fitting
###################

from ROOT import *
import glob,sys

gROOT.Reset()
gStyle.SetOptFit()
gStyle.SetStatY(0.9);
gStyle.SetStatX(0.9);
gStyle.SetStatW(0.15);
gStyle.SetStatH(0.15); 

###### Input parameters
st1mass = int (sys.argv[1])
st2mass = int (sys.argv[2])
decay = sys.argv[3]
histo = str ( sys.argv[4] )
#folder = str ( sys.argv[5] )
print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]


####### Input files
input1 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_preNtuplesPlots.root"
#input2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_plots.root"
input2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_plots.root"
#input2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_plots.root"
inputFile1 = TFile(input1)
inputFile2 = TFile(input2)
print "Input files: ", input2
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

binSize = 10
eventsGenerated = 100000
mcScaleFile = open("MCScaleFactors.txt")
for line in mcScaleFile:
	line = line.rstrip()
	#if "HT250-500XS" == line.split()[0]:
	#	HT250_500XS = float(line.split()[1])
	#if "HT500-1000XS" == line.split()[0]: 
	#	HT500_1000XS = float(line.split()[1])
	#if "HT1000-InfXS" == line.split()[0]: 
	#	HT1000_InfXS = float(line.split()[1])
	#if "HT250-500nEvents" == line.split()[0]: 
	#	HT250_500nEvents = float(line.split()[1])
	#if "HT500-1000nEvents" == line.split()[0]: 
	#	HT500_1000nEvents = float(line.split()[1])
	#if "HT1000-InfnEvents" == line.split()[0]: 
	#	HT1000_InfnEvents = float(line.split()[1])
	if "st2XS"+str(st2mass) == line.split()[0]:
		signalXS = float(line.split()[1])
	#if decay1+"_"+str(st2mass1)+"_"+str(st1mass1) == line.split()[0]: 
	#	signalnEvents = float(line.split()[1])


weightSignal = 20000 * signalXS/eventsGenerated
#weightHT250_500 = 20000 * HT250_500XS/HT250_500nEvents
#weightHT500_1000 = 20000 * HT500_1000XS/HT500_1000nEvents
#weightHT1000_Inf = 20000 * HT1000_InfXS/HT1000_InfnEvents

######## Fit Functions
P4PreFit = TF1("P4PreFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",0,1000);
GausPreFit = TF1("GausPreFit", "gaus",0,1000);
P4GausFit= TF1("P4GausFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))+gaus(4)",0,1000);

######## Get Histograms 
tmp_h_Reco_WOStop1 = inputFile2.Get('step3plots1D/'+histo+'_WOStop1')
h_PreFitP4 = tmp_h_Reco_WOStop1.Clone('tmp_'+histo+'_WOStop1')
tmp_h_Reco = inputFile2.Get('step3plots1D/'+histo)
h_P4GausFit = tmp_h_Reco.Clone(histo)
tmp_h_Match = inputFile1.Get('plots/AnalysisPlots/massmatchdijetWOMatchBjets')
h_PreFitGauss = tmp_h_Match.Clone("tmp_Stop1_Match")

#h_PreFitP4.Scale(weightSignal)
#h_PreFitGauss.Scale(weightSignal) 
#h_P4GausFit.Scale(weightSignal) 

############## Pre-Fitting combinations
PreFitStart= h_PreFitP4.GetMaximumBin()*binSize-50
PreFitEnd  = h_PreFitP4.GetMaximumBin()*binSize+1000
#PreFitStart= h_PreFitP4.GetMaximumBin()*binSize-20
#PreFitEnd  = h_PreFitP4.GetMaximumBin()*binSize+1000
#print h_PreFitP4.GetMaximumBin(), PreFitStart, PreFitEnd

#P4PreFit.SetParameter(0,1)
#P4PreFit.SetParameter(1,1)
#P4PreFit.SetParameter(2, 15)   # max 11.9
#P4PreFit.SetParameter(3, 1.2)   # max 6
h_PreFitP4.Fit(P4PreFit,"RQM","",PreFitStart,PreFitEnd)
h_PreFitP4.Fit(P4PreFit,"RQM","",PreFitStart,PreFitEnd)
h_PreFitP4.Fit(P4PreFit,"RQM","",PreFitStart,PreFitEnd)


################ Gaussian Fit Match 
h_PreFitGauss.Fit(GausPreFit,"RQM","",st1mass-30,st1mass+30)
#print GausPreFit.GetChisquare(), GausPreFit.GetNDF() 
print round((GausPreFit.GetChisquare()-GausPreFit.GetNDF())/sqrt(2*GausPreFit.GetNDF()),3), ' standards deviation away from the mean.'


################# Real Fitting
P4GausFit.SetParameter(0,P4PreFit.GetParameter(0));				
P4GausFit.SetParameter(1,P4PreFit.GetParameter(1));
P4GausFit.SetParameter(2,P4PreFit.GetParameter(2));
P4GausFit.SetParameter(3,P4PreFit.GetParameter(3));
P4GausFit.SetParameter(4,GausPreFit.GetParameter(0));
#P4GausFit.SetParameter(5,GausPreFit.GetParameter(1));
#P4GausFit.SetParLimits(5,GausPreFit.GetParameter(1)-40,GausPreFit.GetParameter(1)+40);
P4GausFit.SetParameter(5,st1mass);

P4GausFit.SetParLimits(5,st1mass-30,st1mass+30);
P4GausFit.SetParameter(6,GausPreFit.GetParameter(2));
P4GausFit.SetParLimits(6,GausPreFit.GetParameter(2)-30,GausPreFit.GetParameter(2)+30);

if ( h_P4GausFit.GetMaximumBin() > 150 ):
	FitStart= h_P4GausFit.GetMaximumBin()*binSize-40
else:
	FitStart= 50
	#FitStart= 80
FitEnd = 1000
h_P4GausFit.Fit(P4GausFit,"RQM","",FitStart,FitEnd);
h_P4GausFit.Fit(P4GausFit,"RQM","",FitStart,FitEnd);
h_P4GausFit.Fit(P4GausFit,"RQM","",FitStart,FitEnd);
print 'Standard Deviation of Fit: ', round((P4GausFit.GetChisquare()-P4GausFit.GetNDF())/sqrt(2*P4GausFit.GetNDF()),3)

########### Calculate Acceptance
GausOnly = TF1("GausOnly","gaus", 0, 1000)
P4Only = TF1("P4Only","[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))", 0, 1000)
		
P4Only.SetParameter(0,P4GausFit.GetParameter(0))
P4Only.SetParameter(1,P4GausFit.GetParameter(1))
P4Only.SetParameter(2,P4GausFit.GetParameter(2))
P4Only.SetParameter(3,P4GausFit.GetParameter(3))
		
GausOnly.SetParameter(0,P4GausFit.GetParameter(4))
GausOnly.SetParameter(1,P4GausFit.GetParameter(5))
GausOnly.SetParameter(2,P4GausFit.GetParameter(6))
		
GausOnly.SetParError(0,P4GausFit.GetParError(4))
GausOnly.SetParError(1,P4GausFit.GetParError(5))
GausOnly.SetParError(2,P4GausFit.GetParError(6))
		
#GausOnly.SetLineColor(6)
#GausOnly.SetName(("Gaus_P4"+to_string(mass[m])+"_"+to_string(pT6V)).c_str())
#P4Only.SetLineColor(1)
#P4Only.SetName(("P4_"+to_string(mass[m])+"_"+to_string(pT6V)).c_str())
#string bfolder
#fnew1.cd()
#h_CombFitP4.Write()
#GausOnly.Write()
#P4Only.Write()
		
SigP4 = GausOnly.Integral(0,1000)
FullSigP4 = h_P4GausFit.Integral(0,1000)
GaussIntegral = SigP4/binSize/eventsGenerated 
FullIntegral = FullSigP4/eventsGenerated
Acceptance = sqrt(2*3.14)*GausOnly.GetParameter(2)*GausOnly.GetParameter(0)/binSize/eventsGenerated 
GaussYield = sqrt(2*3.14)*GausOnly.GetParameter(2)*GausOnly.GetParameter(0)/binSize
#print 'Sample: ', decay+"_"+str(st2mass)+"_"+str(st1mass)
print 'Gaus Integral: ', round(GaussIntegral,5)
print 'Gaus Acceptance: ', round(Acceptance, 5), 'Gaus Yield: ', round(GaussYield, 2), ' Full Acceptance: ', round(FullIntegral, 5)
#print GausOnly.GetParameter(0) , 1/GausOnly.GetParameter(0), GausOnly.GetParameter(2), 1/(sqrt(2*3.14)*GausOnly.GetParameter(2))

######### Plotting Histograms
c1 = TCanvas('c1', 'c1',  10, 10, 750, 500 )
h_PreFitP4.Draw()
#c1.SaveAs(outputDir+histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_WOStop1_FitP40.pdf")
c1.SaveAs(outputDir+histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_WOStop1_FitP40.pdf")
#c1.SaveAs(outputDir+histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_WOStop1_FitP40.pdf")
c1.Close()

c2 = TCanvas('c2', 'c2',  10, 10, 750, 500 )
h_PreFitGauss.Draw()
#c2.SaveAs(outputDir+"MatchStop1"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_Gaus.pdf")
c2.SaveAs(outputDir+"MatchStop1"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_Gaus.pdf")
#c2.SaveAs(outputDir+"MatchStop1"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_Gaus.pdf")
c2.Close()

c3 = TCanvas('c3', 'c3',  10, 10, 750, 500 )
h_P4GausFit.GetXaxis().SetTitle("Mass [GeV]")
h_P4GausFit.GetYaxis().SetTitle("Dijets/10 GeV")
h_P4GausFit.GetYaxis().SetTitleOffset(1.2);
h_P4GausFit.Draw()
c3.SaveAs(outputDir+histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_FitP4Gauss.pdf")
#c3.SaveAs(outputDir+histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_FitP4Gauss.pdf")
#c3.SaveAs(outputDir+histo+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_FitP4Gauss.pdf")
c3.Close()

