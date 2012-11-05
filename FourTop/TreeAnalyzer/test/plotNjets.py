from ROOT import *
import sys
import os
import sys
gROOT.SetStyle("Plain")
gStyle.SetOptStat(0)
gStyle.SetHistFillColor(kWhite)

# CMS Style
if os.path.isfile('tdrstyle.C'):
    gROOT.ProcessLine('.L tdrstyle.C')
    ROOT.setTDRStyle()
    print "Found tdrstyle.C file, using this style."
    HasCMSStyle = True
    if os.path.isfile('CMSTopStyle.cc'):
        gROOT.ProcessLine('.L CMSTopStyle.cc+')
        style = CMSTopStyle()
        style.setupDefault()
        print "Found CMSTopStyle.cc file, use TOP style if requested in xml file."
                                                       
gROOT.SetBatch()

filename = sys.argv[1] #'mu_templates.root'
file = TFile(filename)

variable = sys.argv[2]


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
    if name ==variable+"_data":
        hbb['data'] = obj
    if name ==variable+"_ttbar":
        hbb['ttbar'] = obj
    if name ==variable+"_TTbar":
        hbb['TTbar'] = obj
    if name ==variable+"_tttt_Gh400":
        hbb['tttt_Gh400'] = obj
    if name ==variable+"_tttt_Gh500":
        hbb['tttt_Gh500'] = obj
    if name ==variable+"_tttt_Gh600":
        hbb['tttt_Gh600'] = obj
    if name ==variable+"_tttt_Gh700":
        hbb['tttt_Gh700'] = obj
    if name ==variable+"_tttt_Gh800":
        hbb['tttt_Gh800'] = obj
    if name ==variable+"_tttt_Gh900":
        hbb['tttt_Gh900'] = obj
    if name ==variable+"_tttt_Gh1000":
        hbb['tttt_Gh1000'] = obj
    if name ==variable+"_tttt_SM":
        hbb['tttt_SM'] = obj
    #if name.find("mu1allb_mass__wqq") !=-1: obj.Scale( SF_Wqq)
    #outfile.cd()
    #obj.Write()



cbb = TCanvas("cbb","cbb",800,600)
#hbb['ttbar'].SetLineColor(1)
#hbb['ttbar'].SetLineWidth(2)
#hbb['ttbar'].SetYTitle("Unit Area")
#max1 = hbb['ttbar'].GetMaximum()
#hbb['TTbar'].SetLineColor(2)
#hbb['TTbar'].SetLineWidth(2)
#max2 = hbb['TTbar'].GetMaximum()
#hbb['tttt_SM'].SetLineColor(1)
#hbb['tttt_SM'].SetLineWidth(2)
#hbb['tttt_SM'].SetYTitle("Unit Area")
#max5 = hbb['tttt_SM'].GetMaximum()
#hbb['tttt_Gh400'].SetLineColor(6)
#hbb['tttt_Gh400'].SetLineWidth(2)
#hbb['tttt_Gh400'].SetLineStyle(2)
#max2 = hbb['tttt_Gh400'].GetMaximum()
##hbb['tttt_Gh500'].SetLineColor(2)
##hbb['tttt_Gh500'].SetLineWidth(2)
##hbb['tttt_Gh500'].SetLineStyle(2)
##max4 = hbb['tttt_Gh500'].GetMaximum()
#hbb['tttt_Gh600'].SetLineColor(4)
#hbb['tttt_Gh600'].SetLineWidth(2)
#hbb['tttt_Gh600'].SetLineStyle(3)
#max3 = hbb['tttt_Gh600'].GetMaximum()
##hbb['tttt_Gh700'].SetLineColor(5)
##hbb['tttt_Gh800'].SetLineColor(4)
##hbb['tttt_Gh800'].SetLineWidth(2)
##hbb['tttt_Gh800'].SetLineStyle(4)
##hbb['tttt_Gh900'].SetLineColor(7)
#hbb['tttt_Gh1000'].SetLineColor(3)
#hbb['tttt_Gh1000'].SetLineWidth(2)
#hbb['tttt_Gh1000'].SetLineStyle(5)
#max4 = hbb['tttt_Gh1000'].GetMaximum()
#hbb['tttt_SM'].SetLineColor(2)
#hbb['tttt_SM'].SetLineWidth(2)
#max5 = hbb['tttt_SM'].GetMaximum()

