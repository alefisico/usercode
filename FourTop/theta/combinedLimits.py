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
colors = {}

nominal = 'nominal'
cutbased = 'cutbased'

CSVL=''
CSVM=''
Nobtag=''
btag=''
N4jets=''
N5jets=''
N6jets=''
N5jetswithOneIsoMuon=''
N5jetswithoutOneIsoMuon=''
N5jetsbtagwithoutOneIsoMuon=''
N5jetsbtagwithOneIsoMuon=''
N4jetswithOneIsoMuon=''
N4jetswithoutOneIsoMuon=''
N4jetsbtagwithoutOneIsoMuon=''
N4jetsbtagwithOneIsoMuon=''
prev=''
set1=''
set2=''
both=''

#exp_filename['BDT'] = "bayesian_limits_expected_BDT110_Nbjets3_v1.txt"
#colors['BDT'] = ROOT.kBlue
#exp_filename['BDT with Nbjets'] = "bayesian_limits_expected_BDT110_Nbjets3_v2.txt"
#colors['BDT with Nbjets'] = ROOT.kOrange

#exp_filename['Summer11'] = "bayesian_limits_expected_BDT108_v12.txt"
#colors['Summer11'] = ROOT.kBlue
#exp_filename['Fall11'] = "bayesian_limits_expected_BDT109_Nbjets1_v1.txt"
#colors['Fall11'] = ROOT.kOrange

#exp_filename['21 var'] = "bayesian_limits_expected_BDT108_v12.txt"
#colors['21 var'] = ROOT.kBlue
#exp_filename['12 var'] = "bayesian_limits_expected_BDT108_v20.txt"
#colors['12 var'] = ROOT.kOrange
#exp_filename['6 var'] = "bayesian_limits_expected_BDT108_v21.txt"
#colors['6 var'] = ROOT.kCyan

##exp_filename['Nbjets #geq 1'] = "bayesian_limits_expected_stjet_109.txt"
#exp_filename['Nbjets #geq 1'] = "bayesian_limits_expected_BDT109_Nbjets1_v1.txt"
#colors['Nbjets #geq 1'] = ROOT.kBlack
#exp_filename['Nbjets #geq 1 with Nbjets '] = "bayesian_limits_expected_BDT109_Nbjets1_v2.txt"
#colors['Nbjets #geq 1 with Nbjets '] = ROOT.kBlue
##exp_filename['Nbjets #geq 2'] = "bayesian_limits_expected_BDT110_Nbjets2_v2.txt"
####exp_filename['Nbjets #geq 2'] = "bayesian_limits_expected_stjet_109_Nbjets2.txt"
##colors['Nbjets #geq 2'] = ROOT.kOrange
#exp_filename['Nbjets #geq 3'] = "bayesian_limits_expected_BDT110_Nbjets3_v1.txt"
####exp_filename['Nbjets #geq 3'] = "bayesian_limits_expected_stjet_109_Nbjets3.txt"
#colors['Nbjets #geq 3'] = ROOT.kCyan
#exp_filename['Nbjets #geq 3 with Nbjets '] = "bayesian_limits_expected_BDT110_Nbjets3_v2.txt"
####exp_filename['Nbjets #geq 3 with Nbjets '] = "bayesian_limits_expected_stjet_109_Nbjets3.txt"
#colors['Nbjets #geq 3 with Nbjets '] = ROOT.kOrange

#exp_filename['Nbjets #geq 1'] = "bayesian_limits_expected_BDT108_v12.txt"
#colors['Nbjets #geq 1'] = ROOT.kBlack
#exp_filename['Nbjets #geq 1 w/o sys'] = "bayesian_limits_expected_BDT108_v12_v0.txt"
#colors['Nbjets #geq 1 w/o sys'] = ROOT.kBlue
#exp_filename['Nbjets #geq 2 w/o sys'] = "bayesian_limits_expected_BDT108_Nbjets2_v1_v0.txt"
#colors['Nbjets #geq 2 w/o sys'] = ROOT.kOrange
#exp_filename['Nbjets #geq 3 w/o sys'] = "bayesian_limits_expected_BDT108_Nbjets3_v1_v0.txt"
#colors['Nbjets #geq 3 w/o sys'] = ROOT.kCyan

