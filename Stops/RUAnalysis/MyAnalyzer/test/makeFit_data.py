#!/usr/bin/env python

###################
### Make Fitting
###################

from ROOT import *
import glob,sys

gROOT.Reset()
TVirtualFitter.SetMaxIterations(5000000000)		######### Trick to increase number of iterations

#minuit = TMinuit(1)

###### Input parameters
hist1 = str ( sys.argv[1] )
folder = str ( sys.argv[2] )


####### Input files
input1 = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_6jet60_plots.root"   ### v1 bin 10
outputDir = "/uscms_data/d3/algomez/files/Stops/Results/Plots/"

########### Integral Funcion
sqrtS = 8000
def INTEGRAL(x0, xf, par0, par1, par2, par3):
	#double xs=par[0];
	#double lumi=par[1];
	#double jes=par[2];
	#double jer=par[3];
	p0 = par0
	p1 = par1
	p2 = par2
	p3 = par3
	#double n[NBKGPARS] = {0.};
	#n[0]=par[8];
	#n[1]=par[9];
	#n[2]=par[10];
	#n[3]=par[11];

	#if( COV_MATRIX[0+shift][0+shift]>0. && (n[0]!=0. || n[1]!=0. || n[2]!=0. || n[3]!=0.) ) {
	#	double g[NBKGPARS] = {0.};
	#	for(int v=0; v<NBKGPARS; ++v) {
	#		for(int k=0; k<NBKGPARS; ++k) g[k]=n[v]*eigenValues(v)*eigenVectors[k][v];
	#		p0 += g[0];
	#		p1 += g[1];
	#		p2 += g[2];
	#		p3 += g[3];
	#	}
	#}

	## uses Simpson's 3/8th rule to compute the background integral over a short interval
	## also use a power series expansion to determine the intermediate intervals since the pow() call is expensive

	dx = (xf-x0)/3./sqrtS
	x = x0/sqrtS
	logx = log(x)

	if (x != 0): 
		a = pow(1-x,p1)/pow(x,p2+p3*logx)				# P4 Function
		b = dx*a/x/(x-1)*(p2+p1*x-p2*x-2*p3*(x-1)*logx)		#/// Derivative of a
		c = 0.5*dx*dx*a*( (p1-1)*p1/(x-1)/(x-1) - 2*p1*(p2+2*p3*logx)/(x-1)/x + (p2+p2*p2-2*p3+2*p3*logx*(1+2*p2+2*p3*logx))/x/x);	#// Second order derivative
		d = 0.166666667*dx*dx*dx*a*( (p1-2)*(p1-1)*p1/(x-1)/(x-1)/(x-1) - 3*(p1-1)*p1*(p2+2*p3*logx)/(x-1)/(x-1)/x - (1+p2+2*p3*logx)*(p2*(2+p2) - 6*p3 + 4*p3*logx*(1+p2*p3*logx))/x/x/x + 3*p1*(p2+p2*p2-2*p3+2*p3*logx*(1+2*p2+2*p3*logx))/(x-1)/x/x )				# Third order?

		bkg = (xf-x0)*p0*(a+0.375*(b+c+d)+0.375*(2*b+4*c+8*d)+0.125*(3*b+9*c+27*d))
		if(bkg<0.): bkg=1e-7
	else:
		bkg = 1e-7

	#if(xs==0.0): return bkg
	return bkg

	#xprimef = jes*(jer*(xf-SIGMASS)+SIGMASS);
	#xprime0 = jes*(jer*(x0-SIGMASS)+SIGMASS);
	#bin1 = HISTCDF->GetXaxis()->FindBin(xprimef);
	#bin2 = HISTCDF->GetXaxis()->FindBin(xprime0);
	#if(bin1<1) bin1=1;
	#if(bin1>HISTCDF->GetNbinsX()) bin1=HISTCDF->GetNbinsX();
	#if(bin2<1) bin2=1;
	#if(bin2>HISTCDF->GetNbinsX()) bin2=HISTCDF->GetNbinsX();
	#double sig=xs*lumi*(HISTCDF->GetBinContent(bin1)-HISTCDF->GetBinContent(bin2));

	#return bkg+sig

########################## end Integral


binSize = 10
#print "Input files: ", input1

f1 = TFile(input1)

h1 = f1.Get(folder+'/' + hist1)

hFit = h1.Clone(hist1)

######## Fit Functions
P4PreFit = TF1("P4PreFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",0,2000);

#print hFit.GetMaximumBin(), PreFitStart, PreFitEnd

