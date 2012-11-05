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
version = sys.argv[3]
version1 = sys.argv[4]
version2 = sys.argv[5]
ExcludePoints = [] #2300, 2500]

exp_filename = {}
obs_filename = {}
colors = {}

BDT = 'BDT'
Likelihood = 'Likelihood'
Stjet = 'Stjet'

CSVL=''
CSVM=''
Nobtag=''
btag=''
N4jets=''
N5jets=''
N6jets=''
withOnedifftheta=''
withoutOnedifftheta=''
N5jetsIsoMu='5jets with OneIsoMu'
N5jetswIsoMu='5jets without OneIsoMu'
btagwithoutOnedifftheta=''
btagwithOnedifftheta=''
newBDT=''
prevBDT=''

if (version1 == 'BDT') and version:
	exp_filename['BDT'] = "bayesian_limits_expected_MLP"+version+".txt"
	obs_filename['BDT'] = "bayesian_limits_observed_MLP"+version+".txt"
	colors['BDT'] = 1

elif (version1 == 'Stjet') and version:
	exp_filename['Stjet'] = "bayesian_limits_expected_stjet_"+version+".txt"
	obs_filename['Stjet'] = "bayesian_limits_observed_stjet_"+version+".txt"
	colors['Stjet'] = 1

elif (version2 == 'combined'):
	exp_filename['BDT'] = "bayesian_limits_expected_BDT"+version+".txt"
	obs_filename['BDT'] = "bayesian_limits_observed_BDT"+version+".txt"
	colors['BDT'] = 1
	exp_filename['Stjet'] = "bayesian_limits_expected_stjet_"+version1+".txt"
	obs_filename['Stjet'] = "bayesian_limits_observed_stjet_"+version1+".txt"
	colors['Stjet'] = ROOT.kRed

elif (version2 == 'njets'):
	exp_filename['N5jets'] = "bayesian_limits_expected_BDT"+version+".txt"
	obs_filename['N5jets'] = "bayesian_limits_observed_BDT"+version+".txt"
	colors['N5jets'] = 1
	exp_filename['N6jets'] = "bayesian_limits_expected_BDT"+version1+".txt"
	obs_filename['N6jets'] = "bayesian_limits_observed_BDT"+version1+".txt"
	colors['N6jets'] = ROOT.kBlue

elif (version2 == 'CSV'):
	exp_filename['BDT with CSVL'] = "bayesian_limits_expected_BDT"+version+".txt"
	obs_filename['BDT with CSVL'] = "bayesian_limits_observed_BDT"+version+".txt"
	colors['BDT with CSVL'] = 1
	exp_filename['BDT with CSVM'] = "bayesian_limits_expected_BDT"+version1+".txt"
	obs_filename['BDT with CSVM'] = "bayesian_limits_observed_BDT"+version1+".txt"
	colors['BDT with CSVM'] = ROOT.kBlue

elif (version2 == 'difftheta'):
	exp_filename['BDT with #Delta #Phi (W,W)'] = "bayesian_limits_expected_BDT"+version+".txt"
	obs_filename['BDT with #Delta #Phi (W,W)'] = "bayesian_limits_observed_BDT"+version+".txt"
	colors['BDT with #Delta #Phi (W,W)'] = 1
	exp_filename['BDT w/o #Delta #Phi (W,W)'] = "bayesian_limits_expected_BDT"+version1+".txt"
	obs_filename['BDT w/o #Delta #Phi (W,W)'] = "bayesian_limits_observed_BDT"+version1+".txt"
	colors['BDT w/o #Delta #Phi (W,W)'] = ROOT.kBlue

elif (version2 == 'Nbjets'):
	exp_filename['Nobtag'] = "bayesian_limits_expected_BDT"+version+".txt"
	obs_filename['Nobtag'] = "bayesian_limits_observed_BDT"+version+".txt"
	colors['Nobtag'] = 1
	exp_filename['btag'] = "bayesian_limits_expected_BDT"+version1+".txt"
	obs_filename['btag'] = "bayesian_limits_observed_BDT"+version1+".txt"
	colors['btag'] = ROOT.kBlue

