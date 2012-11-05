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
	sys.exit()

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

FOpen = ['results_4Top_1000.root',
		'results_4Top_500.root',
		'results_QCD.root',
		'results_STsch.root',
		'results_STsch_bar.root',
		'results_STtWch.root',
		'results_STtWch_bar.root',
		'results_STtch.root',
		'results_STtch_bar.root',
		'results_WJets.root',
		'results_ZJets.root',
		'results_WW.root',
		'results_WZ.root',
		'results_ttbar.root']

#######################################
#def Disc(ofile, name):
######################################	
for ofile in FOpen:
	tmpname = ofile.replace('results_','')
	name = tmpname.replace('.root','')

	infile = TFile(ofile)
	histo1 = infile.Get("/jets/1st_bdisc_"+name)
	histo2 = infile.Get("/jets/2nd_bdisc_"+name)

	scale1 = 1/histo1.Integral()
	scale2 = 1/histo2.Integral()

	histo1.Scale(scale1)
	histo2.Scale(scale2)

	totalhisto = TH1F(histo1*histo2)

	#if ofile[1]:
	#	c1.cd()
	#	histo1.Draw("Hist")
	#else:
	#	c1.cd()
	#	histo1.Draw("Hist same")
	#if ofile[len(FOpen)]: c1.SaveAs("test.png")

def legend():
	leg = TLegend(0.66,0.65,0.93,0.93)
	leg.SetMargin(0.12)
	leg.SetTextSize(0.035)
	leg.SetFillColor(0)
	leg.SetFillStyle(0)
	leg.SetBorderSize(0)
	leg.AddEntry(httbar_1, style.TtbarText, "l" )
	leg.AddEntry(hWJets_1,style.WJetsText, "l" )
	leg.AddEntry(hZJets_1, style.DYZJetsText, "l" )
	leg.AddEntry(hQCD_1,"QCD", "l" )
	leg.AddEntry(hWZ_1,"Dibosons", "l" )
	leg.AddEntry(hSTtch_bar_1,"Single-Top", "l" )
	leg.AddEntry(h4Top_1000_1,"Gh 1 TeV", "l" )
	leg.AddEntry(h4Top_500_1,"Gh 0.5 TeV", "l" )
	leg.Draw()
	return leg


# Open files
f4Top_1000 = TFile("results_4Top_1000.root")
h4Top_1000_1 = f4Top_1000.Get("/jets/1st_bdisc_4Top_1000")
h4Top_1000_1.Scale(1/h4Top_1000_1.Integral())
h4Top_1000_2 = f4Top_1000.Get("/jets/2nd_bdisc_4Top_1000")
h4Top_1000_2.Scale(1/h4Top_1000_2.Integral())
h4Top_1000 = TH1F(h4Top_1000_1*h4Top_1000_2)
#h4Top_1000_3 = f4Top_1000.Get("/jets/3rd_bdisc_4Top_1000")

f4Top_500 = TFile("results_4Top_500.root")
h4Top_500_1 = f4Top_500.Get("/jets/1st_bdisc_4Top_500")
h4Top_500_1.Scale(1/h4Top_500_1.Integral())
h4Top_500_2 = f4Top_500.Get("/jets/2nd_bdisc_4Top_500")
h4Top_500_2.Scale(1/h4Top_500_2.Integral())
h4Top_500 = TH1F(h4Top_500_1*h4Top_500_2)

fQCD = TFile("results_QCD.root")
hQCD_1 = fQCD.Get("/jets/1st_bdisc_QCD")
hQCD_1.Scale(1/hQCD_1.Integral())
hQCD_2 = fQCD.Get("/jets/2nd_bdisc_QCD")
hQCD_2.Scale(1/hQCD_2.Integral())
hQCD = TH1F(hQCD_1*hQCD_2)

fSTsch = TFile("results_STsch.root")
hSTsch_1 = fSTsch.Get("/jets/1st_bdisc_STsch")
hSTsch_1.Scale(1/hSTsch_1.Integral())
hSTsch_2 = fSTsch.Get("/jets/2nd_bdisc_STsch")
hSTsch_2.Scale(1/hSTsch_2.Integral())
hSTsch = TH1F(hSTsch_1*hSTsch_2)

fSTsch_bar = TFile("results_STsch_bar.root")
hSTsch_bar_1 = fSTsch_bar.Get("/jets/1st_bdisc_STsch_bar")
hSTsch_bar_1.Scale(1/hSTsch_bar_1.Integral())
hSTsch_bar_2 = fSTsch_bar.Get("/jets/2nd_bdisc_STsch_bar")
hSTsch_bar_2.Scale(1/hSTsch_bar_2.Integral())
hSTsch_bar = TH1F(hSTsch_bar_1*hSTsch_bar_2)

