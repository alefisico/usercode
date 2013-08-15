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

	void BasicPlots( vector< TLorentzVector > p4GenTruthAlljets, vector< TLorentzVector > p4GenTruthJets, vector< TLorentzVector > p4GenTruthBjets, vector< TLorentzVector > p4GenTruthBjetsHiggs, vector< TLorentzVector > p4GenTruthB1Higgs, vector< TLorentzVector > p4GenTruthB2Higgs, vector< TLorentzVector > p4GenTruthJ1Stop1, vector< TLorentzVector > p4GenTruthJ2Stop1, vector< TLorentzVector > p4GenTruthJetsStop1, vector< TLorentzVector > p4GenTruthBjetsStop1, vector< TLorentzVector > p4MatchTruthJets,  vector< TLorentzVector > p4MatchTruthBjets, vector< TLorentzVector > p4MatchB1Higgs, vector< TLorentzVector > p4MatchB2Higgs, vector< TLorentzVector > p4MatchJ1Stop1, vector< TLorentzVector > p4MatchJ2Stop1,  vector< TLorentzVector > p4RecoJets, vector< TLorentzVector > p4RecoBjetsCSVM );
	void Analysis(  vector< TLorentzVector > p4GenTruthB1Higgs, vector< TLorentzVector > p4GenTruthB2Higgs, vector< TLorentzVector > p4GenTruthJ1Stop1, vector< TLorentzVector > p4GenTruthJ2Stop1, vector< TLorentzVector > p4RecoJets, vector< TLorentzVector > p4RecoBjetsCSVM, vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM, vector< TLorentzVector > p4MatchTruthBjets,  vector< TLorentzVector > p4MatchB1Higgs, vector< TLorentzVector > p4MatchB2Higgs,  vector< TLorentzVector > p4MatchJ1Stop1, vector< TLorentzVector > p4MatchJ2Stop1, vector< TLorentzVector > p4MatchTruthAlljets);

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
	TH1D * h_MatchJ1Stop1_deltaR;
	TH1D * h_MatchJ2Stop1_deltaR;

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
	TH1D * h_genTruthJetsStop1_mass;
	TH1D * h_genTruthJetsStop1_num;
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
	TH1D * h_matchJetsStop1_mass;
	TH1D * h_matchJetsStop1_num;


	///// Stop2 histos
	TH1D * h_stop21_mass;
	TH1D * h_stop21_pt;
	TH1D * h_stop21_eta;
	TH1D * h_stop22_mass;
	TH1D * h_stop22_pt;
	TH1D * h_stop22_eta;

	///// RECO histos
	TH1D * h_massRecoBjetsCSVM;
	TH1D * h_massRecoBjetsCSVM_cutDiagHiggsbb50;
	TH1D * h_massRecoBjetsCSVM_cutDiagHiggsbb100;
	TH1D * h_massRecoBjetsCSVM_cutDiagHiggsbb150;
	TH1D * h_massRecoBjetsCSVM_cutDiagHiggsbb200;
	TH1D * h_avgMassRecoBjetsCSVM;
	TH1D * h_avgMassRecoBjetsCSVM_cutDiagHiggsbb50;
	TH1D * h_avgMassRecoBjetsCSVM_cutDiagHiggsbb100;
	TH1D * h_avgMassRecoBjetsCSVM_cutDiagHiggsbb150;
	TH1D * h_avgMassRecoBjetsCSVM_cutDiagHiggsbb200;
	TH2D * h_avgPtRecoBjetsCSVM;
	TH2D * h_avgPtRecoBjetsCSVM_cutDiagHiggsbb50;
	TH2D * h_avgPtRecoBjetsCSVM_cutDiagHiggsbb100;
	TH2D * h_avgPtRecoBjetsCSVM_cutDiagHiggsbb150;
	TH2D * h_avgPtRecoBjetsCSVM_cutDiagHiggsbb200;
	TH2D * h_scalarPtRecoBjetsCSVM;
	TH2D * h_scalarPtRecoBjetsCSVM_cutDiagHiggsbb50;
	TH2D * h_scalarPtRecoBjetsCSVM_cutDiagHiggsbb100;
	TH2D * h_scalarPtRecoBjetsCSVM_cutDiagHiggsbb150;
	TH2D * h_scalarPtRecoBjetsCSVM_cutDiagHiggsbb200;
	TH2D * h_PtRecoBjetsCSVM;
	//
	TH1D * h_massRecoPartonFlavorBjetsCSVM;
	TH1D * h_massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
	TH1D * h_massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
	TH1D * h_massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
	TH1D * h_massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;
	TH1D * h_avgMassRecoPartonFlavorBjetsCSVM;
	TH1D * h_avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
	TH1D * h_avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
	TH1D * h_avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
	TH1D * h_avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;
	TH2D * h_avgPtRecoPartonFlavorBjetsCSVM;
	TH2D * h_avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
	TH2D * h_avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
	TH2D * h_avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
	TH2D * h_avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;
	TH2D * h_scalarPtRecoPartonFlavorBjetsCSVM;
	TH2D * h_scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
	TH2D * h_scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
	TH2D * h_scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
	TH2D * h_scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;
	TH2D * h_PtRecoPartonFlavorBjetsCSVM;
	//
	TH1D * h_massMatchBjets;
	TH1D * h_massMatchBjets_cutDiagHiggsbb50;
	TH1D * h_massMatchBjets_cutDiagHiggsbb100;
	TH1D * h_massMatchBjets_cutDiagHiggsbb150;
	TH1D * h_massMatchBjets_cutDiagHiggsbb200;
	TH1D * h_avgMassMatchBjets;
	TH1D * h_avgMassMatchBjets_cutDiagHiggsbb50;
	TH1D * h_avgMassMatchBjets_cutDiagHiggsbb100;
	TH1D * h_avgMassMatchBjets_cutDiagHiggsbb150;
	TH1D * h_avgMassMatchBjets_cutDiagHiggsbb200;
	TH2D * h_avgPtMatchBjets;
	TH2D * h_avgPtMatchBjets_cutDiagHiggsbb50;
	TH2D * h_avgPtMatchBjets_cutDiagHiggsbb100;
	TH2D * h_avgPtMatchBjets_cutDiagHiggsbb150;
	TH2D * h_avgPtMatchBjets_cutDiagHiggsbb200;
	TH2D * h_scalarPtMatchBjets;
	TH2D * h_scalarPtMatchBjets_cutDiagHiggsbb50;
	TH2D * h_scalarPtMatchBjets_cutDiagHiggsbb100;
	TH2D * h_scalarPtMatchBjets_cutDiagHiggsbb150;
	TH2D * h_scalarPtMatchBjets_cutDiagHiggsbb200;
	TH2D * h_PtMatchBjets;
	//
	TH1D * h_massMatchBjetsHiggs;
	TH1D * h_massMatchBjetsHiggs_num;
	TH1D * h_massMatchBjetsHiggs_cutDiagHiggsbb50;
	TH1D * h_massMatchBjetsHiggs_cutDiagHiggsbb100;
	TH1D * h_massMatchBjetsHiggs_cutDiagHiggsbb150;
	TH1D * h_massMatchBjetsHiggs_cutDiagHiggsbb200;
	TH1D * h_avgMassMatchBjetsHiggs;
	TH1D * h_avgMassMatchBjetsHiggs_cutDiagHiggsbb50;
	TH1D * h_avgMassMatchBjetsHiggs_cutDiagHiggsbb100;
	TH1D * h_avgMassMatchBjetsHiggs_cutDiagHiggsbb150;
	TH1D * h_avgMassMatchBjetsHiggs_cutDiagHiggsbb200;
	TH2D * h_avgPtMatchBjetsHiggs;
	TH2D * h_avgPtMatchBjetsHiggs_cutDiagHiggsbb50;
	TH2D * h_avgPtMatchBjetsHiggs_cutDiagHiggsbb100;
	TH2D * h_avgPtMatchBjetsHiggs_cutDiagHiggsbb150;
	TH2D * h_avgPtMatchBjetsHiggs_cutDiagHiggsbb200;
	TH2D * h_scalarPtMatchBjetsHiggs;
	TH2D * h_scalarPtMatchBjetsHiggs_cutDiagHiggsbb50;
	TH2D * h_scalarPtMatchBjetsHiggs_cutDiagHiggsbb100;
	TH2D * h_scalarPtMatchBjetsHiggs_cutDiagHiggsbb150;
	TH2D * h_scalarPtMatchBjetsHiggs_cutDiagHiggsbb200;
	TH2D * h_PtMatchBjetsHiggs;
	TH1D * h_massGenTruthHiggs;
	TH2D * h_PtGenTruthHiggs;
	TH2D * h_scalarPtGenTruthHiggs;

	TH2D * h_matchdijet_masspt;
	TH2D * h_matchdijetWOMatchBjets_masspt;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagStop1jj50;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagStop1jj100;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagStop1jj150;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagStop1jj200;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj50;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj100;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj150;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj200;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj50;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj100;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj150;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj200;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj50;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj100;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj150;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj200;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj50;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj100;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj150;
	TH2D * h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj200;
	TH1D * h_massmatchdijetWOMatchBjets;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagStop1jj50;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagStop1jj100;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagStop1jj150;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagStop1jj200;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb50;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj50;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj100;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj150;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj200;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb100;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj50;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj100;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj150;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj200;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb150;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj50;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj100;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj150;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj200;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb200;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj50;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj100;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj150;
	TH1D * h_massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj200;

	TH2D * h_matchdijetHiggs_masspt;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj50;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj100;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj150;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj200;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj50;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj100;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj150;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj200;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj50;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj100;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj150;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj200;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj50;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj100;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj150;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj200;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj50;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj100;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj150;
	TH2D * h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj200;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj50;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj100;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj150;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj200;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj50;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj100;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj150;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj200;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj50;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj100;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj150;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj200;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj50;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj100;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj150;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj200;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj50;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj100;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj150;
	TH1D * h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj200;

	TH2D * h_recoDijet_masspt;
	TH2D * h_dijetWORecoBjetsCSVM_masspt;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj50;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj100;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj150;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj200;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150;
	TH2D * h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200;
	TH1D * h_massdijetWORecoBjetsCSVM;
	TH1D * h_massdijetWORecoBjetsCSVM_WOStop1;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagStop1jj50;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagStop1jj50_WOStop1;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagStop1jj100;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagStop1jj150;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagStop1jj200;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb50;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb100;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb150;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb200;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150;
	TH1D * h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200;

	TH2D * h_recoPartonFlavorDijet_masspt;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj50;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj100;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj150;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj200;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150;
	TH2D * h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj50;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj100;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj150;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj200;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150;
	TH1D * h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200;

	TH1D * h_jetsStop1WORecoBjetsCSVM_num;

	TH1D * h_tmpmassdijetWORecoBjetsCSVM;
	TH1D * h_tmpmassmatchdijetWOMatchBjets;

	TH2D * h_recoDiBjetDiJet_masspt;
	TH2D * h_recoDiBjetDiJet_MassPt;
	TH1D * h_massRecoDiBjetDiJet;
	TH2D * h_recoDiBjetDiJet_cutDiagStop2bbjj50_masspt;
	TH1D * h_massRecoDiBjetDiJet_cutDiagStop2bbjj50;
	TH2D * h_recoDiBjetDiJet_cutDiagStop2bbjj50_MassPt;
	TH1D * h_massPtRecoDiBjetDiJet_cutDiagStop2bbjj50;
	TH2D * h_recoDiBjetDiJet_cutDiagStop2bbjj100_masspt;
	TH1D * h_massRecoDiBjetDiJet_cutDiagStop2bbjj100;
	TH2D * h_recoDiBjetDiJet_cutDiagStop1jj50_masspt;
	TH1D * h_massRecoDiBjetDiJet_cutDiagStop1jj50;
	TH2D * h_recoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50_masspt;
	TH1D * h_massRecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50;
	TH2D * h_recoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100_masspt;
	TH1D * h_massRecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100;
	TH2D * h_recoDiBjetDiJet_cutDiagStop1jj100_masspt;
	TH1D * h_massRecoDiBjetDiJet_cutDiagStop1jj100;
	TH2D * h_recoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50_masspt;
	TH1D * h_massRecoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50;
	TH2D * h_recoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100_masspt;
	TH1D * h_massRecoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100;

	TH2D * h_matchDiBjetDiJet_masspt;
	TH1D * h_massMatchDiBjetDiJet;
	TH2D * h_matchDiBjetDiJet_cutDiagStop2bbjj50_masspt;
	TH1D * h_massMatchDiBjetDiJet_cutDiagStop2bbjj50;
	TH2D * h_matchDiBjetDiJet_cutDiagStop2bbjj100_masspt;
	TH1D * h_massMatchDiBjetDiJet_cutDiagStop2bbjj100;
	TH2D * h_matchDiBjetDiJet_cutDiagStop1jj50_masspt;
	TH1D * h_massMatchDiBjetDiJet_cutDiagStop1jj50;
	TH2D * h_matchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50_masspt;
	TH1D * h_massMatchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50;
	TH2D * h_matchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100_masspt;
	TH1D * h_massMatchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100;
	TH2D * h_matchDiBjetDiJet_cutDiagStop1jj100_masspt;
	TH1D * h_massMatchDiBjetDiJet_cutDiagStop1jj100;
	TH2D * h_matchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50_masspt;
	TH1D * h_massMatchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50;
	TH2D * h_matchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100_masspt;
	TH1D * h_massMatchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100;

	TH2D * h_matchHiggsDiBjetStop1DiJet_masspt;
	TH1D * h_massMatchHiggsDiBjetStop1DiJet;


	TH2D * h_recoDiBjetDiJetSmallestDeltaM_masspt;
	TH1D * h_massRecoDiBjetDiJetSmallestDeltaM;
	TH2D * h_matchDiBjetDiJetSmallestDeltaM_masspt;
	TH1D * h_massMatchDiBjetDiJetSmallestDeltaM;
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
	////// bin size, max and min for plots
	int nbinPt = 100;
	double minPt = 0;
	double maxPt = 1000;
	int nbinNum = 21;
	double minNum = -0.5;
	double maxNum = 20.5;
	int nbinEta = 40;
	double minEta = -5.0;
	double maxEta = 5.0;
	int nbinDeltaR = 50;
	double minDeltaR = 0.0;
	double maxDeltaR = 5.0;
	int nbincosDelta = 26;
	double mincosDelta = -1.2;
	double maxcosDelta = 1.2;

	///// Matching Histos
	h_MatchTruthAlljets_deltaR = basicPlots.make<TH1D>("matchTruthAllJets_deltaR" , "#Delta R (RecoJets, All Truth GenJets)" , nbinDeltaR, minDeltaR, maxDeltaR );
	h_MatchTruthJets_deltaR = basicPlots.make<TH1D>("matchTruthJets_deltaR" , "#Delta R (RecoJets, Truth GenJets)" , nbinDeltaR, minDeltaR, maxDeltaR );
	h_MatchTruthBjets_deltaR = basicPlots.make<TH1D>("matchTruthBjets_deltaR" , "#Delta R (RecoBjets, Truth GenBjets)" , nbinDeltaR, minDeltaR, maxDeltaR );
	h_MatchB1Higgs_deltaR = basicPlots.make<TH1D>("matchB1Higgs_deltaR" , "#Delta R (RecoBjets, Higgs 1 Bjets)" , nbinDeltaR, minDeltaR, maxDeltaR );
	h_MatchB2Higgs_deltaR = basicPlots.make<TH1D>("matchB2Higgs_deltaR" , "#Delta R (RecoBjets, Higgs 2 Bjets)" , nbinDeltaR, minDeltaR, maxDeltaR );
	h_MatchJ1Stop1_deltaR = basicPlots.make<TH1D>("matchJ1Stop1_deltaR" , "#Delta R (RecoBjets, Stop1 1 Jets)" , nbinDeltaR, minDeltaR, maxDeltaR );
	h_MatchJ2Stop1_deltaR = basicPlots.make<TH1D>("matchJ2Stop1_deltaR" , "#Delta R (RecoBjets, Stop1 2 Jets)" , nbinDeltaR, minDeltaR, maxDeltaR );

	h_MatchTruthAlljets_deltaR->Sumw2();
	h_MatchTruthJets_deltaR->Sumw2();
	h_MatchTruthBjets_deltaR->Sumw2();
	h_MatchB1Higgs_deltaR->Sumw2();
	h_MatchB2Higgs_deltaR->Sumw2();
	h_MatchJ1Stop1_deltaR->Sumw2();
	h_MatchJ2Stop1_deltaR->Sumw2();

	//// Bjets histos
	h_genTruthBjets_num = genTruthPlots.make<TH1D>("genTruthBjets_num" , "Number of GenTruth bJets" , nbinNum, minNum, maxNum );
	h_genTruthBjets_pt = genTruthPlots.make<TH1D>("genTruthBjets_pt" , "GenTruth bJets p_{T}" , nbinPt, minPt, maxPt );
	h_genTruthBjets_sumpt = genTruthPlots.make<TH1D>("genTruthBjets_sumpt" , "#sum GenTruth bJets p_{T}" , nbinPt, minPt, maxPt );
	h_genTruthBjets_eta = genTruthPlots.make<TH1D>("genTruthBjets_eta" , "GenTruth bJets #eta" , nbinEta, minEta, maxEta );
	h_genTruthBjets_phi = genTruthPlots.make<TH1D>("genTruthBjets_phi" , "GenTruth bJets #phi" , nbinEta, minEta, maxEta );
	h_matchBjets_num = basicPlots.make<TH1D>("matchBjets_num" , "Number of Match bJets" , nbinNum, minNum, maxNum );
	h_matchBjets_pt = basicPlots.make<TH1D>("matchBjets_pt" , "Match bJets p_{T}" , nbinPt, minPt, maxPt );
	h_matchBjets_sumpt = basicPlots.make<TH1D>("matchBjets_sumpt" , "#sum Match bJets p_{T}" , nbinPt, minPt, maxPt );
	h_matchBjets_eta = basicPlots.make<TH1D>("matchBjets_eta" , "Match bJets #eta" , nbinEta, minEta, maxEta );
	h_matchBjets_phi = basicPlots.make<TH1D>("matchBjets_phi" , "Match bJets #phi" , nbinEta, minEta, maxEta );
	h_matchBjetsHiggs_num = basicPlots.make<TH1D>("matchBjetsHiggs_num" , "Number of Match Higgs bJets" , nbinNum, minNum, maxNum );
	h_matchBjetsHiggs1_num = basicPlots.make<TH1D>("matchBjetsHiggs1_num" , "Number of Match Higgs 1 bJets" , nbinNum, minNum, maxNum );
	h_matchBjetsHiggs2_num = basicPlots.make<TH1D>("matchBjetsHiggs2_num" , "Number of Match Higgs 2 bJets" , nbinNum, minNum, maxNum );
	h_matchBjetsHiggs_pt = basicPlots.make<TH1D>("matchBjetsHiggs_pt" , "Match Higgs bJets p_{T}" , nbinPt, minPt, maxPt );
	h_matchBjetsHiggs_mass = basicPlots.make<TH1D>("matchBjetsHiggs_mass" , "Match Higgs Mass" , nbinPt, minPt, maxPt );
	h_matchBjetsHiggs_sumpt = basicPlots.make<TH1D>("matchBjetsHiggs_sumpt" , "#sum Match bJets p_{T}" , nbinPt, minPt, maxPt );
	h_matchBjetsHiggs_eta = basicPlots.make<TH1D>("matchBjetsHiggs_eta" , "Match Higgs bJets #eta" , nbinEta, minEta, maxEta );
	h_matchBjetsHiggs_phi = basicPlots.make<TH1D>("matchBjetsHiggs_phi" , "Match Higgs bJets #phi" , nbinEta, minEta, maxEta );

	h_genTruthBjets_num->Sumw2();
	h_genTruthBjets_pt->Sumw2();
	h_genTruthBjets_sumpt->Sumw2();
	h_genTruthBjets_eta->Sumw2();
	h_genTruthBjets_phi->Sumw2();
	h_matchBjets_num->Sumw2();
	h_matchBjets_pt->Sumw2();
	h_matchBjets_sumpt->Sumw2();
	h_matchBjets_eta->Sumw2();
	h_matchBjets_phi->Sumw2();
	h_matchBjetsHiggs_num->Sumw2();
	h_matchBjetsHiggs1_num->Sumw2();
	h_matchBjetsHiggs2_num->Sumw2();
	h_matchBjetsHiggs_pt->Sumw2();
	h_matchBjetsHiggs_mass->Sumw2();
	h_matchBjetsHiggs_sumpt->Sumw2();
	h_matchBjetsHiggs_eta->Sumw2();
	h_matchBjetsHiggs_phi->Sumw2();


	//// Jets histos
	h_genTruthJets_num = genTruthPlots.make<TH1D>("genTruthJets_num" , "Number of GenTruth Jets" , nbinNum, minNum, maxNum );
	h_genTruthJets_pt = genTruthPlots.make<TH1D>("genTruthJets_pt" , "GenTruth Jets p_{T}" , nbinPt, minPt, maxPt );
	h_genTruthJets_sumpt = genTruthPlots.make<TH1D>("genTruthJets_sumpt" , "#sum GenTruth Jets p_{T}" , nbinPt, minPt, maxPt );
	h_genTruthJets_eta = genTruthPlots.make<TH1D>("genTruthJets_eta" , "GenTruth Jets #eta" , nbinEta, minEta, maxEta );
	h_genTruthJets_phi = genTruthPlots.make<TH1D>("genTruthJets_phi" , "GenTruth Jets #phi" , nbinEta, minEta, maxEta );
	h_recoJets_num = basicPlots.make<TH1D>("recoJets_num" , "Number of Reco Jets" , nbinNum, minNum, maxNum );
	h_recoJets_pt = basicPlots.make<TH1D>("recoJets_pt" , "Reco Jets p_{T}" , nbinPt, minPt, maxPt );
	h_recoJets_sumpt = basicPlots.make<TH1D>("recoJets_sumpt" , "#sum Reco Jets p_{T}" , nbinPt, minPt, maxPt );
	h_recoJets_eta = basicPlots.make<TH1D>("recoJets_eta" , "Reco Jets #eta" , nbinEta, minEta, maxEta );
	h_recoJets_phi = basicPlots.make<TH1D>("recoJets_phi" , "Reco Jets #phi" , nbinEta, minEta, maxEta );
	h_recoBjets_num = basicPlots.make<TH1D>("recoBjets_num" , "Number of Reco Bjets" , nbinNum, minNum, maxNum );
	h_recoBjets_pt = basicPlots.make<TH1D>("recoBjets_pt" , "Reco Bjets p_{T}" , nbinPt, minPt, maxPt );
	h_recoBjets_sumpt = basicPlots.make<TH1D>("recoBjets_sumpt" , "#sum Reco Bjets p_{T}" , nbinPt, minPt, maxPt );
	h_recoBjets_eta = basicPlots.make<TH1D>("recoBjets_eta" , "Reco Bjets #eta" , nbinEta, minEta, maxEta );
	h_recoBjets_phi = basicPlots.make<TH1D>("recoBjets_phi" , "Reco Bjets #phi" , nbinEta, minEta, maxEta );
	h_matchJets_num = basicPlots.make<TH1D>("matchJets_num" , "Number of Match Jets" , nbinNum, minNum, maxNum );
	h_matchJets_pt = basicPlots.make<TH1D>("matchJets_pt" , "Match Jets p_{T}" , nbinPt, minPt, maxPt );
	h_matchJets_sumpt = basicPlots.make<TH1D>("matchJets_sumpt" , "#sum Match Jets p_{T}" , nbinPt, minPt, maxPt );
	h_matchJets_eta = basicPlots.make<TH1D>("matchJets_eta" , "Match Jets #eta" , nbinEta, minEta, maxEta );
	h_matchJets_phi = basicPlots.make<TH1D>("matchJets_phi" , "Match Jets #phi" , nbinEta, minEta, maxEta );

	h_genTruthJets_num->Sumw2();
	h_genTruthJets_pt->Sumw2();
	h_genTruthJets_sumpt->Sumw2();
	h_genTruthJets_eta->Sumw2();
	h_genTruthJets_phi->Sumw2();
	h_recoJets_num->Sumw2();
	h_recoJets_pt->Sumw2();
	h_recoJets_sumpt->Sumw2();
	h_recoJets_eta->Sumw2();
	h_recoJets_phi->Sumw2();
	h_recoBjets_num->Sumw2();
	h_recoBjets_pt->Sumw2();
	h_recoBjets_sumpt->Sumw2();
	h_recoBjets_eta->Sumw2();
	h_recoBjets_phi->Sumw2();
	h_matchJets_num->Sumw2();
	h_matchJets_pt->Sumw2();
	h_matchJets_sumpt->Sumw2();
	h_matchJets_eta->Sumw2();
	h_matchJets_phi->Sumw2();

	//// Alljets histos
	h_genTruthAlljets_num = genTruthPlots.make<TH1D>("genTruthAlljets_num" , "Number of GenTruth Alljets" , nbinNum, minNum, maxNum );
	h_genTruthAlljets_pt = genTruthPlots.make<TH1D>("genTruthAlljets_pt" , "GenTruth Alljets p_{T}" , nbinPt, minPt, maxPt );
	h_genTruthAlljets_sumpt = genTruthPlots.make<TH1D>("genTruthAlljets_sumpt" , "#sum GenTruth Alljets p_{T}" , nbinPt, minPt, maxPt );
	h_genTruthAlljets_eta = genTruthPlots.make<TH1D>("genTruthAlljets_eta" , "GenTruth Alljets #eta" , nbinEta, minEta, maxEta );
	h_genTruthAlljets_phi = genTruthPlots.make<TH1D>("genTruthAlljets_phi" , "GenTruth Alljets #phi" , nbinEta, minEta, maxEta );

	h_genTruthAlljets_num->Sumw2();
	h_genTruthAlljets_pt->Sumw2();
	h_genTruthAlljets_sumpt->Sumw2();
	h_genTruthAlljets_eta->Sumw2();
	h_genTruthAlljets_phi->Sumw2();

	///// Higgs histos
	h_bjetsHiggs_pt = genTruthPlots.make<TH1D>("bjetsHiggs_pt" , "Bjets from Higgs p_{T}" , nbinPt, minPt, maxPt );
	h_bjetsHiggs_eta = genTruthPlots.make<TH1D>("bjetsHiggs_eta" , "Bjets from Higgs #eta" , nbinEta, minEta, maxEta );
	h_bjetsHiggs_phi = genTruthPlots.make<TH1D>("bjetsHiggs_phi" , "Bjets from Higgs #phi" , nbinEta, minEta, maxEta );
	h_genTruthBjetsHiggs_mass = genTruthPlots.make<TH1D>("genTruthBjetsHiggs_mass" , "GenTruth Higgs Mass" , nbinPt, minPt, maxPt );
	h_genTruthBjetsHiggs_num = genTruthPlots.make<TH1D>("genTruthBjetsHiggs_num" , "Number GenTruth Higgs" , nbinNum, minNum, maxNum );
	h_higgs1_mass = genTruthPlots.make<TH1D>("higgs1_mass" , "1st Higgs Mass" , nbinPt, minPt, maxPt );
	h_higgs1_pt = genTruthPlots.make<TH1D>("higgs1_pt" , "1st Higgs p_{T}" , nbinPt, minPt, maxPt );
	h_higgs1_eta = genTruthPlots.make<TH1D>("higgs1_eta" , "1st Higgs #eta" , nbinEta, minEta, maxEta );
	h_higgs2_mass = genTruthPlots.make<TH1D>("higgs2_mass" , "2nd Higgs Mass" , nbinPt, minPt, maxPt );
	h_higgs2_pt = genTruthPlots.make<TH1D>("higgs2_pt" , "2nd Higgs p_{T}" , nbinPt, minPt, maxPt );
	h_higgs2_eta = genTruthPlots.make<TH1D>("higgs2_eta" , "2nd Higgs #eta" , nbinEta, minEta, maxEta );
	h_higgs_deltaR = genTruthPlots.make<TH1D>("higgs_deltaR" , "#Delta R (h_{1},h_{2})" , nbinDeltaR, minDeltaR, maxDeltaR );
	h_higgs_deltaPhi = genTruthPlots.make<TH1D>("higgs_deltaPhi" , "#Delta #phi (h_{1}, h_{2})" , nbinDeltaR, minDeltaR, maxDeltaR );
	h_higgs_cosDeltaPhi = genTruthPlots.make<TH1D>("higgs_cosDeltaPhi" , "cos [#Delta #phi (h_{1}, h_{2})]" , nbincosDelta, mincosDelta, maxcosDelta );

	h_bjetsHiggs_pt->Sumw2();
	h_bjetsHiggs_eta->Sumw2();
	h_bjetsHiggs_phi->Sumw2();
	h_genTruthBjetsHiggs_mass->Sumw2();
	h_genTruthBjetsHiggs_num->Sumw2();
	h_higgs1_mass->Sumw2();
	h_higgs1_pt->Sumw2();
	h_higgs1_eta->Sumw2();
	h_higgs2_mass->Sumw2();
	h_higgs2_pt->Sumw2();
	h_higgs2_eta->Sumw2();
	h_higgs_deltaR->Sumw2();
	h_higgs_deltaPhi->Sumw2();
	h_higgs_cosDeltaPhi->Sumw2();

	////// Stop1 histos
	h_genTruthJetsStop1_mass = genTruthPlots.make<TH1D>("genTruthJetsStop1_mass" , "GenTruth Stop1 Mass" , nbinPt, minPt, maxPt );
	h_genTruthJetsStop1_num = genTruthPlots.make<TH1D>("genTruthJetsStop1_num" , "Number GenTruth Stop1" , nbinNum, minNum, maxNum );
	h_bjetsStop1_pt = genTruthPlots.make<TH1D>("bjetsStop1_pt" , "Bjets from Stop1 p_{T}" , nbinPt, minPt, maxPt );
	h_bjetsStop1_eta = genTruthPlots.make<TH1D>("bjetsStop1_eta" , "Bjets from Stop1 #eta" , nbinEta, minEta, maxEta );
	h_bjetsStop1_phi = genTruthPlots.make<TH1D>("bjetsStop1_phi" , "Bjets from Stop1 #phi" , nbinEta, minEta, maxEta );
	h_stop11_mass = genTruthPlots.make<TH1D>("stop11_mass" , "1st Stop1 Mass" ,nbinPt, minPt, maxPt );
	h_stop11_pt = genTruthPlots.make<TH1D>("stop11_pt" , "1st Stop1 p_{T}" , nbinPt, minPt, maxPt );
	h_stop11_eta = genTruthPlots.make<TH1D>("stop11_eta" , "1st Stop1 #eta" , nbinEta, minEta, maxEta );
	h_stop12_mass = genTruthPlots.make<TH1D>("stop12_mass" , "2nd Stop1 Mass" , nbinPt, minPt, maxPt );
	h_stop12_pt = genTruthPlots.make<TH1D>("stop12_pt" , "2nd Stop1 p_{T}" , nbinPt, minPt, maxPt );
	h_stop12_eta = genTruthPlots.make<TH1D>("stop12_eta" , "2nd Stop1 #eta" , nbinEta, minEta, maxEta );
	h_stop1_deltaR = genTruthPlots.make<TH1D>("stop1_deltaR" , "#Delta R (st1_{1},st1_{2})" , nbinDeltaR, minDeltaR, maxDeltaR );
	h_stop1_deltaPhi = genTruthPlots.make<TH1D>("stop1_deltaPhi" , "#Delta #phi (st1_{1}, st1_{2})" , nbinDeltaR, minDeltaR, maxDeltaR );
	h_stop1_cosDeltaPhi = genTruthPlots.make<TH1D>("stop1_cosDeltaPhi" , "cos [#Delta #phi (st1_{1}, st1_{2})]" , nbincosDelta, mincosDelta, maxcosDelta );
	h_matchJetsStop1_mass = basicPlots.make<TH1D>("matchJetsStop1_mass" , "Match Stop1 Mass" , nbinPt, minPt, maxPt );
	h_matchJetsStop1_num = basicPlots.make<TH1D>("matchJetsStop1_num" , "Number Match Stop1" , nbinNum, minNum, maxNum );

	h_genTruthJetsStop1_mass->Sumw2();
	h_genTruthJetsStop1_num->Sumw2();
	h_bjetsStop1_pt->Sumw2();
	h_bjetsStop1_eta->Sumw2();
	h_bjetsStop1_phi->Sumw2();
	h_stop11_mass->Sumw2();
	h_stop11_pt->Sumw2();
	h_stop11_eta->Sumw2();
	h_stop12_mass->Sumw2();
	h_stop12_pt->Sumw2();
	h_stop12_eta->Sumw2();
	h_stop1_deltaR->Sumw2();
	h_stop1_deltaPhi->Sumw2();
	h_stop1_cosDeltaPhi->Sumw2();
	h_matchJetsStop1_mass->Sumw2();
	h_matchJetsStop1_num->Sumw2();

	//// Stop2 histos
	h_stop21_mass = genTruthPlots.make<TH1D>("stop21_mass" , "1st Stop2 Mass" , nbinPt, minPt, maxPt );
	h_stop21_pt = genTruthPlots.make<TH1D>("stop21_pt" , "1st Stop2 p_{T}" , nbinPt, minPt, maxPt );
	h_stop21_eta = genTruthPlots.make<TH1D>("stop21_eta" , "1st Stop2 #eta" , nbinEta, minEta, maxEta );
	h_stop22_mass = genTruthPlots.make<TH1D>("stop22_mass" , "1st Stop2 Mass" , nbinPt, minPt, maxPt );
	h_stop22_pt = genTruthPlots.make<TH1D>("stop22_pt" , "1st Stop2 p_{T}" , nbinPt, minPt, maxPt );
	h_stop22_eta = genTruthPlots.make<TH1D>("stop22_eta" , "1st Stop2 #eta" , nbinEta, minEta, maxEta );

	h_stop21_mass->Sumw2();
	h_stop21_pt->Sumw2();
	h_stop21_eta->Sumw2();
	h_stop22_mass->Sumw2();
	h_stop22_pt->Sumw2();
	h_stop22_eta->Sumw2();

	///// Analysis histos
	// Gen Truth
	h_massGenTruthHiggs = analysisPlots.make<TH1D>("massGenTruthHiggs" , "Mass of GenTruthHiggs" , nbinPt, minPt, maxPt );
	h_scalarPtGenTruthHiggs = analysisPlots.make<TH2D>("scalarPtGenTruthHiggs" , "Mass of GenTruthHiggs vs scalar #sum p_{T}^{GenTruthHiggs}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_PtGenTruthHiggs = analysisPlots.make<TH2D>("PtGenTruthHiggs" , "Mass of GenTruthHiggs vs #sum p_{T}^{GenTruthHiggs}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_massGenTruthHiggs->Sumw2();
	h_scalarPtGenTruthHiggs->Sumw2();
	h_PtGenTruthHiggs->Sumw2();

	// Match bjets
	h_massMatchBjets = analysisPlots.make<TH1D>("massMatchBjets" , "Mass of MatchBjets" , nbinPt, minPt, maxPt );
	h_massMatchBjets_cutDiagHiggsbb50 = analysisPlots.make<TH1D>("massMatchBjets_cutDiagHiggsbb50" , "Mass of MatchBjets" , nbinPt, minPt, maxPt );
	h_massMatchBjets_cutDiagHiggsbb100 = analysisPlots.make<TH1D>("massMatchBjets_cutDiagHiggsbb100" , "Mass of MatchBjets" , nbinPt, minPt, maxPt );
	h_massMatchBjets_cutDiagHiggsbb150 = analysisPlots.make<TH1D>("massMatchBjets_cutDiagHiggsbb150" , "Mass of MatchBjets" , nbinPt, minPt, maxPt );
	h_massMatchBjets_cutDiagHiggsbb200 = analysisPlots.make<TH1D>("massMatchBjets_cutDiagHiggsbb200" , "Mass of MatchBjets" , nbinPt, minPt, maxPt );
	h_avgMassMatchBjets = analysisPlots.make<TH1D>("avgMassMatchBjets" , "Average Mass of MatchBjets" , nbinPt, minPt, maxPt );
	h_avgMassMatchBjets_cutDiagHiggsbb50 = analysisPlots.make<TH1D>("avgMassMatchBjets_cutDiagHiggsbb50" , "Average Mass of MatchBjets" , nbinPt, minPt, maxPt );
	h_avgMassMatchBjets_cutDiagHiggsbb100 = analysisPlots.make<TH1D>("avgMassMatchBjets_cutDiagHiggsbb100" , "Average Mass of MatchBjets" , nbinPt, minPt, maxPt );
	h_avgMassMatchBjets_cutDiagHiggsbb150 = analysisPlots.make<TH1D>("avgMassMatchBjets_cutDiagHiggsbb150" , "Average Mass of MatchBjets" , nbinPt, minPt, maxPt );
	h_avgMassMatchBjets_cutDiagHiggsbb200 = analysisPlots.make<TH1D>("avgMassMatchBjets_cutDiagHiggsbb200" , "Average Mass of MatchBjets" , nbinPt, minPt, maxPt );
	h_scalarPtMatchBjets = analysisPlots.make<TH2D>("scalarPtMatchBjets" , "Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_scalarPtMatchBjets_cutDiagHiggsbb50 = analysisPlots.make<TH2D>("scalarPtMatchBjets_cutDiagHiggsbb50" , "Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_scalarPtMatchBjets_cutDiagHiggsbb100 = analysisPlots.make<TH2D>("scalarPtMatchBjets_cutDiagHiggsbb100" , "Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_scalarPtMatchBjets_cutDiagHiggsbb150 = analysisPlots.make<TH2D>("scalarPtMatchBjets_cutDiagHiggsbb150" , "Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_scalarPtMatchBjets_cutDiagHiggsbb200 = analysisPlots.make<TH2D>("scalarPtMatchBjets_cutDiagHiggsbb200" , "Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtMatchBjets = analysisPlots.make<TH2D>("avgPtMatchBjets" , "Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtMatchBjets_cutDiagHiggsbb50 = analysisPlots.make<TH2D>("avgPtMatchBjets_cutDiagHiggsbb50" , "Average Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtMatchBjets_cutDiagHiggsbb100 = analysisPlots.make<TH2D>("avgPtMatchBjets_cutDiagHiggsbb100" , "Average Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtMatchBjets_cutDiagHiggsbb150 = analysisPlots.make<TH2D>("avgPtMatchBjets_cutDiagHiggsbb150" , "Average Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtMatchBjets_cutDiagHiggsbb200 = analysisPlots.make<TH2D>("avgPtMatchBjets_cutDiagHiggsbb200" , "Average Mass of MatchBjets vs scalar #sum p_{T}^{MatchBjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_PtMatchBjets = analysisPlots.make<TH2D>("PtMatchBjets" , "Mass of MatchBjets vs #sum p_{T}^{MatchBjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_massMatchBjets->Sumw2();
	h_massMatchBjets_cutDiagHiggsbb50->Sumw2();
	h_massMatchBjets_cutDiagHiggsbb100->Sumw2();
	h_massMatchBjets_cutDiagHiggsbb150->Sumw2();
	h_massMatchBjets_cutDiagHiggsbb200->Sumw2();
	h_avgMassMatchBjets->Sumw2();
	h_avgMassMatchBjets_cutDiagHiggsbb50->Sumw2();
	h_avgMassMatchBjets_cutDiagHiggsbb100->Sumw2();
	h_avgMassMatchBjets_cutDiagHiggsbb150->Sumw2();
	h_avgMassMatchBjets_cutDiagHiggsbb200->Sumw2();
	h_scalarPtMatchBjets->Sumw2();
	h_scalarPtMatchBjets_cutDiagHiggsbb50->Sumw2();
	h_scalarPtMatchBjets_cutDiagHiggsbb100->Sumw2();
	h_scalarPtMatchBjets_cutDiagHiggsbb150->Sumw2();
	h_scalarPtMatchBjets_cutDiagHiggsbb200->Sumw2();
	h_avgPtMatchBjets->Sumw2();
	h_avgPtMatchBjets_cutDiagHiggsbb50->Sumw2();
	h_avgPtMatchBjets_cutDiagHiggsbb100->Sumw2();
	h_avgPtMatchBjets_cutDiagHiggsbb150->Sumw2();
	h_avgPtMatchBjets_cutDiagHiggsbb200->Sumw2();
	h_PtMatchBjets->Sumw2();

	// Match bjets from Higgs
	h_massMatchBjetsHiggs = analysisPlots.make<TH1D>("massMatchBjetsHiggs" , "Mass of MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massMatchBjetsHiggs_num = analysisPlots.make<TH1D>("massMatchBjetsHiggs_num" , "Num of MatchBjetsHiggs" , nbinNum, minNum, maxNum ); 
	h_massMatchBjetsHiggs_cutDiagHiggsbb50 = analysisPlots.make<TH1D>("massMatchBjetsHiggs_cutDiagHiggsbb50" , "Mass of MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massMatchBjetsHiggs_cutDiagHiggsbb100 = analysisPlots.make<TH1D>("massMatchBjetsHiggs_cutDiagHiggsbb100" , "Mass of MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massMatchBjetsHiggs_cutDiagHiggsbb150 = analysisPlots.make<TH1D>("massMatchBjetsHiggs_cutDiagHiggsbb150" , "Mass of MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massMatchBjetsHiggs_cutDiagHiggsbb200 = analysisPlots.make<TH1D>("massMatchBjetsHiggs_cutDiagHiggsbb200" , "Mass of MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_avgMassMatchBjetsHiggs = analysisPlots.make<TH1D>("avgMassMatchBjetsHiggs" , "Average Mass of MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_avgMassMatchBjetsHiggs_cutDiagHiggsbb50 = analysisPlots.make<TH1D>("avgMassMatchBjetsHiggs_cutDiagHiggsbb50" , "Average Mass of MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_avgMassMatchBjetsHiggs_cutDiagHiggsbb100 = analysisPlots.make<TH1D>("avgMassMatchBjetsHiggs_cutDiagHiggsbb100" , "Average Mass of MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_avgMassMatchBjetsHiggs_cutDiagHiggsbb150 = analysisPlots.make<TH1D>("avgMassMatchBjetsHiggs_cutDiagHiggsbb150" , "Average Mass of MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_avgMassMatchBjetsHiggs_cutDiagHiggsbb200 = analysisPlots.make<TH1D>("avgMassMatchBjetsHiggs_cutDiagHiggsbb200" , "Average Mass of MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_scalarPtMatchBjetsHiggs = analysisPlots.make<TH2D>("scalarPtMatchBjetsHiggs" , "Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_scalarPtMatchBjetsHiggs_cutDiagHiggsbb50 = analysisPlots.make<TH2D>("scalarPtMatchBjetsHiggs_cutDiagHiggsbb50" , "Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_scalarPtMatchBjetsHiggs_cutDiagHiggsbb100 = analysisPlots.make<TH2D>("scalarPtMatchBjetsHiggs_cutDiagHiggsbb100" , "Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", 80.0, 0.0, 800.0, nbinPt, minPt, maxPt );
	h_scalarPtMatchBjetsHiggs_cutDiagHiggsbb150 = analysisPlots.make<TH2D>("scalarPtMatchBjetsHiggs_cutDiagHiggsbb150" , "Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", 80.0, 0.0, 800.0, nbinPt, minPt, maxPt );
	h_scalarPtMatchBjetsHiggs_cutDiagHiggsbb200 = analysisPlots.make<TH2D>("scalarPtMatchBjetsHiggs_cutDiagHiggsbb200" , "Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", 80.0, 0.0, 800.0, nbinPt, minPt, maxPt );
	h_avgPtMatchBjetsHiggs = analysisPlots.make<TH2D>("avgPtMatchBjetsHiggs" , "Average Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtMatchBjetsHiggs_cutDiagHiggsbb50 = analysisPlots.make<TH2D>("avgPtMatchBjetsHiggs_cutDiagHiggsbb50" , "Average Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtMatchBjetsHiggs_cutDiagHiggsbb100 = analysisPlots.make<TH2D>("avgPtMatchBjetsHiggs_cutDiagHiggsbb100" , "Average Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", 80.0, 0.0, 800.0, nbinPt, minPt, maxPt );
	h_avgPtMatchBjetsHiggs_cutDiagHiggsbb150 = analysisPlots.make<TH2D>("avgPtMatchBjetsHiggs_cutDiagHiggsbb150" , "Average Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", 80.0, 0.0, 800.0, nbinPt, minPt, maxPt );
	h_avgPtMatchBjetsHiggs_cutDiagHiggsbb200 = analysisPlots.make<TH2D>("avgPtMatchBjetsHiggs_cutDiagHiggsbb200" , "Average Mass of MatchBjetsHiggs vs scalar #sum p_{T}^{MatchBjetsHiggs}", 80.0, 0.0, 800.0, nbinPt, minPt, maxPt );
	h_PtMatchBjetsHiggs = analysisPlots.make<TH2D>("PtMatchBjetsHiggs" , "Mass of MatchBjetsHiggs vs #sum p_{T}^{MatchBjetsHiggs}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_massMatchBjetsHiggs->Sumw2();
	h_massMatchBjetsHiggs_num->Sumw2();
	h_massMatchBjetsHiggs_cutDiagHiggsbb50->Sumw2();
	h_massMatchBjetsHiggs_cutDiagHiggsbb100->Sumw2();
	h_massMatchBjetsHiggs_cutDiagHiggsbb150->Sumw2();
	h_massMatchBjetsHiggs_cutDiagHiggsbb200->Sumw2();
	h_avgMassMatchBjetsHiggs->Sumw2();
	h_avgMassMatchBjetsHiggs_cutDiagHiggsbb50->Sumw2();
	h_avgMassMatchBjetsHiggs_cutDiagHiggsbb100->Sumw2();
	h_avgMassMatchBjetsHiggs_cutDiagHiggsbb150->Sumw2();
	h_avgMassMatchBjetsHiggs_cutDiagHiggsbb200->Sumw2();
	h_scalarPtMatchBjetsHiggs->Sumw2();
	h_scalarPtMatchBjetsHiggs_cutDiagHiggsbb50->Sumw2();
	h_scalarPtMatchBjetsHiggs_cutDiagHiggsbb100->Sumw2();
	h_scalarPtMatchBjetsHiggs_cutDiagHiggsbb150->Sumw2();
	h_scalarPtMatchBjetsHiggs_cutDiagHiggsbb200->Sumw2();
	h_avgPtMatchBjetsHiggs->Sumw2();
	h_avgPtMatchBjetsHiggs_cutDiagHiggsbb50->Sumw2();
	h_avgPtMatchBjetsHiggs_cutDiagHiggsbb100->Sumw2();
	h_avgPtMatchBjetsHiggs_cutDiagHiggsbb150->Sumw2();
	h_avgPtMatchBjetsHiggs_cutDiagHiggsbb200->Sumw2();
	h_PtMatchBjetsHiggs->Sumw2();

	// RECO Bjets with CSVM
	h_massRecoBjetsCSVM = analysisPlots.make<TH1D>("massRecoBjetsCSVM" , "Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massRecoBjetsCSVM_cutDiagHiggsbb50 = analysisPlots.make<TH1D>("massRecoBjetsCSVM_cutDiagHiggsbb50" , "Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massRecoBjetsCSVM_cutDiagHiggsbb100 = analysisPlots.make<TH1D>("massRecoBjetsCSVM_cutDiagHiggsbb100" , "Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massRecoBjetsCSVM_cutDiagHiggsbb150 = analysisPlots.make<TH1D>("massRecoBjetsCSVM_cutDiagHiggsbb150" , "Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massRecoBjetsCSVM_cutDiagHiggsbb200 = analysisPlots.make<TH1D>("massRecoBjetsCSVM_cutDiagHiggsbb200" , "Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_avgMassRecoBjetsCSVM = analysisPlots.make<TH1D>("avgMassRecoBjetsCSVM" , "Average Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_avgMassRecoBjetsCSVM_cutDiagHiggsbb50 = analysisPlots.make<TH1D>("avgMassRecoBjetsCSVM_cutDiagHiggsbb50" , "Average Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_avgMassRecoBjetsCSVM_cutDiagHiggsbb100 = analysisPlots.make<TH1D>("avgMassRecoBjetsCSVM_cutDiagHiggsbb100" , "Average Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_avgMassRecoBjetsCSVM_cutDiagHiggsbb150 = analysisPlots.make<TH1D>("avgMassRecoBjetsCSVM_cutDiagHiggsbb150" , "Average Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_avgMassRecoBjetsCSVM_cutDiagHiggsbb200 = analysisPlots.make<TH1D>("avgMassRecoBjetsCSVM_cutDiagHiggsbb200" , "Average Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_scalarPtRecoBjetsCSVM = analysisPlots.make<TH2D>("scalarPtRecoBjetsCSVM" , "Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_scalarPtRecoBjetsCSVM_cutDiagHiggsbb50 = analysisPlots.make<TH2D>("scalarPtRecoBjetsCSVM_cutDiagHiggsbb50" , "Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_scalarPtRecoBjetsCSVM_cutDiagHiggsbb100 = analysisPlots.make<TH2D>("scalarPtRecoBjetsCSVM_cutDiagHiggsbb100" , "Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_scalarPtRecoBjetsCSVM_cutDiagHiggsbb150 = analysisPlots.make<TH2D>("scalarPtRecoBjetsCSVM_cutDiagHiggsbb150" , "Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_scalarPtRecoBjetsCSVM_cutDiagHiggsbb200 = analysisPlots.make<TH2D>("scalarPtRecoBjetsCSVM_cutDiagHiggsbb200" , "Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtRecoBjetsCSVM = analysisPlots.make<TH2D>("avgPtRecoBjetsCSVM" , "Average Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtRecoBjetsCSVM_cutDiagHiggsbb50 = analysisPlots.make<TH2D>("avgPtRecoBjetsCSVM_cutDiagHiggsbb50" , "Average Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtRecoBjetsCSVM_cutDiagHiggsbb100 = analysisPlots.make<TH2D>("avgPtRecoBjetsCSVM_cutDiagHiggsbb100" , "Average Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtRecoBjetsCSVM_cutDiagHiggsbb150 = analysisPlots.make<TH2D>("avgPtRecoBjetsCSVM_cutDiagHiggsbb150" , "Average Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtRecoBjetsCSVM_cutDiagHiggsbb200 = analysisPlots.make<TH2D>("avgPtRecoBjetsCSVM_cutDiagHiggsbb200" , "Average Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_PtRecoBjetsCSVM = analysisPlots.make<TH2D>("PtRecoBjetsCSVM" , "Mass of RecoBjetsCSVM vs #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	//
	h_massRecoPartonFlavorBjetsCSVM = analysisPlots.make<TH1D>("massRecoPartonFlavorBjetsCSVM" , "Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 = analysisPlots.make<TH1D>("massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50" , "Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 = analysisPlots.make<TH1D>("massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100" , "Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 = analysisPlots.make<TH1D>("massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150" , "Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 = analysisPlots.make<TH1D>("massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200" , "Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	h_avgMassRecoPartonFlavorBjetsCSVM = analysisPlots.make<TH1D>("avgMassRecoPartonFlavorBjetsCSVM" , "Average Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	h_avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 = analysisPlots.make<TH1D>("avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50" , "Average Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	h_avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 = analysisPlots.make<TH1D>("avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100" , "Average Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	h_avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 = analysisPlots.make<TH1D>("avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150" , "Average Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	h_avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 = analysisPlots.make<TH1D>("avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200" , "Average Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	h_scalarPtRecoPartonFlavorBjetsCSVM = analysisPlots.make<TH2D>("scalarPtRecoPartonFlavorBjetsCSVM" , "Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 = analysisPlots.make<TH2D>("scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50" , "Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 = analysisPlots.make<TH2D>("scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100" , "Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 = analysisPlots.make<TH2D>("scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150" , "Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 = analysisPlots.make<TH2D>("scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200" , "Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtRecoPartonFlavorBjetsCSVM = analysisPlots.make<TH2D>("avgPtRecoPartonFlavorBjetsCSVM" , "Average Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 = analysisPlots.make<TH2D>("avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50" , "Average Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 = analysisPlots.make<TH2D>("avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100" , "Average Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 = analysisPlots.make<TH2D>("avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150" , "Average Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 = analysisPlots.make<TH2D>("avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200" , "Average Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_PtRecoPartonFlavorBjetsCSVM = analysisPlots.make<TH2D>("PtRecoPartonFlavorBjetsCSVM" , "Mass of RecoPartonFlavorBjetsCSVM vs #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_massRecoBjetsCSVM->Sumw2();
	h_massRecoBjetsCSVM_cutDiagHiggsbb50->Sumw2();
	h_massRecoBjetsCSVM_cutDiagHiggsbb100->Sumw2();
	h_massRecoBjetsCSVM_cutDiagHiggsbb150->Sumw2();
	h_massRecoBjetsCSVM_cutDiagHiggsbb200->Sumw2();
	h_avgMassRecoBjetsCSVM->Sumw2();
	h_avgMassRecoBjetsCSVM_cutDiagHiggsbb50->Sumw2();
	h_avgMassRecoBjetsCSVM_cutDiagHiggsbb100->Sumw2();
	h_avgMassRecoBjetsCSVM_cutDiagHiggsbb150->Sumw2();
	h_avgMassRecoBjetsCSVM_cutDiagHiggsbb200->Sumw2();
	h_scalarPtRecoBjetsCSVM->Sumw2();
	h_scalarPtRecoBjetsCSVM_cutDiagHiggsbb50->Sumw2();
	h_scalarPtRecoBjetsCSVM_cutDiagHiggsbb100->Sumw2();
	h_scalarPtRecoBjetsCSVM_cutDiagHiggsbb150->Sumw2();
	h_scalarPtRecoBjetsCSVM_cutDiagHiggsbb200->Sumw2();
	h_avgPtRecoBjetsCSVM->Sumw2();
	h_avgPtRecoBjetsCSVM_cutDiagHiggsbb50->Sumw2();
	h_avgPtRecoBjetsCSVM_cutDiagHiggsbb100->Sumw2();
	h_avgPtRecoBjetsCSVM_cutDiagHiggsbb150->Sumw2();
	h_avgPtRecoBjetsCSVM_cutDiagHiggsbb200->Sumw2();
	h_PtRecoBjetsCSVM->Sumw2();
	//
	h_massRecoPartonFlavorBjetsCSVM->Sumw2();
	h_massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50->Sumw2();
	h_massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100->Sumw2();
	h_massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150->Sumw2();
	h_massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200->Sumw2();
	h_avgMassRecoPartonFlavorBjetsCSVM->Sumw2();
	h_avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50->Sumw2();
	h_avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100->Sumw2();
	h_avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150->Sumw2();
	h_avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200->Sumw2();
	h_scalarPtRecoPartonFlavorBjetsCSVM->Sumw2();
	h_scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50->Sumw2();
	h_scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100->Sumw2();
	h_scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150->Sumw2();
	h_scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200->Sumw2();
	h_avgPtRecoPartonFlavorBjetsCSVM->Sumw2();
	h_avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50->Sumw2();
	h_avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100->Sumw2();
	h_avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150->Sumw2();
	h_avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200->Sumw2();
	h_PtRecoPartonFlavorBjetsCSVM->Sumw2();

	////// STEP 2 - 3
	h_matchdijet_masspt = analysisPlots.make<TH2D>("matchdijet_masspt" , "Mass of dijet vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagStop1jj50 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagStop1jj100 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagStop1jj150 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagStop1jj200 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_matchdijet_masspt->Sumw2();
	h_matchdijetWOMatchBjets_masspt->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagStop1jj50->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagStop1jj100->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagStop1jj150->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagStop1jj200->Sumw2();

	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj50 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj100 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj150 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj200 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj50->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj100->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj150->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb50_cutDiagStop1jj200->Sumw2();

	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj50 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj100 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj150 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj200 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj50->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj100->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj150->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb100_cutDiagStop1jj200->Sumw2();

	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj50 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj100 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj150 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj200 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj50->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj100->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj150->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb150_cutDiagStop1jj200->Sumw2();

	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj50 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj100 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj150 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj200 = analysisPlots.make<TH2D>("matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of Match dijet w/o MatchBjets vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj50->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj100->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj150->Sumw2();
	h_matchdijetWOMatchBjets_masspt_cutDiagHiggsbb200_cutDiagStop1jj200->Sumw2();

	h_massmatchdijetWOMatchBjets = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagStop1jj50" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagStop1jj100" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagStop1jj150" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagStop1jj200" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );

	h_massmatchdijetWOMatchBjets->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagStop1jj50->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagStop1jj100->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagStop1jj150->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagStop1jj200->Sumw2();

	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb50 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb50" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );

	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb50->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj50->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj100->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj150->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb50_cutDiagStop1jj200->Sumw2();

	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb100 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb100" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );

	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb100->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj50->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj100->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj150->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb100_cutDiagStop1jj200->Sumw2();

	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb150 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb150" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );

	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb150->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj50->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj100->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj150->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb150_cutDiagStop1jj200->Sumw2();

	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb200 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb200" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of dijet w/o MatchBjets" , nbinPt, minPt, maxPt );

	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb200->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj50->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj100->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj150->Sumw2();
	h_massmatchdijetWOMatchBjets_cutDiagHiggsbb200_cutDiagStop1jj200->Sumw2();


	//
	h_matchdijetHiggs_masspt = analysisPlots.make<TH2D>("matchdijetHiggs_masspt" , "Mass of dijet vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj50 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj100 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj150 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj200 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj50 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj100 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj150 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj200 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj50 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj100 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj150 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj200 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj50 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj100 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj150 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj200 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj50 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj100 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj150 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj200 = analysisPlots.make<TH2D>("matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of Match dijet w/o MatchBjetsHiggs vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_massmatchdijetWOMatchBjetsHiggs = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj50" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj100" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj150" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj200" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );

	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );

	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );

	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );

	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of dijet w/o MatchBjetsHiggs" , nbinPt, minPt, maxPt );


	h_matchdijetHiggs_masspt->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj50->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj100->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj150->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagStop1jj200->Sumw2();

	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj50->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj100->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj150->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb50_cutDiagStop1jj200->Sumw2();

	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj50->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj100->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj150->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb100_cutDiagStop1jj200->Sumw2();

	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj50->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj100->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj150->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb150_cutDiagStop1jj200->Sumw2();

	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj50->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj100->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj150->Sumw2();
	h_matchdijetWOMatchBjetsHiggs_masspt_cutDiagHiggsbb200_cutDiagStop1jj200->Sumw2();

	h_massmatchdijetWOMatchBjetsHiggs->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj50->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj100->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj150->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagStop1jj200->Sumw2();

	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj50->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj100->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj150->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb50_cutDiagStop1jj200->Sumw2();

	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj50->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj100->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj150->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb100_cutDiagStop1jj200->Sumw2();

	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj50->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj100->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj150->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb150_cutDiagStop1jj200->Sumw2();

	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj50->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj100->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj150->Sumw2();
	h_massmatchdijetWOMatchBjetsHiggs_cutDiagHiggsbb200_cutDiagStop1jj200->Sumw2();


	//
	h_recoDijet_masspt = analysisPlots.make<TH2D>("recoDijet_masspt" , "Mass of dijet vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj50 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj100 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj150 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj200 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200 = analysisPlots.make<TH2D>("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_massdijetWORecoBjetsCSVM = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_WOStop1 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_WOStop1" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagStop1jj50_WOStop1 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagStop1jj50_WOStop1" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb50 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb100 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb150 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb200 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	h_recoDijet_masspt->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj50->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj100->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj150->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj200->Sumw2();

	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200->Sumw2();

	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200->Sumw2();

	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200->Sumw2();

	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150->Sumw2();
	h_dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200->Sumw2();

	h_massdijetWORecoBjetsCSVM->Sumw2();
	h_massdijetWORecoBjetsCSVM_WOStop1->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagStop1jj50->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagStop1jj50_WOStop1->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagStop1jj100->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagStop1jj150->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagStop1jj200->Sumw2();

	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb50->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200->Sumw2();

	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb100->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200->Sumw2();

	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb150->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200->Sumw2();

	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb200->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150->Sumw2();
	h_massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200->Sumw2();

	//
	h_recoPartonFlavorDijet_masspt = analysisPlots.make<TH2D>("recoPartonFlavorDijet_masspt" , "Mass of dijet vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj50 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj100 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj150 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj200 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200 = analysisPlots.make<TH2D>("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	h_massdijetWORecoPartonFlavorBjetsCSVM = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200 = analysisPlots.make<TH1D>("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	h_recoPartonFlavorDijet_masspt->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj50->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj100->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj150->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj200->Sumw2();

	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200->Sumw2();

	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200->Sumw2();

	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200->Sumw2();

	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150->Sumw2();
	h_dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200->Sumw2();

	h_massdijetWORecoPartonFlavorBjetsCSVM->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj50->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj100->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj150->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj200->Sumw2();

	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200->Sumw2();

	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200->Sumw2();

	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200->Sumw2();

	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150->Sumw2();
	h_massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200->Sumw2();


	h_jetsStop1WORecoBjetsCSVM_num = analysisPlots.make<TH1D>("jetsStop1WORecoBjetsCSVM_num" , "Num of Reco dijet from Stop1" , nbinNum, minNum, maxNum ); 

	h_jetsStop1WORecoBjetsCSVM_num->Sumw2();

	h_tmpmassdijetWORecoBjetsCSVM = analysisPlots.make<TH1D>("tmpmassdijetWORecoBjetsCSVM" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	h_tmpmassmatchdijetWOMatchBjets = analysisPlots.make<TH1D>("tmpmassmatchdijetWOMatchBjets" , "Mass of dijet w/o Match Bjets" , nbinPt, minPt, maxPt );

	h_tmpmassdijetWORecoBjetsCSVM->Sumw2();
	h_tmpmassmatchdijetWOMatchBjets->Sumw2();



	///// STEP 4

	// Reco Jets
	h_recoDiBjetDiJet_masspt = analysisPlots.make<TH2D>("recoDiBjetDiJet_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_recoDiBjetDiJet_MassPt = analysisPlots.make<TH2D>("recoDiBjetDiJet_MassPt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massRecoDiBjetDiJet = analysisPlots.make<TH1D>("massRecoDiBjetDiJet" , "Mass of RECO (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_recoDiBjetDiJet_cutDiagStop2bbjj50_masspt = analysisPlots.make<TH2D>("recoDiBjetDiJet_cutDiagStop2bbjj50_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massRecoDiBjetDiJet_cutDiagStop2bbjj50 = analysisPlots.make<TH1D>("massRecoDiBjetDiJet_cutDiagStop2bbjj50" , "Mass of RECO (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_recoDiBjetDiJet_cutDiagStop2bbjj50_MassPt = analysisPlots.make<TH2D>("recoDiBjetDiJet_cutDiagStop2bbjj50_MassPt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massPtRecoDiBjetDiJet_cutDiagStop2bbjj50 = analysisPlots.make<TH1D>("massPtRecoDiBjetDiJet_cutDiagStop2bbjj50" , "Mass of RECO (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_recoDiBjetDiJet_cutDiagStop2bbjj100_masspt = analysisPlots.make<TH2D>("recoDiBjetDiJet_cutDiagStop2bbjj100_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massRecoDiBjetDiJet_cutDiagStop2bbjj100 = analysisPlots.make<TH1D>("massRecoDiBjetDiJet_cutDiagStop2bbjj100" , "Mass of RECO (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_recoDiBjetDiJet_cutDiagStop1jj50_masspt = analysisPlots.make<TH2D>("recoDiBjetDiJet_cutDiagStop1jj50_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massRecoDiBjetDiJet_cutDiagStop1jj50= analysisPlots.make<TH1D>("massRecoDiBjetDiJet_cutDiagStop1jj50" , "Mass of RECO (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_recoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50_masspt = analysisPlots.make<TH2D>("recoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massRecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50 = analysisPlots.make<TH1D>("massRecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50" , "Mass of RECO (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_recoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100_masspt = analysisPlots.make<TH2D>("recoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massRecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100 = analysisPlots.make<TH1D>("massRecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100" , "Mass of RECO (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_recoDiBjetDiJet_cutDiagStop1jj100_masspt = analysisPlots.make<TH2D>("recoDiBjetDiJet_cutDiagStop1jj100_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massRecoDiBjetDiJet_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massRecoDiBjetDiJet_cutDiagStop1jj100" , "Mass of RECO (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_recoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50_masspt = analysisPlots.make<TH2D>("recoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massRecoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50 = analysisPlots.make<TH1D>("massRecoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50" , "Mass of RECO (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_recoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100_masspt = analysisPlots.make<TH2D>("recoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massRecoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100 = analysisPlots.make<TH1D>("massRecoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100" , "Mass of RECO (diBjet + diJet)" , nbinPt, minPt, maxPt );

	h_recoDiBjetDiJet_masspt->Sumw2();
	h_recoDiBjetDiJet_MassPt->Sumw2();
	h_massRecoDiBjetDiJet->Sumw2();
	h_recoDiBjetDiJet_cutDiagStop2bbjj50_masspt->Sumw2();
	h_massRecoDiBjetDiJet_cutDiagStop2bbjj50->Sumw2();
	h_recoDiBjetDiJet_cutDiagStop2bbjj50_MassPt->Sumw2();
	h_massPtRecoDiBjetDiJet_cutDiagStop2bbjj50->Sumw2();
	h_recoDiBjetDiJet_cutDiagStop2bbjj100_masspt->Sumw2();
	h_massRecoDiBjetDiJet_cutDiagStop2bbjj100->Sumw2();
	h_recoDiBjetDiJet_cutDiagStop1jj50_masspt->Sumw2();
	h_massRecoDiBjetDiJet_cutDiagStop1jj50->Sumw2();
	h_recoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50_masspt->Sumw2();
	h_massRecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50->Sumw2();
	h_recoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100_masspt->Sumw2();
	h_massRecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100->Sumw2();
	h_recoDiBjetDiJet_cutDiagStop1jj100_masspt->Sumw2();
	h_massRecoDiBjetDiJet_cutDiagStop1jj100->Sumw2();
	h_recoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50_masspt->Sumw2();
	h_massRecoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50->Sumw2();
	h_recoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100_masspt->Sumw2();
	h_massRecoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100->Sumw2();

	//// Match Jets
	h_matchDiBjetDiJet_masspt = analysisPlots.make<TH2D>("matchDiBjetDiJet_masspt" , "Mass of Match (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massMatchDiBjetDiJet = analysisPlots.make<TH1D>("massMatchDiBjetDiJet" , "Mass of Match (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_matchDiBjetDiJet_cutDiagStop2bbjj50_masspt = analysisPlots.make<TH2D>("matchDiBjetDiJet_cutDiagStop2bbjj50_masspt" , "Mass of Match (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massMatchDiBjetDiJet_cutDiagStop2bbjj50 = analysisPlots.make<TH1D>("massMatchDiBjetDiJet_cutDiagStop2bbjj50" , "Mass of Match (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_matchDiBjetDiJet_cutDiagStop2bbjj100_masspt = analysisPlots.make<TH2D>("matchDiBjetDiJet_cutDiagStop2bbjj100_masspt" , "Mass of Match (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massMatchDiBjetDiJet_cutDiagStop2bbjj100 = analysisPlots.make<TH1D>("massMatchDiBjetDiJet_cutDiagStop2bbjj100" , "Mass of Match (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_matchDiBjetDiJet_cutDiagStop1jj50_masspt = analysisPlots.make<TH2D>("matchDiBjetDiJet_cutDiagStop1jj50_masspt" , "Mass of Match (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massMatchDiBjetDiJet_cutDiagStop1jj50 = analysisPlots.make<TH1D>("massMatchDiBjetDiJet_cutDiagStop1jj50" , "Mass of Match (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_matchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50_masspt = analysisPlots.make<TH2D>("matchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50_masspt" , "Mass of Match (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massMatchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50 = analysisPlots.make<TH1D>("massMatchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50" , "Mass of Match (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_matchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100_masspt = analysisPlots.make<TH2D>("matchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100_masspt" , "Mass of Match (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massMatchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100 = analysisPlots.make<TH1D>("massMatchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100" , "Mass of Match (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_matchDiBjetDiJet_cutDiagStop1jj100_masspt = analysisPlots.make<TH2D>("matchDiBjetDiJet_cutDiagStop1jj100_masspt" , "Mass of Match (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massMatchDiBjetDiJet_cutDiagStop1jj100 = analysisPlots.make<TH1D>("massMatchDiBjetDiJet_cutDiagStop1jj100" , "Mass of Match (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_matchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50_masspt = analysisPlots.make<TH2D>("matchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50_masspt" , "Mass of Match (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massMatchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50 = analysisPlots.make<TH1D>("massMatchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50" , "Mass of Match (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_matchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100_masspt = analysisPlots.make<TH2D>("matchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100_masspt" , "Mass of Match (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massMatchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100 = analysisPlots.make<TH1D>("massMatchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100" , "Mass of Match (diBjet + diJet)" , nbinPt, minPt, maxPt );

	h_matchDiBjetDiJet_masspt->Sumw2();
	h_massMatchDiBjetDiJet->Sumw2();
	h_matchDiBjetDiJet_cutDiagStop2bbjj50_masspt->Sumw2();
	h_massMatchDiBjetDiJet_cutDiagStop2bbjj50->Sumw2();
	h_matchDiBjetDiJet_cutDiagStop2bbjj100_masspt->Sumw2();
	h_massMatchDiBjetDiJet_cutDiagStop2bbjj100->Sumw2();
	h_matchDiBjetDiJet_cutDiagStop1jj50_masspt->Sumw2();
	h_massMatchDiBjetDiJet_cutDiagStop1jj50->Sumw2();
	h_matchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50_masspt->Sumw2();
	h_massMatchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50->Sumw2();
	h_matchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100_masspt->Sumw2();
	h_massMatchDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100->Sumw2();
	h_matchDiBjetDiJet_cutDiagStop1jj100_masspt->Sumw2();
	h_massMatchDiBjetDiJet_cutDiagStop1jj100->Sumw2();
	h_matchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50_masspt->Sumw2();
	h_massMatchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50->Sumw2();
	h_matchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100_masspt->Sumw2();
	h_massMatchDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100->Sumw2();

	//// Match Jets Higgs + Stop1
	h_matchHiggsDiBjetStop1DiJet_masspt = analysisPlots.make<TH2D>("matchHiggsDiBjetStop1DiJet_masspt" , "Mass of Match (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massMatchHiggsDiBjetStop1DiJet = analysisPlots.make<TH1D>("massMatchHiggsDiBjetStop1DiJet" , "Mass of Match (diBjet + diJet)" , nbinPt, minPt, maxPt );
	h_matchHiggsDiBjetStop1DiJet_masspt->Sumw2();
	h_massMatchHiggsDiBjetStop1DiJet->Sumw2();

	///// STEP 5
	h_recoDiBjetDiJetSmallestDeltaM_masspt = analysisPlots.make<TH2D>("recoDiBjetDiJetSmallestDeltaM_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets} SmallestDeltaM" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	h_massRecoDiBjetDiJetSmallestDeltaM = analysisPlots.make<TH1D>("massRecoDiBjetDiJetSmallestDeltaM" , "Mass of RECO (diBjet + diJet) SmallestDeltaM" , nbinPt, minPt, maxPt );
	h_recoDiBjetDiJetSmallestDeltaM_masspt->Sumw2();
	h_massRecoDiBjetDiJetSmallestDeltaM->Sumw2();
}


MyAnalyzer::~MyAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

