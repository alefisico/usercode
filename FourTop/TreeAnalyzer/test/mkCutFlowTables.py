#! /usr/bin/env python

from ROOT import *
from decimal import Decimal
import sys,os, math

path = "./"
Ndecimals = 2
IsMC = False
Lumi = 10.93 # in pb-1
JetType = "PF" #calo
showGh = False
kFactor = 1.0
#WprimeType = "RH"
splitWjets = False

if len(sys.argv) < 2:
    print "usage: mkCutFlowTables.py <path to files> <Lumi for MC> < 1 =will produce table for signal Gh>"
    sys.exit()

if len(sys.argv) > 1:

    path = sys.argv[1]
    print " path: "+ path
    if len(sys.argv)>2:
        IsMC = True
        Lumi = float(sys.argv[2])
        print " Luminosity for MC samples: "+str(Lumi)

        if len(sys.argv)>3:

            if sys.argv[3] == "Wbb":
                splitWjets = True
            else:
                showGh = True
                GhType = sys.argv[3]
                print "checking Gh files of type: " + GhType

Nevents = {}
xsec = {} # xsection in pb
file = {}
label = {}
# MC files
if IsMC:
    file['ttbar'] = path+'/results_ttbar.root'
    file['ttbar_powheg'] = path+'/results_ttbar_powheg.root'
    file['QCD']   = path+'/results_QCD.root'
    if splitWjets:
        file['Wbb'] = path+'/results_Wbb.root'
        file['Wcc']= path+'/results_Wcc.root'
        file['Wqq']= path+'/results_Wqq.root'
    else:
        file['Wjets'] = path+'/results_WJets.root'
    file['Zjets'] = path+'/results_ZJets.root'
    file['tch']   = path+'/results_STtch.root'
    file['tWch']  = path+'/results_STtWch.root'
    file['sch']   = path+'/results_STsch.root'
    file['tch_bar']   = path+'/results_STtch_bar.root'
    file['tWch_bar']  = path+'/results_STtWch_bar.root'
    file['sch_bar']   = path+'/results_STsch_bar.root'
    file['WW']    = path+'/results_WW.root'
    file['WZ']    = path+'/results_WZ.root'

    file['ttttSM']    = path+'/results_tttt_SM.root'
    file['Gh1100'] = path+'/results_4Top1100.root'
    file['UED6'] = path+'/results_4TopUED6.root'
    file['Gh400'] = path+'/results_tttt_Gh400.root'
    file['Gh500'] = path+'/results_tttt_Gh500.root'
    file['Gh600'] = path+'/results_tttt_Gh600.root'
    file['Gh700'] = path+'/results_tttt_Gh700.root'
    file['Gh800'] = path+'/results_tttt_Gh800.root'
    file['Gh900'] = path+'/results_tttt_Gh900.root'
    file['Gh1000'] = path+'/results_tttt_Gh1000.root'
    file['Gh1200'] = path+'/results_tttt_Gh1200.root'
    file['UED6'] = path+'/results_tttt_UED6.root'
    
    xsec['ttbar'] = 163.0 #157.5
    xsec['ttbar_powheg'] = 163.0 #157.5
    xsec['QCD']   = 84679.3
    xsec['Wjets'] = 31314.0

    xsec['Wbb'] = 31314.0 * 1.21
    xsec['Wcc'] = 31314.0 * 1.66
    xsec['Wqq'] = 31314.0 * 0.58

    xsec['Zjets'] = 3048.0
    xsec['tch']   = 41.92
    xsec['tWch']  = 7.87
    xsec['sch']   = 3.19
    xsec['tch_bar']   = 22.65
    xsec['tWch_bar']  = 7.87
    xsec['sch_bar']   = 1.44
    xsec['WW']    = 43.0
    xsec['WZ']    = 18.0
    xsec['ttttSM'] = 0.0004746*kFactor
    xsec['Gh400'] = 0.89550*kFactor
    xsec['Gh500'] = 0.18182*kFactor
    xsec['Gh600'] = 0.044471*kFactor
    xsec['Gh700'] = 0.012131*kFactor
    xsec['Gh800'] = 0.003609*kFactor
    xsec['Gh900'] = 0.0011484*kFactor
    xsec['Gh1000'] = 0.00038029*kFactor
    xsec['Gh1100'] = 0.00012830*kFactor
    xsec['UED6'] = 0.134*0.2*0.2*kFactor
    xsec['Gh1200'] = 0.000044145*kFactor
    
    #Nevents['ttbar'] = 3701947.0 #Summer11
    Nevents['ttbar'] = 59366767.0
    Nevents['ttbar_powheg'] =  16330372.0
    Nevents['Wjets'] = 77105816.0

    Nevents['Wbb'] = 77105816.0
    Nevents['Wcc'] = 77105816.0
    Nevents['Wqq'] = 77105816.0
    
    Nevents['Zjets']  = 36277961.0
    Nevents['WW'] = 4225916.0
    Nevents['WZ'] = 4265243.0
    Nevents['QCD'] = 25080241.0
    Nevents['sch'] = 259971.0
    Nevents['sch_bar'] = 137980.0
    Nevents['tch'] = 3900171.0
    Nevents['tch_bar'] = 1944826.0
    Nevents['tWch'] = 814390.0
    Nevents['tWch_bar'] = 809984.0
    
    Nevents['ttttSM'] = 10000.0 #3057.0
    Nevents['Gh1100'] =  10000.0  #4041.0
    Nevents['UED6'] =  10000.0  #3290.0

    Nevents['Gh400'] = 100000.0 # 19000.0
    Nevents['Gh500'] =  98000.0
    Nevents['Gh600'] =  98000.0 #100000.0
    Nevents['Gh700'] =  100000.0
    Nevents['Gh800'] =  100000.0
    Nevents['Gh900'] =  100000.0
    Nevents['Gh1000'] =  86000.0
    Nevents['Gh1200'] = 20000.0
    
    label['ttbar'] = '$t\\overline{t}$'
    label['ttbar_powheg'] = '$t\\overline{t}$'
    label['QCD'] = 'QCD'
    label['Wjets'] = '$W\\rightarrow l\\nu$'

    label['Wbb'] = '$Wbb$'
    label['Wcc'] = '$Wcc$'
    label['Wqq'] = '$Wqq$'
    
    label['Zjets'] = '$Z\\rightarrow l^{+}l^{-}$'
    label['tch'] = 'top t-ch'
    label['tWch'] = 'top tW-ch'
    label['sch'] = 'top s-ch'
    label['tch_bar'] = 'anti-top t-ch'
    label['tWch_bar'] = 'anti-top tW-ch'
    label['sch_bar'] = 'anti-top s-ch'
    label['WW'] = 'WW'
    label['WZ'] = 'WZ'

    label['ttttSM'] = 'tttt SM'

    label['Gh400'] = "Gh 0.4 TeV"
    label['Gh500'] = "Gh 0.5 TeV"
    label['Gh600'] = "Gh 0.6 TeV"
    label['Gh700'] = "Gh 0.7 TeV"
    label['Gh800'] = "Gh 0.8 TeV"
    label['Gh900'] = "Gh 0.9 TeV"
    label['Gh1000'] = "Gh 1.0 TeV"
    label['Gh1100'] = "Gh 1.1 TeV"
    label['Gh1200'] = "Gh 1.2 TeV"
    label['UED6'] = "4Top UED6"
                                    
    label['Total'] = 'Total MC'
