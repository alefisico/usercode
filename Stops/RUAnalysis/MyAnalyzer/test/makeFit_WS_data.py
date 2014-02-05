#!/usr/bin/env python

###################
### Make Fitting
###################

from ROOT import *
from ROOT import RooFit
import glob,sys
from array import array
ROOT.gSystem.SetIncludePath('-I$ROOFITSYS/include')

gROOT.Reset()
#TVirtualFitter.SetMaxIterations(5000000000)		######### Trick to increase number of iterations

#minuit = TMinuit(1)

##################################################
#### Initial Parameters                       ####
##################################################

####### Bin Info
binSize = 10.0
massBins = [0, 30, 60, 90, 120, 150, 180, 210, 250, 290, 330, 370, 410, 460, 510, 560, 610, 670, 730, 790, 860, 930, 1000, 1080, 1160, 1240, 1330, 1420, 1520, 1620, 1730, 1840, 2000]
FitStart = 350
FitEnd = 2000

####### RooFit Variables
x = RooRealVar("x","Quadruplet Invariant Mass [GeV]",0.,2000.)
quadruplets = RooArgList( x )
x.setRange(FitStart,FitEnd)


#########################################################
#### Fit P4 function to Data and create WorkSpace    ####
#########################################################
def fitP4toData(histo, folder, file, outputDir, WSname, WSDir):

	##### Background (P4 function)
	p1 = RooRealVar("p1","p1", -113.6,-1000,1000) 		########################## This values are from previous fits, but any value works.
	p2 = RooRealVar("p2","p2", 56.39,-100,100)
	p3 = RooRealVar("p3","p3", 8.31,-100,100)
	bkgP4 = RooGenericPdf("bkgP4","bkg pdf","pow(1-x/8000.0,p1)/pow(x/8000.0,p2+p3*log(x/8000.))",RooArgList(x,p1,p2,p3))

	inputFile = TFile(file)
	h_Data = inputFile.Get(folder+'/' + histo)
	data = RooDataHist("data_obs","data", quadruplets, h_Data)
	nData = h_Data.Integral(36,120)
	nBkg = RooRealVar("nBkg","number of background events",nData,0,2*nData)
	exPdfBkgP4 = RooExtendPdf("bkg","background PDF P4 fit",bkgP4,nBkg) 

	#bkgP4.fitTo(data,RooFit.Strategy(2))
	exPdfBkgP4.fitTo(data,RooFit.Strategy(2))

	#### Create Workspace for RooFit              ####
	pWs = RooWorkspace("myWS")
	# import dataset into workspace
	getattr(pWs,'import')(data) # we call it this way because "import" is a reserved word in python
	getattr(pWs,'import')(exPdfBkgP4)
	nBkgDataCard = nBkg.getVal()
	#print nBkgDataCard, nSigDataCard
	#save the workspace to a file
	pWs.writeToFile(WSDir+WSname+'.root')

	#### Plot Data
	plotDataP4(data, exPdfBkgP4, histo, outputDir, WSDir, WSname)

	return p1.getValV(), p2.getValV(), p3.getValV(), p1.getError(), p2.getError(), p3.getError(), nBkg.getValV(), nBkg.getError()



####################################################
#####  Plots  					####
####################################################
textBox=TLatex( 0.10,0.91,"CMS Preliminary 19.5 fb^{-1} at #sqrt{s} = 8 TeV")
textBox.SetNDC()
textBox.SetTextSize(0.05) 
textBox.SetTextColor(50)

