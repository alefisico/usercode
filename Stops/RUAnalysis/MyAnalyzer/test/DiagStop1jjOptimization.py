#!/usr/bin/env python

from ROOT import *
import glob,sys
from array import array

gROOT.Reset()
gStyle.SetOptStat(0)

outputDir = "/uscms/home/algomez/nobackup/files/Stops/Results/Plots/"

########### 300 - 900
#MPV = [ 4.66805e+02,
#		4.66603e+02,
#		4.67459e+02,
#		4.67945e+02,
#		4.69907e+02,
#		4.71789e+02,
#		4.73090e+02,
#		4.73839e+02,
#		4.79119e+02,
#		4.85310e+02,
#		4.99696e+02
#		]

############ 350 - 700
MPV = [ 4.70966e+02,
		4.70745e+02,
		4.71820e+02,
		4.73778e+02,
		4.74146e+02,
		4.74245e+02,
		4.74744e+02,
		4.76909e+02,
		4.83852e+02,
		4.88955e+02,
		5.02589e+02
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
gr.GetXaxis().SetTitle( 'Diagonal Offset for Stop1 [GeV]' )
gr.GetYaxis().SetTitle( 'Landau MPV [GeV]' )
gr.Draw( 'ACP' )

textBox=TLatex()
textBox.SetNDC()
textBox.SetTextSize(0.05) 
textBox.SetTextColor(50)
textBox.DrawLatex(0.10,0.91,"CMS Preliminary 19.5 fb^{-1} at #sqrt{s} = 8 TeV")
	
c.SaveAs(outputDir+"DiagStop1jjOptimization.pdf")
