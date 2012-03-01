from ROOT import *
import sys
import os
import sys

# CMS Style
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
                                                        
gROOT.SetBatch()

filename = sys.argv[1] #'mu_templates.root'
file = TFile(filename)

sample = sys.argv[2]
systname = sys.argv[3]
ghsample = sys.argv[4]
variable = sys.argv[5]

#SF_Wbb = 1.21
#SF_Wcc = 1.66
#SF_Wqq = 1.

#outfile = TFile(filename,"RECREATE")

file.cd()
list = file.GetListOfKeys()

hbb = {}
#hcc = {}
#hqq = {}

for key in list:
    file.cd()
    #print "key name "+key.GetName()
    #print "name = "+ key.GetName() +" title = "+ key.GetTitle()
    name = key.GetName()
    #print "key name = " + name
    obj = file.Get(name)
    #obj.SetName( key.GetName() )
    #print "obj name = "+ obj.GetName() +" title = "+ obj.GetTitle()
    if name =="mu_"+variable+ghsample+"__"+sample:
        hbb['nominal'] = obj
    if name == "mu_"+variable+ghsample+"__"+sample+"__"+systname+"__plus":
        hbb['plus'] = obj
    if name == "mu_"+variable+ghsample+"__"+sample+"__"+systname+"__minus":
        hbb['minus'] = obj
                                        
    #if name.find("mu1allb_mass__wqq") !=-1: obj.Scale( SF_Wqq)
    #outfile.cd()
    #obj.Write()



cbb = TCanvas("cbb","cbb",700,700)
hbb['plus'].SetLineColor(2)
hbb['nominal'].SetLineColor(1)
hbb['minus'].SetLineColor(4)
hbb['plus'].SetMarkerColor(2)
hbb['nominal'].SetMarkerColor(1)
hbb['minus'].SetMarkerColor(4)
hbb['plus'].SetMarkerStyle(2)
hbb['nominal'].SetMarkerStyle(2)
hbb['minus'].SetMarkerStyle(2)

hbb['plus'].DrawNormalized()
hbb['nominal'].DrawNormalized("same")
hbb['minus'].DrawNormalized("same")

aleg = TLegend(0.6,0.65,0.93,0.93)
SetOwnership( aleg, 0 )
aleg.SetHeader("#splitline{CMS 2011 Simulation}{"+sample+" "+systname+" syst.}")
aleg.SetMargin(0.12)
aleg.SetTextSize(0.035)
aleg.SetFillColor(0)
aleg.SetFillStyle(0)
aleg.SetBorderSize(0)

aleg.AddEntry(hbb['plus'],"plus","PL")
aleg.AddEntry(hbb['nominal'],"nominal","PL")
aleg.AddEntry(hbb['minus'],"minus","PL")
aleg.Draw()

cbb.Print(sample+"_"+variable+"_"+ghsample+"_"+systname+"_syst.png")

#sigma_bb = 0.28
#sigma_cc = 0.06
#sigma_qq = 0.5
#outfile.Close()
#os.system('rm tmp_'+filename)

raw_input ("Enter to quit:")
