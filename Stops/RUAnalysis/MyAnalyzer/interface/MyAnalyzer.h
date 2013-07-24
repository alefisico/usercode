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

	void BasicPlots( vector< TLorentzVector > p4GenTruthAlljets, vector< TLorentzVector > p4GenTruthJets, vector< TLorentzVector > p4GenTruthBjets, vector< TLorentzVector > p4GenTruthBjetsHiggs, vector< TLorentzVector > p4GenTruthB1Higgs, vector< TLorentzVector > p4GenTruthB2Higgs, vector< TLorentzVector > p4GenTruthJetsStop1, vector< TLorentzVector > p4GenTruthBjetsStop1, vector< TLorentzVector > p4MatchTruthJets,  vector< TLorentzVector > p4MatchTruthBjets, vector< TLorentzVector > p4MatchB1Higgs, vector< TLorentzVector > p4MatchB2Higgs, vector< TLorentzVector > p4RecoJets, vector< TLorentzVector > p4RecoBjetsCSVM );
	void Analysis(  vector< TLorentzVector > p4GenTruthB1Higgs, vector< TLorentzVector > p4GenTruthB2Higgs, vector< TLorentzVector > p4RecoJets, vector< TLorentzVector > p4RecoBjetsCSVM, vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM, vector< TLorentzVector > p4MatchTruthBjets,  vector< TLorentzVector > p4MatchB1Higgs, vector< TLorentzVector > p4MatchB2Higgs, vector< TLorentzVector > p4MatchTruthAlljets);

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
	TH1D * h_genTruthBjets_num;
	TH1D * h_genTruthBjets_pt;
	TH1D * h_genTruthBjets_sumpt;
	TH1D * h_genTruthBjets_eta;
	TH1D * h_genTruthBjets_phi;
	TH1D * h_matchBjets_num;
	TH1D * h_matchBjets_pt;
	TH1D * h_matchBjets_sumpt;
	TH1D * h_matchBjets_eta;
	TH1D * h_matchBjets_phi;
	TH1D * h_matchBjetsHiggs_num;
	TH1D * h_matchBjetsHiggs1_num;
	TH1D * h_matchBjetsHiggs2_num;
	TH1D * h_matchBjetsHiggs_mass;
	TH1D * h_matchBjetsHiggs_pt;
	TH1D * h_matchBjetsHiggs_sumpt;
	TH1D * h_matchBjetsHiggs_eta;
	TH1D * h_matchBjetsHiggs_phi;

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
	TH1D * h_recoBjets_num;
	TH1D * h_recoBjets_pt;
	TH1D * h_recoBjets_sumpt;
	TH1D * h_recoBjets_eta;
	TH1D * h_recoBjets_phi;
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
	TH1D * h_genTruthBjetsHiggs_mass;
	TH1D * h_genTruthBjetsHiggs_num;
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
	TH1D * h_massRecoBjetsCSVM;
	TH1D * h_massRecoBjetsCSVM_1cut50;
	TH1D * h_massRecoBjetsCSVM_1cut100;
	TH1D * h_massRecoBjetsCSVM_1cut150;
	TH1D * h_massRecoBjetsCSVM_1cut200;
	TH1D * h_avgMassRecoBjetsCSVM;
	TH1D * h_avgMassRecoBjetsCSVM_1cut50;
	TH1D * h_avgMassRecoBjetsCSVM_1cut100;
	TH1D * h_avgMassRecoBjetsCSVM_1cut150;
	TH1D * h_avgMassRecoBjetsCSVM_1cut200;
	TH2D * h_avgPtRecoBjetsCSVM;
	TH2D * h_avgPtRecoBjetsCSVM_1cut50;
	TH2D * h_avgPtRecoBjetsCSVM_1cut100;
	TH2D * h_avgPtRecoBjetsCSVM_1cut150;
	TH2D * h_avgPtRecoBjetsCSVM_1cut200;
	TH2D * h_scalarPtRecoBjetsCSVM;
	TH2D * h_scalarPtRecoBjetsCSVM_1cut50;
	TH2D * h_scalarPtRecoBjetsCSVM_1cut100;
	TH2D * h_scalarPtRecoBjetsCSVM_1cut150;
	TH2D * h_scalarPtRecoBjetsCSVM_1cut200;
	TH2D * h_PtRecoBjetsCSVM;
	//
	TH1D * h_massRecoPartonFlavorBjetsCSVM;
	TH1D * h_massRecoPartonFlavorBjetsCSVM_1cut50;
	TH1D * h_massRecoPartonFlavorBjetsCSVM_1cut100;
	TH1D * h_massRecoPartonFlavorBjetsCSVM_1cut150;
	TH1D * h_massRecoPartonFlavorBjetsCSVM_1cut200;
	TH1D * h_avgMassRecoPartonFlavorBjetsCSVM;
	TH1D * h_avgMassRecoPartonFlavorBjetsCSVM_1cut50;
	TH1D * h_avgMassRecoPartonFlavorBjetsCSVM_1cut100;
	TH1D * h_avgMassRecoPartonFlavorBjetsCSVM_1cut150;
	TH1D * h_avgMassRecoPartonFlavorBjetsCSVM_1cut200;
	TH2D * h_avgPtRecoPartonFlavorBjetsCSVM;
	TH2D * h_avgPtRecoPartonFlavorBjetsCSVM_1cut50;
	TH2D * h_avgPtRecoPartonFlavorBjetsCSVM_1cut100;
	TH2D * h_avgPtRecoPartonFlavorBjetsCSVM_1cut150;
	TH2D * h_avgPtRecoPartonFlavorBjetsCSVM_1cut200;
	TH2D * h_scalarPtRecoPartonFlavorBjetsCSVM;
	TH2D * h_scalarPtRecoPartonFlavorBjetsCSVM_1cut50;
	TH2D * h_scalarPtRecoPartonFlavorBjetsCSVM_1cut100;
	TH2D * h_scalarPtRecoPartonFlavorBjetsCSVM_1cut150;
	TH2D * h_scalarPtRecoPartonFlavorBjetsCSVM_1cut200;
	TH2D * h_PtRecoPartonFlavorBjetsCSVM;
	//
	TH1D * h_massMatchBjets;
	TH1D * h_massMatchBjets_1cut50;
	TH1D * h_massMatchBjets_1cut100;
	TH1D * h_massMatchBjets_1cut150;
	TH1D * h_massMatchBjets_1cut200;
	TH1D * h_avgMassMatchBjets;
	TH1D * h_avgMassMatchBjets_1cut50;
	TH1D * h_avgMassMatchBjets_1cut100;
	TH1D * h_avgMassMatchBjets_1cut150;
	TH1D * h_avgMassMatchBjets_1cut200;
	TH2D * h_avgPtMatchBjets;
	TH2D * h_avgPtMatchBjets_1cut50;
	TH2D * h_avgPtMatchBjets_1cut100;
	TH2D * h_avgPtMatchBjets_1cut150;
	TH2D * h_avgPtMatchBjets_1cut200;
	TH2D * h_scalarPtMatchBjets;
	TH2D * h_scalarPtMatchBjets_1cut50;
	TH2D * h_scalarPtMatchBjets_1cut100;
	TH2D * h_scalarPtMatchBjets_1cut150;
	TH2D * h_scalarPtMatchBjets_1cut200;
	TH2D * h_PtMatchBjets;
	//
	TH1D * h_massMatchBjetsHiggs;
	TH1D * h_massMatchBjetsHiggs_num;
	TH1D * h_massMatchBjetsHiggs_1cut50;
	TH1D * h_massMatchBjetsHiggs_1cut100;
	TH1D * h_massMatchBjetsHiggs_1cut150;
	TH1D * h_massMatchBjetsHiggs_1cut200;
	TH1D * h_avgMassMatchBjetsHiggs;
	TH1D * h_avgMassMatchBjetsHiggs_1cut50;
	TH1D * h_avgMassMatchBjetsHiggs_1cut100;
	TH1D * h_avgMassMatchBjetsHiggs_1cut150;
	TH1D * h_avgMassMatchBjetsHiggs_1cut200;
	TH2D * h_avgPtMatchBjetsHiggs;
	TH2D * h_avgPtMatchBjetsHiggs_1cut50;
	TH2D * h_avgPtMatchBjetsHiggs_1cut100;
	TH2D * h_avgPtMatchBjetsHiggs_1cut150;
	TH2D * h_avgPtMatchBjetsHiggs_1cut200;
	TH2D * h_scalarPtMatchBjetsHiggs;
	TH2D * h_scalarPtMatchBjetsHiggs_1cut50;
	TH2D * h_scalarPtMatchBjetsHiggs_1cut100;
	TH2D * h_scalarPtMatchBjetsHiggs_1cut150;
	TH2D * h_scalarPtMatchBjetsHiggs_1cut200;
	TH2D * h_PtMatchBjetsHiggs;
	TH1D * h_massGenTruthHiggs;
	TH2D * h_PtGenTruthHiggs;
	TH2D * h_scalarPtGenTruthHiggs;
	TH2D * h_matchdijet_masspt;
	TH2D * h_matchdijetWOMatchBjets_masspt;
	TH2D * h_matchdijetWOMatchBjets_masspt_1cut50;
	TH2D * h_matchdijetWOMatchBjets_masspt_1cut100;
	TH2D * h_matchdijetWOMatchBjets_masspt_1cut150;
	TH2D * h_matchdijetWOMatchBjets_masspt_1cut200;
	TH1D * h_massmatchdijetWOMatchBjets;
	TH1D * h_massmatchdijetWOMatchBjets_1cut50;
	TH1D * h_massmatchdijetWOMatchBjets_1cut100;
	TH1D * h_massmatchdijetWOMatchBjets_1cut150;
	TH1D * h_massmatchdijetWOMatchBjets_1cut200;
	TH2D * h_matchdijetHiggs_masspt;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_1cut50;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_1cut100;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_1cut150;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_1cut200;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_1cut50;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_1cut100;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_1cut150;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_1cut200;
	TH2D * h_RecoBtagJetsdijet_masspt;
	TH2D * h_dijetWORecoBjetsCSVM_masspt;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_1cut50;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_1cut100;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_1cut150;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_1cut200;
	TH1D * h_massdijetWORecoBjetsCSVM;
	TH1D * h_massdijetWORecoBjetsCSVM_1cut50;
	TH1D * h_massdijetWORecoBjetsCSVM_1cut100;
	TH1D * h_massdijetWORecoBjetsCSVM_1cut150;
	TH1D * h_massdijetWORecoBjetsCSVM_1cut200;
	TH2D * h_RecoPartonFlavorBtagJetsdijet_masspt;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut50;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut100;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut150;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut200;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_1cut50;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_1cut100;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_1cut150;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_1cut200;

	TH2D * h_recodijet_masspt;
	TH2D * h_recodijetWOMatchBjets_masspt;
	TH2D * h_recoMatchBjetsdijet_masspt;
	TH2D * h_MatchBjetsdijet_masspt;
	TH1D * h_massrecoMatchBjetsdijet;
	TH1D * h_massMatchBjetsdijet;


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
	double avgMass;
	double avgPt;
} smallMassDiffVectors;

