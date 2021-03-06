// @(#)root/tmva $Id: myTMVAClassification.C,v 1.1 2011/12/16 14:26:35 kalanand Exp $
/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Root Macro: myTMVAClassification                                                 *
 *                                                                                *
 * This macro provides examples for the training and testing of the               *
 * TMVA classifiers.                                                              *
 *                                                                                *
 * As input data is used a toy-MC sample consisting of four Gaussian-distributed  *
 * and linearly correlated input variables.                                       *
 *                                                                                *
 * The methods to be used can be switched on and off by means of booleans, or     *
 * via the prompt command, for example:                                           *
 *                                                                                *
 *    root -l myTMVAClassification.C\(\"Fisher,Likelihood\"\)                       *
 *                                                                                *
 * (note that the backslashes are mandatory)                                      *
 * If no method given, a default set is used.                                     *
 *                                                                                *
 * The output file "TMVA.root" can be analysed with the use of dedicated          *
 * macros (simply say: root -l <macro.C>), which can be conveniently              *
 * invoked through a GUI that will appear at the end of the run of this macro.    *
 * Launch the GUI via the command:                                                *
 *                                                                                *
 *    root -l ./TMVAGui.C                                                         *
 *                                                                                *
 **********************************************************************************/
////////////////////////////////////////////////////
///   TMVA Training for each mass point's signal and background  
///   Author: Wei Zou
///   Email: weizou.pku@gmail.com
//    Modified: Alejandro Gomez
//    Email: alejandro.gomez@cern.ch
//////////////////////////////////////////////////
#include <cstdlib>
#include <iostream> 
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TPluginManager.h"

#include "TMVAGui.C"
#include "mvas_v1.C"
#include "correlations1.C"


#include "LOString.h"
#include "LOFileName.h"
#include "LOTopSetting.h"
#include "RootUsefulFun.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#endif

// read input data file with ascii format (otherwise ROOT) ?
Bool_t ReadDataFromAsciiIFormat = kFALSE;

using namespace std;

//void myTMVAClassification( TString myMethodList = "", TString signalfilename, vector<string> backgroundfilename, TString outputfilename, TString masspoint, TString treename, LOString BDTTrainName) 
void myTMVAClassification( TString myMethodList = "", TString signalfilename, vector<string> backgroundfilename, TString outputfilename, TString masspoint, TString treename, TString bdttraindir , vector<string> weightname, TString cutname,vector<string> variablename, vector<string> variabletype, vector<string> spectatorname, vector<string> spectatortype) 
{
   // The explicit loading of the shared libTMVA is done in TMVAlogon.C, defined in .rootrc
   // if you use your private .rootrc, or run from a different directory, please copy the 
   // corresponding lines from .rootrc

   // methods to be processed can be given as an argument; use format:
   //
   // mylinux~> root -l myTMVAClassification.C\(\"myMethod1,myMethod2,myMethod3\"\)
   //
   // if you like to use a method via the plugin mechanism, we recommend using
   // 
   // mylinux~> root -l myTMVAClassification.C\(\"P_myMethod\"\)
   // (an example is given for using the BDT as plugin (see below),
   // but of course the real application is when you write your own
   // method based)
   //gROOT->Reset();
   // this loads the library
   TMVA::Tools::Instance();

   //---------------------------------------------------------------
   // default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   // --- Cut optimisation
   Use["Cuts"]            = 0;
   Use["CutsD"]           = 0;
   Use["CutsPCA"]         = 0;
   Use["CutsGA"]          = 0;
   Use["CutsSA"]          = 0;
   // ---
   // --- 1-dimensional likelihood ("naive Bayes estimator")
   Use["Likelihood"]      = 0;
   Use["LikelihoodD"]     = 0; // the "D" extension indicates decorrelated input variables (see option strings)
   Use["LikelihoodPCA"]   = 0; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
   Use["LikelihoodKDE"]   = 0;
   Use["LikelihoodMIX"]   = 0;
   // ---
   // --- Mutidimensional likelihood and Nearest-Neighbour methods
   Use["PDERS"]           = 0;
   Use["PDERSD"]          = 0;
   Use["PDERSPCA"]        = 0;
   Use["PDERSkNN"]        = 0; // depreciated until further notice
   Use["PDEFoam"]         = 0;
   Use["PDEFoamBoost"]    = 0; // uses generalised MVA method boosting
   Use["KNN"]             = 0; // k-nearest neighbour method
   // ---
   // --- Linear Discriminant Analysis
   Use["LD"]              = 0; // Linear Discriminant identical to Fisher
   Use["Fisher"]          = 0;
   Use["FisherG"]         = 0;
   Use["BoostedFisher"]   = 0; // uses generalised MVA method boosting
   Use["HMatrix"]         = 0;
   //
   // --- Function Discriminant analysis
   Use["FDA_GA"]          = 0; // minimisation of user-defined function using Genetics Algorithm
   Use["FDA_SA"]          = 0;
   Use["FDA_MC"]          = 0;
   Use["FDA_MT"]          = 0;
   Use["FDA_GAMT"]        = 0;
   Use["FDA_MCMT"]        = 0;
   // ---
   // --- Neural Networks (all are feed-forward Multilayer Perceptrons)
   Use["MLP"]             = 0; // Recommended ANN
   Use["MLPBFGS"]         = 0; // Recommended ANN with optional training method
   Use["MLPBNN"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
   Use["CFMlpANN"]        = 0; // Depreciated ANN from ALEPH
   Use["TMlpANN"]         = 0; // ROOT's own ANN
   // ---
   // --- Support Vector Machine 
   Use["SVM"]             = 0;
   // ---
   // --- Boosted Decision Trees
   Use["BDT"]             = 1; // uses Adaptive Boost
   Use["BDTG"]            = 1; // uses Gradient Boost
   Use["BDTB"]            = 1; // uses Bagging
   Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
   Use["BDTF"]            = 0; // allow usage of fisher discriminant for node splitting 
   // 
   // --- Friedman's RuleFit method, ie, an optimised series of cuts ("rules")
   Use["RuleFit"]         = 0;
   // ---
   Use["Plugin"]          = 0;
   // ---------------------------------------------------------------

   std::cout << std::endl;
   std::cout << "==> Start myTMVAClassification" << std::endl;

   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
            std::cout << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------

   // --- Here the preparation phase begins

   // Create a new root output file.
   TString outfileName( outputfilename );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
   cout << "Check 1" << endl;

   //cout << BDTTrainName.GetVariableName()[0] << endl;
   // Create the factory object. Later you can choose the methods
   // whose performance you'd like to investigate. The factory will
   // then run the performance analysis for you.
   //
   // The first argument is the base of the name of all the
   // weightfiles in the directory weight/ 
   //
   // The second argument is the output file for the training results
   // All TMVA output can be suppressed by removing the "!" (not) in 
   // front of the "Silent" argument in the option string
   //TMVA::Factory *factory = new TMVA::Factory( "myTMVAClassification_" + masspoint, outputFile, 
   TMVA::Factory *factory = new TMVA::Factory( "myTMVAClassification", outputFile, 
         //"!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
      "!V:!Silent:Color:DrawProgressBar:Transformations=I;G" ); //,D" );

   // If you wish to modify default settings 
   // (please check "src/Config.h" to see all available global options)
   //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
   //TString tmpbdtdir = BDTTrainName.GetTrainDirectorty(); 
   //(TMVA::gConfig().GetIONames()).fWeightFileDir = tmpbdtdir + "/" + masspoint;
   (TMVA::gConfig().GetIONames()).fWeightFileDir = bdttraindir;

   // Define the input variables that shall be used for the MVA training
   // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
   // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]

   cout << "Check 2" << endl;
   // B discriminant
   //cout << BDTTrainName.GetVariableName()[0] << endl;

   //for(Int_t ii = 0; ii < BDTTrainName.GetVariableName().size(); ii++)
   for(Int_t ii = 0; ii < variablename.size(); ii++)
   {
      //cout << BDTTrainName.GetVariableName()[ii]<< endl;
      //TString variable = BDTTrainName.GetVariableName()[ii];
      //char vtype = BDTTrainName.GetVariableType()[ii];
      TString variable = variablename[ii];
      char vtype = variabletype[ii];
      //factory->AddVariable(variable.Data(), vtype.Data());
      factory->AddVariable(variable.Data(), vtype);
   }
   
   cout << "Check 3" << endl;
   // You can add so-called "Spectator variables", which are not used in the MVA training, 
   // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the 
   // input variables, the response values of all trained MVAs, and the spectator variables
   //for(Int_t jj = 0; jj < BDTTrainName.GetSpectatorName().size(); jj++)
   for(Int_t jj = 0; jj < spectatorname.size(); jj++)
   {
     //TString spectator = BDTTrainName.GetSpectatorName()[jj];
     //char stype     = BDTTrainName.GetSpectatorType()[jj];
     TString spectator = spectatorname[jj];
     char stype     = spectatortype[jj];
     factory->AddSpectator(spectator, stype);
   }
   cout << "Check 4" << endl;
   // read training and test data
   TString tmpsignalfilename = signalfilename;
   TFile *signalfile = TFile::Open(tmpsignalfilename,"READ");
   const int filesize = backgroundfilename.size();
   TFile *backgroundfile[filesize];
   for(Int_t kk = 0; kk < backgroundfilename.size(); kk++)
   {
      TString tmpfilename = backgroundfilename[kk];
      backgroundfile[kk] = TFile::Open(tmpfilename,"READ");
   }
   cout << "Check 5" << endl;
   //std::cout << "--- myTMVAClassification : Using input file: " << input1->GetName() << std::endl;
   //
   TTree *signaltree     = (TTree*)signalfile->Get(treename);
   TTree *backgroundtree[filesize];
   for(Int_t ll = 0; ll < backgroundfilename.size(); ll++)
   {
      backgroundtree[ll] = (TTree*)backgroundfile[ll]->Get(treename);
   }

   // global event weights per tree (see below for setting event-wise weights)
   Double_t signalWeight     = 1.0;
   Double_t backgroundWeight = 1.0;

   // ====== register trees ====================================================
   //
   // the following method is the prefered one:
   // you can add an arbitrary number of signal or background trees
   factory->AddSignalTree    ( signaltree,     signalWeight     );
   for(Int_t mm = 0; mm < backgroundfilename.size(); mm++)
   {
     factory->AddBackgroundTree( backgroundtree[mm], backgroundWeight );
   }
   // To give different trees for training and testing, do as follows:
   //    factory->AddSignalTree( signalTrainingTree, signalTrainWeight, "Training" );
   //    factory->AddSignalTree( signalTestTree,     signalTestWeight,  "Test" );

   // Use the following code instead of the above two or four lines to add signal and background 
   // training and test events "by hand"
   // NOTE that in this case one should not give expressions (such as "var1+var2") in the input 
   //      variable definition, but simply compute the expression before adding the event
   // 
   //    // --- begin ----------------------------------------------------------
   //    std::vector<Double_t> vars( 4 ); // vector has size of number of input variables
   //    Float_t  treevars[4];
   //    for (Int_t ivar=0; ivar<4; ivar++) signal->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
   //    for (Int_t i=0; i<signal->GetEntries(); i++) {
   //       signal->GetEntry(i);
   //       for (Int_t ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
   //       // add training and test events; here: first half is training, second is testing
   //       // note that the weight can also be event-wise	
   //       if (i < signal->GetEntries()/2) factory->AddSignalTrainingEvent( vars, signalWeight ); 
   //       else                            factory->AddSignalTestEvent    ( vars, signalWeight ); 
   //    }
   //
   //    for (Int_t ivar=0; ivar<4; ivar++) background->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
   //    for (Int_t i=0; i<background->GetEntries(); i++) {
   //       background->GetEntry(i); 
   //       for (Int_t ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
   //       // add training and test events; here: first half is training, second is testing
   //       // note that the weight can also be event-wise	
   //       if (i < background->GetEntries()/2) factory->AddBackgroundTrainingEvent( vars, backgroundWeight ); 
   //       else                                factory->AddBackgroundTestEvent    ( vars, backgroundWeight ); 
   //    }
   //    // --- end ------------------------------------------------------------
   //
   // ====== end of register trees ==============================================


   // This would set individual event weights (the variables defined in the 
   // expression need to exist in the original TTree)
   //    for signal    : factory->SetSignalWeightExpression("weight1*weight2");
   //    for background: factory->SetBackgroundWeightExpression("weight1*weight2");
   
   //TString weightname = BDTTrainName.GetWeightName();
   //Pending for Signal
   //    for signal    : factory->SetSignalWeightExpression(TString::Format("%s",weightname.Data()));
   TString tmpweight = WeightMerge(weightname);
   //    for signal    : factory->SetSignalWeightExpression(TString::Format("%s",tmpweight.Data()));
   //for background: factory->SetBackgroundWeightExpression(TString::Format("%s",tmpweight.Data()));
   factory->SetBackgroundWeightExpression(tmpweight);

   // Apply additional cuts on the signal and background samples (can be different)
   //   TCut mycuts = "abs(eta)>1.5"; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
   //    TCut mycutb = "abs(eta)>1.5"; // for example: TCut mycutb = "abs(var1)<0.5";
   //TString tmpcut = BDTTrainName.GetCutName();
   TCut mycuts = cutname;

   // tell the factory to use all remaining events in the trees after training for testing:
   factory->PrepareTrainingAndTestTree( mycuts, mycuts, 
         "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

   // If no numbers of events are given, half of the events in the tree are used for training, and 
   // the other half for testing:
   //    factory->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );  
   // To also specify the number of testing events, use:
   //    factory->PrepareTrainingAndTestTree( mycut, 
   //                                         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );  

   // ---- Book MVA methods
   //
   // please lookup the various method configuration options in the corresponding cxx files, eg:
   // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
   // it is possible to preset ranges in the option string in which the cut optimisation should be done:
   // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

   // Cut optimisation
   if (Use["Cuts"])
      factory->BookMethod( TMVA::Types::kCuts, "Cuts", 
            "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );

   if (Use["CutsD"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsD", 
            "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate" );

   if (Use["CutsPCA"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsPCA", 
            "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA" );

   if (Use["CutsGA"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsGA",
            "H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95" );

   if (Use["CutsSA"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsSA",
            "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

   // Likelihood
   if (Use["Likelihood"])
      factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood", 
            "H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" ); 

   // test the decorrelated likelihood
   if (Use["LikelihoodD"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodD", 
            "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=Decorrelate" ); 

   if (Use["LikelihoodPCA"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodPCA", 
            "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=PCA" ); 

   // test the new kernel density estimator
   if (Use["LikelihoodKDE"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodKDE", 
            "!H:!V:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50" ); 

   // test the mixed splines and kernel density estimator (depending on which variable)
   if (Use["LikelihoodMIX"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodMIX", 
            "!H:!V:!TransformOutput:PDFInterpolSig[0]=KDE:PDFInterpolBkg[0]=KDE:PDFInterpolSig[1]=KDE:PDFInterpolBkg[1]=KDE:PDFInterpolSig[2]=Spline2:PDFInterpolBkg[2]=Spline2:PDFInterpolSig[3]=Spline2:PDFInterpolBkg[3]=Spline2:KDEtype=Gauss:KDEiter=Nonadaptive:KDEborder=None:NAvEvtPerBin=50" ); 

   // test the multi-dimensional probability density estimator
   // here are the options strings for the MinMax and RMS methods, respectively:
   //      "!H:!V:VolumeRangeMode=MinMax:DeltaFrac=0.2:KernelEstimator=Gauss:GaussSigma=0.3" );   
   //      "!H:!V:VolumeRangeMode=RMS:DeltaFrac=3:KernelEstimator=Gauss:GaussSigma=0.3" );   
   if (Use["PDERS"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERS", 
            "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );

   if (Use["PDERSkNN"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERSkNN", 
            "!H:!V:VolumeRangeMode=kNN:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );

   if (Use["PDERSD"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERSD", 
            "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=Decorrelate" );

   if (Use["PDERSPCA"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERSPCA", 
            "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=PCA" );

   // Multi-dimensional likelihood estimator using self-adapting phase-space binning
   if (Use["PDEFoam"])
      factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoam", 
            "H:!V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.0333:nActiveCells=500:nSampl=2000:nBin=5:CutNmin=T:Nmin=100:Kernel=None:Compress=T" );

   // K-Nearest Neighbour classifier (KNN)
   if (Use["KNN"])
      factory->BookMethod( TMVA::Types::kKNN, "KNN", 
            "H:nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );
   // H-Matrix (chi2-squared) method
   if (Use["HMatrix"])
      factory->BookMethod( TMVA::Types::kHMatrix, "HMatrix", "!H:!V" ); 

   // Fisher discriminant   
   if (Use["Fisher"])
      factory->BookMethod( TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=60:NsmoothMVAPdf=10" );

   // Fisher with Gauss-transformed input variables
   if (Use["FisherG"])
      factory->BookMethod( TMVA::Types::kFisher, "FisherG", "H:!V:VarTransform=Gauss" );

   // Composite classifier: ensemble (tree) of boosted Fisher classifiers
   if (Use["BoostedFisher"])
      factory->BookMethod( TMVA::Types::kFisher, "BoostedFisher", "H:!V:Boost_Num=20:Boost_Transform=log:Boost_Type=AdaBoost:Boost_AdaBoostBeta=0.2");

   // Linear discriminant (same as Fisher)
   if (Use["LD"])
      factory->BookMethod( TMVA::Types::kLD, "LD", "H:!V:VarTransform=None" );

   // Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
   if (Use["FDA_MC"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_MC",
            "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:SampleSize=100000:Sigma=0.1" );

   if (Use["FDA_GA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_GA",
            "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=300:Cycles=3:Steps=20:Trim=True:SaveBestGen=1" );

   if (Use["FDA_SA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_SA",
            "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=SA:MaxCalls=15000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

   if (Use["FDA_MT"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_MT",
            "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch" );

   if (Use["FDA_GAMT"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_GAMT",
            "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim" );

   if (Use["FDA_MCMT"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_MCMT",
            "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:SampleSize=20" );

   // TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
   if (Use["MLP"])
      //factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:V:NeuronType=tanh:VarTransform=N:NCycles=500:HiddenLayers=N+5:TestRate=10:EpochMonitoring" );    // HiddenLayers=N+5 default
      factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:V:NeuronType=tanh:VarTransform=N:NCycles=2000:HiddenLayers=N+1:TestRate=10:EpochMonitoring" ); 

   if (Use["MLPBFGS"])
      factory->BookMethod( TMVA::Types::kMLP, "MLPBFGS", "H:V:NeuronType=tanh:VarTransform=N:NCycles=500:HiddenLayers=N+5:TestRate=10:TrainingMethod=BFGS:!EpochMonitoring" );


   // CF(Clermont-Ferrand)ANN
   if (Use["CFMlpANN"])
      factory->BookMethod( TMVA::Types::kCFMlpANN, "CFMlpANN", "!H:!V:NCycles=2000:HiddenLayers=N+1,N"  ); // n_cycles:#nodes:#nodes:...  

   // Tmlp(Root)ANN
   if (Use["TMlpANN"])
      factory->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN", "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3"  ); // n_cycles:#nodes:#nodes:...

   // Support Vector Machine
   if (Use["SVM"])
      factory->BookMethod( TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );

   // Boosted Decision Trees
   if (Use["BDTG"]) // Gradient Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDTG", 
            "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:NNodesMax=5" );

   if (Use["BDT"])  // Adaptive Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDT", 
	//"H:V:NTrees=200:BoostType=AdaBoost:SeparationType=CrossEntropy:PruneStrength=10:nCuts=20:PruneMethod=ExpectedError");
	"H:V:NTrees=200:BoostType=AdaBoost:SeparationType=CrossEntropy:PruneStrength=10:nCuts=20");
//	"H:V:PruneStrength=10:NTrees=200:nEventsMin=100:nCuts=20:SeparationType=CrossEntropy:AdaBoostBeta=0.2");  //// BEST TRY!!!!
//	"H:V:PruneStrength=10:NTrees=200:nEventsMin=100:nCuts=20:SeparationType=CrossEntropy");  //// Wei setup 
	//"H:V:AdaBoostBeta=0.2:nCuts=50:PruneMethod=NoPruning:NTrees=400:SeparationType=CrossEntropy");  //// second try
        //"H:V:NTrees=850:nEventsMin=150:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );  //// from TMVAClassificacion.C
        //"!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning"); //default

   if (Use["BDTB"]) // Bagging
      factory->BookMethod( TMVA::Types::kBDT, "BDTB", 
            "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );

   if (Use["BDTD"]) // Decorrelation + Adaptive Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDTD", 
            "!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning:VarTransform=Decorrelate" );

   // RuleFit -- TMVA implementation of Friedman's method
   if (Use["RuleFit"])
      factory->BookMethod( TMVA::Types::kRuleFit, "RuleFit",
            "H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );

   // For an example of the category classifier, see: myTMVAClassificationCategory

   // --------------------------------------------------------------------------------------------------

   // As an example how to use the ROOT plugin mechanism, book BDT via
   // plugin mechanism
   if (Use["Plugin"]) {
      //
      // first the plugin has to be defined, which can happen either through the following line in the local or global .rootrc:
      //
      // # plugin handler          plugin name(regexp) class to be instanciated library        constructor format
      // Plugin.TMVA@@MethodBase:  ^BDT                TMVA::MethodBDT          TMVA.1         "MethodBDT(TString,TString,DataSet&,TString)"
      // 
      // or by telling the global plugin manager directly
      gPluginMgr->AddHandler("TMVA@@MethodBase", "BDT", "TMVA::MethodBDT", "TMVA.1", "MethodBDT(TString,TString,DataSet&,TString)");
      factory->BookMethod( TMVA::Types::kPlugins, "BDT",
            "!H:!V:NTrees=400:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=CostComplexity:PruneStrength=50" );
   }

   // --------------------------------------------------------------------------------------------------

   // ---- Now you can tell the factory to train, test, and evaluate the MVAs

   // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // ---- Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();    

   // --------------------------------------------------------------

   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> myTMVAClassification is done!" << std::endl;      

   delete factory;

  mvas(outfileName,3,bdttraindir);
  correlations1(outfileName,bdttraindir);
   // Launch the GUI for the root macros
   //if (!gROOT->IsBatch()) TMVAGui( outfileName );
}

void TMVAClassification_Version1(string topsettingfile)
{
   //gROOT->ProcessLine(".L stdload_C.so");
   LOTopSetting tmpsetting;  
   //tmpsetting.LoadFile("/uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/TMVA/config/BDTInputOutputDirectoryVersion1.txt");
   tmpsetting.LoadFile(topsettingfile); // read file and take options

   string inputdir =  tmpsetting.GetTrainInputDirName();
   string outputdir = tmpsetting.GetTrainOutputDirName();

   string treename = tmpsetting.GetTrainTreeName();
   string trainmethod = tmpsetting.GetTrainMethodName();

   LOString BDTTrainName;
   BDTTrainName.LoadFile(tmpsetting.GetTrainVariableTxtName());

   LOFileName BDTTrainFileName;
   BDTTrainFileName.LoadFile(tmpsetting.GetTrainFileTxtName());
   vector<string> signalfilename = BDTTrainFileName.GetSignalFileName();
   vector<string> backgroundfilename = BDTTrainFileName.GetBackgroundFileName();
   for(Int_t iback; iback < backgroundfilename.size(); iback++)
    {
            backgroundfilename[iback] = inputdir + backgroundfilename[iback];
    }
   
   // :qTODO, using the Class to Load the BDT Setting, For some unknown reason, Loading the class is causing something wrong, Need to figure out
   gSystem->MakeDirectory(BDTTrainName.GetTrainDirectorty().c_str());
   vector<string> variablename = BDTTrainName.GetVariableName();
   vector<string> variabletype = BDTTrainName.GetVariableType();
   vector<string> spectatorname = BDTTrainName.GetSpectatorName();
   vector<string> spectatortype = BDTTrainName.GetSpectatorType();

   vector<string> weightname = BDTTrainName.GetWeightName();
   string cutname    = BDTTrainName.GetCutName();

   for(Int_t masspoint = 0; masspoint < BDTTrainName.GetMassPointName().size(); masspoint++)
   {
      string outputname = signalfilename[masspoint];
      outputname = TrailStripString(outputname,".root");
      string bdttraindirectory = BDTTrainName.GetTrainDirectorty();
      bdttraindirectory = bdttraindirectory + BDTTrainName.GetMassPointName()[masspoint]; 
      //cout << outputname << endl;
      //myTMVAClassification(trainmethod, inputdir + signalfilename[masspoint], backgroundfilename, outputdir + outputname + "_" + trainmethod + ".root", BDTTrainName.GetMassPointName()[masspoint], treename, BDTTrainName);
      myTMVAClassification(trainmethod, inputdir + signalfilename[masspoint], backgroundfilename, outputdir + outputname + "_" + trainmethod + ".root", BDTTrainName.GetMassPointName()[masspoint], treename, bdttraindirectory,weightname,cutname,variablename,variabletype,spectatorname,spectatortype);
   }
}
