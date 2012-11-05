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

#FOpen = ['results_ttbar.root',
#		'results_STtch.root',
#		'results_STtch_bar.root',
#		'results_STsch.root',
#		'results_STsch_bar.root',
#		'results_STtWch.root']
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
	leg.AddEntry(httbar_1, style.TtbarText, "l" )
	leg.AddEntry(hSTsch_bar_1,style.ST_t_sText, "l" )
	leg.AddEntry(hSTtWch_bar_1,style.ST_tWText, "l" )
	leg.Draw()
	return leg


# Open files
fttbar = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_ttbar.root")
httbar_1 = fttbar.Get("/MET/Stjet_ttbar")
httbar_1.Scale(1/httbar_1.Integral())
httbar_2 = fttbar.Get("/MET/Ht_ttbar")
httbar_2.Scale(1/httbar_2.Integral())
httbar_3 = fttbar.Get("/MET/Stlep_ttbar")
httbar_3.Scale(1/httbar_3.Integral())
httbar_4 = fttbar.Get("/jets/Njets_cut1_ttbar")
httbar_4.Scale(1/httbar_4.Integral())
httbar_5 = fttbar.Get("/jets/Nbjets_CSVL_ttbar")
httbar_5.Scale(1/httbar_5.Integral())
#httbar_6 = fttbar.Get("/jets/Nbjets_CSVL_ttbar")
#httbar_6.Scale(1/httbar_6.Integral())

fSTtch = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_STtch.root")
hSTtch_1 = fSTtch.Get("/MET/Stjet_STtch")
hSTtch_2 = fSTtch.Get("/MET/Ht_STtch")
hSTtch_3 = fSTtch.Get("/MET/Stlep_STtch")
hSTtch_4 = fSTtch.Get("/jets/Njets_cut1_STtch")
hSTtch_5 = fSTtch.Get("/jets/Nbjets_CSVL_STtch")

fSTtch_bar = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_STtch_bar.root")
hSTtch_bar_1 = fSTtch_bar.Get("/MET/Stjet_STtch_bar")
hSTtch_bar_1.Add(hSTtch_1)
hSTtch_bar_2 = fSTtch_bar.Get("/MET/Ht_STtch_bar")
hSTtch_bar_2.Add(hSTtch_2)
hSTtch_bar_3 = fSTtch_bar.Get("/MET/Stlep_STtch_bar")
hSTtch_bar_3.Add(hSTtch_3)
hSTtch_bar_4 = fSTtch_bar.Get("/jets/Njets_cut1_STtch_bar")
hSTtch_bar_4.Add(hSTtch_4)
hSTtch_bar_5 = fSTtch_bar.Get("/jets/Nbjets_CSVL_STtch_bar")
hSTtch_bar_5.Add(hSTtch_5)

fSTsch = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_STsch.root")
hSTsch_1 = fSTsch.Get("/MET/Stjet_STsch")
hSTsch_1.Add(hSTtch_bar_1)
hSTsch_2 = fSTsch.Get("/MET/Ht_STsch")
hSTsch_2.Add(hSTtch_bar_2)
hSTsch_3 = fSTsch.Get("/MET/Stlep_STsch")
hSTsch_3.Add(hSTtch_bar_3)
hSTsch_4 = fSTsch.Get("/jets/Njets_cut1_STsch")
hSTsch_4.Add(hSTtch_bar_4)
hSTsch_5 = fSTsch.Get("/jets/Nbjets_CSVL_STsch")
hSTsch_5.Add(hSTtch_bar_5)

fSTsch_bar = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_STsch_bar.root")
hSTsch_bar_1 = fSTsch_bar.Get("/MET/Stjet_STsch_bar")
hSTsch_bar_1.Add(hSTsch_1)
hSTsch_bar_1.Sumw2()
hSTsch_bar_1.Scale(1/hSTsch_bar_1.Integral())
hSTsch_bar_2 = fSTsch_bar.Get("/MET/Ht_STsch_bar")
hSTsch_bar_2.Add(hSTsch_2)
hSTsch_bar_2.Sumw2()
hSTsch_bar_2.Scale(1/hSTsch_bar_2.Integral())
hSTsch_bar_3 = fSTsch_bar.Get("/MET/Stlep_STsch_bar")
hSTsch_bar_3.Add(hSTsch_3)
hSTsch_bar_3.Sumw2()
hSTsch_bar_3.Scale(1/hSTsch_bar_3.Integral())
hSTsch_bar_4 = fSTsch_bar.Get("/jets/Njets_cut1_STsch_bar")
hSTsch_bar_4.Add(hSTsch_4)
hSTsch_bar_4.Sumw2()
hSTsch_bar_4.Scale(1/hSTsch_bar_4.Integral())
hSTsch_bar_5 = fSTsch_bar.Get("/jets/Nbjets_CSVL_STsch_bar")
hSTsch_bar_5.Add(hSTsch_5)
hSTsch_bar_5.Sumw2()
hSTsch_bar_5.Scale(1/hSTsch_bar_5.Integral())

fSTtWch = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_STtWch.root")
hSTtWch_1 = fSTtWch.Get("/MET/Stjet_STtWch")
hSTtWch_2 = fSTtWch.Get("/MET/Ht_STtWch")
hSTtWch_3 = fSTtWch.Get("/MET/Stlep_STtWch")
hSTtWch_4 = fSTtWch.Get("/jets/Njets_cut1_STtWch")
hSTtWch_5 = fSTtWch.Get("/jets/Nbjets_CSVL_STtWch")

