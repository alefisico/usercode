/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Exectuable: ClassApplication                                                   *
 *                                                                                *
 * Test suit for comparison of Reader and standalone class outputs                *
 **********************************************************************************/
////////////////////////////////////////////////////
///   TMVA ClassApplication for each mass point's signal and background  
///   Author: Wei Zou
///   Email: weizou.pku@gmaik.com
//
//    Modify by: Alejandro Gomez
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

#include "TMVA/Tools.h"

#include <vector>
#include "LOString.h"
#include "LOFileName.h"
#include "LOTopSetting.h"
#include "RootUsefulFun.h"

class ReadLikelihood;
class ReadLikelihoodD;
class ReadLikelihoodPCA;
class ReadLikelihoodMIX;
class ReadHMatrix;
class ReadFisher;
class ReadFisherG;
class ReadLD;
class ReadFDA_MT;
class ReadFDA_MC;
class ReadFDA_GA;
class ReadMLP;
class ReadMLPBFGS;
class ReadBDT;
class ReadBDTD;
class ReadBDTG;
class ReadBDTB;

//void ClassApplication( TString myMethodList = "Fisher", TString inputfilename, TString outputfilename, TString treename, TString masspoint, LOString BDTTrainName) 
void ClassApplication( TString myMethodList = "BDT",TString inputfilename, TString outputdir, TString outputfilename, TString masspoint, TString treename,TString bdttraindirectory, vector<string> variablename, vector<string> variabletype, vector<string> weightname) 
{
   cout << endl;
   cout << "==> start ClassApplication" << endl;
   const int Nmvas = 16;

   const char* bulkname[Nmvas] = { "MLP","MLPBFGS","Fisher","FisherG","Likelihood","LikelihoodD","LikelihoodPCA","LD","HMatrix","FDA_MT","FDA_MC","FDA_GA","BDT","BDTD","BDTG","BDTB"};

   bool iuse[Nmvas] = { Nmvas*kFALSE };

   // interpret input list
   if (myMethodList != "") {
      TList* mlist = TMVA::gTools().ParseFormatLine( myMethodList, " :," );
      for (int imva=0; imva<Nmvas; imva++) if (mlist->FindObject( bulkname[imva] )) iuse[imva] = kTRUE;
      delete mlist;
   }

   // create a set of variables and declare them to the reader
   // - the variable names must corresponds in name and type to 
   // those given in the weight file(s) that you use
   //std::vector<std::string> inputVars = BDTTrainName.GetVariableName();
   std::vector<std::string> inputVars = variablename;
   
   //##########################Get the Class#######################################################
   // preload standalone class(es)
   //string dir    = BDTTrainName.GetTrainDirectorty();
   TString prefix = "myTMVAClassification";
    
   TString libraryname = "";

   for (int imva=0; imva<Nmvas; imva++) {
      if (iuse[imva]) {
         //TString sofile = bdttraindirectory + masspoint + "/" + prefix + "_" + masspoint + "_" + bulkname[imva] + ".class_C.so";
         TString sofile = bdttraindirectory + masspoint + "/" + prefix + "_" + bulkname[imva] + ".class_C.so";
         //TString cfile = bdttraindirectory + masspoint + "/" + prefix + "_" + masspoint + "_" + bulkname[imva] + ".class.C++";
         TString cfile = bdttraindirectory + masspoint + "/" + prefix + "_"  + bulkname[imva] + ".class.C++";

         if(gSystem->AccessPathName(sofile))//Inprove the computing speed
         {cout << "=== Macro        : Loading class  file: " << cfile << endl;         

            //load the classifier's standalone class
            //gROOT->LoadMacro( cfile );
            gROOT->ProcessLine( TString::Format(".L %s",cfile.Data()));
	    libraryname = sofile;
         }
         else
         {
            cout << "=== We Load the shared library  file: " << sofile << endl;         
            gROOT->ProcessLine(TString::Format(".L %s",sofile.Data())); 
            libraryname = sofile;
         }
      }
   }
   cout << "=== Macro        : Classifier class loading successfully terminated" << endl;
   //##########################Get the Class#######################################################

   // define classes
   IClassifierReader* classReader[Nmvas] = { Nmvas*0 };

   // ... and create them (and some histograms for the output)
   int nbin = 100;
   TH1* hist[Nmvas];

   for (int imva=0; imva<Nmvas; imva++) {
      if (iuse[imva]) {
         cout << "=== Macro        : Testing " << bulkname[imva] << endl;
         if (bulkname[imva] == "Likelihood"   ) {
            classReader[imva] = new ReadLikelihood   ( inputVars );            
            hist[imva] = new TH1D( "MVA_Likelihood",    "MVA_Likelihood",    nbin,  0, 1 );
         }
         if (bulkname[imva] == "LikelihoodD"  ) {
            classReader[imva] = new ReadLikelihoodD  ( inputVars );
            hist[imva] = new TH1D( "MVA_LikelihoodD",   "MVA_LikelihoodD",   nbin,  0, 1 );
         }
         if (bulkname[imva] == "LikelihoodPCA") {
            classReader[imva] = new ReadLikelihoodPCA( inputVars );
            hist[imva] = new TH1D( "MVA_LikelihoodPCA", "MVA_LikelihoodPCA", nbin,  0, 1 );
         }
         if (bulkname[imva] == "LikelihoodMIX") {
            classReader[imva] = new ReadLikelihoodMIX( inputVars );
            hist[imva] = new TH1D( "MVA_LikelihoodMIX", "MVA_LikelihoodMIX", nbin,  0, 1 );
         }
         if (bulkname[imva] == "HMatrix"      ) {
            classReader[imva] = new ReadHMatrix      ( inputVars );
            hist[imva] = new TH1D( "MVA_HMatrix",       "MVA_HMatrix",       nbin, -0.95, 1.55 );
         }
         if (bulkname[imva] == "Fisher"       ) {
            classReader[imva] = new ReadFisher       ( inputVars );
            hist[imva] = new TH1D( "MVA_Fisher",        "MVA_Fisher",        nbin, -4, 4 );
         }
         if (bulkname[imva] == "FisherG"       ) {
            classReader[imva] = new ReadFisherG       ( inputVars );
            hist[imva] = new TH1D( "MVA_FisherG",        "MVA_FisherG",        nbin, -4, 4 );
         }
         if (bulkname[imva] == "LD"   ) {
            classReader[imva] = new ReadLD   ( inputVars );            
            hist[imva] = new TH1D( "MVA_LD",    "MVA_LD",    nbin,  -1., 1 );
         }	 
         if (bulkname[imva] == "FDA_MT"       ) {
            classReader[imva] = new ReadFDA_MT       ( inputVars );
            hist[imva] = new TH1D( "MVA_FDA_MT",        "MVA_FDA_MT",        nbin, -2.0, 3.0 );
         }
         if (bulkname[imva] == "FDA_MC"       ) {
            classReader[imva] = new ReadFDA_MC       ( inputVars );
            hist[imva] = new TH1D( "MVA_FDA_MC",        "MVA_FDA_MC",        nbin, -2.0, 3.0 );
         }
         if (bulkname[imva] == "FDA_GA"       ) {
            classReader[imva] = new ReadFDA_GA       ( inputVars );
            hist[imva] = new TH1D( "MVA_FDA_GA",        "MVA_FDA_GA",        nbin, -2.0, 3.0 );
         }
         if (bulkname[imva] == "MLP"          ) {
            classReader[imva] = new ReadMLP          ( inputVars );
            hist[imva] = new TH1D( "MVA_MLP",           "MVA_MLP",           nbin, -1.2, 1.2 );
         }
         if (bulkname[imva] == "MLPBFGS"          ) {
            classReader[imva] = new ReadMLPBFGS          ( inputVars );
            hist[imva] = new TH1D( "MVA_MLPBFGS",           "MVA_MLPBFGS",           nbin, -1.5, 1.5 );
         }
         if (bulkname[imva] == "BDT"          ) {
            classReader[imva] = new ReadBDT          ( inputVars );
            hist[imva] = new TH1D( "MVA_BDT",           "MVA_BDT",           50, -0.4, 0.4 );
         }
         if (bulkname[imva] == "BDTD"          ) {
            classReader[imva] = new ReadBDTD          ( inputVars );
            hist[imva] = new TH1D( "MVA_BDTD",           "MVA_BDTD",         nbin, -1, 1 );
         }
         if (bulkname[imva] == "BDTG"          ) {
            classReader[imva] = new ReadBDTG          ( inputVars );
            hist[imva] = new TH1D( "MVA_BDTG",           "MVA_BDTG",         nbin, -1, 1 );
         }
         if (bulkname[imva] == "BDTB"          ) {
            classReader[imva] = new ReadBDTB          ( inputVars );
            hist[imva] = new TH1D( "MVA_BDTB",           "MVA_BDTB",         nbin, -1, 1 );
         }
      }
   }
   cout << "=== Macro        : Class creation was successful" << endl;

   // Prepare input tree (this must be replaced by your data source)
   // in this example, there is a toy tree with signal and one with background events
   // we'll later on use only the "signal" events for the test in this example.
   //   
   TFile *input(0);
   if (!gSystem->AccessPathName(inputfilename)) {
      // first we try to find tmva_example.root in the local directory
      cout << "=== Macro        : "<< inputfilename << endl;
      input = TFile::Open(inputfilename,"READ");
   } 

   if (!input) {
      cout << "ERROR: could not open data file" << endl;
      exit(1);
   }

   //
   // prepare the tree
   // - here the variable names have to corresponds to your tree
   // - you can use the same variables as above which is slightly faster,
   //   but of course you can use different ones and copy the values inside the event loop
   //
   TTree* theTree = (TTree*)input->Get(treename);
   cout << "=== Macro        : Loop over signal sample" << endl;

   // the references to the variables
   //const int variablesize = BDTTrainName.GetVariableName().size();
   int doublevariablesize = 0;
   int intvariablesize = 0;
   for(Int_t ll = 0; ll < variabletype.size(); ll++)
   {
      if(variabletype[ll] == "F")
      {doublevariablesize = doublevariablesize + 1;}
      if(variabletype[ll] == "I")
      {intvariablesize = intvariablesize + 1;}
   }
   cout << "Double Variable Size " << doublevariablesize << endl;
   cout << "Int Variable Size " << intvariablesize << endl;

   const int fixdoublevariablesize = doublevariablesize;
   const int fixintvariablesize = intvariablesize;
   const int variablesize = variablename.size();
   const int weightsize = weightname.size();

   Double_t  variablevalues[fixdoublevariablesize]; //Double_t size
   //Double_t  *addressofvariable = variablevalues;
   Int_t  intvariablevalues[fixintvariablesize]; //Int_t size
   //Int_t  *addressofintvariable = intvariablevalues;
   Double_t  weightvalues[weightsize];
   //Double_t  *addressofweight = weightvalues;

   int idouble = 0;
   int iint = 0;
   for(Int_t jj = 0; jj < variablesize; jj++)
   {     
      //TString branchname = BDTTrainName.GetVariableName()[jj];
      TString branchname = variablename[jj];
      //theTree->SetBranchAddress(branchname.Data(),addressofvariable + jj);
      if(variabletype[jj] == "F")
      {
         //theTree->SetBranchAddress(branchname.Data(),addressofvariable + idouble);
         theTree->SetBranchAddress(branchname.Data(),&(variablevalues[idouble]));
         idouble = idouble + 1;
      }
      if(variabletype[jj] == "I")
      {
         //theTree->SetBranchAddress(branchname.Data(),addressofintvariable + iint);
         theTree->SetBranchAddress(branchname.Data(),&(intvariablevalues[iint]));
         iint = iint + 1;
      }
   }

   for(Int_t oo = 0; oo < weightsize; oo++)
   {
      TString tmpbranchname = weightname[oo];
      //theTree->SetBranchAddress(tmpbranchname.Data(),addressofweight + oo);
      theTree->SetBranchAddress(tmpbranchname.Data(),&weightvalues[oo]);
   }

   cout << "=== Macro        : Processing total of " << theTree->GetEntries() << " events ... " << endl;
   
   //These histograms are used for monitoring the Variables' shape, They are working very Well
   /*
   TH1D* Ht = new TH1D("Ht","Ht",60,0,3000); 
   TH1D* Stjet = new TH1D("Stjet","Stjet",60,0,3000); 
   TH1D* Stlep = new TH1D("Stlep","Stlep",50,0,1000); 
   TH1D* jetnum = new TH1D("jetnum","jetnum",10,0.5,10.5); 
   TH1D* btagjet = new TH1D("btagjet","btagjet",8,-0.5,7.5);
   */

   std::vector<double>* inputVec = new std::vector<double>( variablesize );

   for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {

      if (ievt%1000 == 0) cout << "=== Macro        : ... processing event: " << ievt << endl;

      theTree->GetEntry(ievt);
      
      inputVec->clear();//Very important when using the vector's push_back, Be Careful again!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      Double_t totalweight = 1.0;
     
      for(Int_t pp = 0; pp < weightsize; pp++)
      {
          totalweight = totalweight * weightvalues[pp];
      }

      for(Int_t kk = 0; kk < fixdoublevariablesize; kk++)
      {
         //cout<<variablevalues[kk]<< endl;
         inputVec->push_back(variablevalues[kk]);
      }
    
      for(Int_t nn = 0; nn < fixintvariablesize; nn++)
      {
         //cout<<intvariablevalues[nn]<< endl;
         double tmpint = intvariablevalues[nn];
         inputVec->push_back(tmpint);
      }

      // loop over all booked classifiers
      for (int imva=0; imva<Nmvas; imva++) {

         if (iuse[imva]) {

            // retrive the classifier responses            
            //Ht->Fill(variablevalues[0],totalweight);
            //Stlep->Fill(variablevalues[1],totalweight);
            //Stjet->Fill(variablevalues[2],totalweight);
            //jetnum->Fill(intvariablevalues[0],totalweight);
            //btagjet->Fill(intvariablevalues[1],totalweight);
            double retval = classReader[imva]->GetMvaValue( *inputVec );
            //cout << retval << endl;
            hist[imva]->Fill( retval, totalweight );
         }
      }
   }

   cout << "=== Macro        : Event loop done! " << endl;

   TFile *target  = new TFile( outputdir + outputfilename,"RECREATE" );
   string tmpname = myMethodList;
   string tmpstrhistname = TrailStripString(outputfilename,"_" + tmpname + "_ClassApplication.root");
   TString tmphistname = tmpstrhistname;

   for (int imva=0; imva<Nmvas; imva++) {
      if (iuse[imva]) {
         /*Ht->Write();
         Stlep->Write();
         Stjet->Write();
         jetnum->Write();
         btagjet->Write();
         */
         hist[imva]->SetName(tmphistname);
         hist[imva]->SetTitle(tmphistname);
         hist[imva]->Write();
      }
   }
   cout << "=== Macro        : Created target file: " << target->GetName() << endl;
   target->Close();

   delete target;
   delete inputVec;

   for (int imva=0; imva<Nmvas; imva++) {
      if(iuse[imva]){
         delete classReader[imva];
      }
   }

   gSystem->Unload(libraryname.Data());
   cout << "==> ClassApplication is done!" << endl << endl;
} 

