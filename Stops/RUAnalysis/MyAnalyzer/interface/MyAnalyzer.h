// system include files
#include <memory>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <functional>

// ROOT
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
#include <TROOT.h>
#include <TBranch.h>
#include "TVector3.h"
#include "TH1D.h"


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/Common/interface/HandleBase.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include <fstream>
#include <vector>
#include <functional>

//CMSSW includes
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/Jet.h" // based on DataFormats/Candidate/interface/Particle.h
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"  
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"


#include "DataFormats/PatCandidates/interface/Electron.h" 
#include "DataFormats/PatCandidates/interface/Muon.h" 
#include "DataFormats/PatCandidates/interface/MET.h" 
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaTowerIsolation.h"
//filtering susy events
//#include "UserCode/ModelFilter/interface/ModelFilter.h"

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
//pile upt stuff
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"



#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/FWLite/interface/Handle.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Common/interface/Ref.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"

#include "RUAnalysis/MyAnalyzer/interface/NtpReader.h"


using namespace std;

class MyAnalyzer : public edm::EDAnalyzer {
   public:
	   int   run;
	   int   event;
	   int   lumis;
	   explicit MyAnalyzer(const edm::ParameterSet&);
	   ~MyAnalyzer();



   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

	void BasicPlots( vector< TLorentzVector > p4GenTruthAlljets, vector< TLorentzVector > p4GenTruthJets, vector< TLorentzVector > p4GenTruthBjets, vector< TLorentzVector > p4GenTruthBjetsHiggs, vector< TLorentzVector > p4GenTruthB1Higgs, vector< TLorentzVector > p4GenTruthB2Higgs, vector< TLorentzVector > p4GenTruthJetsStop1, vector< TLorentzVector > p4GenTruthBjetsStop1, vector< TLorentzVector > p4MatchTruthJets,  vector< TLorentzVector > p4MatchTruthBjets, vector< TLorentzVector > p4MatchB1Higgs, vector< TLorentzVector > p4MatchB2Higgs, vector< TLorentzVector > p4RecoJets, vector< TLorentzVector > p4RecoBjets );
	void Analysis(  vector< TLorentzVector > p4GenTruthB1Higgs, vector< TLorentzVector > p4GenTruthB2Higgs, vector< TLorentzVector > p4RecoJets, vector< TLorentzVector > p4RecoBjets, vector< TLorentzVector > p4RecoBtagJets_CSVM, vector< TLorentzVector > p4RecoPartonFlavorBtagJets_CSVM, vector< TLorentzVector > p4MatchTruthBjets,  vector< TLorentzVector > p4MatchB1Higgs, vector< TLorentzVector > p4MatchB2Higgs, vector< TLorentzVector > p4MatchTruthAlljets, vector<double> RecoBjets_CSVM );

	// Input definitions
	edm::InputTag Gensrc;
	std::string _patJetType;
	std::string st1decay;
	std::string _jetCorrectionService;
	double stop1Mass;
	double stop2Mass;

	// Matching Histos
	TH1D * h_MatchTruthAlljets_deltaR;
	TH1D * h_MatchTruthJets_deltaR;
	TH1D * h_MatchTruthBjets_deltaR;
	TH1D * h_MatchB1Higgs_deltaR;
	TH1D * h_MatchB2Higgs_deltaR;

	// Bjets Histos
	TH1D * h_genTruthBJets_num;
	TH1D * h_genTruthBJets_pt;
	TH1D * h_genTruthBJets_sumpt;
	TH1D * h_genTruthBJets_eta;
	TH1D * h_genTruthBJets_phi;
	TH1D * h_recoBJets_num;
	TH1D * h_recoBJets_pt;
	TH1D * h_recoBJets_sumpt;
	TH1D * h_recoBJets_eta;
	TH1D * h_recoBJets_phi;
	TH1D * h_matchBJets_num;
	TH1D * h_matchBJets_pt;
	TH1D * h_matchBJets_sumpt;
	TH1D * h_matchBJets_eta;
	TH1D * h_matchBJets_phi;
	TH1D * h_matchBJetsHiggs_num;
	TH1D * h_matchBJetsHiggs1_num;
	TH1D * h_matchBJetsHiggs2_num;
	TH1D * h_matchBJetsHiggs_mass;
	TH1D * h_matchBJetsHiggs_pt;
	TH1D * h_matchBJetsHiggs_sumpt;
	TH1D * h_matchBJetsHiggs_eta;
	TH1D * h_matchBJetsHiggs_phi;

	// Jets Histos
	TH1D * h_genTruthJets_num;
	TH1D * h_genTruthJets_pt;
	TH1D * h_genTruthJets_sumpt;
	TH1D * h_genTruthJets_eta;
	TH1D * h_genTruthJets_phi;
	TH1D * h_recoJets_num;
	TH1D * h_recoJets_pt;
	TH1D * h_recoJets_sumpt;
	TH1D * h_recoJets_eta;
	TH1D * h_recoJets_phi;
	TH1D * h_matchJets_num;
	TH1D * h_matchJets_pt;
	TH1D * h_matchJets_sumpt;
	TH1D * h_matchJets_eta;
	TH1D * h_matchJets_phi;

	// Bjets Histos
	TH1D * h_genTruthAlljets_num;
	TH1D * h_genTruthAlljets_pt;
	TH1D * h_genTruthAlljets_sumpt;
	TH1D * h_genTruthAlljets_eta;
	TH1D * h_genTruthAlljets_phi;

	// Higgs histos
	TH1D * h_bjetsHiggs_pt;
	TH1D * h_bjetsHiggs_eta;
	TH1D * h_bjetsHiggs_phi;
	TH1D * h_genTruthBJetsHiggs_mass;
	TH1D * h_genTruthBJetsHiggs_num;
	TH1D * h_higgs1_mass;
	TH1D * h_higgs1_pt;
	TH1D * h_higgs1_eta;
	TH1D * h_higgs2_mass;
	TH1D * h_higgs2_pt;
	TH1D * h_higgs2_eta;
	TH1D * h_higgs_deltaR;
	TH1D * h_higgs_deltaPhi;
	TH1D * h_higgs_cosDeltaPhi;