typedef struct {
	vector< TLorentzVector > s3VectorTL;
	vector< double > s3Mass;
	vector< double > s3ScalarPt;
	//double avgMass;
	//double avgPt;
} s3VecMassSumPt;


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

	//// Bjets histos
	h_genTruthBjets_num = genTruthPlots.make<TH1D>("genTruthBjets_num" , "Number of GenTruth bJets" , 11 , -0.5, 10.5 );
	h_genTruthBjets_pt = genTruthPlots.make<TH1D>("genTruthBjets_pt" , "GenTruth bJets p_{T}" , 98 , 0., 500. );
	h_genTruthBjets_sumpt = genTruthPlots.make<TH1D>("genTruthBjets_sumpt" , "#sum GenTruth bJets p_{T}" , 200 , 0., 1000. );
	h_genTruthBjets_eta = genTruthPlots.make<TH1D>("genTruthBjets_eta" , "GenTruth bJets #eta" , 40 , -5.0, 5.0 );
	h_genTruthBjets_phi = genTruthPlots.make<TH1D>("genTruthBjets_phi" , "GenTruth bJets #phi" , 40 , -5.0, 5.0 );
	h_matchBjets_num = basicPlots.make<TH1D>("matchBjets_num" , "Number of Match bJets" , 11 , -0.5, 10.5 );
	h_matchBjets_pt = basicPlots.make<TH1D>("matchBjets_pt" , "Match bJets p_{T}" , 98 , 0., 500. );
	h_matchBjets_sumpt = basicPlots.make<TH1D>("matchBjets_sumpt" , "#sum Match bJets p_{T}" , 200 , 0., 1000. );
	h_matchBjets_eta = basicPlots.make<TH1D>("matchBjets_eta" , "Match bJets #eta" , 40 , -5.0, 5.0 );
	h_matchBjets_phi = basicPlots.make<TH1D>("matchBjets_phi" , "Match bJets #phi" , 40 , -5.0, 5.0 );
	h_matchBjetsHiggs_num = basicPlots.make<TH1D>("matchBjetsHiggs_num" , "Number of Match Higgs bJets" , 11 , -0.5, 10.5 );
	h_matchBjetsHiggs1_num = basicPlots.make<TH1D>("matchBjetsHiggs1_num" , "Number of Match Higgs 1 bJets" , 11 , -0.5, 10.5 );
	h_matchBjetsHiggs2_num = basicPlots.make<TH1D>("matchBjetsHiggs2_num" , "Number of Match Higgs 2 bJets" , 11 , -0.5, 10.5 );
	h_matchBjetsHiggs_pt = basicPlots.make<TH1D>("matchBjetsHiggs_pt" , "Match Higgs bJets p_{T}" , 98 , 0., 500. );
	h_matchBjetsHiggs_mass = basicPlots.make<TH1D>("matchBjetsHiggs_mass" , "Match Higgs Mass" , 98 , 0., 300. );
	h_matchBjetsHiggs_sumpt = basicPlots.make<TH1D>("matchBjetsHiggs_sumpt" , "#sum Match bJets p_{T}" , 200 , 0., 1000. );
	h_matchBjetsHiggs_eta = basicPlots.make<TH1D>("matchBjetsHiggs_eta" , "Match Higgs bJets #eta" , 40 , -5.0, 5.0 );
	h_matchBjetsHiggs_phi = basicPlots.make<TH1D>("matchBjetsHiggs_phi" , "Match Higgs bJets #phi" , 40 , -5.0, 5.0 );

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
	h_recoBjets_num = basicPlots.make<TH1D>("recoBjets_num" , "Number of Reco Bjets" , 21 , -0.5, 20.5 );
	h_recoBjets_pt = basicPlots.make<TH1D>("recoBjets_pt" , "Reco Bjets p_{T}" , 98 , 0., 500. );
	h_recoBjets_sumpt = basicPlots.make<TH1D>("recoBjets_sumpt" , "#sum Reco Bjets p_{T}" , 200 , 0., 1000. );
	h_recoBjets_eta = basicPlots.make<TH1D>("recoBjets_eta" , "Reco Bjets #eta" , 40 , -5.0, 5.0 );
	h_recoBjets_phi = basicPlots.make<TH1D>("recoBjets_phi" , "Reco Bjets #phi" , 40 , -5.0, 5.0 );
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
	h_genTruthBjetsHiggs_mass = genTruthPlots.make<TH1D>("genTruthBjetsHiggs_mass" , "GenTruth Higgs Mass" , 21.0 , 100.0, 200. );
	h_genTruthBjetsHiggs_num = genTruthPlots.make<TH1D>("genTruthBjetsHiggs_num" , "Number GenTruth Higgs" , 11 , -0.5, 10.5 );
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
	h_massMatchBjets = analysisPlots.make<TH1D>("massMatchBjets" , "Mass of MatchBjets" , 80.0 , 0.0, 800.0 );
	h_massMatchBjets_1cut50 = analysisPlots.make<TH1D>("massMatchBjets_1cut50" , "Mass of MatchBjets" , 80.0 , 0.0, 800.0 );
	h_massMatchBjets_1cut100 = analysisPlots.make<TH1D>("massMatchBjets_1cut100" , "Mass of MatchBjets" , 80.0 , 0.0, 800.0 );
	h_massMatchBjets_1cut150 = analysisPlots.make<TH1D>("massMatchBjets_1cut150" , "Mass of MatchBjets" , 80.0 , 0.0, 800.0 );
	h_massMatchBjets_1cut200 = analysisPlots.make<TH1D>("massMatchBjets_1cut200" , "Mass of MatchBjets" , 80.0 , 0.0, 800.0 );
	h_avgMassMatchBjets = analysisPlots.make<TH1D>("avgMassMatchBjets" , "Average Mass of MatchBjets" , 80.0 , 0.0, 800.0 );
	h_avgMassMatchBjets_1cut50 = analysisPlots.make<TH1D>("avgMassMatchBjets_1cut50" , "Average Mass of MatchBjets" , 80.0 , 0.0, 800.0 );
	h_avgMassMatchBjets_1cut100 = analysisPlots.make<TH1D>("avgMassMatchBjets_1cut100" , "Average Mass of MatchBjets" , 80.0 , 0.0, 800.0 );
	h_avgMassMatchBjets_1cut150 = analysisPlots.make<TH1D>("avgMassMatchBjets_1cut150" , "Average Mass of MatchBjets" , 80.0 , 0.0, 800.0 );
	h_avgMassMatchBjets_1cut200 = analysisPlots.make<TH1D>("avgMassMatchBjets_1cut200" , "Average Mass of MatchBjets" , 80.0 , 0.0, 800.0 );
	h_scalarPtMatchBjets = analysisPlots.make<TH2D>("scalarPtMatchBjets" , "Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtMatchBjets_1cut50 = analysisPlots.make<TH2D>("scalarPtMatchBjets_1cut50" , "Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtMatchBjets_1cut100 = analysisPlots.make<TH2D>("scalarPtMatchBjets_1cut100" , "Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtMatchBjets_1cut150 = analysisPlots.make<TH2D>("scalarPtMatchBjets_1cut150" , "Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtMatchBjets_1cut200 = analysisPlots.make<TH2D>("scalarPtMatchBjets_1cut200" , "Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtMatchBjets = analysisPlots.make<TH2D>("avgPtMatchBjets" , "Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtMatchBjets_1cut50 = analysisPlots.make<TH2D>("avgPtMatchBjets_1cut50" , "Average Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtMatchBjets_1cut100 = analysisPlots.make<TH2D>("avgPtMatchBjets_1cut100" , "Average Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtMatchBjets_1cut150 = analysisPlots.make<TH2D>("avgPtMatchBjets_1cut150" , "Average Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtMatchBjets_1cut200 = analysisPlots.make<TH2D>("avgPtMatchBjets_1cut200" , "Average Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_PtMatchBjets = analysisPlots.make<TH2D>("PtMatchBjets" , "Mass of MatchBjets vs #sum p_{T}^{MatchBjets}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );

	// Match bjets from Higgs
	h_massMatchBjetsHiggs = analysisPlots.make<TH1D>("massMatchBjetsHiggs" , "Mass of MatchBjetsHiggs" , 98.0 , 0.0, 300.0 );
	h_massMatchBjetsHiggs_num = analysisPlots.make<TH1D>("massMatchBjetsHiggs_num" , "Num of MatchBjetsHiggs" , 11 , -0.5, 10.5 ); 
	h_massMatchBjetsHiggs_1cut50 = analysisPlots.make<TH1D>("massMatchBjetsHiggs_1cut50" , "Mass of MatchBjetsHiggs" , 98.0 , 0.0, 300.0 );
	h_massMatchBjetsHiggs_1cut100 = analysisPlots.make<TH1D>("massMatchBjetsHiggs_1cut100" , "Mass of MatchBjetsHiggs" , 98.0 , 0.0, 300.0 );
	h_massMatchBjetsHiggs_1cut150 = analysisPlots.make<TH1D>("massMatchBjetsHiggs_1cut150" , "Mass of MatchBjetsHiggs" , 98.0 , 0.0, 300.0 );
	h_massMatchBjetsHiggs_1cut200 = analysisPlots.make<TH1D>("massMatchBjetsHiggs_1cut200" , "Mass of MatchBjetsHiggs" , 98.0 , 0.0, 300.0 );
	h_avgMassMatchBjetsHiggs = analysisPlots.make<TH1D>("avgMassMatchBjetsHiggs" , "Average Mass of MatchBjetsHiggs" , 98.0 , 0.0, 300.0 );
	h_avgMassMatchBjetsHiggs_1cut50 = analysisPlots.make<TH1D>("avgMassMatchBjetsHiggs_1cut50" , "Average Mass of MatchBjetsHiggs" , 98.0 , 0.0, 300.0 );
	h_avgMassMatchBjetsHiggs_1cut100 = analysisPlots.make<TH1D>("avgMassMatchBjetsHiggs_1cut100" , "Average Mass of MatchBjetsHiggs" , 98.0 , 0.0, 300.0 );
	h_avgMassMatchBjetsHiggs_1cut150 = analysisPlots.make<TH1D>("avgMassMatchBjetsHiggs_1cut150" , "Average Mass of MatchBjetsHiggs" , 98.0 , 0.0, 300.0 );
	h_avgMassMatchBjetsHiggs_1cut200 = analysisPlots.make<TH1D>("avgMassMatchBjetsHiggs_1cut200" , "Average Mass of MatchBjetsHiggs" , 98.0 , 0.0, 300.0 );
	h_scalarPtMatchBjetsHiggs = analysisPlots.make<TH2D>("scalarPtMatchBjetsHiggs" , "Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtMatchBjetsHiggs_1cut50 = analysisPlots.make<TH2D>("scalarPtMatchBjetsHiggs_1cut50" , "Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtMatchBjetsHiggs_1cut100 = analysisPlots.make<TH2D>("scalarPtMatchBjetsHiggs_1cut100" , "Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", 80.0, 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtMatchBjetsHiggs_1cut150 = analysisPlots.make<TH2D>("scalarPtMatchBjetsHiggs_1cut150" , "Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", 80.0, 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtMatchBjetsHiggs_1cut200 = analysisPlots.make<TH2D>("scalarPtMatchBjetsHiggs_1cut200" , "Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", 80.0, 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtMatchBjetsHiggs = analysisPlots.make<TH2D>("avgPtMatchBjetsHiggs" , "Average Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtMatchBjetsHiggs_1cut50 = analysisPlots.make<TH2D>("avgPtMatchBjetsHiggs_1cut50" , "Average Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtMatchBjetsHiggs_1cut100 = analysisPlots.make<TH2D>("avgPtMatchBjetsHiggs_1cut100" , "Average Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", 80.0, 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtMatchBjetsHiggs_1cut150 = analysisPlots.make<TH2D>("avgPtMatchBjetsHiggs_1cut150" , "Average Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", 80.0, 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtMatchBjetsHiggs_1cut200 = analysisPlots.make<TH2D>("avgPtMatchBjetsHiggs_1cut200" , "Average Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", 80.0, 0.0, 800.0, 80 , 0.0, 800.0 );
	h_PtMatchBjetsHiggs = analysisPlots.make<TH2D>("PtMatchBjetsHiggs" , "Mass of MatchBjetsHiggs vs #sum p_{T}^{MatchBjetsHiggs}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );

	// RECO Bjets with CSVM
	h_massRecoBjetsCSVM = analysisPlots.make<TH1D>("massRecoBjetsCSVM" , "Mass of RecoBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_massRecoBjetsCSVM_1cut50 = analysisPlots.make<TH1D>("massRecoBjetsCSVM_1cut50" , "Mass of RecoBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_massRecoBjetsCSVM_1cut100 = analysisPlots.make<TH1D>("massRecoBjetsCSVM_1cut100" , "Mass of RecoBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_massRecoBjetsCSVM_1cut150 = analysisPlots.make<TH1D>("massRecoBjetsCSVM_1cut150" , "Mass of RecoBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_massRecoBjetsCSVM_1cut200 = analysisPlots.make<TH1D>("massRecoBjetsCSVM_1cut200" , "Mass of RecoBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_avgMassRecoBjetsCSVM = analysisPlots.make<TH1D>("avgMassRecoBjetsCSVM" , "Average Mass of RecoBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_avgMassRecoBjetsCSVM_1cut50 = analysisPlots.make<TH1D>("avgMassRecoBjetsCSVM_1cut50" , "Average Mass of RecoBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_avgMassRecoBjetsCSVM_1cut100 = analysisPlots.make<TH1D>("avgMassRecoBjetsCSVM_1cut100" , "Average Mass of RecoBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_avgMassRecoBjetsCSVM_1cut150 = analysisPlots.make<TH1D>("avgMassRecoBjetsCSVM_1cut150" , "Average Mass of RecoBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_avgMassRecoBjetsCSVM_1cut200 = analysisPlots.make<TH1D>("avgMassRecoBjetsCSVM_1cut200" , "Average Mass of RecoBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecoBjetsCSVM = analysisPlots.make<TH2D>("scalarPtRecoBjetsCSVM" , "Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtRecoBjetsCSVM_1cut50 = analysisPlots.make<TH2D>("scalarPtRecoBjetsCSVM_1cut50" , "Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtRecoBjetsCSVM_1cut100 = analysisPlots.make<TH2D>("scalarPtRecoBjetsCSVM_1cut100" , "Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtRecoBjetsCSVM_1cut150 = analysisPlots.make<TH2D>("scalarPtRecoBjetsCSVM_1cut150" , "Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtRecoBjetsCSVM_1cut200 = analysisPlots.make<TH2D>("scalarPtRecoBjetsCSVM_1cut200" , "Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtRecoBjetsCSVM = analysisPlots.make<TH2D>("avgPtRecoBjetsCSVM" , "Average Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtRecoBjetsCSVM_1cut50 = analysisPlots.make<TH2D>("avgPtRecoBjetsCSVM_1cut50" , "Average Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtRecoBjetsCSVM_1cut100 = analysisPlots.make<TH2D>("avgPtRecoBjetsCSVM_1cut100" , "Average Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtRecoBjetsCSVM_1cut150 = analysisPlots.make<TH2D>("avgPtRecoBjetsCSVM_1cut150" , "Average Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtRecoBjetsCSVM_1cut200 = analysisPlots.make<TH2D>("avgPtRecoBjetsCSVM_1cut200" , "Average Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_PtRecoBjetsCSVM = analysisPlots.make<TH2D>("PtRecoBjetsCSVM" , "Mass of RecoBjetsCSVM vs #sum p_{T}^{RecoBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	//
	h_massRecoPartonFlavorBjetsCSVM = analysisPlots.make<TH1D>("massRecoPartonFlavorBjetsCSVM" , "Mass of RecoPartonFlavorBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_massRecoPartonFlavorBjetsCSVM_1cut50 = analysisPlots.make<TH1D>("massRecoPartonFlavorBjetsCSVM_1cut50" , "Mass of RecoPartonFlavorBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_massRecoPartonFlavorBjetsCSVM_1cut100 = analysisPlots.make<TH1D>("massRecoPartonFlavorBjetsCSVM_1cut100" , "Mass of RecoPartonFlavorBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_massRecoPartonFlavorBjetsCSVM_1cut150 = analysisPlots.make<TH1D>("massRecoPartonFlavorBjetsCSVM_1cut150" , "Mass of RecoPartonFlavorBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_massRecoPartonFlavorBjetsCSVM_1cut200 = analysisPlots.make<TH1D>("massRecoPartonFlavorBjetsCSVM_1cut200" , "Mass of RecoPartonFlavorBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_avgMassRecoPartonFlavorBjetsCSVM = analysisPlots.make<TH1D>("avgMassRecoPartonFlavorBjetsCSVM" , "Average Mass of RecoPartonFlavorBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_avgMassRecoPartonFlavorBjetsCSVM_1cut50 = analysisPlots.make<TH1D>("avgMassRecoPartonFlavorBjetsCSVM_1cut50" , "Average Mass of RecoPartonFlavorBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_avgMassRecoPartonFlavorBjetsCSVM_1cut100 = analysisPlots.make<TH1D>("avgMassRecoPartonFlavorBjetsCSVM_1cut100" , "Average Mass of RecoPartonFlavorBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_avgMassRecoPartonFlavorBjetsCSVM_1cut150 = analysisPlots.make<TH1D>("avgMassRecoPartonFlavorBjetsCSVM_1cut150" , "Average Mass of RecoPartonFlavorBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_avgMassRecoPartonFlavorBjetsCSVM_1cut200 = analysisPlots.make<TH1D>("avgMassRecoPartonFlavorBjetsCSVM_1cut200" , "Average Mass of RecoPartonFlavorBjetsCSVM" , 80.0 , 0.0, 800.0 );
	h_scalarPtRecoPartonFlavorBjetsCSVM = analysisPlots.make<TH2D>("scalarPtRecoPartonFlavorBjetsCSVM" , "Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtRecoPartonFlavorBjetsCSVM_1cut50 = analysisPlots.make<TH2D>("scalarPtRecoPartonFlavorBjetsCSVM_1cut50" , "Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtRecoPartonFlavorBjetsCSVM_1cut100 = analysisPlots.make<TH2D>("scalarPtRecoPartonFlavorBjetsCSVM_1cut100" , "Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtRecoPartonFlavorBjetsCSVM_1cut150 = analysisPlots.make<TH2D>("scalarPtRecoPartonFlavorBjetsCSVM_1cut150" , "Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_scalarPtRecoPartonFlavorBjetsCSVM_1cut200 = analysisPlots.make<TH2D>("scalarPtRecoPartonFlavorBjetsCSVM_1cut200" , "Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtRecoPartonFlavorBjetsCSVM = analysisPlots.make<TH2D>("avgPtRecoPartonFlavorBjetsCSVM" , "Average Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtRecoPartonFlavorBjetsCSVM_1cut50 = analysisPlots.make<TH2D>("avgPtRecoPartonFlavorBjetsCSVM_1cut50" , "Average Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtRecoPartonFlavorBjetsCSVM_1cut100 = analysisPlots.make<TH2D>("avgPtRecoPartonFlavorBjetsCSVM_1cut100" , "Average Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtRecoPartonFlavorBjetsCSVM_1cut150 = analysisPlots.make<TH2D>("avgPtRecoPartonFlavorBjetsCSVM_1cut150" , "Average Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_avgPtRecoPartonFlavorBjetsCSVM_1cut200 = analysisPlots.make<TH2D>("avgPtRecoPartonFlavorBjetsCSVM_1cut200" , "Average Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_PtRecoPartonFlavorBjetsCSVM = analysisPlots.make<TH2D>("PtRecoPartonFlavorBjetsCSVM" , "Mass of RecoPartonFlavorBjetsCSVM vs #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );


	h_matchdijet_masspt = analysisPlots.make<TH2D>("matchdijet_masspt" , "Mass of dijet vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchdijetWOMatchBjets_masspt = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massmatchdijetWOMatchBjets = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets" , "Mass of dijet w/o MatchBjets" , 80.0 , 0.0, 800.0 );
	h_matchdijetWOMatchBjets_masspt_1cut50 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_1cut50" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchdijetWOMatchBjets_masspt_1cut100 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_1cut100" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchdijetWOMatchBjets_masspt_1cut150 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_1cut150" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchdijetWOMatchBjets_masspt_1cut200 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_1cut200" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massmatchdijetWOMatchBjets_1cut50 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_1cut50" , "Mass of dijet w/o MatchBjets" , 80.0 , 0.0, 800.0 );
	h_massmatchdijetWOMatchBjets_1cut100 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_1cut100" , "Mass of dijet w/o MatchBjets" , 80.0 , 0.0, 800.0 );
	h_massmatchdijetWOMatchBjets_1cut150 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_1cut150" , "Mass of dijet w/o MatchBjets" , 80.0 , 0.0, 800.0 );
	h_massmatchdijetWOMatchBjets_1cut200 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_1cut200" , "Mass of dijet w/o MatchBjets" , 80.0 , 0.0, 800.0 );
	//
	h_matchdijetHiggs_masspt = analysisPlots.make<TH2D>("matchdijetHiggs_masspt" , "Mass of dijet vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchdijetWOMatchBjetsHiggs_masspt = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massmatchdijetWOMatchBjetsHiggs = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs" , "Mass of dijet w/o MatchBjetsHiggs" , 80.0 , 0.0, 800.0 );
	h_matchdijetWOMatchBjetsHiggs_masspt_1cut50 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_1cut50" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchdijetWOMatchBjetsHiggs_masspt_1cut100 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_1cut100" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchdijetWOMatchBjetsHiggs_masspt_1cut150 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_1cut150" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_matchdijetWOMatchBjetsHiggs_masspt_1cut200 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_1cut200" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massmatchdijetWOMatchBjetsHiggs_1cut50 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_1cut50" , "Mass of dijet w/o MatchBjetsHiggs" , 80.0 , 0.0, 800.0 );
	h_massmatchdijetWOMatchBjetsHiggs_1cut100 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_1cut100" , "Mass of dijet w/o MatchBjetsHiggs" , 80.0 , 0.0, 800.0 );
	h_massmatchdijetWOMatchBjetsHiggs_1cut150 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_1cut150" , "Mass of dijet w/o MatchBjetsHiggs" , 80.0 , 0.0, 800.0 );
	h_massmatchdijetWOMatchBjetsHiggs_1cut200 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_1cut200" , "Mass of dijet w/o MatchBjetsHiggs" , 80.0 , 0.0, 800.0 );
	//
	h_RecoBtagJetsdijet_masspt = analysisPlots.make<TH2D>("RecoBtagJetsdijet_masspt" , "Mass of dijet vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_dijetWORecoBjetsCSVM_masspt = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt" , "Mass of Match dijet w/o RecoBtagJets vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_dijetWORecoBjetsCSVM_masspt_1cut50 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_1cut50" , "Mass of Match dijet w/o RecoBtagJets vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_dijetWORecoBjetsCSVM_masspt_1cut100 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_1cut100" , "Mass of Match dijet w/o RecoBtagJets vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_dijetWORecoBjetsCSVM_masspt_1cut150 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_1cut150" , "Mass of Match dijet w/o RecoBtagJets vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_dijetWORecoBjetsCSVM_masspt_1cut200 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_1cut200" , "Mass of Match dijet w/o RecoBtagJets vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massdijetWORecoBjetsCSVM = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM" , "Mass of dijet w/o RecoBtagJets" , 80.0 , 0.0, 800.0 );
	h_massdijetWORecoBjetsCSVM_1cut50 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_1cut50" , "Mass of dijet w/o RecoBtagJets" , 80.0 , 0.0, 800.0 );
	h_massdijetWORecoBjetsCSVM_1cut100 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_1cut100" , "Mass of dijet w/o RecoBtagJets" , 80.0 , 0.0, 800.0 );
	h_massdijetWORecoBjetsCSVM_1cut150 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_1cut150" , "Mass of dijet w/o RecoBtagJets" , 80.0 , 0.0, 800.0 );
	h_massdijetWORecoBjetsCSVM_1cut200 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_1cut200" , "Mass of dijet w/o RecoBtagJets" , 80.0 , 0.0, 800.0 );
	//
	h_RecoPartonFlavorBtagJetsdijet_masspt = analysisPlots.make<TH2D>("RecoPartonFlavorBtagJetsdijet_masspt" , "Mass of dijet vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt" , "Mass of Match dijet w/o RecoPartonFlavorBtagJets vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut50 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut50" , "Mass of Match dijet w/o RecoPartonFlavorBtagJets vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut100 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut100" , "Mass of Match dijet w/o RecoPartonFlavorBtagJets vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut150 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut150" , "Mass of Match dijet w/o RecoPartonFlavorBtagJets vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut200 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut200" , "Mass of Match dijet w/o RecoPartonFlavorBtagJets vs scalar #sum p_{T}^{dijet}" , 80.0 , 0.0, 800.0, 80 , 0.0, 800.0 );
	h_massdijetWORecoPartonFlavorBjetsCSVM = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM" , "Mass of dijet w/o RecoPartonFlavorBtagJets" , 80.0 , 0.0, 800.0 );
	h_massdijetWORecoPartonFlavorBjetsCSVM_1cut50 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_1cut50" , "Mass of dijet w/o RecoPartonFlavorBtagJets" , 80.0 , 0.0, 800.0 );
	h_massdijetWORecoPartonFlavorBjetsCSVM_1cut100 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_1cut100" , "Mass of dijet w/o RecoPartonFlavorBtagJets" , 80.0 , 0.0, 800.0 );
	h_massdijetWORecoPartonFlavorBjetsCSVM_1cut150 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_1cut150" , "Mass of dijet w/o RecoPartonFlavorBtagJets" , 80.0 , 0.0, 800.0 );
	h_massdijetWORecoPartonFlavorBjetsCSVM_1cut200 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_1cut200" , "Mass of dijet w/o RecoPartonFlavorBtagJets" , 80.0 , 0.0, 800.0 );


	h_recoMatchBjetsdijet_masspt = analysisPlots.make<TH2D>("recoMatchBjetsdijet_masspt" , "Mass of MatchBjetsdijet vs scalar #sum p_{T}^{MatchBjetsdijet}" , 120.0 , 0.0, 1200.0, 120 , 0.0, 1200.0 );

	h_MatchBjetsdijet_masspt = analysisPlots.make<TH2D>("MatchBjetsdijet_masspt" , "Mass of MatchBjetsdijet vs scalar #sum p_{T}^{MatchBjetsdijet}" , 120.0 , 0.0, 1200.0, 120 , 0.0, 1200.0 );
	h_massrecoMatchBjetsdijet = analysisPlots.make<TH1D>("massrecoMatchBjetsdijet" , "Mass of Reco MatchBjetsdijet" , 80.0 , 0.0, 800.0 );
	h_massMatchBjetsdijet = analysisPlots.make<TH1D>("massMatchBjetsdijet" , "Mass of MatchBjetsdijet" , 80.0 , 0.0, 800.0 );

}


MyAnalyzer::~MyAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

