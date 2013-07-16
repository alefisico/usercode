// -*- C++ -*-
//
// Package:    MyAnalyzer
// Class:      MyAnalyzer
// 
/**\class MyAnalyzer MyAnalyzer.cc RUAnalysis/MyAnalyzer/src/MyAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Alejandro Gomez Espinosa
//         Created:  Wed Jun 19 15:34:21 EDT 2013
// $Id$
//
//

#include "RUAnalysis/MyAnalyzer/interface/MyAnalyzer.h"

//Own libraries
//
// class declaration
//



//
// member functions
//

step1Vectors step1( vector< TLorentzVector > p4StoreJets ){

	double tmpMassDiff = 9999999;
	Int_t bestMassIndex[4] = {-1, -1, -1, -1};
	vector< TLorentzVector > p4bbSmallMassDiff;
	vector< double > massbb;
	vector< double > ptbb;
	vector< double > scalarPtbb;

	if ( p4StoreJets.size() > 3 ){
	//if ( p4StoreJets.size() == 4 ){
		////// Check index of the bs with the smallest mass diff
		for(unsigned int ii = 0; ii < p4StoreJets.size(); ++ii) {
			for(unsigned int ij = 0; ij < p4StoreJets.size(); ++ij) {
				for(unsigned int ik = 0; ik < p4StoreJets.size(); ++ik) {
					for(unsigned int il = 0; il < p4StoreJets.size(); ++il) {
						if ( ii==ij || ii==ik || ii==il ) continue;
						if ( ij==ik || ij==il ) continue;
						if ( ik==il ) continue;
						//cout << " FUNCTION " << endl;
						//cout << ii << " " << ij << " " << ik << " " << il << endl;
						TLorentzVector candMassbb1 = p4StoreJets[ii]+p4StoreJets[ij];
						TLorentzVector candMassbb2 = p4StoreJets[ik]+p4StoreJets[il];
						double massDiff = abs( candMassbb1.M() - candMassbb2.M() );
						if( tmpMassDiff > massDiff ){
							tmpMassDiff = massDiff;
							bestMassIndex[0] = ii;
							bestMassIndex[1] = ij;
							bestMassIndex[2] = ik;
							bestMassIndex[3] = il;
							//cout << ii << " " << ij << " " << ik << " " << il << " " << massDiff << " " << candMassbb1.M() << " " << candMassbb2.M() << endl;
						}
					}
				}
			}
		}
		
		p4bbSmallMassDiff.push_back( p4StoreJets[bestMassIndex[0]] );
		p4bbSmallMassDiff.push_back( p4StoreJets[bestMassIndex[1]] );
		p4bbSmallMassDiff.push_back( p4StoreJets[bestMassIndex[2]] );
		p4bbSmallMassDiff.push_back( p4StoreJets[bestMassIndex[3]] );
		TLorentzVector tmpMass1 = p4StoreJets[bestMassIndex[0]] + p4StoreJets[bestMassIndex[1]];
		TLorentzVector tmpMass2 = p4StoreJets[bestMassIndex[2]] + p4StoreJets[bestMassIndex[3]];
		double scalarPtbb1 = p4StoreJets[bestMassIndex[0]].Pt() + p4StoreJets[bestMassIndex[1]].Pt();
		double scalarPtbb2 = p4StoreJets[bestMassIndex[2]].Pt() + p4StoreJets[bestMassIndex[3]].Pt();

		////// Store vectors
		massbb.push_back ( tmpMass1.M() );
		massbb.push_back ( tmpMass2.M() );
		ptbb.push_back ( tmpMass1.Pt() );
		ptbb.push_back ( tmpMass2.Pt() );
		scalarPtbb.push_back ( scalarPtbb1 );
		scalarPtbb.push_back ( scalarPtbb2 );
	}
	step1Vectors tmpVectors;
	tmpVectors.p4SmallMassDiff = p4bbSmallMassDiff;
	tmpVectors.mass = massbb;
	tmpVectors.pt = ptbb;
	tmpVectors.scalarPt = scalarPtbb;


	return tmpVectors;
}

// ------------ method called for each event  ------------
void
MyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

	//////////////////////////
	/// Definitions
	/////////////////////////
	using namespace edm;
	using namespace reco;

	/// Call GenParticles
	edm::Handle<std::vector<reco::GenParticle>> particles;
	iEvent.getByLabel( Gensrc , particles );
	const std::vector<reco::GenParticle> & p = *particles; 

	// Call goodPatJetsPFlow
	edm::Handle< std::vector<pat::Jet> > PatJets;
	iEvent.getByLabel(_patJetType, PatJets); 

	///////////////// Jet Corrections
	JetCorrectionUncertainty *jecUnc = 0;
	//if (! _isData) {
		edm::ESHandle < JetCorrectorParametersCollection > JetCorParColl;
		// get the jet corrector parameters collection from the global tag
		iSetup.get<JetCorrectionsRecord>().get(std::string("AK5PFchs"), JetCorParColl);
		// get the uncertainty parameters from the collection
		JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
		// instantiate the jec uncertainty object
		jecUnc = new JetCorrectionUncertainty(JetCorPar);
	//}
	///////////////////////////////////////////////////////////////
			

	/////////// Take info for GenParticles 
	std::auto_ptr<reco::GenParticleCollection> parents(new reco::GenParticleCollection());
	const JetCorrector* corrector = JetCorrector::getJetCorrector(_jetCorrectionService, iSetup);   //Get the jet corrector from the event setup
	////////////////////////////////////////////////////////////////
	

	///////// List of struct jetElem
	std::list<jetElem> adjJetList;
	
	/////////// define some TLorentzVectors
	vector< TLorentzVector > p4GenTruthAlljets;
	vector< TLorentzVector > p4GenTruthJets;
	vector< TLorentzVector > p4GenTruthBjets; 
	vector< TLorentzVector > p4GenTruthHiggs;
	vector< TLorentzVector > p4GenTruthBjetsHiggs; 
	vector< TLorentzVector > p4GenTruthB1Higgs; 
	vector< TLorentzVector > p4GenTruthB2Higgs; 
	vector< TLorentzVector > p4GenTruthJetsStop1;
	vector< TLorentzVector > p4GenTruthBjetsStop1; 

	vector< TLorentzVector > p4RecoJets;
	vector< TLorentzVector > p4RecoBjets;
	vector< TLorentzVector > p4RecoBtagJets_CSVM;
	vector< TLorentzVector > p4RecoPartonFlavorBtagJets_CSVM;
	vector< double > RecoBjets_CSVM;

	vector< TLorentzVector > p4MatchB1Higgs;
	vector< TLorentzVector > p4MatchB2Higgs;
	vector< TLorentzVector > p4MatchAllBjetsStop1;
	vector< TLorentzVector > p4MatchAllJetsStop1;
	vector< TLorentzVector > p4MatchTruthAlljets;
	vector< TLorentzVector > p4MatchTruthJets;
	vector< TLorentzVector > p4MatchTruthBjets;
	//////////////////////////////////////////////////


	//////////// GET EVT INFO
	run   = iEvent.id().run();
	event = iEvent.id().event();
	lumis = iEvent.id().luminosityBlock();
	//cout << run << " " << event << endl;
	//////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////
	//  GenParticles
	///////////////////////////////////////////////////////////////////////////////////////////////
	for(unsigned int i = 0; i < particles->size(); ++ i) {

		if( p[i].status()!= 3 ) continue;				// Make sure only "final" particles are present
		const Candidate * mom = p[i].mother();				// call mother particle

		//////////// Storing TLorentz Vectors
		TLorentzVector tmpGenTruthAlljets, tmpGenTruthBjets, tmpGenTruthJets, tmpGenTruthHiggs, tmpGenTruthBjetsHiggs, tmpGenTruthB2Higgs,tmpGenTruthB1Higgs, tmpGenTruthBjetsStop1, tmpGenTruthJetsStop1;
		if( mom ){
			if ( p[i].pt() > 20.0 && fabs( p[i].eta() ) < 3.0){
				// AllJets 
				if( ( abs( p[i].pdgId() ) == 1 ) || ( abs( p[i].pdgId() ) == 2 ) || ( abs( p[i].pdgId() ) == 3 ) || ( abs( p[i].pdgId() ) == 4) || ( abs( p[i].pdgId() ) == 5 ) || ( abs( p[i].pdgId() ) == 21 )){
					tmpGenTruthAlljets.SetPtEtaPhiE( p[i].pt(), p[i].eta(), p[i].phi(), p[i].energy() );
					p4GenTruthAlljets.push_back( tmpGenTruthAlljets );
				}
				// Jets w/o b
				if( ( abs( p[i].pdgId() ) == 1 ) || ( abs( p[i].pdgId() ) == 2 ) || ( abs( p[i].pdgId() ) == 3 ) || ( abs( p[i].pdgId() ) == 4) || ( abs( p[i].pdgId() ) == 21 )){
					tmpGenTruthJets.SetPtEtaPhiE( p[i].pt(), p[i].eta(), p[i].phi(), p[i].energy() );
					p4GenTruthJets.push_back( tmpGenTruthJets );
				} 
				// Bjets
				if( abs( p[i].pdgId() ) == 5 ){
					tmpGenTruthBjets.SetPtEtaPhiE( p[i].pt(), p[i].eta(), p[i].phi(), p[i].energy() );
					p4GenTruthBjets.push_back( tmpGenTruthBjets );
				}
				////// Store components of Higgs
				if( abs( p[i].pdgId() ) == 25 && abs( p[i].mother()->pdgId() ) == 1000004 ){
					tmpGenTruthHiggs.SetPtEtaPhiE( p[i].pt(), p[i].eta(), p[i].phi(), p[i].energy() );
					p4GenTruthHiggs.push_back( tmpGenTruthHiggs );
				}
				if ( abs( p[i].mother()->pdgId() ) == 25 ){

					if ( abs( p[i].pdgId() ) != 5 ) continue;
					tmpGenTruthBjetsHiggs.SetPtEtaPhiE( p[i].pt(), p[i].eta(), p[i].phi(), p[i].energy() );
					p4GenTruthBjetsHiggs.push_back( tmpGenTruthBjetsHiggs );

					if ( p4GenTruthHiggs.size() == 2 ){
						if ( abs( p4GenTruthHiggs[0].P() - p[i].mother()->p() ) < 0.0001 ){
							tmpGenTruthB1Higgs.SetPtEtaPhiE( p[i].pt(), p[i].eta(), p[i].phi(), p[i].energy() );
							p4GenTruthB1Higgs.push_back( tmpGenTruthB1Higgs );
						}
						if ( abs( p4GenTruthHiggs[1].P() - p[i].mother()->p() ) < 0.0001 ){
							tmpGenTruthB2Higgs.SetPtEtaPhiE( p[i].pt(), p[i].eta(), p[i].phi(), p[i].energy() );
							p4GenTruthB2Higgs.push_back( tmpGenTruthB2Higgs );
						}
					}
				} 


				////// Store components of Stop1
				if ( abs( p[i].mother()->pdgId() ) == 1000002 ){
					if ( abs( p[i].pdgId() ) == 5 ){
					       tmpGenTruthBjetsStop1.SetPtEtaPhiE( p[i].pt(), p[i].eta(), p[i].phi(), p[i].energy()  );
					       p4GenTruthBjetsStop1.push_back( tmpGenTruthBjetsStop1 );
					}
					if ( abs(p[i].pdgId() ) == 1 || abs(p[i].pdgId() ) == 2 || abs(p[i].pdgId() ) == 3 || abs(p[i].pdgId() ) == 4 ){
					       tmpGenTruthJetsStop1.SetPtEtaPhiE( p[i].pt(), p[i].eta(), p[i].phi(), p[i].energy()  );
					       p4GenTruthJetsStop1.push_back( tmpGenTruthJetsStop1 );
					}
				} 
			}

		}
		std::sort(p4GenTruthAlljets.begin(), p4GenTruthAlljets.end(), ComparePt);
		std::sort(p4GenTruthBjets.begin(), p4GenTruthBjets.end(), ComparePt);
		std::sort(p4GenTruthJets.begin(), p4GenTruthJets.end(), ComparePt);
		std::sort(p4GenTruthBjetsHiggs.begin(), p4GenTruthBjetsHiggs.end(), ComparePt);
		std::sort(p4GenTruthJetsStop1.begin(), p4GenTruthJetsStop1.end(), ComparePt);
		std::sort(p4GenTruthBjetsStop1.begin(), p4GenTruthBjetsStop1.end(), ComparePt);
	}
	//cout << p4GenTruthB1Higgs.size() << " "  << p4GenTruthB2Higgs.size() << endl;
	//cout << p4GenTruthAlljets[0].Pt() << " "  << p4GenTruthBjets[0].Pt() << " "  << p4GenTruthJets[0].Pt() << endl;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	//  RECO
	/////////////////////////////////////////////////////////////////////
	

	///////////////////////////
	//  Jet Corrections    ////
	///////////////////////////
	for (std::vector<pat::Jet>::const_iterator rawJet = PatJets->begin(); rawJet != PatJets->end(); ++rawJet) {       

		double jec = corrector->correction(rawJet->correctedJet("Uncorrected"), iEvent, iSetup); 
		pat::Jet correctedJet = rawJet->correctedJet("Uncorrected");  //copy original jet

		if (jec > 0.0) correctedJet.scaleEnergy(jec);                        // apply the correction
		else cout << "Bad jec " << jec << endl;

		if (jec < 0.1 && rawJet->pt() > 30 && fabs(correctedJet.eta()) < 2.5) {
			cout << "Warning: Invalid(?) JEC " << jec << " uncorrected pt ";
			cout << rawJet->correctedJet("Uncorrected").pt() << " corrected pt " << correctedJet.pt();
			cout << " eta " << correctedJet.eta() << endl;
		}

		bool goodJecUnc = false;

		//if (! _isData) {   // dont worry yet about data

		// Apply sanity check to avoid exception for bad values
		if (fabs(rawJet->eta()) < 5.2 && correctedJet.pt() > 0.0 && correctedJet.pt() < 20000.0) {
			jecUnc->setJetEta(rawJet->eta());
			jecUnc->setJetPt(correctedJet.pt()); // the uncertainty is a function of the corrected pt
			goodJecUnc = true;
		} else cout << "Bad jet with out-of-range eta/pt. Can't get JEC unc. Eta " << rawJet->eta() << " pt " << correctedJet.pt() << endl;

		// Storing jets in struct list 
		jetElem tmpjet;
		tmpjet.origJet = &(*rawJet);
		tmpjet.adjJet = correctedJet.p4();
		double corrFactor = 1.0;
		tmpjet.jecUnc = 0;

		//if (! _isData && goodJecUnc) tmpjet.jecUnc = jecUnc->getUncertainty(true);
		if (goodJecUnc) tmpjet.jecUnc = jecUnc->getUncertainty(true);
		/// NOT yet JER 
		/*if (! _isData && (_jecAdj.compare("jerup") == 0 || _jecAdj.compare("jerdown") == 0)) {
			bool jerdown = (_jecAdj.compare("jerdown") == 0);
			corrFactor = getJERAdj(correctedJet.pt(), *Jet, jerdown);
		} 
		else if (_jecAdj.compare("up") == 0) corrFactor += tmpjet.jecUnc;
		else if (_jecAdj.compare("down") == 0) corrFactor -= tmpjet.jecUnc;*/

		if (corrFactor != 1.0 && corrFactor > 0 && corrFactor < 5.0) tmpjet.adjJet *= corrFactor; // Apply factor only for reasonable values
		tmpjet.diffVec = correctedJet.p4() - tmpjet.adjJet;

		// After corrections, store jets
		adjJetList.push_back(tmpjet); 			// add to list
	}
	adjJetList.sort(cmpJets); 			// sort list of jets
	////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////
	// Loop over corrected jets      ///
	////////////////////////////////////
	
	double bdiscCSV_PF = -999;
	for (std::list<jetElem>::const_iterator correctedJet = adjJetList.begin(); correctedJet != adjJetList.end(); ++correctedJet) {
		
		//const reco::Candidate::LorentzVector *adjJet = &(correctedJet->adjJet); 	// Only TLorentzVector
		const pat::Jet *Jet = correctedJet->origJet;					// pat::Jet info (whole info)
		TLorentzVector p4Jets;
	       	p4Jets.SetPtEtaPhiE( Jet->pt(), Jet->eta(), Jet->phi(), Jet->energy() );

		///////////////////// Preeliminary Selection
		if ( Jet->pt() < 20.0 || fabs( Jet->eta() ) > 3.0) continue;
		//if ( Jet->pt() > 20.0 && fabs( Jet->eta() ) < 3.0)
		//cout << Jet->pt() << " " << Jet->partonFlavour() << " " << Jet->bDiscriminator("combinedSecondaryVertexBJetTags") << endl;
		//////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////
		/////////// Store TLorentzVecots RECO jets and bjets
		//////////////////////////////////////////////////

		p4RecoJets.push_back( p4Jets );
		
		//////////////////// Matching with bjets from Higgs
		if( p4GenTruthB1Higgs.size() == 2 ) {
			double deltaRB1Higgs = 999999;
			for(unsigned int i = 0; i < p4GenTruthB1Higgs.size(); ++i) {
				double tmpdeltaRB1Higgs = p4GenTruthB1Higgs[i].DeltaR( p4Jets );
				if ( tmpdeltaRB1Higgs < deltaRB1Higgs ) deltaRB1Higgs = tmpdeltaRB1Higgs;
			}
			h_MatchB1Higgs_deltaR->Fill( deltaRB1Higgs );
			if ( deltaRB1Higgs < 0.4 ) p4MatchB1Higgs.push_back( p4Jets );   
		}
		if( p4GenTruthB2Higgs.size() == 2 ) {
			double deltaRB2Higgs = 999999;
			for(unsigned int i = 0; i < p4GenTruthB2Higgs.size(); ++i) {
				double tmpdeltaRB2Higgs = p4GenTruthB2Higgs[i].DeltaR( p4Jets );
				if ( tmpdeltaRB2Higgs < deltaRB2Higgs ) deltaRB2Higgs = tmpdeltaRB2Higgs;
			}
			h_MatchB2Higgs_deltaR->Fill( deltaRB2Higgs );
			if ( deltaRB2Higgs < 0.4 ) p4MatchB2Higgs.push_back( p4Jets );   
			/////////////////// TEST
			//if ( event == 45037 ){
			//	cout << deltaRB2Higgs << " " << Jet->partonFlavour() << endl;
			//}
			///////////////////////////////////////////

		}
		/////////////////////////////////////////////////////////////////

		//////////////////// Matching with genParticles
		// All Jets 
		double deltaRTruthAlljets = 99999;
		for(unsigned int i = 0; i < p4GenTruthAlljets.size(); ++i) {
			double tmpdeltaRTruthAlljets = p4GenTruthAlljets[i].DeltaR( p4Jets );
			if ( tmpdeltaRTruthAlljets < deltaRTruthAlljets ) deltaRTruthAlljets = tmpdeltaRTruthAlljets;
		}
		h_MatchTruthAlljets_deltaR->Fill( deltaRTruthAlljets );
		if ( deltaRTruthAlljets < 0.4 ) p4MatchTruthAlljets.push_back( p4Jets );   

		// Jets without bs
		double deltaRTruthjets = 99999;
		for(unsigned int i = 0; i < p4GenTruthJets.size(); ++i) {
			double tmpdeltaRTruthjets = p4GenTruthJets[i].DeltaR( p4Jets );
			if ( tmpdeltaRTruthjets < deltaRTruthjets ) deltaRTruthjets = tmpdeltaRTruthjets;
		}	
		h_MatchTruthJets_deltaR->Fill( deltaRTruthjets );
		if ( deltaRTruthjets < 0.4 ) p4MatchTruthJets.push_back( p4Jets );   

		// Bjets
		double deltaRTruthBjets = 999999;
		for(unsigned int i = 0; i < p4GenTruthBjets.size(); ++i) {
			double tmpdeltaRTruthBjets = p4GenTruthBjets[i].DeltaR( p4Jets );
			if ( tmpdeltaRTruthBjets < deltaRTruthBjets ) deltaRTruthBjets = tmpdeltaRTruthBjets;
		}
		h_MatchTruthBjets_deltaR->Fill( deltaRTruthBjets );
		if ( deltaRTruthBjets < 0.4 ) p4MatchTruthBjets.push_back( p4Jets );   
		/////////////////////////////////////////////////////////////////
	
	//if ( p4MatchB2Higgs.size() > 2 || p4MatchB1Higgs.size() > 2) {
	//	cout << p4MatchB1Higgs.size() << " " << p4MatchB2Higgs.size() << endl;
	//}
		/////// Btagging
		if ( bdiscCSV_PF > 0.679  ) {
			p4RecoBtagJets_CSVM.push_back( p4Jets );
		}
		if ( abs( Jet->partonFlavour() ) != 5 ) continue;
		p4RecoBjets.push_back( p4Jets );
		bdiscCSV_PF = Jet->bDiscriminator("combinedSecondaryVertexBJetTags");
		if ( bdiscCSV_PF < 0.679  ) continue;
		p4RecoPartonFlavorBtagJets_CSVM.push_back( p4Jets );
		RecoBjets_CSVM.push_back( bdiscCSV_PF );
	}
	std::sort(p4MatchTruthAlljets.begin(), p4MatchTruthAlljets.end(), ComparePt);
	std::sort(p4MatchTruthJets.begin(), p4MatchTruthJets.end(), ComparePt);
	std::sort(p4MatchTruthBjets.begin(), p4MatchTruthBjets.end(), ComparePt);
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	
	// Plot Reconstructed Mass/particles plots
	BasicPlots( p4GenTruthAlljets, p4GenTruthJets, p4GenTruthBjets, p4GenTruthBjetsHiggs, p4GenTruthB1Higgs, p4GenTruthB2Higgs, p4GenTruthJetsStop1, p4GenTruthBjetsStop1, p4MatchTruthJets,  p4MatchTruthBjets, p4MatchB1Higgs, p4MatchB2Higgs, p4RecoJets, p4RecoBjets );
	Analysis( p4GenTruthB1Higgs, p4GenTruthB2Higgs, p4RecoJets, p4RecoBjets, p4RecoBtagJets_CSVM, p4RecoPartonFlavorBtagJets_CSVM, p4MatchTruthBjets, p4MatchB1Higgs,  p4MatchB2Higgs, p4MatchTruthAlljets, RecoBjets_CSVM);

}