def plotDataP4(data, exPdfBkgP4, histo, outputDir, WSDir, WSname):
	##### Test - Plot Data histo
	dataFrame = x.frame()
	dataFrame.SetTitle("")
	dataFrame.GetYaxis().SetTitle("Quadruplets / 10 GeV")
	data.plotOn( dataFrame, RooFit.DataError(RooAbsData.SumW2))
	exPdfBkgP4.plotOn( dataFrame )
	exPdfBkgP4.paramOn( dataFrame, RooFit.Layout(0.65,0.85,0.5) )
	dataFrame.getAttText().SetTextSize(0.0284)
	dataFrame.getAttText().SetTextFont(42)

	leg = TLegend(0.419598,0.5087413,0.8731156,0.8356643,'NULL',"brNDC")
	leg.SetBorderSize(1)
	leg.SetTextFont(42)
	leg.SetLineColor(0)
	leg.SetLineStyle(1)
	leg.SetLineWidth(1)
	leg.SetFillColor(0)
	leg.SetFillStyle(1001)
	leg.SetHeader("#splitline{4^{th} Jet p_{T} > 80 GeV, 6^{th} Jet p_{T} > 60 GeV}{bjets #geq 4, #Delta = 0 GeV}")
	leg.AddEntry(dataFrame.getObject(0),"Data", "lep")    
	leg.AddEntry(dataFrame.getObject(1),"Fit to the data","l")

	cDataFitP4 = TCanvas("cDataFitP4","data hist",800,600)
	dataFrame.Draw()
	leg.Draw()
	textBox.Draw()
	cDataFitP4.SaveAs(outputDir+histo+"_data_FitP4.pdf")
	del cDataFitP4
	c2DataFitP4 = TCanvas("c2DataFitP4","data hist",800,600)
	c2DataFitP4.SetLogy()
	dataFrame.SetMaximum(100)
	dataFrame.SetMinimum(0.1)
	dataFrame.Draw()
	leg.Draw()
	textBox.Draw()
	c2DataFitP4.SaveAs(outputDir+histo+"_data_FitP4_log.pdf")
	del c2DataFitP4
	print '|-------------- ChiSquare :', dataFrame.chiSquare()

	hResidual = RooHist(dataFrame.residHist())
	ResidualFrame = x.frame()
	ResidualFrame.SetTitle("")
	ResidualFrame.GetYaxis().SetTitle("Residuals / 10 GeV")
	ResidualFrame.addPlotable(hResidual,"P")
	cResidual = TCanvas("cResidual","Residual",800,600)
	ResidualFrame.Draw()
	textBox.Draw()
	cResidual.SaveAs(outputDir+histo+"_data_FitP4_Residual.pdf")
	del cResidual

	hPull = RooHist(dataFrame.pullHist())
	PullFrame = x.frame()
	PullFrame.SetTitle("")
	PullFrame.GetYaxis().SetTitle("Pulls / 10 GeV")
	PullFrame.addPlotable(hPull,"P")
	cPull = TCanvas("cPull","Pull",800,600)
	PullFrame.Draw()
	textBox.Draw()
	cPull.SaveAs(outputDir+histo+"_data_FitP4_Pull.pdf")
	del cPull

	### Save histos in file
	outputrootFile = TFile(WSDir+WSname+'.root','UPDATE')
	dataFrame.Write()
	hPull.Write()
	hResidual.Write()
	outputrootFile.Close()


