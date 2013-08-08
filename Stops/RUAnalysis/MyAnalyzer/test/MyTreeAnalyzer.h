//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Jul 28 12:41:56 2013 by ROOT version 5.32/00
// from TTree EvTree/EvTree
// found on file: /cms/gomez/Stops/Ntuples/st2_h_bb_st1_bj_750_100_tree.root
//////////////////////////////////////////////////////////

#ifndef MyTreeAnalyzer_h
#define MyTreeAnalyzer_h

#include <memory>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <functional>
#include <map>

// ROOT
#include <TROOT.h>
#include "TH1.h"
#include "TH2.h"
#include <TString.h> 
#include <TFile.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TH3D.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TRandom.h>
#include <TBranch.h>
#include "TVector3.h"
#include <TChain.h>
#include <TSelector.h>
#include <TProofOutputFile.h>
#include <TSystem.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class MyTreeAnalyzer : public TSelector {
private:
   void			ParseInput();
   TString		fMyOpt;
   TString		fSample;
   Int_t		fSt1;
   TH1F			*h1test;
   TH1D			*hcutflow;
   map< string, TH1*> basicPlots;
   map< string, TH1*> step1plots1D;
   map< string, TH2*> step1plots2D;
   map< string, TH1*> step2plots1D;
   map< string, TH2*> step2plots2D;
   map< string, TH1*> step3plots1D;
   map< string, TH2*> step3plots2D;
   vector< string > fCutLabels;
     
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Int_t           entry;
   Int_t           run;
   Int_t           event;
   Int_t           lumis;
   Bool_t          HasSelTrigger;
   Bool_t          HasBaseTrigger;
   Bool_t          DataIs;
   Double_t        MSquark;
   Double_t        MLSP;
   Float_t         nTruePileUp;
   Int_t           nGoodVtx;
   Int_t           nGenPart;
   Int_t           pdgID[1];   //
   Float_t         MCpx[1];   //[nGenPart]
   Float_t         MCpy[1];   //[nGenPart]
   Float_t         MCpz[1];   //[nGenPart]
   Float_t         MCe[1];   //[nGenPart]
   Int_t           MCmotherind[1];   //[nGenPart]
   Int_t           nElectrons;
   Int_t           nMuons;
   Int_t           nPhotons;
   Int_t           nTriplets;
   Int_t           nPFJets;
   Float_t         jet_PF_pt[100];   //[nPFJets]
   Float_t         jet_PF_et[100];   //[nPFJets]
   Float_t         jet_PF_eta[100];   //[nPFJets]
   Float_t         jet_PF_phi[100];   //[nPFJets]
   Float_t         jet_PF_theta[100];   //[nPFJets]
   Float_t         jet_PF_mass[100];   //[nPFJets]
   Float_t         jet_PF_NeutralHad[100];   //[nPFJets]
   Float_t         jet_PF_area[100];   //[nPFJets]
   Float_t         jet_PF_nJetDaughters[100];   //[nPFJets]
   Int_t           jet_PF_PartonFlav[100];   //[nPFJets]
   Int_t           jet_PF_JetMom[100];   //[nPFJets]
   Float_t         jet_PF_px[100];   //[nPFJets]
   Float_t         jet_PF_py[100];   //[nPFJets]
   Float_t         jet_PF_pz[100];   //[nPFJets]
   Float_t         jet_PF_e[100];   //[nPFJets]
   Float_t         jet_PF_jec_unc[100];   //[nPFJets]
   Float_t         bdiscTCHE_PF[100];   //[nPFJets]
   Float_t         bdiscTCHP_PF[100];   //[nPFJets]
   Float_t         bdiscSSVHE_PF[100];   //[nPFJets]
   Float_t         bdiscSSSVHP_PF[100];   //[nPFJets]
   Float_t         bdiscCSV_PF[100];   //[nPFJets]
   Float_t         bdiscJP_PF[100];   //[nPFJets]
   Int_t           nCA8PFJets;
   Float_t         jet_CA8PF_pt[15];   //[nCA8PFJets]
   Float_t         jet_CA8PF_et[15];   //[nCA8PFJets]
   Float_t         jet_CA8PF_eta[15];   //[nCA8PFJets]
   Float_t         jet_CA8PF_phi[15];   //[nCA8PFJets]
   Float_t         jet_CA8PF_theta[15];   //[nCA8PFJets]
   Float_t         jet_CA8PF_mass[15];   //[nCA8PFJets]
   Float_t         jet_CA8PF_area[15];   //[nCA8PFJets]
   Float_t         jet_CA8PF_nJetDaughters[15];   //[nCA8PFJets]
   Float_t         jet_CA8PF_px[15];   //[nCA8PFJets]
   Float_t         jet_CA8PF_py[15];   //[nCA8PFJets]
   Float_t         jet_CA8PF_pz[15];   //[nCA8PFJets]
   Float_t         jet_CA8PF_e[15];   //[nCA8PFJets]
   Float_t         bdiscTCHE_CA8PF[15];   //[nCA8PFJets]
   Float_t         bdiscTCHP_CA8PF[15];   //[nCA8PFJets]
   Float_t         bdiscSSVHE_CA8PF[15];   //[nCA8PFJets]
   Float_t         bdiscSSSVHP_CA8PF[15];   //[nCA8PFJets]
   Float_t         bdiscCSV_CA8PF[15];   //[nCA8PFJets]
   Float_t         bdiscJP_CA8PF[15];   //[nCA8PFJets]
   Int_t           nCA8PrunedPFJets;
   Float_t         jet_CA8PrunedPF_pt[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_et[15];   //[nCA8PFJets]
   Float_t         jet_CA8PrunedPF_eta[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_phi[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_theta[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_mass[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_area[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_nJetDaughters[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_subJet1_mass[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_subJet2_mass[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_subJet3_mass[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_subJet1_px[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_subJet2_px[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_subJet3_px[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_subJet1_py[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_subJet2_py[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_subJet3_py[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_subJet1_pz[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_subJet2_pz[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_subJet3_pz[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_subJet1_e[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_subJet2_e[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_subJet3_e[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_px[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_py[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_pz[13];   //[nCA8PrunedPFJets]
   Float_t         jet_CA8PrunedPF_e[13];   //[nCA8PrunedPFJets]
   Float_t         bdiscTCHE_CA8PrunedPF[13];   //[nCA8PrunedPFJets]
   Float_t         bdiscTCHP_CA8PrunedPF[13];   //[nCA8PrunedPFJets]
   Float_t         bdiscSSVHE_CA8PrunedPF[13];   //[nCA8PrunedPFJets]
   Float_t         bdiscSSSVHP_CA8PrunedPF[13];   //[nCA8PrunedPFJets]
   Float_t         bdiscCSV_CA8PrunedPF[13];   //[nCA8PrunedPFJets]
   Float_t         bdiscJP_CA8PrunedPF[13];   //[nCA8PrunedPFJets]
   Float_t         epx[1];   //[nElectrons]
   Float_t         epy[1];   //[nElectrons]
   Float_t         epz[1];   //[nElectrons]
   Float_t         ee[1];   //[nElectrons]
   Float_t         ept[1];   //[nElectrons]
   Int_t           echarge[1];   //[nElectrons]
   Float_t         mpx[1];   //[nMuons]
   Float_t         mpy[1];   //[nMuons]
   Float_t         mpz[1];   //[nMuons]
   Float_t         me[1];   //[nMuons]
   Float_t         mpt[1];   //[nMuons]
   Float_t         mcharge[1];   //[nMuons]
   Float_t         mPFIso[1];   //[nMuons]
   Float_t         phpt[1];   //[nPhotons]
   Float_t         phpx[1];   //[nPhotons]
   Float_t         phpy[1];   //[nPhotons]
   Float_t         phpz[1];   //[nPhotons]
   Float_t         phe[1];   //[nPhotons]
   Float_t         pfMET;
   Float_t         pfMETphi;

   // List of branches
   TBranch        *b_entry;   //!
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_lumis;   //!
   TBranch        *b_HasSelTrigger;   //!
   TBranch        *b_HasBaseTrigger;   //!
   TBranch        *b_DataIs;   //!
   TBranch        *b_MSquark;   //!
   TBranch        *b_MLSP;   //!
   TBranch        *b_nTruePileUp;   //!
   TBranch        *b_nGoodVtx;   //!
   TBranch        *b_nGenPart;   //!
   TBranch        *b_pdgID;   //!
   TBranch        *b_MCpx;   //!
   TBranch        *b_MCpy;   //!
   TBranch        *b_MCpz;   //!
   TBranch        *b_MCe;   //!
   TBranch        *b_MCmotherind;   //!
   TBranch        *b_nElectrons;   //!
   TBranch        *b_nMuons;   //!
   TBranch        *b_nPhotons;   //!
   TBranch        *b_nTriplets;   //!
   TBranch        *b_nPFJets;   //!
   TBranch        *b_jet_PF_pt;   //!
   TBranch        *b_jet_PF_et;   //!
   TBranch        *b_jet_PF_eta;   //!
   TBranch        *b_jet_PF_phi;   //!
   TBranch        *b_jet_PF_theta;   //!
   TBranch        *b_jet_PF_mass;   //!
   TBranch        *b_jet_PF_NeutralHad;   //!
   TBranch        *b_jet_PF_area;   //!
   TBranch        *b_jet_PF_nJetDaughters;   //!
   TBranch        *b_jet_PF_PartonFlav;   //!
   TBranch        *b_jet_PF_JetMom;   //!
   TBranch        *b_jet_PF_px;   //!
   TBranch        *b_jet_PF_py;   //!
   TBranch        *b_jet_PF_pz;   //!
   TBranch        *b_jet_PF_e;   //!
   TBranch        *b_jet_PF_jec_unc;   //!
   TBranch        *b_bdiscTCHE_PF;   //!
   TBranch        *b_bdiscTCHP_PF;   //!
   TBranch        *b_bdiscSSVHE_PF;   //!
   TBranch        *b_bdiscSSSVHP_PF;   //!
   TBranch        *b_bdiscCSV_PF;   //!
   TBranch        *b_bdiscJP_PF;   //!
   TBranch        *b_nCA8PFJets;   //!
   TBranch        *b_jet_CA8PF_pt;   //!
   TBranch        *b_jet_CA8PF_et;   //!
   TBranch        *b_jet_CA8PF_eta;   //!
   TBranch        *b_jet_CA8PF_phi;   //!
   TBranch        *b_jet_CA8PF_theta;   //!
   TBranch        *b_jet_CA8PF_mass;   //!
   TBranch        *b_jet_CA8PF_area;   //!
   TBranch        *b_jet_CA8PF_nJetDaughters;   //!
   TBranch        *b_jet_CA8PF_px;   //!
   TBranch        *b_jet_CA8PF_py;   //!
   TBranch        *b_jet_CA8PF_pz;   //!
   TBranch        *b_jet_CA8PF_e;   //!
   TBranch        *b_bdiscTCHE_CA8PF;   //!
   TBranch        *b_bdiscTCHP_CA8PF;   //!
   TBranch        *b_bdiscSSVHE_CA8PF;   //!
   TBranch        *b_bdiscSSSVHP_CA8PF;   //!
   TBranch        *b_bdiscCSV_CA8PF;   //!
   TBranch        *b_bdiscJP_CA8PF;   //!
   TBranch        *b_nCA8PrunedPFJets;   //!
   TBranch        *b_jet_CA8PrunedPF_pt;   //!
   TBranch        *b_jet_CA8PrunedPF_et;   //!
   TBranch        *b_jet_CA8PrunedPF_eta;   //!
   TBranch        *b_jet_CA8PrunedPF_phi;   //!
   TBranch        *b_jet_CA8PrunedPF_theta;   //!
   TBranch        *b_jet_CA8PrunedPF_mass;   //!
   TBranch        *b_jet_CA8PrunedPF_area;   //!
   TBranch        *b_jet_CA8PrunedPF_nJetDaughters;   //!
   TBranch        *b_jet_CA8PrunedPF_subJet1_mass;   //!
   TBranch        *b_jet_CA8PrunedPF_subJet2_mass;   //!
   TBranch        *b_jet_CA8PrunedPF_subJet3_mass;   //!
   TBranch        *b_jet_CA8PrunedPF_subJet1_px;   //!
   TBranch        *b_jet_CA8PrunedPF_subJet2_px;   //!
   TBranch        *b_jet_CA8PrunedPF_subJet3_px;   //!
   TBranch        *b_jet_CA8PrunedPF_subJet1_py;   //!
   TBranch        *b_jet_CA8PrunedPF_subJet2_py;   //!
   TBranch        *b_jet_CA8PrunedPF_subJet3_py;   //!
   TBranch        *b_jet_CA8PrunedPF_subJet1_pz;   //!
   TBranch        *b_jet_CA8PrunedPF_subJet2_pz;   //!
   TBranch        *b_jet_CA8PrunedPF_subJet3_pz;   //!
   TBranch        *b_jet_CA8PrunedPF_subJet1_e;   //!
   TBranch        *b_jet_CA8PrunedPF_subJet2_e;   //!
   TBranch        *b_jet_CA8PrunedPF_subJet3_e;   //!
   TBranch        *b_jet_CA8PrunedPF_px;   //!
   TBranch        *b_jet_CA8PrunedPF_py;   //!
   TBranch        *b_jet_CA8PrunedPF_pz;   //!
   TBranch        *b_jet_CA8PrunedPF_e;   //!
   TBranch        *b_bdiscTCHE_CA8PrunedPF;   //!
   TBranch        *b_bdiscTCHP_CA8PrunedPF;   //!
   TBranch        *b_bdiscSSVHE_CA8PrunedPF;   //!
   TBranch        *b_bdiscSSSVHP_CA8PrunedPF;   //!
   TBranch        *b_bdiscCSV_CA8PrunedPF;   //!
   TBranch        *b_bdiscJP_CA8PrunedPF;   //!
   TBranch        *b_epx;   //!
   TBranch        *b_epy;   //!
   TBranch        *b_epz;   //!
   TBranch        *b_ee;   //!
   TBranch        *b_ept;   //!
   TBranch        *b_echarge;   //!
   TBranch        *b_mpx;   //!
   TBranch        *b_mpy;   //!
   TBranch        *b_mpz;   //!
   TBranch        *b_me;   //!
   TBranch        *b_mpt;   //!
   TBranch        *b_mcharge;   //!
   TBranch        *b_mPFIso;   //!
   TBranch        *b_phpt;   //!
   TBranch        *b_phpx;   //!
   TBranch        *b_phpy;   //!
   TBranch        *b_phpz;   //!
   TBranch        *b_phe;   //!
   TBranch        *b_pfMET;   //!
   TBranch        *b_pfMETphi;   //!

   TFile            *fFile;
   TProofOutputFile *fProofFile; // For optimized merging of the ntuple
   map< string, double > cutmap;

   MyTreeAnalyzer(TTree * /*tree*/ =0) : h1test(0),fChain(0),fFile(0),fProofFile(0) {
	   fSample = "";
	   fSt1 = 0;
   }
   virtual ~MyTreeAnalyzer() { }
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

   ClassDef(MyTreeAnalyzer,0);

};

//
// constants, enums and typedefs
//

// Structure that contains Jet info
typedef struct {
	std::vector< TLorentzVector > p4SmallMassDiff;
	std::vector< double > mass;
	std::vector< double > pt;
	std::vector< double > scalarPt;
	double avgMass;
	double avgPt;
} smallMassDiffVectors;

typedef struct {
	std::vector< TLorentzVector > s3VectorTL;
	std::vector< double > s3Mass;
	std::vector< double > s3ScalarPt;
	//double avgMass;
	//double avgPt;
} s3VecMassSumPt;


// Comparison function for jet list. Want highest pt first
bool ComparePt(TLorentzVector a, TLorentzVector b) { return a.Pt() > b.Pt(); }
#endif

#ifdef MyTreeAnalyzer_cxx
void MyTreeAnalyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("entry", &entry, &b_entry);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("lumis", &lumis, &b_lumis);
   fChain->SetBranchAddress("HasSelTrigger", &HasSelTrigger, &b_HasSelTrigger);
   fChain->SetBranchAddress("HasBaseTrigger", &HasBaseTrigger, &b_HasBaseTrigger);
   fChain->SetBranchAddress("DataIs", &DataIs, &b_DataIs);
   fChain->SetBranchAddress("MSquark", &MSquark, &b_MSquark);
   fChain->SetBranchAddress("MLSP", &MLSP, &b_MLSP);
   fChain->SetBranchAddress("nTruePileUp", &nTruePileUp, &b_nTruePileUp);
   fChain->SetBranchAddress("nGoodVtx", &nGoodVtx, &b_nGoodVtx);
   fChain->SetBranchAddress("nGenPart", &nGenPart, &b_nGenPart);
   fChain->SetBranchAddress("pdgID[nGenPart]", &pdgID, &b_pdgID);
   fChain->SetBranchAddress("MCpx[nGenPart]", &MCpx, &b_MCpx);
   fChain->SetBranchAddress("MCpy[nGenPart]", &MCpy, &b_MCpy);
   fChain->SetBranchAddress("MCpz[nGenPart]", &MCpz, &b_MCpz);
   fChain->SetBranchAddress("MCe[nGenPart]", &MCe, &b_MCe);
   fChain->SetBranchAddress("MCmotherind[nGenPart]", &MCmotherind, &b_MCmotherind);
   fChain->SetBranchAddress("nElectrons", &nElectrons, &b_nElectrons);
   fChain->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
   fChain->SetBranchAddress("nPhotons", &nPhotons, &b_nPhotons);
   fChain->SetBranchAddress("nTriplets", &nTriplets, &b_nTriplets);
   fChain->SetBranchAddress("nPFJets", &nPFJets, &b_nPFJets);
   fChain->SetBranchAddress("jet_PF_pt[nPFJets]", jet_PF_pt, &b_jet_PF_pt);
   fChain->SetBranchAddress("jet_PF_et[nPFJets]", jet_PF_et, &b_jet_PF_et);
   fChain->SetBranchAddress("jet_PF_eta[nPFJets]", jet_PF_eta, &b_jet_PF_eta);
   fChain->SetBranchAddress("jet_PF_phi[nPFJets]", jet_PF_phi, &b_jet_PF_phi);
   fChain->SetBranchAddress("jet_PF_theta[nPFJets]", jet_PF_theta, &b_jet_PF_theta);
   fChain->SetBranchAddress("jet_PF_mass[nPFJets]", jet_PF_mass, &b_jet_PF_mass);
   fChain->SetBranchAddress("jet_PF_NeutralHad[nPFJets]", jet_PF_NeutralHad, &b_jet_PF_NeutralHad);
   fChain->SetBranchAddress("jet_PF_area[nPFJets]", jet_PF_area, &b_jet_PF_area);
   fChain->SetBranchAddress("jet_PF_nJetDaughters[nPFJets]", jet_PF_nJetDaughters, &b_jet_PF_nJetDaughters);
   fChain->SetBranchAddress("jet_PF_PartonFlav[nPFJets]", jet_PF_PartonFlav, &b_jet_PF_PartonFlav);
   fChain->SetBranchAddress("jet_PF_JetMom[nPFJets]", jet_PF_JetMom, &b_jet_PF_JetMom);
   fChain->SetBranchAddress("jet_PF_px[nPFJets]", jet_PF_px, &b_jet_PF_px);
   fChain->SetBranchAddress("jet_PF_py[nPFJets]", jet_PF_py, &b_jet_PF_py);
   fChain->SetBranchAddress("jet_PF_pz[nPFJets]", jet_PF_pz, &b_jet_PF_pz);
   fChain->SetBranchAddress("jet_PF_e[nPFJets]", jet_PF_e, &b_jet_PF_e);
   fChain->SetBranchAddress("jet_PF_jec_unc[nPFJets]", jet_PF_jec_unc, &b_jet_PF_jec_unc);
   fChain->SetBranchAddress("bdiscTCHE_PF[nPFJets]", bdiscTCHE_PF, &b_bdiscTCHE_PF);
   fChain->SetBranchAddress("bdiscTCHP_PF[nPFJets]", bdiscTCHP_PF, &b_bdiscTCHP_PF);
   fChain->SetBranchAddress("bdiscSSVHE_PF[nPFJets]", bdiscSSVHE_PF, &b_bdiscSSVHE_PF);
   fChain->SetBranchAddress("bdiscSSSVHP_PF[nPFJets]", bdiscSSSVHP_PF, &b_bdiscSSSVHP_PF);
   fChain->SetBranchAddress("bdiscCSV_PF[nPFJets]", bdiscCSV_PF, &b_bdiscCSV_PF);
   fChain->SetBranchAddress("bdiscJP_PF[nPFJets]", bdiscJP_PF, &b_bdiscJP_PF);
   fChain->SetBranchAddress("nCA8PFJets", &nCA8PFJets, &b_nCA8PFJets);
   fChain->SetBranchAddress("jet_CA8PF_pt[nCA8PFJets]", jet_CA8PF_pt, &b_jet_CA8PF_pt);
   fChain->SetBranchAddress("jet_CA8PF_et[nCA8PFJets]", jet_CA8PF_et, &b_jet_CA8PF_et);
   fChain->SetBranchAddress("jet_CA8PF_eta[nCA8PFJets]", jet_CA8PF_eta, &b_jet_CA8PF_eta);
   fChain->SetBranchAddress("jet_CA8PF_phi[nCA8PFJets]", jet_CA8PF_phi, &b_jet_CA8PF_phi);
   fChain->SetBranchAddress("jet_CA8PF_theta[nCA8PFJets]", jet_CA8PF_theta, &b_jet_CA8PF_theta);
   fChain->SetBranchAddress("jet_CA8PF_mass[nCA8PFJets]", jet_CA8PF_mass, &b_jet_CA8PF_mass);
   fChain->SetBranchAddress("jet_CA8PF_area[nCA8PFJets]", jet_CA8PF_area, &b_jet_CA8PF_area);
   fChain->SetBranchAddress("jet_CA8PF_nJetDaughters[nCA8PFJets]", jet_CA8PF_nJetDaughters, &b_jet_CA8PF_nJetDaughters);
   fChain->SetBranchAddress("jet_CA8PF_px[nCA8PFJets]", jet_CA8PF_px, &b_jet_CA8PF_px);
   fChain->SetBranchAddress("jet_CA8PF_py[nCA8PFJets]", jet_CA8PF_py, &b_jet_CA8PF_py);
   fChain->SetBranchAddress("jet_CA8PF_pz[nCA8PFJets]", jet_CA8PF_pz, &b_jet_CA8PF_pz);
   fChain->SetBranchAddress("jet_CA8PF_e[nCA8PFJets]", jet_CA8PF_e, &b_jet_CA8PF_e);
   fChain->SetBranchAddress("bdiscTCHE_CA8PF[nCA8PFJets]", bdiscTCHE_CA8PF, &b_bdiscTCHE_CA8PF);
   fChain->SetBranchAddress("bdiscTCHP_CA8PF[nCA8PFJets]", bdiscTCHP_CA8PF, &b_bdiscTCHP_CA8PF);
   fChain->SetBranchAddress("bdiscSSVHE_CA8PF[nCA8PFJets]", bdiscSSVHE_CA8PF, &b_bdiscSSVHE_CA8PF);
   fChain->SetBranchAddress("bdiscSSSVHP_CA8PF[nCA8PFJets]", bdiscSSSVHP_CA8PF, &b_bdiscSSSVHP_CA8PF);
   fChain->SetBranchAddress("bdiscCSV_CA8PF[nCA8PFJets]", bdiscCSV_CA8PF, &b_bdiscCSV_CA8PF);
   fChain->SetBranchAddress("bdiscJP_CA8PF[nCA8PFJets]", bdiscJP_CA8PF, &b_bdiscJP_CA8PF);
   fChain->SetBranchAddress("nCA8PrunedPFJets", &nCA8PrunedPFJets, &b_nCA8PrunedPFJets);
   fChain->SetBranchAddress("jet_CA8PrunedPF_pt[nCA8PrunedPFJets]", jet_CA8PrunedPF_pt, &b_jet_CA8PrunedPF_pt);
   fChain->SetBranchAddress("jet_CA8PrunedPF_et[nCA8PFJets]", jet_CA8PrunedPF_et, &b_jet_CA8PrunedPF_et);
   fChain->SetBranchAddress("jet_CA8PrunedPF_eta[nCA8PrunedPFJets]", jet_CA8PrunedPF_eta, &b_jet_CA8PrunedPF_eta);
   fChain->SetBranchAddress("jet_CA8PrunedPF_phi[nCA8PrunedPFJets]", jet_CA8PrunedPF_phi, &b_jet_CA8PrunedPF_phi);
   fChain->SetBranchAddress("jet_CA8PrunedPF_theta[nCA8PrunedPFJets]", jet_CA8PrunedPF_theta, &b_jet_CA8PrunedPF_theta);
   fChain->SetBranchAddress("jet_CA8PrunedPF_mass[nCA8PrunedPFJets]", jet_CA8PrunedPF_mass, &b_jet_CA8PrunedPF_mass);
   fChain->SetBranchAddress("jet_CA8PrunedPF_area[nCA8PrunedPFJets]", jet_CA8PrunedPF_area, &b_jet_CA8PrunedPF_area);
   fChain->SetBranchAddress("jet_CA8PrunedPF_nJetDaughters[nCA8PrunedPFJets]", jet_CA8PrunedPF_nJetDaughters, &b_jet_CA8PrunedPF_nJetDaughters);
   fChain->SetBranchAddress("jet_CA8PrunedPF_subJet1_mass[nCA8PrunedPFJets]", jet_CA8PrunedPF_subJet1_mass, &b_jet_CA8PrunedPF_subJet1_mass);
   fChain->SetBranchAddress("jet_CA8PrunedPF_subJet2_mass[nCA8PrunedPFJets]", jet_CA8PrunedPF_subJet2_mass, &b_jet_CA8PrunedPF_subJet2_mass);
   fChain->SetBranchAddress("jet_CA8PrunedPF_subJet3_mass[nCA8PrunedPFJets]", jet_CA8PrunedPF_subJet3_mass, &b_jet_CA8PrunedPF_subJet3_mass);
   fChain->SetBranchAddress("jet_CA8PrunedPF_subJet1_px[nCA8PrunedPFJets]", jet_CA8PrunedPF_subJet1_px, &b_jet_CA8PrunedPF_subJet1_px);
   fChain->SetBranchAddress("jet_CA8PrunedPF_subJet2_px[nCA8PrunedPFJets]", jet_CA8PrunedPF_subJet2_px, &b_jet_CA8PrunedPF_subJet2_px);
   fChain->SetBranchAddress("jet_CA8PrunedPF_subJet3_px[nCA8PrunedPFJets]", jet_CA8PrunedPF_subJet3_px, &b_jet_CA8PrunedPF_subJet3_px);
   fChain->SetBranchAddress("jet_CA8PrunedPF_subJet1_py[nCA8PrunedPFJets]", jet_CA8PrunedPF_subJet1_py, &b_jet_CA8PrunedPF_subJet1_py);
   fChain->SetBranchAddress("jet_CA8PrunedPF_subJet2_py[nCA8PrunedPFJets]", jet_CA8PrunedPF_subJet2_py, &b_jet_CA8PrunedPF_subJet2_py);
   fChain->SetBranchAddress("jet_CA8PrunedPF_subJet3_py[nCA8PrunedPFJets]", jet_CA8PrunedPF_subJet3_py, &b_jet_CA8PrunedPF_subJet3_py);
   fChain->SetBranchAddress("jet_CA8PrunedPF_subJet1_pz[nCA8PrunedPFJets]", jet_CA8PrunedPF_subJet1_pz, &b_jet_CA8PrunedPF_subJet1_pz);
   fChain->SetBranchAddress("jet_CA8PrunedPF_subJet2_pz[nCA8PrunedPFJets]", jet_CA8PrunedPF_subJet2_pz, &b_jet_CA8PrunedPF_subJet2_pz);
   fChain->SetBranchAddress("jet_CA8PrunedPF_subJet3_pz[nCA8PrunedPFJets]", jet_CA8PrunedPF_subJet3_pz, &b_jet_CA8PrunedPF_subJet3_pz);
   fChain->SetBranchAddress("jet_CA8PrunedPF_subJet1_e[nCA8PrunedPFJets]", jet_CA8PrunedPF_subJet1_e, &b_jet_CA8PrunedPF_subJet1_e);
   fChain->SetBranchAddress("jet_CA8PrunedPF_subJet2_e[nCA8PrunedPFJets]", jet_CA8PrunedPF_subJet2_e, &b_jet_CA8PrunedPF_subJet2_e);
   fChain->SetBranchAddress("jet_CA8PrunedPF_subJet3_e[nCA8PrunedPFJets]", jet_CA8PrunedPF_subJet3_e, &b_jet_CA8PrunedPF_subJet3_e);
   fChain->SetBranchAddress("jet_CA8PrunedPF_px[nCA8PrunedPFJets]", jet_CA8PrunedPF_px, &b_jet_CA8PrunedPF_px);
   fChain->SetBranchAddress("jet_CA8PrunedPF_py[nCA8PrunedPFJets]", jet_CA8PrunedPF_py, &b_jet_CA8PrunedPF_py);
   fChain->SetBranchAddress("jet_CA8PrunedPF_pz[nCA8PrunedPFJets]", jet_CA8PrunedPF_pz, &b_jet_CA8PrunedPF_pz);
   fChain->SetBranchAddress("jet_CA8PrunedPF_e[nCA8PrunedPFJets]", jet_CA8PrunedPF_e, &b_jet_CA8PrunedPF_e);
   fChain->SetBranchAddress("bdiscTCHE_CA8PrunedPF[nCA8PrunedPFJets]", bdiscTCHE_CA8PrunedPF, &b_bdiscTCHE_CA8PrunedPF);
   fChain->SetBranchAddress("bdiscTCHP_CA8PrunedPF[nCA8PrunedPFJets]", bdiscTCHP_CA8PrunedPF, &b_bdiscTCHP_CA8PrunedPF);
   fChain->SetBranchAddress("bdiscSSVHE_CA8PrunedPF[nCA8PrunedPFJets]", bdiscSSVHE_CA8PrunedPF, &b_bdiscSSVHE_CA8PrunedPF);
   fChain->SetBranchAddress("bdiscSSSVHP_CA8PrunedPF[nCA8PrunedPFJets]", bdiscSSSVHP_CA8PrunedPF, &b_bdiscSSSVHP_CA8PrunedPF);
   fChain->SetBranchAddress("bdiscCSV_CA8PrunedPF[nCA8PrunedPFJets]", bdiscCSV_CA8PrunedPF, &b_bdiscCSV_CA8PrunedPF);
   fChain->SetBranchAddress("bdiscJP_CA8PrunedPF[nCA8PrunedPFJets]", bdiscJP_CA8PrunedPF, &b_bdiscJP_CA8PrunedPF);
   fChain->SetBranchAddress("epx[nElectrons]", epx, &b_epx);
   fChain->SetBranchAddress("epy[nElectrons]", epy, &b_epy);
   fChain->SetBranchAddress("epz[nElectrons]", epz, &b_epz);
   fChain->SetBranchAddress("ee[nElectrons]", ee, &b_ee);
   fChain->SetBranchAddress("ept[nElectrons]", ept, &b_ept);
   fChain->SetBranchAddress("echarge[nElectrons]", echarge, &b_echarge);
   fChain->SetBranchAddress("mpx[nMuons]", mpx, &b_mpx);
   fChain->SetBranchAddress("mpy[nMuons]", mpy, &b_mpy);
   fChain->SetBranchAddress("mpz[nMuons]", mpz, &b_mpz);
   fChain->SetBranchAddress("me[nMuons]", me, &b_me);
   fChain->SetBranchAddress("mpt[nMuons]", mpt, &b_mpt);
   fChain->SetBranchAddress("mcharge[nMuons]", mcharge, &b_mcharge);
   fChain->SetBranchAddress("mPFIso[nMuons]", mPFIso, &b_mPFIso);
   fChain->SetBranchAddress("phpt[nPhotons]", &phpt, &b_phpt);
   fChain->SetBranchAddress("phpx[nPhotons]", &phpx, &b_phpx);
   fChain->SetBranchAddress("phpy[nPhotons]", &phpy, &b_phpy);
   fChain->SetBranchAddress("phpz[nPhotons]", &phpz, &b_phpz);
   fChain->SetBranchAddress("phe[nPhotons]", &phe, &b_phe);
   fChain->SetBranchAddress("pfMET", &pfMET, &b_pfMET);
   fChain->SetBranchAddress("pfMETphi", &pfMETphi, &b_pfMETphi);
}

Bool_t MyTreeAnalyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef MyTreeAnalyzer_cxx