// ------------ method called once each job just before starting event loop  ------------
void 
MyAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MyAnalyzer::endJob() 
{
}
///////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////           General Basic Plots                          ///////
//////////////////////////////////////////////////////////////////////
void MyAnalyzer::BasicPlots( vector< TLorentzVector > p4GenTruthAlljets, vector< TLorentzVector > p4GenTruthJets, vector< TLorentzVector > p4GenTruthBjets, vector< TLorentzVector > p4GenTruthBjetsHiggs, vector< TLorentzVector > p4GenTruthB1Higgs, vector< TLorentzVector > p4GenTruthB2Higgs, vector< TLorentzVector > p4GenTruthJetsStop1, vector< TLorentzVector > p4GenTruthBjetsStop1, vector< TLorentzVector > p4MatchTruthJets,  vector< TLorentzVector > p4MatchTruthBjets, vector< TLorentzVector > p4MatchB1Higgs, vector< TLorentzVector > p4MatchB2Higgs, vector< TLorentzVector > p4RecoJets, vector< TLorentzVector > p4RecoBjets ) {

	///////////////////////////////////////////////////////
	//   Basic Plots RECO Level                          //
	///////////////////////////////////////////////////////
	
	//////// Basic plots for All jets
	double sumRecoJetsPt = 0;
	for(unsigned int j = 0; j < p4RecoJets.size(); ++j) {
		sumRecoJetsPt += p4RecoJets[j].Pt();
		h_recoJets_pt->Fill( p4RecoJets[j].Pt() );	
		h_recoJets_eta->Fill( p4RecoJets[j].Eta() );
		h_recoJets_phi->Fill(p4RecoJets[j].Phi());
	} 
	h_recoJets_sumpt->Fill( sumRecoJetsPt );	
	h_recoJets_num->Fill( p4RecoJets.size() );	

	//////// Basic plots for bs 
	double sumRecoBjetsPt = 0;
	if ( p4RecoBjets.size() > 0 ){
		for(unsigned int k = 0; k < p4RecoBjets.size(); ++k) {
			sumRecoBjetsPt += p4RecoBjets[k].Pt();
			h_recoBJets_pt->Fill( p4RecoBjets[k].Pt() );	
			h_recoBJets_eta->Fill( p4RecoBjets[k].Eta() );
			h_recoBJets_phi->Fill( p4RecoBjets[k].Phi() );
		}
	}
	h_recoBJets_num->Fill( p4RecoBjets.size() );	
	h_recoBJets_sumpt->Fill( sumRecoBjetsPt );	
	//////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	//   Basic Plots RECO Level - MATCH!!!               //
	///////////////////////////////////////////////////////
	
	//////// Basic plots for jets without bs
	double sumMatchTruthJetsPt = 0;
	for(unsigned int j = 0; j < p4MatchTruthJets.size(); ++j) {
		sumMatchTruthJetsPt += p4MatchTruthJets[j].Pt();
		h_matchJets_pt->Fill( p4MatchTruthJets[j].Pt() );	
		h_matchJets_eta->Fill( p4MatchTruthJets[j].Eta() );
		h_matchJets_phi->Fill(p4MatchTruthJets[j].Phi());
	} 
	h_matchJets_num->Fill( p4MatchTruthJets.size() );	
	h_matchJets_sumpt->Fill( sumMatchTruthJetsPt );	
	//////////////////////////////////////////////////////////

	// bjets
	double sumMatchTruthBjetsPt = 0;
	if ( p4MatchTruthBjets.size() > 0 ){
		for(unsigned int k = 0; k < p4MatchTruthBjets.size(); ++k) {
			sumMatchTruthBjetsPt += p4MatchTruthBjets[k].Pt();
			h_matchBJets_pt->Fill( p4MatchTruthBjets[k].Pt() );	
			h_matchBJets_eta->Fill( p4MatchTruthBjets[k].Eta() );
			h_matchBJets_phi->Fill( p4MatchTruthBjets[k].Phi() );
		}
	}	
	h_matchBJets_num->Fill( p4MatchTruthBjets.size() );	
	h_matchBJets_sumpt->Fill( sumMatchTruthBjetsPt );	

	// Match All bjets from Higgs
	// HERE I am plotting ALL the Higgs in the sample, e.g., there are events with only one Higgs.
	double sumMatchB1HiggsPt = 0;
	if ( p4MatchB1Higgs.size() > 1 ){
		TLorentzVector MatchHiggs1Mass = p4MatchB1Higgs[0] + p4MatchB1Higgs[1];
		h_matchBJetsHiggs_mass->Fill( MatchHiggs1Mass.M() );	
		for(unsigned int k = 0; k < p4MatchB1Higgs.size(); ++k) {
			sumMatchB1HiggsPt += p4MatchB1Higgs[k].Pt();
			h_matchBJetsHiggs_pt->Fill( p4MatchB1Higgs[k].Pt() );	
			h_matchBJetsHiggs_eta->Fill( p4MatchB1Higgs[k].Eta() );
			h_matchBJetsHiggs_phi->Fill( p4MatchB1Higgs[k].Phi() );
		}
	}	
	h_matchBJetsHiggs_num->Fill( p4MatchB1Higgs.size() );	
	h_matchBJetsHiggs1_num->Fill( p4MatchB1Higgs.size() );	
	h_matchBJetsHiggs_sumpt->Fill( sumMatchB1HiggsPt );	
	double sumMatchB2HiggsPt = 0;
	if ( p4MatchB2Higgs.size() > 1 ){
		TLorentzVector MatchHiggs2Mass = p4MatchB2Higgs[0] + p4MatchB2Higgs[1];
		h_matchBJetsHiggs_mass->Fill( MatchHiggs2Mass.M() );	
		for(unsigned int k = 0; k < p4MatchB2Higgs.size(); ++k) {
			sumMatchB2HiggsPt += p4MatchB2Higgs[k].Pt();
			h_matchBJetsHiggs_pt->Fill( p4MatchB2Higgs[k].Pt() );	
			h_matchBJetsHiggs_eta->Fill( p4MatchB2Higgs[k].Eta() );
			h_matchBJetsHiggs_phi->Fill( p4MatchB2Higgs[k].Phi() );
		}
	}	
	h_matchBJetsHiggs_num->Fill( p4MatchB2Higgs.size() );	
	h_matchBJetsHiggs2_num->Fill( p4MatchB2Higgs.size() );	
	h_matchBJetsHiggs_sumpt->Fill( sumMatchB2HiggsPt );	
	/////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	//   Basic Plots Gen Level                           //
	///////////////////////////////////////////////////////
	
	//////// Basic plots for All jets
	double sumGenTruthAlljetsPt = 0;
	for(unsigned int j = 0; j < p4GenTruthAlljets.size(); ++j) {
		sumGenTruthAlljetsPt += p4GenTruthAlljets[j].Pt();
		h_genTruthAlljets_pt->Fill( p4GenTruthAlljets[j].Pt() );	
		h_genTruthAlljets_eta->Fill( p4GenTruthAlljets[j].Eta() );
		h_genTruthAlljets_phi->Fill(p4GenTruthAlljets[j].Phi());
	} 
	h_genTruthAlljets_sumpt->Fill( sumGenTruthAlljetsPt );	
	h_genTruthAlljets_num->Fill( p4GenTruthAlljets.size() );	

	//////// Basic plots for jets without bs
	double sumGenTruthJetsPt = 0;
	for(unsigned int j = 0; j < p4GenTruthJets.size(); ++j) {
		sumGenTruthJetsPt += p4GenTruthJets[j].Pt();
		h_genTruthJets_pt->Fill( p4GenTruthJets[j].Pt() );	
		h_genTruthJets_eta->Fill( p4GenTruthJets[j].Eta() );
		h_genTruthJets_phi->Fill(p4GenTruthJets[j].Phi());
	} 
	h_genTruthJets_sumpt->Fill( sumGenTruthJetsPt );	
	h_genTruthJets_num->Fill( p4GenTruthJets.size() );	

	//////// Basic plots for bs 
	double sumGenTruthBjetsPt = 0;
	if ( p4GenTruthBjets.size() > 0 ){
		for(unsigned int k = 0; k < p4GenTruthBjets.size(); ++k) {
			sumGenTruthBjetsPt += p4GenTruthBjets[k].Pt();
			h_genTruthBJets_pt->Fill( p4GenTruthBjets[k].Pt() );	
			h_genTruthBJets_eta->Fill( p4GenTruthBjets[k].Eta() );
			h_genTruthBJets_phi->Fill( p4GenTruthBjets[k].Phi() );
		}
	}
	h_genTruthBJets_num->Fill( p4GenTruthBjets.size() );	
	h_genTruthBJets_sumpt->Fill( sumGenTruthBjetsPt );	

	//////// Basic plots for bs coming from higgs
	if ( p4GenTruthBjetsHiggs.size() > 0 ){
		for(unsigned int k = 0; k < p4GenTruthBjetsHiggs.size(); ++k) {
			h_bjetsHiggs_pt->Fill( p4GenTruthBjetsHiggs[k].Pt() );	
			h_bjetsHiggs_eta->Fill( p4GenTruthBjetsHiggs[k].Eta() );
			h_bjetsHiggs_phi->Fill( p4GenTruthBjetsHiggs[k].Phi() );
		}
	}	

	//////// Higgs Truth Mass
	if ( p4GenTruthB1Higgs.size() > 1 ){
		TLorentzVector GenTruthHiggsMass = p4GenTruthB1Higgs[0] + p4GenTruthB1Higgs[1];
		h_genTruthBJetsHiggs_mass->Fill( GenTruthHiggsMass.M() );	
	}	
	h_genTruthBJetsHiggs_num->Fill( p4GenTruthB1Higgs.size() );	
	if ( p4GenTruthB2Higgs.size() > 1 ){
		TLorentzVector GenTruthHiggsMass = p4GenTruthB2Higgs[0] + p4GenTruthB2Higgs[1];
		h_genTruthBJetsHiggs_mass->Fill( GenTruthHiggsMass.M() );	
	}	
	h_genTruthBJetsHiggs_num->Fill( p4GenTruthB2Higgs.size() );	

	/////// Higgs reconstructed mass
	vector< TLorentzVector > p4GenTruthHiggs; 
	if ( p4GenTruthBjetsHiggs.size() > 1 ){
		for(unsigned int j = 0; j < p4GenTruthBjetsHiggs.size()-1; ++j) {
			for(unsigned int k=j+1; k < p4GenTruthBjetsHiggs.size(); ++k) {
				if(j==k) continue;							// avoid repetition
				TLorentzVector p4CandidateHiggs = p4GenTruthBjetsHiggs[j] + p4GenTruthBjetsHiggs[k];		// Higgs TLorentzVector
				Double_t massHiggs = 125.0;						// nominal mass value
				Double_t diffmassHiggs = p4CandidateHiggs.M() - massHiggs;		// mass difference
				if( abs(diffmassHiggs) < 0.5 ){						// diffmass presicion
					p4GenTruthHiggs.push_back( p4CandidateHiggs );				// store final TLorentzVector
				}
			}
		}
	}

	if ( p4GenTruthHiggs.size() > 0 ){
		// Plots for hardest Higgs
		h_higgs1_mass->Fill( p4GenTruthHiggs[0].M() );
		h_higgs1_pt->Fill( p4GenTruthHiggs[0].Pt() );	
		h_higgs1_eta->Fill( p4GenTruthHiggs[0].Eta() );	
		// Plots for softer Higgs
		if ( p4GenTruthHiggs.size() > 1 ){
		       	h_higgs2_mass->Fill( p4GenTruthHiggs[1].M() );
		       	h_higgs2_pt->Fill( p4GenTruthHiggs[1].Pt() );
		       	h_higgs2_eta->Fill( p4GenTruthHiggs[1].Eta() );
		       	h_higgs_deltaR->Fill( p4GenTruthHiggs[0].DeltaR( p4GenTruthHiggs[1] ) );
		       	h_higgs_deltaPhi->Fill( p4GenTruthHiggs[0].DeltaPhi( p4GenTruthHiggs[1] ) );
		       	h_higgs_cosDeltaPhi->Fill( cos (p4GenTruthHiggs[0].DeltaPhi( p4GenTruthHiggs[1] ) ) );
		}
	}
	///////////////////////////// /


	////// bjets from Stop1 histos
	if ( p4GenTruthBjetsStop1.size() > 0 ){
		for(unsigned int k = 0; k < p4GenTruthBjetsStop1.size(); ++k) {
			h_bjetsStop1_pt->Fill( p4GenTruthBjetsStop1[k].Pt() );	
			h_bjetsStop1_eta->Fill( p4GenTruthBjetsStop1[k].Eta() );
			h_bjetsStop1_phi->Fill( p4GenTruthBjetsStop1[k].Phi() );
		}
	}	

	/////// Stop1 reconstructed mass
	vector< TLorentzVector > p4GenTruthStop1; 
	if( st1decay.compare("bj") == 0){
		if( p4GenTruthBjetsStop1.size() > 0 && p4GenTruthJetsStop1.size() > 0 ){
			for(unsigned int j = 0; j < p4GenTruthBjetsStop1.size(); ++j) {
				for(unsigned int k= 0; k < p4GenTruthJetsStop1.size(); ++k) {
					TLorentzVector p4CandidateStop1 = p4GenTruthBjetsStop1[j] + p4GenTruthJetsStop1[k];		// higgs tlorentzvector
					double diffmassStop1 = p4CandidateStop1.M() - stop1Mass;		// mass difference
					if( abs(diffmassStop1) < 1.0 ){						// diffmass presicion
						p4GenTruthStop1.push_back( p4CandidateStop1 );
					}
				}
			}
		}
	} else {
		if( p4GenTruthJetsStop1.size() > 0 ){
			for(unsigned int j = 0; j < p4GenTruthJetsStop1.size(); ++j) {
				for(unsigned int k= 0; k < p4GenTruthJetsStop1.size(); ++k) {
					if( j==k ) continue;
					TLorentzVector p4CandidateStop1 = p4GenTruthJetsStop1[j] + p4GenTruthJetsStop1[k];		// higgs tlorentzvector 
					Double_t diffmassStop1 = p4CandidateStop1.M() - stop1Mass;		// mass difference
					if( abs(diffmassStop1) < 1.0 ){						// diffmass presicion
						p4GenTruthStop1.push_back( p4CandidateStop1 );
					}
				}
			}
		}
	}


	if ( p4GenTruthStop1.size() > 0) {
		// Plots for hardest Stop1
		h_stop11_mass->Fill( p4GenTruthStop1[0].M() );
		h_stop11_pt->Fill( p4GenTruthStop1[0].Pt() );	
		h_stop11_eta->Fill( p4GenTruthStop1[0].Eta() );	

		// Plots for softer Stop1
		if ( p4GenTruthStop1.size() > 1 ){
			h_stop12_mass->Fill( p4GenTruthStop1[1].M() );
			h_stop12_pt->Fill( p4GenTruthStop1[1].Pt() );
		       	h_stop12_eta->Fill( p4GenTruthStop1[1].Eta() );
		       	h_stop1_deltaR->Fill( p4GenTruthStop1[0].DeltaR( p4GenTruthStop1[1] ) );
		       	h_stop1_deltaPhi->Fill( p4GenTruthStop1[0].DeltaPhi( p4GenTruthStop1[1] ) );
	       		h_stop1_cosDeltaPhi->Fill( cos (p4GenTruthStop1[0].DeltaPhi( p4GenTruthStop1[1] ) ) );
		}
	}
	/////////////////////////////////////////////////


	// Stop2 reconstructed mass
	vector< TLorentzVector > p4GenTruthStop2; 
	for(unsigned int j = 0; j < p4GenTruthHiggs.size(); ++j) {
		for(unsigned int k= 0; k < p4GenTruthStop1.size(); ++k) {
			TLorentzVector p4CandidateStop2 = p4GenTruthHiggs[j] + p4GenTruthStop1[k];		// Higgs TLorentzVector
			Double_t diffmassStop2 = p4CandidateStop2.M() - stop2Mass;		// mass difference
			if( abs(diffmassStop2) < 1.0 ){						// diffmass presicion
				p4GenTruthStop2.push_back( p4CandidateStop2 );
			}
		}
	}

	if ( p4GenTruthStop2.size() > 0 ){
		// Plots for hardest Stop2
		h_stop21_mass->Fill( p4GenTruthStop2[0].M() );
		h_stop21_pt->Fill( p4GenTruthStop2[0].Pt() );	
		h_stop21_eta->Fill( p4GenTruthStop2[0].Eta() );	
		// Plots for softer Stop2
		if ( p4GenTruthStop2.size() > 1 ){
		       	h_stop22_mass->Fill( p4GenTruthStop2[1].M() );
		       	h_stop22_pt->Fill( p4GenTruthStop2[1].Pt() );
		       	h_stop22_eta->Fill( p4GenTruthStop2[1].Eta() );
		}
	}
	//////////////////////////////////////////////////// */
}


