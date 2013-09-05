#include <iostream>
#include <cmath>
#include <cassert>
#include <sstream>

#include <TGraph.h>
#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TMath.h>
#include <TROOT.h>
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>

#include "binneddata.hh"
#include "fit.hh"
#include "statistics.hh"

using namespace std;

//##################################################################################################################################
// User Section 1
//
// (Change the code outside the User Sections only if you know what you are doing)
//

////////////////////////////////////////////////////////////////////////////////
// magic numbers
////////////////////////////////////////////////////////////////////////////////

// number of pseudoexperiments (when greater than 0, expected limit with +/- 1 and 2 sigma bands is calculated)
const int NPES=200; // 200 (the more pseudo-experiments, the better. However, 200 is a reasonable choice)

// number of samples of nuisance parameters for Bayesian MC integration (when greater than 0, systematic uncertanties are included in the limit calculation)
const int NSAMPLES=0; // 5000 (larger value is better but it also slows down the code. 5000 is a reasonable compromise between the speed and precision)

// alpha (1-alpha=confidence interval)
const double ALPHA=0.05;

// left side tail
const double LEFTSIDETAIL=0.0;

// center-of-mass energy
const double sqrtS = 8000.;

// histogram binning
//const int NBINS=38;
/*double BOUNDARIES[NBINS+1] = {  890,  944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530,
                               1607, 1687, 1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546,
                               2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010,
                               4171, 4337, 4509, 4686, 4869, 5058 };*/
/*   STOP 1  */
/*const int NBINS=93;
double BOUNDARIES[NBINS+1] = { 	       70,  80,  90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 
	210, 220, 230, 240, 250, 260, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360, 370, 380, 390, 400, 
	410, 420, 430, 440, 450, 460, 470, 480, 490, 500, 510, 520, 530, 540, 550, 560, 570, 580, 590, 600, 
	610, 620, 630, 640, 650, 660, 670, 680, 690, 700, 710, 720, 730, 740, 750, 760, 770, 780, 790, 800, 
	810, 820, 830, 840, 850, 860, 870, 880, 890, 900, 910, 920, 930, 940, 950, 960, 970, 980, 990, 1000 }; */


const int NBINS=60;
double BOUNDARIES[NBINS+1] = { 400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 500, 510, 520, 530, 540, 550, 560, 570, 580, 590, 600, 
		 		610, 620, 630, 640, 650, 660, 670, 680, 690, 700, 710, 720, 730, 740, 750, 760, 770, 780, 790, 800, 
				810, 820, 830, 840, 850, 860, 870, 880, 890, 900, 910, 920, 930, 940, 950, 960, 970, 980, 990, 1000 };
/*				1010, 1020, 1030, 1040, 1050, 1060, 1070, 1080, 1090, 1100, 1110, 1120, 1130, 1140, 1150, 1160, 1170, 1180, 1190, 1200,
				1210, 1220, 1230, 1240, 1250, 1260, 1270, 1280, 1290, 1300, 1310, 1320, 1330, 1340, 1350, 1360, 1370, 1380, 1390, 1400,
				1410, 1420, 1430, 1440, 1450, 1460, 1470, 1480, 1490, 1500, 1510, 1520, 1530, 1540, 1550, 1560, 1570, 1580, 1590, 1600,
				1610, 1620, 1630, 1640, 1650, 1660, 1670, 1680, 1690, 1700, 1710, 1720, 1730, 1740, 1750, 1760, 1770, 1780, 1790, 1800,
				1810, 1820, 1830, 1840, 1850, 1860, 1870, 1880, 1890, 1900, 1910, 1920, 1930, 1940, 1950, 1960, 1970, 1980, 1990, 2000 }; */