fSTtWch = TFile("results_STtWch.root")
hSTtWch_1 = fSTtWch.Get("/jets/1st_bdisc_STtWch")
hSTtWch_1.Scale(1/hSTtWch_1.Integral())
hSTtWch_2 = fSTtWch.Get("/jets/2nd_bdisc_STtWch")
hSTtWch_2.Scale(1/hSTtWch_2.Integral())
hSTtWch = TH1F(hSTtWch_1*hSTtWch_2)

fSTtWch_bar = TFile("results_STtWch_bar.root")
hSTtWch_bar_1 = fSTtWch_bar.Get("/jets/1st_bdisc_STtWch_bar")
hSTtWch_bar_1.Scale(1/hSTtWch_bar_1.Integral())
hSTtWch_bar_2 = fSTtWch_bar.Get("/jets/2nd_bdisc_STtWch_bar")
hSTtWch_bar_2.Scale(1/hSTtWch_bar_2.Integral())
hSTtWch_bar = TH1F(hSTtWch_bar_1*hSTtWch_bar_2)

fSTtch = TFile("results_STtch.root")
hSTtch_1 = fSTtch.Get("/jets/1st_bdisc_STtch")
hSTtch_1.Scale(1/hSTtch_1.Integral())
hSTtch_2 = fSTtch.Get("/jets/2nd_bdisc_STtch")
hSTtch_2.Scale(1/hSTtch_2.Integral())
hSTtch = TH1F(hSTtch_1*hSTtch_2)

fSTtch_bar = TFile("results_STtch_bar.root")
hSTtch_bar_1 = fSTtch_bar.Get("/jets/1st_bdisc_STtch_bar")
hSTtch_bar_1.Scale(1/hSTtch_bar_1.Integral())
hSTtch_bar_2 = fSTtch_bar.Get("/jets/2nd_bdisc_STtch_bar")
hSTtch_bar_2.Scale(1/hSTtch_bar_2.Integral())
hSTtch_bar = TH1F(hSTtch_bar_1*hSTtch_bar_2)

fWJets = TFile("results_WJets.root")
hWJets_1 = fWJets.Get("/jets/1st_bdisc_WJets")
hWJets_1.Scale(1/hWJets_1.Integral())
hWJets_2 = fWJets.Get("/jets/2nd_bdisc_WJets")
hWJets_2.Scale(1/hWJets_2.Integral())
hWJets = TH1F(hWJets_1*hWJets_2)

fZJets = TFile("results_ZJets.root")
hZJets_1 = fZJets.Get("/jets/1st_bdisc_ZJets")
hZJets_1.Scale(1/hZJets_1.Integral())
hZJets_2 = fZJets.Get("/jets/2nd_bdisc_ZJets")
hZJets_2.Scale(1/hZJets_2.Integral())
hZJets = TH1F(hZJets_1*hZJets_2)

fWW = TFile("results_WW.root")
hWW_1 = fWW.Get("/jets/1st_bdisc_WW")
hWW_1.Scale(1/hWW_1.Integral())
hWW_2 = fWW.Get("/jets/2nd_bdisc_WW")
hWW_2.Scale(1/hWW_2.Integral())
hWW = TH1F(hWW_1*hWW_2)

fWZ = TFile("results_WZ.root")
hWZ_1 = fWZ.Get("/jets/1st_bdisc_WZ")
hWZ_1.Scale(1/hWZ_1.Integral())
hWZ_2 = fWZ.Get("/jets/2nd_bdisc_WZ")
hWZ_2.Scale(1/hWZ_2.Integral())
hWZ = TH1F(hWZ_1*hWZ_2)

fttbar = TFile("results_ttbar.root")
httbar_1 = fttbar.Get("/jets/1st_bdisc_ttbar")
httbar_1.Scale(1/httbar_1.Integral())
httbar_2 = fttbar.Get("/jets/2nd_bdisc_ttbar")
httbar_2.Scale(1/httbar_2.Integral())
httbar_3 = fttbar.Get("/jets/3rd_bdisc_ttbar")
httbar = TH1F(httbar_1*httbar_2)

						
can = TCanvas('can', '', 1000,800)
can.cd()
can.SetLogy()

h4Top_1000_1.SetLineColor(1)
h4Top_1000_1.Draw("Hist")

h4Top_500_1.SetLineColor(1)
h4Top_500_1.SetLineWidth(2)
h4Top_500_1.Draw("Hist same")

hQCD_1.SetLineColor(style.QCDColor)
hQCD_1.SetLineWidth(2)
hQCD_1.Draw("Hist same")

