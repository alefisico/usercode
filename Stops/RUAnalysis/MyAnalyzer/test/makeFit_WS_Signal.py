#!/usr/bin/env python

###################
### Make Fitting
###################

from ROOT import *
from ROOT import RooFit
import glob,sys, datetime

gROOT.Reset()

##################################################
#### Initial Parameters                       ####
##################################################

####### Bin Info
binSize = 10.0
massBins = [0, 30, 60, 90, 120, 150, 180, 210, 250, 290, 330, 370, 410, 460, 510, 560, 610, 670, 730, 790, 860, 930, 1000, 1080, 1160, 1240, 1330, 1420, 1520, 1620, 1730, 1840, 2000]
FitStart = 400
FitEnd = 2000
Lumi = 19445
#Mass = [      450,       550,       650,        750 ]
listXS    = [ 0.169668, 0.0452067, 0.0139566, 0.00480639 ]	

####### RooFit Variables
x = RooRealVar("x","Quadruplet Invariant Mass [GeV]",0.,2000.)
quadruplets = RooArgList( x )
x.setRange(FitStart,FitEnd)


##############################################################
#### Fit P4 + Gaussian for Signal and create WorkSpace    ####
##############################################################
def fitP4toData(st2mass, histo, folder, file, outputDir, output, WSname, WSDir, jes):

	inputFile = TFile(file)
	h_initSignal = inputFile.Get(folder+'/' + histo)

	############ Pre Fit Gaussian
	############ Looks like Roofit does not need this part
#	h_Signal_Gaussian = h_initSignal.Clone()
#
#	xGauss = RooRealVar("xGauss","Quadruplet Invariant Mass [GeV]",st2mass-50,st2mass+50)
#	quadrupletsGauss = RooArgList( xGauss )
#
#	signal_Gaussian = RooDataHist("signal_Gauss", "signal_Gauss", quadrupletsGauss, h_Signal_Gaussian)
#
#	initGausMean = RooRealVar("initGausMean", "initGausMean", st2mass, st2mass-50,st2mass+50)
#	initGausSigma = RooRealVar("initGausSigma", "initGausSigma", 25, -50, 50)
#	initSignalGaus = RooGaussian("initSignal_pdf", "init gaussian pdf", xGauss, initGausMean, initGausSigma)
#
#	initSignalGaus.fitTo(signal_Gaussian, RooFit.Strategy(2))
#
#	integralGaus = h_Signal_Gaussian.Integral((st2mass/10)-4, (st2mass/10)+4)/10	 #preSignalGaus.analyticalIntegral(2)
	#print '#############################################', integralGaus
	numSig = RooRealVar("numSig","number of events in signal", 100, 0, 10000)


	############ Pre Fit P4
	h_Signal_P4 = h_initSignal.Clone()
	nSigBkg = h_Signal_P4.Integral(36,200)

	# Background (P4 function)
	initP1 = RooRealVar("initP1","initP1", 0,-1000,1000)
	initP2 = RooRealVar("initP2","initP2", 0,-100,100)
	initP3 = RooRealVar("initP3","initP3", 0,-100,100)
	initBkgP4 = RooGenericPdf("initBkgP4","bkg pdf","pow(1-x/8000.0,initP1)/pow(x/8000.0,initP2+initP3*log(x/8000.))",RooArgList(x,initP1,initP2,initP3))

	signal_P4 = RooDataHist("signal_P4", "signal_P4", quadruplets, h_Signal_P4)
	numSigBkg = RooRealVar("numSigBkg","number of background events in signal", nSigBkg, 0, 2*nSigBkg)
	exPdfSigBkgP4 = RooExtendPdf("sigBkg","background in signal PDF P4 fit", initBkgP4, numSigBkg) 

	exPdfSigBkgP4.fitTo( signal_P4, RooFit.Strategy(2) )


	############ Signal + Bkg Fit
	P1 = RooRealVar("P1","P1", initP1.getValV(),-2*initP1.getValV(), 2*initP1.getValV())
	P2 = RooRealVar("P2","P2", initP2.getValV(),-2*initP2.getValV(), 2*initP2.getValV())
	P3 = RooRealVar("P3","P3", initP3.getValV(),-2*initP3.getValV(), 2*initP3.getValV())
	BkgP4 = RooGenericPdf("BkgP4","bkg pdf","pow(1-x/8000.0,P1)/pow(x/8000.0,P2+P3*log(x/8000.))",RooArgList(x,P1,P2,P3))


	GausMean = RooRealVar("GausMean", "GausMean", st2mass, st2mass-30, st2mass+30)
	GausSigma = RooRealVar("GausSigma", "GausSigma", 10, -100, 100 )#initGausSigma.getValV()-10, initGausSigma.getValV()+10)
	SignalGaus = RooGaussian("SignalGaus", " gaussian pdf", x , GausMean, GausSigma)

	h_Signal = h_initSignal.Clone()
	signal = RooDataHist("signal", "signal", quadruplets, h_Signal)
	model = RooAddPdf("Sig+Bkg", "signal+bkgd", RooArgList( BkgP4, SignalGaus), RooArgList( numSigBkg, numSig ) )
	fitRes = model.fitTo( signal , RooFit.Strategy(2), RooFit.Save() ) 	#### RooFitResults
	fitRes.Print() 		##### RooFitResults

	#### Just to Calculate Gaussian Amplitude (there must be a better way to do it)
	print '################################################################# Calculating Gaussian Amplitude (without RooFit) '
	h_AmpSignal = h_initSignal.Clone()
	P4GausFit= TF1("P4GausFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))+gaus(4)",300,1200);
	P4GausFit.FixParameter( 1, P1.getValV() )
	P4GausFit.FixParameter( 2, P2.getValV() )
	P4GausFit.FixParameter( 3, P3.getValV() )
	P4GausFit.FixParameter( 5, GausMean.getValV() )
	P4GausFit.FixParameter( 6, GausSigma.getValV() )
	h_AmpSignal.Fit(P4GausFit,"B","", 300, 2000)
	print '#################################################################################################################'

	Acceptance = TMath.Sqrt( 2*3.1416 )*( GausSigma.getValV() )*( P4GausFit.GetParameter(4) )
	print ' Acceptance : ', Acceptance
	
	######################################## Workspace
	if ( st2mass == 450 ): XS = listXS[0]
	elif ( st2mass == 550 ): XS = listXS[1]
	elif ( st2mass == 650 ): XS = listXS[2]
	elif ( st2mass == 750 ): XS = listXS[3]
	if ( jes == 'nom' ):
		#### Variables for Workspace
	
		signal = RooDataHist("massRecoDiBjetDiJet_cutDiagStop2bbjj","signal", quadruplets, h_initSignal)
	
		nSig = RooRealVar("nSig","number of signal events",XS*Acceptance*Lumi)
		finalGausMean = RooRealVar("finalGausMean","finalGausMean", st2mass)
		finalGausSigma = RooRealVar("finalGausSigma","finalGausSigma", GausSigma.getValV(), -100,100)
		finalSignalGaus = RooGaussian("finalSignal_pdf","gaussian PDF", x, finalGausMean, finalGausSigma)
		exPdfSignalGaus = RooExtendPdf("signal", "extended signal PDF", finalSignalGaus, nSig)
	
		#### Create Workspace for RooFit              ####
		pWs = RooWorkspace("myWS")
		# import dataset into workspace
		getattr(pWs,'import')(exPdfSignalGaus) # we call it this way because "import" is a reserved word in python
		nSigDataCard = nSig.getVal()
		#save the workspace to a file
		pWs.Print()
		pWs.writeToFile(WSDir+WSname+'.root')

	##### Save Info into file
	saveInfo( WSDir, st2mass, XS, Acceptance, GausMean.getValV(), GausSigma.getValV(),  GausMean.getError(), GausSigma.getError(), jes  )
