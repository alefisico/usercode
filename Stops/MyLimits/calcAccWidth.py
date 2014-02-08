#!/usr/bin/env python

'''
File: calcAccWidth.py
Author: Alejandro Gomez Espinosa
Email: gomez@physics.rutgers.edu
Description: Plot Signal Width, Mean and Acceptance from text file
'''

from ROOT import *
import glob,sys
from array import array

gROOT.Reset()
gStyle.SetOptStat(0)
gStyle.SetOptFit()
gStyle.SetStatY(0.9)
gStyle.SetStatX(0.4)
gStyle.SetStatW(0.12)
gStyle.SetStatH(0.12)

##################################################
#### Main part of the program                 ####
##################################################
def readInfo( infofile, dir ):
	"""docstring for readInfo"""

	listNomMass = []
	listNomAcceptance = []
	listNomWidth = []
	listJesUpAcceptance = []
	listJesUpWidth = []
	listJesDownAcceptance = []
	listJesDownWidth = []

	for line in open( dir+infofile ):
		columns = line.split()
		li=line.strip()
		if not li.startswith("#"):
			if 'nom' in columns[7]:
				listNomMass.append( float(columns[0]))
			       	listNomAcceptance.append( float(columns[2]))
       				listNomWidth.append( float(columns[3]))
			elif 'up' in columns[7]:
			       	listJesUpAcceptance.append( float(columns[2]))
       				listJesUpWidth.append( float(columns[3]))
			elif 'down' in columns[7]:
			       	listJesDownAcceptance.append( float(columns[2]))
       				listJesDownWidth.append( float(columns[3]))
			else: 
				pass

	print listNomMass, listNomAcceptance, listJesUpAcceptance, listJesDownAcceptance
	
	Mass = array('d', listNomMass)
	MassErr = array('d', [ 0, 0, 0, 0])	#### just for plot
	AcceptanceNom = array('d', listNomAcceptance)
	WidthNom = array('d', listNomWidth)
	AcceptanceJesUp = array('d', listJesUpAcceptance)
	WidthJesUp = array('d', listJesUpWidth)
	AcceptanceJesDown = array('d', listJesDownAcceptance)
	WidthJesDown = array('d', listJesDownWidth)

	AccNom = TGraph(len(Mass), Mass, AcceptanceNom)
	AccNom.Fit("pol3")
	AccNom.GetFunction("pol3").SetLineColor(1)
	AccNom.SetLineColor(1)
	AccNom.SetMarkerStyle(2)

	WidNom = TGraph(len(Mass), Mass, WidthNom)
	WidNom.Fit("pol3")
	WidNom.GetFunction("pol3").SetLineColor(1)
	WidNom.SetLineColor(1)
	WidNom.SetMarkerStyle(2)

	maxJesDiffAcc = calcMaxUncer( AcceptanceNom, AcceptanceJesUp, AcceptanceJesDown )
	maxJesDiffWidth = calcMaxUncer( WidthNom, WidthJesUp, WidthJesDown )

	Acceptance = TGraphErrors( len(Mass), Mass, AcceptanceNom, MassErr, maxJesDiffAcc )
	c = TCanvas('c1', 'c1',  10, 10, 750, 500 )
	Acceptance.SetFillColor(3)
	Acceptance.SetFillStyle(3001)
	Acceptance.Draw("a4")
	AccNom.Draw("psame")

	legend=TLegend(0.70,0.3,0.90,0.45)
	legend.SetFillColor(0);
	legend.AddEntry(Acceptance,"Acc x Eff", "l")
	legend.AddEntry(AccNom,"JES Uncertainty", "l")
	legend.Draw()

	c.SaveAs('test.pdf')
	del c

	comparativeGraphs( dir, Mass, AccNom, WidNom, AcceptanceJesUp, WidthJesUp, AcceptanceJesDown, WidthJesDown)
	saveInfo( dir, Mass, maxJesDiffAcc, maxJesDiffWidth )

##############################################################
#### Save info in a file                                  ####
##############################################################
def saveInfo( dir, Mass, Acceptance, Width):
	
	try:
		listName = [ '#Mass', 'JESAcceptance', 'JESWidth']

		myfile = open( dir+"signalJESInfo.txt", "w" ) 
		myfile.write("\t".join(listName)+ '\n')
		for i in range(4):
			list = [ str(Mass[i]), str(Acceptance[i]), str(Width[i]) ]
			myfile.write("\t".join(list)+ '\n')
			
	except IOError:
		print 'File does not exist.'

##############################################################
#### Calculate uncertainty                                ####
##############################################################
def calcMaxUncer( Nominal, Up, Down):
	"""docstring for calcMaxUncer"""
	Diff = []
	for i in range(len(Nominal)):
		diff1 = abs(Nominal[i] - Up[i])
		diff2 = abs(Nominal[i] - Down[i])
		if diff1 > diff2:
			Diff.append( Up[i] )
		else:
			Diff.append( Down[i] )
		#print 'yes', AcceptanceNom[i], AcceptanceJesUp[i], AcceptanceJesDown[i], diff1, diff2
	maxDiff = array('d', Diff)
	return maxDiff



##############################################################
#### Plot comparative graphs                              ####
##############################################################
def comparativeGraphs(dir, Mass, AccNom, WidNom, AcceptanceJesUp, WidthJesUp, AcceptanceJesDown, WidthJesDown):
	"""docstring for comparativeGraphs"""

	AccJesUp = TGraph(len(Mass), Mass, AcceptanceJesUp)
	AccJesUp.Fit("pol3")
	AccJesUp.GetFunction("pol3").SetLineColor(2)
	AccJesUp.SetLineColor(2)

	WidJesUp = TGraph(len(Mass), Mass, WidthJesUp)
	WidJesUp.Fit("pol3")
	WidJesUp.GetFunction("pol3").SetLineColor(2)
	WidJesUp.SetLineColor(2)

	AccJesDown = TGraph(len(Mass), Mass, AcceptanceJesDown)
	AccJesDown.Fit("pol3")
	AccJesDown.GetFunction("pol3").SetLineColor(4)
	AccJesDown.SetLineColor(4)

	WidJesDown = TGraph(len(Mass), Mass, WidthJesDown)
	WidJesDown.Fit("pol3")
	WidJesDown.GetFunction("pol3").SetLineColor(4)
	WidJesDown.SetLineColor(4)

	plotCompJes( dir, AccNom, AccJesUp, AccJesDown, 'Acceptance' )
	plotCompJes( dir, WidNom, WidJesUp, WidJesDown, 'Gaussian Width' )

##############################################################
#### Canvas for comparative plots                         ####
##############################################################
def plotCompJes( dir, Nominal, Up, Down, name):

	Up.SetTitle( name )
	Up.GetXaxis().SetTitle("Stop2 Mass [GeV]")
	Up.GetYaxis().SetTitle( name )

	c = TCanvas('c1', 'c1',  10, 10, 750, 500 )
	Up.Draw("ap")
	Down.Draw("psame")
	Nominal.Draw("psame")

	legend=TLegend(0.70,0.3,0.90,0.45)
	legend.SetFillColor(0);
	legend.AddEntry(Nominal,"Nominal", "l")
	legend.AddEntry(Up,"JES Up", "l")
	legend.AddEntry(Down,"JES Down", "l")
	legend.Draw()

	c.SaveAs(dir+"comp"+name+".pdf")
	del c


if __name__ == '__main__':

	Dir = '/uscms_data/d3/algomez/files/Stops/Limits/'
	inputfile = 'signalInfo.txt'

	readInfo( inputfile, Dir )
