#!/usr/bin/env python

###################
# plot_Compare2Histos.py
# description: compares two histograms from one root file
# instructions:
#  python -i plot_Compare2Histos.py
###################

from ROOT import *
import glob,sys

####### Calculate Weight
mcScaleFile = open("MCScaleFactors.txt")
for line in mcScaleFile:
	line = line.rstrip()
	if "HT250-500XS" == line.split()[0]:
		HT250_500XS = float(line.split()[1])
	if "HT500-1000XS" == line.split()[0]: 
		HT500_1000XS = float(line.split()[1])
	if "HT1000-InfXS" == line.split()[0]: 
		HT1000_InfXS = float(line.split()[1])
	if "HT250-500nEvents" == line.split()[0]: 
		HT250_500nEvents = float(line.split()[1])
	if "HT500-1000nEvents" == line.split()[0]: 
		HT500_1000nEvents = float(line.split()[1])
	if "HT1000-InfnEvents" == line.split()[0]: 
		HT1000_InfnEvents = float(line.split()[1])
	if "st2XS450" == line.split()[0]:
		XSsample = float(line.split()[1])

weightHT250_500 = 19500*HT250_500XS/HT250_500nEvents
weightHT500_1000 = 19500*HT500_1000XS/HT500_1000nEvents
weightHT1000_Inf = 19500*HT1000_InfXS/HT1000_InfnEvents
weightjj_450_100 = 19500 * XSsample/100000
weightjj_450_200 = 19500 * XSsample/100000
weightjj_450_300 = 19500 * XSsample/100000
##############################################################



def signal(input, weight, label, outputDir) :

	gROOT.Reset()
	gStyle.SetOptStat(0)

	f1 = TFile(input1)
	h1 = f1.Get('cutflow')
	h1clone = h1.Clone("h1")
	h1clone.Scale( weight )

	h1clone.SetLineColor(2)
	h1clone.SetLineWidth(2)

	c = TCanvas('c1', 'c1',  10, 10, 750, 500 )
	c.SetLogy()
	c.SetGrid()
	h1clone.GetYaxis().SetTitle("Events")
	h1clone.GetXaxis().SetBinLabel(1, 'Processed')
	h1clone.GetXaxis().SetBinLabel(2, 'Simple')
	h1clone.GetXaxis().SetBinLabel(3, '4jetTrigger')
	h1clone.GetXaxis().SetBinLabel(4, 'Step1')
	h1clone.GetXaxis().SetBinLabel(5, 'Step2')
	h1clone.GetXaxis().SetBinLabel(6, 'Step2_diag50')
	h1clone.GetXaxis().SetBinLabel(7, 'Step2_diag100')
	h1clone.GetXaxis().SetBinLabel(8, 'Step2_diag150')
	h1clone.GetXaxis().SetBinLabel(9, 'Step2_diag200')
	h1clone.GetXaxis().SetBinLabel(10, 'Step3')
	h1clone.GetXaxis().SetBinLabel(11, 'Step3_diag50')
	h1clone.GetXaxis().SetBinLabel(12, 'Step3_diag100')
	h1clone.GetXaxis().SetBinLabel(13, 'Step4')
	h1clone.Draw()

	legend=TLegend(0.75,0.75,0.90,0.9)
	legend.SetFillColor(0);
	legend.AddEntry(h1clone,label, "l")
	legend.Draw()

	c.SaveAs(outputDir + 'cutflow_'+ label + ".pdf")
	return

