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
	vector< TLorentzVector > p4RecoJets;
	vector< TLorentzVector > p4RecoBjets;
	vector< TLorentzVector > p4GenTruthAlljets;
	vector< TLorentzVector > p4GenTruthJets;
	vector< TLorentzVector > p4GenTruthBjets; 
	vector< TLorentzVector > p4GenAlljets;
	vector< TLorentzVector > p4GenJets;
	vector< TLorentzVector > p4GenBjets; 
	vector< TLorentzVector > p4GenbjetHiggs;
	vector< TLorentzVector > p4GenHiggs;
	vector< TLorentzVector > p4GenJetsStop1;
	vector< TLorentzVector > p4GenbjetStop1;
	vector< TLorentzVector > p4GenStop1;
	vector< TLorentzVector > p4GenStop2;
	vector< TLorentzVector > p4MatchTruthGenAlljets;
	vector< TLorentzVector > p4MatchTruthGenJets;
	vector< TLorentzVector > p4MatchTruthGenBjets;
	vector< TLorentzVector > p4MatchTruthAlljets;
	vector< TLorentzVector > p4MatchTruthJets;
	vector< TLorentzVector > p4MatchTruthBjets;
	vector< TLorentzVector > p4MatchAlljets;
	vector< TLorentzVector > p4MatchJets;
	vector< TLorentzVector > p4MatchBjets;
	vector< TLorentzVector > p4MatchBjetsHiggs;
	//////////////////////////////////////////////////


	//////////// GET EVT INFO
	run   = iEvent.id().run();
	event = iEvent.id().event();
	lumis = iEvent.id().luminosityBlock();
	//std::cout << run << " " << event << std::endl;
	//////////////////////////////////////////////////

	////////////////////////////
	//  GenParticles
	////////////////////////////
	for(unsigned int i = 0; i < particles->size(); ++ i) {

		if( p[i].status()!= 3 ) continue;				// Make sure only "final" particles are present
		const Candidate * mom = p[i].mother();				// call mother particle

		//////////// Storing TLorentz Vectors
		TLorentzVector tmpGenTruthAlljets, tmpGenTruthBjets, tmpGenTruthJets;
		if( mom ){
			if ( p[i].pt() > 20.0 && fabs( p[i].eta() ) < 3.0){
				// AllJets 
				if( ( abs( p[i].pdgId() ) == 1 ) || ( abs( p[i].pdgId() ) == 2 ) || ( abs( p[i].pdgId() ) == 3 ) || ( abs( p[i].pdgId() ) == 4) || ( abs( p[i].pdgId() ) == 5 ) || ( abs( p[i].pdgId() ) == 21 )){
					tmpGenTruthAlljets.SetPxPyPzE(p[i].px(),p[i].py(),p[i].pz(),p[i].energy());
					p4GenTruthAlljets.push_back( tmpGenTruthAlljets );
				}
				// Jets w/o b
				if( ( abs( p[i].pdgId() ) == 1 ) || ( abs( p[i].pdgId() ) == 2 ) || ( abs( p[i].pdgId() ) == 3 ) || ( abs( p[i].pdgId() ) == 4) || ( abs( p[i].pdgId() ) == 21 )){
					tmpGenTruthJets.SetPxPyPzE(p[i].px(),p[i].py(),p[i].pz(),p[i].energy());
					p4GenTruthJets.push_back( tmpGenTruthJets );
				} 
				// Bjets
				if( abs( p[i].pdgId() ) == 5 ){
					tmpGenTruthBjets.SetPxPyPzE(p[i].px(),p[i].py(),p[i].pz(),p[i].energy());
					p4GenTruthBjets.push_back( tmpGenTruthBjets );
				}
			}
		}
		std::sort(p4GenTruthAlljets.begin(), p4GenTruthAlljets.end(), ComparePt);
		std::sort(p4GenTruthBjets.begin(), p4GenTruthBjets.end(), ComparePt);
		std::sort(p4GenTruthJets.begin(), p4GenTruthJets.end(), ComparePt);
	}
	//cout << p4GenTruthAlljets[0].Pt() << " "  << p4GenTruthBjets[0].Pt() << " "  << p4GenTruthJets[0].Pt() << endl;
	/////////////////////////////////////////////////

	///////////////////////////
	//  Jet Corrections
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
	// Loop over corrected jets
	///////////////////////////////////
	
	double bdiscCSV_PF = -999;
	for (std::list<jetElem>::const_iterator correctedJet = adjJetList.begin(); correctedJet != adjJetList.end(); ++correctedJet) {
		
		//const reco::Candidate::LorentzVector *adjJet = &(correctedJet->adjJet); 	// Only TLorentzVector
		const pat::Jet *Jet = correctedJet->origJet;					// pat::Jet info (whole info)

		/////////// RECO Store jets and bjets
		bdiscCSV_PF=Jet->bDiscriminator("combinedSecondaryVertexBJetTags");		// define btagging CSV
		TLorentzVector tmpRecoJets, tmpRecoBjets;

		if ( Jet->pt() > 20.0 && fabs( Jet->eta() ) < 3.0){
			tmpRecoJets.SetPtEtaPhiM( Jet->pt(), Jet->eta(), Jet->phi(), Jet->mass() );
			p4RecoJets.push_back( tmpRecoJets );

			if ( abs( Jet->partonFlavour() ) == 5 ){
			//if ( bdiscCSV_PF > 0.679  ){
				tmpRecoBjets.SetPxPyPzE( Jet->px(), Jet->py(), Jet->pz(), Jet->energy() );
				p4RecoBjets.push_back( tmpRecoBjets );
				//cout << Jet->partonFlavour() << " " << Jet->pt() << " " << tmpbjets.Pt() << " " << bdiscCSV_PF << endl;
			}
		}
		std::sort(p4RecoJets.begin(), p4RecoJets.end(), ComparePt);
		std::sort(p4RecoBjets.begin(), p4RecoBjets.end(), ComparePt);
		////////////////////////////////////////////////////////////////////////////////////////////
	

		/// Add here some definitions for btagging, bdisc, num of daughters, etc...
		//


		////////////////// GenParticle
		// In future, check if is data or not
		//int jetMom = -1; 
		const reco::GenParticle * part = Jet->genParton();

		if (part){
			//cout<<"GenParton: "<<part->pdgId()<<endl;
			const reco::GenParticle * mom = (const reco::GenParticle*) (*part).mother();

			////////////// Store Parton Info
			TLorentzVector tmpGenAllJets, tmpGenJets, tmpGenBjets, tmpGenbjetHiggs, tmpGenbjetStop1, tmpGenjetStop1;

				/*//////////////////////////////// TEST
				if (mom->pdgId() == 25){
					if ( Jet->pt() > 20.0 && fabs( Jet->eta() ) < 3.0){
						if ( abs( Jet->partonFlavour() ) == 5 ){
						//if ( bdiscCSV_PF > 0.679  ){
					       tmpGenbjetHiggs.SetPtEtaPhiE( Jet->px(), Jet->py(), Jet->pz(), Jet->energy() );
					       cout << Jet->partonFlavour() << " " << tmpGenbjetHiggs.Pt() << endl;
					       p4GenbjetHiggs.push_back( tmpGenbjetHiggs );
						}
					}
				} 
				///////////////////////////////////////////////////////////////////////////////////////*/

			/*////// This is part of Claudia's code... I am not sure where should I use it.
			//cout<<mom->pdgId()<<endl;
			for (int y = 0; y < int(parents->size()); ++y){
				//cout << (*parents)[y].p() << endl;
				if (fabs((*parents)[y].p() - (*part).mother()->p()) < 0.001) jetMom = y;
			}

			
			if (jetMom == -1){
				jetMom = int(parents->size());
				reco::GenParticle cand(*mom);
				parents->push_back(cand);
				//std::cout << "Found Mom with number of daughters: " << parents->size() << std::endl;
			}///////////////////////////////////////////////////*/



			if ( part->pt() > 20.0 && fabs( part->eta() ) < 3.0){
				//// Jets with Bjets
				if ( abs(part->pdgId() ) == 1 || abs(part->pdgId() ) == 2 || abs(part->pdgId() ) == 3 || abs(part->pdgId() ) == 4 || abs(part->pdgId() ) == 5 || abs(part->pdgId() ) == 21){
				       tmpGenAllJets.SetPtEtaPhiE( part->pt(), part->eta(), part->phi(), part->energy()  );
				       p4GenAlljets.push_back( tmpGenAllJets );
				}

				//// Jets without Bjets
				if ( abs(part->pdgId() ) == 1 || abs(part->pdgId() ) == 2 || abs(part->pdgId() ) == 3 || abs(part->pdgId() ) == 4 || abs(part->pdgId() ) == 21){
				       tmpGenJets.SetPtEtaPhiE( part->pt(), part->eta(), part->phi(), part->energy()  );
				       p4GenJets.push_back( tmpGenJets );
				}

				//// Bjets
				if ( abs( part->pdgId() ) == 5 ){
				       tmpGenBjets.SetPtEtaPhiE( part->pt(), part->eta(), part->phi(), part->energy()  );
				       p4GenBjets.push_back( tmpGenBjets );
				}

				////// Store components of Higgs
				if (mom->pdgId() == 25){
					if ( abs(part->pdgId() ) == 5 ){
					       tmpGenbjetHiggs.SetPtEtaPhiE( part->pt(), part->eta(), part->phi(), part->energy()  );
					       p4GenbjetHiggs.push_back( tmpGenbjetHiggs );
					}
				} 

				////// Store components of Stop1
				if ( abs(mom->pdgId() ) == 1000002 ){
					if ( abs(part->pdgId() ) == 5 ){
					       tmpGenbjetStop1.SetPtEtaPhiE( part->pt(), part->eta(), part->phi(), part->energy()  );
					       p4GenbjetStop1.push_back( tmpGenbjetStop1 );
					}
					if ( abs(part->pdgId() ) == 1 || abs(part->pdgId() ) == 2 || abs(part->pdgId() ) == 3 || abs(part->pdgId() ) == 4 ){
					       tmpGenjetStop1.SetPtEtaPhiE( part->pt(), part->eta(), part->phi(), part->energy()  );
					       p4GenJetsStop1.push_back( tmpGenjetStop1 );
					}
				} 
				std::sort(p4GenAlljets.begin(), p4GenAlljets.end(), ComparePt);
				std::sort(p4GenJets.begin(), p4GenJets.end(), ComparePt);
				std::sort(p4GenBjets.begin(), p4GenBjets.end(), ComparePt);
				std::sort(p4GenbjetHiggs.begin(), p4GenbjetHiggs.end(), ComparePt);
				std::sort(p4GenbjetStop1.begin(), p4GenbjetStop1.end(), ComparePt);
				std::sort(p4GenJetsStop1.begin(), p4GenJetsStop1.end(), ComparePt);
			}
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	


	//////////////////// Matching with genParticles
	// All Jets + b
	for(unsigned int j = 0; j < p4RecoJets.size(); ++j) {
		double deltaRTruthAlljets = 99999;
		for(unsigned int i = 0; i < p4GenTruthAlljets.size(); ++i) {
			double tmpdeltaRTruthAlljets = p4GenTruthAlljets[i].DeltaR( p4RecoJets[j] );
			if ( tmpdeltaRTruthAlljets < deltaRTruthAlljets ) deltaRTruthAlljets = tmpdeltaRTruthAlljets;
		}
		h_MatchTruthAlljets_deltaR->Fill( deltaRTruthAlljets );
		if ( deltaRTruthAlljets < 0.4 ) p4MatchTruthAlljets.push_back( p4RecoJets[j] );   
	}

	// Jets
	for(unsigned int j = 0; j < p4RecoJets.size(); ++j) {
		double deltaRTruthjets = 99999;
		for(unsigned int i = 0; i < p4GenTruthJets.size(); ++i) {
			double tmpdeltaRTruthjets = p4GenTruthJets[i].DeltaR( p4RecoJets[j] );
			if ( tmpdeltaRTruthjets < deltaRTruthjets ) deltaRTruthjets = tmpdeltaRTruthjets;
		}	
		h_MatchTruthJets_deltaR->Fill( deltaRTruthjets );
		if ( deltaRTruthjets < 0.4 ) p4MatchTruthJets.push_back( p4RecoJets[j] );   
	}

	// Bjets
	for(unsigned int j = 0; j < p4RecoBjets.size(); ++j) {
		double deltaRTruthBjets = 999999;
		for(unsigned int i = 0; i < p4GenTruthBjets.size(); ++i) {
			double tmpdeltaRTruthBjets = p4GenTruthBjets[i].DeltaR( p4RecoBjets[j] );
			if ( tmpdeltaRTruthBjets < deltaRTruthBjets ) deltaRTruthBjets = tmpdeltaRTruthBjets;
		}
		h_MatchTruthBjets_deltaR->Fill( deltaRTruthBjets );
		if ( deltaRTruthBjets < 0.4 ) p4MatchTruthBjets.push_back( p4RecoBjets[j] );   
	}
	std::sort(p4MatchTruthAlljets.begin(), p4MatchTruthAlljets.end(), ComparePt);
	std::sort(p4MatchTruthJets.begin(), p4MatchTruthJets.end(), ComparePt);
	std::sort(p4MatchTruthBjets.begin(), p4MatchTruthBjets.end(), ComparePt);
	/////////////////////////////////////////////////////////////////
	
	//////////////////// Matching with genParticles and Pat::GenParticle
	// All Jets + b
	for(unsigned int j = 0; j < p4GenAlljets.size(); ++j) {
		double deltaRTruthGenAlljets = 99999;
		for(unsigned int i = 0; i < p4GenTruthAlljets.size(); ++i) {
			double tmpdeltaRTruthGenAlljets = p4GenTruthAlljets[i].DeltaR( p4GenAlljets[j] );
			if ( tmpdeltaRTruthGenAlljets < deltaRTruthGenAlljets ) deltaRTruthGenAlljets = tmpdeltaRTruthGenAlljets;
		}
		h_MatchTruthGenAlljets_deltaR->Fill( deltaRTruthGenAlljets );
		if ( deltaRTruthGenAlljets < 0.4 ) p4MatchTruthGenAlljets.push_back( p4GenAlljets[j] );   
	}

	// Jets
	for(unsigned int j = 0; j < p4GenAlljets.size(); ++j) {
		double deltaRTruthGenjets = 99999;
		for(unsigned int i = 0; i < p4GenTruthJets.size(); ++i) {
			double tmpdeltaRTruthGenjets = p4GenTruthJets[i].DeltaR( p4GenAlljets[j] );
			if ( tmpdeltaRTruthGenjets < deltaRTruthGenjets ) deltaRTruthGenjets = tmpdeltaRTruthGenjets;
		}	
		h_MatchTruthGenJets_deltaR->Fill( deltaRTruthGenjets );
		if ( deltaRTruthGenjets < 0.4 ) p4MatchTruthGenJets.push_back( p4GenAlljets[j] );   
	}

	// Bjets
	for(unsigned int j = 0; j < p4GenAlljets.size(); ++j) {
		double deltaRTruthGenBjets = 999999;
		for(unsigned int i = 0; i < p4GenTruthBjets.size(); ++i) {
			double tmpdeltaRTruthGenBjets = p4GenTruthBjets[i].DeltaR( p4GenAlljets[j] );
			if ( tmpdeltaRTruthGenBjets < deltaRTruthGenBjets ) deltaRTruthGenBjets = tmpdeltaRTruthGenBjets;
		}
		h_MatchTruthGenBjets_deltaR->Fill( deltaRTruthGenBjets );
		if ( deltaRTruthGenBjets < 0.4 ) p4MatchTruthGenBjets.push_back( p4GenAlljets[j] );   
	}
	std::sort(p4MatchTruthGenAlljets.begin(), p4MatchTruthGenAlljets.end(), ComparePt);
	std::sort(p4MatchTruthGenJets.begin(), p4MatchTruthGenJets.end(), ComparePt);
	std::sort(p4MatchTruthGenBjets.begin(), p4MatchTruthGenBjets.end(), ComparePt);
	/////////////////////////////////////////////////////////////////
	
	//////////////////// Matching with Pat::Jets
	// All Jets + b
	for(unsigned int j = 0; j < p4RecoJets.size(); ++j) {
		double deltaRAlljets = 99999;
		for(unsigned int i = 0; i < p4GenAlljets.size(); ++i) {
			double tmpdeltaRAlljets = p4GenAlljets[i].DeltaR( p4RecoJets[j] );
			if ( tmpdeltaRAlljets < deltaRAlljets ) deltaRAlljets = tmpdeltaRAlljets;
		}
		h_MatchAlljets_deltaR->Fill( deltaRAlljets );
		if ( deltaRAlljets < 0.4 ) p4MatchAlljets.push_back( p4RecoJets[j] );   
	}

	// Jets
	for(unsigned int j = 0; j < p4RecoJets.size(); ++j) {
		double deltaRjets = 99999;
		for(unsigned int i = 0; i < p4GenJets.size(); ++i) {
			double tmpdeltaRjets = p4GenJets[i].DeltaR( p4RecoJets[j] );
			if ( tmpdeltaRjets < deltaRjets ) deltaRjets = tmpdeltaRjets;
		}	
		h_MatchJets_deltaR->Fill( deltaRjets );
		if ( deltaRjets < 0.4 ) p4MatchJets.push_back( p4RecoJets[j] );   
		//cout << deltaRjets << endl;
	}

	// Bjets
	for(unsigned int j = 0; j < p4RecoBjets.size(); ++j) {
		double deltaRBjets = 999999;
		for(unsigned int i = 0; i < p4GenBjets.size(); ++i) {
			double tmpdeltaRBjets = p4GenBjets[i].DeltaR( p4RecoBjets[j] );
			if ( tmpdeltaRBjets < deltaRBjets ) deltaRBjets = tmpdeltaRBjets;
		}
		h_MatchBjets_deltaR->Fill( deltaRBjets );
		if ( deltaRBjets < 0.4 ) p4MatchBjets.push_back( p4RecoBjets[j] );   
		//cout << deltaRBjets << " " << p4GenBjets[i].Pt() << " " << p4RecoBjets[j].Pt() << endl;
	}
	std::sort(p4MatchAlljets.begin(), p4MatchAlljets.end(), ComparePt);
	std::sort(p4MatchJets.begin(), p4MatchJets.end(), ComparePt);
	std::sort(p4MatchBjets.begin(), p4MatchBjets.end(), ComparePt);
	/////////////////////////////////////////////////////////////////


	//////////////////// Matching with bjets from Higgs
	// Bjets
	for(unsigned int j = 0; j < p4RecoBjets.size(); ++j) {
		double deltaRBjetsHiggs = 999999;
		for(unsigned int i = 0; i < p4GenbjetHiggs.size(); ++i) {
			double tmpdeltaRBjetsHiggs = p4GenbjetHiggs[i].DeltaR( p4RecoBjets[j] );
			if ( tmpdeltaRBjetsHiggs < deltaRBjetsHiggs ) deltaRBjetsHiggs = tmpdeltaRBjetsHiggs;
		}
		h_MatchBjetsHiggs_deltaR->Fill( deltaRBjetsHiggs );
		if ( deltaRBjetsHiggs < 0.4 ) p4MatchBjetsHiggs.push_back( p4RecoBjets[j] );   
	}
	std::sort(p4MatchBjetsHiggs.begin(), p4MatchBjetsHiggs.end(), ComparePt);
	////////////////////////////////////////////////////////////////*/

	// Plot Reconstructed Mass/particles plots
	Plots( p4MatchJets, p4MatchBjets,p4MatchAlljets, p4MatchBjetsHiggs, p4RecoJets, p4RecoBjets, p4GenJets, p4GenBjets, p4GenAlljets, p4GenTruthJets, p4GenTruthBjets, p4GenTruthAlljets, p4GenbjetHiggs, p4GenHiggs, p4GenJetsStop1, p4GenbjetStop1, p4GenStop1, p4GenStop2 );
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


///////////////////////////////////////////////////////////////
/////////////  Reco Mass and some histos
void MyAnalyzer::Plots( vector< TLorentzVector > p4MatchJets, vector< TLorentzVector > p4MatchBjets, vector< TLorentzVector > p4MatchAlljets, vector< TLorentzVector > p4MatchBjetsHiggs, vector< TLorentzVector > p4RecoJets, vector< TLorentzVector > p4RecoBjets, vector< TLorentzVector > p4GenJets, vector< TLorentzVector > p4GenBjets, vector< TLorentzVector > p4GenAlljets,  vector< TLorentzVector > p4GenTruthJets, vector< TLorentzVector > p4GenTruthBjets, vector< TLorentzVector > p4GenTruthAlljets , vector< TLorentzVector > p4bjetHiggs, vector< TLorentzVector > p4Higgs, vector< TLorentzVector > p4jetStop1, vector< TLorentzVector > p4bjetStop1, vector< TLorentzVector > p4Stop1, vector< TLorentzVector > p4Stop2 ){


	//////////////////////////////////////////////////////////////////////
	///////           General Basic Plots                          ///////
	//////////////////////////////////////////////////////////////////////

	//////// Basic plots for All jets
	// Gen Truth Level
	double sumGenTruthAlljetsPt = 0;
	for(unsigned int j = 0; j < p4GenTruthAlljets.size(); ++j) {
		sumGenTruthAlljetsPt += p4GenTruthAlljets[j].Pt();
		h_genTruthAlljets_pt->Fill( p4GenTruthAlljets[j].Pt() );	
		h_genTruthAlljets_eta->Fill( p4GenTruthAlljets[j].Eta() );
		h_genTruthAlljets_phi->Fill(p4GenTruthAlljets[j].Phi());
	} 
	h_genTruthAlljets_sumpt->Fill( sumGenTruthAlljetsPt );	
	h_genTruthAlljets_num->Fill( p4GenTruthAlljets.size() );	

	// Gen Level from PAT
	double sumGenAlljetsPt = 0;
	for(unsigned int j = 0; j < p4GenAlljets.size(); ++j) {
		sumGenAlljetsPt += p4GenAlljets[j].Pt();
		h_genAlljets_pt->Fill( p4GenAlljets[j].Pt() );	
		h_genAlljets_eta->Fill( p4GenAlljets[j].Eta() );
		h_genAlljets_phi->Fill(p4GenAlljets[j].Phi());
	} 
	h_genAlljets_sumpt->Fill( sumGenAlljetsPt );	
	h_genAlljets_num->Fill( p4GenAlljets.size() );	

	// RECO level
	double sumRecoJetsPt = 0;
	for(unsigned int j = 0; j < p4RecoJets.size(); ++j) {
		sumRecoJetsPt += p4RecoJets[j].Pt();
		h_recoJets_pt->Fill( p4RecoJets[j].Pt() );	
		h_recoJets_eta->Fill( p4RecoJets[j].Eta() );
		h_recoJets_phi->Fill(p4RecoJets[j].Phi());
	} 
	h_recoJets_sumpt->Fill( sumRecoJetsPt );	
	h_recoJets_num->Fill( p4RecoJets.size() );	

	// Match jets
	double sumMatchAlljetsPt = 0;
	for(unsigned int j = 0; j < p4MatchAlljets.size(); ++j) {
		sumMatchAlljetsPt += p4MatchAlljets[j].Pt();
		h_matchAlljets_pt->Fill( p4MatchAlljets[j].Pt() );	
		h_matchAlljets_eta->Fill( p4MatchAlljets[j].Eta() );
		h_matchAlljets_phi->Fill(p4MatchAlljets[j].Phi());
	} 
	h_matchAlljets_num->Fill( p4MatchAlljets.size() );	
	h_matchAlljets_sumpt->Fill( sumMatchAlljetsPt );	
	//////////////////////////////////////////////////////////

	//////// Basic plots for jets without bs
	// Gen Truth Level
	double sumGenTruthJetsPt = 0;
	for(unsigned int j = 0; j < p4GenTruthJets.size(); ++j) {
		sumGenTruthJetsPt += p4GenTruthJets[j].Pt();
		h_genTruthJets_pt->Fill( p4GenTruthJets[j].Pt() );	
		h_genTruthJets_eta->Fill( p4GenTruthJets[j].Eta() );
		h_genTruthJets_phi->Fill(p4GenTruthJets[j].Phi());
	} 
	h_genTruthJets_sumpt->Fill( sumGenTruthJetsPt );	
	h_genTruthJets_num->Fill( p4GenTruthJets.size() );	

	// Gen Level from PAT
	double sumGenJetsPt = 0;
	for(unsigned int j = 0; j < p4GenJets.size(); ++j) {
		sumGenJetsPt += p4GenJets[j].Pt();
		h_genJets_pt->Fill( p4GenJets[j].Pt() );	
		h_genJets_eta->Fill( p4GenJets[j].Eta() );
		h_genJets_phi->Fill(p4GenJets[j].Phi());
	} 
	h_genJets_sumpt->Fill( sumGenJetsPt );	
	h_genJets_num->Fill( p4GenJets.size() );	

	// Match jets
	double sumMatchJetsPt = 0;
	for(unsigned int j = 0; j < p4MatchJets.size(); ++j) {
		sumMatchJetsPt += p4MatchJets[j].Pt();
		h_matchJets_pt->Fill( p4MatchJets[j].Pt() );	
		h_matchJets_eta->Fill( p4MatchJets[j].Eta() );
		h_matchJets_phi->Fill(p4MatchJets[j].Phi());
	} 
	h_matchJets_num->Fill( p4MatchJets.size() );	
	h_matchJets_sumpt->Fill( sumMatchJetsPt );	
	//////////////////////////////////////////////////////////

	//////// Basic plots for bs 
	// Gen Truth Level
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

	// Gen Level from PAT
	double sumGenBjetsPt = 0;
	if ( p4GenBjets.size() > 0 ){
		for(unsigned int k = 0; k < p4GenBjets.size(); ++k) {
			sumGenBjetsPt += p4GenBjets[k].Pt();
			h_genBJets_pt->Fill( p4GenBjets[k].Pt() );	
			h_genBJets_eta->Fill( p4GenBjets[k].Eta() );
			h_genBJets_phi->Fill( p4GenBjets[k].Phi() );
		}
	}
	h_genBJets_num->Fill( p4GenBjets.size() );	
	h_genBJets_sumpt->Fill( sumGenBjetsPt );	

	// RECO level
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

	// Match bjets
	double sumMatchBjetsPt = 0;
	if ( p4MatchBjets.size() > 0 ){
		for(unsigned int k = 0; k < p4MatchBjets.size(); ++k) {
			sumMatchBjetsPt += p4MatchBjets[k].Pt();
			h_matchBJets_pt->Fill( p4MatchBjets[k].Pt() );	
			h_matchBJets_eta->Fill( p4MatchBjets[k].Eta() );
			h_matchBJets_phi->Fill( p4MatchBjets[k].Phi() );
		}
	}	
	h_matchBJets_num->Fill( p4MatchBjets.size() );	
	h_matchBJets_sumpt->Fill( sumMatchBjetsPt );	

	// Match bjets from Higgs
	double sumMatchBjetsHiggsPt = 0;
	if ( p4MatchBjetsHiggs.size() > 0 ){
		for(unsigned int k = 0; k < p4MatchBjetsHiggs.size(); ++k) {
			sumMatchBjetsHiggsPt += p4MatchBjetsHiggs[k].Pt();
			h_matchBJetsHiggs_pt->Fill( p4MatchBjetsHiggs[k].Pt() );	
			h_matchBJetsHiggs_eta->Fill( p4MatchBjetsHiggs[k].Eta() );
			h_matchBJetsHiggs_phi->Fill( p4MatchBjetsHiggs[k].Phi() );
		}
	}	
	h_matchBJetsHiggs_num->Fill( p4MatchBjetsHiggs.size() );	
	h_matchBJetsHiggs_sumpt->Fill( sumMatchBjetsHiggsPt );	
	/////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	//   Basic Plots PAT::Gen Level                      //
	///////////////////////////////////////////////////////
	
	//////// Basic plots for bs coming from higgs
	if ( p4bjetHiggs.size() > 0 ){
		for(unsigned int k = 0; k < p4bjetHiggs.size(); ++k) {
			h_bjetsHiggs_pt->Fill( p4bjetHiggs[k].Pt() );	
			h_bjetsHiggs_eta->Fill( p4bjetHiggs[k].Eta() );
			h_bjetsHiggs_phi->Fill( p4bjetHiggs[k].Phi() );
		}
	}	

	/////// Higgs reconstructed mass
	vector< TLorentzVector > p4MatchbbsHiggs;
	if ( p4bjetHiggs.size() > 1 ){
		for(unsigned int j = 0; j < p4bjetHiggs.size()-1; ++j) {
			for(unsigned int k=j+1; k < p4bjetHiggs.size(); ++k) {
				if(j==k) continue;							// avoid repetition
				TLorentzVector p4CandidateHiggs = p4bjetHiggs[j] + p4bjetHiggs[k];		// Higgs TLorentzVector
				Double_t massHiggs = 125.0;						// nominal mass value
				Double_t diffmassHiggs = p4CandidateHiggs.M() - massHiggs;		// mass difference
				if( abs(diffmassHiggs) < 0.5 ){						// diffmass presicion
					p4Higgs.push_back( p4CandidateHiggs );				// store final TLorentzVector
					p4MatchbbsHiggs.push_back( p4bjetHiggs[j] );
					p4MatchbbsHiggs.push_back( p4bjetHiggs[k] );
				}
			}
		}
	}
	///// Just to test index of p4MatchbbsHiggs
	/*/if ( p4bbsHiggs.size() > 2 ){
		TLorentzVector tmpp4bbsHiggs1 = p4bbsHiggs[0] + p4bbsHiggs[1];
		TLorentzVector tmpp4bbsHiggs2 = p4bbsHiggs[2] + p4bbsHiggs[3];
		cout << tmpp4bbsHiggs1.M() << " " << tmpp4bbsHiggs2.M() << " " << p4bbsHiggs[0].Pt()  << " " << p4bbsHiggs[1].Pt() << " " << p4bbsHiggs[2].Pt() << " " << p4bbsHiggs[3].Pt() << endl;
	}*/

	if ( p4Higgs.size() > 0 ){
		// Plots for hardest Higgs
		h_higgs1_mass->Fill( p4Higgs[0].M() );
		h_higgs1_pt->Fill( p4Higgs[0].Pt() );	
		h_higgs1_eta->Fill( p4Higgs[0].Eta() );	
		// Plots for softer Higgs
		if ( p4Higgs.size() > 1 ){
		       	h_higgs2_mass->Fill( p4Higgs[1].M() );
		       	h_higgs2_pt->Fill( p4Higgs[1].Pt() );
		       	h_higgs2_eta->Fill( p4Higgs[1].Eta() );
		       	h_higgs_deltaR->Fill( p4Higgs[0].DeltaR( p4Higgs[1] ) );
		       	h_higgs_deltaPhi->Fill( p4Higgs[0].DeltaPhi( p4Higgs[1] ) );
		       	h_higgs_cosDeltaPhi->Fill( cos (p4Higgs[0].DeltaPhi( p4Higgs[1] ) ) );
		}
	}
	///////////////////////////// /


	////// bjets from Stop1 histos
	if ( p4bjetStop1.size() > 0 ){
		for(unsigned int k = 0; k < p4bjetStop1.size(); ++k) {
			h_bjetsStop1_pt->Fill( p4bjetStop1[k].Pt() );	
			h_bjetsStop1_eta->Fill( p4bjetStop1[k].Eta() );
			h_bjetsStop1_phi->Fill( p4bjetStop1[k].Phi() );
		}
	}	

	/////// Stop1 reconstructed mass
	if( st1decay.compare("bj") == 0){
		if( p4bjetStop1.size() > 0 && p4jetStop1.size() > 0 ){
			for(unsigned int j = 0; j < p4bjetStop1.size(); ++j) {
				for(unsigned int k= 0; k < p4jetStop1.size(); ++k) {
					TLorentzVector p4CandidateStop1 = p4bjetStop1[j] + p4jetStop1[k];		// higgs tlorentzvector
					Double_t diffmassStop1 = p4CandidateStop1.M() - stop1Mass;		// mass difference
					if( abs(diffmassStop1) < 1.0 ){						// diffmass presicion
						p4Stop1.push_back( p4CandidateStop1 );
					}
				}
			}
		}
	} else {
		if( p4jetStop1.size() > 0 ){
			for(unsigned int j = 0; j < p4jetStop1.size(); ++j) {
				for(unsigned int k= 0; k < p4jetStop1.size(); ++k) {
					if( j==k ) continue;
					TLorentzVector p4CandidateStop1 = p4jetStop1[j] + p4jetStop1[k];		// higgs tlorentzvector 
					Double_t diffmassStop1 = p4CandidateStop1.M() - stop1Mass;		// mass difference
					if( abs(diffmassStop1) < 1.0 ){						// diffmass presicion
						p4Stop1.push_back( p4CandidateStop1 );
					}
				}
			}
		}
	}


	if ( p4Stop1.size() > 0) {
		// Plots for hardest Stop1
		h_stop11_mass->Fill( p4Stop1[0].M() );
		h_stop11_pt->Fill( p4Stop1[0].Pt() );	
		h_stop11_eta->Fill( p4Stop1[0].Eta() );	

		// Plots for softer Stop1
		if ( p4Stop1.size() > 1 ){
			h_stop12_mass->Fill( p4Stop1[1].M() );
			h_stop12_pt->Fill( p4Stop1[1].Pt() );
		       	h_stop12_eta->Fill( p4Stop1[1].Eta() );
		       	h_stop1_deltaR->Fill( p4Stop1[0].DeltaR( p4Stop1[1] ) );
		       	h_stop1_deltaPhi->Fill( p4Stop1[0].DeltaPhi( p4Stop1[1] ) );
	       		h_stop1_cosDeltaPhi->Fill( cos (p4Stop1[0].DeltaPhi( p4Stop1[1] ) ) );
		}
	}
	/////////////////////////////////////////////////


	// Stop2 reconstructed mass
	for(unsigned int j = 0; j < p4Higgs.size(); ++j) {
		for(unsigned int k= 0; k < p4Stop1.size(); ++k) {
			TLorentzVector p4CandidateStop2 = p4Higgs[j] + p4Stop1[k];		// Higgs TLorentzVector
			Double_t diffmassStop2 = p4CandidateStop2.M() - stop2Mass;		// mass difference
			if( abs(diffmassStop2) < 1.0 ){						// diffmass presicion
				p4Stop2.push_back( p4CandidateStop2 );
			}
		}
	}

	if ( p4Stop2.size() > 0 ){
		// Plots for hardest Stop2
		h_stop21_mass->Fill( p4Stop2[0].M() );
		h_stop21_pt->Fill( p4Stop2[0].Pt() );	
		h_stop21_eta->Fill( p4Stop2[0].Eta() );	
		// Plots for softer Stop2
		if ( p4Stop2.size() > 1 ){
		       	h_stop22_mass->Fill( p4Stop2[1].M() );
		       	h_stop22_pt->Fill( p4Stop2[1].Pt() );
		       	h_stop22_eta->Fill( p4Stop2[1].Eta() );
		}
	}
	//////////////////////////////////////////////////// */
	////////////////////////////////////////////////////////////////////
	//////       STEP 1                                         ////////
	//////  Best combination of bjets with smallest delta Mass  ////////
	////////////////////////////////////////////////////////////////////

	
	///// RecoBJets
	vector< TLorentzVector > p4RecobbSmallMassDiff;
	vector< double > massRecobb;
	vector< double > ptRecobb;
	vector< double > scalarPtRecobb;

	double tmpRecoMassDiff = -9999999;
	Int_t bestRecoMassIndex[4] = {-1, -1, -1, -1};
	int t=0;

	//if ( p4RecoBjets.size() > 3 ){
	if ( p4RecoBjets.size() == 4 ){
		////// Check index of the bs with the smallest mass diff
		for(unsigned int ii = 0; ii < p4RecoBjets.size()-3; ++ii) {
			for(unsigned int ij = 1; ij < p4RecoBjets.size()-2; ++ij) {
				for(unsigned int ik = 2; ik < p4RecoBjets.size()-1; ++ik) {
					for(unsigned int il = 3; il < p4RecoBjets.size(); ++il) {
						if ( ii==ij ) continue;
						if ( ii==ik ) continue;
						if ( ii==il ) continue;
						if ( ij==ik ) continue;
						if ( ij==il ) continue;
						if ( ik==il ) continue;
						TLorentzVector candRecoMassbb1 = p4RecoBjets[ii]+p4RecoBjets[ij];
						double tmpRecomassbb1 = candRecoMassbb1.M();
						TLorentzVector candRecoMassbb2 = p4RecoBjets[ik]+p4RecoBjets[il];
						double tmpRecomassbb2 = candRecoMassbb2.M();
						double massRecoDiff = abs( tmpRecomassbb1 - tmpRecomassbb2 );
						if( tmpRecoMassDiff < massRecoDiff ){
							tmpRecoMassDiff = massRecoDiff;
							bestRecoMassIndex[0] = ii;
							bestRecoMassIndex[1] = ij;
							bestRecoMassIndex[2] = ik;
							bestRecoMassIndex[3] = il;
						}
					}
				}
			}
		}
		
		p4RecobbSmallMassDiff.push_back( p4RecoBjets[bestRecoMassIndex[0]] );
		p4RecobbSmallMassDiff.push_back( p4RecoBjets[bestRecoMassIndex[1]] );
		p4RecobbSmallMassDiff.push_back( p4RecoBjets[bestRecoMassIndex[2]] );
		p4RecobbSmallMassDiff.push_back( p4RecoBjets[bestRecoMassIndex[3]] );
		TLorentzVector tmpRecoMass1 = p4RecoBjets[bestRecoMassIndex[0]] + p4RecoBjets[bestRecoMassIndex[1]];
		TLorentzVector tmpRecoMass2 = p4RecoBjets[bestRecoMassIndex[2]] + p4RecoBjets[bestRecoMassIndex[3]];
		double scalarPtRecobb1 = p4RecoBjets[bestRecoMassIndex[0]].Pt() + p4RecoBjets[bestRecoMassIndex[1]].Pt();
		double scalarPtRecobb2 = p4RecoBjets[bestRecoMassIndex[2]].Pt() + p4RecoBjets[bestRecoMassIndex[3]].Pt();

		////// Store vectors
		massRecobb.push_back ( tmpRecoMass1.M() );
		massRecobb.push_back ( tmpRecoMass2.M() );
		ptRecobb.push_back ( tmpRecoMass1.Pt() );
		ptRecobb.push_back ( tmpRecoMass2.Pt() );
		scalarPtRecobb.push_back ( scalarPtRecobb1 );
		scalarPtRecobb.push_back ( scalarPtRecobb2 );

		///// Simple plots
		h_massRecobb->Fill( massRecobb[t] );
		h_PtRecobb->Fill( ptRecobb[t], massRecobb[t] );
		h_scalarPtRecobb->Fill( scalarPtRecobb[t], massRecobb[t] );

		///// Diagonal cuts
		double iDiag=(double)t*10.0+50.0;
		if( massRecobb[t] < ( scalarPtRecobb[t]-iDiag ) ) {
			h_scalarPtRecobb_cut50->Fill( scalarPtRecobb[t], massRecobb[t] );
			h_massRecobb_cut50->Fill( massRecobb[t] );
			h_scalarPtRecobb_cut50->Fill( scalarPtRecobb[t], massRecobb[t] );
		}
		double iiDiag=(double)t*10.0+100.0;
		if( massRecobb[t] < ( scalarPtRecobb[t]-iiDiag ) ) {
			h_scalarPtRecobb_cut100->Fill( scalarPtRecobb[t], massRecobb[t] );
			h_massRecobb_cut100->Fill( massRecobb[t] );
			h_scalarPtRecobb_cut100->Fill( scalarPtRecobb[t], massRecobb[t] );
		}
		double iiiDiag=(double)t*10.0+150.0;
		if( massRecobb[t] < ( scalarPtRecobb[t]-iiiDiag ) ) {
			h_scalarPtRecobb_cut150->Fill( scalarPtRecobb[t], massRecobb[t] );
			h_massRecobb_cut150->Fill( massRecobb[t] );
			h_scalarPtRecobb_cut150->Fill( scalarPtRecobb[t], massRecobb[t] );
		}
		double ivDiag=(double)t*10.0+200.0;
		if( massRecobb[t] < ( scalarPtRecobb[t]-ivDiag ) ) {
			h_scalarPtRecobb_cut200->Fill( scalarPtRecobb[t], massRecobb[t] );
			h_massRecobb_cut200->Fill( massRecobb[t] );
			h_scalarPtRecobb_cut200->Fill( scalarPtRecobb[t], massRecobb[t] );
		}
	} 
	//////////////////////////////////////////////////////////


	////// MatchBJets
	vector< TLorentzVector > p4bbSmallMassDiff;
	vector< double > massbb;
	vector< double > ptbb;
	vector< double > scalarPtbb;

	double tmpMassDiff = -9999999;
	Int_t bestMassIndex[4] = {-1, -1, -1, -1};
	int ti=0;

	//if ( p4MatchBjets.size() > 3 ){
	if ( p4MatchBjets.size() == 4 ){
		////// Check index of the bs with the smallest mass diff
		for(unsigned int ii = 0; ii < p4MatchBjets.size()-3; ++ii) {
			for(unsigned int ij = 1; ij < p4MatchBjets.size()-2; ++ij) {
				for(unsigned int ik = 2; ik < p4MatchBjets.size()-1; ++ik) {
					for(unsigned int il = 3; il < p4MatchBjets.size(); ++il) {
						//if( ii==ij && ik==il && ii==ik && ii==il && ij==ik && ij==il  ) continue;
						if ( ii==ij ) continue;
						if ( ii==ik ) continue;
						if ( ii==il ) continue;
						if ( ij==ik ) continue;
						if ( ij==il ) continue;
						if ( ik==il ) continue;
						//cout << ii << " " << ij << " " << ik << " " << il << endl;
						TLorentzVector candMassbb1 = p4MatchBjets[ii]+p4MatchBjets[ij];
						double tmpmassbb1 = candMassbb1.M();
						//cout << tmpmassbb1 << endl;
						TLorentzVector candMassbb2 = p4MatchBjets[ik]+p4MatchBjets[il];
						double tmpmassbb2 = candMassbb2.M();
						//cout << tmpmassbb2 << endl;
						double massDiff = abs( tmpmassbb1 - tmpmassbb2 );
						//cout << tmpMassDiff << " " <<  massDiff << endl;
						if( tmpMassDiff < massDiff ){
							tmpMassDiff = massDiff;
							bestMassIndex[0] = ii;
							bestMassIndex[1] = ij;
							bestMassIndex[2] = ik;
							bestMassIndex[3] = il;
						}
					}
				}
			}
		}
		
		//cout << bestMassIndex[0] << " "  << bestMassIndex[1] << " " << bestMassIndex[2] << " " << bestMassIndex[3] << " "<< endl;
		p4bbSmallMassDiff.push_back( p4MatchBjets[bestMassIndex[0]] );
		p4bbSmallMassDiff.push_back( p4MatchBjets[bestMassIndex[1]] );
		p4bbSmallMassDiff.push_back( p4MatchBjets[bestMassIndex[2]] );
		p4bbSmallMassDiff.push_back( p4MatchBjets[bestMassIndex[3]] );
		TLorentzVector tmpMass1 = p4MatchBjets[bestMassIndex[0]] + p4MatchBjets[bestMassIndex[1]];
		TLorentzVector tmpMass2 = p4MatchBjets[bestMassIndex[2]] + p4MatchBjets[bestMassIndex[3]];
		double scalarPtbb1 = p4MatchBjets[bestMassIndex[0]].Pt() + p4MatchBjets[bestMassIndex[1]].Pt();
		double scalarPtbb2 = p4MatchBjets[bestMassIndex[2]].Pt() + p4MatchBjets[bestMassIndex[3]].Pt();

		////// Store vectors
		massbb.push_back ( tmpMass1.M() );
		massbb.push_back ( tmpMass2.M() );
		ptbb.push_back ( tmpMass1.Pt() );
		ptbb.push_back ( tmpMass2.Pt() );
		scalarPtbb.push_back ( scalarPtbb1 );
		scalarPtbb.push_back ( scalarPtbb2 );

		///// Simple plots
		h_massbb->Fill( massbb[ti] );
		h_Ptbb->Fill( ptbb[t], massbb[ti] );
		h_scalarPtbb->Fill( scalarPtbb[ti], massbb[ti] );

		///// Diagonal cuts
		double iDiag=(double)ti*10.0+50.0;
		if( massbb[ti] < ( scalarPtbb[ti]-iDiag ) ){
			h_scalarPtbb_cut50->Fill( scalarPtbb[ti], massbb[ti] );
			h_massbb_cut50->Fill( massbb[ti] );
			h_scalarPtbb_cut50->Fill( scalarPtbb[ti], massbb[ti] );
		}

		double iiDiag=(double)ti*10.0+100.0;
		if( massbb[ti] < ( scalarPtbb[ti]-iiDiag ) ) {
			h_scalarPtbb_cut100->Fill( scalarPtbb[ti], massbb[ti] );
			h_massbb_cut100->Fill( massbb[ti] );
			h_scalarPtbb_cut100->Fill( scalarPtbb[ti], massbb[ti] );
		}
		double iiiDiag=(double)ti*10.0+150.0;
		if( massbb[ti] < ( scalarPtbb[ti]-iiiDiag ) ) {
			h_scalarPtbb_cut150->Fill( scalarPtbb[ti], massbb[ti] );
			h_massbb_cut150->Fill( massbb[ti] );
			h_scalarPtbb_cut150->Fill( scalarPtbb[ti], massbb[ti] );
		}
		double ivDiag=(double)ti*10.0+200.0;
		if( massbb[ti] < ( scalarPtbb[ti]-ivDiag ) ) {
			h_scalarPtbb_cut200->Fill( scalarPtbb[ti], massbb[ti] );
			h_massbb_cut200->Fill( massbb[ti] );
			h_scalarPtbb_cut200->Fill( scalarPtbb[ti], massbb[ti] );
		}
	} 
	///////////////////////////////////////////////////////////////////

	// MatchBJets from Higgs
	vector< TLorentzVector > p4bbHiggsSmallMassDiff;
	vector< double > massbbHiggs;
	vector< double > ptbbHiggs;
	vector< double > scalarPtbbHiggs;

	int tii=0;
	if ( p4MatchbbsHiggs.size() == 4 ){
		
		TLorentzVector tmpMassbbHiggs1 = p4MatchbbsHiggs[0] + p4MatchbbsHiggs[1];
		TLorentzVector tmpMassbbHiggs2 = p4MatchbbsHiggs[2] + p4MatchbbsHiggs[3];
		double scalarPtbbHiggs1 = p4MatchbbsHiggs[0].Pt() + p4MatchbbsHiggs[1].Pt();
		double scalarPtbbHiggs2 = p4MatchbbsHiggs[2].Pt() + p4MatchbbsHiggs[3].Pt();

		////// Store vectors
		massbbHiggs.push_back ( tmpMassbbHiggs1.M() );
		massbbHiggs.push_back ( tmpMassbbHiggs2.M() );
		ptbbHiggs.push_back ( tmpMassbbHiggs1.Pt() );
		ptbbHiggs.push_back ( tmpMassbbHiggs2.Pt() );
		scalarPtbbHiggs.push_back ( scalarPtbbHiggs1 );
		scalarPtbbHiggs.push_back ( scalarPtbbHiggs2 );

		///// Simple plots
		h_massbbHiggs->Fill( massbbHiggs[tii] );
		h_PtbbHiggs->Fill( ptbbHiggs[tii], massbbHiggs[tii] );
		h_scalarPtbbHiggs->Fill( scalarPtbbHiggs[tii], massbbHiggs[tii] );

		///// Diagonal cuts
		double iDiag=(double)tii*10.0+50.0;
		if( massbbHiggs[tii] < ( scalarPtbbHiggs[tii]-iDiag ) ) {
			h_scalarPtbbHiggs_cut50->Fill( scalarPtbbHiggs[tii], massbbHiggs[tii] );
			h_massbbHiggs_cut50->Fill( massbbHiggs[tii] );
			h_scalarPtbbHiggs_cut50->Fill( scalarPtbbHiggs[tii], massbbHiggs[tii] );
		}
		double iiDiag=(double)tii*10.0+100.0;
		if( massbbHiggs[tii] < ( scalarPtbbHiggs[tii]-iiDiag ) ) {
			h_scalarPtbbHiggs_cut100->Fill( scalarPtbbHiggs[tii], massbbHiggs[tii] );
			h_massbbHiggs_cut100->Fill( massbbHiggs[tii] );
			h_scalarPtbbHiggs_cut100->Fill( scalarPtbbHiggs[tii], massbbHiggs[tii] );
		}
		double iiiDiag=(double)tii*10.0+150.0;
		if( massbbHiggs[tii] < ( scalarPtbbHiggs[tii]-iiiDiag ) ) {
			h_scalarPtbbHiggs_cut150->Fill( scalarPtbbHiggs[tii], massbbHiggs[tii] );
			h_massbbHiggs_cut150->Fill( massbbHiggs[tii] );
			h_scalarPtbbHiggs_cut150->Fill( scalarPtbbHiggs[tii], massbbHiggs[tii] );
		}
		double ivDiag=(double)tii*10.0+200.0;
		if( massbbHiggs[tii] < ( scalarPtbbHiggs[tii]-ivDiag ) ) {
			h_scalarPtbbHiggs_cut200->Fill( scalarPtbbHiggs[tii], massbbHiggs[tii] );
			h_massbbHiggs_cut200->Fill( massbbHiggs[tii] );
			h_scalarPtbbHiggs_cut200->Fill( scalarPtbbHiggs[tii], massbbHiggs[tii] );
		}
	} 
	///////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////
	/////////// STEP 2 - 3 ////////////////
	/////// Mass jj vs sum pt jj //////////
	///////////////////////////////////////

	///// Match All jets
	vector< TLorentzVector > p4jjWObb;
	vector< TLorentzVector > p4jWObb;
	vector< double > scalarsumjjWObbpt;
	if ( p4MatchAlljets.size() > 1 ){
		for(unsigned int iii = 0; iii < p4MatchAlljets.size(); ++iii) {

			////// Step 2 - dijet mass vs sum pt of jets
			for(unsigned int iij = 0; iij < p4MatchAlljets.size(); ++iij) {
				if ( iii==iij ) continue;
				TLorentzVector p4jj = p4MatchAlljets[iii] + p4MatchAlljets[iij];
				double sumjjpt = p4MatchAlljets[iii].Pt() + p4MatchAlljets[iij].Pt();
				h_jj_masspt->Fill( sumjjpt, p4jj.M() );
			}

			////// Step 2 - dijet mass vs sum pt of jets
			for(unsigned int iij = 0; iij < p4bbSmallMassDiff.size(); ++iij) {
				if ( p4MatchAlljets[iii].Pt() == p4bbSmallMassDiff[iij].Pt() ) continue;
				p4jWObb.push_back( p4MatchAlljets[iii] );
				TLorentzVector tmpp4jjWObb = p4MatchAlljets[iii] + p4MatchAlljets[iij];
				p4jjWObb.push_back( tmpp4jjWObb );
				double sumjjWObbpt = p4MatchAlljets[iii].Pt() + p4MatchAlljets[iij].Pt();
				scalarsumjjWObbpt.push_back( sumjjWObbpt );
				h_jjWObb_masspt->Fill( sumjjWObbpt, tmpp4jjWObb.M() );
			}
		}
	}

	///// RECO jets
	vector< TLorentzVector > p4recojjWObb;
	vector< TLorentzVector > p4recojWObb;
	vector< double > scalarsumrecojjWObbpt;
	if ( p4RecoJets.size() > 1 ){
		for(unsigned int iii = 0; iii < p4RecoJets.size(); ++iii) {

			////// Step 2 - dijet mass vs sum pt of jets
			for(unsigned int iij = 0; iij < p4RecoJets.size(); ++iij) {
				if ( iii==iij ) continue;
				TLorentzVector p4recojj = p4RecoJets[iii] + p4RecoJets[iij];
				double sumrecojjpt = p4RecoJets[iii].Pt() + p4RecoJets[iij].Pt();
				h_recojj_masspt->Fill( sumrecojjpt, p4recojj.M() );
			}

			////// Step 2 - dijet mass vs sum pt of jets
			for(unsigned int iij = 0; iij < p4RecobbSmallMassDiff.size(); ++iij) {
				if ( p4RecoJets[iii].Pt() == p4RecobbSmallMassDiff[iij].Pt() ) continue;
				p4recojWObb.push_back( p4RecoJets[iii] );
				TLorentzVector tmpp4recojjWObb = p4RecoJets[iii] + p4RecoJets[iij];
				p4recojjWObb.push_back( tmpp4recojjWObb );
				double sumrecojjWObbpt = p4RecoJets[iii].Pt() + p4RecoJets[iij].Pt();
				scalarsumrecojjWObbpt.push_back( sumrecojjWObbpt );
				h_recojjWObb_masspt->Fill( sumrecojjWObbpt, tmpp4recojjWObb.M() );
			}
		}
	}

	////////////////////////////////////////////////////////////////
	//////           STEP 4                                    /////
	//////  Mass of bb from 1 and jj from 3 vs sum pt of bbjj  /////
	////////////////////////////////////////////////////////////////
	
	/////// Reco Jets
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
	/////////////////////////////////////////////////////////////////////////////////////////////
	

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
