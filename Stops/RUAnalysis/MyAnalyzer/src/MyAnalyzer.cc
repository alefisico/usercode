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

smallMassDiffVectors smallMassDiff( vector< TLorentzVector > p4StoreJets ){

	double tmpMassDiff = 9999999;
	Int_t bestMassIndex[4] = {-1, -1, -1, -1};
	vector< TLorentzVector > p4MatchBjetsSmallMassDiff;
	vector< double > massMatchBjets;
	vector< double > ptMatchBjets;
	vector< double > scalarPtMatchBjets;

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
						TLorentzVector candMassMatchBjets1 = p4StoreJets[ii]+p4StoreJets[ij];
						TLorentzVector candMassMatchBjets2 = p4StoreJets[ik]+p4StoreJets[il];
						double massDiff = abs( candMassMatchBjets1.M() - candMassMatchBjets2.M() );
						if( tmpMassDiff > massDiff ){
							tmpMassDiff = massDiff;
							bestMassIndex[0] = ii;
							bestMassIndex[1] = ij;
							bestMassIndex[2] = ik;
							bestMassIndex[3] = il;
							//cout << ii << " " << ij << " " << ik << " " << il << " " << massDiff << " " << candMassMatchBjets1.M() << " " << candMassMatchBjets2.M() << endl;
						}
					}
				}
			}
		}
		
		p4MatchBjetsSmallMassDiff.push_back( p4StoreJets[bestMassIndex[0]] );
		p4MatchBjetsSmallMassDiff.push_back( p4StoreJets[bestMassIndex[1]] );
		p4MatchBjetsSmallMassDiff.push_back( p4StoreJets[bestMassIndex[2]] );
		p4MatchBjetsSmallMassDiff.push_back( p4StoreJets[bestMassIndex[3]] );
		TLorentzVector tmpMass1 = p4StoreJets[bestMassIndex[0]] + p4StoreJets[bestMassIndex[1]];
		TLorentzVector tmpMass2 = p4StoreJets[bestMassIndex[2]] + p4StoreJets[bestMassIndex[3]];
		double scalarPtMatchBjets1 = p4StoreJets[bestMassIndex[0]].Pt() + p4StoreJets[bestMassIndex[1]].Pt();
		double scalarPtMatchBjets2 = p4StoreJets[bestMassIndex[2]].Pt() + p4StoreJets[bestMassIndex[3]].Pt();

		////// Store vectors
		massMatchBjets.push_back ( tmpMass1.M() );
		massMatchBjets.push_back ( tmpMass2.M() );
		ptMatchBjets.push_back ( tmpMass1.Pt() );
		ptMatchBjets.push_back ( tmpMass2.Pt() );
		scalarPtMatchBjets.push_back ( scalarPtMatchBjets1 );
		scalarPtMatchBjets.push_back ( scalarPtMatchBjets2 );
	}
	smallMassDiffVectors tmpVectors;
	tmpVectors.p4SmallMassDiff = p4MatchBjetsSmallMassDiff;
	tmpVectors.mass = massMatchBjets;
	tmpVectors.pt = ptMatchBjets;
	tmpVectors.scalarPt = scalarPtMatchBjets;


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
	vector< TLorentzVector > p4RecoBjetsCSVM;
	vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM;
	vector< double > RecoBjetsCSVM;

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
		bdiscCSV_PF = Jet->bDiscriminator("combinedSecondaryVertexBJetTags");
		if ( bdiscCSV_PF > 0.679  ) {
			p4RecoBjetsCSVM.push_back( p4Jets );
		}
		if ( abs( Jet->partonFlavour() ) != 5 ) continue;
		if ( bdiscCSV_PF < 0.679  ) continue;
		p4RecoPartonFlavorBjetsCSVM.push_back( p4Jets );
		RecoBjetsCSVM.push_back( bdiscCSV_PF );
	}
	std::sort(p4MatchTruthAlljets.begin(), p4MatchTruthAlljets.end(), ComparePt);
	std::sort(p4MatchTruthJets.begin(), p4MatchTruthJets.end(), ComparePt);
	std::sort(p4MatchTruthBjets.begin(), p4MatchTruthBjets.end(), ComparePt);
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	
	// Plot Reconstructed Mass/particles plots
	BasicPlots( p4GenTruthAlljets, p4GenTruthJets, p4GenTruthBjets, p4GenTruthBjetsHiggs, p4GenTruthB1Higgs, p4GenTruthB2Higgs, p4GenTruthJetsStop1, p4GenTruthBjetsStop1, p4MatchTruthJets,  p4MatchTruthBjets, p4MatchB1Higgs, p4MatchB2Higgs, p4RecoJets, p4RecoBjetsCSVM );
	Analysis( p4GenTruthB1Higgs, p4GenTruthB2Higgs, p4RecoJets, p4RecoBjetsCSVM, p4RecoPartonFlavorBjetsCSVM, p4MatchTruthBjets, p4MatchB1Higgs,  p4MatchB2Higgs, p4MatchTruthAlljets);

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
void MyAnalyzer::BasicPlots( vector< TLorentzVector > p4GenTruthAlljets, vector< TLorentzVector > p4GenTruthJets, vector< TLorentzVector > p4GenTruthBjets, vector< TLorentzVector > p4GenTruthBjetsHiggs, vector< TLorentzVector > p4GenTruthB1Higgs, vector< TLorentzVector > p4GenTruthB2Higgs, vector< TLorentzVector > p4GenTruthJetsStop1, vector< TLorentzVector > p4GenTruthBjetsStop1, vector< TLorentzVector > p4MatchTruthJets,  vector< TLorentzVector > p4MatchTruthBjets, vector< TLorentzVector > p4MatchB1Higgs, vector< TLorentzVector > p4MatchB2Higgs, vector< TLorentzVector > p4RecoJets , vector< TLorentzVector > p4RecoBjetsCSVM ) {

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
	if ( p4RecoBjetsCSVM.size() > 0 ){
		for(unsigned int k = 0; k < p4RecoBjetsCSVM.size(); ++k) {
			sumRecoBjetsPt += p4RecoBjetsCSVM[k].Pt();
			h_recoBjets_pt->Fill( p4RecoBjetsCSVM[k].Pt() );	
			h_recoBjets_eta->Fill( p4RecoBjetsCSVM[k].Eta() );
			h_recoBjets_phi->Fill( p4RecoBjetsCSVM[k].Phi() );
		}
	}
	h_recoBjets_num->Fill( p4RecoBjetsCSVM.size() );	
	h_recoBjets_sumpt->Fill( sumRecoBjetsPt );	
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
			h_matchBjets_pt->Fill( p4MatchTruthBjets[k].Pt() );	
			h_matchBjets_eta->Fill( p4MatchTruthBjets[k].Eta() );
			h_matchBjets_phi->Fill( p4MatchTruthBjets[k].Phi() );
		}
	}	
	h_matchBjets_num->Fill( p4MatchTruthBjets.size() );	
	h_matchBjets_sumpt->Fill( sumMatchTruthBjetsPt );	

	// Match All bjets from Higgs
	// HERE I am plotting ALL the Higgs in the sample, e.g., there are events with only one Higgs.
	double sumMatchB1HiggsPt = 0;
	if ( p4MatchB1Higgs.size() > 1 ){
		TLorentzVector MatchHiggs1Mass = p4MatchB1Higgs[0] + p4MatchB1Higgs[1];
		h_matchBjetsHiggs_mass->Fill( MatchHiggs1Mass.M() );	
		for(unsigned int k = 0; k < p4MatchB1Higgs.size(); ++k) {
			sumMatchB1HiggsPt += p4MatchB1Higgs[k].Pt();
			h_matchBjetsHiggs_pt->Fill( p4MatchB1Higgs[k].Pt() );	
			h_matchBjetsHiggs_eta->Fill( p4MatchB1Higgs[k].Eta() );
			h_matchBjetsHiggs_phi->Fill( p4MatchB1Higgs[k].Phi() );
		}
	}	
	h_matchBjetsHiggs_num->Fill( p4MatchB1Higgs.size() );	
	h_matchBjetsHiggs1_num->Fill( p4MatchB1Higgs.size() );	
	h_matchBjetsHiggs_sumpt->Fill( sumMatchB1HiggsPt );	
	double sumMatchB2HiggsPt = 0;
	if ( p4MatchB2Higgs.size() > 1 ){
		TLorentzVector MatchHiggs2Mass = p4MatchB2Higgs[0] + p4MatchB2Higgs[1];
		h_matchBjetsHiggs_mass->Fill( MatchHiggs2Mass.M() );	
		for(unsigned int k = 0; k < p4MatchB2Higgs.size(); ++k) {
			sumMatchB2HiggsPt += p4MatchB2Higgs[k].Pt();
			h_matchBjetsHiggs_pt->Fill( p4MatchB2Higgs[k].Pt() );	
			h_matchBjetsHiggs_eta->Fill( p4MatchB2Higgs[k].Eta() );
			h_matchBjetsHiggs_phi->Fill( p4MatchB2Higgs[k].Phi() );
		}
	}	
	h_matchBjetsHiggs_num->Fill( p4MatchB2Higgs.size() );	
	h_matchBjetsHiggs2_num->Fill( p4MatchB2Higgs.size() );	
	h_matchBjetsHiggs_sumpt->Fill( sumMatchB2HiggsPt );	
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
			h_genTruthBjets_pt->Fill( p4GenTruthBjets[k].Pt() );	
			h_genTruthBjets_eta->Fill( p4GenTruthBjets[k].Eta() );
			h_genTruthBjets_phi->Fill( p4GenTruthBjets[k].Phi() );
		}
	}
	h_genTruthBjets_num->Fill( p4GenTruthBjets.size() );	
	h_genTruthBjets_sumpt->Fill( sumGenTruthBjetsPt );	

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
		h_genTruthBjetsHiggs_mass->Fill( GenTruthHiggsMass.M() );	
	}	
	h_genTruthBjetsHiggs_num->Fill( p4GenTruthB1Higgs.size() );	
	if ( p4GenTruthB2Higgs.size() > 1 ){
		TLorentzVector GenTruthHiggsMass = p4GenTruthB2Higgs[0] + p4GenTruthB2Higgs[1];
		h_genTruthBjetsHiggs_mass->Fill( GenTruthHiggsMass.M() );	
	}	
	h_genTruthBjetsHiggs_num->Fill( p4GenTruthB2Higgs.size() );	

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
void MyAnalyzer::Analysis( vector< TLorentzVector > p4GenTruthB1Higgs, vector< TLorentzVector > p4GenTruthB2Higgs, vector< TLorentzVector > p4RecoJets, vector< TLorentzVector > p4RecoBjetsCSVM, vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM, vector< TLorentzVector > p4MatchTruthBjets,  vector< TLorentzVector > p4MatchB1Higgs,  vector< TLorentzVector > p4MatchB2Higgs, vector< TLorentzVector > p4MatchTruthAlljets){

	////////////////////////////////////////////////////////////////////
	//////       STEP 1                                         ////////
	//////  Best combination of bjets with smallest delta Mass  ////////
	////////////////////////////////////////////////////////////////////

	////////////////////////////  RecoBjets with Btagging
	
	//// Structure function smallMassDiff
	smallMassDiffVectors RecoBjetsCSVM_Vectors;
	RecoBjetsCSVM_Vectors = smallMassDiff( p4RecoBjetsCSVM );
	vector< double > massRecoBjetsCSVM = RecoBjetsCSVM_Vectors.mass;
	vector< double > ptRecoBjetsCSVM = RecoBjetsCSVM_Vectors.pt;
	vector< double > scalarPtRecoBjetsCSVM = RecoBjetsCSVM_Vectors.scalarPt;
	vector< TLorentzVector > p4RecoBjetsCSVM_SmallMassDiff = RecoBjetsCSVM_Vectors.p4SmallMassDiff;

	//// TLorentzVectors for cuts
	vector< TLorentzVector > p4RecoBjetsCSVM_SmallMassDiff_cut50;
	vector< TLorentzVector > p4RecoBjetsCSVM_SmallMassDiff_cut100;
	vector< TLorentzVector > p4RecoBjetsCSVM_SmallMassDiff_cut150;
	vector< TLorentzVector > p4RecoBjetsCSVM_SmallMassDiff_cut200;

	for(unsigned int tiii = 0; tiii < massRecoBjetsCSVM.size(); ++tiii) {
		///// Simple plots
		h_massRecoBjetsCSVM->Fill( massRecoBjetsCSVM[tiii] );
		h_PtRecoBjetsCSVM->Fill( ptRecoBjetsCSVM[tiii], massRecoBjetsCSVM[tiii] );
		h_scalarPtRecoBjetsCSVM->Fill( scalarPtRecoBjetsCSVM[tiii], massRecoBjetsCSVM[tiii] );

		///// Diagonal cuts
		double iDiag=(double)tiii*10.0+50.0;
		if( massRecoBjetsCSVM[tiii] < ( scalarPtRecoBjetsCSVM[tiii]-iDiag ) ) {
			h_scalarPtRecoBjetsCSVM_cut50->Fill( scalarPtRecoBjetsCSVM[tiii], massRecoBjetsCSVM[tiii] );
			h_massRecoBjetsCSVM_cut50->Fill( massRecoBjetsCSVM[tiii] );
			h_scalarPtRecoBjetsCSVM_cut50->Fill( scalarPtRecoBjetsCSVM[tiii], massRecoBjetsCSVM[tiii] );
			p4RecoBjetsCSVM_SmallMassDiff_cut50 = p4RecoBjetsCSVM_SmallMassDiff;
		}
							
		double iiDiag=(double)tiii*10.0+100.0;
		if( massRecoBjetsCSVM[tiii] < ( scalarPtRecoBjetsCSVM[tiii]-iiDiag ) ) {
			h_scalarPtRecoBjetsCSVM_cut100->Fill( scalarPtRecoBjetsCSVM[tiii], massRecoBjetsCSVM[tiii] );
			h_massRecoBjetsCSVM_cut100->Fill( massRecoBjetsCSVM[tiii] );
			h_scalarPtRecoBjetsCSVM_cut100->Fill( scalarPtRecoBjetsCSVM[tiii], massRecoBjetsCSVM[tiii] );
			p4RecoBjetsCSVM_SmallMassDiff_cut100 = p4RecoBjetsCSVM_SmallMassDiff;
		}
		
		double iiiDiag=(double)tiii*10.0+150.0;
		if( massRecoBjetsCSVM[tiii] < ( scalarPtRecoBjetsCSVM[tiii]-iiiDiag ) ) {
			h_scalarPtRecoBjetsCSVM_cut150->Fill( scalarPtRecoBjetsCSVM[tiii], massRecoBjetsCSVM[tiii] );
			h_massRecoBjetsCSVM_cut150->Fill( massRecoBjetsCSVM[tiii] );
			h_scalarPtRecoBjetsCSVM_cut150->Fill( scalarPtRecoBjetsCSVM[tiii], massRecoBjetsCSVM[tiii] );
			p4RecoBjetsCSVM_SmallMassDiff_cut150 = p4RecoBjetsCSVM_SmallMassDiff;
		}

		double ivDiag=(double)tiii*10.0+200.0;
		if( massRecoBjetsCSVM[tiii] < ( scalarPtRecoBjetsCSVM[tiii]-ivDiag ) ){
			h_scalarPtRecoBjetsCSVM_cut200->Fill( scalarPtRecoBjetsCSVM[tiii], massRecoBjetsCSVM[tiii] );
			h_massRecoBjetsCSVM_cut200->Fill( massRecoBjetsCSVM[tiii] );
			h_scalarPtRecoBjetsCSVM_cut200->Fill( scalarPtRecoBjetsCSVM[tiii], massRecoBjetsCSVM[tiii] );
			p4RecoBjetsCSVM_SmallMassDiff_cut200 = p4RecoBjetsCSVM_SmallMassDiff;
		}

	} 
	//////////////////////////////////////////////////////////

	///// PartonFlavor and  RecoBjets with Btagging
	
	//// Structure function smallMassDiff
	smallMassDiffVectors RecoPartonFlavorBjetsCSVM_Vectors;
	RecoPartonFlavorBjetsCSVM_Vectors = smallMassDiff( p4RecoPartonFlavorBjetsCSVM );
	vector< double > massRecoPartonFlavorBjetsCSVM = RecoPartonFlavorBjetsCSVM_Vectors.mass;
	vector< double > ptRecoPartonFlavorBjetsCSVM = RecoPartonFlavorBjetsCSVM_Vectors.pt;
	vector< double > scalarPtRecoPartonFlavorBjetsCSVM = RecoPartonFlavorBjetsCSVM_Vectors.scalarPt;
	vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM_SmallMassDiff = RecoPartonFlavorBjetsCSVM_Vectors.p4SmallMassDiff;

	//// TLorentzVectors for cuts
	vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut50;
	vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut100;
	vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut150;
	vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut200;

	for(unsigned int tiii = 0; tiii < massRecoPartonFlavorBjetsCSVM.size(); ++tiii) {
		///// Simple plots
		h_massRecoPartonFlavorBjetsCSVM->Fill( massRecoPartonFlavorBjetsCSVM[tiii] );
		h_PtRecoPartonFlavorBjetsCSVM->Fill( ptRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
		h_scalarPtRecoPartonFlavorBjetsCSVM->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );

		///// Diagonal cuts
		double iDiag=(double)tiii*10.0+50.0;
		if( massRecoPartonFlavorBjetsCSVM[tiii] < ( scalarPtRecoPartonFlavorBjetsCSVM[tiii]-iDiag ) ) {
			h_scalarPtRecoPartonFlavorBjetsCSVM_cut50->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			h_massRecoPartonFlavorBjetsCSVM_cut50->Fill( massRecoPartonFlavorBjetsCSVM[tiii] );
			h_scalarPtRecoPartonFlavorBjetsCSVM_cut50->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut50 = p4RecoPartonFlavorBjetsCSVM_SmallMassDiff;
		}
		
		double iiDiag=(double)tiii*10.0+100.0;
		if( massRecoPartonFlavorBjetsCSVM[tiii] < ( scalarPtRecoPartonFlavorBjetsCSVM[tiii]-iiDiag ) ) {
			h_scalarPtRecoPartonFlavorBjetsCSVM_cut100->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			h_massRecoPartonFlavorBjetsCSVM_cut100->Fill( massRecoPartonFlavorBjetsCSVM[tiii] );
			h_scalarPtRecoPartonFlavorBjetsCSVM_cut100->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut100 = p4RecoPartonFlavorBjetsCSVM_SmallMassDiff;
		}
		
		double iiiDiag=(double)tiii*10.0+150.0;
		if( massRecoPartonFlavorBjetsCSVM[tiii] < ( scalarPtRecoPartonFlavorBjetsCSVM[tiii]-iiiDiag ) ) {
			h_scalarPtRecoPartonFlavorBjetsCSVM_cut150->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			h_massRecoPartonFlavorBjetsCSVM_cut150->Fill( massRecoPartonFlavorBjetsCSVM[tiii] );
			h_scalarPtRecoPartonFlavorBjetsCSVM_cut150->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut150 = p4RecoPartonFlavorBjetsCSVM_SmallMassDiff;
		}

		double ivDiag=(double)tiii*10.0+200.0;
		if( massRecoPartonFlavorBjetsCSVM[tiii] < ( scalarPtRecoPartonFlavorBjetsCSVM[tiii]-ivDiag ) ) {
			h_scalarPtRecoPartonFlavorBjetsCSVM_cut200->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			h_massRecoPartonFlavorBjetsCSVM_cut200->Fill( massRecoPartonFlavorBjetsCSVM[tiii] );
			h_scalarPtRecoPartonFlavorBjetsCSVM_cut200->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut200 = p4RecoPartonFlavorBjetsCSVM_SmallMassDiff;
		}

	} 
	//////////////////////////////////////////////////////////

	////// MatchBjets
	
	//// Structure function smallMassDiff
	smallMassDiffVectors MatchBjetsVectors;
	MatchBjetsVectors = smallMassDiff( p4MatchTruthBjets );
	vector< double > massMatchBjets = MatchBjetsVectors.mass;
	vector< double > ptMatchBjets = MatchBjetsVectors.pt;
	vector< double > scalarPtMatchBjets = MatchBjetsVectors.scalarPt;
	vector< TLorentzVector > p4MatchBjetsSmallMassDiff = MatchBjetsVectors.p4SmallMassDiff;

	//// TLorentzVectors for cuts
	vector< TLorentzVector > p4MatchBjetsSmallMassDiff_cut50;
	vector< TLorentzVector > p4MatchBjetsSmallMassDiff_cut100;
	vector< TLorentzVector > p4MatchBjetsSmallMassDiff_cut150;
	vector< TLorentzVector > p4MatchBjetsSmallMassDiff_cut200;

	for(unsigned int ti = 0; ti < massMatchBjets.size(); ++ti) {

		///// Simple plots
		h_massMatchBjets->Fill( massMatchBjets[ti] );
		h_PtMatchBjets->Fill( ptMatchBjets[ti], massMatchBjets[ti] );
		h_scalarPtMatchBjets->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );

		//if ( massMatchBjets[ti] > 200 ) continue;

		///// Diagonal cuts
		double iDiag=(double)ti*10.0+50.0;
		if( massMatchBjets[ti] < ( scalarPtMatchBjets[ti]-iDiag ) ){
			h_scalarPtMatchBjets_cut50->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			h_massMatchBjets_cut50->Fill( massMatchBjets[ti] );
			h_scalarPtMatchBjets_cut50->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			p4MatchBjetsSmallMassDiff_cut50 = p4MatchBjetsSmallMassDiff;
		}

		double iiDiag=(double)ti*10.0+100.0;
		if( massMatchBjets[ti] < ( scalarPtMatchBjets[ti]-iiDiag ) ) {
			h_scalarPtMatchBjets_cut100->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			h_massMatchBjets_cut100->Fill( massMatchBjets[ti] );
			h_scalarPtMatchBjets_cut100->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			p4MatchBjetsSmallMassDiff_cut100 = p4MatchBjetsSmallMassDiff;
		}
	
		double iiiDiag=(double)ti*10.0+150.0;
		if( massMatchBjets[ti] < ( scalarPtMatchBjets[ti]-iiiDiag ) ) {
			h_scalarPtMatchBjets_cut150->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			h_massMatchBjets_cut150->Fill( massMatchBjets[ti] );
			h_scalarPtMatchBjets_cut150->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			p4MatchBjetsSmallMassDiff_cut150 = p4MatchBjetsSmallMassDiff;
		}

		double ivDiag=(double)ti*10.0+200.0;
		if( massMatchBjets[ti] < ( scalarPtMatchBjets[ti]-ivDiag ) ) {
			h_scalarPtMatchBjets_cut200->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			h_massMatchBjets_cut200->Fill( massMatchBjets[ti] );
			h_scalarPtMatchBjets_cut200->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			p4MatchBjetsSmallMassDiff_cut200 = p4MatchBjetsSmallMassDiff;
		}
	}
	//cout << p4MatchBjetsSmallMassDiff.size() << " " << p4MatchBjetsSmallMassDiff_cut50.size() << " " << p4MatchBjetsSmallMassDiff_cut100.size() << " " << p4MatchBjetsSmallMassDiff_cut150.size() << " " << p4MatchBjetsSmallMassDiff_cut200.size() << " " << endl;
	///////////////////////////////////////////////////////////////////

	// MatchBjets from Higgs
	vector< TLorentzVector > p4MatchBjetsHiggsSmallMassDiff;
	vector< TLorentzVector > p4MatchBjetsHiggsSmallMassDiff_cut50;
	vector< TLorentzVector > p4MatchBjetsHiggsSmallMassDiff_cut100;
	vector< TLorentzVector > p4MatchBjetsHiggsSmallMassDiff_cut150;
	vector< TLorentzVector > p4MatchBjetsHiggsSmallMassDiff_cut200;
	vector< double > massMatchBjetsHiggs;
	vector< double > ptMatchBjetsHiggs;
	vector< double > scalarPtMatchBjetsHiggs;

	if ( p4MatchB1Higgs.size() == 2 && p4MatchB2Higgs.size() == 2 ){
		
		TLorentzVector tmpMassMatchBjetsHiggs1 = p4MatchB1Higgs[0] + p4MatchB1Higgs[1];
		TLorentzVector tmpMassMatchBjetsHiggs2 = p4MatchB2Higgs[0] + p4MatchB2Higgs[1];
		double scalarPtMatchBjetsHiggs1 = p4MatchB1Higgs[0].Pt() + p4MatchB1Higgs[1].Pt();
		double scalarPtMatchBjetsHiggs2 = p4MatchB2Higgs[0].Pt() + p4MatchB2Higgs[1].Pt();
		p4MatchBjetsHiggsSmallMassDiff.push_back( p4MatchB1Higgs[0] );
		p4MatchBjetsHiggsSmallMassDiff.push_back( p4MatchB1Higgs[1] );
		p4MatchBjetsHiggsSmallMassDiff.push_back( p4MatchB2Higgs[0] );
		p4MatchBjetsHiggsSmallMassDiff.push_back( p4MatchB2Higgs[1] );

		////// Store vectors
		massMatchBjetsHiggs.push_back ( tmpMassMatchBjetsHiggs1.M() );
		massMatchBjetsHiggs.push_back ( tmpMassMatchBjetsHiggs2.M() );
		ptMatchBjetsHiggs.push_back ( tmpMassMatchBjetsHiggs1.Pt() );
		ptMatchBjetsHiggs.push_back ( tmpMassMatchBjetsHiggs2.Pt() );
		scalarPtMatchBjetsHiggs.push_back ( scalarPtMatchBjetsHiggs1 );
		scalarPtMatchBjetsHiggs.push_back ( scalarPtMatchBjetsHiggs2 );
	}

	h_massMatchBjetsHiggs_num->Fill( massMatchBjetsHiggs.size() );	/// test number of MatchBjetsHiggs
	for(unsigned int tiv = 0; tiv < massMatchBjetsHiggs.size(); ++tiv) {
		///// Simple plots
		h_massMatchBjetsHiggs->Fill( massMatchBjetsHiggs[tiv] );
		h_PtMatchBjetsHiggs->Fill( ptMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
		h_scalarPtMatchBjetsHiggs->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );

		///// Diagonal cuts
		double iDiag=(double)tiv*10.0+50.0;
		if( massMatchBjetsHiggs[tiv] < ( scalarPtMatchBjetsHiggs[tiv]-iDiag ) ) {
			h_scalarPtMatchBjetsHiggs_cut50->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			h_massMatchBjetsHiggs_cut50->Fill( massMatchBjetsHiggs[tiv] );
			h_scalarPtMatchBjetsHiggs_cut50->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			p4MatchBjetsHiggsSmallMassDiff_cut50.push_back( p4MatchB1Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_cut50.push_back( p4MatchB1Higgs[1] );
			p4MatchBjetsHiggsSmallMassDiff_cut50.push_back( p4MatchB2Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_cut50.push_back( p4MatchB2Higgs[1] );
		}

		double iiDiag=(double)tiv*10.0+100.0;
		if( massMatchBjetsHiggs[tiv] < ( scalarPtMatchBjetsHiggs[tiv]-iiDiag ) ){
			h_scalarPtMatchBjetsHiggs_cut100->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			h_massMatchBjetsHiggs_cut100->Fill( massMatchBjetsHiggs[tiv] );
			h_scalarPtMatchBjetsHiggs_cut100->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			p4MatchBjetsHiggsSmallMassDiff_cut100.push_back( p4MatchB1Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_cut100.push_back( p4MatchB1Higgs[1] );
			p4MatchBjetsHiggsSmallMassDiff_cut100.push_back( p4MatchB2Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_cut100.push_back( p4MatchB2Higgs[1] );
		}

		double iiiDiag=(double)tiv*10.0+150.0;
		if( massMatchBjetsHiggs[tiv] < ( scalarPtMatchBjetsHiggs[tiv]-iiiDiag ) ) {
			h_scalarPtMatchBjetsHiggs_cut150->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			h_massMatchBjetsHiggs_cut150->Fill( massMatchBjetsHiggs[tiv] );
			h_scalarPtMatchBjetsHiggs_cut150->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			p4MatchBjetsHiggsSmallMassDiff_cut150.push_back( p4MatchB1Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_cut150.push_back( p4MatchB1Higgs[1] );
			p4MatchBjetsHiggsSmallMassDiff_cut150.push_back( p4MatchB2Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_cut150.push_back( p4MatchB2Higgs[1] );
		}

		double ivDiag=(double)tiv*10.0+200.0;
		if( massMatchBjetsHiggs[tiv] < ( scalarPtMatchBjetsHiggs[tiv]-ivDiag ) ) {
			h_scalarPtMatchBjetsHiggs_cut200->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			h_massMatchBjetsHiggs_cut200->Fill( massMatchBjetsHiggs[tiv] );
			h_scalarPtMatchBjetsHiggs_cut200->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			p4MatchBjetsHiggsSmallMassDiff_cut200.push_back( p4MatchB1Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_cut200.push_back( p4MatchB1Higgs[1] );
			p4MatchBjetsHiggsSmallMassDiff_cut200.push_back( p4MatchB2Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_cut200.push_back( p4MatchB2Higgs[1] );
		}

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
	vector< TLorentzVector > p4MatchjjWOMatchBjets;
	vector< TLorentzVector > p4MatchjjWOMatchBjets_cut50;
	vector< TLorentzVector > p4MatchjjWOMatchBjets_cut100;
	vector< TLorentzVector > p4MatchjjWOMatchBjets_cut150;
	vector< TLorentzVector > p4MatchjjWOMatchBjets_cut200;
	vector< TLorentzVector > p4jWOMatchBjets;
	vector< TLorentzVector > p4jWOMatchBjets_cut50;
	vector< TLorentzVector > p4jWOMatchBjets_cut100;
	vector< TLorentzVector > p4jWOMatchBjets_cut150;
	vector< TLorentzVector > p4jWOMatchBjets_cut200;
	vector< double > scalarsumMatchjjWOMatchBjetspt;
	vector< double > scalarsumMatchjjWOMatchBjetspt_cut50;
	vector< double > scalarsumMatchjjWOMatchBjetspt_cut100;
	vector< double > scalarsumMatchjjWOMatchBjetspt_cut150;
	vector< double > scalarsumMatchjjWOMatchBjetspt_cut200;

	if ( p4MatchTruthAlljets.size() > 5 && p4MatchTruthBjets.size() > 3 ){
		for(unsigned int iii = 0; iii < p4MatchTruthAlljets.size(); ++iii) {

			////// Step 2 - dijet mass vs sum pt of jets
			for(unsigned int iij = 0; iij < p4MatchTruthAlljets.size(); ++iij) {
				if ( iii==iij ) continue;
				TLorentzVector p4jj = p4MatchTruthAlljets[iii] + p4MatchTruthAlljets[iij];
				double sumjjpt = p4MatchTruthAlljets[iii].Pt() + p4MatchTruthAlljets[iij].Pt();
				h_matchjj_masspt->Fill( sumjjpt, p4jj.M() );
			}
			
			////// Step 3 - dijet mass vs sum pt of jets without bs from step 1
			//cout << "1 " << iii << " " << p4MatchTruthAlljets[iii].Pt() << " " << p4MatchTruthAlljets.size() << endl;
			if ( p4MatchBjetsSmallMassDiff.size() == 4 ){
				//cout << p4MatchBjetsSmallMassDiff[0].Pt() << " " << p4MatchBjetsSmallMassDiff[1].Pt() << " " << p4MatchBjetsSmallMassDiff[2].Pt() << " " << p4MatchBjetsSmallMassDiff[3].Pt() << endl; 
				if ( ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff[0] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff[1] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff[2] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff[3] ) ) continue;
				//cout << "3 " << iii << " " << p4MatchTruthAlljets[iii].Pt() << endl;
				p4jWOMatchBjets.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4MatchBjetsSmallMassDiff_cut50.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut50[0] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut50[1] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut50[2] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut50[3] ) )continue;
				p4jWOMatchBjets_cut50.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4MatchBjetsSmallMassDiff_cut100.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut100[0] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut100[1] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut100[2] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut100[3] ) )continue;
				p4jWOMatchBjets_cut100.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4MatchBjetsSmallMassDiff_cut150.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut150[0] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut150[1] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut150[2] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut150[3] ) )continue;
				p4jWOMatchBjets_cut150.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4MatchBjetsSmallMassDiff_cut200.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut200[0] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut200[1] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut200[2] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsSmallMassDiff_cut200[3] ) )continue;
				p4jWOMatchBjets_cut200.push_back( p4MatchTruthAlljets[iii] );
			}
		}
	}
	//cout << " 10 " << p4jWOMatchBjets.size() << " " << p4jWOMatchBjets_cut50.size() << " " << p4jWOMatchBjets_cut100.size() << " " << p4jWOMatchBjets_cut150.size() << " " << p4jWOMatchBjets_cut200.size() << " " << endl;
			
	for(unsigned int iim = 0; iim < p4jWOMatchBjets.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWOMatchBjets.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWOMatchBjets[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWOMatchBjets = p4jWOMatchBjets[iim] + p4jWOMatchBjets[iin];
			double sumMatchjjWOMatchBjetspt = p4jWOMatchBjets[iim].Pt() + p4jWOMatchBjets[iin].Pt();
			p4MatchjjWOMatchBjets.push_back( tmpp4MatchjjWOMatchBjets );
			scalarsumMatchjjWOMatchBjetspt.push_back( sumMatchjjWOMatchBjetspt );
			h_matchjjWOMatchBjets_masspt->Fill( sumMatchjjWOMatchBjetspt, tmpp4MatchjjWOMatchBjets.M() );
			h_massmatchjjWOMatchBjets->Fill( tmpp4MatchjjWOMatchBjets.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWOMatchBjets_cut50.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWOMatchBjets_cut50.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWOMatchBjets_cut50[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWOMatchBjets_cut50 = p4jWOMatchBjets_cut50[iim] + p4jWOMatchBjets_cut50[iin];
			double sumMatchjjWOMatchBjetspt_cut50 = p4jWOMatchBjets_cut50[iim].Pt() + p4jWOMatchBjets_cut50[iin].Pt();
			p4MatchjjWOMatchBjets_cut50.push_back( tmpp4MatchjjWOMatchBjets_cut50 );
			scalarsumMatchjjWOMatchBjetspt_cut50.push_back( sumMatchjjWOMatchBjetspt_cut50 );
			h_matchjjWOMatchBjets_masspt_cut50->Fill( sumMatchjjWOMatchBjetspt_cut50, tmpp4MatchjjWOMatchBjets_cut50.M() );
			h_massmatchjjWOMatchBjets_cut50->Fill( tmpp4MatchjjWOMatchBjets_cut50.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWOMatchBjets_cut100.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWOMatchBjets_cut100.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWOMatchBjets_cut100[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWOMatchBjets_cut100 = p4jWOMatchBjets_cut100[iim] + p4jWOMatchBjets_cut100[iin];
			double sumMatchjjWOMatchBjetspt_cut100 = p4jWOMatchBjets_cut100[iim].Pt() + p4jWOMatchBjets_cut100[iin].Pt();
			p4MatchjjWOMatchBjets_cut100.push_back( tmpp4MatchjjWOMatchBjets_cut100 );
			scalarsumMatchjjWOMatchBjetspt_cut100.push_back( sumMatchjjWOMatchBjetspt_cut100 );
			h_matchjjWOMatchBjets_masspt_cut100->Fill( sumMatchjjWOMatchBjetspt_cut100, tmpp4MatchjjWOMatchBjets_cut100.M() );
			h_massmatchjjWOMatchBjets_cut100->Fill( tmpp4MatchjjWOMatchBjets_cut100.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWOMatchBjets_cut150.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWOMatchBjets_cut150.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWOMatchBjets_cut150[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWOMatchBjets_cut150 = p4jWOMatchBjets_cut150[iim] + p4jWOMatchBjets_cut150[iin];
			double sumMatchjjWOMatchBjetspt_cut150 = p4jWOMatchBjets_cut150[iim].Pt() + p4jWOMatchBjets_cut150[iin].Pt();
			p4MatchjjWOMatchBjets_cut150.push_back( tmpp4MatchjjWOMatchBjets_cut150 );
			scalarsumMatchjjWOMatchBjetspt_cut150.push_back( sumMatchjjWOMatchBjetspt_cut150 );
			h_matchjjWOMatchBjets_masspt_cut150->Fill( sumMatchjjWOMatchBjetspt_cut150, tmpp4MatchjjWOMatchBjets_cut150.M() );
			h_massmatchjjWOMatchBjets_cut150->Fill( tmpp4MatchjjWOMatchBjets_cut150.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWOMatchBjets_cut200.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWOMatchBjets_cut200.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWOMatchBjets_cut200[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWOMatchBjets_cut200 = p4jWOMatchBjets_cut200[iim] + p4jWOMatchBjets_cut200[iin];
			double sumMatchjjWOMatchBjetspt_cut200 = p4jWOMatchBjets_cut200[iim].Pt() + p4jWOMatchBjets_cut200[iin].Pt();
			p4MatchjjWOMatchBjets_cut200.push_back( tmpp4MatchjjWOMatchBjets_cut200 );
			scalarsumMatchjjWOMatchBjetspt_cut200.push_back( sumMatchjjWOMatchBjetspt_cut200 );
			h_matchjjWOMatchBjets_masspt_cut200->Fill( sumMatchjjWOMatchBjetspt_cut200, tmpp4MatchjjWOMatchBjets_cut200.M() );
			h_massmatchjjWOMatchBjets_cut200->Fill( tmpp4MatchjjWOMatchBjets_cut200.M() );
		}
	}
	//////////////////////////////////////////////////////////////////////////////////


	///// Match jets from Higgs
	vector< TLorentzVector > p4MatchjjWOMatchBjetsHiggs;
	vector< TLorentzVector > p4MatchjjWOMatchBjetsHiggs_cut50;
	vector< TLorentzVector > p4MatchjjWOMatchBjetsHiggs_cut100;
	vector< TLorentzVector > p4MatchjjWOMatchBjetsHiggs_cut150;
	vector< TLorentzVector > p4MatchjjWOMatchBjetsHiggs_cut200;
	vector< TLorentzVector > p4jWOMatchBjetsHiggs;
	vector< TLorentzVector > p4jWOMatchBjetsHiggs_cut50;
	vector< TLorentzVector > p4jWOMatchBjetsHiggs_cut100;
	vector< TLorentzVector > p4jWOMatchBjetsHiggs_cut150;
	vector< TLorentzVector > p4jWOMatchBjetsHiggs_cut200;
	vector< double > scalarsumMatchjjWOMatchBjetsHiggspt;
	vector< double > scalarsumMatchjjWOMatchBjetsHiggspt_cut50;
	vector< double > scalarsumMatchjjWOMatchBjetsHiggspt_cut100;
	vector< double > scalarsumMatchjjWOMatchBjetsHiggspt_cut150;
	vector< double > scalarsumMatchjjWOMatchBjetsHiggspt_cut200;

	if ( p4MatchTruthAlljets.size() > 5 && p4MatchB1Higgs.size() == 2 && p4MatchB2Higgs.size() == 2 ){
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
			if ( p4MatchBjetsHiggsSmallMassDiff.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff[0] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff[1] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff[2] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff[3] ) )continue;
				//cout << "3 " << iii << " " << p4MatchTruthAlljets[iii].Pt() << endl;
				p4jWOMatchBjetsHiggs.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4MatchBjetsHiggsSmallMassDiff_cut50.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut50[0] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut50[1] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut50[2] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut50[3] ) )continue;
				p4jWOMatchBjetsHiggs_cut50.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4MatchBjetsHiggsSmallMassDiff_cut100.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut100[0] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut100[1] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut100[2] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut100[3] ) )continue;
				p4jWOMatchBjetsHiggs_cut100.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4MatchBjetsHiggsSmallMassDiff_cut150.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut150[0] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut150[1] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut150[2] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut150[3] ) )continue;
				p4jWOMatchBjetsHiggs_cut150.push_back( p4MatchTruthAlljets[iii] );
			}

			if ( p4MatchBjetsHiggsSmallMassDiff_cut200.size() == 4 ){
				if ( ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut200[0] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut200[1] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut200[2] ) || ( p4MatchTruthAlljets[iii] == p4MatchBjetsHiggsSmallMassDiff_cut200[3] ) )continue;
				p4jWOMatchBjetsHiggs_cut200.push_back( p4MatchTruthAlljets[iii] );
			}
		}
	}
			
	for(unsigned int iim = 0; iim < p4jWOMatchBjetsHiggs.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWOMatchBjetsHiggs.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWOMatchBjetsHiggs[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWOMatchBjetsHiggs = p4jWOMatchBjetsHiggs[iim] + p4jWOMatchBjetsHiggs[iin];
			double sumMatchjjWOMatchBjetsHiggspt = p4jWOMatchBjetsHiggs[iim].Pt() + p4jWOMatchBjetsHiggs[iin].Pt();
			p4MatchjjWOMatchBjetsHiggs.push_back( tmpp4MatchjjWOMatchBjetsHiggs );
			scalarsumMatchjjWOMatchBjetsHiggspt.push_back( sumMatchjjWOMatchBjetsHiggspt );
			h_matchjjWOMatchBjetsHiggs_masspt->Fill( sumMatchjjWOMatchBjetsHiggspt, tmpp4MatchjjWOMatchBjetsHiggs.M() );
			h_massmatchjjWOMatchBjetsHiggs->Fill( tmpp4MatchjjWOMatchBjetsHiggs.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWOMatchBjetsHiggs_cut50.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWOMatchBjetsHiggs_cut50.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWOMatchBjetsHiggs_cut50[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWOMatchBjetsHiggs_cut50 = p4jWOMatchBjetsHiggs_cut50[iim] + p4jWOMatchBjetsHiggs_cut50[iin];
			double sumMatchjjWOMatchBjetsHiggspt_cut50 = p4jWOMatchBjetsHiggs_cut50[iim].Pt() + p4jWOMatchBjetsHiggs_cut50[iin].Pt();
			p4MatchjjWOMatchBjetsHiggs_cut50.push_back( tmpp4MatchjjWOMatchBjetsHiggs_cut50 );
			scalarsumMatchjjWOMatchBjetsHiggspt_cut50.push_back( sumMatchjjWOMatchBjetsHiggspt_cut50 );
			h_matchjjWOMatchBjetsHiggs_masspt_cut50->Fill( sumMatchjjWOMatchBjetsHiggspt_cut50, tmpp4MatchjjWOMatchBjetsHiggs_cut50.M() );
			h_massmatchjjWOMatchBjetsHiggs_cut50->Fill( tmpp4MatchjjWOMatchBjetsHiggs_cut50.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWOMatchBjetsHiggs_cut100.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWOMatchBjetsHiggs_cut100.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWOMatchBjetsHiggs_cut100[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWOMatchBjetsHiggs_cut100 = p4jWOMatchBjetsHiggs_cut100[iim] + p4jWOMatchBjetsHiggs_cut100[iin];
			double sumMatchjjWOMatchBjetsHiggspt_cut100 = p4jWOMatchBjetsHiggs_cut100[iim].Pt() + p4jWOMatchBjetsHiggs_cut100[iin].Pt();
			p4MatchjjWOMatchBjetsHiggs_cut100.push_back( tmpp4MatchjjWOMatchBjetsHiggs_cut100 );
			scalarsumMatchjjWOMatchBjetsHiggspt_cut100.push_back( sumMatchjjWOMatchBjetsHiggspt_cut100 );
			h_matchjjWOMatchBjetsHiggs_masspt_cut100->Fill( sumMatchjjWOMatchBjetsHiggspt_cut100, tmpp4MatchjjWOMatchBjetsHiggs_cut100.M() );
			h_massmatchjjWOMatchBjetsHiggs_cut100->Fill( tmpp4MatchjjWOMatchBjetsHiggs_cut100.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWOMatchBjetsHiggs_cut150.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWOMatchBjetsHiggs_cut150.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWOMatchBjetsHiggs_cut150[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWOMatchBjetsHiggs_cut150 = p4jWOMatchBjetsHiggs_cut150[iim] + p4jWOMatchBjetsHiggs_cut150[iin];
			double sumMatchjjWOMatchBjetsHiggspt_cut150 = p4jWOMatchBjetsHiggs_cut150[iim].Pt() + p4jWOMatchBjetsHiggs_cut150[iin].Pt();
			p4MatchjjWOMatchBjetsHiggs_cut150.push_back( tmpp4MatchjjWOMatchBjetsHiggs_cut150 );
			scalarsumMatchjjWOMatchBjetsHiggspt_cut150.push_back( sumMatchjjWOMatchBjetsHiggspt_cut150 );
			h_matchjjWOMatchBjetsHiggs_masspt_cut150->Fill( sumMatchjjWOMatchBjetsHiggspt_cut150, tmpp4MatchjjWOMatchBjetsHiggs_cut150.M() );
			h_massmatchjjWOMatchBjetsHiggs_cut150->Fill( tmpp4MatchjjWOMatchBjetsHiggs_cut150.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWOMatchBjetsHiggs_cut200.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWOMatchBjetsHiggs_cut200.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWOMatchBjetsHiggs_cut200[iin].Pt() << endl;
			TLorentzVector tmpp4MatchjjWOMatchBjetsHiggs_cut200 = p4jWOMatchBjetsHiggs_cut200[iim] + p4jWOMatchBjetsHiggs_cut200[iin];
			double sumMatchjjWOMatchBjetsHiggspt_cut200 = p4jWOMatchBjetsHiggs_cut200[iim].Pt() + p4jWOMatchBjetsHiggs_cut200[iin].Pt();
			p4MatchjjWOMatchBjetsHiggs_cut200.push_back( tmpp4MatchjjWOMatchBjetsHiggs_cut200 );
			scalarsumMatchjjWOMatchBjetsHiggspt_cut200.push_back( sumMatchjjWOMatchBjetsHiggspt_cut200 );
			h_matchjjWOMatchBjetsHiggs_masspt_cut200->Fill( sumMatchjjWOMatchBjetsHiggspt_cut200, tmpp4MatchjjWOMatchBjetsHiggs_cut200.M() );
			h_massmatchjjWOMatchBjetsHiggs_cut200->Fill( tmpp4MatchjjWOMatchBjetsHiggs_cut200.M() );
		}
	}
	///////////////////////////////////////////////////////////////////////


	///////// RECO jets + Btagging CSVM
	vector< TLorentzVector > p4jjWORecoBjetsCSVM;
	vector< TLorentzVector > p4jjWORecoBjetsCSVM_cut50;
	vector< TLorentzVector > p4jjWORecoBjetsCSVM_cut100;
	vector< TLorentzVector > p4jjWORecoBjetsCSVM_cut150;
	vector< TLorentzVector > p4jjWORecoBjetsCSVM_cut200;
	vector< TLorentzVector > p4jWORecoBjetsCSVM;
	vector< TLorentzVector > p4jWORecoBjetsCSVM_cut50;
	vector< TLorentzVector > p4jWORecoBjetsCSVM_cut100;
	vector< TLorentzVector > p4jWORecoBjetsCSVM_cut150;
	vector< TLorentzVector > p4jWORecoBjetsCSVM_cut200;
	vector< double > scalarsumjjWORecoBjetsCSVMpt;
	vector< double > scalarsumjjWORecoBjetsCSVMpt_cut50;
	vector< double > scalarsumjjWORecoBjetsCSVMpt_cut100;
	vector< double > scalarsumjjWORecoBjetsCSVMpt_cut150;
	vector< double > scalarsumjjWORecoBjetsCSVMpt_cut200;

	if ( p4RecoJets.size() > 5 && p4RecoBjetsCSVM.size() > 3 ){
		for(unsigned int iii = 0; iii < p4RecoJets.size(); ++iii) {

			////// Step 2 - dijet mass vs sum pt of jets
			for(unsigned int iij = 0; iij < p4RecoJets.size(); ++iij) {
				if ( iii==iij ) continue;
				TLorentzVector p4RecoBtagJetsjj = p4RecoJets[iii] + p4RecoJets[iij];
				double sumRecoBtagJetsjjpt = p4RecoJets[iii].Pt() + p4RecoJets[iij].Pt();
				h_RecoBtagJetsjj_masspt->Fill( sumRecoBtagJetsjjpt, p4RecoBtagJetsjj.M() );
			}
			
			////// Step 3 - dijet mass vs sum pt of jets without bs from step 1
			if ( p4RecoBjetsCSVM_SmallMassDiff.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff[0] ) || ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff[1] ) || ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff[2] ) || ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff[3] ) )continue;
				p4jWORecoBjetsCSVM.push_back( p4RecoJets[iii] );
			}

			if ( p4RecoBjetsCSVM_SmallMassDiff_cut50.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut50[0] ) || ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut50[1] ) || ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut50[2] ) || ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut50[3] ) )continue;
				p4jWORecoBjetsCSVM_cut50.push_back( p4RecoJets[iii] );
			}

			if ( p4RecoBjetsCSVM_SmallMassDiff_cut100.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut100[0] ) || ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut100[1] ) || ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut100[2] ) || ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut100[3] ) )continue;
				p4jWORecoBjetsCSVM_cut100.push_back( p4RecoJets[iii] );
			}

			if ( p4RecoBjetsCSVM_SmallMassDiff_cut150.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut150[0] ) || ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut150[1] ) || ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut150[2] ) || ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut150[3] ) )continue;
				p4jWORecoBjetsCSVM_cut150.push_back( p4RecoJets[iii] );
			}

			if ( p4RecoBjetsCSVM_SmallMassDiff_cut200.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut200[0] ) || ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut200[1] ) || ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut200[2] ) || ( p4RecoJets[iii] == p4RecoBjetsCSVM_SmallMassDiff_cut200[3] ) )continue;
				p4jWORecoBjetsCSVM_cut200.push_back( p4RecoJets[iii] );
			}
		}
	}
			
	for(unsigned int iim = 0; iim < p4jWORecoBjetsCSVM.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecoBjetsCSVM.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecoBjetsCSVM[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecoBjetsCSVM = p4jWORecoBjetsCSVM[iim] + p4jWORecoBjetsCSVM[iin];
			double sumjjWORecoBjetsCSVMpt = p4jWORecoBjetsCSVM[iim].Pt() + p4jWORecoBjetsCSVM[iin].Pt();
			p4jjWORecoBjetsCSVM.push_back( tmpp4jjWORecoBjetsCSVM );
			scalarsumjjWORecoBjetsCSVMpt.push_back( sumjjWORecoBjetsCSVMpt );
			h_jjWORecoBjetsCSVM_masspt->Fill( sumjjWORecoBjetsCSVMpt, tmpp4jjWORecoBjetsCSVM.M() );
			h_massjjWORecoBjetsCSVM->Fill( tmpp4jjWORecoBjetsCSVM.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecoBjetsCSVM_cut50.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecoBjetsCSVM_cut50.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecoBjetsCSVM_cut50[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecoBjetsCSVM_cut50 = p4jWORecoBjetsCSVM_cut50[iim] + p4jWORecoBjetsCSVM_cut50[iin];
			double sumjjWORecoBjetsCSVMpt_cut50 = p4jWORecoBjetsCSVM_cut50[iim].Pt() + p4jWORecoBjetsCSVM_cut50[iin].Pt();
			p4jjWORecoBjetsCSVM_cut50.push_back( tmpp4jjWORecoBjetsCSVM_cut50 );
			scalarsumjjWORecoBjetsCSVMpt_cut50.push_back( sumjjWORecoBjetsCSVMpt_cut50 );
			h_jjWORecoBjetsCSVM_masspt_cut50->Fill( sumjjWORecoBjetsCSVMpt_cut50, tmpp4jjWORecoBjetsCSVM_cut50.M() );
			h_massjjWORecoBjetsCSVM_cut50->Fill( tmpp4jjWORecoBjetsCSVM_cut50.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecoBjetsCSVM_cut100.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecoBjetsCSVM_cut100.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecoBjetsCSVM_cut100[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecoBjetsCSVM_cut100 = p4jWORecoBjetsCSVM_cut100[iim] + p4jWORecoBjetsCSVM_cut100[iin];
			double sumjjWORecoBjetsCSVMpt_cut100 = p4jWORecoBjetsCSVM_cut100[iim].Pt() + p4jWORecoBjetsCSVM_cut100[iin].Pt();
			p4jjWORecoBjetsCSVM_cut100.push_back( tmpp4jjWORecoBjetsCSVM_cut100 );
			scalarsumjjWORecoBjetsCSVMpt_cut100.push_back( sumjjWORecoBjetsCSVMpt_cut100 );
			h_jjWORecoBjetsCSVM_masspt_cut100->Fill( sumjjWORecoBjetsCSVMpt_cut100, tmpp4jjWORecoBjetsCSVM_cut100.M() );
			h_massjjWORecoBjetsCSVM_cut100->Fill( tmpp4jjWORecoBjetsCSVM_cut100.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecoBjetsCSVM_cut150.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecoBjetsCSVM_cut150.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecoBjetsCSVM_cut150[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecoBjetsCSVM_cut150 = p4jWORecoBjetsCSVM_cut150[iim] + p4jWORecoBjetsCSVM_cut150[iin];
			double sumjjWORecoBjetsCSVMpt_cut150 = p4jWORecoBjetsCSVM_cut150[iim].Pt() + p4jWORecoBjetsCSVM_cut150[iin].Pt();
			p4jjWORecoBjetsCSVM_cut150.push_back( tmpp4jjWORecoBjetsCSVM_cut150 );
			scalarsumjjWORecoBjetsCSVMpt_cut150.push_back( sumjjWORecoBjetsCSVMpt_cut150 );
			h_jjWORecoBjetsCSVM_masspt_cut150->Fill( sumjjWORecoBjetsCSVMpt_cut150, tmpp4jjWORecoBjetsCSVM_cut150.M() );
			h_massjjWORecoBjetsCSVM_cut150->Fill( tmpp4jjWORecoBjetsCSVM_cut150.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecoBjetsCSVM_cut200.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecoBjetsCSVM_cut200.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecoBjetsCSVM_cut200[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecoBjetsCSVM_cut200 = p4jWORecoBjetsCSVM_cut200[iim] + p4jWORecoBjetsCSVM_cut200[iin];
			double sumjjWORecoBjetsCSVMpt_cut200 = p4jWORecoBjetsCSVM_cut200[iim].Pt() + p4jWORecoBjetsCSVM_cut200[iin].Pt();
			p4jjWORecoBjetsCSVM_cut200.push_back( tmpp4jjWORecoBjetsCSVM_cut200 );
			scalarsumjjWORecoBjetsCSVMpt_cut200.push_back( sumjjWORecoBjetsCSVMpt_cut200 );
			h_jjWORecoBjetsCSVM_masspt_cut200->Fill( sumjjWORecoBjetsCSVMpt_cut200, tmpp4jjWORecoBjetsCSVM_cut200.M() );
			h_massjjWORecoBjetsCSVM_cut200->Fill( tmpp4jjWORecoBjetsCSVM_cut200.M() );
		}
	}
	/////////////////////////////////////////////////////

	///////// RECO jets + PartonFlavor + Btagging CSVM
	vector< TLorentzVector > p4jjWORecoPartonFlavorBjetsCSVM;
	vector< TLorentzVector > p4jjWORecoPartonFlavorBjetsCSVM_cut50;
	vector< TLorentzVector > p4jjWORecoPartonFlavorBjetsCSVM_cut100;
	vector< TLorentzVector > p4jjWORecoPartonFlavorBjetsCSVM_cut150;
	vector< TLorentzVector > p4jjWORecoPartonFlavorBjetsCSVM_cut200;
	vector< TLorentzVector > p4jWORecoPartonFlavorBjetsCSVM;
	vector< TLorentzVector > p4jWORecoPartonFlavorBjetsCSVM_cut50;
	vector< TLorentzVector > p4jWORecoPartonFlavorBjetsCSVM_cut100;
	vector< TLorentzVector > p4jWORecoPartonFlavorBjetsCSVM_cut150;
	vector< TLorentzVector > p4jWORecoPartonFlavorBjetsCSVM_cut200;
	vector< double > scalarsumjjWORecoPartonFlavorBjetsCSVMpt;
	vector< double > scalarsumjjWORecoPartonFlavorBjetsCSVMpt_cut50;
	vector< double > scalarsumjjWORecoPartonFlavorBjetsCSVMpt_cut100;
	vector< double > scalarsumjjWORecoPartonFlavorBjetsCSVMpt_cut150;
	vector< double > scalarsumjjWORecoPartonFlavorBjetsCSVMpt_cut200;

	if ( p4RecoJets.size() > 5 && p4RecoPartonFlavorBjetsCSVM.size() > 3 ){
		for(unsigned int iii = 0; iii < p4RecoJets.size(); ++iii) {

			////// Step 2 - dijet mass vs sum pt of jets
			for(unsigned int iij = 0; iij < p4RecoJets.size(); ++iij) {
				if ( iii==iij ) continue;
				TLorentzVector p4RecoPartonFlavorBtagJetsjj = p4RecoJets[iii] + p4RecoJets[iij];
				double sumRecoPartonFlavorBtagJetsjjpt = p4RecoJets[iii].Pt() + p4RecoJets[iij].Pt();
				h_RecoPartonFlavorBtagJetsjj_masspt->Fill( sumRecoPartonFlavorBtagJetsjjpt, p4RecoPartonFlavorBtagJetsjj.M() );
			}
			
			////// Step 3 - dijet mass vs sum pt of jets without bs from step 1
			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff[0] ) || ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff[1] ) || ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff[2] ) || ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff[3] ) )continue;
				p4jWORecoPartonFlavorBjetsCSVM.push_back( p4RecoJets[iii] );
			}

			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut50.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut50[0] ) || ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut50[1] ) || ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut50[2] ) || ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut50[3] ) )continue;
				p4jWORecoPartonFlavorBjetsCSVM_cut50.push_back( p4RecoJets[iii] );
			}

			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut100.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut100[0] ) || ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut100[1] ) || ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut100[2] ) || ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut100[3] ) )continue;
				p4jWORecoPartonFlavorBjetsCSVM_cut100.push_back( p4RecoJets[iii] );
			}

			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut150.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut150[0] ) || ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut150[1] ) || ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut150[2] ) || ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut150[3] ) )continue;
				p4jWORecoPartonFlavorBjetsCSVM_cut150.push_back( p4RecoJets[iii] );
			}

			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut200.size() == 4 ){
				if ( ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut200[0] ) || ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut200[1] ) || ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut200[2] ) || ( p4RecoJets[iii] == p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cut200[3] ) )continue;
				p4jWORecoPartonFlavorBjetsCSVM_cut200.push_back( p4RecoJets[iii] );
			}
		}
	}
			
	for(unsigned int iim = 0; iim < p4jWORecoPartonFlavorBjetsCSVM.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecoPartonFlavorBjetsCSVM.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecoPartonFlavorBjetsCSVM[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecoPartonFlavorBjetsCSVM = p4jWORecoPartonFlavorBjetsCSVM[iim] + p4jWORecoPartonFlavorBjetsCSVM[iin];
			double sumjjWORecoPartonFlavorBjetsCSVMpt = p4jWORecoPartonFlavorBjetsCSVM[iim].Pt() + p4jWORecoPartonFlavorBjetsCSVM[iin].Pt();
			p4jjWORecoPartonFlavorBjetsCSVM.push_back( tmpp4jjWORecoPartonFlavorBjetsCSVM );
			scalarsumjjWORecoPartonFlavorBjetsCSVMpt.push_back( sumjjWORecoPartonFlavorBjetsCSVMpt );
			h_jjWORecoPartonFlavorBjetsCSVM_masspt->Fill( sumjjWORecoPartonFlavorBjetsCSVMpt, tmpp4jjWORecoPartonFlavorBjetsCSVM.M() );
			h_massjjWORecoPartonFlavorBjetsCSVM->Fill( tmpp4jjWORecoPartonFlavorBjetsCSVM.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecoPartonFlavorBjetsCSVM_cut50.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecoPartonFlavorBjetsCSVM_cut50.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecoPartonFlavorBjetsCSVM_cut50[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecoPartonFlavorBjetsCSVM_cut50 = p4jWORecoPartonFlavorBjetsCSVM_cut50[iim] + p4jWORecoPartonFlavorBjetsCSVM_cut50[iin];
			double sumjjWORecoPartonFlavorBjetsCSVMpt_cut50 = p4jWORecoPartonFlavorBjetsCSVM_cut50[iim].Pt() + p4jWORecoPartonFlavorBjetsCSVM_cut50[iin].Pt();
			p4jjWORecoPartonFlavorBjetsCSVM_cut50.push_back( tmpp4jjWORecoPartonFlavorBjetsCSVM_cut50 );
			scalarsumjjWORecoPartonFlavorBjetsCSVMpt_cut50.push_back( sumjjWORecoPartonFlavorBjetsCSVMpt_cut50 );
			h_jjWORecoPartonFlavorBjetsCSVM_masspt_cut50->Fill( sumjjWORecoPartonFlavorBjetsCSVMpt_cut50, tmpp4jjWORecoPartonFlavorBjetsCSVM_cut50.M() );
			h_massjjWORecoPartonFlavorBjetsCSVM_cut50->Fill( tmpp4jjWORecoPartonFlavorBjetsCSVM_cut50.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecoPartonFlavorBjetsCSVM_cut100.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecoPartonFlavorBjetsCSVM_cut100.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecoPartonFlavorBjetsCSVM_cut100[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecoPartonFlavorBjetsCSVM_cut100 = p4jWORecoPartonFlavorBjetsCSVM_cut100[iim] + p4jWORecoPartonFlavorBjetsCSVM_cut100[iin];
			double sumjjWORecoPartonFlavorBjetsCSVMpt_cut100 = p4jWORecoPartonFlavorBjetsCSVM_cut100[iim].Pt() + p4jWORecoPartonFlavorBjetsCSVM_cut100[iin].Pt();
			p4jjWORecoPartonFlavorBjetsCSVM_cut100.push_back( tmpp4jjWORecoPartonFlavorBjetsCSVM_cut100 );
			scalarsumjjWORecoPartonFlavorBjetsCSVMpt_cut100.push_back( sumjjWORecoPartonFlavorBjetsCSVMpt_cut100 );
			h_jjWORecoPartonFlavorBjetsCSVM_masspt_cut100->Fill( sumjjWORecoPartonFlavorBjetsCSVMpt_cut100, tmpp4jjWORecoPartonFlavorBjetsCSVM_cut100.M() );
			h_massjjWORecoPartonFlavorBjetsCSVM_cut100->Fill( tmpp4jjWORecoPartonFlavorBjetsCSVM_cut100.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecoPartonFlavorBjetsCSVM_cut150.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecoPartonFlavorBjetsCSVM_cut150.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecoPartonFlavorBjetsCSVM_cut150[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecoPartonFlavorBjetsCSVM_cut150 = p4jWORecoPartonFlavorBjetsCSVM_cut150[iim] + p4jWORecoPartonFlavorBjetsCSVM_cut150[iin];
			double sumjjWORecoPartonFlavorBjetsCSVMpt_cut150 = p4jWORecoPartonFlavorBjetsCSVM_cut150[iim].Pt() + p4jWORecoPartonFlavorBjetsCSVM_cut150[iin].Pt();
			p4jjWORecoPartonFlavorBjetsCSVM_cut150.push_back( tmpp4jjWORecoPartonFlavorBjetsCSVM_cut150 );
			scalarsumjjWORecoPartonFlavorBjetsCSVMpt_cut150.push_back( sumjjWORecoPartonFlavorBjetsCSVMpt_cut150 );
			h_jjWORecoPartonFlavorBjetsCSVM_masspt_cut150->Fill( sumjjWORecoPartonFlavorBjetsCSVMpt_cut150, tmpp4jjWORecoPartonFlavorBjetsCSVM_cut150.M() );
			h_massjjWORecoPartonFlavorBjetsCSVM_cut150->Fill( tmpp4jjWORecoPartonFlavorBjetsCSVM_cut150.M() );
		}
	}

	for(unsigned int iim = 0; iim < p4jWORecoPartonFlavorBjetsCSVM_cut200.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4jWORecoPartonFlavorBjetsCSVM_cut200.size(); ++iin) {
			if ( iim == iin ) continue;
			//cout << "5 " << iin << " " << p4jWORecoPartonFlavorBjetsCSVM_cut200[iin].Pt() << endl;
			TLorentzVector tmpp4jjWORecoPartonFlavorBjetsCSVM_cut200 = p4jWORecoPartonFlavorBjetsCSVM_cut200[iim] + p4jWORecoPartonFlavorBjetsCSVM_cut200[iin];
			double sumjjWORecoPartonFlavorBjetsCSVMpt_cut200 = p4jWORecoPartonFlavorBjetsCSVM_cut200[iim].Pt() + p4jWORecoPartonFlavorBjetsCSVM_cut200[iin].Pt();
			p4jjWORecoPartonFlavorBjetsCSVM_cut200.push_back( tmpp4jjWORecoPartonFlavorBjetsCSVM_cut200 );
			scalarsumjjWORecoPartonFlavorBjetsCSVMpt_cut200.push_back( sumjjWORecoPartonFlavorBjetsCSVMpt_cut200 );
			h_jjWORecoPartonFlavorBjetsCSVM_masspt_cut200->Fill( sumjjWORecoPartonFlavorBjetsCSVMpt_cut200, tmpp4jjWORecoPartonFlavorBjetsCSVM_cut200.M() );
			h_massjjWORecoPartonFlavorBjetsCSVM_cut200->Fill( tmpp4jjWORecoPartonFlavorBjetsCSVM_cut200.M() );
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//////           STEP 4                                    /////
	//////  Mass of MatchBjets from 1 and jj from 3 vs sum pt of MatchBjetsjj  /////
	////////////////////////////////////////////////////////////////
	
	/*////// Reco Jets
	for(unsigned int iiii = 0; iiii < p4RecoMatchBjetsSmallMassDiff.size(); ++iiii) {
		for(unsigned int iiij = 0; iiij < p4RecoMatchBjetsSmallMassDiff.size(); ++iiij) {
			if ( iiii == iiij ) continue;
			for(unsigned int iijj = 0; iijj < p4recojWOMatchBjets.size(); ++iijj) {
				for(unsigned int ijjj = 0; ijjj < p4recojWOMatchBjets.size(); ++ijjj) {
					if ( iijj == ijjj ) continue;
					TLorentzVector p4recoMatchBjetsjjMass = p4RecoMatchBjetsSmallMassDiff[iiii] + p4RecoMatchBjetsSmallMassDiff[iiij] + p4recojWOMatchBjets[iijj] + p4recojWOMatchBjets[ijjj];
					double sumrecoMatchBjetsjjPt = p4RecoMatchBjetsSmallMassDiff[iiii].Pt() + p4RecoMatchBjetsSmallMassDiff[iiij].Pt() + p4recojWOMatchBjets[iijj].Pt() + p4recojWOMatchBjets[ijjj].Pt();
					h_recoMatchBjetsjj_masspt->Fill( p4recoMatchBjetsjjMass.M(), sumrecoMatchBjetsjjPt );
				}
			}
		}
	}
	////////////////////////////
	
	/////// Match Jets
	for(unsigned int iiii = 0; iiii < p4MatchBjetsSmallMassDiff.size(); ++iiii) {
		for(unsigned int iiij = 0; iiij < p4MatchBjetsSmallMassDiff.size(); ++iiij) {
			if ( iiii == iiij ) continue;
			for(unsigned int iijj = 0; iijj < p4jWOMatchBjets.size(); ++iijj) {
				for(unsigned int ijjj = 0; ijjj < p4jWOMatchBjets.size(); ++ijjj) {
					if ( iijj == ijjj ) continue;
					TLorentzVector p4MatchBjetsjjMass = p4MatchBjetsSmallMassDiff[iiii] + p4MatchBjetsSmallMassDiff[iiij] + p4jWOMatchBjets[iijj] + p4jWOMatchBjets[ijjj];
					double sumMatchBjetsjjPt = p4MatchBjetsSmallMassDiff[iiii].Pt() + p4MatchBjetsSmallMassDiff[iiij].Pt() + p4jWOMatchBjets[iijj].Pt() + p4jWOMatchBjets[ijjj].Pt();
					h_MatchBjetsjj_masspt->Fill( p4MatchBjetsjjMass.M(), sumMatchBjetsjjPt );
				}
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////*/
	

	////////////////////////////////////////////////////////////////
	//////           STEP 5                                    /////
	//////  Smallest mass MatchBjetsjj                                 /////
	////////////////////////////////////////////////////////////////
	
	/*////// Reco Jets
	for(unsigned int jjjj = 0; jjjj < p4RecoMatchBjetsSmallMassDiff.size()-3; ++jjjj) {
		for(unsigned int jjjk = 1; jjjk < p4RecoMatchBjetsSmallMassDiff.size()-2; ++jjjk) {
			if ( jjjj == jjjk ) continue;
			for(unsigned int iijj = 0; iijj < p4recojWOMatchBjets.size(); ++iijj) {
				for(unsigned int ijjj = 0; ijjj < p4recojWOMatchBjets.size(); ++ijjj) {
					if ( iijj == ijjj ) continue;
					TLorentzVector p4recoMatchBjetsjjMass = p4RecoMatchBjetsSmallMassDiff[iiii] + p4RecoMatchBjetsSmallMassDiff[iiij] + p4recojWOMatchBjets[iijj] + p4recojWOMatchBjets[ijjj];
					double sumrecoMatchBjetsjjPt = p4RecoMatchBjetsSmallMassDiff[iiii].Pt() + p4RecoMatchBjetsSmallMassDiff[iiij].Pt() + p4recojWOMatchBjets[iijj].Pt() + p4recojWOMatchBjets[ijjj].Pt();
					h_massrecoMatchBjetsjj->Fill( p4recoMatchBjetsjjMass.M(), sumrecoMatchBjetsjjPt );
				}
			}
		}
	}
	///////////////////////////*/


}

//define this as a plug-in
DEFINE_FWK_MODULE(MyAnalyzer);
