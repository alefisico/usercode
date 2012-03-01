#!/usr/bin/env python
###################################################
# This python script ...
####################################################

import os,sys,string,re,math
from array import array

try:
	from ROOT import *
except:
	print "\nCannot load PYROOT, make sure you have setup ROOT in the path"
	print "and pyroot library is also defined in the variable PYTHONPATH, try:\n"
	if (os.getenv("PYTHONPATH")):
		print " setenv PYTHONPATH ${PYTHONPATH}:$ROOTSYS/lib\n"
	else:
		print " setenv PYTHONPATH $ROOTSYS/lib\n"
	#sys.exit()

#import Inspector
#import Style

gROOT.SetStyle("Plain")
gStyle.SetOptStat(0)
DEBUG = False

# style
#thestyle = Style.Style()
HasCMSStyle = False
style = None
if os.path.isfile('tdrstyle.C'):
	gROOT.ProcessLine('.L tdrstyle.C')
	ROOT.setTDRStyle()
	print "Found tdrstyle.C file, using this style."
	HasCMSStyle = True
	if os.path.isfile('CMSTopStyle.cc'):
		gROOT.ProcessLine('.L CMSTopStyle.cc+')
		style = CMSTopStyle()
		print "Found CMSTopStyle.cc file, use TOP style if requested in xml file."
if not HasCMSStyle:
	print "No CMSTopStyle.cc"

#FOpen = ['results_4Top_1100.root',
#		'results_4Top_1000.root',
#		'results_4Top_900.root',
#		'results_4Top_700.root',
#		'results_4Top_500.root',
#		'results_4TopSM.root']
#
########################################
##def Disc(ofile, name):
#######################################	
#for ofile in FOpen:
#	tmpname = ofile.replace('results_','')
#	name = tmpname.replace('.root','')
#
#	infile = TFile(ofile)
#	histo1 = infile.Get("/MET/Stlep_"+name)
#	histo2 = infile.Get("/MET/Ht_"+name)
#
#	scale1 = 1/histo1.Integral()
#	scale2 = 1/histo2.Integral()
#
#	histo1.Scale(scale1)
#	histo2.Scale(scale2)
#
#	#totalhisto = TH1F(histo1*histo2)
#
#	#if ofile[1]:
#	#	c1.cd()
#	#	histo1.Draw("Hist")
#	#else:
#	#	c1.cd()
#	#	histo1.Draw("Hist same")
#	#if ofile[len(FOpen)]: c1.SaveAs("test.png")

def legend():
	leg = TLegend(0.75,0.65,0.93,0.93)
	leg.SetMargin(0.12)
	leg.SetTextSize(0.035)
	leg.SetFillColor(0)
	leg.SetFillStyle(0)
	leg.SetBorderSize(0)
	leg.AddEntry(h4TopSM_1, style.FourTopSMText, "l" )
	leg.AddEntry(h4Top_1100_1,"Gh 1.1 TeV", "l" )
	leg.AddEntry(h4Top_1000_1,"Gh 1.0 TeV", "l" )
	leg.AddEntry(h4Top_900_1,"Gh 0.9 TeV", "l" )
	leg.AddEntry(h4Top_700_1,"Gh 0.7 TeV", "l" )
	leg.AddEntry(h4Top_500_1,"Gh 0.5 TeV", "l" )
	leg.Draw()
	return leg


# Open files
f4Top_1100 = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_4Top1100.root")
h4Top_1100_1 = f4Top_1100.Get("/MET/Stjet_4Top1100")
h4Top_1100_1.Scale(1/h4Top_1100_1.Integral())
h4Top_1100_2 = f4Top_1100.Get("/MET/Ht_4Top1100")
h4Top_1100_2.Scale(1/h4Top_1100_2.Integral())
h4Top_1100_3 = f4Top_1100.Get("/MET/Stlep_4Top1100")
h4Top_1100_3.Scale(1/h4Top_1100_3.Integral())
h4Top_1100_4 = f4Top_1100.Get("/jets/Njets_cut1_4Top1100")
h4Top_1100_4.Scale(1/h4Top_1100_4.Integral())
h4Top_1100_5 = f4Top_1100.Get("/jets/Nbjets_CSVL_4Top1100")
h4Top_1100_5.Scale(1/h4Top_1100_5.Integral())
#h4Top_1100_6 = f4Top_1100.Get("/jets/Nbjets_CSVL_4Top1100")
#h4Top_1100_6.Scale(1/h4Top_1100_6.Integral())