hSTsch_bar_1.Add(hSTsch_1)
hSTtWch_1.Add(hSTsch_bar_1)
hSTtWch_bar_1.Add(hSTtWch_1)
hSTtch_1.Add(hSTtWch_bar_1)
hSTtch_bar_1.Add(hSTtch_1)
hSTtch_bar_1.SetLineColor(style.SingleTopColor)
hSTtch_bar_1.SetLineWidth(2)
hSTtch_bar_1.Draw("Hist same")

hWJets_1.SetLineColor(style.WJetsColor)
hWJets_1.SetLineWidth(2)
hWJets_1.Draw("Hist same")

hZJets_1.SetLineColor(style.DYZJetsColor)
hZJets_1.SetLineWidth(2)
hZJets_1.Draw("Hist same")

hWZ_1.Add(hWW_1)
hWZ_1.SetLineColor(style.DibosonsColor)
hWZ_1.SetLineWidth(2)
hWZ_1.Draw("Hist same")

httbar_1.SetLineColor(style.TtbarColor)
httbar_1.SetLineWidth(2)
httbar_1.Draw("Hist same")

leg = legend()
can.SaveAs("1st_bdisc.png")


can2 = TCanvas('can2', '', 1000,800)
can2.cd()
can2.SetLogy()
h4Top_1000_2.SetLineColor(1)
h4Top_1000_2.Draw("Hist")

h4Top_500_2.SetLineColor(1)
h4Top_500_2.SetLineWidth(2)
h4Top_500_2.Draw("Hist same")

hQCD_2.SetLineColor(style.QCDColor)
hQCD_2.SetLineWidth(2)
hQCD_2.Draw("Hist same")

hSTsch_bar_2.Add(hSTsch_2)
hSTtWch_2.Add(hSTsch_bar_2)
hSTtWch_bar_2.Add(hSTtWch_2)
hSTtch_2.Add(hSTtWch_bar_2)
hSTtch_bar_2.Add(hSTtch_2)
hSTtch_bar_2.SetLineColor(style.SingleTopColor)
hSTtch_bar_2.SetLineWidth(2)
hSTtch_bar_2.Draw("Hist same")

hWJets_2.SetLineColor(style.WJetsColor)
hWJets_2.SetLineWidth(2)
hWJets_2.Draw("Hist same")

hZJets_2.SetLineColor(style.DYZJetsColor)
hZJets_2.SetLineWidth(2)
hZJets_2.Draw("Hist same")

hWZ_2.Add(hWW_2)
hWZ_2.SetLineColor(style.DibosonsColor)
hWZ_2.SetLineWidth(2)
hWZ_2.Draw("Hist same")

httbar_2.SetLineColor(style.TtbarColor)
httbar_2.SetLineWidth(2)
httbar_2.Draw("Hist same")

leg = legend()
can2.SaveAs("2nd_bdisc.png")


can3 = TCanvas('can3', '', 1000,800)
can3.cd()
can3.SetLogy()

h4Top_1000.SetLineColor(1)
h4Top_1000.Draw("Hist")
h4Top_1000.GetXaxis().SetTitle("total b discriminator")
h4Top_1000.SetTitle("Total B Discriminator")
h4Top_1000.SetMinimum(10e-7)
h4Top_1000.SetMaximum(10e-2)

h4Top_500.SetLineColor(1)
h4Top_500.SetLineWidth(2)
h4Top_500.Draw("Hist same")

hQCD.SetLineColor(style.QCDColor)
hQCD.SetLineWidth(2)
hQCD.Draw("Hist same")

hSTsch_bar.Add(hSTsch)
hSTtWch.Add(hSTsch_bar)
hSTtWch_bar.Add(hSTtWch)
hSTtch.Add(hSTtWch_bar)
hSTtch_bar.Add(hSTtch)
hSTtch_bar.SetLineColor(style.SingleTopColor)
hSTtch_bar.SetLineWidth(2)
hSTtch_bar.Draw("Hist same")

hWJets.SetLineColor(style.WJetsColor)
hWJets.SetLineWidth(2)
hWJets.Draw("Hist same")

hZJets.SetLineColor(style.DYZJetsColor)
hZJets.SetLineWidth(2)
hZJets.Draw("Hist same")

hWZ.Add(hWW)
hWZ.SetLineColor(style.DibosonsColor)
hWZ.SetLineWidth(2)
hWZ.Draw("Hist same")

httbar.SetLineColor(style.TtbarColor)
httbar.SetLineWidth(2)
httbar.Draw("Hist same")

leg = legend()
can3.SaveAs("total_bdisc.png")