// parameters
double SIGMASS=0;
const int NPARS=12;
const int NBKGPARS=4;
const int POIINDEX=0; // which parameter is "of interest"  "cross section"
const char* PAR_NAMES[NPARS]    = { "xs", "lumi",  "jes", "jer",        "p0",        "p1",        "p2",         "p3", "n0", "n1", "n2", "n3" };
      //double PAR_GUESSES[NPARS] = { 1E-6,  4976.,    1.0,   1.0, 3.27713e-01, 8.33753e+00, 5.37123e+00,  4.05975e-02,    0,    0,    0,    0 };
      //double PAR_GUESSES[NPARS] = { 1E-6, 20000.,    1.0,   1.0, 2.18518e-04, -1.44935e-01, 6.91955e+00, 7.29084e-01,    0,    0,    0,    0 };  /// STOP1
      //double PAR_GUESSES[NPARS] = { 1E-6, 20000.,    1.0,   1.0, 2.77210e-06, 5.59013e+00, 8.86246e+00, 1.06444e+00,    0,    0,    0,    0 };  /// STOP1
      double PAR_GUESSES[NPARS] = { 1E-6, 19500.,    1.0,   1.0, 2.10567e+06, 8.16039e+01, -1.43295e+00, 2.43521e-01,    0,    0,    0,    0 };
      double PAR_MIN[NPARS]     = {    0,    0.0,    0.0,   0.0,       -9999,       -9999,       -9999,        -9999, -100, -100, -100, -100 };
const double PAR_MAX[NPARS]     = {  1E2,  6000.,    2.0,   2.0,        9999,        9999,        9999,         9999,  100,  100,  100,  100 };
      double PAR_ERR[NPARS]     = { 1E-6,   110., 0.0125,  0.10,      1e-02,        1e-01,       1e-01,        1e-02,    1,    1,    1,    1 };
const int PAR_TYPE[NPARS]       = {    1,      2,      2,     2,          0,            0,           0,            0,    3,    3,    3,    3 }; // // 1,2 = signal (2 not used in the fit); 0,3 = background (3 not used in the fit)
//const int PAR_NUIS[NPARS]       = {    0,      1,      1,     1,          0,            0,           0,            0,    4,    4,    4,    4 }; // 0 = not varied, >=1 = nuisance parameters with different priors (1 = Lognormal, 2 = Gaussian, 3 = Gamma, >=4 = Uniform)

//const int PAR_NUIS[NPARS]       = {    0,      1,     1,     1,          0,            0,           0,            0,    4,    4,    4,    4 }; // all (same as above)
//const int PAR_NUIS[NPARS]       = {    0,      1,     0,     0,          0,            0,           0,            0,    0,    0,    0,    0 }; // lumi only
//const int PAR_NUIS[NPARS]       = {    0,      0,     1,     0,          0,            0,           0,            0,    0,    0,    0,    0 }; // jes only
//const int PAR_NUIS[NPARS]       = {    0,      0,     0,     1,          0,            0,           0,            0,    0,    0,    0,    0 }; // jer only
const int PAR_NUIS[NPARS]       = {    0,      0,     0,     0,          0,            0,           0,            0,    4,    4,    4,    4 }; // background only

//
// End of User Section 1
//##################################################################################################################################

// input files vector
vector<string> INPUTFILES;

// covariance matrix
double COV_MATRIX[NPARS][NPARS];
TMatrixDSym covMatrix = TMatrixDSym(NBKGPARS);
TVectorD eigenValues = TVectorD(NBKGPARS);
TMatrixD eigenVectors = TMatrixD(NBKGPARS,NBKGPARS);

// constrain S to be positive in the S+B fit
const bool posS = 0;

// use B-only fit with fixed but non-zero signal when calculating the covariance matrix used for background systematics
const bool BonlyFitForSyst = 1;

// shift in the counter used to extract the covariance matrix
int shift = 1;

// branching fraction 
double BR = 1.;

// resonance shape type
//string ResShapeType = "gg";

TH1D* HISTCDF=0; // signal CDF


