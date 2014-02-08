#!/usr/bin/env python
'''
File: plot_xs_limit.py
Author: Alejandro Gomez Espinosa
Email: gomez@physics.rutgers.edu
Description: Draw Limit Plots from Combination Tool 
'''

import sys, os
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

###############################################################
### Draw Limits                                            #####
###############################################################
def drawLimits( dir, st2mass, XS ):
	"""docstring for drawLimits"""

	listMass = []
	listObs = []
	listExp = []
	listExpLower2Sigma = []
	listExpLower1Sigma = []
	listExpUpper1Sigma = []
	listExpUpper2Sigma = []
	k = 0 	### dummy counter
	for mass in st2mass:
		file = dir+'higgsCombineTest.Asymptotic.mH'+str(mass)+'.root'
		signalStrength = readInfoFromFile( file )	#### signal Strengh is number of signal events / number of expected signal events ( effectively you have to multiply by the XS )
		listObs.append( signalStrength[5] * XS[k] )
		listExp.append( signalStrength[2] * XS[k] )
		listExpLower2Sigma.append( signalStrength[0] * XS[k] )
		listExpLower1Sigma.append( signalStrength[1] * XS[k] )
		listExpUpper1Sigma.append( signalStrength[3] * XS[k] )
		listExpUpper2Sigma.append( signalStrength[4] * XS[k] )
		k+=1	### dummy counter

	#print listObs, listExp, listExpLower2Sigma, listExpLower1Sigma, listExpUpper1Sigma, listExpUpper2Sigma
	masses = array('d', st2mass )
	xs_obs_limits = array('d', listObs )
	xs_exp_limits = array('d', listExp )
	
	reverseSt2mass = st2mass[::-1]
	reverselistExpLower1Sigma = listExpLower1Sigma[::-1]
	reverselistExpLower2Sigma = listExpLower2Sigma[::-1]

	masses_exp = array('d', st2mass + reverseSt2mass )
	xs_exp_limits_1sigma = array('d', listExpUpper1Sigma + reverselistExpLower1Sigma )
	xs_exp_limits_2sigma = array('d', listExpUpper2Sigma + reverselistExpLower2Sigma )
	
	xs_th = array('d', XS )

	#### Graphs
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
	
	gPad.RedrawAxis()
	
	c.SetLogy()
	c.SaveAs(dir+'limit_obsexp.pdf')

###############################################################
### read Info from limit file                             #####
###############################################################
def readInfoFromFile( file ):
	"""docstring for readInfoFromFile"""
	inputFile = TFile.Open( file )
	tree = inputFile.Get( "limit" )
	numEntries = tree.GetEntriesFast()

	list = []
	for i in xrange( numEntries ):
		tree.GetEntry(i)
		list.append( tree.limit )
	return list

################################################################################
if __name__ == "__main__":

	##### Initialize some arguments
	dir = '/uscms_data/d3/algomez/files/Stops/Limits/'
	st2mass = [ 450, 550, 650, 750 ]
	XS =  [0.169668, 0.0452067, 0.0139566, 0.00480639]

	drawLimits( dir, st2mass, XS )