else:
# data files
    #file['Jun14'] = ''
    #file['MB']    = '/uscmst1b_scratch/lpc1/cmsroc/yumiceva/top_prod_Sep22/Jun14/ttmuj_Jun14_MB_PSep23.root'#'/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2.88pb-1/ttmuj_MB_Sep3.root'
    #file['Jul16'] = '/uscmst1b_scratch/lpc1/cmsroc/yumiceva/top_prod_Sep22/Jul16/ttmuj_Jul16_PSep23.root'#'/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2.88pb-1/ttmuj_Jul16_Sep3.root'
    #file['Prompt']= '/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2.88pb-1/ttmuj_Prompt_Sep3.root'

    #label['Jun14'] = 'Jun 14th Mu'
    #label['MB'] = 'Jun 14th MinimumBias'
    #label['Jul16'] = 'Jul 16th'
    #label['Prompt'] = 'PromptReco'

    #if os.path.isfile(path+'/JPT/cutflow_JPT_data.txt'):
    #    file['dataJPT'] = path+'/cutflow_JPT_data.txt'
    #    label['dataJPT'] = 'JPT Ref. Sel.'
    #if os.path.isfile(path+'/calo/cutflow_calo_data.txt'):
    #    file['datacalo'] = path+'/cutflow_calo_data.txt'
    #    label['datacalo'] = 'Calo Ref. Sel.'
    #if os.path.isfile(path+'/PF/cutflow_PF_data.txt'):
    #    file['dataPF'] = path+'/cutflow_PF_data.txt'
    #    label['dataPF'] = 'PF Ref. Sel.'

    file['data'] = path+'/results_data.root';
    label['data'] = "Data";
    
    
