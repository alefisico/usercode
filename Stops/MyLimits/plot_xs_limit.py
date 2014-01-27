#!/usr/bin/env python

import sys, os, subprocess, string, re
from ROOT import *
from array import array


gROOT.SetBatch(kTRUE);
gStyle.SetOptStat(0)
gStyle.SetOptTitle(0)
gStyle.SetTitleFont(42, "XYZ")
gStyle.SetTitleSize(0.06, "XYZ")
gStyle.SetLabelFont(42, "XYZ")
gStyle.SetLabelSize(0.05, "XYZ")
gStyle.SetCanvasBorderMode(0)
gStyle.SetFrameBorderMode(0)
gStyle.SetCanvasColor(kWhite)
gStyle.SetPadTickX(1)
gStyle.SetPadTickY(1)
gStyle.SetPadLeftMargin(0.15)
gStyle.SetPadRightMargin(0.05)
gStyle.SetPadTopMargin(0.05)
gStyle.SetPadBottomMargin(0.15)
gROOT.ForceStyle()


########################################################
## Comment out this part if running the limit code

masses = array('d', [450.0, 550.0, 650.0, 750.0])
xs_obs_limits = array('d', [0.789720, 0.592578, 0.354166, 0.280477])
xs_exp_limits = array('d', [0.644212, 0.709180, 0.504182, 0.200066])

masses_exp = array('d', [450.0, 550.0, 650.0, 750.0, 750.0, 650.0, 550.0, 450.0])
xs_exp_limits_1sigma = array('d', [0.462973, 0.509556, 0.362877, 0.141824, 0.280611, 0.703142, 0.994688, 0.908691])
xs_exp_limits_2sigma = array('d', [0.340982, 0.383678, 0.270801, 0.107457, 0.376634, 0.946306, 1.325989, 1.216384])

xs_th = array('d', [0.169668, 0.0452067, 0.0139566, 0.00480639] )

##
########################################################
graph_exp_2sigma = TGraph(len(masses_exp),masses_exp,xs_exp_limits_2sigma)
graph_exp_2sigma.SetFillColor(kYellow)
graph_exp_2sigma.GetXaxis().SetTitle("Quadruplet Invariant Mass [GeV]")
graph_exp_2sigma.GetYaxis().SetTitle("95% CL Limit #sigma#timesBR(X#rightarrowjjjj) [pb]")
graph_exp_2sigma.GetYaxis().SetRangeUser(1e-03,10)
graph_exp_2sigma.GetXaxis().SetNdivisions(1005)

graph_exp_1sigma = TGraph(len(masses_exp),masses_exp,xs_exp_limits_1sigma)
graph_exp_1sigma.SetFillColor(kGreen+1)

graph_exp = TGraph(len(masses),masses,xs_exp_limits)
#graph_exp.SetMarkerStyle(24)
graph_exp.SetLineWidth(2)
graph_exp.SetLineStyle(2)
graph_exp.SetLineColor(4)

graph_obs = TGraph(len(masses),masses,xs_obs_limits)
graph_obs.SetMarkerStyle(20)
graph_obs.SetLineWidth(2)
#graph_obs.SetLineStyle(1)
graph_obs.SetLineColor(1)

graph_th = TGraph(len(masses),masses,xs_th)
#graph_th.SetMarkerStyle(24)
graph_th.SetLineWidth(2)
#graph_th.SetLineStyle(2)
graph_th.SetLineColor(kRed)


c = TCanvas("c", "",800,800)
c.cd()

graph_exp_2sigma.Draw("AF")
graph_exp_1sigma.Draw("F")
graph_exp.Draw("L")
graph_obs.Draw("LP")
graph_th.Draw("L")

legend = TLegend(.18,.20,.80,.43)
legend.SetBorderSize(0)
legend.SetFillColor(0)
legend.SetFillStyle(0)
legend.SetTextFont(42)
legend.SetTextSize(0.03)
#legend.SetHeader('95% CL Upper Limits (stat. only)')
legend.AddEntry(graph_obs,"Observed","lp")
legend.AddEntry(graph_exp,"Expected","lp")
legend.AddEntry(graph_exp_1sigma,"Expected #pm 1#sigma","f")
legend.AddEntry(graph_exp_2sigma,"Expected #pm 2#sigma","f")
legend.AddEntry(graph_th,"Theory","lp")
legend.Draw()

l1 = TLatex()
l1.SetTextAlign(12)
l1.SetTextFont(42)
l1.SetNDC()
l1.SetTextSize(0.04)
l1.DrawLatex(0.30,0.97, "CMS Preliminary 19.5 fb^{-1} at #sqrt{s} = 8 TeV")
l1.SetTextSize(0.04)
#l1.DrawLatex(0.18,0.43, "CMS Preliminary")
#l1.DrawLatex(0.18,0.35, "#intLdt = 5 fb^{-1}")
#l1.DrawLatex(0.19,0.30, "#sqrt{s} = 7 TeV")
#l1.DrawLatex(0.18,0.25, "|#eta| < 2.5, |#Delta#eta| < 1.3")
#l1.DrawLatex(0.18,0.20, "Wide Jets")
#l1.SetTextSize(0.035)
#l1.DrawLatex(0.70,0.52, "f_{b#bar{b}} = #frac{BR(X#rightarrowb#bar{b})}{BR(X#rightarrowjj)}")
#l1.SetTextSize(0.055)
#l1.DrawLatex(0.50,0.80, "0, 1 and 2 b-tags")

gPad.RedrawAxis();

c.SetLogy()
c.SaveAs('limit_obsexp.pdf')

