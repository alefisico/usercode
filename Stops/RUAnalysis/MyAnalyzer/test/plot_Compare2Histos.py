#!/usr/bin/env python

###################
# plot_Compare2Histos.py
# description: compares two histograms from one root file
# instructions:
#  python -i plot_Compare2Histos.py
###################

from ROOT import *
import glob,sys

st1mass1 = int (sys.argv[1])
st2mass1 = int (sys.argv[2])
decay1 = sys.argv[3]
st1mass2 = int (sys.argv[4])
st2mass2 = int (sys.argv[5])
decay2 = sys.argv[6]
hist = str ( sys.argv[7] )
print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6], sys.argv[7]

input1 = "/cms/gomez/Stops/Results/st2_h_bb_st1_"+decay1+"_"+str(st2mass1)+"_"+str(st1mass1)+"_plots.root"
input2 = "/cms/gomez/Stops/Results/st2_h_bb_st1_"+decay2+"_"+str(st2mass2)+"_"+str(st1mass2)+"_plots.root"
outputDir = "/cms/gomez/Stops/Results/Plots/"

print "Input files: ", input1, input2 
#print outputs

canvs = []
outhists = []
stacks = []
leg = []

#f1 = TFile(options.inputFile1)
f1 = TFile(input1)
f2 = TFile(input2)

###########################
# PLOTTING FUNCTION
#   plot("histogram1","histogram2","Title;XaxisLabel;YaxisLabel","histo1legendEntry","histo2legendEntry","OutputFileName.png")

def plot(hist1,label1,label2) :

	gROOT.Reset()
	gStyle.SetPadRightMargin(0.2);

	#h1 = f1.Get(options.dir + hist1)
	h1 = f1.Get('plots/AnalysisPlots/' + hist1)
	h2 = f2.Get('plots/AnalysisPlots/' + hist1)

	h1clone = h1.Clone("h1")
	h2clone = h2.Clone("h2")

  	hs = THStack( h1.GetName()+h2.GetName(), h1.GetTitle() )

  	h2clone.Scale(1.0 / h2clone.Integral() * h1clone.Integral() )
  
  	h1clone.SetLineColor(1)
	h2clone.SetLineColor(6)
  	h1clone.SetLineWidth(2)
  	h2clone.SetLineWidth(2)

	hs.SetTitle(hist1)
	hs.Add( h1clone, 'hist' )
	hs.Add( h2clone, 'hist' )

	c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
	hs.Draw('nostack')

	legend=TLegend(0.83,0.4,0.98,0.8)
	legend.SetFillColor(0);
	legend.AddEntry(h1clone,label1, "l")
	legend.AddEntry(h2clone,label2, "l")    
	legend.Draw()
  
	outhists.append( [h1clone,h2clone] )
	stacks.append( hs )
	leg.append( legend )
	canvs.append(c)

	c.SaveAs(outputDir + hist1 + "_diff_" + decay1 + "_" + str(st2mass1) + "_" +str(st1mass1) + "_"  + decay2 + "_" + str(st2mass2) + "_" +str(st1mass2) + ".pdf")
	return

############################################
## EXERCISE 1A

# compare jet mass
plot( hist, "st2_h_bb_st1_"+decay1+"_"+str(st2mass1)+"_"+str(st1mass1), "st2_h_bb_st1_"+decay2+"_"+str(st2mass2)+"_"+str(st1mass2) )