def QCD(input1, input2, input3, weight1, weight2, weight3, label, outputDir) :

	gROOT.Reset()
	gStyle.SetOptStat(0)

	f1 = TFile(input2)
	f2 = TFile(input3)
	f3 = TFile(input1)
	h1 = f1.Get('cutflow')
	h2 = f2.Get('cutflow')
	h3 = f3.Get('cutflow')
	h1clone = h1.Clone("h1")
	h2clone = h2.Clone("h2")
	h3clone = h3.Clone("h3")

	h1clone.Scale( weight1 )
	h2clone.Scale( weight2 )
	h3clone.Scale( weight3 )

	h1clone.Add( h2clone )
	#h1clone.Add( h3clone )

	h1clone.SetLineColor(2)
	h1clone.SetLineWidth(2)

	c = TCanvas('c1', 'c1',  10, 10, 750, 500 )
	c.SetLogy()
	c.SetGrid()
	h1clone.GetYaxis().SetTitle("Events")
	h1clone.GetXaxis().SetBinLabel(1, 'Processed')
	h1clone.GetXaxis().SetBinLabel(2, 'Simple')
	h1clone.GetXaxis().SetBinLabel(3, '4jetTrigger')
	h1clone.GetXaxis().SetBinLabel(4, 'Step1')
	h1clone.GetXaxis().SetBinLabel(5, 'Step2')
	h1clone.GetXaxis().SetBinLabel(6, 'Step2_diag50')
	h1clone.GetXaxis().SetBinLabel(7, 'Step2_diag100')
	h1clone.GetXaxis().SetBinLabel(8, 'Step2_diag150')
	h1clone.GetXaxis().SetBinLabel(9, 'Step2_diag200')
	h1clone.GetXaxis().SetBinLabel(10, 'Step3')
	h1clone.GetXaxis().SetBinLabel(11, 'Step3_diag50')
	h1clone.GetXaxis().SetBinLabel(12, 'Step3_diag100')
	h1clone.GetXaxis().SetBinLabel(13, 'Step4')
	h1clone.Draw()


	legend=TLegend(0.75,0.75,0.90,0.9)
	legend.SetFillColor(0);
	legend.AddEntry(h1clone,label, "l")
	legend.Draw()

	c.SaveAs(outputDir + 'cutflow_'+ label + ".pdf")
	return

#############################
sample = str(sys.argv[1])
trigger = str(sys.argv[2])
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

if ( sample == 'signal' ):
	input3 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_450_100_"+trigger+"_plots.root"
	input1 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_450_200_"+trigger+"_plots.root"
	input2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_450_300_"+trigger+"_plots.root"
	signal(input1, weightjj_450_200, 'jj_450_200_'+trigger, outputDir)
	signal(input2, weightjj_450_300, 'jj_450_300_'+trigger, outputDir)
	signal(input3, weightjj_450_100, 'jj_450_100_'+trigger, outputDir)
	signal(input1, 1, 'jj_450_200_'+trigger+'_NOScale', outputDir)
	signal(input2, 1, 'jj_450_300_'+trigger+'NOScale', outputDir)
	signal(input3, 1, 'jj_450_100_'+trigger+'NOScale', outputDir)
elif ( sample == 'QCD' ):
	input1 = "/uscms_data/d3/algomez/files/Stops/Results/HT250-500_"+trigger+"_plots.root"
	input2 = "/uscms_data/d3/algomez/files/Stops/Results/HT500-1000_"+trigger+"_plots.root"
	input3 = "/uscms_data/d3/algomez/files/Stops/Results/HT1000-Inf_"+trigger+"_plots.root"
	QCD(input1, input2, input3,  weightHT250_500, weightHT500_1000, weightHT1000_Inf, 'QCD_'+trigger, outputDir)
	QCD(input1, input2, input3,  1, 1, 1, 'QCD_'+trigger+'_NOScale', outputDir)
else:
	input1 = "/uscms_data/d3/algomez/files/Stops/Results/data_"+trigger+"_plots.root"
	signal(input1, 1, 'data_'+trigger, outputDir)
	#input2 = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet60_plots.root"
	#signal(input2, 1, 'data_4jet60', outputDir)
	#input3 = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet120_plots.root"
	#signal(input3, 1, 'data_4jet120', outputDir)


#print "Input files: ", input1
