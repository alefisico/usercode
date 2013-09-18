#!/usr/bin/env python

from ROOT import *
import glob,sys
from array import array

gROOT.Reset()
gStyle.SetOptStat(0)

outputDir = "/uscms/home/algomez/nobackup/files/Stops/Results/Plots/"

########## 350 - 900
#MPV = [ 3.83752e+02,
#		3.82593e+02,
#		3.79254e+02,
#		3.77822e+02,
#		3.74519e+02,
#		3.77419e+02,
#		3.78703e+02,
#		3.76845e+02,
#		3.80290e+02,
#		3.77176e+02,
#		3.81958e+02
#		]

MPV = [3.79999e+02,
		3.77332e+02,
		3.74138e+02,
		3.74375e+02,
		3.72698e+02,
		3.75866e+02,
		3.74515e+02,
		3.72191e+02,
		3.74843e+02,
		3.68148e+02,
		3.72417e+02
		]

x, y = array( 'd' ), array( 'd' )

c = TCanvas('c', 'c',  10, 10, 750, 500 )
for i in range( len(MPV) ):
	x.append( 10*i )
	y.append( MPV[i] )
	#print ' i %i %f %f ' % (i,x[i],y[i])

gr = TGraph( len(MPV), x, y )
gr.SetTitle("")
gr.SetLineColor( 2 )
gr.SetLineWidth( 2 )
gr.SetMarkerColor( 4 )
gr.SetMarkerStyle( 21 )
gr.GetXaxis().SetTitle( 'Diagonal Offset for Stop2 [GeV]' )
gr.GetYaxis().SetTitle( 'Landau MPV [GeV]' )
gr.Draw( 'ACP' )

textBox=TLatex()
textBox.SetNDC()
textBox.SetTextSize(0.05) 
textBox.SetTextColor(50)
textBox.DrawLatex(0.10,0.91,"CMS Preliminary 19.5 fb^{-1} at #sqrt{s} = 8 TeV")
	
c.SaveAs(outputDir+"DiagStop2jjOptimization.pdf")
