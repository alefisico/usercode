#!/usr/bin/env python

###################
### Make Fitting
###################

from ROOT import *
import glob,sys

gStyle.SetOptFit()

###### Input parameters
st1mass = int (sys.argv[1])
st2mass = int (sys.argv[2])
decay = sys.argv[3]
histo = str ( sys.argv[4] )
#folder = str ( sys.argv[5] )
print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]


####### Input files
input1 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_preNtuplesPlots.root"
input2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_plots.root"
inputFile1 = TFile(input1)
inputFile2 = TFile(input2)
print "Input files: ", input1
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

nRebin = 10

######## Fit Functions
P4PreFit0 = TF1("P4PreFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",0,1000);
GausPreFit = TF1("GausPreFit", "gaus",0,1000);
P4GausFit= TF1("P4GausFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))+gaus(4)",0,1000);

######## Get Histograms 
tmp_h_Reco = inputFile1.Get('plots/AnalysisPlots/'+histo)
tmp_h_Reco_WOStop1 = inputFile2.Get('step3plots1D/'+histo+'_WOStop1')
tmp_h_Match = inputFile1.Get('plots/AnalysisPlots/massmatchdijetWOMatchBjets')
h_PreFitP40 = tmp_h_Reco_WOStop1.Clone('tmp_'+histo+'_WOStop1')
h_PreFitGauss = tmp_h_Match.Clone("tmp_Stop1_Match")
h_PreFitP41 = tmp_h_Reco.Clone(histo)

#h_PreFitP4.Scale(1)   ### I guess I have to scale to unity to compare to MC
#h_PreFitGauss.Scale(1) 

############## Pre-Fitting combinations
FitStart0= h_PreFitP40.GetMaximumBin()*nRebin-40			#### 50 Claudia's parameter
FitEnd0  = h_PreFitP40.GetMaximumBin()*nRebin+1000		#### 800 Claudia's parameter
print h_PreFitP40.GetMaximumBin(), FitStart0, FitEnd0

#P4PreFit.SetParameter(0,1500)
#P4PreFit0.SetParameter(1, -5)
#P4PreFit0.SetParameter(2, 15)   # max 11.9
#P4PreFit0.SetParameter(3, 1.2)   # max 6
h_PreFitP40.Fit(P4PreFit0,"RQ","",FitStart0,FitEnd0)
h_PreFitP40.Fit(P4PreFit0,"RQ","",FitStart0,FitEnd0)
h_PreFitP40.Fit(P4PreFit0,"RQ","",FitStart0,FitEnd0)


################ Gaussian Fit Match 
h_PreFitGauss.Fit(GausPreFit,"RQ","",st1mass-30,st1mass+30)

print P4PreFit0.GetChisquare(), P4PreFit0.GetNDF() 
print P4PreFit0.GetChisquare()/P4PreFit0.GetNDF() 


################# Real Fitting
P4GausFit.SetParameter(0,P4PreFit0.GetParameter(0));				
P4GausFit.SetParameter(1,P4PreFit0.GetParameter(1));
P4GausFit.SetParameter(2,P4PreFit0.GetParameter(2));
P4GausFit.SetParameter(3,P4PreFit0.GetParameter(3));
P4GausFit.SetParameter(4,GausPreFit.GetParameter(0));
#P4GausFit.SetParameter(5,GausPreFit.GetParameter(1));
#P4GausFit.SetParLimits(5,GausPreFit.GetParameter(1)-40,GausPreFit.GetParameter(1)+40);
P4GausFit.SetParameter(5,st1mass);

P4GausFit.SetParLimits(5,st1mass-30,st1mass+30);
P4GausFit.SetParameter(6,GausPreFit.GetParameter(2));
P4GausFit.SetParLimits(6,GausPreFit.GetParameter(2)-20,GausPreFit.GetParameter(2)+20);

FitStart= h_PreFitP41.GetMaximumBin()*nRebin-30
FitEnd = 1000
h_PreFitP41.Fit(P4GausFit,"RQ","",FitStart,FitEnd);
h_PreFitP41.Fit(P4GausFit,"RQ","",FitStart,FitEnd);
h_PreFitP41.Fit(P4GausFit,"RQ","",FitStart,FitEnd);

######### Plotting Histograms
c1 = TCanvas('c1', 'c1',  10, 10, 750, 500 )
h_PreFitP40.Draw()
c1.SaveAs(outputDir+histo+decay+"_WOStop1_FitP40.pdf")
c1.Close()

c2 = TCanvas('c2', 'c2',  10, 10, 750, 500 )
h_PreFitGauss.Draw()
c2.SaveAs(outputDir+"MatchStop1"+decay+"_Gaus.pdf")
c2.Close()

c3 = TCanvas('c3', 'c3',  10, 10, 750, 500 )
h_PreFitP41.Draw()
c3.SaveAs(outputDir+histo+decay+"_FitP4Gauss.pdf")
c3.Close()