////////////////////////////////////////////////////////////////////////////////
// function integral
////////////////////////////////////////////////////////////////////////////////
double INTEGRAL(double *x0, double *xf, double *par){
	double xs=par[0];
	double lumi=par[1];
	double jes=par[2];
	double jer=par[3];
	double p0=par[4];
	double p1=par[5];
	double p2=par[6];
	double p3=par[7];
	double n[NBKGPARS] = {0.};
	n[0]=par[8];
	n[1]=par[9];
	n[2]=par[10];
	n[3]=par[11];

	if( COV_MATRIX[0+shift][0+shift]>0. && (n[0]!=0. || n[1]!=0. || n[2]!=0. || n[3]!=0.) ) {
		double g[NBKGPARS] = {0.};
		for(int v=0; v<NBKGPARS; ++v) {
			for(int k=0; k<NBKGPARS; ++k) g[k]=n[v]*eigenValues(v)*eigenVectors[k][v];
			p0 += g[0];
			p1 += g[1];
			p2 += g[2];
			p3 += g[3];
		}
	}

	// uses Simpson's 3/8th rule to compute the background integral over a short interval
	// also use a power series expansion to determine the intermediate intervals since the pow() call is expensive

	double dx=(xf[0]-x0[0])/3./sqrtS;
	double x=x0[0]/sqrtS;
	double logx=log(x);

	double a=pow(1-x,p1)/pow(x,p2+p3*logx);
	double b=dx*a/x/(x-1)*(p2+p1*x-p2*x-2*p3*(x-1)*logx);
	double c=0.5*dx*dx*a*( (p1-1)*p1/(x-1)/(x-1) - 2*p1*(p2+2*p3*logx)/(x-1)/x + (p2+p2*p2-2*p3+2*p3*logx*(1+2*p2+2*p3*logx))/x/x );
	double d=0.166666667*dx*dx*dx*a*( (p1-2)*(p1-1)*p1/(x-1)/(x-1)/(x-1) - 3*(p1-1)*p1*(p2+2*p3*logx)/(x-1)/(x-1)/x - (1+p2+2*p3*logx)*(p2*(2+p2) - 6*p3 + 4*p3*logx*(1+p2*p3*logx))/x/x/x + 3*p1*(p2+p2*p2-2*p3+2*p3*logx*(1+2*p2+2*p3*logx))/(x-1)/x/x );

	double bkg=(xf[0]-x0[0])*p0*(a+0.375*(b+c+d)+0.375*(2*b+4*c+8*d)+0.125*(3*b+9*c+27*d));
	if(bkg<0.) bkg=1e-7;

	if(xs==0.0) return bkg;

	double xprimef=jes*(jer*(xf[0]-SIGMASS)+SIGMASS);
	double xprime0=jes*(jer*(x0[0]-SIGMASS)+SIGMASS);
	int bin1=HISTCDF->GetXaxis()->FindBin(xprimef);
	int bin2=HISTCDF->GetXaxis()->FindBin(xprime0);
	if(bin1<1) bin1=1;
	if(bin1>HISTCDF->GetNbinsX()) bin1=HISTCDF->GetNbinsX();
	if(bin2<1) bin2=1;
	if(bin2>HISTCDF->GetNbinsX()) bin2=HISTCDF->GetNbinsX();
	double sig=xs*lumi*(HISTCDF->GetBinContent(bin1)-HISTCDF->GetBinContent(bin2));

	return bkg+sig;
}