elif (version2 == 'otherBDT'):
	exp_filename['BDT old #chi^{2}'] = "bayesian_limits_expected_BDT"+version+".txt"
	obs_filename['BDT old #chi^{2}'] = "bayesian_limits_observed_BDT"+version+".txt"
	colors['BDT old #chi^{2}'] = 1
	exp_filename['BDT new #chi^{2}'] = "bayesian_limits_expected_BDT"+version1+".txt"
	obs_filename['BDT new #chi^{2}'] = "bayesian_limits_observed_BDT"+version1+".txt"
	colors['BDT new #chi^{2}'] = ROOT.kBlue

elif (version2 == 'diffBDT'):
	exp_filename['old BDT'] = "bayesian_limits_expected_BDT"+version+".txt"
	obs_filename['old BDT'] = "bayesian_limits_observed_BDT"+version+".txt"
	colors['old BDT'] = ROOT.kBlue
	exp_filename['new BDT'] = "bayesian_limits_expected_BDT"+version1+".txt"
	obs_filename['new BDT'] = "bayesian_limits_observed_BDT"+version1+".txt"
	colors['new BDT'] = 1

elif (version1 == 'Likelihood') and version:
	exp_filename['Likelihood'] = "bayesian_limits_expected_Likelihood"+version+".txt"
	obs_filename['Likelihood'] = "bayesian_limits_observed_Likelihood"+version+".txt"
	colors['Likelihood'] = 1


if calculatemethod == "cls":
	if (version1 == 'BDT') and version:
		exp_filename['BDT'] = "cls_limits_expected_BDT"+version+".txt"
		obs_filename['BDT'] = "cls_limits_observed_BDT"+version+".txt"
		colors['BDT'] = 1

	elif (version1 == 'Likelihood') and version:
		exp_filename['Likelihood'] = "cls_limits_expected_Likelihood"+version+".txt"
		obs_filename['Likelihood'] = "cls_limits_observed_Likelihood"+version+".txt"
		colors['Likelihood'] = 1

	elif (version1 == 'Stjet') and version:
		exp_filename['Stjet'] = "cls_limits_expected_stjet_"+version+".txt"
		obs_filename['Stjet'] = "cls_limits_observed_stjet_"+version+".txt"
		colors['Stjet'] = 1

	elif (version2 == 'combined'):
		exp_filename['BDT'] = "cls_limits_expected_BDT"+version+".txt"
		obs_filename['BDT'] = "cls_limits_observed_BDT"+version+".txt"
		colors['BDT'] = 1
		exp_filename['Stjet'] = "cls_limits_expected_stjet_"+version1+".txt"
		obs_filename['Stjet'] = "cls_limits_observed_stjet_"+version1+".txt"
		colors['Stjet'] = ROOT.kRed


# W' cross sections
kFactor = 1.0
xsec = {}
# right-handed
#xsec['sm'] = 0.0004746*kFactor;
xsec[400] = 0.89550*kFactor;
xsec[500] = 0.18182*kFactor;
xsec[600] = 0.044471*kFactor;
xsec[700] = 0.012131*kFactor;
xsec[800] = 0.003609*kFactor;
xsec[900] = 0.0011484*kFactor;
xsec[1000] = 0.00038029*kFactor;
xsec[1100] = 0.00012830*kFactor;
xsec[1200] = 0.000044145*kFactor;

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
frame = TH2F("frame", "frame", 10, 300, x_exp[len(x_exp)-1]+100, 100, 0.001, 2.0)
#frame = TH2F("frame", "frame", 10, 490, x_obs[len(x_obs)-1]+10, 100, 0, 0.2)
#frame = TH2F("frame", "frame", 10, 300, x_obs[len(x_obs)-1]+100, 100, 0.001, 2.0);
frame.GetXaxis().SetTitle("G_{H} mass [GeV]")
frame.GetYaxis().SetTitle("#sigma#upointBR(pp#rightarrow G_{H}G_{H} #rightarrow t#bar{t}t#bar{t}) [pb]")
frame.SetNdivisions(505)
      
