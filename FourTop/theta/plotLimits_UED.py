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
WType = "GH"
calculatemethod = sys.argv[2] #"bayesian" #"cls" #"bayesian"
ExcludePoints = [] #2300, 2500]

#dir = 'LimitPlot_2012_6_12_twentyonevariablesvariables_default_BDT_UED/'
#dir = 'LimitPlot_2012_6_12_twentyonevariablesvariables_default_BDT_UED_v2/'
#dir = 'LimitPlot_2012_6_12_twentyonevariablesvariables_default_BDT_UED_v3/'
#dir = 'LimitPlot_2012_6_12_twentyonevariablesvariables_default_BDT_UED_v4/'

exp_filename = {}
obs_filename = {}
colors = {}

#os.system('mkdir %s'%dir)

#Baydir = '/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/theta_testing/utils/examples/wprime/FourTopTemplate/'
#Clsdir = '/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/theta_testing/utils/examples/wprime/FourTopTemplate/'

nominal =   sys.argv[3] 

###########UED #########################
#exp_filename['BDT'] = "bayesian_limits_expected_BDT110_Nbjets3_UED6.txt"
exp_filename['BDT'] = "bayesian_limits_expected_stjet_UED6_Nbjets3.txt"
#obs_filename['BDT'] = "bayesian_limits_observed_BDT110_Nbjets3_UED6.txt"
obs_filename['BDT'] = "bayesian_limits_observed_stjet_UED6_Nbjets3.txt"
colors['BDT'] = 1

#exp_filename['BDT'] = Baydir + "VariableVerison1_TwentyoneVariables_AllMass_newchi2_opitimizied_newsamples_new_UED6_v2/bayesian_limits_expected_fourtop.txt"
#obs_filename['BDT'] = Baydir + "VariableVerison1_TwentyoneVariables_AllMass_newchi2_opitimizied_newsamples_new_UED6_v2/bayesian_limits_observed_fourtop.txt"
#colors['BDT'] =1# ROOT.kCyan + 2 
#
#exp_filename['BDT'] = Baydir + "VariableVerison1_TwentyoneVariables_AllMass_newchi2_opitimizied_newsamples_new_UED6_v3/bayesian_limits_expected_fourtop.txt"
#obs_filename['BDT'] = Baydir + "VariableVerison1_TwentyoneVariables_AllMass_newchi2_opitimizied_newsamples_new_UED6_v3/bayesian_limits_observed_fourtop.txt"
#colors['BDT'] =1# ROOT.kCyan + 2 

#exp_filename['BDT'] = Baydir + "VariableVerison1_TwentyoneVariables_AllMass_newchi2_opitimizied_newsamples_new_UED6_v4/bayesian_limits_expected_fourtop.txt"
#obs_filename['BDT'] = Baydir + "VariableVerison1_TwentyoneVariables_AllMass_newchi2_opitimizied_newsamples_new_UED6_v4/bayesian_limits_observed_fourtop.txt"
#colors['BDT'] =1# ROOT.kCyan + 2 
##Four top cross sections
kFactor = 1.0
xsec = {}
xsecnew = {}
# right-handed
theory_crosssection = 1.433
xsecnew[0.050] = theory_crosssection* 0.050 * 0.050 * kFactor
xsecnew[0.075] = theory_crosssection* 0.075 * 0.075 * kFactor
xsecnew[0.100] = theory_crosssection* 0.100 * 0.100 * kFactor
xsecnew[0.125] = theory_crosssection* 0.125 * 0.125 * kFactor
xsecnew[0.150] = theory_crosssection* 0.150 * 0.150 * kFactor
xsecnew[0.175] = theory_crosssection* 0.175 * 0.175 * kFactor
xsecnew[0.200] = theory_crosssection* 0.200 * 0.200 * kFactor
xsecnew[0.225] = theory_crosssection* 0.225 * 0.225 * kFactor
xsecnew[0.250] = theory_crosssection* 0.250 * 0.250 * kFactor
xsecnew[0.275] = theory_crosssection* 0.275 * 0.275 * kFactor
xsecnew[0.300] = theory_crosssection* 0.300 * 0.300 * kFactor
xsecnew[0.325] = theory_crosssection* 0.325 * 0.325 * kFactor
xsecnew[0.350] = theory_crosssection* 0.350 * 0.350 * kFactor

