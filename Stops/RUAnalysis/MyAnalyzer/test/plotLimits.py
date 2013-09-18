#!/usr/bin/env python
from ROOT import *
import sys
import os
import sys
from array import array

gROOT.Reset()
gStyle.SetOptStat(0)

#calculatemethod = sys.argv[2] #"bayesian" #"cls" #"bayesian"
ExcludePoints = [] #2300, 2500]

exp_filename = {}
obs_filename = {}
colors = {}

jj = 'jj'
inputTextFile = '/uscms/home/algomez/nobackup/files/Stops/Limits/'

#if (version1 == 'BDT') and version:
exp_filename['jj'] = inputTextFile+"bayesian_limits_expected.txt"
obs_filename['jj'] = inputTextFile+"bayesian_limits_observed.txt"
colors['jj'] = 1

#elif (version2 == 'combined'):
#	exp_filename['BDT'] = "bayesian_limits_expected_BDT"+version+".txt"
#	obs_filename['BDT'] = "bayesian_limits_observed_BDT"+version+".txt"
#	colors['BDT'] = 1
#	exp_filename['Stjet'] = "bayesian_limits_expected_stjet_"+version1+".txt"
#	obs_filename['Stjet'] = "bayesian_limits_observed_stjet_"+version1+".txt"
#	colors['Stjet'] = ROOT.kRed
#
#if calculatemethod == "cls":
#	if (version1 == 'BDT') and version:
#		exp_filename['BDT'] = "cls_limits_expected_BDT"+version+".txt"
#		obs_filename['BDT'] = "cls_limits_observed_BDT"+version+".txt"
#		colors['BDT'] = 1


kFactor = 1
xsec = {}
# right-handed
xsec[450] = 0.169668*kFactor;
xsec[550] = 0.0452067*kFactor;
xsec[650] = 0.0139566*kFactor;
xsec[750] = 0.00480639*kFactor;

# read input text files

g_exp = {}
g_sigma1 = {}
g_sigma2 = {}
g_obs = {}


# theory
#xlist = [500, 700 ,900, 1000, 1100]
#ylist = [0.00025442, 0.000099366, 0.000022059, 0.0000098699, 0.0000043429]
xlist = xsec.keys()
xlist.sort()
ylist = []
for ii in xlist:
    ylist.append(xsec[ii])
    
x_theory = array('d')
y_theory = array('d')
x_theory.fromlist( xlist)
y_theory.fromlist( ylist)

g_theory = TGraph( len(x_theory), x_theory, y_theory)
g_theory.SetLineColor(2)
g_theory.SetLineWidth(2)


filekeys = exp_filename.keys()

for test in filekeys:
    
    # read expected
    exp_file = open(exp_filename[test])

    expected = {}
    expected_1sig_up = {}
    expected_1sig_low= {}
    expected_2sig_up = {}
    expected_2sig_low= {}

    for line in exp_file:
        if line.startswith("#"): continue
        list = line.split()
        point = int(list[0])
        skip = False
        for i in ExcludePoints:
            if point == i: skip = True
        if not skip:
            expected[point] = float(list[1])*xsec[point]
            expected_1sig_low[point] = float(list[4])*xsec[point]
            expected_1sig_up[point] = float(list[5])*xsec[point]
            expected_2sig_low[point] = float(list[2])*xsec[point]
            expected_2sig_up[point] = float(list[3])*xsec[point]

    exp_file.close()
#print expected_1sig_low
    print expected

    observed = {}

    obs_file = open(obs_filename[test])
    for line in obs_file:
	    if line.startswith("#"): continue
	    list = line.split()
	    point = int(list[0])
	    skip = False
	    for i in ExcludePoints:
		    if point == i: skip = True
	    if not skip:
		    observed[point] = float(list[1])*xsec[point]
    obs_file.close()
    print observed

    # create arrays
    x_exp = array('d')
    x_exp_onesigma = array('d')
    y_exp = array('d')
    y_exp_onesigma = array('d')
    y_exp_twosigma = array('d')

    thekeys = expected.keys()
    thekeys.sort()
    tmplist = []
    tmplist1 = []
    tmplist2 = []
    
    for i in thekeys:
        tmplist.append(expected[i])
        tmplist1.append(expected_1sig_low[i])
        tmplist2.append(expected_2sig_low[i])
    thekeys2 = expected.keys() 
    thekeys2.sort()
    thekeys2.reverse()
    for i in thekeys2:
        tmplist1.append(expected_1sig_up[i])
        tmplist2.append(expected_2sig_up[i])


    x_exp_onesigma.fromlist( thekeys + thekeys2 )
    x_exp.fromlist(thekeys)
    y_exp.fromlist( tmplist )
    y_exp_onesigma.fromlist(tmplist1)
    y_exp_twosigma.fromlist(tmplist2)

    print x_exp
    x_obs = array('d')
    y_obs =array('d')

    thekeys = observed.keys()
    thekeys.sort()
    tmplist = []
    for i in thekeys:
        tmplist.append(observed[i])

    x_obs.fromlist( thekeys )
    y_obs.fromlist( tmplist )


    # create graphs
    g_exp[test] = TGraph( len(x_exp), x_exp, y_exp)
    g_exp[test].SetLineStyle(kDashed)
    g_exp[test].SetLineColor( colors[test] )
    g_exp[test].SetLineWidth(2)
  
    g_sigma1[test] = TGraph( len(x_exp_onesigma), x_exp_onesigma, y_exp_onesigma)
    g_sigma1[test].SetFillColor(ROOT.kGreen)
  
    g_sigma2[test] = TGraph( len(x_exp_onesigma), x_exp_onesigma, y_exp_twosigma)
    g_sigma2[test].SetFillColor(ROOT.kYellow)

    g_obs[test] = TGraph( len(x_obs), x_obs, y_obs)
    g_obs[test].SetMarkerStyle(20)
    g_obs[test].SetMarkerColor( colors[test] )
    g_obs[test].SetLineColor( colors[test] )

