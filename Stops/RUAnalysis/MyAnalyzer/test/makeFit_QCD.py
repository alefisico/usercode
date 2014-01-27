#!/usr/bin/env python

###################
### Make Fitting
###################

from ROOT import *
import glob,sys

gROOT.Reset()
TVirtualFitter.SetMaxIterations(50000000)		######### Trick to increase number of iterations

###### Input parameters
hist1 = str ( sys.argv[1] )
folder = str ( sys.argv[2] )


####### Input files
input3 = "/uscms_data/d3/algomez/files/Stops/Results/HT250-500_4jet80_6jet60_plots.root"
input2 = "/uscms_data/d3/algomez/files/Stops/Results/HT500-1000_4jet80_6jet60_plots.root"
input1 = "/uscms_data/d3/algomez/files/Stops/Results/HT1000-Inf_4jet80_6jet60_plots.root"
#input4 = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_6jet60_plots.root"   ### v1 bin 10
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

if not "resoBased" in hist1:
	binSize = 10
else:
	massBins = [0, 30, 60, 90, 120, 150, 180, 210, 250, 290, 330, 370, 410, 460, 510, 560, 610, 670, 730, 790, 860, 930, 1000, 1080, 1160, 1240, 1330, 1420, 1520, 1620, 1730, 1840, 2000]

f1 = TFile(input1)
f2 = TFile(input2)
f3 = TFile(input3)

h1 = f1.Get(folder+'/' + hist1)
h2 = f2.Get(folder+'/' + hist1)
#h3 = f3.Get(folder+'/' + hist1)

h1clone = h1.Clone(hist1)
h2clone = h2.Clone(hist1)
#h3clone = h3.Clone(hist1)

h1clone.Add( h2clone )
#h1clone.Add( h3clone )

######## Fit Functions
P4PreFit = TF1("P4PreFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",0,2000);
P4PreFitData = TF1("P4PreFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",0,2000);

if not "resoBased" in hist1:
	PreFitStart= h1clone.GetMaximumBin()*binSize-10
	PreFitEnd  = h1clone.GetMaximumBin()*binSize+1000
	h1clone.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)
else:
	PreFitStart = 350
	PreFitEnd  = 1500
	#P4PreFit.SetParameter(1,-10)
	h1clone.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)
	h1clone.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)
#print h1clone.GetMaximumBin(), PreFitStart, PreFitEnd

#P4PreFit.SetParameter(0,0.1)
#P4PreFit.SetParameter(1,-5)
#P4PreFit.SetParameter(0,1500)
#P4PreFit.SetParameter(1, -5)
#P4PreFit.SetParameter(2, 15)   # max 11.9
#P4PreFit.SetParameter(3, 1.2)   # max 6
h1clone.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)

		
######### Plotting Histograms
c1 = TCanvas('c1', 'c1',  10, 10, 750, 500 )
gStyle.SetOptFit()
gStyle.SetStatY(0.9)
gStyle.SetStatX(0.9)
gStyle.SetStatW(0.15)
gStyle.SetStatH(0.15) 
h1clone.GetXaxis().SetTitle("Heavy stop reconstruction invariant mass [GeV]")
h1clone.GetYaxis().SetTitle("dN/dM_{bbjj} [GeV^{-1}]")
h1clone.GetYaxis().SetTitleOffset(1.2);
h1clone.SetTitle("")
h1clone.Sumw2()
h1clone.Draw()

textBox=TLatex()
textBox.SetNDC()
textBox.SetTextSize(0.05) 
textBox.SetTextColor(50)
textBox.DrawLatex(0.10,0.91,"CMS Preliminary Simulation")

legend=TLegend(0.25,0.15,0.45,0.25)
legend.SetFillColor(0);
legend.SetBorderSize(0);
legend.AddEntry(h1clone,"QCD Simulation", "l")
legend.AddEntry(P4PreFit,"P4 parameter fit", "l")    
legend.Draw()

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

if "cutDiag" in hist1:
	textBox2=TLatex()
	textBox2.SetNDC()
	textBox2.SetTextSize(0.04) 
	textBox2.DrawLatex(0.70,0.25,"#Delta = 0 GeV")
c1.SaveAs(outputDir+hist1+"_QCD_FitP4Gauss.pdf")
del c1