keys = file.keys()
cutflow = {}
cutflowerr = {}
cutlabel = {}
cutlabel['Processed'] = 'Processed'
cutlabel['CleanFilters'] = 'CleanFilters'
cutlabel['HLT'] = 'HLT'
cutlabel['GoodPV'] = 'Skim'
cutlabel['OneIsoMu'] = 'One Iso $\mu$'
cutlabel['LooseMuVeto'] = 'Loose $\mu$ veto'
cutlabel['ElectronVeto'] = 'Electron veto'
cutlabel['ZMassVeto'] = 'Z Mass veto'
cutlabel['MET'] = 'MET'
cutlabel['1Jet'] = 'jets $\geq 1$'
cutlabel['2Jet'] = 'jets $\geq 2$'
cutlabel['3Jet'] = 'jets $\geq 3$'
cutlabel['4Jet'] = 'jets $\geq 4$'
cutlabel['7Jet'] = 'jets $\geq 7$'
cutlabel['8Jet'] = 'jets $\geq 8$'
cutlabel['9Jet'] = 'jets $\geq 9$'
cutlabel['Ht'] = '$H_t \geq 300$'
cutlabel['4Jet0b'] = 'btags $ = 0$'
cutlabel['4Jet1b'] = 'btags $\geq 3$'
cutlabel['4JetCut'] = '$p_t^{jet4} > 40$ '
cutlabel['Ht2'] = '$H_t \geq 500$'
cutlabel['Stjet'] = '$S_t^{jet} \geq 500$'

cutlabelvector = [ 'GoodPV', 'OneIsoMu', 'LooseMuVeto', 'ElectronVeto', 'MET', '4Jet','Ht', '4Jet0b', '4Jet1b', '4JetCut', 'Stjet', '7Jet', '8Jet', '9Jet']
#cutlabelvector = [ 'GoodPV', 'OneIsoMu', 'LooseMuVeto', 'ElectronVeto', 'ZMassVeto', 'MET', '4Jet','Ht', '4Jet0b', '4Jet1b', '4JetCut', 'Stjet']
#cutlabelvector = [ 'GoodPV', 'OneIsoMu', 'ElectronVeto', 'MET' '4Jet','Ht', '4Jet0b', '4Jet1b', '4JetCut', 'Stjet']
#SKIPCUTS = ['LooseMuVeto','4Jet0b']#'GoodPV', 'OneIsoMu', 'LooseMuVeto', 'ElectronVeto', 'MET',]
SKIPCUTS = ['GoodPV', 'OneIsoMu', 'LooseMuVeto', 'ElectronVeto', 'MET', '4Jet0b']

allmap = {}
allmaperr = {}

weightmap = {}

#tablelist = ['ttbar','Wjets','Zjets', 'QCD', 'tch','tch_bar','tWch','tWch_bar','sch','sch_bar','WW','WZ','Total']
tablelist = ['ttbar']

if splitWjets:
    tablelist = ['ttbar','Wbb','Wcc','Wqq','Zjets','tch','tch_bar','tWch','tWch_bar','sch','sch_bar','WW','WZ','Total']
    #tablelist = ['ttbar','Wbb','Wcc','Wqq','Zjets','QCD','tch','tch_bar','tWch','tWch_bar','sch','sch_bar','WW','WZ','Total']
    #tablelist = ['ttbar_powheg','Wbb','Wcc','Wqq','Zjets','QCD','tch','tch_bar','tWch','tWch_bar','sch','sch_bar','WW','WZ','Total']
    
if showGh:
    #tablelist = ['Gh500','Gh700','Gh900','Gh1000','Gh1100']
    #tablelist = ['4TopSM','Gh500','Gh900','Gh1100','UED6']
    tablelist = ['ttttSM', 'Gh400','Gh500','Gh600','Gh700','Gh800','Gh900','Gh1000', 'UED6']

