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

#FOpen = ['results_WW.root',
#		'results_WZ.root',
#		'results_WJets.root',
#		'results_ZJets.root',
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
	leg.AddEntry(hWZ_1,style.DibosonsText, "l" )
	leg.AddEntry(hWJets_1, style.WJetsText, "l" )
	leg.AddEntry(hZJets_1,style.DYZJetsText, "l" )
	leg.Draw()
	return leg


# Open files
fWW = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_WW.root")
hWW_1 = fWW.Get("/MET/Stjet_WW")
hWW_2 = fWW.Get("/MET/Ht_WW")
hWW_3 = fWW.Get("/MET/Stlep_WW")
hWW_4 = fWW.Get("/jets/Njets_cut1_WW")
hWW_5 = fWW.Get("/jets/Nbjets_CSVL_WW")
#hWW_6 = fWW.Get("/jets/Nbjets_CSVL_WW")

fWZ = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_WZ.root")
hWZ_1 = fWZ.Get("/MET/Stjet_WZ")
hWZ_1.Add(hWW_1)
hWZ_1.Sumw2()
hWZ_1.Scale(1/hWZ_1.Integral())
hWZ_2 = fWZ.Get("/MET/Ht_WZ")
hWZ_2.Add(hWW_2)
hWZ_2.Sumw2()
hWZ_2.Scale(1/hWZ_2.Integral())
hWZ_3 = fWZ.Get("/MET/Stlep_WZ")
hWZ_3.Add(hWW_3)
hWZ_3.Sumw2()
hWZ_3.Scale(1/hWZ_3.Integral())
hWZ_4 = fWZ.Get("/jets/Njets_cut1_WZ")
hWZ_4.Add(hWW_4)
hWZ_4.Sumw2()
hWZ_4.Scale(1/hWZ_4.Integral())
hWZ_5 = fWZ.Get("/jets/Nbjets_CSVL_WZ")
hWZ_5.Add(hWW_5)
hWZ_5.Sumw2()
hWZ_5.Scale(1/hWZ_5.Integral())

fWJets = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_WJets.root")
hWJets_1 = fWJets.Get("/MET/Stjet_WJets")
hWJets_1.Scale(1/hWJets_1.Integral())
hWJets_2 = fWJets.Get("/MET/Ht_WJets")
hWJets_2.Scale(1/hWJets_2.Integral())
hWJets_3 = fWJets.Get("/MET/Stlep_WJets")
hWJets_3.Scale(1/hWJets_3.Integral())
hWJets_4 = fWJets.Get("/jets/Njets_cut1_WJets")
hWJets_4.Scale(1/hWJets_4.Integral())
hWJets_5 = fWJets.Get("/jets/Nbjets_CSVL_WJets")
hWJets_5.Scale(1/hWJets_5.Integral())

fZJets = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_ZJets.root")
hZJets_1 = fZJets.Get("/MET/Stjet_ZJets")
hZJets_1.Scale(1/hZJets_1.Integral())
hZJets_2 = fZJets.Get("/MET/Ht_ZJets")
hZJets_2.Scale(1/hZJets_2.Integral())
hZJets_3 = fZJets.Get("/MET/Stlep_ZJets")
hZJets_3.Scale(1/hZJets_3.Integral())
hZJets_4 = fZJets.Get("/jets/Njets_cut1_ZJets")
hZJets_4.Scale(1/hZJets_4.Integral())
hZJets_5 = fZJets.Get("/jets/Nbjets_CSVL_ZJets")
hZJets_5.Scale(1/hZJets_5.Integral())

						
can1 = TCanvas('can1', '', 800,600)
can1.cd()
can1.SetLogy()

#hDibosons.SetLineColor(1)
#hDibosons.SetMaximum(1)
#hDibosons.Draw("Hist")

hWZ_1.SetLineColor(style.DibosonsColor)
hWZ_1.SetLineWidth(1)
hWZ_1.SetYTitle('1/N dN/dx')
hWZ_1.Draw("Hist")

hWJets_1.SetLineColor(style.WJetsColor)
hWJets_1.SetLineWidth(1)
hWJets_1.Draw("Hist same")

hZJets_1.SetLineColor(style.DYZJetsColor)
hZJets_1.SetLineWidth(1)
hZJets_1.Draw("Hist same")

leg = legend()
can1.SaveAs("diff_Stjet_dibosons.png")

can2 = TCanvas('can2', '', 800,600)
can2.cd()
can2.SetLogy()

hWZ_2.SetLineColor(style.DibosonsColor)
hWZ_2.SetLineWidth(1)
hWZ_2.SetYTitle('1/N dN/dx')
hWZ_2.Draw("Hist")

hWJets_1.SetLineColor(style.WJetsColor)
hWJets_1.SetLineWidth(1)
hWJets_1.Draw("Hist same")

hZJets_1.SetLineColor(style.DYZJetsColor)
hZJets_1.SetLineWidth(1)
hZJets_1.Draw("Hist same")

leg = legend()
can2.SaveAs("diff_Ht_dibosons.png")

can3 = TCanvas('can3', '', 800,600)
can3.cd()
can3.SetLogy()

hWZ_3.SetLineColor(style.DibosonsColor)
hWZ_3.SetLineWidth(1)
hWZ_3.SetYTitle('1/N dN/dx')
hWZ_3.Draw("Hist")

hWJets_3.SetLineColor(style.WJetsColor)
hWJets_3.SetLineWidth(1)
hWJets_3.Draw("Hist same")

hZJets_3.SetLineColor(style.DYZJetsColor)
hZJets_3.SetLineWidth(1)
hZJets_3.Draw("Hist same")

leg = legend()
can3.SaveAs("diff_Stlep_dibosons.png")

can4 = TCanvas('can4', '', 800,600)
can4.cd()
can4.SetLogy()

hWZ_4.SetLineColor(style.DibosonsColor)
hWZ_4.SetLineWidth(1)
hWZ_4.SetYTitle('1/N dN/dx')
hWZ_4.Draw("Hist")

hWJets_4.SetLineColor(style.WJetsColor)
hWJets_4.SetLineWidth(1)
hWJets_4.Draw("Hist same")

hZJets_4.SetLineColor(style.DYZJetsColor)
hZJets_4.SetLineWidth(1)
hZJets_4.Draw("Hist same")

leg = legend()
can4.SaveAs("diff_Njets_dibosons.png")

can5 = TCanvas('can5', '', 800,600)
can5.cd()
can5.SetLogy()

hWZ_5.SetLineColor(style.DibosonsColor)
hWZ_5.SetLineWidth(1)
hWZ_5.SetYTitle('1/N dN/dx')
hWZ_5.Draw("Hist")

hWJets_5.SetLineColor(style.WJetsColor)
hWJets_5.SetLineWidth(1)
hWJets_5.Draw("Hist same")

hZJets_5.SetLineColor(style.DYZJetsColor)
hZJets_5.SetLineWidth(1)
hZJets_5.Draw("Hist same")

leg = legend()
can5.SaveAs("diff_Nbjets_CSVL_dibosons.png")