#exp_filename['BDT'] = "bayesian_limits_expected_BDT108_Nbjets2_v3.txt"
#colors['BDT'] = ROOT.kBlue
#exp_filename['without ttbar Match'] = "bayesian_limits_expected_BDT108_Nbjets2_v3_v2.txt"
#colors['without ttbar Match'] = ROOT.kOrange
#exp_filename['without ttbar Q^{2}'] = "bayesian_limits_expected_BDT108_Nbjets2_v3_v3.txt"
#colors['without ttbar Q^{2}'] = ROOT.kCyan
#exp_filename['w/o ttbar Match & Q^{2}'] = "bayesian_limits_expected_BDT108_Nbjets2_v3_v4.txt"
#colors['w/o ttbar Match & Q^{2}'] = ROOT.kBlack

#exp_filename['5'] = "bayesian_limits_expected_BDT103_v5.txt"
#colors['5'] = ROOT.kGreen
#exp_filename['6'] = "bayesian_limits_expected_BDT103_v6.txt"
#colors['6'] = ROOT.kPink-9
#exp_filename['60'] = "cls_limits_expected_BDT103_v6.txt"
#colors['60'] = ROOT.kBlack
#exp_filename['7'] = "bayesian_limits_expected_BDT103_v7.txt"
#colors['7'] = ROOT.kViolet-7
#exp_filename['8'] = "bayesian_limits_expected_BDT103_v8.txt"
#colors['8'] = ROOT.kMagenta
#exp_filename['Stjet'] = "bayesian_limits_expected_stjet_103.txt"
#colors['Stjet'] = ROOT.kBlack
#exp_filename['10'] = "bayesian_limits_expected_BDTvfinal_10.txt"
#colors['10'] = ROOT.kTeal+9
#exp_filename['11'] = "bayesian_limits_expected_BDTvfinal_11.txt"
#colors['11'] = ROOT.kSpring-7
#exp_filename['12'] = "bayesian_limits_expected_BDTvfinal_12.txt"
#colors['12'] = ROOT.kGray
#exp_filename['13'] = "bayesian_limits_expected_BDTvfinal_13.txt"
#colors['13'] = ROOT.kYellow
#

#exp_filename['N4jets'] = "bayesian_limits_expected_BDTv4.txt"
#colors['N4jets'] = ROOT.kBlue
#exp_filename['N4jetswithoutOneIsoMuon'] = "bayesian_limits_expected_BDTv3.txt"
#colors['N4jetswithoutOneIsoMuon'] = ROOT.kAzure
#exp_filename['N4jetsbtagwithoutOneIsoMuon'] = "bayesian_limits_expected_BDTv5.txt"
#colors['N4jetsbtagwithoutOneIsoMuon'] = ROOT.kCyan
#exp_filename['N4jetsbtagwithOneIsoMuon'] = "bayesian_limits_expected_BDTv6.txt"
#colors['N4jetsbtagwithOneIsoMuon'] = ROOT.kGreen
#exp_filename['N5jets'] = "bayesian_limits_expected_BDTv9.txt"
#colors['N5jets'] = ROOT.kRed
#exp_filename['N5jetswithoutOneIsoMuon'] = "bayesian_limits_expected_BDTv10.txt"
#colors['N5jetswithoutOneIsoMuon'] = ROOT.kMagenta
#exp_filename['N5jetsbtagwithoutOneIsoMuon'] = "bayesian_limits_expected_BDTv13.txt"
#colors['N5jetsbtagwithoutOneIsoMuon'] = ROOT.kYellow
#exp_filename['N5jetsbtagwithOneIsoMuon'] = "bayesian_limits_expected_BDTv12.txt"
#colors['N5jetsbtagwithOneIsoMuon'] = ROOT.kOrange
#exp_filename['N6jets'] = "bayesian_limits_expected_BDTv11.txt"
#colors['N6jets'] = ROOT.kViolet

