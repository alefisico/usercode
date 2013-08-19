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

weightHT250_500 = 20000*HT250_500XS/HT250_500nEvents
weightHT500_1000 = 20000*HT500_1000XS/HT500_1000nEvents
weightHT1000_Inf = 20000*HT1000_InfXS/HT1000_InfnEvents
weightjj_450_200 = 20000 * XSsample/100000
weightjj_450_300 = 20000 * XSsample/100000
##############################################################



def signal(input, weight, label, outputDir) :

	gROOT.Reset()
	gStyle.SetOptStat(0)

	f1 = TFile(input1)
	h1 = f1.Get('cutflow')
	h1clone = h1.Clone("h1")
	h1clone.Scale( weight )

	c = TCanvas('c1', 'c1',  10, 10, 750, 500 )
	c.SetLogy()
	c.SetGrid()
	h1clone.GetYaxis().SetTitle("Entries")
	h1clone.GetXaxis().SetBinLabel(1, 'Processed')
	h1clone.GetXaxis().SetBinLabel(2, 'Simple')
	h1clone.GetXaxis().SetBinLabel(3, '4jetTrigger')
	h1clone.GetXaxis().SetBinLabel(4, 'Step1')
	h1clone.GetXaxis().SetBinLabel(5, 'Step3')
	h1clone.GetXaxis().SetBinLabel(6, 'Step3_diag50')
	h1clone.GetXaxis().SetBinLabel(7, 'Step3_diag100')
	h1clone.GetXaxis().SetBinLabel(8, 'Step3_diag150')
	h1clone.GetXaxis().SetBinLabel(9, 'Step3_diag200')
	h1clone.Draw()

	c.SaveAs(outputDir + 'cutflow_'+ label + ".pdf")
	return

def QCD(input1, input2, input3, weight1, weight2, weight3, label, outputDir) :

	gROOT.Reset()
	gStyle.SetOptStat(0)

	f1 = TFile(input1)
	f2 = TFile(input2)
	f3 = TFile(input3)
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
	h1clone.Add( h3clone )

	c = TCanvas('c1', 'c1',  10, 10, 750, 500 )
	c.SetLogy()
	c.SetGrid()
	h1clone.GetYaxis().SetTitle("Entries")
	h1clone.GetXaxis().SetBinLabel(1, 'Processed')
	h1clone.GetXaxis().SetBinLabel(2, 'Simple')
	h1clone.GetXaxis().SetBinLabel(3, '4jetTrigger')
	h1clone.GetXaxis().SetBinLabel(4, 'Step1')
	h1clone.GetXaxis().SetBinLabel(5, 'Step3')
	h1clone.GetXaxis().SetBinLabel(6, 'Step3_diag50')
	h1clone.GetXaxis().SetBinLabel(7, 'Step3_diag100')
	h1clone.GetXaxis().SetBinLabel(8, 'Step3_diag150')
	h1clone.GetXaxis().SetBinLabel(9, 'Step3_diag200')
	h1clone.Draw()

	c.SaveAs(outputDir + 'cutflow_'+ label + ".pdf")
	return

#############################
sample = str(sys.argv[1])
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

if ( sample == 'signal' ):
	input1 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_450_200_4jet80_plots.root"
	input2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_450_300_4jet80_plots.root"
	signal(input1, weightjj_450_200, 'jj_450_200', outputDir)
	signal(input2, weightjj_450_300, 'jj_450_300', outputDir)
	signal(input1, 1, 'jj_450_200_NOScale', outputDir)
	signal(input2, 1, 'jj_450_300_NOScale', outputDir)
elif ( sample == 'QCD' ):
	input1 = "/uscms_data/d3/algomez/files/Stops/Results/HT250-500_4jet80_plots.root"
	input2 = "/uscms_data/d3/algomez/files/Stops/Results/HT500-1000_4jet80_plots.root"
	input3 = "/uscms_data/d3/algomez/files/Stops/Results/HT1000-Inf_4jet80_plots.root"
	QCD(input1, input2, input3,  weightHT250_500, weightHT500_1000, weightHT1000_Inf, 'QCD', outputDir)
	QCD(input1, input2, input3,  1, 1, 1, 'QCD_NOScale', outputDir)
else:
	input1 = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_plots.root"
	signal(input1, 1, 'data', outputDir)


#print "Input files: ", input1
