//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 25 13:10:09 2011 by ROOT version 5.27/06b
// from TTree top/top
// found on file: /uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/41x/v2/SingleMu2011A_v1.root
//////////////////////////////////////////////////////////

#ifndef Analyzer_h
#define Analyzer_h

#define STANDALONE

#include <TROOT.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH2.h>
#include <TH3.h>
#include <TProofOutputFile.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TSelector.h>
#include <TSystem.h>
#include <TProfile.h>
#include <fstream>
#include <iostream>
#include "BTagSFUtil_lite.h"

#include "Yumiceva/Top7TeV/interface/TopEventNtuple.h"
#include "Yumiceva/TreeAnalyzer/interface/MuonSelector.h"
#include "Yumiceva/TreeAnalyzer/interface/ElectronSelector.h"
#include "Yumiceva/TreeAnalyzer/interface/StoreTreeVariable.h"
#include "Yumiceva/TreeAnalyzer/interface/METzCalculator.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "PhysicsTools/Utilities/interface/Lumi3DReWeighting.h"

#include "weights/myTMVAClassification_BDT.class.C"

#include <map>
#include <string>
#include <vector>

#include "LOTable.h"

class Analyzer : public TSelector {

private:
  void            ParseInput();
  TString         fMyOpt;
  int             fChannel;
  bool            fVerbose;
  bool            fIsMC;
  bool            fdoQCD1SideBand;
  bool            fdoQCD2SideBand;
  bool            fdoJECunc;
  bool            fdoJECup;
  bool            fdoMtopCut;
  bool            fdoJERunc;
  bool            fdoJERup;
  bool            fdoJERdown;
  bool            fdodiLepton;
  bool            fpu_up;
  bool            fpu_down;
  bool            fbtag_up;
  bool            fbtag_down;

  //HistoManager    *fHist;
  TString         fSample;
  TString         fOutdir;
  TH1F            *h1test;
  TH1D            *hcutflow;
  map< string, TH1*> hmuons;
  map< string, TH1*> hbtageff;
  map< string, TH1*> helectrons;
  map< string, TH1*> hjets;
  map< string, TH1*> hPVs;
  map< string, TH1*> hMET;
  map< string, TH1*> hM;
  vector< string > fCutLabels;
  vector< double > fpu_weights_vec;
  edm::Lumi3DReWeighting LumiWeights_;
  edm::Lumi3DReWeighting LumiWeightsdown_;
  edm::Lumi3DReWeighting LumiWeightsup_;
  
      TFile*   scfactorfile;
      TH2D*    f2Delecsc;
      TH2D*    f2Djetsc;
      TH2D*    f2Djetasysc;
      TH1D*    f1Dpfmhtsc;
      TFile*   efffile;
      TH2D*    f2Djetdataeff;
      TH2D*    f2Djetmceff;
      TFile*   btagefffile;
      TH2D*    f2Dttbarbtag;
      TH2D*    f2Dttbarctag;
      TH2D*    f2Dttbarlighttag;
      TH2D*    f2Dwprimebtag;
      TH2D*    f2Dwprimectag;
      TH2D*    f2Dwprimelighttag;
      //Medium
      LOTable  bSF_table;
      LOTable  bSF_table_error;
      LOTable  lSF_table;
      LOTable  lSF_table_error_down;
      LOTable  lSF_table_error_up;
      LOTable  leff_table;
      //Loose
      LOTable  bSF_table_loose;
      LOTable  bSF_table_error_loose;
      LOTable  lSF_table_loose;
      LOTable  lSF_table_error_down_loose;
      LOTable  lSF_table_error_up_loose;
      LOTable  leff_table_loose;
      //Tight
      LOTable  bSF_table_tight;
      LOTable  bSF_table_error_tight;
      LOTable  lSF_table_tight;
      LOTable  lSF_table_error_down_tight;
      LOTable  lSF_table_error_up_tight;
      LOTable  leff_table_tight;

      //Fast Sim Btag correction factor
      //Medium
      LOTable  bCF_table;
      LOTable  bCF_table_error;
      LOTable  cCF_table;
      LOTable  cCF_table_error;
      LOTable  lCF_table_error;
      LOTable  leff_CF_table;
      //Loose
      /*LOTable  bCF_table_loose;
      LOTable  bCF_table_error_loose;
      LOTable  cCF_table_loose;
      LOTable  cCF_table_error_loose;
      LOTable  lCF_table_error_loose;
      LOTable  leff_CF_table_loose;
      */
      //Tight
      LOTable  bCF_table_tight;
      LOTable  bCF_table_error_tight;
      LOTable  cCF_table_tight;
      LOTable  cCF_table_error_tight;
      LOTable  lCF_table_error_tight;
      LOTable  leff_CF_table_tight;

      map<string, TH1*> jethist;
      map<string, TH2*> btageff;

public :

   JetCorrectionUncertainty *fJECunc;
   TTree            *fChain;   //!pointer to the analyzed TTree or TChain
   TopEventNtuple   *ntuple;
   StoreTreeVariable *MyStoreTree;  // My Storing Tree
   TFile            *fFile;
   TProofOutputFile *fProofFile; // For optimized merging of the ntuple
   MuonSelector     fMuSelector;
   ElectronSelector fEleSelector;
   METzCalculator fzCalculator;
   map< string, double > cutmap;

   Analyzer(TTree * /*tree*/ =0):h1test(0),fChain(0),ntuple(),fFile(0),fProofFile(0),fMuSelector(),fEleSelector()
     {
       fChannel = 1; //default mu+jets
       fVerbose = false;
       fIsMC = true;
       fSample = "";
       fOutdir = "";
       fdoQCD1SideBand = false;
       fdoQCD2SideBand = false;
       fdoJECunc = false;
       fdoJECup = true;
       fdoJERunc = true;//false;
       fdoJERup = false; 
       fdoJERdown = false; 
       fdodiLepton = false; 
       fdoMtopCut = true;
       fpu_up = false;
       fpu_down = false;
       fbtag_up = false;
       fbtag_down = false;
     }
   virtual ~Analyzer() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();
   
   ClassDef(Analyzer,0);
};

#endif


//#ifdef Analyzer_cxx
void Analyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   Info("Init","tree: %p", tree);

   // Set branch addresses and branch pointers
   if (!tree) return;
   
   fChain = tree;
   fChain->SetBranchAddress("top.", &ntuple); 
}

Bool_t Analyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

//#endif // #ifdef Analyzer_cxx