exp_filename['MLP'] = "bayesian_limits_expected_MLPv1.txt"
colors['MLP'] = ROOT.kBlue
exp_filename['BDT'] = "bayesian_limits_expected_BDT110_Nbjets3_v2.txt"
colors['BDT'] = ROOT.kOrange
exp_filename['Likelihood'] = "bayesian_limits_expected_Likelihoodv1.txt"
colors['Likelihood'] = ROOT.kRed

#exp_filename['BDT'] = "bayesian_limits_expected_BDT108_v12.txt"
#colors['BDT'] = ROOT.kBlue
#exp_filename['Likelihood'] = "bayesian_limits_expected_Likelihood108_v1.txt"
#colors['Likelihood'] = ROOT.kOrange

if calculatemethod == "cls":
    exp_filename = "cls_limits_expected_wprh.txt"


# W' cross sections
kFactor = 1.0
xsec = {}
# right-handed
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

    # create graphs
    g_exp[test] = TGraph( len(x_exp), x_exp, y_exp)
    g_exp[test].SetLineStyle(kDashed)
    g_exp[test].SetLineColor( colors[test] )
    g_exp[test].SetLineWidth(3)
  

# create frame
#frame = TH2F("frame", "frame", 10, 390, x_exp[len(x_exp)-1]+10, 100, 0, 1.0)
frame = TH2F("frame", "frame", 10, 300, x_exp[len(x_exp)-1]+100, 100, 0.001, 2.0);
frame.GetXaxis().SetTitle("G_{H} mass [GeV]")
frame.GetYaxis().SetTitle("#sigma#upointBR(pp#rightarrow G_{H} G_{H} #rightarrow t#bar{t}t#bar{t}) [pb]")
frame.SetNdivisions(505)
      
# plot
cv = TCanvas("cv","cv",700,700)
frame.Draw()


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
    
    aleg.AddEntry( g_exp[test], "95% Expected "+test,"L")

aleg.AddEntry( g_theory, "Dobrescu et.al., PLB 670 (2008) 119-123","L")
aleg.Draw()

g_theory.Draw("c")
   
sbanner = "#splitline{CMS Preliminary}{5.0 fb^{-1} at #sqrt{s}=7TeV #mu+jets}"
if sample == "combined": sbanner = "#splitline{CMS Preliminary}{5.0 fb^{-1} at #sqrt{s}=7TeV #mu+jets,e+jets}"
if sample == "e": sbanner = "#splitline{CMS Preliminary}{4.7 fb^{-1} at #sqrt{s}=7TeV e+jets}"

banner = TLatex(0.23,0.20, sbanner);
banner.SetNDC();
banner.SetTextSize(0.035);
banner.Draw();

cv.SetLogy();

# Print
#outname = "fourtop_combined_nbjets_stjet_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
#outname = "fourtop_combined_nbjets_v2_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
#outname = "fourtop_combined_nbjets_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
#outname = "fourtop_combined_ttbarMatchScale_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
#outname = "fourtop_combined_diffdelta_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
#outname = "fourtop_combined_diffttbar_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
#outname = "fourtop_combined_diffNbjets3BDT_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
#outname = "fourtop_combined_diffvarBDT_"+ calculatemethod +"_" + "Limits_"+sample+".pdf"
#outname = "fourtop_combined_difflimitmethod_Limits_"+sample+".pdf"
outname = "fourtop_combined_diffMVA_Limits_"+sample+".pdf"

cv.Print(outname)

raw_input( 'Press ENTER to continue\n ' )