if Lumi<=0:
    tablelist = ['ttbar','Wjets','Zjets','tch','tch_bar','tWch','tWch_bar','sch','sch_bar','WW','WZ']
    #tablelist = ['ttbar','Wjets','Zjets','tch','tch_bar','tWch','tWch_bar','sch','WW','WZ']

if not IsMC:
    tablelist = ['data']

for sample in tablelist:

    if sample=="Total": continue
    print " processing " + sample

    cutmap = {}
    cutmaperr = {}
    #txtfile = open(file[sample])
    roofile = TFile(file[sample])
    hcutflow = gDirectory.Get("cutflow")
    print "  entries in cutflow histogram: " + str(hcutflow.GetEntries())

    for ibin in xrange(1, hcutflow.GetNbinsX() +1 ):

        skipthiscut = False
        for skipcut in SKIPCUTS:
            if skipcut == cutlabelvector[ibin-1]: skipthiscut = True
        if skipthiscut:
            print "skip counting cut name: "+cutlabelvector[ibin-1]
            continue
        cutname = cutlabelvector[ibin-1]
        acut = hcutflow.GetBinContent(ibin)
        #print cutname
        #print acut
        #if sample=="data":
        if sample.find("data")!=-1:
            cutmap[ cutname ] = int(float(acut))
            cutmaperr[ cutname ] = math.sqrt( cutmap[cutname] )
        else:
            cutmap[ cutname ] = Decimal(str(acut))
            cutmaperr[ cutname ] = cutmap[cutname].sqrt() #math.sqrt( cutmap[cutname] )
            
    roofile.Close()
    
    scale = 1.
    if IsMC and Lumi>0:
        scale = Decimal( str( Lumi * xsec[ sample ] / Nevents[sample] ))
        print "sample weight = "+ str( xsec[ sample ] / Nevents[sample] )
        weightmap[sample] = xsec[ sample ] / Nevents[sample]
    ilabel = 0
    for key in cutmap.keys():

        cutmap[ key ] = Decimal(str(scale)) * cutmap[ key]
        cutmaperr[key]= Decimal(str(scale)) * Decimal(str(cutmaperr[key])) * Decimal( str(scale) ) * Decimal(str(cutmaperr[key])) 
        
        print " cut "+str(key) + " ("+cutlabel[key]+") "+" = "+str( round(cutmap[key],Ndecimals) )

        if allmap.has_key(key):
            allmap[ key ] += cutmap[ key ]
            allmaperr[ key ] += cutmaperr[key]
        else:
            allmap[ key ] = cutmap[ key ]
            allmaperr[ key ] = cutmap[key]
        ilabel += 1
    cutflow[ sample ] = cutmap
    cutflowerr[ sample ] = cutmaperr
    
print " TOTAL"
ilabel=0
for key in allmap.keys():
    print " cut "+str(key) + " ("+cutlabel[key]+") "+" = "+str( round(allmap[key],Ndecimals) )
    ilabel += 1
    
cutflow["Total"] = allmap
cutflowerr["Total"] = allmaperr

# write latex
#sortedcutlist = ['GoodPV','OneIsoMu','LooseMuVeto','ElectronVeto', 'MET']
#sortedcutlist = ['GoodPV','OneIsoMu','LooseMuVeto','ElectronVeto', 'ZMassVeto', 'MET']
#sortedcutlist = ['GoodPV','OneIsoMu','ElectronVeto', 'MET','4Jet']
#sortedcutlist = ['GoodPV','ElectronVeto', '4Jet', 'Ht']
sortedcutlist= ['4Jet', 'Ht', '4Jet1b', '4JetCut','Stjet'] 
#sortedcutlist= ['4Jet', '4Jet1b'] 
#sortedcutlist= ['4Jet', '7Jet', '8Jet', '9Jet'] 
#sortedcutlist2= [ 'Ht', '4Jet1b', '4JetCut', 'Stjet'] 
sortedcutlist2= [] 

if IsMC:
    cutlabel['CleanFilters'] = 'Processed'

texname = "cutflow_"+JetType+"_data.tex"

if IsMC:
    texname = "cutflow_"+JetType+"_MC.tex"
    if showGh:
        texname = "cutflow_"+JetType+"_MC_4Top.tex"
        