# create frame
frame = TH2F("frame", "frame", 10, 400, x_exp[len(x_exp)-1]+50, 100, 0.000001, 1.0)
frame.SetTitle("")
frame.GetXaxis().SetTitle("Mass [GeV]")
#frame.GetYaxis().SetTitle("#sigma#upointBR(pp#rightarrow G_{H}G_{H} #rightarrow t#bar{t}t#bar{t}) [pb]")
frame.GetYaxis().SetTitle("95% Bayesian CL #sigma #times BR(pp#rightarrow #tilde{t}#tilde{t}) [pb]")
frame.GetYaxis().SetTitleOffset(1.3)
frame.SetNdivisions(505)
      
# plot
cv = TCanvas("cv","cv",750,500)
frame.Draw()


#if (version1 =='BDT' or version1=='Stjet' or version1=='Likelihood'):
#	if version1=='BDT':
g_sigma2[jj].Draw("F")
g_sigma1[jj].Draw("F")
#	elif version1=='Likelihood':
#		g_sigma2[Likelihood].Draw("F")
#		g_sigma1[Likelihood].Draw("F")
#	else:
#		g_sigma2[Stjet].Draw("F")
#		g_sigma1[Stjet].Draw("F")

# legend
aleg = TLegend(0.65,0.65,0.99,0.90)
aleg.SetMargin(0.12)
aleg.SetTextSize(0.025)
aleg.SetFillColor(10)
aleg.SetBorderSize(0)
aleg.SetLineStyle(0)
aleg.SetFillStyle(0)

    
for test in filekeys:
    
	g_exp[test].Draw("c")
#    if version1=='combined':
#	    g_exp[BDT].SetLineStyle(1)
#	    g_exp[Stjet].SetLineStyle(2)
	aleg.AddEntry( g_exp[test], "Expected Limit","L")

#if (version1=='BDT' or version1=='Stjet' or version1=='Likelihood'):
        g_obs[test].Draw("Pl")
        aleg.AddEntry( g_obs[test], "Observed Limit","P")
#	if version1=='BDT':
	aleg.AddEntry( g_sigma1[jj], "#pm1#sigma Expected","F")
	aleg.AddEntry( g_sigma2[jj], "#pm2#sigma Expected","F")
#	elif version1=='Likelihood':
#		aleg.AddEntry( g_sigma1[Likelihood], "#pm1#sigma Expected","F")
#		aleg.AddEntry( g_sigma2[Likelihood], "#pm2#sigma Expected","F")
#	else:
#		aleg.AddEntry( g_sigma1[Stjet], "#pm1#sigma Expected","F")
#		aleg.AddEntry( g_sigma2[Stjet], "#pm2#sigma Expected","F")

aleg.AddEntry( g_theory, "Theory Stop2 NLO and NLL","L")
aleg.Draw()

g_theory.Draw("c")
   
#sbanner = "#splitline{CMS Preliminary}{19.5 fb^{-1} at #sqrt{s}=8TeV}"
sbanner = "CMS Preliminary, 19.5 fb^{-1} at #sqrt{s}=8TeV"
#if sample == "combined": sbanner = "#splitline{CMS Preliminary}{4.7 fb^{-1} at #sqrt{s}=7TeV #mu+jets,e+jets}"
#if sample == "e": sbanner = "#splitline{CMS Preliminary}{4.7 fb^{-1} at #sqrt{s}=7TeV e+jets}"

banner = TLatex(0.10,0.91, sbanner)
banner.SetNDC()
banner.SetTextSize(0.05)
banner.SetTextColor(50)
banner.Draw()

cv.SetLogy();

# Print
#if (version1=='BDT'): 
#	outname = "fourtop_MLP"+version +"_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
#elif (version1=='Likelihood'): 
#	outname = "fourtop_Likelihood"+version +"_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
#elif (version1=='Stjet'):
#	outname = "fourtop_stjet_BDT"+version +"_" + calculatemethod +"_" + "Limits_"+sample+".pdf"
#elif (version1=='SM'):
#	outname = "fourtop_SM"+version +"_" + calculatemethod +"_" + "Limits_"+sample+".pdf"
#elif (version2=='combined'):
#	outname = "fourtop_combined_BDT"+version+"_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
#elif (version2=='njets' or version2=='difftheta' or version2=='Nbjets' or version2=='otherBDT' or version2=='diffBDT' or version2=='CSV'):
outname = "st2_Limits.pdf"
cv.SaveAs(outname)