f4Top_1000 = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_4Top1000.root")
h4Top_1000_1 = f4Top_1000.Get("/MET/Stjet_4Top1000")
h4Top_1000_1.Scale(1/h4Top_1000_1.Integral())
h4Top_1000_2 = f4Top_1000.Get("/MET/Ht_4Top1000")
h4Top_1000_2.Scale(1/h4Top_1000_2.Integral())
h4Top_1000_3 = f4Top_1000.Get("/MET/Stlep_4Top1000")
h4Top_1000_3.Scale(1/h4Top_1000_3.Integral())
h4Top_1000_4 = f4Top_1000.Get("/jets/Njets_cut1_4Top1000")
h4Top_1000_4.Scale(1/h4Top_1000_4.Integral())
h4Top_1000_5 = f4Top_1000.Get("/jets/Nbjets_CSVL_4Top1000")
h4Top_1000_5.Scale(1/h4Top_1000_5.Integral())

f4Top_900 = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_4Top900.root")
h4Top_900_1 = f4Top_900.Get("/MET/Stjet_4Top900")
h4Top_900_1.Scale(1/h4Top_900_1.Integral())
h4Top_900_2 = f4Top_900.Get("/MET/Ht_4Top900")
h4Top_900_2.Scale(1/h4Top_900_2.Integral())
h4Top_900_3 = f4Top_900.Get("/MET/Stlep_4Top900")
h4Top_900_3.Scale(1/h4Top_900_3.Integral())
h4Top_900_4 = f4Top_900.Get("/jets/Njets_cut1_4Top900")
h4Top_900_4.Scale(1/h4Top_900_4.Integral())
h4Top_900_5 = f4Top_900.Get("/jets/Nbjets_CSVL_4Top900")
h4Top_900_5.Scale(1/h4Top_900_5.Integral())

f4Top_700 = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_4Top700.root")
h4Top_700_1 = f4Top_700.Get("/MET/Stjet_4Top700")
h4Top_700_1.Scale(1/h4Top_700_1.Integral())
h4Top_700_2 = f4Top_700.Get("/MET/Ht_4Top700")
h4Top_700_2.Scale(1/h4Top_700_2.Integral())
h4Top_700_3 = f4Top_700.Get("/MET/Stlep_4Top700")
h4Top_700_3.Scale(1/h4Top_700_3.Integral())
h4Top_700_4 = f4Top_700.Get("/jets/Njets_cut1_4Top700")
h4Top_700_4.Scale(1/h4Top_700_4.Integral())
h4Top_700_5 = f4Top_700.Get("/jets/Nbjets_CSVL_4Top700")
h4Top_700_5.Scale(1/h4Top_700_5.Integral())

f4Top_500 = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_4Top500.root")
h4Top_500_1 = f4Top_500.Get("/MET/Stjet_4Top500")
h4Top_500_1.Scale(1/h4Top_500_1.Integral())
h4Top_500_2 = f4Top_500.Get("/MET/Ht_4Top500")
h4Top_500_2.Scale(1/h4Top_500_2.Integral())
h4Top_500_3 = f4Top_500.Get("/MET/Stlep_4Top500")
h4Top_500_3.Scale(1/h4Top_500_3.Integral())
h4Top_500_4 = f4Top_500.Get("/jets/Njets_cut1_4Top500")
h4Top_500_4.Scale(1/h4Top_500_4.Integral())
h4Top_500_5 = f4Top_500.Get("/jets/Nbjets_CSVL_4Top500")
h4Top_500_5.Scale(1/h4Top_500_5.Integral())

f4TopSM = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_4TopSM.root")
h4TopSM_1 = f4TopSM.Get("/MET/Stjet_4TopSM")
h4TopSM_1.Scale(1/h4TopSM_1.Integral())
h4TopSM_2 = f4TopSM.Get("/MET/Ht_4TopSM")
h4TopSM_2.Scale(1/h4TopSM_2.Integral())
h4TopSM_3 = f4TopSM.Get("/MET/Stlep_4TopSM")
h4TopSM_3.Scale(1/h4TopSM_3.Integral())
h4TopSM_4 = f4TopSM.Get("/jets/Njets_cut1_4TopSM")
h4TopSM_4.Scale(1/h4TopSM_4.Integral())
h4TopSM_5 = f4TopSM.Get("/jets/Nbjets_CSVL_4TopSM")
h4TopSM_5.Scale(1/h4TopSM_5.Integral())

						
can1 = TCanvas('can1', '', 800,600)
can1.cd()
can1.SetLogy()

h4Top_1100_1.SetLineColor(1)
h4Top_1100_1.SetMaximum(1)
h4Top_1100_1.SetYTitle('1/N dN/dx')
h4Top_1100_1.Draw("Hist")

h4Top_1000_1.SetLineColor(2)
h4Top_1000_1.SetLineWidth(1)
h4Top_1000_1.Draw("Hist same")

h4Top_900_1.SetLineColor(3)
h4Top_900_1.SetLineWidth(1)
h4Top_900_1.Draw("Hist same")

h4Top_700_1.SetLineColor(4)
h4Top_700_1.SetLineWidth(1)
h4Top_700_1.Draw("Hist same")

h4Top_500_1.SetLineColor(6)
h4Top_500_1.SetLineWidth(1)
h4Top_500_1.Draw("Hist same")

