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
hist1 = str ( sys.argv[1] )
folder = str ( sys.argv[2] )


####### Input files
input3 = "/uscms_data/d3/algomez/files/Stops/Results/HT250-500_4jet80_6jet60_plots.root"
input2 = "/uscms_data/d3/algomez/files/Stops/Results/HT500-1000_4jet80_6jet60_plots.root"
input1 = "/uscms_data/d3/algomez/files/Stops/Results/HT1000-Inf_4jet80_6jet60_plots.root"
input4 = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_6jet60_plots.root"   ### v1 bin 10
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

binSize = 10
#print "Input files: ", input1

f1 = TFile(input1)
f2 = TFile(input2)
f3 = TFile(input3)
f4 = TFile(input4)

h1 = f1.Get(folder+'/' + hist1)
h2 = f2.Get(folder+'/' + hist1)
#h3 = f3.Get(folder+'/' + hist1)
h4 = f4.Get(folder+'/' + hist1)

h1clone = h1.Clone(hist1)
h2clone = h2.Clone(hist1)
#h3clone = h3.Clone(hist1)
h4clone = h4.Clone(hist1)

h1clone.Add( h2clone )
#h1clone.Add( h3clone )

######## Fit Functions
P4PreFit = TF1("P4PreFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",0,2000);
P4PreFitData = TF1("P4PreFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",0,2000);

PreFitStart= h1clone.GetMaximumBin()*binSize-100
PreFitEnd  = h1clone.GetMaximumBin()*binSize+1200
#print h1clone.GetMaximumBin(), PreFitStart, PreFitEnd

P4PreFit.SetParameter(0,0.1)
P4PreFit.SetParameter(1,-5)
#P4PreFit.SetParameter(0,1500)
#P4PreFit.SetParameter(1, -5)
#P4PreFit.SetParameter(2, 15)   # max 11.9
#P4PreFit.SetParameter(3, 1.2)   # max 6
h1clone.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)

################# Fit Data
PreFitStartData = h4clone.GetMaximumBin()*binSize-10
PreFitEndData  = h4clone.GetMaximumBin()*binSize+1000
P4PreFitData.SetParameter(1,-100)
h4clone.Fit(P4PreFitData,"MRI","",PreFitStartData,PreFitEndData)
h4clone.Fit(P4PreFitData,"MRI","",PreFitStartData,PreFitEndData)
P4FitData = TF1("P4Fit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",0,2000);
P4FitData.SetParameter(0, P4PreFitData.GetParameter(0))
P4FitData.SetParameter(1, P4PreFitData.GetParameter(1))
P4FitData.SetParameter(2, P4PreFitData.GetParameter(2))
P4FitData.SetParameter(3, P4PreFitData.GetParameter(3))
		
######### Plotting Histograms
c1 = TCanvas('c1', 'c1',  10, 10, 750, 500 )
P4FitData.SetLineColor(3)
P4FitData.Draw()
h1clone.GetXaxis().SetTitle("Mass [GeV]")
#h1clone.GetYaxis().SetTitle("Dijets/10 GeV")
h1clone.GetYaxis().SetTitle("Quadjets / 10 GeV")
h1clone.GetYaxis().SetTitleOffset(1.2);
h1clone.SetTitle("")
h1clone.Sumw2()
h1clone.Draw('same')

textBox=TLatex()
textBox.SetNDC()
textBox.SetTextSize(0.05) 
textBox.SetTextColor(50)
textBox.DrawLatex(0.10,0.91,"CMS Preliminary Simulation 19.5 fb^{-1} at #sqrt{s} = 8 TeV")

textBox1=TLatex()
textBox1.SetNDC()
textBox1.SetTextSize(0.04) 
textBox1.SetTextColor(50)
textBox1.DrawText(0.56,0.85,"QCD")
	
c1.SaveAs(outputDir+hist1+"_QCD_FitP4Gauss.pdf")
del c1