# plot
cv = TCanvas("cv","cv",700,700)
frame.Draw()


if (version1 =='BDT' or version1=='Stjet' or version1=='Likelihood'):
	if version1=='BDT':
		g_sigma2[BDT].Draw("F")
		g_sigma1[BDT].Draw("F")
	elif version1=='Likelihood':
		g_sigma2[Likelihood].Draw("F")
		g_sigma1[Likelihood].Draw("F")
	else:
		g_sigma2[Stjet].Draw("F")
		g_sigma1[Stjet].Draw("F")

# legend
aleg = TLegend(0.43,0.65,0.99,0.90)
aleg.SetMargin(0.12)
aleg.SetTextSize(0.025)
aleg.SetFillColor(10)
aleg.SetBorderSize(0)
aleg.SetLineStyle(0)
aleg.SetFillStyle(0)

    
for test in filekeys:
    
    g_exp[test].Draw("c")
    if version1=='combined':
	    g_exp[BDT].SetLineStyle(1)
	    g_exp[Stjet].SetLineStyle(2)
    aleg.AddEntry( g_exp[test], "95% Expected "+test,"L")

if (version1=='BDT' or version1=='Stjet' or version1=='Likelihood'):
        g_obs[test].Draw("Pl")
        aleg.AddEntry( g_obs[test], "95% Observed "+test,"P")
	if version1=='BDT':
		aleg.AddEntry( g_sigma1[BDT], "#pm1#sigma Expected","F")
		aleg.AddEntry( g_sigma2[BDT], "#pm2#sigma Expected","F")
	elif version1=='Likelihood':
		aleg.AddEntry( g_sigma1[Likelihood], "#pm1#sigma Expected","F")
		aleg.AddEntry( g_sigma2[Likelihood], "#pm2#sigma Expected","F")
	else:
		aleg.AddEntry( g_sigma1[Stjet], "#pm1#sigma Expected","F")
		aleg.AddEntry( g_sigma2[Stjet], "#pm2#sigma Expected","F")

aleg.AddEntry( g_theory, "Dobrescu et.al., PLB 670 (2008) 119-123","L")
aleg.Draw()

g_theory.Draw("c")
   
sbanner = "#splitline{CMS Preliminary}{5.0 fb^{-1} at #sqrt{s}=7TeV #mu+jets}"
if sample == "combined": sbanner = "#splitline{CMS Preliminary}{4.7 fb^{-1} at #sqrt{s}=7TeV #mu+jets,e+jets}"
if sample == "e": sbanner = "#splitline{CMS Preliminary}{4.7 fb^{-1} at #sqrt{s}=7TeV e+jets}"

banner = TLatex(0.23,0.20, sbanner);
banner.SetNDC();
banner.SetTextSize(0.035);
banner.Draw();

cv.SetLogy();

# Print
if (version1=='BDT'): 
	outname = "fourtop_MLP"+version +"_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
elif (version1=='Likelihood'): 
	outname = "fourtop_Likelihood"+version +"_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
elif (version1=='Stjet'):
	outname = "fourtop_stjet_BDT"+version +"_" + calculatemethod +"_" + "Limits_"+sample+".pdf"
elif (version1=='SM'):
	outname = "fourtop_SM"+version +"_" + calculatemethod +"_" + "Limits_"+sample+".pdf"
elif (version2=='combined'):
	outname = "fourtop_combined_BDT"+version+"_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
elif (version2=='njets' or version2=='difftheta' or version2=='Nbjets' or version2=='otherBDT' or version2=='diffBDT' or version2=='CSV'):
	outname = "fourtop_combined_BDT"+version+"_BDT"+version1+"_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
cv.Print(outname)

raw_input( 'Press ENTER to continue\n ' )