h4TopSM_1.SetLineColor(style.FourTopSMColor)
h4TopSM_1.SetLineWidth(2)
h4TopSM_1.Draw("Hist same")

leg = legend()
can1.SaveAs("diff_Stjet_4Top.png")

can2 = TCanvas('can2', '', 800,600)
can2.cd()
can2.SetLogy()

h4Top_1100_2.SetLineColor(1)
h4Top_1100_2.SetMaximum(1)
h4Top_1100_2.SetYTitle('1/N dN/dx')
h4Top_1100_2.Draw("Hist")

h4Top_1000_2.SetLineColor(2)
h4Top_1000_2.SetLineWidth(1)
h4Top_1000_2.Draw("Hist same")

h4Top_900_2.SetLineColor(3)
h4Top_900_2.SetLineWidth(1)
h4Top_900_2.Draw("Hist same")

h4Top_700_2.SetLineColor(4)
h4Top_700_2.SetLineWidth(1)
h4Top_700_2.Draw("Hist same")

h4Top_500_2.SetLineColor(6)
h4Top_500_2.SetLineWidth(1)
h4Top_500_2.Draw("Hist same")

h4TopSM_2.SetLineColor(style.FourTopSMColor)
h4TopSM_2.SetLineWidth(2)
h4TopSM_2.Draw("Hist same")

leg = legend()
can2.SaveAs("diff_Ht_4Top.png")

can3 = TCanvas('can3', '', 800,600)
can3.cd()
can3.SetLogy()

h4Top_1100_3.SetLineColor(1)
h4Top_1100_3.SetMaximum(1)
h4Top_1100_3.SetYTitle('1/N dN/dx')
h4Top_1100_3.Draw("Hist")

h4Top_1000_3.SetLineColor(2)
h4Top_1000_3.SetLineWidth(1)
h4Top_1000_3.Draw("Hist same")

h4Top_900_3.SetLineColor(3)
h4Top_900_3.SetLineWidth(1)
h4Top_900_3.Draw("Hist same")

h4Top_700_3.SetLineColor(4)
h4Top_700_3.SetLineWidth(1)
h4Top_700_3.Draw("Hist same")

h4Top_500_3.SetLineColor(6)
h4Top_500_3.SetLineWidth(1)
h4Top_500_3.Draw("Hist same")

h4TopSM_3.SetLineColor(style.FourTopSMColor)
h4TopSM_3.SetLineWidth(2)
h4TopSM_3.Draw("Hist same")

leg = legend()
can3.SaveAs("diff_Stlep_4Top.png")

can4 = TCanvas('can4', '', 800,600)
can4.cd()
can4.SetLogy()

h4Top_1100_4.SetLineColor(1)
h4Top_1100_4.SetMaximum(1)
h4Top_1100_4.SetYTitle('1/N dN/dx')
h4Top_1100_4.Draw("Hist")

h4Top_1000_4.SetLineColor(2)
h4Top_1000_4.SetLineWidth(1)
h4Top_1000_4.Draw("Hist same")

h4Top_900_4.SetLineColor(3)
h4Top_900_4.SetLineWidth(1)
h4Top_900_4.Draw("Hist same")

h4Top_700_4.SetLineColor(4)
h4Top_700_4.SetLineWidth(1)
h4Top_700_4.Draw("Hist same")

h4Top_500_4.SetLineColor(6)
h4Top_500_4.SetLineWidth(1)
h4Top_500_4.Draw("Hist same")

h4TopSM_4.SetLineColor(style.FourTopSMColor)
h4TopSM_4.SetLineWidth(2)
h4TopSM_4.Draw("Hist same")

leg = legend()
can4.SaveAs("diff_Njets_4Top.png")

can5 = TCanvas('can5', '', 800,600)
can5.cd()
can5.SetLogy()

h4Top_1100_5.SetLineColor(1)
h4Top_1100_5.SetMaximum(1)
h4Top_1100_5.SetYTitle('1/N dN/dx')
h4Top_1100_5.Draw("Hist")

h4Top_1000_5.SetLineColor(2)
h4Top_1000_5.SetLineWidth(1)
h4Top_1000_5.Draw("Hist same")

h4Top_900_5.SetLineColor(3)
h4Top_900_5.SetLineWidth(1)
h4Top_900_5.Draw("Hist same")

h4Top_700_5.SetLineColor(4)
h4Top_700_5.SetLineWidth(1)
h4Top_700_5.Draw("Hist same")

h4Top_500_5.SetLineColor(6)
h4Top_500_5.SetLineWidth(1)
h4Top_500_5.Draw("Hist same")

h4TopSM_5.SetLineColor(style.FourTopSMColor)
h4TopSM_5.SetLineWidth(2)
h4TopSM_5.Draw("Hist same")

leg = legend()
can5.SaveAs("diff_Nbjets_CSVL_4Top.png")
