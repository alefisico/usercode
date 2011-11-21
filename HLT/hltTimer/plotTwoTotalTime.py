#!/usr/bin/env python
###################################################
# This python script takes totalTime plots of two root files
# and compare them in the same canvas with different axes scale
# and plots both stats boxes.
####################################################

import os,sys
from ROOT import *

gROOT.SetStyle("Plain")
DEBUG = False

###############################################################
def usage():
###############################################################

    if (DEBUG):
        print "This is the usage function"
        
    print '\n'
    print 'Usage: '+sys.argv[0]+' <Zero Bias HPU file> <Reference file>'
    print 'e.g.:  '+sys.argv[0]+' outputTiming1.root outputTiming2.root\n'


####################################
def plotTwoHists(infile1, infile2):
#####################################
	# Open files
	f1 = TFile(infile1, "READ")
	h1 = f1.Get("totalTime")
	f2 = TFile(infile2, "READ")
	h2 = f2.Get("totalTime")

	can = TCanvas('can', '', 800,600)
	can.cd()
	h1.UseCurrentStyle()
	h1.Draw()
	h1.SetLineWidth(2)
	h1.SetLineColor(kRed)
	h1.GetXaxis().SetLabelSize(0.03)
	h1.GetYaxis().SetLabelColor(kRed)
	h1.GetYaxis().SetAxisColor(kRed)
	gStyle.SetStatX(0.8)
	gStyle.SetStatY(0.8)
	gPad.Update()
	statsbox = gPad.GetPrimitive("stats")
	statsbox.SetTextColor(kRed)
	statsbox.SetLineColor(kRed)

	y1 = statsbox.GetY1NDC()
	y2 = statsbox.GetY2NDC()
	newy1 = 2* y1 - y2
	newy2 = y1
	statsbox.SetY1NDC(newy1)
	statsbox.SetY2NDC(newy2)

	rightmax = 1.1*h1.GetMaximum()
	scale = gPad.GetUymax()/rightmax
	h2.SetLineWidth(2)
	h2.Draw("sames")
	axis = TGaxis(gPad.GetUxmax(), gPad.GetUymin(), gPad.GetUxmax(), gPad.GetUymax(), 0, rightmax, 510, "+L")
	axis.Draw()
	lg = TLegend(0.60, 0.80, 0.80, 0.90);
	lg.AddEntry(h2,"Reference","L");
	lg.AddEntry(h1,"Zero Bias HPU","L");
	lg.SetTextSize(0.025);
	lg.SetBorderSize(kBlack);
	lg.SetFillColor(0);
	lg.Draw("same");
	can.SetBorderMode(0)
	can.SetFillColor(kWhite)
	can.SaveAs('totalTime_ZBvsReference.png')
	del can





#################################
def main():
################################
	#check the number of parameter
	numarg = len(sys.argv)
	if numarg < 2:
        	usage()
	        return 1

	print "\nPython script for drawing two Total Time Histograms"
	print "For more info, please contact: Alejandro Gomez"
	print "email: alejandro.gomez@cern.ch\n"

	infile1 = sys.argv[1]
	print infile1+ " is the ZB Skim file "
	infile2 = sys.argv[2]
	print  infile2+ " is the Reference Skim file\n"

	#check if input files exist
	if  not(os.path.isfile(infile1)):
		print infile1+" does not exist. Please check."
		sys.exit(1)
	if  not(os.path.isfile(infile2)):
		print infile2+" does not exist. Please check."
		sys.exit(1)

	#print "This process takes a while.... please be patient\n"
	plotTwoHists(infile1, infile2)


#######################################################
if __name__ =='__main__':
#######################################################
	sys.exit(main())