#########################################################
#####  Plot P4 function in Resolution Based Binning  ####
#########################################################
def plotResoBasedBin(file, h, folder, p1, p2, p3, p1Err, p2Err, p3Err, nBkg, nBkgErr):

	histo = h + '_resoBasedBin'
	inputFile = TFile(file)
	h_Initial = inputFile.Get(folder+'/' + histo )
	hFit = h_Initial.Clone(histo)
	

	######### Fit Functions
	P4Fit = TF1("P4Fit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",350,2000)
	##print hFit.GetMaximumBin(), PreFitStart, PreFitEnd
	p0 = nBkg/binSize/100
	p0Err = nBkgErr/binSize/100

	############################### Stop2
	P4Fit.FixParameter(1,p1)
	P4Fit.FixParameter(2,p2)
	P4Fit.SetParError(2,p2Err)
	P4Fit.FixParameter(3,p3)
	P4Fit.SetParError(3,p3Err)
	hFit.Fit(P4Fit,'B',"",FitStart,FitEnd)
	P4Fit.SetParError(1,p1Err)
	#hFit.Fit(P4Fit,"MRI","",FitStart,FitEnd)

	################################ Pull and Residual Distro
	alpha = 1 - 0.6827

	hPull = h_Initial.Clone()
	hResidual = h_Initial.Clone()

	P4 = TF1("P4", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",350,2000)
	P4.SetParameter(0,P4Fit.GetParameter(0))
	P4.SetParameter(1,p1)
	P4.SetParameter(2,p2)
	P4.SetParameter(3,p3)
	
	listBinMass = []
	listCorrectP4 = []
	for bin in range(0, len(massBins)-1):
		hPull.SetBinContent(bin, 0.)
		hPull.SetBinError(bin, 0.)
		hResidual.SetBinContent(bin+1, 0.)
		hResidual.SetBinError(bin+1, 0.)
		
		N = (h_Initial.GetBinContent(bin))
		binMiddlepoint = (massBins[bin] + massBins[bin+1])/2
		valIntegral = (P4.Eval(binMiddlepoint))
		print N, massBins[bin], massBins[bin+1], valIntegral, (massBins[bin] + massBins[bin+1])/2

		if ( massBins[bin] > FitStart ):
			sizeBin = massBins[bin+1] - massBins[bin]
			correctValP4 = valIntegral*binSize/sizeBin
			print '|-------------', bin, massBins[bin], sizeBin, correctValP4



		errDiff = sqrt(N)
		diff = (N - valIntegral)/ valIntegral
		#print N, errDiff
		if (( massBins[bin] >= FitStart) and (N != 0) and ( massBins[bin] <= FitEnd) ):
		#if (N != 0):
			pull = (N - valIntegral)/ errDiff
			#print bin*massBins[bin], N, valIntegral, errDiff, pull
			hPull.SetBinContent(bin, pull)
			hPull.SetBinError(bin, 1.0)
	
		#print diff
		#if (( ibin >= FitStart/massBins[ibin]) and (N != 0) and (ibin <= FitEnd/massBins[ibin])):
			hResidual.SetBinContent(bin, diff)
			hResidual.SetBinError(bin, errDiff/valIntegral)
	############################################################################### 

	##### Histo to save
	h_data = hFit.Clone()
	h_QCDP4 = P4Fit.Clone()    
			
	###### Formatting Plots 
	hFit.SetTitle("")
	hFit.GetXaxis().SetTitle("Heavy stop reconstruction invariant mass [GeV]")
	#hFit.GetYaxis().SetTitle("Dijets/10 GeV")
	hFit.GetYaxis().SetTitle("dN/dM_{bbjj} [GeV^{-1}]")
	hFit.GetYaxis().SetTitleOffset(1.2)

	hPull.SetTitle("")
	hPull.GetXaxis().SetTitle("Heavy stop reconstruction invariant mass [GeV]")
	hPull.GetYaxis().SetTitle("Pull Distribution")
	#hPull.GetYaxis().SetTitleSize(0.1)
	#hPull.GetYaxis().SetTitleOffset(1.2)
	hPull.SetMarkerStyle(7)

	hResidual.SetTitle("")
	hResidual.GetXaxis().SetTitle("Heavy stop reconstruction invariant mass [GeV]")
	hResidual.GetYaxis().SetTitle("Residuals")
	#hResidual.GetYaxis().SetTitleSize(0.1)
	#hResidual.GetYaxis().SetTitleOffset(1.2)
	hResidual.SetMarkerStyle(7)
	hResidual.GetYaxis().SetLimits(-5,5)
	hResidual.SetMaximum(1)

	######## Legends and text
	legend=TLegend(0.25,0.15,0.45,0.25)
	legend.SetFillColor(0)
	legend.SetBorderSize(0)
	legend.AddEntry(hFit,"Data", "l")
	legend.AddEntry(P4Fit,"P4 parameter fit", "l")    
	  
	textBox3=TLatex(0.70,0.60,"4^{th} jet > 80 GeV ")
	textBox3.SetNDC()
	textBox3.SetTextSize(0.04) 
	
	textBox4=TLatex(0.70,0.55,"6^{th} jet > 60 GeV ")
	textBox4.SetNDC()
	textBox4.SetTextSize(0.04) 
	
	textBox5=TLatex(0.70,0.50,"#geq 4 bjets")
	textBox5.SetNDC()
	textBox5.SetTextSize(0.04) 
	
	if "cutDiag" in histo:
		textBox2=TLatex(0.70,0.45,"#Delta = 0 GeV")
		textBox2.SetNDC()
		textBox2.SetTextSize(0.04) 

	xline = array('d', [0,2000])
	yline = array('d', [0,0])
	line = TGraph(2, xline, yline)
	line.SetLineColor(kRed)

	######### Plotting Histograms
	c1 = TCanvas('c1', 'c1',  10, 10, 750, 1000 )
	pad1 = TPad("pad1", "Fit",0,0.50,1.00,1.00,-1)
	pad2 = TPad("pad2", "Pull",0,0.25,1.00,0.50,-1)
	pad3 = TPad("pad3", "Residual",0,0,1.00,0.25,-1)
	pad1.Draw()
	pad2.Draw()
	pad3.Draw()
	
	pad1.cd()
	pad1.SetLogy()
	gStyle.SetOptFit(1102)
	gStyle.SetStatY(0.9)
	gStyle.SetStatX(0.9)
	gStyle.SetStatW(0.15)
	gStyle.SetStatH(0.13) 
	hFit.Sumw2()
	hFit.Draw("L")
	#P4Fit.Draw("same")
	legend.Draw()
	textBox.Draw()
	textBox3.Draw()
	textBox4.Draw()
	textBox5.Draw()
	textBox2.Draw()
	
	pad2.cd()
	gStyle.SetOptStat(0)
	hPull.GetXaxis().SetTitle("")
	hPull.GetYaxis().SetLabelSize(0.08)
	hPull.GetXaxis().SetLabelSize(0.08)
	hPull.GetYaxis().SetTitleSize(0.06)
	hPull.GetYaxis().SetTitleOffset(0.6)
	hPull.Sumw2()
	hPull.Draw("E")
	line.Draw("L")
	
	pad3.cd()
	gStyle.SetOptStat(0)
	hResidual.GetXaxis().SetTitle("")
	hResidual.GetYaxis().SetLabelSize(0.08)
	hResidual.GetXaxis().SetLabelSize(0.08)
	hResidual.GetYaxis().SetTitleSize(0.06)
	hResidual.GetYaxis().SetTitleOffset(0.6)
	#hResidual.Sumw2()
	hResidual.Draw("e")
	line.Draw("L")
	
	c1.SaveAs(outputDir+histo+"_data_FitP4Gauss.pdf")
	c1.SaveAs(outputDir+histo+"_data_FitP4Gauss.png")
	del c1
	
	c2 = TCanvas('c2', 'c2',  10, 10, 750, 500 )
	gStyle.SetOptStat(0)
	hPull.Sumw2()
	hPull.Draw("e")
	line.Draw("L")
	textBox.Draw("same")
	c2.SaveAs(outputDir+histo+"_data_FitP4Gauss_Pull.pdf")
	c2.SaveAs(outputDir+histo+"_data_FitP4Gauss_Pull.png")
	del c2
	
	c3 = TCanvas('c3', 'c3',  10, 10, 750, 500 )
	gStyle.SetOptStat(0)
	#hResidual.Sumw2()
	hResidual.Draw("e")
	line.Draw("L")
	textBox.Draw("same")
	c3.SaveAs(outputDir+histo+"_data_FitP4Gauss_Residual.pdf")
	del c3
	
	c4 = TCanvas('c4', 'c4',  10, 10, 750, 500 )
	gStyle.SetOptFit()
	gStyle.SetStatY(0.9)
	gStyle.SetStatX(0.9)
	gStyle.SetStatW(0.15)
	gStyle.SetStatH(0.15) 
	c4.SetLogy()
	hFit.Sumw2()
	hFit.Draw()
	textBox.Draw("same")
	
	
	c4.SaveAs(outputDir+histo+"_data_FitP4Gauss_FitOnly.pdf")
	legend.Draw()
	textBox.Draw()
	textBox2.Draw()
	textBox3.Draw()
	textBox4.Draw()
	textBox5.Draw()
	del c4

if __name__ == '__main__':

	###### Input parameters
	histo = 'massRecoDiBjetDiJet_cutDiagStop2bbjj0' #str ( sys.argv[1] )
	folder = 'step3plots1D' #str ( sys.argv[2] )

	####### Input files
	inputFile = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_6jet60_plots.root" 

	####### Output files
	outputWS = 'workspace_dataPlusP4'
	outputWSDir = "/uscms_data/d3/algomez/files/Stops/Limits/"
	outputDir = "/uscms_data/d3/algomez/files/Stops/FitPlots/"

	p1fitValue, p2fitValue, p3fitValue, p1ErrValue, p2ErrValue, p3ErrValue, nEvents, nEventsErr = fitP4toData(histo, folder, inputFile, outputDir, outputWS, outputWSDir)
	if ( sys.argv[1] == 'resoBin' ) : plotResoBasedBin( inputFile, histo, folder, p1fitValue, p2fitValue, p3fitValue, p1ErrValue, p2ErrValue, p3ErrValue, nEvents, nEventsErr )



