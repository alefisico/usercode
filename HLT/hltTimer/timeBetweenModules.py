import os,sys
import operator
#import subprocess
#import string, re
#import fileinput
#import commands
#from time import gmtime, localtime, strftime
from ROOT import *

gROOT.SetStyle("Plain")
gStyle.SetOptStat(0)


###############################################################
def main():
###############################################################
	repo = {}
	repo1 = {}
	infile = open('test10.log','r')
	lines = infile.readlines()[2:-1]
	for i in lines:
		module, time = [a.strip() for a in i.split(',')]
		if float(time) > 0.0001:
			repo.setdefault(module, []).append(float(time))
	for modules, values in repo.iteritems():
		repo1[modules] = '%.3f' % (sum(values)*1000 / float(len(values)))	
	repo2 = dict(sorted(repo1.iteritems(), key=operator.itemgetter(1),reverse=True)[1:30])
	print repo2

	c1 = TCanvas('c1', '', 1600,1000)
       	h1 = TH1F("h1","Modules", len(repo2),0,len(repo2))
	for mod, mean in repo2.iteritems():
		h1.Fill(mod , float(mean))	

	h1.Draw()
	h1.SetLineWidth(2)
	h1.GetYaxis().SetTitle("msec")
	h1.GetXaxis().SetLabelSize(0.03)
        c1.SetGridx()
        c1.SetGridy()
	c1.SetBottomMargin(0.2)
	c1.SaveAs("test_v10_2.png")

#######################################################
if __name__ =='__main__':
#######################################################
        sys.exit(main())

