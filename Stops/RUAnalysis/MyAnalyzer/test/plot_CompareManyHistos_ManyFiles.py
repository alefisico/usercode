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
decay = sys.argv[3]
hist = str ( sys.argv[4] )
folder = str ( sys.argv[5] )
#st1mass2 = int (sys.argv[5])
#st1mass3 = int (sys.argv[6])
#st1mass4 = int (sys.argv[7])
#st1mass5 = int (sys.argv[8])
#print "Options: ", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6], sys.argv[7]

#input2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass1)+"_"+str(st1mass2)+"_plots_v7.root"
#input3 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass1)+"_"+str(st1mass3)+"_plots_v7.root"
input1 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass1)+"_"+str(st1mass1)+"_plots.root"
input2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass1)+"_"+str(st1mass1)+"_4jet80_6jet60_plots.root"
input3 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass1)+"_"+str(st1mass1)+"_4jet80_plots.root"
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

#print "Input files: ", input1, input2, input3, input4
#print outputs

canvs = []
outhists = []
stacks = []
leg = []

#f1 = TFile(options.inputFile1)
f1 = TFile(input1)
f2 = TFile(input2)
f3 = TFile(input3)
#f4 = TFile(input4)
#f5 = TFile(input5)

###########################
# PLOTTING FUNCTION
#   plot("histogram1","histogram2","Title;XaxisLabel;YaxisLabel","histo1legendEntry","histo2legendEntry","OutputFileName.png")

#def plot(hist1,label1,label2) :
def plot(hist1,label1,label2,label3) :
#def plot(hist1,label1,label2,label3,label4) :
#def plot(hist1,label1,label2,label3,label4,label5) :

	gROOT.Reset()
	#gStyle.SetPadRightMargin(0.2);

	#h1 = f1.Get(options.dir + hist1)
	h1 = f1.Get(folder+'/' + hist1)
	h2 = f2.Get(folder+'/' + hist1)
	h3 = f3.Get(folder+'/' + hist1)
	#h4 = f4.Get('plots/AnalysisPlots/' + hist1)
	#h5 = f5.Get('plots/AnalysisPlots/' + hist1)


	h1clone = h1.Clone("h1")
	h2clone = h2.Clone("h2")
	h3clone = h3.Clone("h3")
	#h4clone = h4.Clone("h4")
	#h5clone = h5.Clone("h5")

  	#hs = THStack( h1.GetName()+h2.GetName(), h1.GetTitle() )
  	hs = THStack( h1.GetName()+h2.GetName()+h3.GetName(), h1.GetTitle() )
  	#hs = THStack( h1.GetName()+h2.GetName()+h3.GetName()+h4.GetName(), h1.GetTitle() )
  	#hs = THStack( h1.GetName()+h2.GetName()+h3.GetName()+h4.GetName()+h5.GetName(), h1.GetTitle() )

  	h1clone.SetLineColor(1)
	h2clone.SetLineColor(6)
	h3clone.SetLineColor(2)
	#h4clone.SetLineColor(3)
	#h5clone.SetLineColor(4)
  	h1clone.SetLineWidth(2)
  	h2clone.SetLineWidth(2)
  	h3clone.SetLineWidth(2)
  	#h4clone.SetLineWidth(2)
  	#h5clone.SetLineWidth(2)

	hs.SetTitle(hist1)
	hs.Add( h1clone, 'hist' )
	hs.Add( h2clone, 'hist' )
	hs.Add( h3clone, 'hist' )
	#hs.Add( h4clone, 'hist' )
	#hs.Add( h5clone, 'hist' )

	c = TCanvas('c_' + hist1, 'c_' + hist1,  10, 10, 750, 500 )
  	#c.SetLogy()
  
	hs.Draw('nostack')
	hs.GetXaxis().SetTitle("Mass [GeV]")
	hs.GetYaxis().SetTitle("Events/10 GeV")
	hs.GetYaxis().SetTitleOffset(1.4);


	legend=TLegend(0.70,0.7,0.90,0.9)
	legend.SetFillColor(0);
	legend.AddEntry(h1clone,label1, "l")
	legend.AddEntry(h2clone,label2, "l")    
	legend.AddEntry(h3clone,label3, "l")    
	#legend.AddEntry(h4clone,label4, "l")    
	#legend.AddEntry(h5clone,label5, "l")    
	legend.Draw()
  
	#outhists.append( [h1clone,h2clone] )
	outhists.append( [h1clone,h2clone,h3clone] )
	#outhists.append( [h1clone,h2clone,h3clone,h4clone] )
	#outhists.append( [h1clone,h2clone,h3clone,h4clone,h5clone] )
	stacks.append( hs )
	leg.append( legend )
	canvs.append(c)

	#c.SaveAs(outputDir + hist1 + "_diff_" + decay + "_" + str(st2mass1) + "_" +str(st1mass1) + "_" +str(st1mass2) +".pdf")
	#c.SaveAs(outputDir + hist1 + "_diff_" + decay + "_" + str(st2mass1) + "_" +str(st1mass1) + "_" +str(st1mass2) + "_"  +str(st1mass3) + ".pdf")
	c.SaveAs(outputDir + hist1 + "_diffTriggers_NoScale_" + decay + "_" + str(st2mass1) + "_" +str(st1mass1) + ".pdf")
	#c.SaveAs(outputDir + hist1 + "_diff_" + decay + "_" + str(st2mass1) + "_" +str(st1mass1) + "_" +str(st1mass2) + "_"  +str(st1mass3) + "_"+ str(st1mass4) +".pdf")
	#c.SaveAs(outputDir + hist1 + "_diff_" + decay + "_" + str(st2mass1) + "_" +str(st1mass1) + "_" +str(st1mass2) + "_"  +str(st1mass3) + "_"+ str(st1mass4) + "_"+ str(st1mass4) +".pdf")
	return


#plot( hist, decay+"_"+str(st2mass1)+"_"+str(st1mass1), decay+"_"+str(st2mass1)+"_"+str(st1mass2))
#plot( hist, decay+"_"+str(st2mass1)+"_"+str(st1mass1), decay+"_"+str(st2mass1)+"_"+str(st1mass2), decay+"_"+str(st2mass1)+"_"+str(st1mass3) )
plot( hist, decay+"_"+str(st2mass1)+"_"+str(st1mass1), "4jet80 6jet60", "4jet80" )
#plot( hist, decay+"_"+str(st2mass1)+"_"+str(st1mass1), decay+"_"+str(st2mass1)+"_"+str(st1mass2), decay+"_"+str(st2mass1)+"_"+str(st1mass3), decay+"_"+str(st2mass1)+"_"+str(st1mass4) )
#plot( hist, decay+"_"+str(st2mass1)+"_"+str(st1mass1), decay+"_"+str(st2mass1)+"_"+str(st1mass2), decay+"_"+str(st2mass1)+"_"+str(st1mass3), decay+"_"+str(st2mass1)+"_"+str(st1mass4) , decay+"_"+str(st2mass1)+"_"+str(st1mass5) )

