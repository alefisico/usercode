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
    if name ==variable+"_tttt_Gh400":
        hbb['tttt_Gh400'] = obj
    if name ==variable+"_tttt_Gh400_woC":
        hbb['tttt_Gh400_woC'] = obj
    if name ==variable+"_tttt_Gh600":
        hbb['tttt_Gh600'] = obj
    if name ==variable+"_tttt_Gh600_woC":
        hbb['tttt_Gh600_woC'] = obj
    if name ==variable+"_tttt_Gh1000":
        hbb['tttt_Gh1000'] = obj
    if name ==variable+"_tttt_Gh1000_woC":
        hbb['tttt_Gh1000_woC'] = obj
    #if name.find("mu1allb_mass__wqq") !=-1: obj.Scale( SF_Wqq)
    #outfile.cd()
    #obj.Write()



cbb = TCanvas("cbb","cbb",800,600)
hbb['tttt_Gh400'].SetLineColor(kBlue)
hbb['tttt_Gh400'].SetLineWidth(2)
hbb['tttt_Gh400'].SetLineStyle(2)
hbb['tttt_Gh400'].SetYTitle("Unit Area")
#hbb['tttt_Gh400'].SetXTitle("Tagged b-jets CSVM")
max1 = hbb['tttt_Gh400'].GetMaximum()
hbb['tttt_Gh400_woC'].SetLineColor(kBlue)
hbb['tttt_Gh400_woC'].SetLineWidth(2)
hbb['tttt_Gh400_woC'].SetLineStyle(3)
max2 = hbb['tttt_Gh400_woC'].GetMaximum()
hbb['tttt_Gh600'].SetLineColor(kRed)
hbb['tttt_Gh600'].SetLineWidth(2)
hbb['tttt_Gh600'].SetLineStyle(2)
max3 = hbb['tttt_Gh600'].GetMaximum()
hbb['tttt_Gh600_woC'].SetLineColor(kRed)
hbb['tttt_Gh600_woC'].SetLineWidth(2)
hbb['tttt_Gh600_woC'].SetLineStyle(3)
max4 = hbb['tttt_Gh600_woC'].GetMaximum()
hbb['tttt_Gh1000'].SetLineColor(kGreen)
hbb['tttt_Gh1000'].SetLineWidth(2)
hbb['tttt_Gh1000'].SetLineStyle(2)
max5 = hbb['tttt_Gh1000'].GetMaximum()
hbb['tttt_Gh1000_woC'].SetLineColor(kGreen)
hbb['tttt_Gh1000_woC'].SetLineWidth(2)
hbb['tttt_Gh1000_woC'].SetLineStyle(3)
max6 = hbb['tttt_Gh1000_woC'].GetMaximum()

list = [max1, max2, max3, max4, max5, max6]
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
#hbb['ttbar'].SetMaximum( max1 * 2.00)
#hbb['ttbar'].DrawNormalized("hist")
#hbb['ttbar'].DrawNormalized("hist same")
#hbb['tttt_Gh400'].DrawNormalized("hist")
hbb['tttt_Gh400'].DrawNormalized("hist")
hbb['tttt_Gh400'].SetMaximum( theMax * 2.00)
hbb['tttt_Gh400_woC'].DrawNormalized("hist same")
hbb['tttt_Gh600'].DrawNormalized("hist same")
hbb['tttt_Gh600_woC'].DrawNormalized("hist same")
hbb['tttt_Gh1000'].DrawNormalized("hist same")
hbb['tttt_Gh1000_woC'].DrawNormalized("hist same")
#hbb['tttt_Gh700'].DrawNormalized("hist same")
#hbb['tttt_Gh800'].DrawNormalized("hist same")
#hbb['tttt_Gh900'].DrawNormalized("hist same")
#hbb['tttt_Gh1000'].DrawNormalized("hist same")

#aleg = TLegend(0.2,0.20,0.50,0.40)
aleg = TLegend(0.60,0.60,0.93,0.93)
SetOwnership( aleg, 0 )
aleg.SetHeader("CMS Simulation #mu+jets") #Preliminary") # - 5.0 fb^{-1} at #sqrt{s}=7TeV")
aleg.SetMargin(0.12)
aleg.SetTextSize(0.035)
aleg.SetFillColor(0)
aleg.SetFillStyle(0)
aleg.SetBorderSize(0)

#aleg.AddEntry(hbb['data'],"Data","L")
#aleg.AddEntry(hbb['ttbar'],"t#bar{t}","L")
aleg.AddEntry(hbb['tttt_Gh400'],"t#bar{t}t#bar{t} G_{H} 0.4 TeV","L")
aleg.AddEntry(hbb['tttt_Gh400_woC'],"t#bar{t}t#bar{t} G_{H} 0.4 TeV w/o Btag Corr","L")
aleg.AddEntry(hbb['tttt_Gh600'],"t#bar{t}t#bar{t} G_{H} 0.6 TeV","L")
aleg.AddEntry(hbb['tttt_Gh600_woC'],"t#bar{t}t#bar{t} G_{H} 0.6 TeV w/o Btag Corr","L")
aleg.AddEntry(hbb['tttt_Gh1000'],"t#bar{t}t#bar{t} G_{H} 1.0 TeV","L")
aleg.AddEntry(hbb['tttt_Gh1000_woC'],"t#bar{t}t#bar{t} G_{H} 1.0 TeV w/o Btag Corr","L")
aleg.Draw()

cbb.Print(variable+"_diff.pdf")

#sigma_bb = 0.28
#sigma_cc = 0.06
#sigma_qq = 0.5
#outfile.Close()
#os.system('rm tmp_'+filename)

#raw_input ("Enter to quit:")
