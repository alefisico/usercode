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
st1mass = int (sys.argv[3])
st2mass = int (sys.argv[4])


####### Input files
input1 = "/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_6jet60_plots_v1.root"   ### v1 bin 10
input2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_"+str(st2mass)+"_"+str(st1mass)+"_4jet80_6jet60_plots.root"
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
f2 = TFile(input2)

h1 = f1.Get(folder+'/' + hist1)
h2 = f2.Get(folder+'/' + hist1)

hFit = h1.Clone(hist1)

######## Fit Functions
P4PreFit = TF1("P4PreFit", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",0,2000);

############################## Stop2
PreFitStart= hFit.GetMaximumBin()*binSize-10
PreFitEnd  = hFit.GetMaximumBin()*binSize+1000
#print PreFitStart, PreFitEnd
#P4PreFit.SetParameter(0,0.1)
#P4PreFit.SetParameter(1,-5)
#P4PreFit.SetParameter(2,16)
#	P4PreFit.SetParameter(3, 1)
hFit.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)
#hFit.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)
#hFit.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)
#hFit.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)
#hFit.Fit(P4PreFit,"MRI","",PreFitStart,PreFitEnd)
#print minuit.fCstatu.Data()


########### Create P4 with random numbers
if (st2mass == 250): XSample = 5.57596 
if (st2mass == 350): XSample = 0.807323
if (st2mass == 450): XSample = 0.169668
if (st2mass == 550): XSample = 0.0452067
if (st2mass == 650): XSample = 0.0139566
if (st2mass == 750): XSample = 0.00480639
signalweight = 19500 * XSample/100000

gRandom.SetSeed(0)	### Change Random seed

hP4 = h1.Clone()
hx2 = h1.Clone()
for bin in range(0, 200):
	hP4.SetBinContent(bin, 0.)
	hP4.SetBinError(bin, 0.)
	hx2.SetBinContent(bin, 0.)
	hx2.SetBinError(bin, 0.)

P4 = TF1("P4", "[0]*pow(1-x/8000.0,[1])/pow(x/8000.0,[2]+[3]*log(x/8000.))",0,2000);
x2 = TF1("x2", "log(x)",0,2000);
P4.FixParameter(0, P4PreFit.GetParameter(0))
P4.FixParameter(1, P4PreFit.GetParameter(1))
P4.FixParameter(2, P4PreFit.GetParameter(2))
P4.FixParameter(3, P4PreFit.GetParameter(3))

Nentries = int(h1.GetEntries())
x1 = TF1("x1", "x", Nentries-sqrt(Nentries), Nentries+sqrt(Nentries))
randomNentries = int(x1.GetRandom())
#randomNentries = 1600
print Nentries, randomNentries
#for i in range(0, 10000):
for i in range(0, randomNentries):
	#print int(P4.GetRandom())
	hP4.Fill(int(P4.GetRandom()))
	hx2.Fill(x2.GetRandom())
hSignal = h2.Clone(hist1)
hSignal1 = h2.Clone(hist1)
hSignal.Scale( signalweight )
hSignal1.Scale( signalweight )
hSignal.SetFillColor(2)
hSignal.SetLineColor(2)
hSignal1.SetLineColor(2)

hs = THStack( "hs", "hs" )
hs.Add( hP4 )
hs.Add( hSignal )
		
######### Plotting Histograms

c1 = TCanvas('c1', 'c1',  10, 10, 750, 500 )
hP4.SetTitle("")
hP4.GetXaxis().SetTitle("Invariant Mass [GeV]")
##hP4.GetYaxis().SetTitle("Dijets/10 GeV")
hP4.GetYaxis().SetTitle("Quadjets / 10 GeV")
hP4.GetYaxis().SetTitleOffset(1.2);
print hP4.Integral()

gStyle.SetOptStat(0)
#gStyle.SetStatY(0.9)
#gStyle.SetStatX(0.9)
#gStyle.SetStatW(0.15)
#gStyle.SetStatH(0.15) 
hP4.Sumw2()
hP4.Draw()
P4.Draw("same")


textBox=TLatex()
textBox.SetNDC()
textBox.SetTextSize(0.05) 
textBox.SetTextColor(50)
textBox.DrawLatex(0.10,0.91,"CMS Preliminary PseudoExperiment Simulation ")

textBox1=TLatex()
textBox1.SetNDC()
#textBox1.SetTextSize(0.05) 
#textBox1.SetTextColor(50)
textBox1.DrawLatex(0.70,0.80,"Entries "+str(randomNentries))

c1.SaveAs(outputDir+hist1+"_PSE_P4Only.pdf")
del c1

c2 = TCanvas('c2', 'c2',  10, 10, 750, 500 )
hx2.Draw()
hx2.SetTitle("")
hx2.GetXaxis().SetTitle("Invariant Mass [GeV]")
##hP4.GetYaxis().SetTitle("Dijets/10 GeV")
hx2.GetYaxis().SetTitle("Quadjets / 10 GeV")
hx2.GetYaxis().SetTitleOffset(1.2);
x2.Draw("same")

legend=TLegend(0.75,0.8,0.90,0.9)
legend.SetFillColor(0);
legend.AddEntry(hx2, "log(x)", "l")
legend.Draw()

gStyle.SetOptStat(0)

textBox=TLatex()
textBox.SetNDC()
textBox.SetTextSize(0.05) 
textBox.SetTextColor(50)
textBox.DrawLatex(0.10,0.91,"CMS Preliminary PseudoExperiment Simulation ")

c2.SaveAs(outputDir+hist1+"_PSE_check.pdf")
del c2

c3 = TCanvas('c3', 'c3',  10, 10, 750, 500 )
hs.Draw("hist")
hs.SetTitle("")
hs.GetXaxis().SetTitle("Invariant Mass [GeV]")
##hP4.GetYaxis().SetTitle("Dijets/10 GeV")
hs.GetYaxis().SetTitle("Quadjets / 10 GeV")
hs.GetYaxis().SetTitleOffset(1.2);
hSignal1.Draw("histsame")

legend=TLegend(0.70,0.75,0.90,0.9)
legend.SetFillColor(0);
legend.AddEntry(hP4, "Background P4", "l")
legend.AddEntry(hSignal, 'jj_'+str(st2mass)+"_"+str(st1mass), "l")    
legend.Draw()

gStyle.SetOptStat(0)
#gStyle.SetStatY(0.9)
#gStyle.SetStatX(0.9)
#gStyle.SetStatW(0.15)
#gStyle.SetStatH(0.15) 
#hP4.Sumw2()
#hP4.Draw()


textBox=TLatex()
textBox.SetNDC()
textBox.SetTextSize(0.05) 
textBox.SetTextColor(50)
textBox.DrawLatex(0.10,0.91,"CMS Preliminary PseudoExperiment Simulation ")

textBox1=TLatex()
textBox1.SetNDC()
#textBox1.SetTextSize(0.05) 
#textBox1.SetTextColor(50)
textBox1.DrawLatex(0.70,0.70,"Entries "+str(randomNentries))

c3.SaveAs(outputDir+hist1+"_PSE_jj_"+str(st2mass)+"_"+str(st1mass)+".pdf")
del c3