//////////////////////////////////////////////////////////////////////
///////           ANALYSIS                                     ///////
//////////////////////////////////////////////////////////////////////
void MyAnalyzer::Analysis( vector< TLorentzVector > p4GenTruthB1Higgs, vector< TLorentzVector > p4GenTruthB2Higgs, vector< TLorentzVector > p4RecoJets, vector< TLorentzVector > p4RecoBjets, vector< TLorentzVector > p4RecoBtagJets_CSVM, vector< TLorentzVector > p4RecoPartonFlavorBtagJets_CSVM, vector< TLorentzVector > p4MatchTruthBjets,  vector< TLorentzVector > p4MatchB1Higgs,  vector< TLorentzVector > p4MatchB2Higgs, vector< TLorentzVector > p4MatchTruthAlljets, vector<double> RecoBjets_CSVM){

	////////////////////////////////////////////////////////////////////
	//////       STEP 1                                         ////////
	//////  Best combination of bjets with smallest delta Mass  ////////
	////////////////////////////////////////////////////////////////////

	/////////////////////////////// RecoBJets
	
	//// Structure function step1
	step1Vectors RecobbVectors;
	RecobbVectors = step1( p4RecoBjets );
	vector< double > massRecobb = RecobbVectors.mass;
	vector< double > ptRecobb = RecobbVectors.pt;
	vector< double > scalarPtRecobb = RecobbVectors.scalarPt;
	vector< TLorentzVector > p4RecobbSmallMassDiff = RecobbVectors.p4SmallMassDiff;

	//// TLorentzVectors for cuts
	vector< TLorentzVector > p4RecobbSmallMassDiff_cut50;
	vector< TLorentzVector > p4RecobbSmallMassDiff_cut100;
	vector< TLorentzVector > p4RecobbSmallMassDiff_cut150;
	vector< TLorentzVector > p4RecobbSmallMassDiff_cut200;

	h_massRecobb_num->Fill( massRecobb.size() );

	for(unsigned int t = 0; t < massRecobb.size(); ++t) {

		///// Simple plots
		h_massRecobb->Fill( massRecobb[t] );
		h_PtRecobb->Fill( ptRecobb[t], massRecobb[t] );
		h_scalarPtRecobb->Fill( scalarPtRecobb[t], massRecobb[t] );

		///// Diagonal cuts
		double iDiag=(double)t*10.0+50.0;
		if( massRecobb[t] > ( scalarPtRecobb[t]-iDiag ) ) continue;
		h_scalarPtRecobb_cut50->Fill( scalarPtRecobb[t], massRecobb[t] );
		h_massRecobb_cut50->Fill( massRecobb[t] );
		h_scalarPtRecobb_cut50->Fill( scalarPtRecobb[t], massRecobb[t] );
		p4RecobbSmallMassDiff_cut50.push_back( p4RecobbSmallMassDiff[0] );
		p4RecobbSmallMassDiff_cut50.push_back( p4RecobbSmallMassDiff[1] );
		p4RecobbSmallMassDiff_cut50.push_back( p4RecobbSmallMassDiff[2] );
		p4RecobbSmallMassDiff_cut50.push_back( p4RecobbSmallMassDiff[3] );
	
		double iiDiag=(double)t*10.0+100.0;
		if( massRecobb[t] > ( scalarPtRecobb[t]-iiDiag ) ) continue;
		h_scalarPtRecobb_cut100->Fill( scalarPtRecobb[t], massRecobb[t] );
		h_massRecobb_cut100->Fill( massRecobb[t] );
		h_scalarPtRecobb_cut100->Fill( scalarPtRecobb[t], massRecobb[t] );
		p4RecobbSmallMassDiff_cut100.push_back( p4RecobbSmallMassDiff[0] );
		p4RecobbSmallMassDiff_cut100.push_back( p4RecobbSmallMassDiff[1] );
		p4RecobbSmallMassDiff_cut100.push_back( p4RecobbSmallMassDiff[2] );
		p4RecobbSmallMassDiff_cut100.push_back( p4RecobbSmallMassDiff[3] );
		
		double iiiDiag=(double)t*10.0+150.0;
		if( massRecobb[t] > ( scalarPtRecobb[t]-iiiDiag ) ) continue;
		h_scalarPtRecobb_cut150->Fill( scalarPtRecobb[t], massRecobb[t] );
		h_massRecobb_cut150->Fill( massRecobb[t] );
		h_scalarPtRecobb_cut150->Fill( scalarPtRecobb[t], massRecobb[t] );
		p4RecobbSmallMassDiff_cut150.push_back( p4RecobbSmallMassDiff[0] );
		p4RecobbSmallMassDiff_cut150.push_back( p4RecobbSmallMassDiff[1] );
		p4RecobbSmallMassDiff_cut150.push_back( p4RecobbSmallMassDiff[2] );
		p4RecobbSmallMassDiff_cut150.push_back( p4RecobbSmallMassDiff[3] );
	
		double ivDiag=(double)t*10.0+200.0;
		if( massRecobb[t] > ( scalarPtRecobb[t]-ivDiag ) ) continue;
		h_scalarPtRecobb_cut200->Fill( scalarPtRecobb[t], massRecobb[t] );
		h_massRecobb_cut200->Fill( massRecobb[t] );
		h_scalarPtRecobb_cut200->Fill( scalarPtRecobb[t], massRecobb[t] );
		p4RecobbSmallMassDiff_cut200.push_back( p4RecobbSmallMassDiff[0] );
		p4RecobbSmallMassDiff_cut200.push_back( p4RecobbSmallMassDiff[1] );
		p4RecobbSmallMassDiff_cut200.push_back( p4RecobbSmallMassDiff[2] );
		p4RecobbSmallMassDiff_cut200.push_back( p4RecobbSmallMassDiff[3] );
	}
	//////////////////////////////////////////////////////////


	////////////////////////////  RecoBJets with Btagging
	
	//// Structure function step1
	step1Vectors RecoBtagJets_CSVM_Vectors;
	RecoBtagJets_CSVM_Vectors = step1( p4RecoBtagJets_CSVM );
	vector< double > massRecoBtagJets_CSVM = RecoBtagJets_CSVM_Vectors.mass;
	vector< double > ptRecoBtagJets_CSVM = RecoBtagJets_CSVM_Vectors.pt;
	vector< double > scalarPtRecoBtagJets_CSVM = RecoBtagJets_CSVM_Vectors.scalarPt;
	vector< TLorentzVector > p4RecoBtagJets_CSVM_SmallMassDiff = RecoBtagJets_CSVM_Vectors.p4SmallMassDiff;

	//// TLorentzVectors for cuts
	vector< TLorentzVector > p4RecoBtagJets_CSVM_SmallMassDiff_cut50;
	vector< TLorentzVector > p4RecoBtagJets_CSVM_SmallMassDiff_cut100;
	vector< TLorentzVector > p4RecoBtagJets_CSVM_SmallMassDiff_cut150;
	vector< TLorentzVector > p4RecoBtagJets_CSVM_SmallMassDiff_cut200;

	for(unsigned int tiii = 0; tiii < massRecoBtagJets_CSVM.size(); ++tiii) {
		///// Simple plots
		h_massRecoBtagJets_CSVM->Fill( massRecoBtagJets_CSVM[tiii] );
		h_PtRecoBtagJets_CSVM->Fill( ptRecoBtagJets_CSVM[tiii], massRecoBtagJets_CSVM[tiii] );
		h_scalarPtRecoBtagJets_CSVM->Fill( scalarPtRecoBtagJets_CSVM[tiii], massRecoBtagJets_CSVM[tiii] );

		///// Diagonal cuts
		double iDiag=(double)tiii*10.0+50.0;
		if( massRecoBtagJets_CSVM[tiii] > ( scalarPtRecoBtagJets_CSVM[tiii]-iDiag ) ) continue;
		h_scalarPtRecoBtagJets_CSVM_cut50->Fill( scalarPtRecoBtagJets_CSVM[tiii], massRecoBtagJets_CSVM[tiii] );
		h_massRecoBtagJets_CSVM_cut50->Fill( massRecoBtagJets_CSVM[tiii] );
		h_scalarPtRecoBtagJets_CSVM_cut50->Fill( scalarPtRecoBtagJets_CSVM[tiii], massRecoBtagJets_CSVM[tiii] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut50.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[0] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut50.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[1] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut50.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[2] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut50.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[3] );
		
		double iiDiag=(double)tiii*10.0+100.0;
		if( massRecoBtagJets_CSVM[tiii] > ( scalarPtRecoBtagJets_CSVM[tiii]-iiDiag ) ) continue;
		h_scalarPtRecoBtagJets_CSVM_cut100->Fill( scalarPtRecoBtagJets_CSVM[tiii], massRecoBtagJets_CSVM[tiii] );
		h_massRecoBtagJets_CSVM_cut100->Fill( massRecoBtagJets_CSVM[tiii] );
		h_scalarPtRecoBtagJets_CSVM_cut100->Fill( scalarPtRecoBtagJets_CSVM[tiii], massRecoBtagJets_CSVM[tiii] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut100.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[0] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut100.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[1] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut100.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[2] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut100.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[3] );
		
		double iiiDiag=(double)tiii*10.0+150.0;
		if( massRecoBtagJets_CSVM[tiii] > ( scalarPtRecoBtagJets_CSVM[tiii]-iiiDiag ) ) continue;
		h_scalarPtRecoBtagJets_CSVM_cut150->Fill( scalarPtRecoBtagJets_CSVM[tiii], massRecoBtagJets_CSVM[tiii] );
		h_massRecoBtagJets_CSVM_cut150->Fill( massRecoBtagJets_CSVM[tiii] );
		h_scalarPtRecoBtagJets_CSVM_cut150->Fill( scalarPtRecoBtagJets_CSVM[tiii], massRecoBtagJets_CSVM[tiii] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut150.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[0] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut150.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[1] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut150.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[2] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut150.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[3] );

		double ivDiag=(double)tiii*10.0+200.0;
		if( massRecoBtagJets_CSVM[tiii] > ( scalarPtRecoBtagJets_CSVM[tiii]-ivDiag ) ) continue;
		h_scalarPtRecoBtagJets_CSVM_cut200->Fill( scalarPtRecoBtagJets_CSVM[tiii], massRecoBtagJets_CSVM[tiii] );
		h_massRecoBtagJets_CSVM_cut200->Fill( massRecoBtagJets_CSVM[tiii] );
		h_scalarPtRecoBtagJets_CSVM_cut200->Fill( scalarPtRecoBtagJets_CSVM[tiii], massRecoBtagJets_CSVM[tiii] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut200.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[0] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut200.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[1] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut200.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[2] );
		p4RecoBtagJets_CSVM_SmallMassDiff_cut200.push_back( p4RecoBtagJets_CSVM_SmallMassDiff[3] );

	} 
	//////////////////////////////////////////////////////////

	///// PartonFlavor and  RecoBJets with Btagging
	
	//// Structure function step1
	step1Vectors RecoPartonFlavorBtagJets_CSVM_Vectors;
	RecoPartonFlavorBtagJets_CSVM_Vectors = step1( p4RecoPartonFlavorBtagJets_CSVM );
	vector< double > massRecoPartonFlavorBtagJets_CSVM = RecoPartonFlavorBtagJets_CSVM_Vectors.mass;
	vector< double > ptRecoPartonFlavorBtagJets_CSVM = RecoPartonFlavorBtagJets_CSVM_Vectors.pt;
	vector< double > scalarPtRecoPartonFlavorBtagJets_CSVM = RecoPartonFlavorBtagJets_CSVM_Vectors.scalarPt;
	vector< TLorentzVector > p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff = RecoPartonFlavorBtagJets_CSVM_Vectors.p4SmallMassDiff;

	//// TLorentzVectors for cuts
	vector< TLorentzVector > p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut50;
	vector< TLorentzVector > p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut100;
	vector< TLorentzVector > p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut150;
	vector< TLorentzVector > p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut200;

	for(unsigned int tiii = 0; tiii < massRecoPartonFlavorBtagJets_CSVM.size(); ++tiii) {
		///// Simple plots
		h_massRecoPartonFlavorBtagJets_CSVM->Fill( massRecoPartonFlavorBtagJets_CSVM[tiii] );
		h_PtRecoPartonFlavorBtagJets_CSVM->Fill( ptRecoPartonFlavorBtagJets_CSVM[tiii], massRecoPartonFlavorBtagJets_CSVM[tiii] );
		h_scalarPtRecoPartonFlavorBtagJets_CSVM->Fill( scalarPtRecoPartonFlavorBtagJets_CSVM[tiii], massRecoPartonFlavorBtagJets_CSVM[tiii] );

		///// Diagonal cuts
		double iDiag=(double)tiii*10.0+50.0;
		if( massRecoPartonFlavorBtagJets_CSVM[tiii] > ( scalarPtRecoPartonFlavorBtagJets_CSVM[tiii]-iDiag ) ) continue;
		h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut50->Fill( scalarPtRecoPartonFlavorBtagJets_CSVM[tiii], massRecoPartonFlavorBtagJets_CSVM[tiii] );
		h_massRecoPartonFlavorBtagJets_CSVM_cut50->Fill( massRecoPartonFlavorBtagJets_CSVM[tiii] );
		h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut50->Fill( scalarPtRecoPartonFlavorBtagJets_CSVM[tiii], massRecoPartonFlavorBtagJets_CSVM[tiii] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut50.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[0] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut50.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[1] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut50.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[2] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut50.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[3] );
		
		double iiDiag=(double)tiii*10.0+100.0;
		if( massRecoPartonFlavorBtagJets_CSVM[tiii] > ( scalarPtRecoPartonFlavorBtagJets_CSVM[tiii]-iiDiag ) ) continue;
		h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut100->Fill( scalarPtRecoPartonFlavorBtagJets_CSVM[tiii], massRecoPartonFlavorBtagJets_CSVM[tiii] );
		h_massRecoPartonFlavorBtagJets_CSVM_cut100->Fill( massRecoPartonFlavorBtagJets_CSVM[tiii] );
		h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut100->Fill( scalarPtRecoPartonFlavorBtagJets_CSVM[tiii], massRecoPartonFlavorBtagJets_CSVM[tiii] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut100.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[0] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut100.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[1] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut100.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[2] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut100.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[3] );
		
		double iiiDiag=(double)tiii*10.0+150.0;
		if( massRecoPartonFlavorBtagJets_CSVM[tiii] > ( scalarPtRecoPartonFlavorBtagJets_CSVM[tiii]-iiiDiag ) ) continue;
		h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut150->Fill( scalarPtRecoPartonFlavorBtagJets_CSVM[tiii], massRecoPartonFlavorBtagJets_CSVM[tiii] );
		h_massRecoPartonFlavorBtagJets_CSVM_cut150->Fill( massRecoPartonFlavorBtagJets_CSVM[tiii] );
		h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut150->Fill( scalarPtRecoPartonFlavorBtagJets_CSVM[tiii], massRecoPartonFlavorBtagJets_CSVM[tiii] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut150.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[0] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut150.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[1] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut150.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[2] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut150.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[3] );

		double ivDiag=(double)tiii*10.0+200.0;
		if( massRecoPartonFlavorBtagJets_CSVM[tiii] > ( scalarPtRecoPartonFlavorBtagJets_CSVM[tiii]-ivDiag ) ) continue;
		h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut200->Fill( scalarPtRecoPartonFlavorBtagJets_CSVM[tiii], massRecoPartonFlavorBtagJets_CSVM[tiii] );
		h_massRecoPartonFlavorBtagJets_CSVM_cut200->Fill( massRecoPartonFlavorBtagJets_CSVM[tiii] );
		h_scalarPtRecoPartonFlavorBtagJets_CSVM_cut200->Fill( scalarPtRecoPartonFlavorBtagJets_CSVM[tiii], massRecoPartonFlavorBtagJets_CSVM[tiii] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut200.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[0] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut200.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[1] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut200.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[2] );
		p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff_cut200.push_back( p4RecoPartonFlavorBtagJets_CSVM_SmallMassDiff[3] );

	} 
	//////////////////////////////////////////////////////////

	////// MatchBJets
	
	//// Structure function step1
	step1Vectors bbVectors;
	bbVectors = step1( p4MatchTruthBjets );
	vector< double > massbb = bbVectors.mass;
	vector< double > ptbb = bbVectors.pt;
	vector< double > scalarPtbb = bbVectors.scalarPt;
	vector< TLorentzVector > p4bbSmallMassDiff = bbVectors.p4SmallMassDiff;

	//// TLorentzVectors for cuts
	vector< TLorentzVector > p4bbSmallMassDiff_cut50;
	vector< TLorentzVector > p4bbSmallMassDiff_cut100;
	vector< TLorentzVector > p4bbSmallMassDiff_cut150;
	vector< TLorentzVector > p4bbSmallMassDiff_cut200;

	for(unsigned int ti = 0; ti < massbb.size(); ++ti) {

		///// Simple plots
		h_massbb->Fill( massbb[ti] );
		h_Ptbb->Fill( ptbb[ti], massbb[ti] );
		h_scalarPtbb->Fill( scalarPtbb[ti], massbb[ti] );

		///// Diagonal cuts
		double iDiag=(double)ti*10.0+50.0;
		if( massbb[ti] > ( scalarPtbb[ti]-iDiag ) ) continue;
		h_scalarPtbb_cut50->Fill( scalarPtbb[ti], massbb[ti] );
		h_massbb_cut50->Fill( massbb[ti] );
		h_scalarPtbb_cut50->Fill( scalarPtbb[ti], massbb[ti] );
		p4bbSmallMassDiff_cut50.push_back( p4bbSmallMassDiff[0] );
		p4bbSmallMassDiff_cut50.push_back( p4bbSmallMassDiff[1] );
		p4bbSmallMassDiff_cut50.push_back( p4bbSmallMassDiff[2] );
		p4bbSmallMassDiff_cut50.push_back( p4bbSmallMassDiff[3] );

		double iiDiag=(double)ti*10.0+100.0;
		if( massbb[ti] > ( scalarPtbb[ti]-iiDiag ) ) continue;
		h_scalarPtbb_cut100->Fill( scalarPtbb[ti], massbb[ti] );
		h_massbb_cut100->Fill( massbb[ti] );
		h_scalarPtbb_cut100->Fill( scalarPtbb[ti], massbb[ti] );
		p4bbSmallMassDiff_cut100.push_back( p4bbSmallMassDiff[0] );
		p4bbSmallMassDiff_cut100.push_back( p4bbSmallMassDiff[1] );
		p4bbSmallMassDiff_cut100.push_back( p4bbSmallMassDiff[2] );
		p4bbSmallMassDiff_cut100.push_back( p4bbSmallMassDiff[3] );
	
		double iiiDiag=(double)ti*10.0+150.0;
		if( massbb[ti] > ( scalarPtbb[ti]-iiiDiag ) ) continue;
		h_scalarPtbb_cut150->Fill( scalarPtbb[ti], massbb[ti] );
		h_massbb_cut150->Fill( massbb[ti] );
		h_scalarPtbb_cut150->Fill( scalarPtbb[ti], massbb[ti] );
		p4bbSmallMassDiff_cut150.push_back( p4bbSmallMassDiff[0] );
		p4bbSmallMassDiff_cut150.push_back( p4bbSmallMassDiff[1] );
		p4bbSmallMassDiff_cut150.push_back( p4bbSmallMassDiff[2] );
		p4bbSmallMassDiff_cut150.push_back( p4bbSmallMassDiff[3] );

		double ivDiag=(double)ti*10.0+200.0;
		if( massbb[ti] > ( scalarPtbb[ti]-ivDiag ) ) continue;
		h_scalarPtbb_cut200->Fill( scalarPtbb[ti], massbb[ti] );
		h_massbb_cut200->Fill( massbb[ti] );
		h_scalarPtbb_cut200->Fill( scalarPtbb[ti], massbb[ti] );
		p4bbSmallMassDiff_cut200.push_back( p4bbSmallMassDiff[0] );
		p4bbSmallMassDiff_cut200.push_back( p4bbSmallMassDiff[1] );
		p4bbSmallMassDiff_cut200.push_back( p4bbSmallMassDiff[2] );
		p4bbSmallMassDiff_cut200.push_back( p4bbSmallMassDiff[3] );
	}
	///////////////////////////////////////////////////////////////////

	// MatchBJets from Higgs
	vector< TLorentzVector > p4bbHiggsSmallMassDiff;
	vector< TLorentzVector > p4bbHiggsSmallMassDiff_cut50;
	vector< TLorentzVector > p4bbHiggsSmallMassDiff_cut100;
	vector< TLorentzVector > p4bbHiggsSmallMassDiff_cut150;
	vector< TLorentzVector > p4bbHiggsSmallMassDiff_cut200;
	vector< double > massbbHiggs;
	vector< double > ptbbHiggs;
	vector< double > scalarPtbbHiggs;

	if ( p4MatchB1Higgs.size() == 2 && p4MatchB2Higgs.size() == 2 ){
		
		TLorentzVector tmpMassbbHiggs1 = p4MatchB1Higgs[0] + p4MatchB1Higgs[1];
		TLorentzVector tmpMassbbHiggs2 = p4MatchB2Higgs[0] + p4MatchB2Higgs[1];
		double scalarPtbbHiggs1 = p4MatchB1Higgs[0].Pt() + p4MatchB1Higgs[1].Pt();
		double scalarPtbbHiggs2 = p4MatchB2Higgs[0].Pt() + p4MatchB2Higgs[1].Pt();
		p4bbHiggsSmallMassDiff.push_back( p4MatchB1Higgs[0] );
		p4bbHiggsSmallMassDiff.push_back( p4MatchB1Higgs[1] );
		p4bbHiggsSmallMassDiff.push_back( p4MatchB2Higgs[0] );
		p4bbHiggsSmallMassDiff.push_back( p4MatchB2Higgs[1] );

		////// Store vectors
		massbbHiggs.push_back ( tmpMassbbHiggs1.M() );
		massbbHiggs.push_back ( tmpMassbbHiggs2.M() );
		ptbbHiggs.push_back ( tmpMassbbHiggs1.Pt() );
		ptbbHiggs.push_back ( tmpMassbbHiggs2.Pt() );
		scalarPtbbHiggs.push_back ( scalarPtbbHiggs1 );
		scalarPtbbHiggs.push_back ( scalarPtbbHiggs2 );
	}

	h_massbbHiggs_num->Fill( massbbHiggs.size() );	/// test number of bbHiggs
	for(unsigned int tiv = 0; tiv < massbbHiggs.size(); ++tiv) {
		///// Simple plots
		h_massbbHiggs->Fill( massbbHiggs[tiv] );
		h_PtbbHiggs->Fill( ptbbHiggs[tiv], massbbHiggs[tiv] );
		h_scalarPtbbHiggs->Fill( scalarPtbbHiggs[tiv], massbbHiggs[tiv] );

		///// Diagonal cuts
		double iDiag=(double)tiv*10.0+50.0;
		if( massbbHiggs[tiv] > ( scalarPtbbHiggs[tiv]-iDiag ) ) continue;
		h_scalarPtbbHiggs_cut50->Fill( scalarPtbbHiggs[tiv], massbbHiggs[tiv] );
		h_massbbHiggs_cut50->Fill( massbbHiggs[tiv] );
		h_scalarPtbbHiggs_cut50->Fill( scalarPtbbHiggs[tiv], massbbHiggs[tiv] );
		p4bbHiggsSmallMassDiff_cut50.push_back( p4MatchB1Higgs[0] );
		p4bbHiggsSmallMassDiff_cut50.push_back( p4MatchB1Higgs[1] );
		p4bbHiggsSmallMassDiff_cut50.push_back( p4MatchB2Higgs[0] );
		p4bbHiggsSmallMassDiff_cut50.push_back( p4MatchB2Higgs[1] );

		double iiDiag=(double)tiv*10.0+100.0;
		if( massbbHiggs[tiv] > ( scalarPtbbHiggs[tiv]-iiDiag ) ) continue;
		h_scalarPtbbHiggs_cut100->Fill( scalarPtbbHiggs[tiv], massbbHiggs[tiv] );
		h_massbbHiggs_cut100->Fill( massbbHiggs[tiv] );
		h_scalarPtbbHiggs_cut100->Fill( scalarPtbbHiggs[tiv], massbbHiggs[tiv] );
		p4bbHiggsSmallMassDiff_cut100.push_back( p4MatchB1Higgs[0] );
		p4bbHiggsSmallMassDiff_cut100.push_back( p4MatchB1Higgs[1] );
		p4bbHiggsSmallMassDiff_cut100.push_back( p4MatchB2Higgs[0] );
		p4bbHiggsSmallMassDiff_cut100.push_back( p4MatchB2Higgs[1] );

		double iiiDiag=(double)tiv*10.0+150.0;
		if( massbbHiggs[tiv] > ( scalarPtbbHiggs[tiv]-iiiDiag ) ) continue;
		h_scalarPtbbHiggs_cut150->Fill( scalarPtbbHiggs[tiv], massbbHiggs[tiv] );
		h_massbbHiggs_cut150->Fill( massbbHiggs[tiv] );
		h_scalarPtbbHiggs_cut150->Fill( scalarPtbbHiggs[tiv], massbbHiggs[tiv] );
		p4bbHiggsSmallMassDiff_cut150.push_back( p4MatchB1Higgs[0] );
		p4bbHiggsSmallMassDiff_cut150.push_back( p4MatchB1Higgs[1] );
		p4bbHiggsSmallMassDiff_cut150.push_back( p4MatchB2Higgs[0] );
		p4bbHiggsSmallMassDiff_cut150.push_back( p4MatchB2Higgs[1] );

		double ivDiag=(double)tiv*10.0+200.0;
		if( massbbHiggs[tiv] > ( scalarPtbbHiggs[tiv]-ivDiag ) ) continue;
		h_scalarPtbbHiggs_cut200->Fill( scalarPtbbHiggs[tiv], massbbHiggs[tiv] );
		h_massbbHiggs_cut200->Fill( massbbHiggs[tiv] );
		h_scalarPtbbHiggs_cut200->Fill( scalarPtbbHiggs[tiv], massbbHiggs[tiv] );
		p4bbHiggsSmallMassDiff_cut200.push_back( p4MatchB1Higgs[0] );
		p4bbHiggsSmallMassDiff_cut200.push_back( p4MatchB1Higgs[1] );
		p4bbHiggsSmallMassDiff_cut200.push_back( p4MatchB2Higgs[0] );
		p4bbHiggsSmallMassDiff_cut200.push_back( p4MatchB2Higgs[1] );

	}

	// Gen Truth Higgs
	vector< double > massGenTruthHiggs;
	vector< double > ptGenTruthHiggs;
	vector< double > scalarPtGenTruthHiggs;

	if ( p4GenTruthB1Higgs.size() == 2 && p4GenTruthB2Higgs.size() == 2 ){
		
		TLorentzVector tmpMassGenTruthHiggs1 = p4GenTruthB1Higgs[0] + p4GenTruthB1Higgs[1];
		TLorentzVector tmpMassGenTruthHiggs2 = p4GenTruthB2Higgs[0] + p4GenTruthB2Higgs[1];
		double scalarPtGenTruthHiggs1 = p4GenTruthB1Higgs[0].Pt() + p4GenTruthB1Higgs[1].Pt();
		double scalarPtGenTruthHiggs2 = p4GenTruthB2Higgs[0].Pt() + p4GenTruthB2Higgs[1].Pt();

		////// Store vectors
		massGenTruthHiggs.push_back ( tmpMassGenTruthHiggs1.M() );
		massGenTruthHiggs.push_back ( tmpMassGenTruthHiggs2.M() );
		ptGenTruthHiggs.push_back ( tmpMassGenTruthHiggs1.Pt() );
		ptGenTruthHiggs.push_back ( tmpMassGenTruthHiggs2.Pt() );
		scalarPtGenTruthHiggs.push_back ( scalarPtGenTruthHiggs1 );
		scalarPtGenTruthHiggs.push_back ( scalarPtGenTruthHiggs2 );
	}

	for(unsigned int tii = 0; tii < massGenTruthHiggs.size(); ++tii) {
		///// Simple plots
		h_massGenTruthHiggs->Fill( massGenTruthHiggs[tii] );
		h_PtGenTruthHiggs->Fill( ptGenTruthHiggs[tii], massGenTruthHiggs[tii] );
		h_scalarPtGenTruthHiggs->Fill( scalarPtGenTruthHiggs[tii], massGenTruthHiggs[tii] );
	}
	///////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////
	/////////// STEP 2 - 3 ////////////////
	/////// Mass jj vs sum pt jj //////////
	///////////////////////////////////////

	///// Match All jets
	vector< TLorentzVector > p4MatchjjWObb;
	vector< TLorentzVector > p4MatchjjWObb_cut50;
	vector< TLorentzVector > p4MatchjjWObb_cut100;
	vector< TLorentzVector > p4MatchjjWObb_cut150;
	vector< TLorentzVector > p4MatchjjWObb_cut200;
	vector< TLorentzVector > p4jWObb;
	vector< TLorentzVector > p4jWObb_cut50;
	vector< TLorentzVector > p4jWObb_cut100;
	vector< TLorentzVector > p4jWObb_cut150;
	vector< TLorentzVector > p4jWObb_cut200;
	vector< double > scalarsumMatchjjWObbpt;
	vector< double > scalarsumMatchjjWObbpt_cut50;
	vector< double > scalarsumMatchjjWObbpt_cut100;
	vector< double > scalarsumMatchjjWObbpt_cut150;
	vector< double > scalarsumMatchjjWObbpt_cut200;

	if ( p4MatchTruthAlljets.size() > 1 && p4MatchTruthBjets.size() > 3 ){
		for(unsigned int iii = 0; iii < p4MatchTruthAlljets.size(); ++iii) {

			////// Step 2 - dijet mass vs sum pt of jets
			for(unsigned int iij = 0; iij < p4MatchTruthAlljets.size(); ++iij) {
				if ( iii==iij ) continue;
				TLorentzVector p4jj = p4MatchTruthAlljets[iii] + p4MatchTruthAlljets[iij];
				double sumjjpt = p4MatchTruthAlljets[iii].Pt() + p4MatchTruthAlljets[iij].Pt();
				h_matchjj_masspt->Fill( sumjjpt, p4jj.M() );
			}
			
			////// Step 3 - dijet mass vs sum pt of jets without bs from step 1
			//cout << "1 " << iii << " " << p4MatchTruthAlljets[iii].Pt() << endl;
			if ( p4bbSmallMassDiff.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff[0] ) || ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff[1] ) || ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff[2] ) || ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff[3] ) )continue;
				//cout << "3 " << iii << " " << p4MatchTruthAlljets[iii].Pt() << endl;
				p4jWObb.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4bbSmallMassDiff_cut50.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut50[0] ) || ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut50[1] ) || ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut50[2] ) || ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut50[3] ) )continue;
				p4jWObb_cut50.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4bbSmallMassDiff_cut100.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut100[0] ) || ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut100[1] ) || ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut100[2] ) || ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut100[3] ) )continue;
				p4jWObb_cut100.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4bbSmallMassDiff_cut150.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut150[0] ) || ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut150[1] ) || ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut150[2] ) || ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut150[3] ) )continue;
				p4jWObb_cut150.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4bbSmallMassDiff_cut200.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut200[0] ) || ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut200[1] ) || ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut200[2] ) || ( p4MatchTruthAlljets[iii] == p4bbSmallMassDiff_cut200[3] ) )continue;
				p4jWObb_cut200.push_back( p4MatchTruthAlljets[iii] );
			}
		}
	}
			
	for(unsigned int iim = 0; iim < p4jWObb.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWObb.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWObb[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWObb = p4jWObb[iim] + p4jWObb[iin];
			double sumMatchjjWObbpt = p4jWObb[iim].Pt() + p4jWObb[iin].Pt();
			p4MatchjjWObb.push_back( tmpp4MatchjjWObb );
			scalarsumMatchjjWObbpt.push_back( sumMatchjjWObbpt );
			h_matchjjWObb_masspt->Fill( sumMatchjjWObbpt, tmpp4MatchjjWObb.M() );
			h_massmatchjjWObb->Fill( tmpp4MatchjjWObb.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWObb_cut50.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWObb_cut50.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWObb_cut50[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWObb_cut50 = p4jWObb_cut50[iim] + p4jWObb_cut50[iin];
			double sumMatchjjWObbpt_cut50 = p4jWObb_cut50[iim].Pt() + p4jWObb_cut50[iin].Pt();
			p4MatchjjWObb_cut50.push_back( tmpp4MatchjjWObb_cut50 );
			scalarsumMatchjjWObbpt_cut50.push_back( sumMatchjjWObbpt_cut50 );
			h_matchjjWObb_masspt_cut50->Fill( sumMatchjjWObbpt_cut50, tmpp4MatchjjWObb_cut50.M() );
			h_massmatchjjWObb_cut50->Fill( tmpp4MatchjjWObb_cut50.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWObb_cut100.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWObb_cut100.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWObb_cut100[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWObb_cut100 = p4jWObb_cut100[iim] + p4jWObb_cut100[iin];
			double sumMatchjjWObbpt_cut100 = p4jWObb_cut100[iim].Pt() + p4jWObb_cut100[iin].Pt();
			p4MatchjjWObb_cut100.push_back( tmpp4MatchjjWObb_cut100 );
			scalarsumMatchjjWObbpt_cut100.push_back( sumMatchjjWObbpt_cut100 );
			h_matchjjWObb_masspt_cut100->Fill( sumMatchjjWObbpt_cut100, tmpp4MatchjjWObb_cut100.M() );
			h_massmatchjjWObb_cut100->Fill( tmpp4MatchjjWObb_cut100.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWObb_cut150.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWObb_cut150.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWObb_cut150[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWObb_cut150 = p4jWObb_cut150[iim] + p4jWObb_cut150[iin];
			double sumMatchjjWObbpt_cut150 = p4jWObb_cut150[iim].Pt() + p4jWObb_cut150[iin].Pt();
			p4MatchjjWObb_cut150.push_back( tmpp4MatchjjWObb_cut150 );
			scalarsumMatchjjWObbpt_cut150.push_back( sumMatchjjWObbpt_cut150 );
			h_matchjjWObb_masspt_cut150->Fill( sumMatchjjWObbpt_cut150, tmpp4MatchjjWObb_cut150.M() );
			h_massmatchjjWObb_cut150->Fill( tmpp4MatchjjWObb_cut150.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWObb_cut200.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWObb_cut200.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWObb_cut200[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWObb_cut200 = p4jWObb_cut200[iim] + p4jWObb_cut200[iin];
			double sumMatchjjWObbpt_cut200 = p4jWObb_cut200[iim].Pt() + p4jWObb_cut200[iin].Pt();
			p4MatchjjWObb_cut200.push_back( tmpp4MatchjjWObb_cut200 );
			scalarsumMatchjjWObbpt_cut200.push_back( sumMatchjjWObbpt_cut200 );
			h_matchjjWObb_masspt_cut200->Fill( sumMatchjjWObbpt_cut200, tmpp4MatchjjWObb_cut200.M() );
			h_massmatchjjWObb_cut200->Fill( tmpp4MatchjjWObb_cut200.M() );
		}
	}
	//////////////////////////////////////////////////////////////////////////////////


	///// Match jets from Higgs
	vector< TLorentzVector > p4MatchjjWObbHiggs;
	vector< TLorentzVector > p4MatchjjWObbHiggs_cut50;
	vector< TLorentzVector > p4MatchjjWObbHiggs_cut100;
	vector< TLorentzVector > p4MatchjjWObbHiggs_cut150;
	vector< TLorentzVector > p4MatchjjWObbHiggs_cut200;
	vector< TLorentzVector > p4jWObbHiggs;
	vector< TLorentzVector > p4jWObbHiggs_cut50;
	vector< TLorentzVector > p4jWObbHiggs_cut100;
	vector< TLorentzVector > p4jWObbHiggs_cut150;
	vector< TLorentzVector > p4jWObbHiggs_cut200;
	vector< double > scalarsumMatchjjWObbHiggspt;
	vector< double > scalarsumMatchjjWObbHiggspt_cut50;
	vector< double > scalarsumMatchjjWObbHiggspt_cut100;
	vector< double > scalarsumMatchjjWObbHiggspt_cut150;
	vector< double > scalarsumMatchjjWObbHiggspt_cut200;

	if ( p4MatchTruthAlljets.size() > 1 && p4MatchB1Higgs.size() == 2 && p4MatchB2Higgs.size() == 2 ){
		for(unsigned int iii = 0; iii < p4MatchTruthAlljets.size(); ++iii) {

			////// Step 2 - dijet mass vs sum pt of jets
			for(unsigned int iij = 0; iij < p4MatchTruthAlljets.size(); ++iij) {
				if ( iii==iij ) continue;
				TLorentzVector p4jjHiggs = p4MatchTruthAlljets[iii] + p4MatchTruthAlljets[iij];
				double sumjjHiggspt = p4MatchTruthAlljets[iii].Pt() + p4MatchTruthAlljets[iij].Pt();
				h_matchjjHiggs_masspt->Fill( sumjjHiggspt, p4jjHiggs.M() );
			}
			
			////// Step 3 - dijet mass vs sum pt of jets without bs from step 1
			//cout << "1 " << iii << " " << p4MatchTruthAlljets[iii].Pt() << endl;
			if ( p4bbHiggsSmallMassDiff.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff[0] ) || ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff[1] ) || ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff[2] ) || ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff[3] ) )continue;
				//cout << "3 " << iii << " " << p4MatchTruthAlljets[iii].Pt() << endl;
				p4jWObbHiggs.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4bbHiggsSmallMassDiff_cut50.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut50[0] ) || ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut50[1] ) || ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut50[2] ) || ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut50[3] ) )continue;
				p4jWObbHiggs_cut50.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4bbHiggsSmallMassDiff_cut100.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut100[0] ) || ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut100[1] ) || ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut100[2] ) || ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut100[3] ) )continue;
				p4jWObbHiggs_cut100.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4bbHiggsSmallMassDiff_cut150.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut150[0] ) || ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut150[1] ) || ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut150[2] ) || ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut150[3] ) )continue;
				p4jWObbHiggs_cut150.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4bbHiggsSmallMassDiff_cut200.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut200[0] ) || ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut200[1] ) || ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut200[2] ) || ( p4MatchTruthAlljets[iii] == p4bbHiggsSmallMassDiff_cut200[3] ) )continue;
				p4jWObbHiggs_cut200.push_back( p4MatchTruthAlljets[iii] );
			}
		}
	}
			
	for(unsigned int iim = 0; iim < p4jWObbHiggs.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWObbHiggs.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWObbHiggs[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWObbHiggs = p4jWObbHiggs[iim] + p4jWObbHiggs[iin];
			double sumMatchjjWObbHiggspt = p4jWObbHiggs[iim].Pt() + p4jWObbHiggs[iin].Pt();
			p4MatchjjWObbHiggs.push_back( tmpp4MatchjjWObbHiggs );
			scalarsumMatchjjWObbHiggspt.push_back( sumMatchjjWObbHiggspt );
			h_matchjjWObbHiggs_masspt->Fill( sumMatchjjWObbHiggspt, tmpp4MatchjjWObbHiggs.M() );
			h_massmatchjjWObbHiggs->Fill( tmpp4MatchjjWObbHiggs.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWObbHiggs_cut50.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWObbHiggs_cut50.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWObbHiggs_cut50[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWObbHiggs_cut50 = p4jWObbHiggs_cut50[iim] + p4jWObbHiggs_cut50[iin];
			double sumMatchjjWObbHiggspt_cut50 = p4jWObbHiggs_cut50[iim].Pt() + p4jWObbHiggs_cut50[iin].Pt();
			p4MatchjjWObbHiggs_cut50.push_back( tmpp4MatchjjWObbHiggs_cut50 );
			scalarsumMatchjjWObbHiggspt_cut50.push_back( sumMatchjjWObbHiggspt_cut50 );
			h_matchjjWObbHiggs_masspt_cut50->Fill( sumMatchjjWObbHiggspt_cut50, tmpp4MatchjjWObbHiggs_cut50.M() );
			h_massmatchjjWObbHiggs_cut50->Fill( tmpp4MatchjjWObbHiggs_cut50.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWObbHiggs_cut100.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWObbHiggs_cut100.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWObbHiggs_cut100[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWObbHiggs_cut100 = p4jWObbHiggs_cut100[iim] + p4jWObbHiggs_cut100[iin];
			double sumMatchjjWObbHiggspt_cut100 = p4jWObbHiggs_cut100[iim].Pt() + p4jWObbHiggs_cut100[iin].Pt();
			p4MatchjjWObbHiggs_cut100.push_back( tmpp4MatchjjWObbHiggs_cut100 );
			scalarsumMatchjjWObbHiggspt_cut100.push_back( sumMatchjjWObbHiggspt_cut100 );
			h_matchjjWObbHiggs_masspt_cut100->Fill( sumMatchjjWObbHiggspt_cut100, tmpp4MatchjjWObbHiggs_cut100.M() );
			h_massmatchjjWObbHiggs_cut100->Fill( tmpp4MatchjjWObbHiggs_cut100.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWObbHiggs_cut150.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWObbHiggs_cut150.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWObbHiggs_cut150[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWObbHiggs_cut150 = p4jWObbHiggs_cut150[iim] + p4jWObbHiggs_cut150[iin];
			double sumMatchjjWObbHiggspt_cut150 = p4jWObbHiggs_cut150[iim].Pt() + p4jWObbHiggs_cut150[iin].Pt();
			p4MatchjjWObbHiggs_cut150.push_back( tmpp4MatchjjWObbHiggs_cut150 );
			scalarsumMatchjjWObbHiggspt_cut150.push_back( sumMatchjjWObbHiggspt_cut150 );
			h_matchjjWObbHiggs_masspt_cut150->Fill( sumMatchjjWObbHiggspt_cut150, tmpp4MatchjjWObbHiggs_cut150.M() );
			h_massmatchjjWObbHiggs_cut150->Fill( tmpp4MatchjjWObbHiggs_cut150.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWObbHiggs_cut200.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWObbHiggs_cut200.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWObbHiggs_cut200[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWObbHiggs_cut200 = p4jWObbHiggs_cut200[iim] + p4jWObbHiggs_cut200[iin];
			double sumMatchjjWObbHiggspt_cut200 = p4jWObbHiggs_cut200[iim].Pt() + p4jWObbHiggs_cut200[iin].Pt();
			p4MatchjjWObbHiggs_cut200.push_back( tmpp4MatchjjWObbHiggs_cut200 );
			scalarsumMatchjjWObbHiggspt_cut200.push_back( sumMatchjjWObbHiggspt_cut200 );
			h_matchjjWObbHiggs_masspt_cut200->Fill( sumMatchjjWObbHiggspt_cut200, tmpp4MatchjjWObbHiggs_cut200.M() );
			h_massmatchjjWObbHiggs_cut200->Fill( tmpp4MatchjjWObbHiggs_cut200.M() );
		}
	}
	///////////////////////////////////////////////////////////////////////

	///////// RECO jets
	vector< TLorentzVector > p4jjWORecobb;
	vector< TLorentzVector > p4jjWORecobb_cut50;
	vector< TLorentzVector > p4jjWORecobb_cut100;
	vector< TLorentzVector > p4jjWORecobb_cut150;
	vector< TLorentzVector > p4jjWORecobb_cut200;
	vector< TLorentzVector > p4jWORecobb;
	vector< TLorentzVector > p4jWORecobb_cut50;
	vector< TLorentzVector > p4jWORecobb_cut100;
	vector< TLorentzVector > p4jWORecobb_cut150;
	vector< TLorentzVector > p4jWORecobb_cut200;
	vector< double > scalarsumjjWORecobbpt;
	vector< double > scalarsumjjWORecobbpt_cut50;
	vector< double > scalarsumjjWORecobbpt_cut100;
	vector< double > scalarsumjjWORecobbpt_cut150;
	vector< double > scalarsumjjWORecobbpt_cut200;

	if ( p4RecoJets.size() > 1 && p4RecoBjets.size() > 3 ){
		for(unsigned int iii = 0; iii < p4RecoJets.size(); ++iii) {

			////// Step 2 - dijet mass vs sum pt of jets
			for(unsigned int iij = 0; iij < p4RecoJets.size(); ++iij) {
				if ( iii==iij ) continue;
				TLorentzVector p4Recojj = p4RecoJets[iii] + p4RecoJets[iij];
				double sumRecojjpt = p4RecoJets[iii].Pt() + p4RecoJets[iij].Pt();
				h_Recojj_masspt->Fill( sumRecojjpt, p4Recojj.M() );
			}
			
			////// Step 3 - dijet mass vs sum pt of jets without bs from step 1
			if ( p4RecobbSmallMassDiff.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecobbSmallMassDiff[0] ) || ( p4RecoJets[iii] == p4RecobbSmallMassDiff[1] ) || ( p4RecoJets[iii] == p4RecobbSmallMassDiff[2] ) || ( p4RecoJets[iii] == p4RecobbSmallMassDiff[3] ) )continue;
				p4jWORecobb.push_back( p4RecoJets[iii] );
			}

			if ( p4RecobbSmallMassDiff_cut50.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut50[0] ) || ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut50[1] ) || ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut50[2] ) || ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut50[3] ) )continue;
				p4jWORecobb_cut50.push_back( p4RecoJets[iii] );
			}

			if ( p4RecobbSmallMassDiff_cut100.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut100[0] ) || ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut100[1] ) || ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut100[2] ) || ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut100[3] ) )continue;
				p4jWORecobb_cut100.push_back( p4RecoJets[iii] );
			}

			if ( p4RecobbSmallMassDiff_cut150.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut150[0] ) || ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut150[1] ) || ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut150[2] ) || ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut150[3] ) )continue;
				p4jWORecobb_cut150.push_back( p4RecoJets[iii] );
			}

			if ( p4RecobbSmallMassDiff_cut200.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut200[0] ) || ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut200[1] ) || ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut200[2] ) || ( p4RecoJets[iii] == p4RecobbSmallMassDiff_cut200[3] ) )continue;
				p4jWORecobb_cut200.push_back( p4RecoJets[iii] );
			}
		}
	}
			
	for(unsigned int iim = 0; iim < p4jWORecobb.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecobb.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecobb[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecobb = p4jWORecobb[iim] + p4jWORecobb[iin];
			double sumjjWORecobbpt = p4jWORecobb[iim].Pt() + p4jWORecobb[iin].Pt();
			p4jjWORecobb.push_back( tmpp4jjWORecobb );
			scalarsumjjWORecobbpt.push_back( sumjjWORecobbpt );
			h_jjWORecobb_masspt->Fill( sumjjWORecobbpt, tmpp4jjWORecobb.M() );
			h_massjjWORecobb->Fill( tmpp4jjWORecobb.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecobb_cut50.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecobb_cut50.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecobb_cut50[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecobb_cut50 = p4jWORecobb_cut50[iim] + p4jWORecobb_cut50[iin];
			double sumjjWORecobbpt_cut50 = p4jWORecobb_cut50[iim].Pt() + p4jWORecobb_cut50[iin].Pt();
			p4jjWORecobb_cut50.push_back( tmpp4jjWORecobb_cut50 );
			scalarsumjjWORecobbpt_cut50.push_back( sumjjWORecobbpt_cut50 );
			h_jjWORecobb_masspt_cut50->Fill( sumjjWORecobbpt_cut50, tmpp4jjWORecobb_cut50.M() );
			h_massjjWORecobb_cut50->Fill( tmpp4jjWORecobb_cut50.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecobb_cut100.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecobb_cut100.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecobb_cut100[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecobb_cut100 = p4jWORecobb_cut100[iim] + p4jWORecobb_cut100[iin];
			double sumjjWORecobbpt_cut100 = p4jWORecobb_cut100[iim].Pt() + p4jWORecobb_cut100[iin].Pt();
			p4jjWORecobb_cut100.push_back( tmpp4jjWORecobb_cut100 );
			scalarsumjjWORecobbpt_cut100.push_back( sumjjWORecobbpt_cut100 );
			h_jjWORecobb_masspt_cut100->Fill( sumjjWORecobbpt_cut100, tmpp4jjWORecobb_cut100.M() );
			h_massjjWORecobb_cut100->Fill( tmpp4jjWORecobb_cut100.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecobb_cut150.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecobb_cut150.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecobb_cut150[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecobb_cut150 = p4jWORecobb_cut150[iim] + p4jWORecobb_cut150[iin];
			double sumjjWORecobbpt_cut150 = p4jWORecobb_cut150[iim].Pt() + p4jWORecobb_cut150[iin].Pt();
			p4jjWORecobb_cut150.push_back( tmpp4jjWORecobb_cut150 );
			scalarsumjjWORecobbpt_cut150.push_back( sumjjWORecobbpt_cut150 );
			h_jjWORecobb_masspt_cut150->Fill( sumjjWORecobbpt_cut150, tmpp4jjWORecobb_cut150.M() );
			h_massjjWORecobb_cut150->Fill( tmpp4jjWORecobb_cut150.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecobb_cut200.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecobb_cut200.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecobb_cut200[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecobb_cut200 = p4jWORecobb_cut200[iim] + p4jWORecobb_cut200[iin];
			double sumjjWORecobbpt_cut200 = p4jWORecobb_cut200[iim].Pt() + p4jWORecobb_cut200[iin].Pt();
			p4jjWORecobb_cut200.push_back( tmpp4jjWORecobb_cut200 );
			scalarsumjjWORecobbpt_cut200.push_back( sumjjWORecobbpt_cut200 );
			h_jjWORecobb_masspt_cut200->Fill( sumjjWORecobbpt_cut200, tmpp4jjWORecobb_cut200.M() );
			h_massjjWORecobb_cut200->Fill( tmpp4jjWORecobb_cut200.M() );
		}
	}
	/////////////////////////////////////////////////////

	///////// RECO jets + Btagging CSVM
	vector< TLorentzVector > p4jjWORecoBtagJets_CSVM;
	vector< TLorentzVector > p4jjWORecoBtagJets_CSVM_cut50;
	vector< TLorentzVector > p4jjWORecoBtagJets_CSVM_cut100;
	vector< TLorentzVector > p4jjWORecoBtagJets_CSVM_cut150;
	vector< TLorentzVector > p4jjWORecoBtagJets_CSVM_cut200;
	vector< TLorentzVector > p4jWORecoBtagJets_CSVM;
	vector< TLorentzVector > p4jWORecoBtagJets_CSVM_cut50;
	vector< TLorentzVector > p4jWORecoBtagJets_CSVM_cut100;
	vector< TLorentzVector > p4jWORecoBtagJets_CSVM_cut150;
	vector< TLorentzVector > p4jWORecoBtagJets_CSVM_cut200;
	vector< double > scalarsumjjWORecoBtagJets_CSVMpt;
	vector< double > scalarsumjjWORecoBtagJets_CSVMpt_cut50;
	vector< double > scalarsumjjWORecoBtagJets_CSVMpt_cut100;
	vector< double > scalarsumjjWORecoBtagJets_CSVMpt_cut150;
	vector< double > scalarsumjjWORecoBtagJets_CSVMpt_cut200;

	if ( p4RecoJets.size() > 1 && p4RecoBtagJets_CSVM.size() > 3 ){
		for(unsigned int iii = 0; iii < p4RecoJets.size(); ++iii) {

			////// Step 2 - dijet mass vs sum pt of jets
			for(unsigned int iij = 0; iij < p4RecoJets.size(); ++iij) {
				if ( iii==iij ) continue;
				TLorentzVector p4RecoBtagJetsjj = p4RecoJets[iii] + p4RecoJets[iij];
				double sumRecoBtagJetsjjpt = p4RecoJets[iii].Pt() + p4RecoJets[iij].Pt();
				h_RecoBtagJetsjj_masspt->Fill( sumRecoBtagJetsjjpt, p4RecoBtagJetsjj.M() );
			}
			
			////// Step 3 - dijet mass vs sum pt of jets without bs from step 1
			if ( p4RecoBtagJets_CSVM_SmallMassDiff.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff[0] ) || ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff[1] ) || ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff[2] ) || ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff[3] ) )continue;
				p4jWORecoBtagJets_CSVM.push_back( p4RecoJets[iii] );
			}

			if ( p4RecoBtagJets_CSVM_SmallMassDiff_cut50.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut50[0] ) || ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut50[1] ) || ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut50[2] ) || ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut50[3] ) )continue;
				p4jWORecoBtagJets_CSVM_cut50.push_back( p4RecoJets[iii] );
			}

			if ( p4RecoBtagJets_CSVM_SmallMassDiff_cut100.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut100[0] ) || ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut100[1] ) || ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut100[2] ) || ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut100[3] ) )continue;
				p4jWORecoBtagJets_CSVM_cut100.push_back( p4RecoJets[iii] );
			}

			if ( p4RecoBtagJets_CSVM_SmallMassDiff_cut150.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut150[0] ) || ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut150[1] ) || ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut150[2] ) || ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut150[3] ) )continue;
				p4jWORecoBtagJets_CSVM_cut150.push_back( p4RecoJets[iii] );
			}

			if ( p4RecoBtagJets_CSVM_SmallMassDiff_cut200.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut200[0] ) || ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut200[1] ) || ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut200[2] ) || ( p4RecoJets[iii] == p4RecoBtagJets_CSVM_SmallMassDiff_cut200[3] ) )continue;
				p4jWORecoBtagJets_CSVM_cut200.push_back( p4RecoJets[iii] );
			}
		}
	}
			
	for(unsigned int iim = 0; iim < p4jWORecoBtagJets_CSVM.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecoBtagJets_CSVM.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecoBtagJets_CSVM[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecoBtagJets_CSVM = p4jWORecoBtagJets_CSVM[iim] + p4jWORecoBtagJets_CSVM[iin];
			double sumjjWORecoBtagJets_CSVMpt = p4jWORecoBtagJets_CSVM[iim].Pt() + p4jWORecoBtagJets_CSVM[iin].Pt();
			p4jjWORecoBtagJets_CSVM.push_back( tmpp4jjWORecoBtagJets_CSVM );
			scalarsumjjWORecoBtagJets_CSVMpt.push_back( sumjjWORecoBtagJets_CSVMpt );
			h_jjWORecoBtagJets_CSVM_masspt->Fill( sumjjWORecoBtagJets_CSVMpt, tmpp4jjWORecoBtagJets_CSVM.M() );
			h_massjjWORecoBtagJets_CSVM->Fill( tmpp4jjWORecoBtagJets_CSVM.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecoBtagJets_CSVM_cut50.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecoBtagJets_CSVM_cut50.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecoBtagJets_CSVM_cut50[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecoBtagJets_CSVM_cut50 = p4jWORecoBtagJets_CSVM_cut50[iim] + p4jWORecoBtagJets_CSVM_cut50[iin];
			double sumjjWORecoBtagJets_CSVMpt_cut50 = p4jWORecoBtagJets_CSVM_cut50[iim].Pt() + p4jWORecoBtagJets_CSVM_cut50[iin].Pt();
			p4jjWORecoBtagJets_CSVM_cut50.push_back( tmpp4jjWORecoBtagJets_CSVM_cut50 );
			scalarsumjjWORecoBtagJets_CSVMpt_cut50.push_back( sumjjWORecoBtagJets_CSVMpt_cut50 );
			h_jjWORecoBtagJets_CSVM_masspt_cut50->Fill( sumjjWORecoBtagJets_CSVMpt_cut50, tmpp4jjWORecoBtagJets_CSVM_cut50.M() );
			h_massjjWORecoBtagJets_CSVM_cut50->Fill( tmpp4jjWORecoBtagJets_CSVM_cut50.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecoBtagJets_CSVM_cut100.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecoBtagJets_CSVM_cut100.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecoBtagJets_CSVM_cut100[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecoBtagJets_CSVM_cut100 = p4jWORecoBtagJets_CSVM_cut100[iim] + p4jWORecoBtagJets_CSVM_cut100[iin];
			double sumjjWORecoBtagJets_CSVMpt_cut100 = p4jWORecoBtagJets_CSVM_cut100[iim].Pt() + p4jWORecoBtagJets_CSVM_cut100[iin].Pt();
			p4jjWORecoBtagJets_CSVM_cut100.push_back( tmpp4jjWORecoBtagJets_CSVM_cut100 );
			scalarsumjjWORecoBtagJets_CSVMpt_cut100.push_back( sumjjWORecoBtagJets_CSVMpt_cut100 );
			h_jjWORecoBtagJets_CSVM_masspt_cut100->Fill( sumjjWORecoBtagJets_CSVMpt_cut100, tmpp4jjWORecoBtagJets_CSVM_cut100.M() );
			h_massjjWORecoBtagJets_CSVM_cut100->Fill( tmpp4jjWORecoBtagJets_CSVM_cut100.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecoBtagJets_CSVM_cut150.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecoBtagJets_CSVM_cut150.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecoBtagJets_CSVM_cut150[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecoBtagJets_CSVM_cut150 = p4jWORecoBtagJets_CSVM_cut150[iim] + p4jWORecoBtagJets_CSVM_cut150[iin];
			double sumjjWORecoBtagJets_CSVMpt_cut150 = p4jWORecoBtagJets_CSVM_cut150[iim].Pt() + p4jWORecoBtagJets_CSVM_cut150[iin].Pt();
			p4jjWORecoBtagJets_CSVM_cut150.push_back( tmpp4jjWORecoBtagJets_CSVM_cut150 );
			scalarsumjjWORecoBtagJets_CSVMpt_cut150.push_back( sumjjWORecoBtagJets_CSVMpt_cut150 );
			h_jjWORecoBtagJets_CSVM_masspt_cut150->Fill( sumjjWORecoBtagJets_CSVMpt_cut150, tmpp4jjWORecoBtagJets_CSVM_cut150.M() );
			h_massjjWORecoBtagJets_CSVM_cut150->Fill( tmpp4jjWORecoBtagJets_CSVM_cut150.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecoBtagJets_CSVM_cut200.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecoBtagJets_CSVM_cut200.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecoBtagJets_CSVM_cut200[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecoBtagJets_CSVM_cut200 = p4jWORecoBtagJets_CSVM_cut200[iim] + p4jWORecoBtagJets_CSVM_cut200[iin];
			double sumjjWORecoBtagJets_CSVMpt_cut200 = p4jWORecoBtagJets_CSVM_cut200[iim].Pt() + p4jWORecoBtagJets_CSVM_cut200[iin].Pt();
			p4jjWORecoBtagJets_CSVM_cut200.push_back( tmpp4jjWORecoBtagJets_CSVM_cut200 );
			scalarsumjjWORecoBtagJets_CSVMpt_cut200.push_back( sumjjWORecoBtagJets_CSVMpt_cut200 );
			h_jjWORecoBtagJets_CSVM_masspt_cut200->Fill( sumjjWORecoBtagJets_CSVMpt_cut200, tmpp4jjWORecoBtagJets_CSVM_cut200.M() );
			h_massjjWORecoBtagJets_CSVM_cut200->Fill( tmpp4jjWORecoBtagJets_CSVM_cut200.M() );
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//////           STEP 4                                    /////
	//////  Mass of bb from 1 and jj from 3 vs sum pt of bbjj  /////
	////////////////////////////////////////////////////////////////
	
	/*////// Reco Jets
	for(unsigned int iiii = 0; iiii < p4RecobbSmallMassDiff.size(); ++iiii) {
		for(unsigned int iiij = 0; iiij < p4RecobbSmallMassDiff.size(); ++iiij) {
			if ( iiii == iiij ) continue;
			for(unsigned int iijj = 0; iijj < p4recojWObb.size(); ++iijj) {
				for(unsigned int ijjj = 0; ijjj < p4recojWObb.size(); ++ijjj) {
					if ( iijj == ijjj ) continue;
					TLorentzVector p4recobbjjMass = p4RecobbSmallMassDiff[iiii] + p4RecobbSmallMassDiff[iiij] + p4recojWObb[iijj] + p4recojWObb[ijjj];
					double sumrecobbjjPt = p4RecobbSmallMassDiff[iiii].Pt() + p4RecobbSmallMassDiff[iiij].Pt() + p4recojWObb[iijj].Pt() + p4recojWObb[ijjj].Pt();
					h_recobbjj_masspt->Fill( p4recobbjjMass.M(), sumrecobbjjPt );
				}
			}
		}
	}
	////////////////////////////
	
	/////// Match Jets
	for(unsigned int iiii = 0; iiii < p4bbSmallMassDiff.size(); ++iiii) {
		for(unsigned int iiij = 0; iiij < p4bbSmallMassDiff.size(); ++iiij) {
			if ( iiii == iiij ) continue;
			for(unsigned int iijj = 0; iijj < p4jWObb.size(); ++iijj) {
				for(unsigned int ijjj = 0; ijjj < p4jWObb.size(); ++ijjj) {
					if ( iijj == ijjj ) continue;
					TLorentzVector p4bbjjMass = p4bbSmallMassDiff[iiii] + p4bbSmallMassDiff[iiij] + p4jWObb[iijj] + p4jWObb[ijjj];
					double sumbbjjPt = p4bbSmallMassDiff[iiii].Pt() + p4bbSmallMassDiff[iiij].Pt() + p4jWObb[iijj].Pt() + p4jWObb[ijjj].Pt();
					h_bbjj_masspt->Fill( p4bbjjMass.M(), sumbbjjPt );
				}
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////*/
	

	////////////////////////////////////////////////////////////////
	//////           STEP 5                                    /////
	//////  Smallest mass bbjj                                 /////
	////////////////////////////////////////////////////////////////
	
	/*////// Reco Jets
	for(unsigned int jjjj = 0; jjjj < p4RecobbSmallMassDiff.size()-3; ++jjjj) {
		for(unsigned int jjjk = 1; jjjk < p4RecobbSmallMassDiff.size()-2; ++jjjk) {
			if ( jjjj == jjjk ) continue;
			for(unsigned int iijj = 0; iijj < p4recojWObb.size(); ++iijj) {
				for(unsigned int ijjj = 0; ijjj < p4recojWObb.size(); ++ijjj) {
					if ( iijj == ijjj ) continue;
					TLorentzVector p4recobbjjMass = p4RecobbSmallMassDiff[iiii] + p4RecobbSmallMassDiff[iiij] + p4recojWObb[iijj] + p4recojWObb[ijjj];
					double sumrecobbjjPt = p4RecobbSmallMassDiff[iiii].Pt() + p4RecobbSmallMassDiff[iiij].Pt() + p4recojWObb[iijj].Pt() + p4recojWObb[ijjj].Pt();
					h_massrecobbjj->Fill( p4recobbjjMass.M(), sumrecobbjjPt );
				}
			}
		}
	}
	///////////////////////////*/


}

//define this as a plug-in
DEFINE_FWK_MODULE(MyAnalyzer);