////////////////////////////////////////////////////////////////////////////////
// main function
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {

	if(argc<=1) {
		//cout << "Usage: stats signalmass [BR ResShapeType]" << endl;
		cout << "Usage: stats signalmass " << endl;
		return 0;
	}

	string st2mass = argv[1];
	SIGMASS = atof(argv[1]);
	//int masspoint = int(SIGMASS);
	/*if(argc>2) BR = atof(argv[2]);
	if(argc>3) ResShapeType = argv[3];*/
	BR = 1;

	//##################################################################################################################################
	// User Section 2
	//
	// (Change the code outside the User Sections only if you know what you are doing)
	//

	// input data file
	//INPUTFILES.push_back("Data_and_ResonanceShapes/Final__histograms_CSVL_0Tag_WideJets.root");
	//INPUTFILES.push_back("/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_plots.root");
	INPUTFILES.push_back("/uscms_data/d3/algomez/files/Stops/Results/data_4jet80_6jet60_plots.root");

	// data histogram name
	//string datahistname = "DATA__cutHisto_allPreviousCuts________DijetMass_pretag";
	//string datahistname = "step2plots1D/massdijetWORecoBjetsCSVM";   /// STOP1
	string datahistname = "step3plots1D/massRecoDiBjetDiJet_cutDiagStop2bbjj0";

	// input signal files with resonance shapes
	//string filename1 = "Data_and_ResonanceShapes/Resonance_Shapes_WideJets_" + ResShapeType + ".root";
	//string filename2 = "Data_and_ResonanceShapes/Resonance_Shapes_WideJets_" + ResShapeType + ".root";
	//string filename1 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_450_200_4jet80_plots.root";
	//string filename2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_450_300_4jet80_plots.root";
	string filename1 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_" + st2mass + "_200_4jet80_6jet60_plots.root";
	//string filename2 = "/uscms_data/d3/algomez/files/Stops/Results/st2_h_bb_st1_jj_450_300_4jet80_6jet60_plots.root";

	// signal histogram names
	ostringstream histname1, histname2;
	//histname1 << "h_" << ResShapeType << "_" << masspoint;
	//histname2 << "h_" << ResShapeType << "_" << masspoint;
	//histname1 << "step2plots1D/massdijetWORecoBjetsCSVM";    //// STOP1
	//histname2 << "step2plots1D/massdijetWORecoBjetsCSVM";    //// STOP1
	histname1 << "step3plots1D/massRecoDiBjetDiJet_cutDiagStop2bbjj0";
	//histname2 << "step3plots1D/massRecoDiBjetDiJet_cutDiagStop2bbjj0";
	
	// output file name
	const string OUTPUTFILE="/uscms_data/d3/algomez/files/Stops/Limits/resultsStats_jj_" + st2mass + "_200.root";

	//
	// End of User Section 2
	//##################################################################################################################################

	if(BonlyFitForSyst) shift = 0;

	if(!posS) PAR_MIN[0] = -PAR_MAX[0];

	// initialize the covariance matrix
	for(int i = 0; i<NPARS; ++i) { for(int j = 0; j<NPARS; ++j) COV_MATRIX[i][j]=0.; }

	//HISTCDF=getSignalCDF(filename1.c_str(), histname1.str().c_str(), filename2.c_str(), histname2.str().c_str(), BR, 1., 1.);
	HISTCDF=getSignalCDF(filename1.c_str(), histname1.str().c_str(), filename1.c_str(), histname1.str().c_str(), BR, 1., 1.);

	//assert(HISTCDF && SIGMASS>0);
	assert(HISTCDF);

	// get the data
	TH1D* data=getData(INPUTFILES, datahistname.c_str(), NBINS, BOUNDARIES);

	// create the output file
	ostringstream outputfile;
	//outputfile << OUTPUTFILE.substr(0,OUTPUTFILE.find(".root")) << "_" << masspoint << "_" << BR << "_" << ResShapeType << ".root";
	outputfile << OUTPUTFILE;
	TFile* rootfile=new TFile(outputfile.str().c_str(), "RECREATE");  rootfile->cd();

	// xs value
	double XSval;

	// setup an initial fitter to perform a signal+background fit
	Fitter initfit(data, INTEGRAL);
	for(int i=0; i<NPARS; i++) initfit.defineParameter(i, PAR_NAMES[i], PAR_GUESSES[i], PAR_ERR[i], PAR_MIN[i], PAR_MAX[i], PAR_NUIS[i]);

	// do an initial signal+background fit first
	for(int i=0; i<NPARS; i++) if(PAR_TYPE[i]>=2 || PAR_MIN[i]==PAR_MAX[i]) initfit.fixParameter(i);
	initfit.doFit();
	XSval = initfit.getParameter(0); // get the xs value for later use
	initfit.fixParameter(0); // a parameter needs to be fixed before its value can be changed
	initfit.setParameter(0, 0.0); // set the xs value to 0 to get the B component of the S+B fit (for calculating pulls and generating pseudo-data)
	initfit.setPrintLevel(0); //  printlevel = -1  quiet (also suppresse all warnings), = 0  normal, = 1  verbose
	initfit.calcPull("pull_bkg_init")->Write();
	initfit.calcDiff("diff_bkg_init")->Write();
	initfit.write("fit_bkg_init");
	initfit.setParameter(0, XSval);

	// setup the limit values
	double observedLowerBound, observedUpperBound;
	vector<double> expectedLowerBounds;
	vector<double> expectedUpperBounds;

	cout << "********************** pe=0 (data) **********************" << endl;

	// setup the fitter with the input from the signal+background fit
	Fitter fit_data(data, INTEGRAL);
	fit_data.setPOIIndex(POIINDEX);
	//fit_data.setPrintLevel(0);
	for(int i=0; i<NPARS; i++) fit_data.defineParameter(i, PAR_NAMES[i], initfit.getParameter(i), PAR_ERR[i], PAR_MIN[i], PAR_MAX[i], PAR_NUIS[i]);

	// perform a signal+background fit possibly followed by a background-only fit with a fixed but non-zero signal
	for(int i=0; i<NPARS; i++) if(PAR_TYPE[i]>=2 || PAR_MIN[i]==PAR_MAX[i]) fit_data.fixParameter(i);
	if(BonlyFitForSyst) { fit_data.doFit(); if(fit_data.getFitStatus().find("CONVERGED")==string::npos) { fit_data.fixParameter(0); fit_data.setParameter(0, 0.0); } else fit_data.fixParameter(0); }
	fit_data.doFit(&COV_MATRIX[0][0], NPARS);
	cout << "Data fit status: " << fit_data.getFitStatus() << endl;
	fit_data.fixParameter(0); // a parameter needs to be fixed before its value can be changed
	fit_data.setParameter(0, 0.0); // set the xs value to 0 to get the B component of the S+B fit (for calculating pulls and generating pseudo-data)
	fit_data.setPrintLevel(0);
	fit_data.calcPull("pull_bkg_0")->Write();
	fit_data.calcDiff("diff_bkg_0")->Write();
	fit_data.write("fit_bkg_0");

	// calculate eigenvalues and eigenvectors
	for(int i = 0; i<NBKGPARS; ++i) { for(int j = 0; j<NBKGPARS; ++j) { covMatrix(i,j)=COV_MATRIX[i+shift][j+shift]; } }
	//covMatrix.Print();
	const TMatrixDSymEigen eigen_data(covMatrix);
	eigenValues = eigen_data.GetEigenValues();
	eigenValues.Sqrt();
	//eigenValues.Print();
	eigenVectors = eigen_data.GetEigenVectors();
	//eigenVectors.Print();

	fit_data.setParLimits(0, 0.0, PAR_MAX[0]); // for the posterior calculation, the signal xs has to be positive
	TGraph* post_data=fit_data.calculatePosterior(NSAMPLES);
	post_data->Write("post_0");
	cout << "Call limit reached: " << (fit_data.callLimitReached() ? "True" : "False") << endl;

	// evaluate the limit
	pair<double, double> bounds_data=evaluateInterval(post_data, ALPHA, LEFTSIDETAIL);
	observedLowerBound=bounds_data.first;
	observedUpperBound=bounds_data.second;

	// reset the covariance matrix
	for(int i = 0; i<NPARS; ++i) { for(int j = 0; j<NPARS; ++j) COV_MATRIX[i][j]=0.; }

	// perform the PEs
	for(int pe=1; pe<=NPES; ++pe) {

		cout << "********************** pe=" << pe << " **********************" << endl;
		ostringstream pestr;
		pestr << "_" << pe;

		// setup the fitter with the input from the signal+background fit
		fit_data.setParameter(0, 0.0); // set the xs value to 0 to get the B component of the S+B fit (for calculating pulls and generating pseudo-data)
		TH1D* hist = fit_data.makePseudoData((string("data")+pestr.str()).c_str());
		fit_data.setParameter(0, PAR_GUESSES[0]);
		
		Fitter fit(hist, INTEGRAL);
		fit.setPOIIndex(POIINDEX);
		fit.setPrintLevel(0);
		for(int i=0; i<NPARS; i++) fit.defineParameter(i, PAR_NAMES[i], fit_data.getParameter(i), PAR_ERR[i], PAR_MIN[i], PAR_MAX[i], PAR_NUIS[i]);

		// perform a signal+background fit possibly followed by a background-only fit with a fixed but non-zero signal
		for(int i=0; i<NPARS; i++) if(PAR_TYPE[i]>=2 || PAR_MIN[i]==PAR_MAX[i]) fit.fixParameter(i);
		if(BonlyFitForSyst) { fit.doFit(); if(fit.getFitStatus().find("CONVERGED")==string::npos) { fit.fixParameter(0); fit.setParameter(0, 0.0); } else fit.fixParameter(0); }
		fit.doFit(&COV_MATRIX[0][0], NPARS);
		if(fit.getFitStatus().find("CONVERGED")==string::npos) continue; // skip the PE if the fit did not converge
		fit.fixParameter(0); // a parameter needs to be fixed before its value can be changed
		fit.setParameter(0, 0.0); // set the xs value to 0 to get the B component of the S+B fit (for calculating pulls and generating pseudo-data)
		fit.calcPull((string("pull_bkg")+pestr.str()).c_str())->Write();
		fit.calcDiff((string("diff_bkg")+pestr.str()).c_str())->Write();
		fit.write((string("fit_bkg")+pestr.str()).c_str());

		// calculate eigenvalues and eigenvectors
		for(int i = 0; i<NBKGPARS; ++i) { for(int j = 0; j<NBKGPARS; ++j) { covMatrix(i,j)=COV_MATRIX[i+shift][j+shift]; } }
		const TMatrixDSymEigen eigen(covMatrix);
		eigenValues = eigen.GetEigenValues();
		bool hasNegativeElement = false;
		for(int i = 0; i<NBKGPARS; ++i) { if(eigenValues(i)<0.) hasNegativeElement = true; }
		if(hasNegativeElement) continue; // this is principle should never happen. However, if it does, skip the PE
		eigenValues.Sqrt();
		eigenVectors = eigen.GetEigenVectors();

		fit.setParLimits(0, 0.0, PAR_MAX[0]); // for the posterior calculation, the signal xs has to be positive
		TGraph* post=fit.calculatePosterior(NSAMPLES);
		post->Write((string("post")+pestr.str()).c_str());
		cout << "Call limit reached in pe=" << pe << ": " << (fit.callLimitReached() ? "True" : "False") << endl;

		// evaluate the limit
		pair<double, double> bounds=evaluateInterval(post, ALPHA, LEFTSIDETAIL);
		if(bounds.first==0. && bounds.second>0.) {
			expectedLowerBounds.push_back(bounds.first);
			expectedUpperBounds.push_back(bounds.second);
		}

		// reset the covariance matrix
		for(int i = 0; i<NPARS; ++i) { for(int j = 0; j<NPARS; ++j) COV_MATRIX[i][j]=0.; }
	}

	////////////////////////////////////////////////////////////////////////////////
	// print the results
	////////////////////////////////////////////////////////////////////////////////

	cout << "**********************************************************************" << endl;
	for(unsigned int i=0; i<expectedLowerBounds.size(); i++)
	  cout << "expected bound(" << (i+1) << ") = [ " << expectedLowerBounds[i] << " , " << expectedUpperBounds[i] << " ]" << endl;

	cout << "\nobserved bound = [ " << observedLowerBound << " , " << observedUpperBound << " ]" << endl;

	if(LEFTSIDETAIL>0.0 && NPES>0) {
		cout << "\n***** expected lower bounds *****" << endl;
		double median;
		pair<double, double> onesigma;
		pair<double, double> twosigma;
		getQuantiles(expectedLowerBounds, median, onesigma, twosigma);
		cout << "median: " << median << endl;
		cout << "+/-1 sigma band: [ " << onesigma.first << " , " << onesigma.second << " ] " << endl;
		cout << "+/-2 sigma band: [ " << twosigma.first << " , " << twosigma.second << " ] " << endl;
	}

	if(LEFTSIDETAIL<1.0 && NPES>0) {
		cout << "\n***** expected upper bounds *****" << endl;
		double median;
		pair<double, double> onesigma;
		pair<double, double> twosigma;
		getQuantiles(expectedUpperBounds, median, onesigma, twosigma);
		cout << "median: " << median << endl;
		cout << "+/-1 sigma band: [ " << onesigma.first << " , " << onesigma.second << " ] " << endl;
		cout << "+/-2 sigma band: [ " << twosigma.first << " , " << twosigma.second << " ] " << endl;
	}

	// close the output file
	rootfile->Close();

	return 0;
}