########################### Stop1
#if not "cutDiag" in hist1:
#	PreFitStart= hFit.GetMaximumBin()*binSize-10
#	PreFitEnd  = hFit.GetMaximumBin()*binSize+1000
#	P4PreFit.SetParameter(0, 0.0002)
#	P4PreFit.SetParameter(1, -1.44)
#	P4PreFit.SetParameter(2, 7)
#	P4PreFit.SetParameter(3, 0.7)
#else:
#	PreFitStart= hFit.GetMaximumBin()*binSize-30
#	PreFitEnd  = hFit.GetMaximumBin()*binSize+1000
#	P4PreFit.SetParameter(0,1)
#	P4PreFit.SetParameter(1,50)
#	P4PreFit.SetParameter(2, 6)
#	P4PreFit.SetParameter(3, 1)
######################################################

############################## Stop2
#PreFitStart= hFit.GetMaximumBin()*binSize-100
PreFitStart= hFit.GetMaximumBin()*binSize-10
PreFitEnd  = hFit.GetMaximumBin()*binSize+1000
#print PreFitStart, PreFitEnd
#P4PreFit.SetParameter(0,1)
#P4PreFit.SetParameter(1,-100)
#P4PreFit.SetParameter(2,16)
#	P4PreFit.SetParameter(3, 1)
hFit.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)
#hFit.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)
#print minuit.fCstatu.Data()

############################### Pull and Residual Distro
alpha = 1 - 0.6827

hPull = h1.Clone()
hResidual = h1.Clone()

for bin in range(0, 200):
	hPull.SetBinContent(bin, 0.)
	hPull.SetBinError(bin, 0.)
	hResidual.SetBinContent(bin, 0.)
	hResidual.SetBinError(bin, 0.)