#	##### Plots
	h_rawSignal = h_initSignal.Clone()
	plotSignalP4Gaus( mass, output, h_rawSignal, 
			P4GausFit.GetParameter(0), P1.getValV(), P2.getValV(), P3.getValV(), 
			P4GausFit.GetParameter(4), GausMean.getValV(), GausSigma.getValV()  )
#	dataFrame = x.frame()
#	signal.plotOn( dataFrame )
#	model.plotOn( dataFrame )
#	model.plotOn( dataFrame, RooFit.Components( 'SignalGaus' ), RooFit.LineStyle(kDashed))
#	model.plotOn( dataFrame, RooFit.Components( 'BkgP4' ), RooFit.LineStyle(kDashed))
#	#signal_P4.plotOn( dataFrame )
#	#exPdfSigBkgP4.plotOn( dataFrame )
#	cDataFitP4 = TCanvas("cDataFitP4","data hist",800,600)
#	#dataFrame.Draw()
#	#h_Signal_Gaussian.Draw()
#	#hSignalIntegral.Draw()
#	h_AmpSignal.Draw()
#	cDataFitP4.SaveAs('test.pdf')
#	print '#############', dataFrame.chiSquare()

	print 'Well done'

##############################################################
#### Save info in a file (not overwrite)                  ####
##############################################################
def saveInfo( WSDir, st2mass , XS, Acceptance, GausMean, GausSigma, GausMeanErr, GausSigmaErr, jes ):
	
	try:
		listName = [ '#Mass', 'XS', 'Acceptance', 'GausMean', 'GausSigma', 'GausMeanErr', 'GausSigmaErr', 'jes' ]
		list = [ str(st2mass), str(XS), str(Acceptance), str(GausMean), str(GausSigma), str(GausMeanErr), str(GausSigmaErr), str(jes) ]

		with open( WSDir+"signalInfo.txt", "a") as myfile:
			if ((st2mass == 450) and ( jes == 'nom') ): 
				myfile.write('#####################################  ')
				myfile.write(datetime.datetime.now().ctime()+'\n') 	### fancy way to keep track of the date
			myfile.write("\t".join(listName)+ '\n')
			myfile.write("\t".join(list)+ '\n')
			
	except IOError:
		print 'File ', filename, 'does not exist.'