hbb['ttbar'].SetLineColor(1)
hbb['ttbar'].SetFillColor(0)
hbb['ttbar'].SetLineWidth(2)
hbb['ttbar'].SetLineStyle(1)
hbb['ttbar'].SetYTitle("Unit Area")
max1 = hbb['ttbar'].GetMaximum()
hbb['TTbar'].SetLineColor(2)
hbb['TTbar'].SetFillColor(0)
hbb['TTbar'].SetLineWidth(2)
hbb['TTbar'].SetLineStyle(2)
max2 = hbb['TTbar'].GetMaximum()
list = [max1, max2]
theMax = max(list)

#hbb['plus'].SetMarkerColor(2)
#hbb['tttt_Gh400'].SetMarkerColor(1)
#hbb['minus'].SetMarkerColor(4)
#hbb['plus'].SetMarkerStyle(2)
#hbb['tttt_Gh400'].SetMarkerStyle(2)
#hbb['minus'].SetMarkerStyle(2)

#cbb.SetLogy()
#hbb['data'].SetMaximum(10)
#hbb['ttbar'].SetMaximum(theMax * 1.55 )
#hbb['TTbar'].SetMaximum( max1 )
#hbb['tttt_Gh1000'].DrawNormalized("hist")
hbb['ttbar'].DrawNormalized("hist")
hbb['TTbar'].DrawNormalized("ehist same")
#hbb['tttt_Gh400'].DrawNormalized("hist same")
##hbb['tttt_Gh400'].DrawNormalized("hist")
##hbb['tttt_Gh400'].SetMaximum( max2 * 2.00)
##hbb['tttt_Gh500'].DrawNormalized("hist same")
#hbb['tttt_Gh600'].DrawNormalized("hist same")
#hbb['tttt_SM'].DrawNormalized("hist same")
#hbb['tttt_Gh700'].DrawNormalized("hist same")
#hbb['tttt_Gh800'].DrawNormalized("hist same")
#hbb['tttt_Gh900'].DrawNormalized("hist same")
#hbb['ttbar'].DrawNormalized("hist")
#hbb['ttbar'].SetMaximum( theMax * 2.00)
#hbb['TTbar'].DrawNormalized("hist esame")

#aleg = TLegend(0.2,0.20,0.50,0.40)
aleg = TLegend(0.65,0.70,0.93,0.93)
SetOwnership( aleg, 0 )
aleg.SetHeader("CMS Simulation #mu+jets") #Preliminary") # - 5.0 fb^{-1} at #sqrt{s}=7TeV")
aleg.SetMargin(0.12)
aleg.SetTextSize(0.035)
aleg.SetFillColor(0)
aleg.SetFillStyle(0)
aleg.SetBorderSize(0)

#aleg.AddEntry(hbb['data'],"Data","L")
aleg.AddEntry(hbb['TTbar'],"t#bar{t} Pythia8 ","L")
aleg.AddEntry(hbb['ttbar'],"t#bar{t} Pythia6 ","L")
#aleg.AddEntry(hbb['tttt_SM'],"t#bar{t}t#bar{t} SM","L")
#aleg.AddEntry(hbb['tttt_Gh400'],"t#bar{t}t#bar{t} G_{H} 0.4 TeV","L")
#aleg.AddEntry(hbb['tttt_Gh500'],"t#bar{t}t#bar{t} G_{H} 0.5 TeV","L")
#aleg.AddEntry(hbb['tttt_Gh600'],"t#bar{t}t#bar{t} G_{H} 0.6 TeV","L")
#aleg.AddEntry(hbb['tttt_Gh700'],"t#bar{t}t#bar{t} G_{H} 0.7 TeV","L")
#aleg.AddEntry(hbb['tttt_Gh1000'],"t#bar{t}t#bar{t} G_{H} 1.0 TeV","L")
#aleg.AddEntry(hbb['TTbar'],"t#bar{t} pythia8 + FastSim","L")
#aleg.AddEntry(hbb['ttbar'],"t#bar{t} pythia6 + FullSim","L")
aleg.Draw()

#cbb.Print(variable+"_ttbardiff.pdf")
cbb.Print(variable+"_diff.pdf")

#sigma_bb = 0.28
#sigma_cc = 0.06
#sigma_qq = 0.5
#outfile.Close()
#os.system('rm tmp_'+filename)

#raw_input ("Enter to quit:")