generate_crosssection = 1.433 
xsec[0.050] = generate_crosssection * 0.050 * 0.050 * kFactor
xsec[0.075] = generate_crosssection * 0.075 * 0.075 * kFactor
xsec[0.100] = generate_crosssection * 0.100 * 0.100 * kFactor
xsec[0.125] = generate_crosssection * 0.125 * 0.125 * kFactor
xsec[0.150] = generate_crosssection * 0.150 * 0.150 * kFactor
xsec[0.175] = generate_crosssection * 0.175 * 0.175 * kFactor
xsec[0.200] = generate_crosssection * 0.200 * 0.200 * kFactor
xsec[0.225] = generate_crosssection * 0.225 * 0.225 * kFactor
xsec[0.250] = generate_crosssection * 0.250 * 0.250 * kFactor
xsec[0.275] = generate_crosssection * 0.275 * 0.275 * kFactor
xsec[0.300] = generate_crosssection * 0.300 * 0.300 * kFactor
xsec[0.325] = generate_crosssection * 0.325 * 0.325 * kFactor
xsec[0.350] = generate_crosssection * 0.350 * 0.350 * kFactor

#xsec[0.050] = 1.0 * kFactor
#xsec[0.075] = 1.0*kFactor
#xsec[0.100] = 1.0 * kFactor
#xsec[0.125] = 1.0 * kFactor
#xsec[0.150] = 1.0 * kFactor
#xsec[0.175] = 1.0 * kFactor
#xsec[0.200] = 1.0* kFactor
## read input text files

g_exp = {}
g_sigma1 = {}
g_sigma2 = {}
g_obs = {}
#g_ratio = {}


# theory
#xlist = xsecnew.keys()
xlist = xsec.keys()

xlist.sort()
ylist = []
for ii in xlist:
    #ylist.append(xsecnew[ii])
    ylist.append(xsec[ii])
print xlist
print ylist
x_theory = array('d')
y_theory = array('d')
x_theory.fromlist( xlist)
y_theory.fromlist( ylist)

print y_theory
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
        point = float(list[0])
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
    print expected

    observed = {}

    obs_file = open(obs_filename[test])
    for line in obs_file:
        if line.startswith("#"): continue
        list = line.split()
        point = float(list[0])
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
frame = TH2F("frame", "frame", 10, 0., 0.4, 100, 0.0001, 2.0)
#frame = TH2F("frame", "frame", 10, 0., 0.25, 100, 0.001, 5.0)
#frame = TH2F("frame", "frame", 10, 300, x_obs[len(x_obs)-1]+100, 100, 0.01, 0.25)
frame.SetMinimum(0.01)
frame.GetXaxis().SetTitle("Branching Ratio of A_{#mu}#rightarrowt#bar{t}")
frame.GetYaxis().SetTitle("#sigma#upointBR(pp#rightarrowA_{#mu}A_{#mu}#rightarrowt#bar{t}t#bar{t}) [pb]")
frame.SetNdivisions(505)

# plot
cv = TCanvas("cv","cv",700,700)
frame.Draw()

if(sys.argv[4] == "drawlimit"):
   g_sigma2[nominal].Draw("F")
   g_sigma1[nominal].Draw("F")

# legend
aleg = TLegend(0.50,0.15,0.90,0.4)
aleg.SetMargin(0.12)
aleg.SetTextSize(0.025)
aleg.SetFillColor(10)
aleg.SetBorderSize(0)
aleg.SetLineStyle(0)
aleg.SetFillStyle(0)


for test in filekeys:
    g_exp[test].Draw("c")
    if(sys.argv[4] == "drawlimit"):
      g_obs[test].Draw("Pl")
    
    if(sys.argv[4] == "drawlimit"):
      aleg.AddEntry( g_obs[test], "95% Observed","P")
      aleg.AddEntry( g_exp[test], "95% Expected","L")
    else:
      aleg.AddEntry( g_exp[test], "95% Expected "+ str(' '.join(test.split('_'))),"L")
      #aleg.AddEntry( g_obs[test], "95% Observed "+test,"P")
if(sys.argv[4] == "drawlimit"):
  aleg.AddEntry( g_sigma1[nominal], "#pm1#sigma Expected","F")
  aleg.AddEntry( g_sigma2[nominal], "#pm2#sigma Expected","F")
aleg.AddEntry( g_theory, "Giacomo et. al., JHEP 10 (2011) 042","L")
aleg.Draw()

g_theory.Draw("C")

sbanner = "#splitline{CMS Preliminary}{5.0 fb^{-1} at #sqrt{s}=7TeV #mu+jets}"
if sample == "combined": sbanner = "#splitline{CMS Preliminary}{5.0 fb^{-1} at #sqrt{s}=7TeV #mu+jets,e+jets}"
if sample == "e": sbanner = "#splitline{CMS Preliminary}{5.0 fb^{-1} at #sqrt{s}=7TeV e+jets}"

banner = TLatex(0.25,0.85, sbanner);
banner.SetNDC();
banner.SetTextSize(0.035);
banner.Draw();

cv.SetLogy();

# Print
#outname = "fourtop_BDTUED6_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
outname = "fourtop_stjetUED6_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"

cv.Print(outname)
#cv.Print(outnamepng)

raw_input( 'Press ENTER to continue\n ' )
