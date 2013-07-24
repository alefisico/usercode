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

//////////////////////////////////////////////////////////////////////////////////////////////////////
////// Function analyze small mass difference
smallMassDiffVectors smallMassDiff( vector< TLorentzVector > p4StoreJets ){

	double tmpMassDiff = 9999999;
	double avgMassBjets = -99999;
	double avgPtBjets = -99999;
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
		avgMassBjets = std::accumulate(massMatchBjets.begin(), massMatchBjets.end(), 0.0) / massMatchBjets.size();
		avgPtBjets = std::accumulate(scalarPtMatchBjets.begin(), scalarPtMatchBjets.end(), 0.0) / scalarPtMatchBjets.size();
	}
	smallMassDiffVectors tmpVectors;
	tmpVectors.p4SmallMassDiff = p4MatchBjetsSmallMassDiff;
	tmpVectors.mass = massMatchBjets;
	tmpVectors.avgMass = avgMassBjets;
	tmpVectors.pt = ptMatchBjets;
	tmpVectors.scalarPt = scalarPtMatchBjets;
	tmpVectors.avgPt = avgPtBjets;


	return tmpVectors;
}
////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////
/////////// Function Compare two TLorentzVectors
vector< TLorentzVector > compare2VectorsTLorentzVectors( vector< TLorentzVector > vec1, vector< TLorentzVector > vec2 ){

	vector< TLorentzVector > diffVec;

	for(unsigned int i = 0; i < vec1.size(); ++i) {
		//cout << vec2[0].Pt() << " " << vec2[1].Pt() << " " << vec2[2].Pt() << " " << vec2[3].Pt() << endl; 
		if ( ( vec1[i].Pt() == vec2[0].Pt() ) || ( vec1[i].Pt() == vec2[1].Pt() ) || ( vec1[i].Pt() == vec2[2].Pt() ) || ( vec1[i].Pt() == vec2[3].Pt() ) ) continue;
		diffVec.push_back( vec1[i] );
	}
	return diffVec;
}
////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
/////// Function Step3 (dijet mass from one vector)
s3VecMassSumPt s3Vectors( vector< TLorentzVector > p4Store ){

	vector< TLorentzVector > p4Vec;
	vector< double > massVec;
	vector< double > scalarSumPtVec;

	for(unsigned int iim = 0; iim < p4Store.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4Store.size(); ++iin) {
			if ( iim >= iin ) continue;
			//cout << "0 " << iin << " " << p4Store[iin].Pt() << endl;
			TLorentzVector tmpp4 = p4Store[iim] + p4Store[iin];
			double sumpt = p4Store[iim].Pt() + p4Store[iin].Pt();
			p4Vec.push_back( tmpp4 );
			massVec.push_back( tmpp4.M() );
			scalarSumPtVec.push_back( sumpt );
		}
	}
	s3VecMassSumPt tmpVectors;
	tmpVectors.s3VectorTL = p4Vec;
	tmpVectors.s3Mass = massVec;
	tmpVectors.s3ScalarPt = scalarSumPtVec;


	return tmpVectors;
}
///////////////////////////////////////////////////////////////////////////////////


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
	double avgMassRecoBjetsCSVM = RecoBjetsCSVM_Vectors.avgMass;
	double avgPtRecoBjetsCSVM = RecoBjetsCSVM_Vectors.avgPt;

	//// TLorentzVectors for cuts
	vector< TLorentzVector > p4RecoBjetsCSVM_SmallMassDiff_1cut50;
	vector< TLorentzVector > p4RecoBjetsCSVM_SmallMassDiff_1cut100;
	vector< TLorentzVector > p4RecoBjetsCSVM_SmallMassDiff_1cut150;
	vector< TLorentzVector > p4RecoBjetsCSVM_SmallMassDiff_1cut200;

	for(unsigned int tii = 0; tii < massRecoBjetsCSVM.size(); ++tii) {
		///// Simple plots
		h_massRecoBjetsCSVM->Fill( massRecoBjetsCSVM[tii] );
		if (tii==0) h_avgMassRecoBjetsCSVM->Fill( avgMassRecoBjetsCSVM );
		if (tii==0) h_avgPtRecoBjetsCSVM->Fill( avgPtRecoBjetsCSVM, avgMassRecoBjetsCSVM );
		h_PtRecoBjetsCSVM->Fill( ptRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );
		h_scalarPtRecoBjetsCSVM->Fill( scalarPtRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );

		//cout << " 1 " << massRecoBjetsCSVM[0] << " " << massRecoBjetsCSVM[1] << endl;
		//	if ( massRecoBjetsCSVM[0] < 130 && massRecoBjetsCSVM[0] > 120 ) continue;
		//	if ( massRecoBjetsCSVM[1] < 130 && massRecoBjetsCSVM[1] > 120 ) continue;
		/*if ( massRecoBjetsCSVM[0] < 130 ) continue;
		if ( massRecoBjetsCSVM[1] > 120 ) continue;
		if ( massRecoBjetsCSVM[0] < 130 ) continue;
		if ( massRecoBjetsCSVM[1] > 120 ) continue;*/
		//cout << " 2 " << massRecoBjetsCSVM[0] << " " << massRecoBjetsCSVM[1] << endl;

		///// Diagonal cuts
		double iDiag=(double)tii*10.0+50.0;
		if( massRecoBjetsCSVM[tii] < ( scalarPtRecoBjetsCSVM[tii]-iDiag ) ) {
			h_scalarPtRecoBjetsCSVM_1cut50->Fill( scalarPtRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );
			h_massRecoBjetsCSVM_1cut50->Fill( massRecoBjetsCSVM[tii] );
			h_scalarPtRecoBjetsCSVM_1cut50->Fill( scalarPtRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );
			if (tii==0) h_avgMassRecoBjetsCSVM_1cut50->Fill( avgMassRecoBjetsCSVM );
			if (tii==0) h_avgPtRecoBjetsCSVM_1cut50->Fill( avgPtRecoBjetsCSVM, avgMassRecoBjetsCSVM);
			p4RecoBjetsCSVM_SmallMassDiff_1cut50 = p4RecoBjetsCSVM_SmallMassDiff;
		}
							
		double iiDiag=(double)tii*10.0+100.0;
		if( massRecoBjetsCSVM[tii] < ( scalarPtRecoBjetsCSVM[tii]-iiDiag ) ) {
			h_scalarPtRecoBjetsCSVM_1cut100->Fill( scalarPtRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );
			h_massRecoBjetsCSVM_1cut100->Fill( massRecoBjetsCSVM[tii] );
			h_scalarPtRecoBjetsCSVM_1cut100->Fill( scalarPtRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );
			if (tii==0) h_avgMassRecoBjetsCSVM_1cut100->Fill( avgMassRecoBjetsCSVM );
			if (tii==0) h_avgPtRecoBjetsCSVM_1cut100->Fill( avgPtRecoBjetsCSVM,avgMassRecoBjetsCSVM );
			p4RecoBjetsCSVM_SmallMassDiff_1cut100 = p4RecoBjetsCSVM_SmallMassDiff;
		}
		
		double iiiDiag=(double)tii*10.0+150.0;
		if( massRecoBjetsCSVM[tii] < ( scalarPtRecoBjetsCSVM[tii]-iiiDiag ) ) {
			h_scalarPtRecoBjetsCSVM_1cut150->Fill( scalarPtRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );
			h_massRecoBjetsCSVM_1cut150->Fill( massRecoBjetsCSVM[tii] );
			h_scalarPtRecoBjetsCSVM_1cut150->Fill( scalarPtRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );
			if (tii==0) h_avgMassRecoBjetsCSVM_1cut150->Fill( avgMassRecoBjetsCSVM );
			if (tii==0) h_avgPtRecoBjetsCSVM_1cut150->Fill( avgPtRecoBjetsCSVM, avgMassRecoBjetsCSVM );
			p4RecoBjetsCSVM_SmallMassDiff_1cut150 = p4RecoBjetsCSVM_SmallMassDiff;
		}

		double ivDiag=(double)tii*10.0+200.0;
		if( massRecoBjetsCSVM[tii] < ( scalarPtRecoBjetsCSVM[tii]-ivDiag ) ){
			h_scalarPtRecoBjetsCSVM_1cut200->Fill( scalarPtRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );
			h_massRecoBjetsCSVM_1cut200->Fill( massRecoBjetsCSVM[tii] );
			h_scalarPtRecoBjetsCSVM_1cut200->Fill( scalarPtRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );
			if (tii==0) h_avgMassRecoBjetsCSVM_1cut200->Fill( avgMassRecoBjetsCSVM );
			if (tii==0) h_avgPtRecoBjetsCSVM_1cut200->Fill( avgPtRecoBjetsCSVM, avgMassRecoBjetsCSVM );
			p4RecoBjetsCSVM_SmallMassDiff_1cut200 = p4RecoBjetsCSVM_SmallMassDiff;
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
	double avgMassRecoPartonFlavorBjetsCSVM = RecoPartonFlavorBjetsCSVM_Vectors.avgMass;
	double avgPtRecoPartonFlavorBjetsCSVM = RecoPartonFlavorBjetsCSVM_Vectors.avgPt;

	//// TLorentzVectors for cuts
	vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut50;
	vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut100;
	vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut150;
	vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut200;

	for(unsigned int tiii = 0; tiii < massRecoPartonFlavorBjetsCSVM.size(); ++tiii) {
		///// Simple plots
		h_massRecoPartonFlavorBjetsCSVM->Fill( massRecoPartonFlavorBjetsCSVM[tiii] );
		h_PtRecoPartonFlavorBjetsCSVM->Fill( ptRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
		h_scalarPtRecoPartonFlavorBjetsCSVM->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
		if (tiii==0) h_avgMassRecoPartonFlavorBjetsCSVM->Fill( avgMassRecoPartonFlavorBjetsCSVM );
		if (tiii==0) h_avgPtRecoPartonFlavorBjetsCSVM->Fill( avgPtRecoPartonFlavorBjetsCSVM, avgMassRecoPartonFlavorBjetsCSVM );


		///// Diagonal cuts
		double iDiag=(double)tiii*10.0+50.0;
		if( massRecoPartonFlavorBjetsCSVM[tiii] < ( scalarPtRecoPartonFlavorBjetsCSVM[tiii]-iDiag ) ) {
			h_scalarPtRecoPartonFlavorBjetsCSVM_1cut50->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			h_massRecoPartonFlavorBjetsCSVM_1cut50->Fill( massRecoPartonFlavorBjetsCSVM[tiii] );
			h_scalarPtRecoPartonFlavorBjetsCSVM_1cut50->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			if (tiii==0) h_avgMassRecoPartonFlavorBjetsCSVM_1cut50->Fill( avgMassRecoPartonFlavorBjetsCSVM );
			if (tiii==0) h_avgPtRecoPartonFlavorBjetsCSVM_1cut50->Fill( avgPtRecoPartonFlavorBjetsCSVM, avgMassRecoPartonFlavorBjetsCSVM );
			p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut50 = p4RecoPartonFlavorBjetsCSVM_SmallMassDiff;
		}
		
		double iiDiag=(double)tiii*10.0+100.0;
		if( massRecoPartonFlavorBjetsCSVM[tiii] < ( scalarPtRecoPartonFlavorBjetsCSVM[tiii]-iiDiag ) ) {
			h_scalarPtRecoPartonFlavorBjetsCSVM_1cut100->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			h_massRecoPartonFlavorBjetsCSVM_1cut100->Fill( massRecoPartonFlavorBjetsCSVM[tiii] );
			h_scalarPtRecoPartonFlavorBjetsCSVM_1cut100->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			if (tiii==0) h_avgMassRecoPartonFlavorBjetsCSVM_1cut100->Fill( avgMassRecoPartonFlavorBjetsCSVM );
			if (tiii==0) h_avgPtRecoPartonFlavorBjetsCSVM_1cut100->Fill( avgPtRecoPartonFlavorBjetsCSVM, avgMassRecoPartonFlavorBjetsCSVM );
			p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut100 = p4RecoPartonFlavorBjetsCSVM_SmallMassDiff;
		}
		
		double iiiDiag=(double)tiii*10.0+150.0;
		if( massRecoPartonFlavorBjetsCSVM[tiii] < ( scalarPtRecoPartonFlavorBjetsCSVM[tiii]-iiiDiag ) ) {
			h_scalarPtRecoPartonFlavorBjetsCSVM_1cut150->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			h_massRecoPartonFlavorBjetsCSVM_1cut150->Fill( massRecoPartonFlavorBjetsCSVM[tiii] );
			h_scalarPtRecoPartonFlavorBjetsCSVM_1cut150->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			if (tiii==0) h_avgMassRecoPartonFlavorBjetsCSVM_1cut150->Fill( avgMassRecoPartonFlavorBjetsCSVM );
			if (tiii==0) h_avgPtRecoPartonFlavorBjetsCSVM_1cut150->Fill( avgPtRecoPartonFlavorBjetsCSVM, avgMassRecoPartonFlavorBjetsCSVM );
			p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut150 = p4RecoPartonFlavorBjetsCSVM_SmallMassDiff;
		}

		double ivDiag=(double)tiii*10.0+200.0;
		if( massRecoPartonFlavorBjetsCSVM[tiii] < ( scalarPtRecoPartonFlavorBjetsCSVM[tiii]-ivDiag ) ) {
			h_scalarPtRecoPartonFlavorBjetsCSVM_1cut200->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			h_massRecoPartonFlavorBjetsCSVM_1cut200->Fill( massRecoPartonFlavorBjetsCSVM[tiii] );
			h_scalarPtRecoPartonFlavorBjetsCSVM_1cut200->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			if (tiii==0) h_avgMassRecoPartonFlavorBjetsCSVM_1cut200->Fill( avgMassRecoPartonFlavorBjetsCSVM );
			if (tiii==0) h_avgPtRecoPartonFlavorBjetsCSVM_1cut200->Fill( avgPtRecoPartonFlavorBjetsCSVM, avgMassRecoPartonFlavorBjetsCSVM );
			p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut200 = p4RecoPartonFlavorBjetsCSVM_SmallMassDiff;
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
	double avgMassMatchBjets = MatchBjetsVectors.avgMass;
	double avgPtMatchBjets = MatchBjetsVectors.avgPt;

	//// TLorentzVectors for cuts
	vector< TLorentzVector > p4MatchBjetsSmallMassDiff_1cut50;
	vector< TLorentzVector > p4MatchBjetsSmallMassDiff_1cut100;
	vector< TLorentzVector > p4MatchBjetsSmallMassDiff_1cut150;
	vector< TLorentzVector > p4MatchBjetsSmallMassDiff_1cut200;

	for(unsigned int ti = 0; ti < massMatchBjets.size(); ++ti) {

		///// Simple plots
		h_massMatchBjets->Fill( massMatchBjets[ti] );
		h_PtMatchBjets->Fill( ptMatchBjets[ti], massMatchBjets[ti] );
		h_scalarPtMatchBjets->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
		if (ti==0) h_avgMassMatchBjets->Fill( avgMassMatchBjets );
		if (ti==0) h_avgPtMatchBjets->Fill( avgPtMatchBjets, avgMassMatchBjets );


		///// Diagonal cuts
		double iDiag=(double)ti*10.0+50.0;
		if( massMatchBjets[ti] < ( scalarPtMatchBjets[ti]-iDiag ) ){
			h_scalarPtMatchBjets_1cut50->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			h_massMatchBjets_1cut50->Fill( massMatchBjets[ti] );
			h_scalarPtMatchBjets_1cut50->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			if (ti==0) h_avgMassMatchBjets_1cut50->Fill( avgMassMatchBjets );
			if (ti==0) h_avgPtMatchBjets_1cut50->Fill( avgPtMatchBjets, avgMassMatchBjets );
			p4MatchBjetsSmallMassDiff_1cut50 = p4MatchBjetsSmallMassDiff;
		}

		double iiDiag=(double)ti*10.0+100.0;
		if( massMatchBjets[ti] < ( scalarPtMatchBjets[ti]-iiDiag ) ) {
			h_scalarPtMatchBjets_1cut100->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			h_massMatchBjets_1cut100->Fill( massMatchBjets[ti] );
			h_scalarPtMatchBjets_1cut100->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			if (ti==0) h_avgMassMatchBjets_1cut100->Fill( avgMassMatchBjets );
			if (ti==0) h_avgPtMatchBjets_1cut100->Fill( avgPtMatchBjets, avgMassMatchBjets );
			p4MatchBjetsSmallMassDiff_1cut100 = p4MatchBjetsSmallMassDiff;
		}
	
		double iiiDiag=(double)ti*10.0+150.0;
		if( massMatchBjets[ti] < ( scalarPtMatchBjets[ti]-iiiDiag ) ) {
			h_scalarPtMatchBjets_1cut150->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			h_massMatchBjets_1cut150->Fill( massMatchBjets[ti] );
			h_scalarPtMatchBjets_1cut150->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			if (ti==0) h_avgMassMatchBjets_1cut150->Fill( avgMassMatchBjets );
			if (ti==0) h_avgPtMatchBjets_1cut150->Fill( avgPtMatchBjets, avgMassMatchBjets );
			p4MatchBjetsSmallMassDiff_1cut150 = p4MatchBjetsSmallMassDiff;
		}

		double ivDiag=(double)ti*10.0+200.0;
		if( massMatchBjets[ti] < ( scalarPtMatchBjets[ti]-ivDiag ) ) {
			h_scalarPtMatchBjets_1cut200->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			h_massMatchBjets_1cut200->Fill( massMatchBjets[ti] );
			h_scalarPtMatchBjets_1cut200->Fill( scalarPtMatchBjets[ti], massMatchBjets[ti] );
			if (ti==0) h_avgMassMatchBjets_1cut200->Fill( avgMassMatchBjets );
			if (ti==0) h_avgPtMatchBjets_1cut200->Fill( avgPtMatchBjets, avgMassMatchBjets );
			p4MatchBjetsSmallMassDiff_1cut200 = p4MatchBjetsSmallMassDiff;
		}
	}
	//cout << p4MatchBjetsSmallMassDiff.size() << " " << p4MatchBjetsSmallMassDiff_1cut50.size() << " " << p4MatchBjetsSmallMassDiff_1cut100.size() << " " << p4MatchBjetsSmallMassDiff_1cut150.size() << " " << p4MatchBjetsSmallMassDiff_1cut200.size() << " " << endl;
	///////////////////////////////////////////////////////////////////

	// MatchBjets from Higgs
	vector< TLorentzVector > p4MatchBjetsHiggsSmallMassDiff;
	vector< TLorentzVector > p4MatchBjetsHiggsSmallMassDiff_1cut50;
	vector< TLorentzVector > p4MatchBjetsHiggsSmallMassDiff_1cut100;
	vector< TLorentzVector > p4MatchBjetsHiggsSmallMassDiff_1cut150;
	vector< TLorentzVector > p4MatchBjetsHiggsSmallMassDiff_1cut200;
	vector< double > massMatchBjetsHiggs;
	vector< double > ptMatchBjetsHiggs;
	vector< double > scalarPtMatchBjetsHiggs;
	double avgMassMatchBjetsHiggs = -99999;
	double avgPtMatchBjetsHiggs = -99999;

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
		avgMassMatchBjetsHiggs = std::accumulate(massMatchBjetsHiggs.begin(), massMatchBjetsHiggs.end(), 0.0) / massMatchBjetsHiggs.size();
		avgPtMatchBjetsHiggs = std::accumulate(scalarPtMatchBjetsHiggs.begin(), scalarPtMatchBjetsHiggs.end(), 0.0) / scalarPtMatchBjetsHiggs.size();
	}

	h_massMatchBjetsHiggs_num->Fill( massMatchBjetsHiggs.size() );	/// test number of MatchBjetsHiggs
	for(unsigned int tiv = 0; tiv < massMatchBjetsHiggs.size(); ++tiv) {
		///// Simple plots
		h_massMatchBjetsHiggs->Fill( massMatchBjetsHiggs[tiv] );
		h_PtMatchBjetsHiggs->Fill( ptMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
		h_scalarPtMatchBjetsHiggs->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
		if (tiv==0) h_avgMassMatchBjetsHiggs->Fill( avgMassMatchBjetsHiggs );
		if (tiv==0) h_avgPtMatchBjetsHiggs->Fill( avgPtMatchBjetsHiggs, avgMassMatchBjetsHiggs );


		///// Diagonal cuts
		double iDiag=(double)tiv*10.0+50.0;
		if( massMatchBjetsHiggs[tiv] < ( scalarPtMatchBjetsHiggs[tiv]-iDiag ) ) {
			h_scalarPtMatchBjetsHiggs_1cut50->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			h_massMatchBjetsHiggs_1cut50->Fill( massMatchBjetsHiggs[tiv] );
			h_scalarPtMatchBjetsHiggs_1cut50->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			if (tiv==0) h_avgMassMatchBjetsHiggs_1cut50->Fill( avgMassMatchBjetsHiggs );
			if (tiv==0) h_avgPtMatchBjetsHiggs_1cut50->Fill( avgPtMatchBjetsHiggs, avgMassMatchBjetsHiggs );
			p4MatchBjetsHiggsSmallMassDiff_1cut50.push_back( p4MatchB1Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_1cut50.push_back( p4MatchB1Higgs[1] );
			p4MatchBjetsHiggsSmallMassDiff_1cut50.push_back( p4MatchB2Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_1cut50.push_back( p4MatchB2Higgs[1] );
		}

		double iiDiag=(double)tiv*10.0+100.0;
		if( massMatchBjetsHiggs[tiv] < ( scalarPtMatchBjetsHiggs[tiv]-iiDiag ) ){
			h_scalarPtMatchBjetsHiggs_1cut100->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			h_massMatchBjetsHiggs_1cut100->Fill( massMatchBjetsHiggs[tiv] );
			h_scalarPtMatchBjetsHiggs_1cut100->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			if (tiv==0) h_avgMassMatchBjetsHiggs_1cut100->Fill( avgMassMatchBjetsHiggs );
			if (tiv==0) h_avgPtMatchBjetsHiggs_1cut100->Fill( avgPtMatchBjetsHiggs, avgMassMatchBjetsHiggs );
			p4MatchBjetsHiggsSmallMassDiff_1cut100.push_back( p4MatchB1Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_1cut100.push_back( p4MatchB1Higgs[1] );
			p4MatchBjetsHiggsSmallMassDiff_1cut100.push_back( p4MatchB2Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_1cut100.push_back( p4MatchB2Higgs[1] );
		}

		double iiiDiag=(double)tiv*10.0+150.0;
		if( massMatchBjetsHiggs[tiv] < ( scalarPtMatchBjetsHiggs[tiv]-iiiDiag ) ) {
			h_scalarPtMatchBjetsHiggs_1cut150->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			h_massMatchBjetsHiggs_1cut150->Fill( massMatchBjetsHiggs[tiv] );
			h_scalarPtMatchBjetsHiggs_1cut150->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			if (tiv==0) h_avgMassMatchBjetsHiggs_1cut150->Fill( avgMassMatchBjetsHiggs );
			if (tiv==0) h_avgPtMatchBjetsHiggs_1cut150->Fill( avgPtMatchBjetsHiggs, avgMassMatchBjetsHiggs );
			p4MatchBjetsHiggsSmallMassDiff_1cut150.push_back( p4MatchB1Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_1cut150.push_back( p4MatchB1Higgs[1] );
			p4MatchBjetsHiggsSmallMassDiff_1cut150.push_back( p4MatchB2Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_1cut150.push_back( p4MatchB2Higgs[1] );
		}

		double ivDiag=(double)tiv*10.0+200.0;
		if( massMatchBjetsHiggs[tiv] < ( scalarPtMatchBjetsHiggs[tiv]-ivDiag ) ) {
			h_scalarPtMatchBjetsHiggs_1cut200->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			h_massMatchBjetsHiggs_1cut200->Fill( massMatchBjetsHiggs[tiv] );
			h_scalarPtMatchBjetsHiggs_1cut200->Fill( scalarPtMatchBjetsHiggs[tiv], massMatchBjetsHiggs[tiv] );
			if (tiv==0) h_avgMassMatchBjetsHiggs_1cut200->Fill( avgMassMatchBjetsHiggs );
			if (tiv==0) h_avgPtMatchBjetsHiggs_1cut200->Fill( avgPtMatchBjetsHiggs, avgMassMatchBjetsHiggs );
			p4MatchBjetsHiggsSmallMassDiff_1cut200.push_back( p4MatchB1Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_1cut200.push_back( p4MatchB1Higgs[1] );
			p4MatchBjetsHiggsSmallMassDiff_1cut200.push_back( p4MatchB2Higgs[0] );
			p4MatchBjetsHiggsSmallMassDiff_1cut200.push_back( p4MatchB2Higgs[1] );
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
	/////// Mass dijet vs sum pt dijet //////////
	///////////////////////////////////////


	///////// RECO jets + Btagging CSVM
	vector< TLorentzVector > p4jetsWORecoJetsCSVM;
	vector< TLorentzVector > p4bWORecoBjetsCSVM;
	vector< TLorentzVector > p4jetsWORecoBjetsCSVM;
	vector< TLorentzVector > p4jetsWORecoBjetsCSVM_1cut50;
	vector< TLorentzVector > p4jetsWORecoBjetsCSVM_1cut100;
	vector< TLorentzVector > p4jetsWORecoBjetsCSVM_1cut150;
	vector< TLorentzVector > p4jetsWORecoBjetsCSVM_1cut200;

	if ( p4RecoJets.size() > 5 && p4RecoBjetsCSVM.size() > 3 ){

		////// Step 2 - dijet mass vs sum pt of jets
		for(unsigned int iii = 0; iii < p4RecoJets.size(); ++iii) {

			//cout << " 3 " << massRecoBjetsCSVM[0] << " " << massRecoBjetsCSVM[1] << endl;
			//if ( massRecoBjetsCSVM[0] < 160 && massRecoBjetsCSVM[0] > 90 ) continue;
			//if ( massRecoBjetsCSVM[1] < 160 && massRecoBjetsCSVM[1] > 90 ) continue;
			//if ( massRecoBjetsCSVM[0] > 130 || massRecoBjetsCSVM[0] < 120 ) continue;
			//if ( massRecoBjetsCSVM[1] > 130 || massRecoBjetsCSVM[1] < 120 ) continue;
			//cout << " 4 " << massRecoBjetsCSVM[0] << " " << massRecoBjetsCSVM[1] << endl;

			//cout << " 0 " << iii <<  " " << p4RecoJets[iii].Pt() << endl;
			for(unsigned int iij = 0; iij < p4RecoJets.size(); ++iij) {
				if ( iii >= iij ) continue;
				TLorentzVector p4RecoBtagJetsdijet = p4RecoJets[iii] + p4RecoJets[iij];
				double sumRecoBtagJetsdijetpt = p4RecoJets[iii].Pt() + p4RecoJets[iij].Pt();
				//cout << " 1 " << iii << " " << iij << " " << p4RecoBtagJetsdijet.M() << endl;
				h_RecoBtagJetsdijet_masspt->Fill( sumRecoBtagJetsdijetpt, p4RecoBtagJetsdijet.M() );
			}
			
			////// Step 3 - dijet mass vs sum pt of jets without bs from step 1
			//p4jetsWORecoJetsCSVM = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoBjetsCSVM );
			//if ( p4RecoBjetsCSVM_SmallMassDiff.size() == 4 ) p4bWORecoBjetsCSVM = compare2VectorsTLorentzVectors( p4RecoBjetsCSVM, p4RecoBjetsCSVM_SmallMassDiff );
			if ( p4RecoBjetsCSVM_SmallMassDiff.size() == 4 ) p4jetsWORecoBjetsCSVM = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoBjetsCSVM_SmallMassDiff );
			if ( p4RecoBjetsCSVM_SmallMassDiff_1cut50.size() == 4 ) p4jetsWORecoBjetsCSVM_1cut50 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoBjetsCSVM_SmallMassDiff_1cut50 );
			if ( p4RecoBjetsCSVM_SmallMassDiff_1cut100.size() == 4 ) p4jetsWORecoBjetsCSVM_1cut100 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoBjetsCSVM_SmallMassDiff_1cut100 );
			if ( p4RecoBjetsCSVM_SmallMassDiff_1cut150.size() == 4 ) p4jetsWORecoBjetsCSVM_1cut150 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoBjetsCSVM_SmallMassDiff_1cut150 );
			if ( p4RecoBjetsCSVM_SmallMassDiff_1cut200.size() == 4 ) p4jetsWORecoBjetsCSVM_1cut200 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoBjetsCSVM_SmallMassDiff_1cut200 );
		}
	}
			
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// TEST
	for(unsigned int iim = 0; iim < p4jetsWORecoJetsCSVM.size(); ++iim) {
		for(unsigned int iin = 0; iin < p4bWORecoBjetsCSVM.size(); ++iin) {
			//cout << " 2 " << iim <<  " " << p4jetsWORecoJetsCSVM[iim].Pt() << " " << iin <<  " " << p4bWORecoBjetsCSVM[iin].Pt() << endl;
			//if ( iim >= iin ) continue;
			//cout << "5 " << iin << " " << p4jetsWORecoBjetsCSVM[iin].Pt() << endl;
			TLorentzVector tmpp4dijetWORecoBjetsCSVM = p4jetsWORecoJetsCSVM[iim] + p4bWORecoBjetsCSVM[iin];
			//cout << " 4 " << iim << " " << iin << " " << tmpp4dijetWORecoBjetsCSVM.M() << endl;

			double sumptdijetWORecoBjetsCSVM = p4jetsWORecoJetsCSVM[iim].Pt() + p4bWORecoBjetsCSVM[iin].Pt();
			p4dijetWORecoBjetsCSVM.push_back( tmpp4dijetWORecoBjetsCSVM );
			scalarSumPtdijetWORecoBjetsCSVM.push_back( sumptdijetWORecoBjetsCSVM );
		}
	}
	if ( p4jetsWORecoJetsCSVM.size() > 1 && p4bWORecoBjetsCSVM.size() > 1){
		double tmpMassDiff = 9999999;
		Int_t bestMassIndex[4] = {-1, -1, -1, -1};
		for(unsigned int ii = 0; ii < p4jetsWORecoJetsCSVM.size(); ++ii) {
			for(unsigned int ij = 0; ij < p4bWORecoBjetsCSVM.size(); ++ij) {
				for(unsigned int ik = 0; ik < p4jetsWORecoJetsCSVM.size(); ++ik) {
					for(unsigned int il = 0; il < p4bWORecoBjetsCSVM.size(); ++il) {
						if ( ii >= ik ) continue;
						if ( ij >= il ) continue;
						//cout << ii << " " << ij << " " << ik << " " << il << endl;
						TLorentzVector candMassMatchBjets1 = p4jetsWORecoJetsCSVM[ii]+p4bWORecoBjetsCSVM[ij];
						TLorentzVector candMassMatchBjets2 = p4jetsWORecoJetsCSVM[ik]+p4bWORecoBjetsCSVM[il];
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
		TLorentzVector tmpMass1 = p4jetsWORecoJetsCSVM[bestMassIndex[0]] + p4bWORecoBjetsCSVM[bestMassIndex[1]];
		TLorentzVector tmpMass2 = p4jetsWORecoJetsCSVM[bestMassIndex[2]] + p4bWORecoBjetsCSVM[bestMassIndex[3]];
		h_massdijetWORecoBjetsCSVM->Fill( tmpMass1.M() );
		h_massdijetWORecoBjetsCSVM->Fill( tmpMass2.M() );
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWORecoBjetsCSVM;
	vectorsjWORecoBjetsCSVM = s3Vectors( p4jetsWORecoBjetsCSVM );
	vector< TLorentzVector > p4dijetWORecoBjetsCSVM = vectorsjWORecoBjetsCSVM.s3VectorTL;
	vector< double > massdijetWORecoBjetsCSVM = vectorsjWORecoBjetsCSVM.s3Mass;
	vector< double > scalarSumPtdijetWORecoBjetsCSVM = vectorsjWORecoBjetsCSVM.s3ScalarPt;

	for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM.size(); ++iim) {
		h_dijetWORecoBjetsCSVM_masspt->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
		h_massdijetWORecoBjetsCSVM->Fill( massdijetWORecoBjetsCSVM[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		/*if( p4jetsWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-iDiag ) ) {
		}*/
	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWORecoBjetsCSVM_1cut50;
	vectorsjWORecoBjetsCSVM_1cut50 = s3Vectors( p4jetsWORecoBjetsCSVM_1cut50 );
	vector< TLorentzVector > p4dijetWORecoBjetsCSVM_1cut50 = vectorsjWORecoBjetsCSVM_1cut50.s3VectorTL;
	vector< double > massdijetWORecoBjetsCSVM_1cut50 = vectorsjWORecoBjetsCSVM_1cut50.s3Mass;
	vector< double > scalarSumPtdijetWORecoBjetsCSVM_1cut50 = vectorsjWORecoBjetsCSVM_1cut50.s3ScalarPt;

	for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM_1cut50.size(); ++iim) {
		h_dijetWORecoBjetsCSVM_masspt_1cut50->Fill( scalarSumPtdijetWORecoBjetsCSVM_1cut50[iim], massdijetWORecoBjetsCSVM_1cut50[iim] );
		h_massdijetWORecoBjetsCSVM_1cut50->Fill( massdijetWORecoBjetsCSVM_1cut50[iim] );

	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWORecoBjetsCSVM_1cut100;
	vectorsjWORecoBjetsCSVM_1cut100 = s3Vectors( p4jetsWORecoBjetsCSVM_1cut100 );
	vector< TLorentzVector > p4dijetWORecoBjetsCSVM_1cut100 = vectorsjWORecoBjetsCSVM_1cut100.s3VectorTL;
	vector< double > massdijetWORecoBjetsCSVM_1cut100 = vectorsjWORecoBjetsCSVM_1cut100.s3Mass;
	vector< double > scalarSumPtdijetWORecoBjetsCSVM_1cut100 = vectorsjWORecoBjetsCSVM_1cut100.s3ScalarPt;

	for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM_1cut100.size(); ++iim) {
		h_dijetWORecoBjetsCSVM_masspt_1cut100->Fill( scalarSumPtdijetWORecoBjetsCSVM_1cut100[iim], massdijetWORecoBjetsCSVM_1cut100[iim] );
		h_massdijetWORecoBjetsCSVM_1cut100->Fill( massdijetWORecoBjetsCSVM_1cut100[iim] );

	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWORecoBjetsCSVM_1cut150;
	vectorsjWORecoBjetsCSVM_1cut150 = s3Vectors( p4jetsWORecoBjetsCSVM_1cut150 );
	vector< TLorentzVector > p4dijetWORecoBjetsCSVM_1cut150 = vectorsjWORecoBjetsCSVM_1cut150.s3VectorTL;
	vector< double > massdijetWORecoBjetsCSVM_1cut150 = vectorsjWORecoBjetsCSVM_1cut150.s3Mass;
	vector< double > scalarSumPtdijetWORecoBjetsCSVM_1cut150 = vectorsjWORecoBjetsCSVM_1cut150.s3ScalarPt;

	for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM_1cut150.size(); ++iim) {
		h_dijetWORecoBjetsCSVM_masspt_1cut150->Fill( scalarSumPtdijetWORecoBjetsCSVM_1cut150[iim], massdijetWORecoBjetsCSVM_1cut150[iim] );
		h_massdijetWORecoBjetsCSVM_1cut150->Fill( massdijetWORecoBjetsCSVM_1cut150[iim] );

	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWORecoBjetsCSVM_1cut200;
	vectorsjWORecoBjetsCSVM_1cut200 = s3Vectors( p4jetsWORecoBjetsCSVM_1cut200 );
	vector< TLorentzVector > p4dijetWORecoBjetsCSVM_1cut200 = vectorsjWORecoBjetsCSVM_1cut200.s3VectorTL;
	vector< double > massdijetWORecoBjetsCSVM_1cut200 = vectorsjWORecoBjetsCSVM_1cut200.s3Mass;
	vector< double > scalarSumPtdijetWORecoBjetsCSVM_1cut200 = vectorsjWORecoBjetsCSVM_1cut200.s3ScalarPt;

	for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM_1cut200.size(); ++iim) {
		h_dijetWORecoBjetsCSVM_masspt_1cut200->Fill( scalarSumPtdijetWORecoBjetsCSVM_1cut200[iim], massdijetWORecoBjetsCSVM_1cut200[iim] );
		h_massdijetWORecoBjetsCSVM_1cut200->Fill( massdijetWORecoBjetsCSVM_1cut200[iim] );

	}
	/////////////////////////////////////////////////////

	///////// RECO jets + PartonFlavor + Btagging CSVM
	vector< TLorentzVector > p4jetsWORecoPartonFlavorBjetsCSVM;
	vector< TLorentzVector > p4jetsWORecoPartonFlavorBjetsCSVM_1cut50;
	vector< TLorentzVector > p4jetsWORecoPartonFlavorBjetsCSVM_1cut100;
	vector< TLorentzVector > p4jetsWORecoPartonFlavorBjetsCSVM_1cut150;
	vector< TLorentzVector > p4jetsWORecoPartonFlavorBjetsCSVM_1cut200;

	if ( p4RecoJets.size() > 5 && p4RecoPartonFlavorBjetsCSVM.size() > 3 ){
		for(unsigned int iii = 0; iii < p4RecoJets.size(); ++iii) {

			////// Step 2 - dijet mass vs sum pt of jets
			for(unsigned int iij = 0; iij < p4RecoJets.size(); ++iij) {
				if ( iii>=iij ) continue;
				TLorentzVector p4RecoPartonFlavorBtagJetsdijet = p4RecoJets[iii] + p4RecoJets[iij];
				double sumRecoPartonFlavorBtagJetsdijetpt = p4RecoJets[iii].Pt() + p4RecoJets[iij].Pt();
				h_RecoPartonFlavorBtagJetsdijet_masspt->Fill( sumRecoPartonFlavorBtagJetsdijetpt, p4RecoPartonFlavorBtagJetsdijet.M() );
			}
			
			////// Step 3 - dijet mass vs sum pt of jets without bs from step 1
			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff.size() == 4 ) p4jetsWORecoPartonFlavorBjetsCSVM = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoPartonFlavorBjetsCSVM_SmallMassDiff );
			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut50.size() == 4 ) p4jetsWORecoPartonFlavorBjetsCSVM_1cut50 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut50 );
			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut100.size() == 4 ) p4jetsWORecoPartonFlavorBjetsCSVM_1cut100 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut100 );
			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut150.size() == 4 ) p4jetsWORecoPartonFlavorBjetsCSVM_1cut150 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut150 );
			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut200.size() == 4 ) p4jetsWORecoPartonFlavorBjetsCSVM_1cut200 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_1cut200 );
		}
	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWORecoPartonFlavorBjetsCSVM;
	vectorsjWORecoPartonFlavorBjetsCSVM = s3Vectors( p4jetsWORecoPartonFlavorBjetsCSVM );
	vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM = vectorsjWORecoPartonFlavorBjetsCSVM.s3VectorTL;
	vector< double > massdijetWORecoPartonFlavorBjetsCSVM = vectorsjWORecoPartonFlavorBjetsCSVM.s3Mass;
	vector< double > scalarSumPtdijetWORecoPartonFlavorBjetsCSVM = vectorsjWORecoPartonFlavorBjetsCSVM.s3ScalarPt;

	for(unsigned int iim = 0; iim < massdijetWORecoPartonFlavorBjetsCSVM.size(); ++iim) {
		h_dijetWORecoPartonFlavorBjetsCSVM_masspt->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim], massdijetWORecoPartonFlavorBjetsCSVM[iim] );
		h_massdijetWORecoPartonFlavorBjetsCSVM->Fill( massdijetWORecoPartonFlavorBjetsCSVM[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		/*if( p4jetsWORecoPartonFlavorBjetsCSVM[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim]-iDiag ) ) {
		}*/
	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWORecoPartonFlavorBjetsCSVM_1cut50;
	vectorsjWORecoPartonFlavorBjetsCSVM_1cut50 = s3Vectors( p4jetsWORecoPartonFlavorBjetsCSVM_1cut50 );
	vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_1cut50 = vectorsjWORecoPartonFlavorBjetsCSVM_1cut50.s3VectorTL;
	vector< double > massdijetWORecoPartonFlavorBjetsCSVM_1cut50 = vectorsjWORecoPartonFlavorBjetsCSVM_1cut50.s3Mass;
	vector< double > scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_1cut50 = vectorsjWORecoPartonFlavorBjetsCSVM_1cut50.s3ScalarPt;

	for(unsigned int iim = 0; iim < massdijetWORecoPartonFlavorBjetsCSVM_1cut50.size(); ++iim) {
		h_dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut50->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_1cut50[iim], massdijetWORecoPartonFlavorBjetsCSVM_1cut50[iim] );
		h_massdijetWORecoPartonFlavorBjetsCSVM_1cut50->Fill( massdijetWORecoPartonFlavorBjetsCSVM_1cut50[iim] );

	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWORecoPartonFlavorBjetsCSVM_1cut100;
	vectorsjWORecoPartonFlavorBjetsCSVM_1cut100 = s3Vectors( p4jetsWORecoPartonFlavorBjetsCSVM_1cut100 );
	vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_1cut100 = vectorsjWORecoPartonFlavorBjetsCSVM_1cut100.s3VectorTL;
	vector< double > massdijetWORecoPartonFlavorBjetsCSVM_1cut100 = vectorsjWORecoPartonFlavorBjetsCSVM_1cut100.s3Mass;
	vector< double > scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_1cut100 = vectorsjWORecoPartonFlavorBjetsCSVM_1cut100.s3ScalarPt;

	for(unsigned int iim = 0; iim < massdijetWORecoPartonFlavorBjetsCSVM_1cut100.size(); ++iim) {
		h_dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut100->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_1cut100[iim], massdijetWORecoPartonFlavorBjetsCSVM_1cut100[iim] );
		h_massdijetWORecoPartonFlavorBjetsCSVM_1cut100->Fill( massdijetWORecoPartonFlavorBjetsCSVM_1cut100[iim] );

	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWORecoPartonFlavorBjetsCSVM_1cut150;
	vectorsjWORecoPartonFlavorBjetsCSVM_1cut150 = s3Vectors( p4jetsWORecoPartonFlavorBjetsCSVM_1cut150 );
	vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_1cut150 = vectorsjWORecoPartonFlavorBjetsCSVM_1cut150.s3VectorTL;
	vector< double > massdijetWORecoPartonFlavorBjetsCSVM_1cut150 = vectorsjWORecoPartonFlavorBjetsCSVM_1cut150.s3Mass;
	vector< double > scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_1cut150 = vectorsjWORecoPartonFlavorBjetsCSVM_1cut150.s3ScalarPt;

	for(unsigned int iim = 0; iim < massdijetWORecoPartonFlavorBjetsCSVM_1cut150.size(); ++iim) {
		h_dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut150->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_1cut150[iim], massdijetWORecoPartonFlavorBjetsCSVM_1cut150[iim] );
		h_massdijetWORecoPartonFlavorBjetsCSVM_1cut150->Fill( massdijetWORecoPartonFlavorBjetsCSVM_1cut150[iim] );

	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWORecoPartonFlavorBjetsCSVM_1cut200;
	vectorsjWORecoPartonFlavorBjetsCSVM_1cut200 = s3Vectors( p4jetsWORecoPartonFlavorBjetsCSVM_1cut200 );
	vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_1cut200 = vectorsjWORecoPartonFlavorBjetsCSVM_1cut200.s3VectorTL;
	vector< double > massdijetWORecoPartonFlavorBjetsCSVM_1cut200 = vectorsjWORecoPartonFlavorBjetsCSVM_1cut200.s3Mass;
	vector< double > scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_1cut200 = vectorsjWORecoPartonFlavorBjetsCSVM_1cut200.s3ScalarPt;

	for(unsigned int iim = 0; iim < massdijetWORecoPartonFlavorBjetsCSVM_1cut200.size(); ++iim) {
		h_dijetWORecoPartonFlavorBjetsCSVM_masspt_1cut200->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_1cut200[iim], massdijetWORecoPartonFlavorBjetsCSVM_1cut200[iim] );
		h_massdijetWORecoPartonFlavorBjetsCSVM_1cut200->Fill( massdijetWORecoPartonFlavorBjetsCSVM_1cut200[iim] );

	}
			
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///// Match All jets
	vector< TLorentzVector > p4jetsWOMatchBjets;
	vector< TLorentzVector > p4jetsWOMatchBjets_1cut50;
	vector< TLorentzVector > p4jetsWOMatchBjets_1cut100;
	vector< TLorentzVector > p4jetsWOMatchBjets_1cut150;
	vector< TLorentzVector > p4jetsWOMatchBjets_1cut200;

	if ( p4MatchTruthAlljets.size() > 5 && p4MatchTruthBjets.size() > 3 ){
		for(unsigned int iii = 0; iii < p4MatchTruthAlljets.size(); ++iii) {

			//if ( massMatchBjets[0] > 130 || massMatchBjets[0] < 120 ) continue;
			//if ( massMatchBjets[1] > 130 || massMatchBjets[1] < 120 ) continue;

			////// Step 2 - dijet mass vs sum pt of jets
			for(unsigned int iij = 0; iij < p4MatchTruthAlljets.size(); ++iij) {
				if ( iii>=iij ) continue;
				TLorentzVector p4dijet = p4MatchTruthAlljets[iii] + p4MatchTruthAlljets[iij];
				double sumdijetpt = p4MatchTruthAlljets[iii].Pt() + p4MatchTruthAlljets[iij].Pt();
				h_matchdijet_masspt->Fill( sumdijetpt, p4dijet.M() );
			}
			
			////// Step 3 - dijet mass vs sum pt of jets without bs from step 1
			//cout << " 0 " << iii << " " << p4MatchTruthAlljets[iii].Pt() << " " << p4MatchTruthAlljets.size() << endl;
			if ( p4MatchBjetsSmallMassDiff.size() == 4 ) p4jetsWOMatchBjets = compare2VectorsTLorentzVectors( p4MatchTruthAlljets, p4MatchBjetsSmallMassDiff );
			if ( p4MatchBjetsSmallMassDiff_1cut50.size() == 4 ) p4jetsWOMatchBjets_1cut50 = compare2VectorsTLorentzVectors( p4MatchTruthAlljets, p4MatchBjetsSmallMassDiff_1cut50 );
			if ( p4MatchBjetsSmallMassDiff_1cut100.size() == 4 ) p4jetsWOMatchBjets_1cut100 = compare2VectorsTLorentzVectors( p4MatchTruthAlljets, p4MatchBjetsSmallMassDiff_1cut100 );
			if ( p4MatchBjetsSmallMassDiff_1cut150.size() == 4 ) p4jetsWOMatchBjets_1cut150 = compare2VectorsTLorentzVectors( p4MatchTruthAlljets, p4MatchBjetsSmallMassDiff_1cut150 );
			if ( p4MatchBjetsSmallMassDiff_1cut200.size() == 4 ) p4jetsWOMatchBjets_1cut200 = compare2VectorsTLorentzVectors( p4MatchTruthAlljets, p4MatchBjetsSmallMassDiff_1cut200 );
		}
	//cout << " 10 " << p4jetsWOMatchBjets.size() << " " << p4jetsWOMatchBjets_1cut50.size() << " " << p4jetsWOMatchBjets_1cut100.size() << " " << p4jetsWOMatchBjets_1cut150.size() << " " << p4jetsWOMatchBjets_1cut200.size() << " " << endl;
			
	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWOMatchBjets;
	vectorsjWOMatchBjets = s3Vectors( p4jetsWOMatchBjets );
	vector< TLorentzVector > p4matchdijetWOMatchBjets = vectorsjWOMatchBjets.s3VectorTL;
	vector< double > massmatchdijetWOMatchBjets = vectorsjWOMatchBjets.s3Mass;
	vector< double > scalarSumPtmatchdijetWOMatchBjets = vectorsjWOMatchBjets.s3ScalarPt;

	for(unsigned int iim = 0; iim < massmatchdijetWOMatchBjets.size(); ++iim) {
		h_matchdijetWOMatchBjets_masspt->Fill( scalarSumPtmatchdijetWOMatchBjets[iim], massmatchdijetWOMatchBjets[iim] );
		h_massmatchdijetWOMatchBjets->Fill( massmatchdijetWOMatchBjets[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		/*if( p4jetsWOMatchBjets[iim] < ( scalarSumPtmatchdijetWOMatchBjets[iim]-iDiag ) ) {
		}*/
	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWOMatchBjets_1cut50;
	vectorsjWOMatchBjets_1cut50 = s3Vectors( p4jetsWOMatchBjets_1cut50 );
	vector< TLorentzVector > p4matchdijetWOMatchBjets_1cut50 = vectorsjWOMatchBjets_1cut50.s3VectorTL;
	vector< double > massmatchdijetWOMatchBjets_1cut50 = vectorsjWOMatchBjets_1cut50.s3Mass;
	vector< double > scalarSumPtmatchdijetWOMatchBjets_1cut50 = vectorsjWOMatchBjets_1cut50.s3ScalarPt;

	for(unsigned int iim = 0; iim < massmatchdijetWOMatchBjets_1cut50.size(); ++iim) {
		h_matchdijetWOMatchBjets_masspt_1cut50->Fill( scalarSumPtmatchdijetWOMatchBjets_1cut50[iim], massmatchdijetWOMatchBjets_1cut50[iim] );
		h_massmatchdijetWOMatchBjets_1cut50->Fill( massmatchdijetWOMatchBjets_1cut50[iim] );

	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWOMatchBjets_1cut100;
	vectorsjWOMatchBjets_1cut100 = s3Vectors( p4jetsWOMatchBjets_1cut100 );
	vector< TLorentzVector > p4matchdijetWOMatchBjets_1cut100 = vectorsjWOMatchBjets_1cut100.s3VectorTL;
	vector< double > massmatchdijetWOMatchBjets_1cut100 = vectorsjWOMatchBjets_1cut100.s3Mass;
	vector< double > scalarSumPtmatchdijetWOMatchBjets_1cut100 = vectorsjWOMatchBjets_1cut100.s3ScalarPt;

	for(unsigned int iim = 0; iim < massmatchdijetWOMatchBjets_1cut100.size(); ++iim) {
		h_matchdijetWOMatchBjets_masspt_1cut100->Fill( scalarSumPtmatchdijetWOMatchBjets_1cut100[iim], massmatchdijetWOMatchBjets_1cut100[iim] );
		h_massmatchdijetWOMatchBjets_1cut100->Fill( massmatchdijetWOMatchBjets_1cut100[iim] );
	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWOMatchBjets_1cut150;
	vectorsjWOMatchBjets_1cut150 = s3Vectors( p4jetsWOMatchBjets_1cut150 );
	vector< TLorentzVector > p4matchdijetWOMatchBjets_1cut150 = vectorsjWOMatchBjets_1cut150.s3VectorTL;
	vector< double > massmatchdijetWOMatchBjets_1cut150 = vectorsjWOMatchBjets_1cut150.s3Mass;
	vector< double > scalarSumPtmatchdijetWOMatchBjets_1cut150 = vectorsjWOMatchBjets_1cut150.s3ScalarPt;

	for(unsigned int iim = 0; iim < massmatchdijetWOMatchBjets_1cut150.size(); ++iim) {
		h_matchdijetWOMatchBjets_masspt_1cut150->Fill( scalarSumPtmatchdijetWOMatchBjets_1cut150[iim], massmatchdijetWOMatchBjets_1cut150[iim] );
		h_massmatchdijetWOMatchBjets_1cut150->Fill( massmatchdijetWOMatchBjets_1cut150[iim] );

	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWOMatchBjets_1cut200;
	vectorsjWOMatchBjets_1cut200 = s3Vectors( p4jetsWOMatchBjets_1cut200 );
	vector< TLorentzVector > p4matchdijetWOMatchBjets_1cut200 = vectorsjWOMatchBjets_1cut200.s3VectorTL;
	vector< double > massmatchdijetWOMatchBjets_1cut200 = vectorsjWOMatchBjets_1cut200.s3Mass;
	vector< double > scalarSumPtmatchdijetWOMatchBjets_1cut200 = vectorsjWOMatchBjets_1cut200.s3ScalarPt;

	for(unsigned int iim = 0; iim < massmatchdijetWOMatchBjets_1cut200.size(); ++iim) {
		h_matchdijetWOMatchBjets_masspt_1cut200->Fill( scalarSumPtmatchdijetWOMatchBjets_1cut200[iim], massmatchdijetWOMatchBjets_1cut200[iim] );
		h_massmatchdijetWOMatchBjets_1cut200->Fill( massmatchdijetWOMatchBjets_1cut200[iim] );

	}
	//////////////////////////////////////////////////////////////////////////////////


	///// Match jets from Higgs
	vector< TLorentzVector > p4jetsWOMatchBjetsHiggs;
	vector< TLorentzVector > p4jetsWOMatchBjetsHiggs_1cut50;
	vector< TLorentzVector > p4jetsWOMatchBjetsHiggs_1cut100;
	vector< TLorentzVector > p4jetsWOMatchBjetsHiggs_1cut150;
	vector< TLorentzVector > p4jetsWOMatchBjetsHiggs_1cut200;

	if ( p4MatchTruthAlljets.size() > 5 && p4MatchB1Higgs.size() == 2 && p4MatchB2Higgs.size() == 2 ){
		for(unsigned int iii = 0; iii < p4MatchTruthAlljets.size(); ++iii) {

			////// Step 2 - dijet mass vs sum pt of jets
			for(unsigned int iij = 0; iij < p4MatchTruthAlljets.size(); ++iij) {
				if ( iii>=iij ) continue;
				TLorentzVector p4dijetHiggs = p4MatchTruthAlljets[iii] + p4MatchTruthAlljets[iij];
				double sumdijetHiggspt = p4MatchTruthAlljets[iii].Pt() + p4MatchTruthAlljets[iij].Pt();
				h_matchdijetHiggs_masspt->Fill( sumdijetHiggspt, p4dijetHiggs.M() );
			}
			
			////// Step 3 - dijet mass vs sum pt of jets without bs from step 1
			if ( p4MatchBjetsHiggsSmallMassDiff.size() == 4 ) p4jetsWOMatchBjetsHiggs = compare2VectorsTLorentzVectors( p4MatchTruthAlljets, p4MatchBjetsHiggsSmallMassDiff );
			if ( p4MatchBjetsHiggsSmallMassDiff_1cut50.size() == 4 ) p4jetsWOMatchBjetsHiggs_1cut50 = compare2VectorsTLorentzVectors( p4MatchTruthAlljets, p4MatchBjetsHiggsSmallMassDiff_1cut50 );
			if ( p4MatchBjetsHiggsSmallMassDiff_1cut100.size() == 4 ) p4jetsWOMatchBjetsHiggs_1cut100 = compare2VectorsTLorentzVectors( p4MatchTruthAlljets, p4MatchBjetsHiggsSmallMassDiff_1cut100 );
			if ( p4MatchBjetsHiggsSmallMassDiff_1cut150.size() == 4 ) p4jetsWOMatchBjetsHiggs_1cut150 = compare2VectorsTLorentzVectors( p4MatchTruthAlljets, p4MatchBjetsHiggsSmallMassDiff_1cut150 );
			if ( p4MatchBjetsHiggsSmallMassDiff_1cut200.size() == 4 ) p4jetsWOMatchBjetsHiggs_1cut200 = compare2VectorsTLorentzVectors( p4MatchTruthAlljets, p4MatchBjetsHiggsSmallMassDiff_1cut200 );
		}
	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWOMatchBjetsHiggs;
	vectorsjWOMatchBjetsHiggs = s3Vectors( p4jetsWOMatchBjetsHiggs );
	vector< TLorentzVector > p4matchdijetWOMatchBjetsHiggs = vectorsjWOMatchBjetsHiggs.s3VectorTL;
	vector< double > massmatchdijetWOMatchBjetsHiggs = vectorsjWOMatchBjetsHiggs.s3Mass;
	vector< double > scalarSumPtmatchdijetWOMatchBjetsHiggs = vectorsjWOMatchBjetsHiggs.s3ScalarPt;

	for(unsigned int iim = 0; iim < massmatchdijetWOMatchBjetsHiggs.size(); ++iim) {
		h_matchdijetWOMatchBjetsHiggs_masspt->Fill( scalarSumPtmatchdijetWOMatchBjetsHiggs[iim], massmatchdijetWOMatchBjetsHiggs[iim] );
		h_massmatchdijetWOMatchBjetsHiggs->Fill( massmatchdijetWOMatchBjetsHiggs[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		/*if( p4jetsWOMatchBjetsHiggs[iim] < ( scalarSumPtmatchdijetWOMatchBjetsHiggs[iim]-iDiag ) ) {
		}*/
	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWOMatchBjetsHiggs_1cut50;
	vectorsjWOMatchBjetsHiggs_1cut50 = s3Vectors( p4jetsWOMatchBjetsHiggs_1cut50 );
	vector< TLorentzVector > p4matchdijetWOMatchBjetsHiggs_1cut50 = vectorsjWOMatchBjetsHiggs_1cut50.s3VectorTL;
	vector< double > massmatchdijetWOMatchBjetsHiggs_1cut50 = vectorsjWOMatchBjetsHiggs_1cut50.s3Mass;
	vector< double > scalarSumPtmatchdijetWOMatchBjetsHiggs_1cut50 = vectorsjWOMatchBjetsHiggs_1cut50.s3ScalarPt;

	for(unsigned int iim = 0; iim < massmatchdijetWOMatchBjetsHiggs_1cut50.size(); ++iim) {
		h_matchdijetWOMatchBjetsHiggs_masspt_1cut50->Fill( scalarSumPtmatchdijetWOMatchBjetsHiggs_1cut50[iim], massmatchdijetWOMatchBjetsHiggs_1cut50[iim] );
		h_massmatchdijetWOMatchBjetsHiggs_1cut50->Fill( massmatchdijetWOMatchBjetsHiggs_1cut50[iim] );

	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWOMatchBjetsHiggs_1cut100;
	vectorsjWOMatchBjetsHiggs_1cut100 = s3Vectors( p4jetsWOMatchBjetsHiggs_1cut100 );
	vector< TLorentzVector > p4matchdijetWOMatchBjetsHiggs_1cut100 = vectorsjWOMatchBjetsHiggs_1cut100.s3VectorTL;
	vector< double > massmatchdijetWOMatchBjetsHiggs_1cut100 = vectorsjWOMatchBjetsHiggs_1cut100.s3Mass;
	vector< double > scalarSumPtmatchdijetWOMatchBjetsHiggs_1cut100 = vectorsjWOMatchBjetsHiggs_1cut100.s3ScalarPt;

	for(unsigned int iim = 0; iim < massmatchdijetWOMatchBjetsHiggs_1cut100.size(); ++iim) {
		h_matchdijetWOMatchBjetsHiggs_masspt_1cut100->Fill( scalarSumPtmatchdijetWOMatchBjetsHiggs_1cut100[iim], massmatchdijetWOMatchBjetsHiggs_1cut100[iim] );
		h_massmatchdijetWOMatchBjetsHiggs_1cut100->Fill( massmatchdijetWOMatchBjetsHiggs_1cut100[iim] );
	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWOMatchBjetsHiggs_1cut150;
	vectorsjWOMatchBjetsHiggs_1cut150 = s3Vectors( p4jetsWOMatchBjetsHiggs_1cut150 );
	vector< TLorentzVector > p4matchdijetWOMatchBjetsHiggs_1cut150 = vectorsjWOMatchBjetsHiggs_1cut150.s3VectorTL;
	vector< double > massmatchdijetWOMatchBjetsHiggs_1cut150 = vectorsjWOMatchBjetsHiggs_1cut150.s3Mass;
	vector< double > scalarSumPtmatchdijetWOMatchBjetsHiggs_1cut150 = vectorsjWOMatchBjetsHiggs_1cut150.s3ScalarPt;

	for(unsigned int iim = 0; iim < massmatchdijetWOMatchBjetsHiggs_1cut150.size(); ++iim) {
		h_matchdijetWOMatchBjetsHiggs_masspt_1cut150->Fill( scalarSumPtmatchdijetWOMatchBjetsHiggs_1cut150[iim], massmatchdijetWOMatchBjetsHiggs_1cut150[iim] );
		h_massmatchdijetWOMatchBjetsHiggs_1cut150->Fill( massmatchdijetWOMatchBjetsHiggs_1cut150[iim] );

	}

	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWOMatchBjetsHiggs_1cut200;
	vectorsjWOMatchBjetsHiggs_1cut200 = s3Vectors( p4jetsWOMatchBjetsHiggs_1cut200 );
	vector< TLorentzVector > p4matchdijetWOMatchBjetsHiggs_1cut200 = vectorsjWOMatchBjetsHiggs_1cut200.s3VectorTL;
	vector< double > massmatchdijetWOMatchBjetsHiggs_1cut200 = vectorsjWOMatchBjetsHiggs_1cut200.s3Mass;
	vector< double > scalarSumPtmatchdijetWOMatchBjetsHiggs_1cut200 = vectorsjWOMatchBjetsHiggs_1cut200.s3ScalarPt;

	for(unsigned int iim = 0; iim < massmatchdijetWOMatchBjetsHiggs_1cut200.size(); ++iim) {
		h_matchdijetWOMatchBjetsHiggs_masspt_1cut200->Fill( scalarSumPtmatchdijetWOMatchBjetsHiggs_1cut200[iim], massmatchdijetWOMatchBjetsHiggs_1cut200[iim] );
		h_massmatchdijetWOMatchBjetsHiggs_1cut200->Fill( massmatchdijetWOMatchBjetsHiggs_1cut200[iim] );

	}
			
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	//////           STEP 4                                    /////
	//////  Mass of MatchBjets from 1 and dijet from 3 vs sum pt of MatchBjetsdijet  /////
	////////////////////////////////////////////////////////////////
	
	/*////// Reco Jets
	for(unsigned int iiii = 0; iiii < p4RecoMatchBjetsSmallMassDiff.size(); ++iiii) {
		for(unsigned int iiij = 0; iiij < p4RecoMatchBjetsSmallMassDiff.size(); ++iiij) {
			if ( iiii == iiij ) continue;
			for(unsigned int iidijet = 0; iidijet < p4recojWOMatchBjets.size(); ++iidijet) {
				for(unsigned int idijetj = 0; idijetj < p4recojWOMatchBjets.size(); ++idijetj) {
					if ( iidijet == idijetj ) continue;
					TLorentzVector p4recoMatchBjetsdijetMass = p4RecoMatchBjetsSmallMassDiff[iiii] + p4RecoMatchBjetsSmallMassDiff[iiij] + p4recojWOMatchBjets[iidijet] + p4recojWOMatchBjets[idijetj];
					double sumrecoMatchBjetsdijetPt = p4RecoMatchBjetsSmallMassDiff[iiii].Pt() + p4RecoMatchBjetsSmallMassDiff[iiij].Pt() + p4recojWOMatchBjets[iidijet].Pt() + p4recojWOMatchBjets[idijetj].Pt();
					h_recoMatchBjetsdijet_masspt->Fill( p4recoMatchBjetsdijetMass.M(), sumrecoMatchBjetsdijetPt );
				}
			}
		}
	}
	////////////////////////////
	
	/////// Match Jets
	for(unsigned int iiii = 0; iiii < p4MatchBjetsSmallMassDiff.size(); ++iiii) {
		for(unsigned int iiij = 0; iiij < p4MatchBjetsSmallMassDiff.size(); ++iiij) {
			if ( iiii == iiij ) continue;
			for(unsigned int iidijet = 0; iidijet < p4jetsWOMatchBjets.size(); ++iidijet) {
				for(unsigned int idijetj = 0; idijetj < p4jetsWOMatchBjets.size(); ++idijetj) {
					if ( iidijet == idijetj ) continue;
					TLorentzVector p4MatchBjetsdijetMass = p4MatchBjetsSmallMassDiff[iiii] + p4MatchBjetsSmallMassDiff[iiij] + p4jetsWOMatchBjets[iidijet] + p4jetsWOMatchBjets[idijetj];
					double sumMatchBjetsdijetPt = p4MatchBjetsSmallMassDiff[iiii].Pt() + p4MatchBjetsSmallMassDiff[iiij].Pt() + p4jetsWOMatchBjets[iidijet].Pt() + p4jetsWOMatchBjets[idijetj].Pt();
					h_MatchBjetsdijet_masspt->Fill( p4MatchBjetsdijetMass.M(), sumMatchBjetsdijetPt );
				}
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////*/
	

	////////////////////////////////////////////////////////////////
	//////           STEP 5                                    /////
	//////  Smallest mass MatchBjetsdijet                                 /////
	////////////////////////////////////////////////////////////////
	
	/*////// Reco Jets
	for(unsigned int dijetdijet = 0; dijetdijet < p4RecoMatchBjetsSmallMassDiff.size()-3; ++dijetdijet) {
		for(unsigned int dijetjk = 1; dijetjk < p4RecoMatchBjetsSmallMassDiff.size()-2; ++dijetjk) {
			if ( dijetdijet == dijetjk ) continue;
			for(unsigned int iidijet = 0; iidijet < p4recojWOMatchBjets.size(); ++iidijet) {
				for(unsigned int idijetj = 0; idijetj < p4recojWOMatchBjets.size(); ++idijetj) {
					if ( iidijet == idijetj ) continue;
					TLorentzVector p4recoMatchBjetsdijetMass = p4RecoMatchBjetsSmallMassDiff[iiii] + p4RecoMatchBjetsSmallMassDiff[iiij] + p4recojWOMatchBjets[iidijet] + p4recojWOMatchBjets[idijetj];
					double sumrecoMatchBjetsdijetPt = p4RecoMatchBjetsSmallMassDiff[iiii].Pt() + p4RecoMatchBjetsSmallMassDiff[iiij].Pt() + p4recojWOMatchBjets[iidijet].Pt() + p4recojWOMatchBjets[idijetj].Pt();
					h_massrecoMatchBjetsdijet->Fill( p4recoMatchBjetsdijetMass.M(), sumrecoMatchBjetsdijetPt );
				}
			}
		}
	}
	///////////////////////////*/


}

//define this as a plug-in
DEFINE_FWK_MODULE(MyAnalyzer);