##############################################################
#### Ploting Histograms                                   ####
##############################################################
textBox=TLatex(0.10,0.91,"CMS Preliminary Simulation")
textBox.SetNDC()
textBox.SetTextSize(0.05) 
textBox.SetTextColor(50)

textBox3=TLatex(0.70,0.30,"4^{th} jet > 80 GeV ")
textBox3.SetNDC()
textBox3.SetTextSize(0.04) 

textBox4=TLatex(0.70,0.25,"6^{th} jet > 60 GeV ")
textBox4.SetNDC()
textBox4.SetTextSize(0.04) 
	
textBox5=TLatex(0.70,0.20,"#geq 4 bjets")
textBox5.SetNDC()
textBox5.SetTextSize(0.04) 
	
textBox2=TLatex(0.70,0.15,"#Delta = 0 GeV")
textBox2.SetNDC()
textBox2.SetTextSize(0.04) 

def plotSignalP4Gaus( mass, output, histo, p0, p1, p2, p3, g0, g1, g2 ):

	signalP4Gaus= TF1("signalP4Gaus", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))+gaus(4)",350,2000);
	signalP4Gaus.SetParameter( 0, p0 )
	signalP4Gaus.SetParameter( 1, p1 )
	signalP4Gaus.SetParameter( 2, p2 )
	signalP4Gaus.SetParameter( 3, p3 )
	signalP4Gaus.SetParameter( 4, g0 )
	signalP4Gaus.SetParameter( 5, g1 )
	signalP4Gaus.SetParameter( 6, g2 )

	signalP4= TF1("signalP4", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",350,2000);
	signalP4.SetParameter( 0, p0 )
	signalP4.SetParameter( 1, p1 )
	signalP4.SetParameter( 2, p2 )
	signalP4.SetParameter( 3, p3 )
	signalP4.SetLineColor(1)

	signalGaus= TF1("signalGaus", "gaus(0)", g1-200 ,g1+ 200);
	signalGaus.SetParameter( 0, g0 )
	signalGaus.SetParameter( 1, g1 )
	signalGaus.SetParameter( 2, g2 )
	signalGaus.SetLineColor(3)

	gStyle.SetOptStat(0)
	gStyle.SetOptFit(1)
	gStyle.SetStatY(0.9)
	gStyle.SetStatX(0.9)
	gStyle.SetStatW(0.12)
	gStyle.SetStatH(0.12) 

	histo.GetXaxis().SetTitle("Heavier Stop Reconstruction Invariant Mass [GeV]")
	histo.GetYaxis().SetTitle("Quadruplets / 10 GeV")
	histo.GetYaxis().SetTitleOffset(1.2);
	histo.SetTitle("")

	legend=TLegend(0.69,0.43,0.89,0.60)
	legend.SetFillColor(0);
	legend.SetBorderSize(0);
	legend.AddEntry(histo, "jj_"+str(mass)+"_"+str(st1mass), "le")
	legend.AddEntry(signalP4Gaus, "P4 + Gauss", "l")
	legend.AddEntry(signalGaus, "Gauss Only", "l")
	legend.AddEntry(signalP4, "P4 Only", "l")

	c3 = TCanvas('c3', 'c3',  10, 10, 750, 500 )
	histo.Sumw2()
	histo.Draw()
	signalP4.Draw("same")
	signalGaus.Draw("same")
	signalP4Gaus.Draw("same")
	textBox.Draw()
	textBox2.Draw()
	textBox3.Draw()
	textBox4.Draw()
	textBox5.Draw()
	legend.Draw()
	c3.SaveAs(outputDir+output+"_FitP4Gauss.pdf")

if __name__ == '__main__':

	###### Input parameters
	st2mass = [ 450 , 550, 650, 750 ]
	st1mass = 200
	jes = [ 'nom', 'down', 'up']
	histo = 'massRecoDiBjetDiJet_cutDiagStop2bbjj0' 
	folder = 'step3plots1D' 

	for mass in st2mass:

		output = 'jj_'+str(mass)+"_"+str(st1mass)

		for updown in jes:
			####### Output files
			outputWS = 'workspace_'+output
			outputWSDir = "/uscms_data/d3/algomez/files/Stops/Limits/"
			outputDir = "/uscms_data/d3/algomez/files/Stops/FitPlots/"
	
			####### Input files
			if ( 'nom' in updown ):
				inputFile = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_"+str(mass)+"_"+str(st1mass)+"_4jet80_6jet60_plots.root"
				outputPlot = histo+'_'+output
			else:
				inputFile = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_"+str(mass)+"_"+str(st1mass)+"_jes"+updown+"_4jet80_6jet60_plots.root"
				outputPlot = histo+'_'+output+'_jes'+updown
	
			fitP4toData(mass, histo, folder, inputFile, outputDir, outputPlot, outputWS, outputWSDir, updown )