	// Stop1 histos
	TH1D * h_bjetsStop1_pt;
	TH1D * h_bjetsStop1_eta;
	TH1D * h_bjetsStop1_phi;
	TH1D * h_stop11_mass;
	TH1D * h_stop11_pt;
	TH1D * h_stop11_eta;
	TH1D * h_stop12_mass;
	TH1D * h_stop12_pt;
	TH1D * h_stop12_eta;
	TH1D * h_stop1_deltaR;
	TH1D * h_stop1_deltaPhi;
	TH1D * h_stop1_cosDeltaPhi;


	///// Stop2 histos
	TH1D * h_stop21_mass;
	TH1D * h_stop21_pt;
	TH1D * h_stop21_eta;
	TH1D * h_stop22_mass;
	TH1D * h_stop22_pt;
	TH1D * h_stop22_eta;

	///// RECO histos
	TH1D * h_massRecobb;
	TH1D * h_massRecobb_num;
	TH1D * h_massRecobb_cut50;
	TH1D * h_massRecobb_cut100;
	TH1D * h_massRecobb_cut150;
	TH1D * h_massRecobb_cut200;
	TH2D * h_scalarPtRecobb;
	TH2D * h_scalarPtRecobb_cut50;
	TH2D * h_scalarPtRecobb_cut100;
	TH2D * h_scalarPtRecobb_cut150;
	TH2D * h_scalarPtRecobb_cut200;
	TH2D * h_PtRecobb;
	TH1D * h_massRecoBtagJets_CSVM;
	TH1D * h_massRecoBtagJets_CSVM_cut50;
	TH1D * h_massRecoBtagJets_CSVM_cut100;
	TH1D * h_massRecoBtagJets_CSVM_cut150;
	TH1D * h_massRecoBtagJets_CSVM_cut200;
	TH2D * h_scalarPtRecoBtagJets_CSVM;
	TH2D * h_scalarPtRecoBtagJets_CSVM_cut50;
	TH2D * h_scalarPtRecoBtagJets_CSVM_cut100;
	TH2D * h_scalarPtRecoBtagJets_CSVM_cut150;
	TH2D * h_scalarPtRecoBtagJets_CSVM_cut200;
	TH2D * h_PtRecoBtagJets_CSVM;
	TH1D * h_massRecoPartonFlavorBtagJets_CSVM;
	TH1D * h_massRecoPartonFlavorBtagJets_CSVM_cut50;
	TH1D * h_massRecoPartonFlavorBtagJets_CSVM_cut100;
	TH1D * h_massRecoPartonFlavorBtagJets_CSVM_cut150;
	TH1D * h_massRecoPartonFlavorBtagJets_CSVM_cut200;
	TH2D * h_scalarPtRecoPartonFlavorBtagJets_CSVM;
	TH2D * h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut50;
	TH2D * h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut100;
	TH2D * h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut150;
	TH2D * h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut200;
	TH2D * h_PtRecoPartonFlavorBtagJets_CSVM;
	TH1D * h_massbb;
	TH1D * h_massbb_cut50;
	TH1D * h_massbb_cut100;
	TH1D * h_massbb_cut150;
	TH1D * h_massbb_cut200;
	TH2D * h_scalarPtbb;
	TH2D * h_scalarPtbb_cut50;
	TH2D * h_scalarPtbb_cut100;
	TH2D * h_scalarPtbb_cut150;
	TH2D * h_scalarPtbb_cut200;
	TH2D * h_Ptbb;
	TH1D * h_massbbHiggs;
	TH1D * h_massbbHiggs_num;
	TH1D * h_massbbHiggs_cut50;
	TH1D * h_massbbHiggs_cut100;
	TH1D * h_massbbHiggs_cut150;
	TH1D * h_massbbHiggs_cut200;
	TH2D * h_scalarPtbbHiggs;
	TH2D * h_scalarPtbbHiggs_cut50;
	TH2D * h_scalarPtbbHiggs_cut100;
	TH2D * h_scalarPtbbHiggs_cut150;
	TH2D * h_scalarPtbbHiggs_cut200;
	TH2D * h_PtbbHiggs;
	TH1D * h_massGenTruthHiggs;
	TH2D * h_PtGenTruthHiggs;
	TH2D * h_scalarPtGenTruthHiggs;
	TH2D * h_matchjj_masspt;
	TH2D * h_matchjjWObb_masspt;
	TH2D * h_matchjjWObb_masspt_cut50;
	TH2D * h_matchjjWObb_masspt_cut100;
	TH2D * h_matchjjWObb_masspt_cut150;
	TH2D * h_matchjjWObb_masspt_cut200;
	TH1D * h_massmatchjjWObb;
	TH1D * h_massmatchjjWObb_cut50;
	TH1D * h_massmatchjjWObb_cut100;
	TH1D * h_massmatchjjWObb_cut150;
	TH1D * h_massmatchjjWObb_cut200;
	TH2D * h_matchjjHiggs_masspt;
	TH2D * h_matchjjWObbHiggs_masspt;
	TH2D * h_matchjjWObbHiggs_masspt_cut50;
	TH2D * h_matchjjWObbHiggs_masspt_cut100;
	TH2D * h_matchjjWObbHiggs_masspt_cut150;
	TH2D * h_matchjjWObbHiggs_masspt_cut200;
	TH1D * h_massmatchjjWObbHiggs;
	TH1D * h_massmatchjjWObbHiggs_cut50;
	TH1D * h_massmatchjjWObbHiggs_cut100;
	TH1D * h_massmatchjjWObbHiggs_cut150;
	TH1D * h_massmatchjjWObbHiggs_cut200;
	TH2D * h_Recojj_masspt;
	TH2D * h_jjWORecobb_masspt;
	TH2D * h_jjWORecobb_masspt_cut50;
	TH2D * h_jjWORecobb_masspt_cut100;
	TH2D * h_jjWORecobb_masspt_cut150;
	TH2D * h_jjWORecobb_masspt_cut200;
	TH1D * h_massjjWORecobb;
	TH1D * h_massjjWORecobb_cut50;
	TH1D * h_massjjWORecobb_cut100;
	TH1D * h_massjjWORecobb_cut150;
	TH1D * h_massjjWORecobb_cut200;
	TH2D * h_RecoBtagJetsjj_masspt;
	TH2D * h_jjWORecoBtagJets_CSVM_masspt;
	TH2D * h_jjWORecoBtagJets_CSVM_masspt_cut50;
	TH2D * h_jjWORecoBtagJets_CSVM_masspt_cut100;
	TH2D * h_jjWORecoBtagJets_CSVM_masspt_cut150;
	TH2D * h_jjWORecoBtagJets_CSVM_masspt_cut200;
	TH1D * h_massjjWORecoBtagJets_CSVM;
	TH1D * h_massjjWORecoBtagJets_CSVM_cut50;
	TH1D * h_massjjWORecoBtagJets_CSVM_cut100;
	TH1D * h_massjjWORecoBtagJets_CSVM_cut150;
	TH1D * h_massjjWORecoBtagJets_CSVM_cut200;