void ClassApplication_Version1(string topsettingfile){

   LOTopSetting tmpsetting;
   //tmpsetting.LoadFile("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/TMVASetUp/BDTInputOutputDirectoryVersion1.txt");
   tmpsetting.LoadFile(topsettingfile);

   string inputdir = tmpsetting.GetApplicationInputDirName(); //This is the input C++ class of BDT Training Directory 
   string outputdir = tmpsetting.GetApplicationOutputDirName(); //This is TMVA Histogram response 

   string treename = tmpsetting.GetTrainTreeName();
   string trainmethod = tmpsetting.GetTrainMethodName();

   LOString BDTTrainName;
   BDTTrainName.LoadFile(tmpsetting.GetTrainVariableTxtName());

   LOFileName BDTTrainFileName;
   BDTTrainFileName.LoadFile(tmpsetting.GetTrainFileTxtName());
   vector<string> signalfilename = BDTTrainFileName.GetSignalFileName();
   vector<string> backgroundfilename = BDTTrainFileName.GetBackgroundClassificationFileName();
   //TODO Same As BDT Training
   vector<string> variablename = BDTTrainName.GetVariableName();
   vector<string> variabletype = BDTTrainName.GetVariableType();
   vector<string> weightname = BDTTrainName.GetWeightName();
   string bdttraindirectory = BDTTrainName.GetTrainDirectorty();
   gSystem->Exec(TString::Format("mkdir %s",outputdir.c_str()));

   for(Int_t masspoint = 0; masspoint < BDTTrainName.GetMassPointName().size(); masspoint++)
   {   
      //We should Make each Directory for each Mass point ClassApplication
      string tmpoutputname = BDTTrainName.GetMassPointName()[masspoint];
      string tmpoutputdir = outputdir + tmpoutputname;
      //gSystem->MakeDirectory(tmpoutputdir.c_str());
      gSystem->Exec(TString::Format("mkdir %s",tmpoutputdir.c_str()));

      // adding all backgrounds, with systematics, and signal for Classification
      vector<string> tmpback = backgroundfilename;
      string tmpbackjesup = "results_4Top"+tmpoutputname+"_JECUP.root";
      string tmpbackjesdown = "results_4Top"+tmpoutputname+"_JECDOWN.root";
      string tmpbackjerup = "results_4Top"+tmpoutputname+"_JERUP.root";
      string tmpbackjerdown = "results_4Top"+tmpoutputname+"_JERDOWN.root";
      string tmpbackpuup = "results_4Top"+tmpoutputname+"_PUUP.root";
      string tmpbackpudown = "results_4Top"+tmpoutputname+"_PUDOWN.root";
      string tmpbackdata = "results_data.root";
      tmpback.push_back(tmpbackjesup);
      tmpback.push_back(tmpbackjesdown);
      tmpback.push_back(tmpbackjerup);
      tmpback.push_back(tmpbackjerdown);
      tmpback.push_back(tmpbackpuup);
      tmpback.push_back(tmpbackpudown);
      tmpback.push_back(tmpbackdata);
      tmpback.push_back(signalfilename[masspoint]);

      for(Int_t ii = 0; ii < tmpback.size(); ii++)
      {
         string outputname = TrailStripString(tmpback[ii],".root");
         gROOT->Reset();
         ClassApplication(trainmethod, inputdir + tmpback[ii], tmpoutputdir+ "/" , outputname + "_" + trainmethod + "_ClassApplication.root", BDTTrainName.GetMassPointName()[masspoint], treename,bdttraindirectory,variablename, variabletype, weightname);

      }
   }
}
