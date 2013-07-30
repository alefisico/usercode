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
hist1 = str ( sys.argv[4] )
folder = str ( sys.argv[5] )
#hist2 = str ( sys.argv[5] )
#print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]

#input1 = "/cms/gomez/Stops/Results/st2_h_bb_st1_"+decay1+"_"+str(st2mass1)+"_"+str(st1mass1)+"_plots_v8.root"
#input1 = "/cms/gomez/Stops/Results/st2_h_bb_st1_"+decay1+"_"+str(st2mass1)+"_"+str(st1mass1)+"_plots_v7_120-130.root"
input1 = "/cms/gomez/Stops/Results/st2_h_bb_st1_"+decay1+"_"+str(st2mass1)+"_"+str(st1mass1)+"_plots.root"
outputDir = "/cms/gomez/Stops/Results/Plots/"

#print "Input files: ", input1


canvs = []
outhists = []
stacks = []
leg = []

f1 = TFile(input1)


mcScaleFile = open("MCScaleFactors.txt")
for line in mcScaleFile:
	line = line.rstrip()
	if "st2XS"+str(st2mass1) == line.split()[0]:
		#print line.split()[1]
		XSsample = float(line.split()[1])
	if decay1+"_"+str(st2mass1)+"_"+str(st1mass1) == line.split()[0]:
		#print line.split()[1]
		nEvents = float(line.split()[1])

#weight = nEvents/XSsample
weight = XSsample/nEvents
print "ScaleFactor for st2_h_bb_st1_"+decay1+"_"+str(st2mass1)+"_"+str(st1mass1) , weight

###########################
# PLOTTING FUNCTION
#   plot("histogram1","histogram2","Title;XaxisLabel;YaxisLabel","histo1legendEntry","histo2legendEntry","OutputFileName.png")

def plot_cuts(hist1,label1) :

	gROOT.Reset()
	gStyle.SetPadRightMargin(0.2);

	#h1 = f1.Get(options.dir + hist1)
	h1 = f1.Get(folder+'/' + hist1)
	h2 = f1.Get(folder+'/' + hist1 + '_cutDiagStop1jj50')
	h3 = f1.Get(folder+'/' + hist1 + '_cutDiagStop1jj100')
	h4 = f1.Get(folder+'/' + hist1 + '_cutDiagStop1jj150')
	h5 = f1.Get(folder+'/' + hist1 + '_cutDiagStop1jj200')

	h1clone = h1.Clone("h1")
	h2clone = h2.Clone("h2")
	h3clone = h3.Clone("h3")
	h4clone = h4.Clone("h4")
	h5clone = h5.Clone("h5")

  	hs = THStack( h1.GetName()+h2.GetName()+h3.GetName()+h4.GetName()+h5.GetName(), h1.GetTitle() )

#  	h2clone.Scale(1.0 / h2clone.Integral() * h1clone.Integral() )
#  	h3clone.Scale(1.0 / h3clone.Integral() * h1clone.Integral() )
#  	h4clone.Scale(1.0 / h4clone.Integral() * h1clone.Integral() )
#  	h5clone.Scale(1.0 / h5clone.Integral() * h1clone.Integral() )
  
	h1clone.Scale( weight )
	h2clone.Scale( weight )
	h3clone.Scale( weight )
	h4clone.Scale( weight )
	h5clone.Scale( weight )

  	h1clone.SetLineColor(1)
	h2clone.SetLineColor(2)
	h3clone.SetLineColor(3)
	h4clone.SetLineColor(4)
	h5clone.SetLineColor(5)
  	h1clone.SetLineWidth(2)
  	h2clone.SetLineWidth(2)
  	h3clone.SetLineWidth(2)
  	h4clone.SetLineWidth(2)
  	h5clone.SetLineWidth(2)

	hs.SetTitle(hist1)
	hs.Add( h1clone, 'hist' )
	hs.Add( h2clone, 'hist' )
	hs.Add( h3clone, 'hist' )
	hs.Add( h4clone, 'hist' )
	hs.Add( h5clone, 'hist' )

	c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
	hs.Draw('nostack')
	hs.GetXaxis().SetTitle("Mass [GeV]")

	legend=TLegend(0.83,0.4,0.98,0.8)
	legend.SetFillColor(0);
	legend.AddEntry(h1clone,label1, "l")
	legend.AddEntry(h2clone,'cutDiagStop1jj50', "l")    
	legend.AddEntry(h3clone,'cutDiagStop1jj100', "l")    
	legend.AddEntry(h4clone,'cutDiagStop1jj150', "l")    
	legend.AddEntry(h5clone,'cutDiagStop1jj200', "l")    
	legend.Draw()
  
	outhists.append( [h1clone,h2clone,h3clone,h4clone,h5clone] )
	stacks.append( hs )
	leg.append( legend )
	canvs.append(c)

	c.SaveAs(outputDir + hist1 + "_diffCuts_" + decay1 + "_" + str(st2mass1) + "_" +str(st1mass1) + ".pdf")
	#c.SaveAs(outputDir + hist1 + "_diffCuts_120-130_" + decay1 + "_" + str(st2mass1) + "_" +str(st1mass1) + ".pdf")
	return

def plot(hist1, hist2) :

	gROOT.Reset()
	gStyle.SetPadRightMargin(0.2);

	#h1 = f1.Get(options.dir + hist1)
	h1 = f1.Get('step1plots1D/' + hist1)
	h2 = f1.Get('step1plots1D/' + hist2)

	h1clone = h1.Clone("h1")
	h2clone = h2.Clone("h2")

  	hs = THStack( h1.GetName()+h2.GetName(), h1.GetTitle() )

  	h1clone.SetLineColor(1)
	h2clone.SetLineColor(2)
  	h1clone.SetLineWidth(2)
  	h2clone.SetLineWidth(2)

	hs.SetTitle(hist1)
	hs.Add( h1clone, 'hist' )
	hs.Add( h2clone, 'hist' )

	c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
	hs.Draw('nostack')

	legend=TLegend(0.83,0.4,0.98,0.8)
	legend.SetFillColor(0);
	legend.AddEntry(h1clone,hist1, "l")
	legend.AddEntry(h2clone,hist2, "l")    
	legend.Draw()
  
	outhists.append( [h1clone,h2clone] )
	stacks.append( hs )
	leg.append( legend )
	canvs.append(c)

	c.SaveAs(outputDir + hist1 + hist2 + "_diff_" + decay1 + "_" + str(st2mass1) + "_" +str(st1mass1) + ".pdf")
	return

plot_cuts( hist1, decay1+"_"+str(st2mass1)+"_"+str(st1mass1) )
#plot( hist1, hist2 )
