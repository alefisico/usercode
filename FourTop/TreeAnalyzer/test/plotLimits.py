#!/usr/bin/env python
#____________________________________________________________
#
#
# Francisco Yumiceva
# yumiceva@fnal.gov
#
# Fermilab, 2008
#
#____________________________________________________________         

from ROOT import *
import sys
import os
import sys
from array import array

if os.path.isfile('tdrstyle.C'):
    gROOT.ProcessLine('.L tdrstyle.C')
    ROOT.setTDRStyle()
    print "Found tdrstyle.C file, using this style."
    HasCMSStyle = True
    if os.path.isfile('CMSTopStyle.cc'):
        gROOT.ProcessLine('.L CMSTopStyle.cc+')
        style = CMSTopStyle()
        style.setupICHEPv1()
        print "Found CMSTopStyle.cc file, use TOP style if requested in xml file."


# configuration
sample = sys.argv[1] # mu or e or combined
#sample = "e"
WType = "Gh"
calculatemethod = sys.argv[2] #"bayesian" #"cls" #"bayesian"
ExcludePoints = [] #2300, 2500]

exp_filename = {}
obs_filename = {}
colors = {}

nominal = 'nominal'

exp_filename['nominal'] = "bayesian_limits_expected_fourtop_test.txt"
#exp_filename['nominal'] = "bayesian_limits_expected_stjet_fourtop.txt"
obs_filename['nominal'] = "bayesian_limits_observed_fourtop_test.txt"
#obs_filename['nominal'] = "bayesian_limits_observed_stjet_fourtop.txt"
colors['nominal'] = 1

#exp_filename['only_stat'] = "res_bayesian_nosyst/bayesian_limits_expected_wprh.txt"
#obs_filename['only_stat'] = "res_bayesian_nosyst/bayesian_limits_observed_wprh.txt"
#colors['only_stat'] = 12

#exp_filename['only_rate'] = "res_bayesian_onlyrate/bayesian_limits_expected_wprh.txt"
#obs_filename['only_rate'] = "res_bayesian_onlyrate/bayesian_limits_observed_wprh.txt"
#colors['only_rate'] = ROOT.kRed

#exp_filename['no_btag'] = "res_bayesian_nobtag/bayesian_limits_expected_wprh.txt"
#obs_filename['no_btag'] = "res_bayesian_nobtag/bayesian_limits_observed_wprh.txt"
#colors['no_btag'] = ROOT.kBlue

#exp_filename['no_btag_ttbarmatch'] = "res_bayesian_nobtag_match/bayesian_limits_expected_wprh.txt"
#obs_filename['no_btag_ttbarmatch'] = "res_bayesian_nobtag_match/bayesian_limits_observed_wprh.txt"
#colors['no_btag_ttbarmatch'] = 6

#exp_filename['no_btag_ttbarmatch_wjets'] = "res_bayesian_nobtag_match_wjets/bayesian_limits_expected_wprh.txt"
#obs_filename['no_btag_ttbarmatch_wjets'] = "res_bayesian_nobtag_match_wjets/bayesian_limits_observed_wprh.txt"
#colors['no_btag_ttbarmatch_wjets'] = 7

#exp_filename['no_btag_ttbarmatch_wjets_nlo'] = "res_bayesian_nobtag_match_wjets_nlo/bayesian_limits_expected_wprh.txt"
#obs_filename['no_btag_ttbarmatch_wjets_nlo'] = "res_bayesian_nobtag_match_wjets_nlo/bayesian_limits_observed_wprh.txt"
#colors['no_btag_ttbarmatch_wjets_nlo'] = 9

#exp_filename['WbbWccWqq'] = "res_bayesian/bayesian_limits_expected_wprh.txt"
#obs_filename['WbbWccWqq'] = "res_bayesian/bayesian_limits_observed_wprh.txt"
#colors['WbbWccWqq'] = ROOT.kBlue


if calculatemethod == "cls":
    exp_filename = "cls_limits_expected_wprh.txt"
    obs_filename = "cls_limits_observed_wprh.txt"


# W' cross sections
kFactor = 1.0
xsec = {}
# right-handed
xsec[500] = 0.18182*kFactor;
xsec[700] = 0.012131*kFactor;
xsec[900] = 0.0011484*kFactor;
xsec[1000] = 0.00038029*kFactor;
xsec[1100] = 0.00012830*kFactor;

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
frame = TH2F("frame", "frame", 10, 490, x_obs[len(x_obs)-1]+10, 100, 0, 0.1)
frame.GetXaxis().SetTitle("Gh(t#bar{t}) mass [GeV]")
frame.GetYaxis().SetTitle("#sigma#upointBR(Gh#rightarrow t#bar{t}) [pb]")
frame.SetNdivisions(505)
      
# plot
cv = TCanvas("cv","cv",700,700)
frame.Draw()


g_sigma2[nominal].Draw("F")
g_sigma1[nominal].Draw("F")

# legend
aleg = TLegend(0.20,0.15,0.75,0.45)
aleg.SetMargin(0.12)
aleg.SetTextSize(0.025)
aleg.SetFillColor(10)
aleg.SetBorderSize(0)
aleg.SetLineStyle(0)
aleg.SetFillStyle(0)

    
for test in filekeys:
    
    g_exp[test].Draw("c")
    g_obs[test].Draw("Pl")
    
    aleg.AddEntry( g_obs[test], "95% Observed "+test,"P")
    aleg.AddEntry( g_exp[test], "95% Expected "+test,"L")

aleg.AddEntry( g_sigma1[nominal], "#pm1#sigma Expected","F")
aleg.AddEntry( g_sigma2[nominal], "#pm2#sigma Expected","F")
aleg.AddEntry( g_theory, "Gh","L")
aleg.Draw()

g_theory.Draw("c")
   
sbanner = "#splitline{CMS Preliminary}{4.7 fb^{-1} at #sqrt{s}=7TeV #mu+jets}"
if sample == "combined": sbanner = "#splitline{CMS Preliminary}{4.7 fb^{-1} at #sqrt{s}=7TeV #mu+jets,e+jets}"
if sample == "e": sbanner = "#splitline{CMS Preliminary}{4.7 fb^{-1} at #sqrt{s}=7TeV e+jets}"

banner = TLatex(0.55,0.85, sbanner);
banner.SetNDC();
banner.SetTextSize(0.035);
banner.Draw();

cv.SetLogy();

# Print
outname = "fourtop"+WType+"_" + calculatemethod +"_" + "Limits_"+sample+"_test.png"
#outname = "fourtopStjet"+WType+"_" + calculatemethod +"_" + "Limits_"+sample+".png"
#outname = "fourtop_" + calculatemethod +"_" + "Limits_"+sample+".png"

cv.Print(outname)

raw_input( 'Press ENTER to continue\n ' )
