#!/usr/bin/env python

import sys
import os
from ROOT import *
#from ROOT import RooFit,RooStats
ROOT.gSystem.SetIncludePath('-I$ROOFITSYS/include')
#if os.access('RooPowerFunction.cxx', os.R_OK):
#ROOT.gROOT.ProcessLine('.L RooPowerFunction.cxx+')

##### Initialize some arguments
Mass = int(sys.argv[1])
Lumi = 19445 
histo = 'step3plots1D/massRecoDiBjetDiJet_cutDiagStop2bbjj0'

##### Read Info in file
with open('signalInfo.txt') as f:
	arrays = [map(float, line.split()) for line in f]
for subarray in arrays:
	if subarray[0] == Mass:
		print subarray
		XS = subarray[1]
		Acceptance = subarray[2]
		Width = subarray[3]
print XS, Acceptance, Width


####### RooFit Variables
x = RooRealVar("x","x",0.,2000.)
quadruplets = RooArgList( x )

#### DATA
dataInput = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_6jet60_plots.root" 
dataFile = TFile(dataInput)
tmph_Data = dataFile.Get(histo)
h_Data = tmph_Data.Clone()
data = RooDataHist("data_obs","data", quadruplets, h_Data)
nData = h_Data.Integral(36,120)
#print nData
nBkg = RooRealVar("nBkg","number of background events",nData,0,2*nData)
#print nBkg

#### SIGNAL
signalInput = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_"+str(Mass)+"_200_4jet80_6jet60_plots.root" 
signalFile = TFile(signalInput)
tmph_Signal = signalFile.Get(histo)
h_Signal = tmph_Signal.Clone()
signal = RooDataHist("massRecoDiBjetDiJet_cutDiagStop2bbjj","signal", quadruplets, h_Signal)

nSig = RooRealVar("nSig","number of signal events",float(XS)*float(Acceptance)*Lumi)
GausMean = RooRealVar("GausMean","GausMean", Mass)
GausSigma = RooRealVar("GausSigma","GausSigma", Width,0,100)
signalGaus = RooGaussian("signal_pdf","gaussian PDF", x, GausMean, GausSigma)
exPdfSignalGaus = RooExtendPdf("signal", "extended signal PDF", signalGaus, nSig)

##### Background (P4 function)
p1 = RooRealVar("p1","p1", -113.6,-1000,1000)
p2 = RooRealVar("p2","p2", 56.39,-100,100)
p3 = RooRealVar("p3","p3", 8.31,-100,100)
    
bkgP4 = RooGenericPdf("bkgP4","bkg pdf","pow(1-x/8000.0,p1)/pow(x/8000.0,p2+p3*log(x/8000.))",RooArgList(x,p1,p2,p3))
exPdfBkgP4 = RooExtendPdf("bkg","background PDF P4 fit",bkgP4,nBkg) 

#exPdfBkgP4.fitTo(data,Strategy(2))
exPdfBkgP4.fitTo(data)
#ExPdfQCDBackground.fitTo(data);
#bkg.fitTo(data);

#### Plot
#xframe = x.frame()
#xframe.SetMinimum(1e-5)
#xframe.SetTitle("")
#xframe.GetYaxis().SetTitle("Quadruplets / 10 GeV")
#RooAbsData.plotOn(data, xframe)
#obj = xframe.findObject("data")
#leg = TLegend(0.419598,0.5087413,0.8731156,0.8356643,NULL,"brNDC")
#leg.SetBorderSize(1)
#leg.SetTextFont(42)
#leg.SetLineColor(0)
#leg.SetLineStyle(1)
#leg.SetLineWidth(1)
#leg.SetFillColor(0)
#leg.SetFillStyle(1001)
    
#RooAbsData.plotOn( exPdfBkgP4, xframe)
#exPdfBkgP4.plotOn( xframe )



##### WORKSPACE FOR ROOFIT
#create workspace
pWs = RooWorkspace("myWS")
# import dataset into workspace
getattr(pWs,'import')(data) # we call it this way because "import" is a reserved word in python
getattr(pWs,'import')(exPdfBkgP4)
getattr(pWs,'import')(exPdfSignalGaus)
nBkgDataCard = nBkg.getVal()
nSigDataCard = nSig.getVal()
print nBkgDataCard, nSigDataCard

#save the workspace to a file
#pWs.SaveAs(outfilename)
pWs.SaveAs('workspace_jj_'+str(Mass)+'_200.root')

######### Create DataCard
DataCard = open('DataCard_jj_'+str(Mass)+'_200.txt',"w")
text = ['imax 1 channels \n',
		'jmax 1 backgrounds \n',
		'kmax 0 systematics \n',
		'------------------------------- \n',
		'shapes * * workspace_jj_'+str(Mass)+'_200.root myWS:$PROCESS \n',
		'-------------------------------\n',
		'bin           1\n',
		'observation  -1\n',
		'-------------------------------\n',
		'bin           1          1\n',
		'process       0          1\n',
		'process     signal     bkg\n',
		'rate        '+str(nSigDataCard)+'   '+str(nBkgDataCard)+'\n',
		'-------------------------------\n']
DataCard.writelines(text)
DataCard.close()




#def MakeWorkspace(outfilename):
	
	# use this area to implement your model for a counting experiment
	#print "building counting model..."

#	return 0

################################################################################
#if __name__ == "__main__":
	#MakeWorkspace("workspace.root")