fSTtWch_bar = TFile("/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results09/results_STtWch_bar.root")
hSTtWch_bar_1 = fSTtWch_bar.Get("/MET/Stjet_STtWch_bar")
hSTtWch_bar_1.Add(hSTtWch_1)
hSTtWch_bar_1.Sumw2()
hSTtWch_bar_1.Scale(1/hSTtWch_bar_1.Integral())
hSTtWch_bar_2 = fSTtWch_bar.Get("/MET/Ht_STtWch_bar")
hSTtWch_bar_2.Add(hSTtWch_2)
hSTtWch_bar_2.Sumw2()
hSTtWch_bar_2.Scale(1/hSTtWch_bar_2.Integral())
hSTtWch_bar_3 = fSTtWch_bar.Get("/MET/Stlep_STtWch_bar")
hSTtWch_bar_3.Add(hSTtWch_3)
hSTtWch_bar_3.Sumw2()
hSTtWch_bar_3.Scale(1/hSTtWch_bar_3.Integral())
hSTtWch_bar_4 = fSTtWch_bar.Get("/jets/Njets_cut1_STtWch_bar")
hSTtWch_bar_4.Add(hSTtWch_4)
hSTtWch_bar_4.Sumw2()
hSTtWch_bar_4.Scale(1/hSTtWch_bar_4.Integral())
hSTtWch_bar_5 = fSTtWch_bar.Get("/jets/Nbjets_CSVL_STtWch_bar")
hSTtWch_bar_5.Add(hSTtWch_5)
hSTtWch_bar_5.Sumw2()
hSTtWch_bar_5.Scale(1/hSTtWch_bar_5.Integral())

						
can1 = TCanvas('can1', '', 800,600)
can1.cd()
can1.SetLogy()

httbar_1.SetLineColor(style.TtbarColor)
httbar_1.SetMaximum(1)
httbar_1.SetYTitle('1/N dN/dx')
httbar_1.Draw("Hist")

hSTsch_bar_1.SetLineColor(style.ST_t_sColor)
hSTsch_bar_1.SetLineWidth(1)
hSTsch_bar_1.Draw("Hist same")

hSTtWch_bar_1.SetLineColor(style.ST_tWColor)
hSTtWch_bar_1.SetLineWidth(2)
hSTtWch_bar_1.Draw("Hist same")

leg = legend()
can1.SaveAs("diff_Stjet_Top.png")

can2 = TCanvas('can2', '', 800,600)
can2.cd()
can2.SetLogy()

httbar_2.SetLineColor(style.TtbarColor)
httbar_2.SetMaximum(1)
httbar_2.SetYTitle('1/N dN/dx')
httbar_2.Draw("Hist")

hSTsch_bar_2.SetLineColor(style.ST_t_sColor)
hSTsch_bar_2.SetLineWidth(1)
hSTsch_bar_2.Draw("Hist same")

hSTtWch_bar_2.SetLineColor(style.ST_tWColor)
hSTtWch_bar_2.SetLineWidth(2)
hSTtWch_bar_2.Draw("Hist same")

leg = legend()
can2.SaveAs("diff_Ht_Top.png")

can3 = TCanvas('can3', '', 800,600)
can3.cd()
can3.SetLogy()

httbar_3.SetLineColor(style.TtbarColor)
httbar_3.SetMaximum(1)
httbar_3.SetYTitle('1/N dN/dx')
httbar_3.Draw("Hist")

hSTsch_bar_3.SetLineColor(style.ST_t_sColor)
hSTsch_bar_3.SetLineWidth(1)
hSTsch_bar_3.Draw("Hist same")

hSTtWch_bar_3.SetLineColor(style.ST_tWColor)
hSTtWch_bar_3.SetLineWidth(2)
hSTtWch_bar_3.Draw("Hist same")

leg = legend()
can3.SaveAs("diff_Stlep_Top.png")

can4 = TCanvas('can4', '', 800,600)
can4.cd()
can4.SetLogy()

httbar_4.SetLineColor(style.TtbarColor)
httbar_4.SetMaximum(1)
httbar_4.SetYTitle('1/N dN/dx')
httbar_4.Draw("Hist")

hSTsch_bar_4.SetLineColor(style.ST_t_sColor)
hSTsch_bar_4.SetLineWidth(1)
hSTsch_bar_4.Draw("Hist same")

hSTtWch_bar_4.SetLineColor(style.ST_tWColor)
hSTtWch_bar_4.SetLineWidth(2)
hSTtWch_bar_4.Draw("Hist same")
leg = legend()
can4.SaveAs("diff_Njets_Top.png")

can5 = TCanvas('can5', '', 800,600)
can5.cd()
can5.SetLogy()

httbar_5.SetLineColor(style.TtbarColor)
httbar_5.SetMaximum(1)
httbar_5.SetYTitle('1/N dN/dx')
httbar_5.Draw("Hist")

hSTsch_bar_5.SetLineColor(style.ST_t_sColor)
hSTsch_bar_5.SetLineWidth(1)
hSTsch_bar_5.Draw("Hist same")

hSTtWch_bar_5.SetLineColor(style.ST_tWColor)
hSTtWch_bar_5.SetLineWidth(2)
hSTtWch_bar_5.Draw("Hist same")

leg = legend()
can5.SaveAs("diff_Nbjets_CSVL_Top.png")