	TH2D * h_recojj_masspt;
	TH2D * h_recojjWObb_masspt;
	TH2D * h_recobbjj_masspt;
	TH2D * h_bbjj_masspt;
	TH1D * h_massrecobbjj;
	TH1D * h_massbbjj;


};

//
// constants, enums and typedefs
//

// Structure that contains Jet info
typedef struct {
	reco::Candidate::LorentzVector adjJet, diffVec;
	const pat::Jet *origJet;
	double jecUnc;
} jetElem;

typedef struct {
	vector< TLorentzVector > p4SmallMassDiff;
	vector< double > mass;
	vector< double > pt;
	vector< double > scalarPt;
} step1Vectors;


// Comparison function for jet list. Want highest pt first
bool cmpJets(jetElem first, jetElem second){
	return (first.adjJet.Pt() > second.adjJet.Pt());
}

bool ComparePt(TLorentzVector a, TLorentzVector b) { return a.Pt() > b.Pt(); }
//
// static data member definitions
//

//
// constructors and destructor
//
MyAnalyzer::MyAnalyzer(const edm::ParameterSet& iConfig){
	//_patJetType = iConfig.getUntrackedParameter<std::vector<std::string> >("PatJetType", std::vector<std::string> ());
	_patJetType = iConfig.getUntrackedParameter<string>("PatJetType", "goodPatJetsPFlow");
	_jetCorrectionService = iConfig.getUntrackedParameter<string> ("jetCorrectionService", "ak5PFL1L2L3");
	Gensrc = iConfig.getParameter<edm::InputTag>( "Gensrc" );
	st1decay = iConfig.getUntrackedParameter<string>("st1decay", "bj");
	stop1Mass = iConfig.getUntrackedParameter<double>( "st1Mass", 100 );
	stop2Mass = iConfig.getUntrackedParameter<double>( "st2Mass", 250 );

   //now do what ever initialization is needed
	edm::Service<TFileService> fs;						// Output File 
	TFileDirectory genTruthPlots = fs->mkdir( "GenTruthPlots" );
	TFileDirectory basicPlots = fs->mkdir( "BasicPlots" );
	TFileDirectory analysisPlots = fs->mkdir( "AnalysisPlots" );

	//cout << "hello" << endl;
	///// Matching Histos
	h_MatchTruthAlljets_deltaR = basicPlots.make<TH1D>("matchTruthAllJets_deltaR" , "#Delta R (RecoJets, All Truth GenJets)" , 100 , -0.5 , 5.5 );
	h_MatchTruthJets_deltaR = basicPlots.make<TH1D>("matchTruthJets_deltaR" , "#Delta R (RecoJets, Truth GenJets)" , 100 , -0.5 , 5.5 );
	h_MatchTruthBjets_deltaR = basicPlots.make<TH1D>("matchTruthBjets_deltaR" , "#Delta R (RecoBjets, Truth GenBjets)" , 100 , -0.5 , 5.5 );
	h_MatchB1Higgs_deltaR = basicPlots.make<TH1D>("matchB1Higgs_deltaR" , "#Delta R (RecoBjets, Higgs 1 Bjets)" , 100 , -0.5 , 5.5 );
	h_MatchB2Higgs_deltaR = basicPlots.make<TH1D>("matchB2Higgs_deltaR" , "#Delta R (RecoBjets, Higgs 2 Bjets)" , 100 , -0.5 , 5.5 );

	//// BJets histos
	h_genTruthBJets_num = genTruthPlots.make<TH1D>("genTruthBJets_num" , "Number of GenTruth bJets" , 11 , -0.5, 10.5 );
	h_genTruthBJets_pt = genTruthPlots.make<TH1D>("genTruthBJets_pt" , "GenTruth bJets p_{T}" , 98 , 0., 500. );
	h_genTruthBJets_sumpt = genTruthPlots.make<TH1D>("genTruthBJets_sumpt" , "#sum GenTruth bJets p_{T}" , 200 , 0., 1000. );
	h_genTruthBJets_eta = genTruthPlots.make<TH1D>("genTruthBJets_eta" , "GenTruth bJets #eta" , 40 , -5.0, 5.0 );
	h_genTruthBJets_phi = genTruthPlots.make<TH1D>("genTruthBJets_phi" , "GenTruth bJets #phi" , 40 , -5.0, 5.0 );
	h_recoBJets_num = basicPlots.make<TH1D>("recoBJets_num" , "Number of Reco bJets" , 11 , -0.5, 10.5 );
	h_recoBJets_pt = basicPlots.make<TH1D>("recoBJets_pt" , "Reco bJets p_{T}" , 98 , 0., 500. );
	h_recoBJets_sumpt = basicPlots.make<TH1D>("recoBJets_sumpt" , "#sum Reco bJets p_{T}" , 200 , 0., 1000. );
	h_recoBJets_eta = basicPlots.make<TH1D>("recoBJets_eta" , "Reco bJets #eta" , 40 , -5.0, 5.0 );
	h_recoBJets_phi = basicPlots.make<TH1D>("recoBJets_phi" , "Reco bJets #phi" , 40 , -5.0, 5.0 );
	h_matchBJets_num = basicPlots.make<TH1D>("matchBJets_num" , "Number of Match bJets" , 11 , -0.5, 10.5 );
	h_matchBJets_pt = basicPlots.make<TH1D>("matchBJets_pt" , "Match bJets p_{T}" , 98 , 0., 500. );
	h_matchBJets_sumpt = basicPlots.make<TH1D>("matchBJets_sumpt" , "#sum Match bJets p_{T}" , 200 , 0., 1000. );
	h_matchBJets_eta = basicPlots.make<TH1D>("matchBJets_eta" , "Match bJets #eta" , 40 , -5.0, 5.0 );
	h_matchBJets_phi = basicPlots.make<TH1D>("matchBJets_phi" , "Match bJets #phi" , 40 , -5.0, 5.0 );
	h_matchBJetsHiggs_num = basicPlots.make<TH1D>("matchBJetsHiggs_num" , "Number of Match Higgs bJets" , 11 , -0.5, 10.5 );
	h_matchBJetsHiggs1_num = basicPlots.make<TH1D>("matchBJetsHiggs1_num" , "Number of Match Higgs 1 bJets" , 11 , -0.5, 10.5 );
	h_matchBJetsHiggs2_num = basicPlots.make<TH1D>("matchBJetsHiggs2_num" , "Number of Match Higgs 2 bJets" , 11 , -0.5, 10.5 );
	h_matchBJetsHiggs_pt = basicPlots.make<TH1D>("matchBJetsHiggs_pt" , "Match Higgs bJets p_{T}" , 98 , 0., 500. );
	h_matchBJetsHiggs_mass = basicPlots.make<TH1D>("matchBJetsHiggs_mass" , "Match Higgs Mass" , 98 , 0., 300. );
	h_matchBJetsHiggs_sumpt = basicPlots.make<TH1D>("matchBJetsHiggs_sumpt" , "#sum Match bJets p_{T}" , 200 , 0., 1000. );
	h_matchBJetsHiggs_eta = basicPlots.make<TH1D>("matchBJetsHiggs_eta" , "Match Higgs bJets #eta" , 40 , -5.0, 5.0 );
	h_matchBJetsHiggs_phi = basicPlots.make<TH1D>("matchBJetsHiggs_phi" , "Match Higgs bJets #phi" , 40 , -5.0, 5.0 );

	//// Jets histos
	h_genTruthJets_num = genTruthPlots.make<TH1D>("genTruthJets_num" , "Number of GenTruth Jets" , 11 , -0.5, 10.5 );
	h_genTruthJets_pt = genTruthPlots.make<TH1D>("genTruthJets_pt" , "GenTruth Jets p_{T}" , 98 , 0., 500. );
	h_genTruthJets_sumpt = genTruthPlots.make<TH1D>("genTruthJets_sumpt" , "#sum GenTruth Jets p_{T}" , 200 , 0., 1000. );
	h_genTruthJets_eta = genTruthPlots.make<TH1D>("genTruthJets_eta" , "GenTruth Jets #eta" , 40 , -5.0, 5.0 );
	h_genTruthJets_phi = genTruthPlots.make<TH1D>("genTruthJets_phi" , "GenTruth Jets #phi" , 40 , -5.0, 5.0 );
	h_recoJets_num = basicPlots.make<TH1D>("recoJets_num" , "Number of Reco Jets" , 21 , -0.5, 20.5 );
	h_recoJets_pt = basicPlots.make<TH1D>("recoJets_pt" , "Reco Jets p_{T}" , 98 , 0., 500. );
	h_recoJets_sumpt = basicPlots.make<TH1D>("recoJets_sumpt" , "#sum Reco Jets p_{T}" , 200 , 0., 1000. );
	h_recoJets_eta = basicPlots.make<TH1D>("recoJets_eta" , "Reco Jets #eta" , 40 , -5.0, 5.0 );
	h_recoJets_phi = basicPlots.make<TH1D>("recoJets_phi" , "Reco Jets #phi" , 40 , -5.0, 5.0 );
	h_matchJets_num = basicPlots.make<TH1D>("matchJets_num" , "Number of Match Jets" , 21 , -0.5, 20.5 );
	h_matchJets_pt = basicPlots.make<TH1D>("matchJets_pt" , "Match Jets p_{T}" , 98 , 0., 500. );
	h_matchJets_sumpt = basicPlots.make<TH1D>("matchJets_sumpt" , "#sum Match Jets p_{T}" , 200 , 0., 1000. );
	h_matchJets_eta = basicPlots.make<TH1D>("matchJets_eta" , "Match Jets #eta" , 40 , -5.0, 5.0 );
	h_matchJets_phi = basicPlots.make<TH1D>("matchJets_phi" , "Match Jets #phi" , 40 , -5.0, 5.0 );

	//// Alljets histos
	h_genTruthAlljets_num = genTruthPlots.make<TH1D>("genTruthAlljets_num" , "Number of GenTruth Alljets" , 16 , -0.5, 15.5 );
	h_genTruthAlljets_pt = genTruthPlots.make<TH1D>("genTruthAlljets_pt" , "GenTruth Alljets p_{T}" , 98 , 0., 500. );
	h_genTruthAlljets_sumpt = genTruthPlots.make<TH1D>("genTruthAlljets_sumpt" , "#sum GenTruth Alljets p_{T}" , 200 , 0., 1000. );
	h_genTruthAlljets_eta = genTruthPlots.make<TH1D>("genTruthAlljets_eta" , "GenTruth Alljets #eta" , 40 , -5.0, 5.0 );
	h_genTruthAlljets_phi = genTruthPlots.make<TH1D>("genTruthAlljets_phi" , "GenTruth Alljets #phi" , 40 , -5.0, 5.0 );

	///// Higgs histos
	h_bjetsHiggs_pt = genTruthPlots.make<TH1D>("bjetsHiggs_pt" , "Bjets from Higgs p_{T}" , 98 , 0., 500. );
	h_bjetsHiggs_eta = genTruthPlots.make<TH1D>("bjetsHiggs_eta" , "Bjets from Higgs #eta" , 40 , -5.0, 5.0 );
	h_bjetsHiggs_phi = genTruthPlots.make<TH1D>("bjetsHiggs_phi" , "Bjets from Higgs #phi" , 40 , -5.0, 5.0 );
	h_genTruthBJetsHiggs_mass = genTruthPlots.make<TH1D>("genTruthBjetsHiggs_mass" , "GenTruth Higgs Mass" , 21.0 , 100.0, 200. );
	h_genTruthBJetsHiggs_num = genTruthPlots.make<TH1D>("genTruthBjetsHiggs_num" , "Number GenTruth Higgs" , 11 , -0.5, 10.5 );
	h_higgs1_mass = genTruthPlots.make<TH1D>("higgs1_mass" , "1st Higgs Mass" , 52.0 , 99.0, 150. );
	h_higgs1_pt = genTruthPlots.make<TH1D>("higgs1_pt" , "1st Higgs p_{T}" , 98 , 0.0, 500. );
	h_higgs1_eta = genTruthPlots.make<TH1D>("higgs1_eta" , "1st Higgs #eta" , 40 , -5.0, 5.0 );
	h_higgs2_mass = genTruthPlots.make<TH1D>("higgs2_mass" , "2nd Higgs Mass" , 52.0 , 99.0, 150. );
	h_higgs2_pt = genTruthPlots.make<TH1D>("higgs2_pt" , "2nd Higgs p_{T}" , 98 , 0., 500. );
	h_higgs2_eta = genTruthPlots.make<TH1D>("higgs2_eta" , "2nd Higgs #eta" , 40 , -5.0, 5.0 );
	h_higgs_deltaR = genTruthPlots.make<TH1D>("higgs_deltaR" , "#Delta R (h_{1},h_{2})" , 50 , 0., 5. );
	h_higgs_deltaPhi = genTruthPlots.make<TH1D>("higgs_deltaPhi" , "#Delta #phi (h_{1}, h_{2})" , 50 , 0., 5. );
	h_higgs_cosDeltaPhi = genTruthPlots.make<TH1D>("higgs_cosDeltaPhi" , "cos [#Delta #phi (h_{1}, h_{2})]" , 26 , -1.2, 1.2 );

	////// Stop1 histos
	h_bjetsStop1_pt = genTruthPlots.make<TH1D>("bjetsStop1_pt" , "Bjets from Stop1 p_{T}" , 98 , 0., 500. );
	h_bjetsStop1_eta = genTruthPlots.make<TH1D>("bjetsStop1_eta" , "Bjets from Stop1 #eta" , 40 , -5.0, 5.0 );
	h_bjetsStop1_phi = genTruthPlots.make<TH1D>("bjetsStop1_phi" , "Bjets from Stop1 #phi" , 40 , -5.0, 5.0 );
	h_stop11_mass = genTruthPlots.make<TH1D>("stop11_mass" , "1st Stop1 Mass" , 45.0 , 75.0, 525. );
	h_stop11_pt = genTruthPlots.make<TH1D>("stop11_pt" , "1st Stop1 p_{T}" , 98 , 0.0, 500. );
	h_stop11_eta = genTruthPlots.make<TH1D>("stop11_eta" , "1st Stop1 #eta" , 40 , -5.0, 5.0 );
	h_stop12_mass = genTruthPlots.make<TH1D>("stop12_mass" , "2nd Stop1 Mass" , 45.0 , 75.0, 525. );
	h_stop12_pt = genTruthPlots.make<TH1D>("stop12_pt" , "2nd Stop1 p_{T}" , 98 , 0., 500. );
	h_stop12_eta = genTruthPlots.make<TH1D>("stop12_eta" , "2nd Stop1 #eta" , 40 , -5.0, 5.0 );
	h_stop1_deltaR = genTruthPlots.make<TH1D>("stop1_deltaR" , "#Delta R (st1_{1},st1_{2})" , 50 , 0., 5. );
	h_stop1_deltaPhi = genTruthPlots.make<TH1D>("stop1_deltaPhi" , "#Delta #phi (st1_{1}, st1_{2})" , 50 , 0., 5. );
	h_stop1_cosDeltaPhi = genTruthPlots.make<TH1D>("stop1_cosDeltaPhi" , "cos [#Delta #phi (st1_{1}, st1_{2})]" , 26 , -1.2, 1.2 );


	//// Stop2 histos
	h_stop21_mass = genTruthPlots.make<TH1D>("stop21_mass" , "1st Stop2 Mass" , 61.0 , 200.0, 800. );
	h_stop21_pt = genTruthPlots.make<TH1D>("stop21_pt" , "1st Stop2 p_{T}" , 98 , 0.0, 500. );
	h_stop21_eta = genTruthPlots.make<TH1D>("stop21_eta" , "1st Stop2 #eta" , 40 , -5.0, 5.0 );
	h_stop22_mass = genTruthPlots.make<TH1D>("stop22_mass" , "1st Stop2 Mass" , 61.0 , 200.0, 800. );
	h_stop22_pt = genTruthPlots.make<TH1D>("stop22_pt" , "1st Stop2 p_{T}" , 98 , 0.0, 500. );
	h_stop22_eta = genTruthPlots.make<TH1D>("stop22_eta" , "1st Stop2 #eta" , 40 , -5.0, 5.0 );


	///// Analysis histos
	// Gen Truth
	h_massGenTruthHiggs = analysisPlots.make<TH1D>("massGenTruthHiggs" , "Mass of GenTruthHiggs" , 98.0 , 0.0, 300.0 );
	h_scalarPtGenTruthHiggs = analysisPlots.make<TH2D>("scalarPtGenTruthHiggs" , "Mass of GenTruthHiggs vs scalar #sum p_{T}^{GenTruthHiggs}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_PtGenTruthHiggs = analysisPlots.make<TH2D>("PtGenTruthHiggs" , "Mass of GenTruthHiggs vs #sum p_{T}^{GenTruthHiggs}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );

	// Match bjets
	h_massbb = analysisPlots.make<TH1D>("massbb" , "Mass of bb" , 80.0 , 0.0, 800.0 );
	h_scalarPtbb = analysisPlots.make<TH2D>("scalarPtbb" , "Mass of bb vs scalar #sum p_{T}^{bb}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_Ptbb = analysisPlots.make<TH2D>("Ptbb" , "Mass of bb vs #sum p_{T}^{bb}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massbb_cut50 = analysisPlots.make<TH1D>("massbb_cut50" , "Mass of bb" , 80.0 , 0.0, 800.0 );
	h_scalarPtbb_cut50 = analysisPlots.make<TH2D>("scalarPtbb_cut50" , "Mass of bb vs scalar #sum p_{T}^{bb}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massbb_cut100 = analysisPlots.make<TH1D>("massbb_cut100" , "Mass of bb" , 80.0 , 0.0, 800.0 );
	h_scalarPtbb_cut100 = analysisPlots.make<TH2D>("scalarPtbb_cut100" , "Mass of bb vs scalar #sum p_{T}^{bb}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massbb_cut150 = analysisPlots.make<TH1D>("massbb_cut150" , "Mass of bb" , 80.0 , 0.0, 800.0 );
	h_scalarPtbb_cut150 = analysisPlots.make<TH2D>("scalarPtbb_cut150" , "Mass of bb vs scalar #sum p_{T}^{bb}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massbb_cut200 = analysisPlots.make<TH1D>("massbb_cut200" , "Mass of bb" , 80.0 , 0.0, 800.0 );
	h_scalarPtbb_cut200 = analysisPlots.make<TH2D>("scalarPtbb_cut200" , "Mass of bb vs scalar #sum p_{T}^{bb}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );

	// Match bjets from Higgs
	h_massbbHiggs = analysisPlots.make<TH1D>("massbbHiggs" , "Mass of bbHiggs" , 98.0 , 0.0, 300.0 );
	h_massbbHiggs_num = analysisPlots.make<TH1D>("massbbHiggs_num" , "Num of bbHiggs" , 11 , -0.5, 10.5 ); 
	h_scalarPtbbHiggs = analysisPlots.make<TH2D>("scalarPtbbHiggs" , "Mass of bbHiggs vs scalar #sum p_{T}^{bbHiggs}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_PtbbHiggs = analysisPlots.make<TH2D>("PtbbHiggs" , "Mass of bbHiggs vs #sum p_{T}^{bbHiggs}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massbbHiggs_cut50 = analysisPlots.make<TH1D>("massbbHiggs_cut50" , "Mass of bbHiggs" , 98.0 , 0.0, 300.0 );
	h_scalarPtbbHiggs_cut50 = analysisPlots.make<TH2D>("scalarPtbbHiggs_cut50" , "Mass of bbHiggs vs scalar #sum p_{T}^{bbHiggs}", 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massbbHiggs_cut100 = analysisPlots.make<TH1D>("massbbHiggs_cut100" , "Mass of bbHiggs" , 98.0 , 0.0, 300.0 );
	h_scalarPtbbHiggs_cut100 = analysisPlots.make<TH2D>("scalarPtbbHiggs_cut100" , "Mass of bbHiggs vs scalar #sum p_{T}^{bbHiggs}", 80.0, 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massbbHiggs_cut150 = analysisPlots.make<TH1D>("massbbHiggs_cut150" , "Mass of bbHiggs" , 98.0 , 0.0, 300.0 );
	h_scalarPtbbHiggs_cut150 = analysisPlots.make<TH2D>("scalarPtbbHiggs_cut150" , "Mass of bbHiggs vs scalar #sum p_{T}^{bbHiggs}", 80.0, 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massbbHiggs_cut200 = analysisPlots.make<TH1D>("massbbHiggs_cut200" , "Mass of bbHiggs" , 98.0 , 0.0, 300.0 );
	h_scalarPtbbHiggs_cut200 = analysisPlots.make<TH2D>("scalarPtbbHiggs_cut200" , "Mass of bbHiggs vs scalar #sum p_{T}^{bbHiggs}", 80.0, 0.0, 800.0, 80 , 0.0, 800.0 );

	// RECO jets
	h_massRecobb = analysisPlots.make<TH1D>("massRecobb" , "Mass of Recobb" , 80.0 , 0.0, 800.0 );
	h_massRecobb_num = analysisPlots.make<TH1D>("massRecobb_num" , "Number of Recobb" , 11 , -0.5, 10.5 );
	h_scalarPtRecobb = analysisPlots.make<TH2D>("scalarPtRecobb" , "Mass of Recobb vs scalar #sum p_{T}^{Recobb}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_PtRecobb = analysisPlots.make<TH2D>("PtRecobb" , "Mass of Recobb vs #sum p_{T}^{Recobb}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massRecobb_cut50 = analysisPlots.make<TH1D>("massRecobb_cut50" , "Mass of Recobb" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecobb_cut50 = analysisPlots.make<TH2D>("scalarPtRecobb_cut50" , "Mass of Recobb vs scalar #sum p_{T}^{Recobb}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massRecobb_cut100 = analysisPlots.make<TH1D>("massRecobb_cut100" , "Mass of Recobb" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecobb_cut100 = analysisPlots.make<TH2D>("scalarPtRecobb_cut100" , "Mass of Recobb vs scalar #sum p_{T}^{Recobb}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massRecobb_cut150 = analysisPlots.make<TH1D>("massRecobb_cut150" , "Mass of Recobb" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecobb_cut150 = analysisPlots.make<TH2D>("scalarPtRecobb_cut150" , "Mass of Recobb vs scalar #sum p_{T}^{Recobb}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massRecobb_cut200 = analysisPlots.make<TH1D>("massRecobb_cut200" , "Mass of Recobb" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecobb_cut200 = analysisPlots.make<TH2D>("scalarPtRecobb_cut200" , "Mass of Recobb vs scalar #sum p_{T}^{Recobb}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );

	// RECO Bjets with CSVM
	h_massRecoBtagJets_CSVM = analysisPlots.make<TH1D>("massRecoBtagJets_CSVM" , "Mass of RecoBtagJets_CSVM" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecoBtagJets_CSVM = analysisPlots.make<TH2D>("scalarPtRecoBtagJets_CSVM" , "Mass of RecoBtagJets_CSVM vs scalar #sum p_{T}^{RecoBtagJets_CSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_PtRecoBtagJets_CSVM = analysisPlots.make<TH2D>("PtRecoBtagJets_CSVM" , "Mass of RecoBtagJets_CSVM vs #sum p_{T}^{RecoBtagJets_CSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massRecoBtagJets_CSVM_cut50 = analysisPlots.make<TH1D>("massRecoBtagJets_CSVM_cut50" , "Mass of RecoBtagJets_CSVM" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecoBtagJets_CSVM_cut50 = analysisPlots.make<TH2D>("scalarPtRecoBtagJets_CSVM_cut50" , "Mass of RecoBtagJets_CSVM vs scalar #sum p_{T}^{RecoBtagJets_CSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massRecoBtagJets_CSVM_cut100 = analysisPlots.make<TH1D>("massRecoBtagJets_CSVM_cut100" , "Mass of RecoBtagJets_CSVM" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecoBtagJets_CSVM_cut100 = analysisPlots.make<TH2D>("scalarPtRecoBtagJets_CSVM_cut100" , "Mass of RecoBtagJets_CSVM vs scalar #sum p_{T}^{RecoBtagJets_CSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massRecoBtagJets_CSVM_cut150 = analysisPlots.make<TH1D>("massRecoBtagJets_CSVM_cut150" , "Mass of RecoBtagJets_CSVM" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecoBtagJets_CSVM_cut150 = analysisPlots.make<TH2D>("scalarPtRecoBtagJets_CSVM_cut150" , "Mass of RecoBtagJets_CSVM vs scalar #sum p_{T}^{RecoBtagJets_CSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massRecoBtagJets_CSVM_cut200 = analysisPlots.make<TH1D>("massRecoBtagJets_CSVM_cut200" , "Mass of RecoBtagJets_CSVM" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecoBtagJets_CSVM_cut200 = analysisPlots.make<TH2D>("scalarPtRecoBtagJets_CSVM_cut200" , "Mass of RecoBtagJets_CSVM vs scalar #sum p_{T}^{RecoBtagJets_CSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	//
	h_massRecoPartonFlavorBtagJets_CSVM = analysisPlots.make<TH1D>("massRecoPartonFlavorBtagJets_CSVM" , "Mass of RecoPartonFlavorBtagJets_CSVM" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecoPartonFlavorBtagJets_CSVM = analysisPlots.make<TH2D>("scalarPtRecoPartonFlavorBtagJets_CSVM" , "Mass of RecoPartonFlavorBtagJets_CSVM vs scalar #sum p_{T}^{RecoPartonFlavorBtagJets_CSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_PtRecoPartonFlavorBtagJets_CSVM = analysisPlots.make<TH2D>("PtRecoPartonFlavorBtagJets_CSVM" , "Mass of RecoPartonFlavorBtagJets_CSVM vs #sum p_{T}^{RecoPartonFlavorBtagJets_CSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massRecoPartonFlavorBtagJets_CSVM_cut50 = analysisPlots.make<TH1D>("massRecoPartonFlavorBtagJets_CSVM_cut50" , "Mass of RecoPartonFlavorBtagJets_CSVM" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut50 = analysisPlots.make<TH2D>("scalarPtRecoPartonFlavorBtagJets_CSVM_cut50" , "Mass of RecoPartonFlavorBtagJets_CSVM vs scalar #sum p_{T}^{RecoPartonFlavorBtagJets_CSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massRecoPartonFlavorBtagJets_CSVM_cut100 = analysisPlots.make<TH1D>("massRecoPartonFlavorBtagJets_CSVM_cut100" , "Mass of RecoPartonFlavorBtagJets_CSVM" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut100 = analysisPlots.make<TH2D>("scalarPtRecoPartonFlavorBtagJets_CSVM_cut100" , "Mass of RecoPartonFlavorBtagJets_CSVM vs scalar #sum p_{T}^{RecoPartonFlavorBtagJets_CSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massRecoPartonFlavorBtagJets_CSVM_cut150 = analysisPlots.make<TH1D>("massRecoPartonFlavorBtagJets_CSVM_cut150" , "Mass of RecoPartonFlavorBtagJets_CSVM" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut150 = analysisPlots.make<TH2D>("scalarPtRecoPartonFlavorBtagJets_CSVM_cut150" , "Mass of RecoPartonFlavorBtagJets_CSVM vs scalar #sum p_{T}^{RecoPartonFlavorBtagJets_CSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massRecoPartonFlavorBtagJets_CSVM_cut200 = analysisPlots.make<TH1D>("massRecoPartonFlavorBtagJets_CSVM_cut200" , "Mass of RecoPartonFlavorBtagJets_CSVM" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut200 = analysisPlots.make<TH2D>("scalarPtRecoPartonFlavorBtagJets_CSVM_cut200" , "Mass of RecoPartonFlavorBtagJets_CSVM vs scalar #sum p_{T}^{RecoPartonFlavorBtagJets_CSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );


	h_matchjj_masspt = analysisPlots.make<TH2D>("matchjj_masspt" , "Mass of jj vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchjjWObb_masspt = analysisPlots.make<TH2D>("matchjjWObb_masspt" , "Mass of Match jj w/o bb vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massmatchjjWObb = analysisPlots.make<TH1D>("massmatchjjWObb" , "Mass of jj w/o bb" , 80.0 , 0.0, 800.0 );
	h_matchjjWObb_masspt_cut50 = analysisPlots.make<TH2D>("matchjjWObb_masspt_cut50" , "Mass of Match jj w/o bb vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchjjWObb_masspt_cut100 = analysisPlots.make<TH2D>("matchjjWObb_masspt_cut100" , "Mass of Match jj w/o bb vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchjjWObb_masspt_cut150 = analysisPlots.make<TH2D>("matchjjWObb_masspt_cut150" , "Mass of Match jj w/o bb vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchjjWObb_masspt_cut200 = analysisPlots.make<TH2D>("matchjjWObb_masspt_cut200" , "Mass of Match jj w/o bb vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massmatchjjWObb_cut50 = analysisPlots.make<TH1D>("massmatchjjWObb_cut50" , "Mass of jj w/o bb" , 80.0 , 0.0, 800.0 );
	h_massmatchjjWObb_cut100 = analysisPlots.make<TH1D>("massmatchjjWObb_cut100" , "Mass of jj w/o bb" , 80.0 , 0.0, 800.0 );
	h_massmatchjjWObb_cut150 = analysisPlots.make<TH1D>("massmatchjjWObb_cut150" , "Mass of jj w/o bb" , 80.0 , 0.0, 800.0 );
	h_massmatchjjWObb_cut200 = analysisPlots.make<TH1D>("massmatchjjWObb_cut200" , "Mass of jj w/o bb" , 80.0 , 0.0, 800.0 );
	//
	h_matchjjHiggs_masspt = analysisPlots.make<TH2D>("matchjjHiggs_masspt" , "Mass of jj vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchjjWObbHiggs_masspt = analysisPlots.make<TH2D>("matchjjWObbHiggs_masspt" , "Mass of Match jj w/o bbHiggs vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massmatchjjWObbHiggs = analysisPlots.make<TH1D>("massmatchjjWObbHiggs" , "Mass of jj w/o bbHiggs" , 80.0 , 0.0, 800.0 );
	h_matchjjWObbHiggs_masspt_cut50 = analysisPlots.make<TH2D>("matchjjWObbHiggs_masspt_cut50" , "Mass of Match jj w/o bbHiggs vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchjjWObbHiggs_masspt_cut100 = analysisPlots.make<TH2D>("matchjjWObbHiggs_masspt_cut100" , "Mass of Match jj w/o bbHiggs vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchjjWObbHiggs_masspt_cut150 = analysisPlots.make<TH2D>("matchjjWObbHiggs_masspt_cut150" , "Mass of Match jj w/o bbHiggs vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchjjWObbHiggs_masspt_cut200 = analysisPlots.make<TH2D>("matchjjWObbHiggs_masspt_cut200" , "Mass of Match jj w/o bbHiggs vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massmatchjjWObbHiggs_cut50 = analysisPlots.make<TH1D>("massmatchjjWObbHiggs_cut50" , "Mass of jj w/o bbHiggs" , 80.0 , 0.0, 800.0 );
	h_massmatchjjWObbHiggs_cut100 = analysisPlots.make<TH1D>("massmatchjjWObbHiggs_cut100" , "Mass of jj w/o bbHiggs" , 80.0 , 0.0, 800.0 );
	h_massmatchjjWObbHiggs_cut150 = analysisPlots.make<TH1D>("massmatchjjWObbHiggs_cut150" , "Mass of jj w/o bbHiggs" , 80.0 , 0.0, 800.0 );
	h_massmatchjjWObbHiggs_cut200 = analysisPlots.make<TH1D>("massmatchjjWObbHiggs_cut200" , "Mass of jj w/o bbHiggs" , 80.0 , 0.0, 800.0 );
	//
	h_Recojj_masspt = analysisPlots.make<TH2D>("Recojj_masspt" , "Mass of jj vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_jjWORecobb_masspt = analysisPlots.make<TH2D>("jjWORecobb_masspt" , "Mass of Match jj w/o Recobb vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massjjWORecobb = analysisPlots.make<TH1D>("massjjWORecobb" , "Mass of jj w/o Recobb" , 80.0 , 0.0, 800.0 );
	h_jjWORecobb_masspt_cut50 = analysisPlots.make<TH2D>("jjWORecobb_masspt_cut50" , "Mass of Match jj w/o Recobb vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_jjWORecobb_masspt_cut100 = analysisPlots.make<TH2D>("jjWORecobb_masspt_cut100" , "Mass of Match jj w/o Recobb vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_jjWORecobb_masspt_cut150 = analysisPlots.make<TH2D>("jjWORecobb_masspt_cut150" , "Mass of Match jj w/o Recobb vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_jjWORecobb_masspt_cut200 = analysisPlots.make<TH2D>("jjWORecobb_masspt_cut200" , "Mass of Match jj w/o Recobb vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massjjWORecobb_cut50 = analysisPlots.make<TH1D>("massjjWORecobb_cut50" , "Mass of jj w/o Recobb" , 80.0 , 0.0, 800.0 );
	h_massjjWORecobb_cut100 = analysisPlots.make<TH1D>("massjjWORecobb_cut100" , "Mass of jj w/o Recobb" , 80.0 , 0.0, 800.0 );
	h_massjjWORecobb_cut150 = analysisPlots.make<TH1D>("massjjWORecobb_cut150" , "Mass of jj w/o Recobb" , 80.0 , 0.0, 800.0 );
	h_massjjWORecobb_cut200 = analysisPlots.make<TH1D>("massjjWORecobb_cut200" , "Mass of jj w/o Recobb" , 80.0 , 0.0, 800.0 );
	//
	h_RecoBtagJetsjj_masspt = analysisPlots.make<TH2D>("RecoBtagJetsjj_masspt" , "Mass of jj vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_jjWORecoBtagJets_CSVM_masspt = analysisPlots.make<TH2D>("jjWORecoBtagJets_CSVM_masspt" , "Mass of Match jj w/o RecoBtagJets vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_jjWORecoBtagJets_CSVM_masspt_cut50 = analysisPlots.make<TH2D>("jjWORecoBtagJets_CSVM_masspt_cut50" , "Mass of Match jj w/o RecoBtagJets vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_jjWORecoBtagJets_CSVM_masspt_cut100 = analysisPlots.make<TH2D>("jjWORecoBtagJets_CSVM_masspt_cut100" , "Mass of Match jj w/o RecoBtagJets vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_jjWORecoBtagJets_CSVM_masspt_cut150 = analysisPlots.make<TH2D>("jjWORecoBtagJets_CSVM_masspt_cut150" , "Mass of Match jj w/o RecoBtagJets vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_jjWORecoBtagJets_CSVM_masspt_cut200 = analysisPlots.make<TH2D>("jjWORecoBtagJets_CSVM_masspt_cut200" , "Mass of Match jj w/o RecoBtagJets vs scalar #sum p_{T}^{jj}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massjjWORecoBtagJets_CSVM = analysisPlots.make<TH1D>("massjjWORecoBtagJets_CSVM" , "Mass of jj w/o RecoBtagJets" , 80.0 , 0.0, 800.0 );
	h_massjjWORecoBtagJets_CSVM_cut50 = analysisPlots.make<TH1D>("massjjWORecoBtagJets_CSVM_cut50" , "Mass of jj w/o RecoBtagJets" , 80.0 , 0.0, 800.0 );
	h_massjjWORecoBtagJets_CSVM_cut100 = analysisPlots.make<TH1D>("massjjWORecoBtagJets_CSVM_cut100" , "Mass of jj w/o RecoBtagJets" , 80.0 , 0.0, 800.0 );
	h_massjjWORecoBtagJets_CSVM_cut150 = analysisPlots.make<TH1D>("massjjWORecoBtagJets_CSVM_cut150" , "Mass of jj w/o RecoBtagJets" , 80.0 , 0.0, 800.0 );
	h_massjjWORecoBtagJets_CSVM_cut200 = analysisPlots.make<TH1D>("massjjWORecoBtagJets_CSVM_cut200" , "Mass of jj w/o RecoBtagJets" , 80.0 , 0.0, 800.0 );


	h_recobbjj_masspt = analysisPlots.make<TH2D>("recobbjj_masspt" , "Mass of bbjj vs scalar #sum p_{T}^{bbjj}" , 120.0 , 0.0, 1200.0, 120 , 0.0, 1200.0 );

	h_bbjj_masspt = analysisPlots.make<TH2D>("bbjj_masspt" , "Mass of bbjj vs scalar #sum p_{T}^{bbjj}" , 120.0 , 0.0, 1200.0, 120 , 0.0, 1200.0 );
	h_massrecobbjj = analysisPlots.make<TH1D>("massrecobbjj" , "Mass of Reco bbjj" , 80.0 , 0.0, 800.0 );
	h_massbbjj = analysisPlots.make<TH1D>("massbbjj" , "Mass of bbjj" , 80.0 , 0.0, 800.0 );

}


MyAnalyzer::~MyAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