for ibin in range(1, hFit.GetNbinsX()):

	#N = h1.GetBinContent(ibin)*binSize
	N = h1.GetBinContent(ibin)
	#l = 0.5*TMath.ChisquareQuantile(alpha/2,2*N)
	#h = 0.5*TMath.ChisquareQuantile(1-alpha/2,2*(N+1))
	#el = N - l 
	#eh = h - N
	#print N, l, h, el, eh, err, valueP4

	#x0 = h1.GetBinLowEdge(ibin)
	#xf = h1.GetBinLowEdge(ibin+1)
	#print ibin, x0, xf
	#valIntegral = (INTEGRAL( x0, xf, P4PreFit.GetParameter(0), P4PreFit.GetParameter(1), P4PreFit.GetParameter(2), P4PreFit.GetParameter(3) ))/binSize
	#print valIntegral
	p0 = P4PreFit.GetParameter(0)
	p1 = P4PreFit.GetParameter(1)
	p2 = P4PreFit.GetParameter(2)
	p3 = P4PreFit.GetParameter(3)
	P4 = TF1("P4", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",0,2000);
	P4.SetParameter(0,p0)
	P4.SetParameter(1,p1)
	P4.SetParameter(2,p2)
	P4.SetParameter(3,p3)
	valIntegral = P4.Eval((ibin*binSize)+5)

	#err = (el+ eh)/2
	#if (N >= valIntegral): err = el
	#if (N < valIntegral): err = eh

	errDiff = sqrt(N)
	#print N, errDiff
	if (( ibin >= PreFitStart/binSize) and (N != 0) and (ibin <= PreFitEnd/binSize)):
		pull = (N - valIntegral)/ errDiff
		print ibin*binSize, N, valIntegral, errDiff, pull
		hPull.SetBinContent(ibin, pull)
		hPull.SetBinError(ibin, 1.0)

	diff = (N - valIntegral)/ valIntegral
	#print diff
	if (( ibin >= PreFitStart/binSize) and (N != 0) and (ibin <= PreFitEnd/binSize)):
		hResidual.SetBinContent(ibin, diff)
		hResidual.SetBinError(ibin, errDiff/valIntegral)
############################################################################### 

		
######### Plotting Histograms
c1 = TCanvas('c1', 'c1',  10, 10, 750, 1000 )
pad1 = TPad("pad1", "Fit",0,0.50,1.00,1.00,-1)
pad2 = TPad("pad2", "Pull",0,0.25,1.00,0.50,-1);
pad3 = TPad("pad3", "Residual",0,0,1.00,0.25,-1);
pad1.Draw()
pad2.Draw()
pad3.Draw()

pad1.cd()
gStyle.SetOptFit()
gStyle.SetStatY(0.9)
gStyle.SetStatX(0.9)
gStyle.SetStatW(0.15)
gStyle.SetStatH(0.15) 
hFit.SetTitle("")
hFit.GetXaxis().SetTitle("Invariant Mass [GeV]")
#hFit.GetYaxis().SetTitle("Dijets/10 GeV")
hFit.GetYaxis().SetTitle("Quadjets / 10 GeV")
hFit.GetYaxis().SetTitleOffset(1.2);
hFit.Sumw2()
hFit.Draw()

textBox=TLatex()
textBox.SetNDC()
textBox.SetTextSize(0.05) 
textBox.SetTextColor(50)
textBox.DrawLatex(0.10,0.91,"CMS Preliminary 19.5 fb^{-1} at #sqrt{s} = 8 TeV")

textBox1=TLatex()
textBox1.SetNDC()
textBox1.SetTextSize(0.04) 
textBox1.SetTextColor(50)
textBox1.DrawText(0.56,0.85,"DATA")

pad2.cd()
gStyle.SetOptStat(0)
hPull.SetTitle("Pull Distribution")
hPull.GetXaxis().SetTitle("Invariant Mass [GeV]")
hPull.GetYaxis().SetTitle("Pulls")
#hPull.GetYaxis().SetTitleSize(0.1)
#hPull.GetYaxis().SetTitleOffset(1.2);
hPull.SetMarkerStyle(7)
hPull.Sumw2()
hPull.Draw("e")
line = TLine(0,0,2000,0)
line.SetLineColor(kRed)
line.Draw("same")

pad3.cd()
gStyle.SetOptStat(0)
hResidual.SetTitle("Residual Distribution")
hResidual.GetXaxis().SetTitle("Invariant Mass [GeV]")
hResidual.GetYaxis().SetTitle("Residuals")
#hResidual.GetYaxis().SetTitleSize(0.1)
#hResidual.GetYaxis().SetTitleOffset(1.2);
hResidual.SetMarkerStyle(7)
#hResidual.SetMinimum(-1)
#hResidual.Sumw2()
hResidual.Draw("e")
line2 = TLine(0,0,2000,0)
line2.SetLineColor(kRed)
line2.Draw("same")

c1.SaveAs(outputDir+hist1+"_data_FitP4Gauss.pdf")
del c1

c2 = TCanvas('c2', 'c2',  10, 10, 750, 500 )
gStyle.SetOptStat(0)
hPull.SetTitle("Pull Distribution")
hPull.GetXaxis().SetTitle("Invariant Mass [GeV]")
hPull.GetYaxis().SetTitle("Pulls")
#hPull.GetYaxis().SetTitleSize(0.1)
#hPull.GetYaxis().SetTitleOffset(1.2);
hPull.SetMarkerStyle(7)
hPull.Sumw2()
hPull.Draw("e")
line = TLine(0,0,2000,0)
line.SetLineColor(kRed)
line.Draw("same")

c2.SaveAs(outputDir+hist1+"_data_FitP4Gauss_Pull.pdf")
del c2

c3 = TCanvas('c3', 'c3',  10, 10, 750, 500 )

gStyle.SetOptStat(0)
hResidual.SetTitle("Residual Distribution")
hResidual.GetXaxis().SetTitle("Invariant Mass [GeV]")
hResidual.GetYaxis().SetTitle("Residuals")
#hResidual.GetYaxis().SetTitleSize(0.1)
#hResidual.GetYaxis().SetTitleOffset(1.2);
hResidual.SetMarkerStyle(7)
hResidual.GetYaxis().SetLimits(-5,5)
hResidual.SetMaximum(1)
#hResidual.Sumw2()
hResidual.Draw("e")
line2 = TLine(0,0,2000,0)
line2.SetLineColor(kRed)
line2.Draw("same")

c3.SaveAs(outputDir+hist1+"_data_FitP4Gauss_Residual.pdf")
del c3

c4 = TCanvas('c4', 'c4',  10, 10, 750, 500 )
gStyle.SetOptFit()
gStyle.SetStatY(0.9)
gStyle.SetStatX(0.9)
gStyle.SetStatW(0.15)
gStyle.SetStatH(0.15) 
hFit.SetTitle("")
hFit.GetXaxis().SetTitle("Invariant Mass [GeV]")
#hFit.GetYaxis().SetTitle("Dijets/10 GeV")
hFit.GetYaxis().SetTitle("Quadjets / 10 GeV")
hFit.GetYaxis().SetTitleOffset(1.2)
hFit.Sumw2()
hFit.Draw()

textBox=TLatex()
textBox.SetNDC()
textBox.SetTextSize(0.05) 
textBox.SetTextColor(50)
textBox.DrawLatex(0.10,0.91,"CMS Preliminary 19.5 fb^{-1} at #sqrt{s} = 8 TeV")

textBox1=TLatex()
textBox1.SetNDC()
textBox1.SetTextSize(0.04) 
textBox1.SetTextColor(50)
textBox1.DrawText(0.56,0.85,"DATA")

c4.SaveAs(outputDir+hist1+"_data_FitP4Gauss_FitOnly.pdf")
del c4


