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
hist1 = str ( sys.argv[1] )
folder = str ( sys.argv[2] )


####### Input files
input1 = "/uscms_data/d3/algomez/files/Stops/Results/HT250-500_4jet80_plots.root"
input2 = "/uscms_data/d3/algomez/files/Stops/Results/HT500-1000_4jet80_plots.root"
input3 = "/uscms_data/d3/algomez/files/Stops/Results/HT1000-Inf_4jet80_plots.root"
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

binSize = 10
#print "Input files: ", input1

f1 = TFile(input1)
f2 = TFile(input2)
f3 = TFile(input3)

h1 = f1.Get(folder+'/' + hist1)
h2 = f2.Get(folder+'/' + hist1)
h3 = f3.Get(folder+'/' + hist1)

h1clone = h1.Clone(hist1)
h2clone = h2.Clone(hist1)
h3clone = h3.Clone(hist1)

h1clone.Add( h2clone )
h1clone.Add( h3clone )

######## Fit Functions
P4PreFit = TF1("P4PreFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",0,1000);

PreFitStart= h1clone.GetMaximumBin()*binSize-40			#### 50 Claudia's parameter
PreFitEnd  = h1clone.GetMaximumBin()*binSize+1000		#### 800 Claudia's parameter
#print h1clone.GetMaximumBin(), PreFitStart, PreFitEnd

#P4PreFit.SetParameter(0,1500)
#P4PreFit.SetParameter(1, -5)
#P4PreFit.SetParameter(2, 15)   # max 11.9
#P4PreFit.SetParameter(3, 1.2)   # max 6
h1clone.Fit(P4PreFit,"RQM","",PreFitStart,PreFitEnd)
h1clone.Fit(P4PreFit,"RQM","",PreFitStart,PreFitEnd)
h1clone.Fit(P4PreFit,"RQM","",PreFitStart,PreFitEnd)


		
######### Plotting Histograms
c1 = TCanvas('c1', 'c1',  10, 10, 750, 500 )
h1clone.GetXaxis().SetTitle("Mass [GeV]")
h1clone.GetYaxis().SetTitle("Dijets/10 GeV")
h1clone.GetYaxis().SetTitleOffset(1.2);
h1clone.Draw()
c1.SaveAs(outputDir+hist1+"_QCD_FitP4Gauss.pdf")
c1.Close()