outtex = open(texname,"w")

sss = " & "

# Write Header
outtex.write('''
\\documentclass[a4paper]{article}
\\usepackage[a4paper, left=1.5cm, right=1cm, top=2cm, bottom=2cm]{geometry}
\\begin{document}
\\begin{table}[h]
\\begin{centering}
''')
if IsMC:
    aline = '\\begin{tabular}{|l|'+'c|'*(len(sortedcutlist)+1) +'} \hline \n'
    if Lumi<=0:
        aline = '\\begin{tabular}{|l|'+'c|'*len(sortedcutlist) +'} \hline \n'
    outtex.write(aline)
    #outtex.write('Cut & ttbar & Wjets & Zjets & QCD & STtch \hline')
    line = "Sample"
    for icut in sortedcutlist:
        line += sss+cutlabel[icut]
    outtex.write(line+" \\\ \hline \n")
else:
    aline = '\\begin{tabular}{l|r|r|r|} \hline \n'
    
    outtex.write(aline)
    line = "Sample"
    for icut in sortedcutlist:
        line += sss+cutlabel[icut]
    outtex.write(line+" \\\ \hline \n")

ilabel = 0                   
#for key in allmap.keys():
# Write content
for sample in tablelist:
    
    line = label[sample]
    
    for key in sortedcutlist:
        cutmap = cutflow[sample]
        cutmaperr = cutflowerr[sample]
        acut = cutmap[key]
        acuterr = cutmaperr[key]
        stracut = str(int(acut))
        stracuterr = str(round(math.sqrt(acuterr),Ndecimals))
        #stracut = stracut.strip('.0')
        if IsMC:
            acut = round(cutmap[key],Ndecimals)
            stracut = str(acut)
            line += sss + stracut + " $\\pm$ " + stracuterr
        else:
            line += sss + stracut
    if sample=="Total":line = '\\hline \n'+line
    outtex.write(line+" \\\ \n")
    ilabel += 1
    
outtex.write(''' \hline
\end{tabular}
%\caption{MC cutflow}\label{tab:tab}
\end{centering}
\end{table}
''')


if len(sortedcutlist2) > 0:

    
    # Write Header
    outtex.write('''
\\begin{table}[h]
\\begin{centering}
    ''')
    if IsMC:
        aline = '\\begin{tabular}{|l|'+'c|'*(len(sortedcutlist2)+1) +'} \hline \n'
        if Lumi<=0:
            aline = '\\begin{tabular}{|l|'+'c|'*len(sortedcutlist2) +'} \hline \n'
        outtex.write(aline)
        #outtex.write('Cut & ttbar & Wjets & Zjets & QCD & STtch \hline')
        line = "Sample"
        for icut in sortedcutlist2:
            line += sss+cutlabel[icut]
        outtex.write(line+" \\\ \hline \n")
    else:
        aline = '\\begin{tabular}{|l|r|r|r|} \hline \n'
            
        outtex.write(aline)
        line = "Sample"
        for icut in sortedcutlist2:
            line += sss+cutlabel[icut]
        outtex.write(line+" \\\ \hline \n")

    ilabel = 0
    #for key in allmap.keys():
    # Write content
    for sample in tablelist:
        
        line = label[sample]

        for key in sortedcutlist2:
            cutmap = cutflow[sample]
            cutmaperr = cutflowerr[sample]
            acut = cutmap[key]
            acuterr = cutmaperr[key]
            stracut = str(int(acut))
            stracuterr = str(round(math.sqrt(acuterr),Ndecimals))
            #stracut = stracut.strip('.0')
            if IsMC:
                acut = round(cutmap[key],Ndecimals)
                stracut = str(acut)
                line += sss + stracut + " $\\pm$ " + stracuterr
            else:
                line += sss + stracut
        if sample=="Total":line = '\\hline \n'+line
        outtex.write(line+" \\\ \n")
        ilabel += 1
            
    outtex.write(''' \hline
\end{tabular}
%\caption{MC cutflow}\label{tab:tab}
\end{centering}
\end{table}
\end{document}
    ''')
    
print "file "+texname+ " has been written."

if IsMC:
    
    print "\n MC Weights"
    tmplistsamples = weightmap.keys()
    tmplistsamples.sort()
    for sample in tmplistsamples:

        print sample+" "+str(weightmap[sample])


    

