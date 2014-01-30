#define MyTreeAnalyzer_cxx
// The class definition in MyTreeAnalyzer.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("MyTreeAnalyzer.C")
// Root > T->Process("MyTreeAnalyzer.C","some options")
// Root > T->Process("MyTreeAnalyzer.C+")
//

#include "MyTreeAnalyzer.h"
#include <TStyle.h>
#include <TSystem.h>

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

void MyTreeAnalyzer::ParseInput(){

	if (fMyOpt.Contains("sample")){
		
		TString tmp = fMyOpt;
		tmp = tmp.Remove(0,fMyOpt.Index("sample")+7);
		//if (!fMyOpt.Contains("data") || !fMyOpt.Contains("HT")) tmp = tmp.Remove(29,8);
		//cout << ">>>>>>>>>>>>>>>>>" << tmp << endl;
		fSample = tmp;
		//if (fdoJECunc && fdoJECup==true) fSample += "_JECUP";
		//if (fdoJECunc && fdoJECup==false) fSample += "_JECDOWN";
		//if (fMyOpt.Contains("PUUP")) fSample += "_PUUP";
		//if (fMyOpt.Contains("PUDOWN")) fSample += "_PUDOWN";
	}

	if ( fSample.Contains("250")) { st2mass = 250; weight = 19500 * 5.57596/100000; }
	if ( fSample.Contains("350")) { st2mass = 350; weight = 19500 * 0.807323/100000; }
	if ( fSample.Contains("450")) { st2mass = 450; weight = 19500 * 0.169668/100000; }
	if ( fSample.Contains("550")) { st2mass = 550; weight = 19500 * 0.0452067/100000; }
	if ( fSample.Contains("650")) { st2mass = 650; weight = 19500 * 0.0139566/100000; }
	if ( fSample.Contains("750")) { st2mass = 750; weight = 19500 * 0.00480639/100000; }
	if ( fSample.Contains("data")) { weight = 1; }
	//if ( fSample.Contains("HT250")) { weight = 19500 * 276000/17021377; }
	if ( fSample.Contains("HT500")) { weight = 19500 * 8426/28122500; }
	if ( fSample.Contains("HT1000")) { weight = 19500 * 204/13795394; }

	if ( fSample.Contains("100")) st1mass = 100; 
	if ( fSample.Contains("200")) st1mass = 200; 
	if ( fSample.Contains("300")) st1mass = 300; 
	if ( fSample.Contains("400")) st1mass = 400; 
	if ( fSample.Contains("500")) st1mass = 500; 

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
////// Function analyze small mass difference
smallMassDiffVectors smallMassDiff( std::vector< TLorentzVector > p4StoreJets ){

	double tmpMassDiff = 9999999;
	double avgMassBjets = -99999;
	double avgPtBjets = -99999;
	Int_t bestMassIndex[4] = {-1, -1, -1, -1};
	std::vector< TLorentzVector > p4MatchBjetsSmallMassDiff;
	std::vector< double > massMatchBjets;
	std::vector< double > ptMatchBjets;
	std::vector< double > scalarPtMatchBjets;

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

		////// Store std::vectors
		massMatchBjets.push_back ( tmpMass1.M() );
		massMatchBjets.push_back ( tmpMass2.M() );
		ptMatchBjets.push_back ( tmpMass1.Pt() );
		ptMatchBjets.push_back ( tmpMass2.Pt() );
		scalarPtMatchBjets.push_back ( scalarPtMatchBjets1 );
		scalarPtMatchBjets.push_back ( scalarPtMatchBjets2 );
		avgMassBjets = (massMatchBjets[0] + massMatchBjets[1]) / massMatchBjets.size();
		avgPtBjets = (scalarPtMatchBjets[0] + scalarPtMatchBjets[1]) / scalarPtMatchBjets.size();
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

//////////////////////////////////////////////////
/////////// Function Compare two TLorentzVectors
std::vector< TLorentzVector > compare2TLVReturnNoEqual( std::vector< TLorentzVector > vec1, std::vector< TLorentzVector > vec2 ){   /// vec2 has 4 TLV

	std::vector< TLorentzVector > diffVec;

	for(unsigned int i = 0; i < vec1.size(); ++i) {
		//cout << vec2[0].Pt() << " " << vec2[1].Pt() << " " << vec2[2].Pt() << " " << vec2[3].Pt() << endl; 
		if ( ( vec1[i].Pt() == vec2[0].Pt() ) || ( vec1[i].Pt() == vec2[1].Pt() ) || ( vec1[i].Pt() == vec2[2].Pt() ) || ( vec1[i].Pt() == vec2[3].Pt() ) ) continue;
		diffVec.push_back( vec1[i] );
	}
	return diffVec;
}

std::vector< TLorentzVector > compare2TLVReturnEqual( std::vector< TLorentzVector > vec1, std::vector< TLorentzVector > vec2 ){	//// vec2 has 2 TLV

	vector< TLorentzVector > diffVec;
	if ( vec2.size() > 0 ) {
		for(unsigned int i = 0; i < vec1.size(); ++i) {
			//cout << vec1[i].Pt() << endl; 
			//cout << vec2[0].Pt() << " " << vec2[1].Pt() << endl; 
			if ( ( vec1[i].Pt() != vec2[0].Pt() ) && ( vec1[i].Pt() != vec2[1].Pt() ) ) continue;
			//cout << vec2[0].Pt() << " " << vec2[1].Pt() << endl; 
			diffVec.push_back( vec1[i] );
		}
	}
	return diffVec;
}
////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
/////// Function Step2 (dijet mass from one vector)
s2VecMassSumPt s2Vectors( std::vector< TLorentzVector > p4Store ){

	std::vector< TLorentzVector > p4Vec;
	std::vector< double > massVec;
	std::vector< double > scalarSumPtVec;

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
	s2VecMassSumPt tmpVectors;
	tmpVectors.s2VectorTL = p4Vec;
	tmpVectors.s2Mass = massVec;
	tmpVectors.s2ScalarPt = scalarSumPtVec;


	return tmpVectors;
}
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
/////// Function Step3 (dijet mass from one vector)
s3Vectors step3Vectors( std::vector< TLorentzVector > p4dijet, std::vector< TLorentzVector > p4diBjet, std::vector< double > scalarSumPtdijet ){

	vector< TLorentzVector > p4RecoDiBjetDiJet;
	vector< double > massRecoDiBjetDiJet;
	vector< double > scalarSumPtRecoDiBjetDiJet;

	for(unsigned int iiii = 0; iiii < p4dijet.size(); ++iiii) {

		TLorentzVector cand1Stop2 = p4dijet[iiii] + p4diBjet[0] + p4diBjet[1];
		TLorentzVector cand2Stop2 = p4dijet[iiii] + p4diBjet[2] + p4diBjet[3];
		double scalarPt1Stop2 = scalarSumPtdijet[iiii] + p4diBjet[0].Pt() + p4diBjet[1].Pt();
		double scalarPt2Stop2 = scalarSumPtdijet[iiii] + p4diBjet[0].Pt() + p4diBjet[1].Pt();
		p4RecoDiBjetDiJet.push_back( cand1Stop2 );
		p4RecoDiBjetDiJet.push_back( cand2Stop2 );
		massRecoDiBjetDiJet.push_back( cand1Stop2.M() );
		massRecoDiBjetDiJet.push_back( cand2Stop2.M() );
		scalarSumPtRecoDiBjetDiJet.push_back( scalarPt1Stop2 );
		scalarSumPtRecoDiBjetDiJet.push_back( scalarPt2Stop2 );
	}
	s3Vectors tmpVectors;
	tmpVectors.s3TLV = p4RecoDiBjetDiJet;
	tmpVectors.s3Mass = massRecoDiBjetDiJet;
	tmpVectors.s3ScalarPt = scalarSumPtRecoDiBjetDiJet;

	return tmpVectors;
}
///////////////////////////////////////////////////////////////////////////////////




void MyTreeAnalyzer::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

	TString option = GetOption();
	fMyOpt = option;
	ParseInput();

	//Info("Begin", "starting with process option: %s", option.Data());

}

void MyTreeAnalyzer::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

	//TString option = GetOption();
	//fMyOpt = option;
	//ParseInput();
		
	///////////////// TEST PROOF
	/*/ We may be creating a dataset or a merge file: check it
	TNamed *nm = dynamic_cast<TNamed *>(fInput->FindObject("SimpleNtuple.root"));
	if (nm) {
		// Just create the object
		UInt_t opt = TProofOutputFile::kRegister | TProofOutputFile::kOverwrite | TProofOutputFile::kVerify;
		fProofFile = new TProofOutputFile("SimpleNtuple.root", TProofOutputFile::kDataset, opt, nm->GetTitle());
	} else {
		// For the ntuple, we use the automatic file merging facility
		// Check if an output URL has been given
		TNamed *out = (TNamed *) fInput->FindObject("PROOF_OUTPUTFILE_LOCATION");
		TString dir = "file:/cms/gomez/Stops/Results/";
		TString tmpfilename = "results.root";
		//if ( fSample != "" ) tmpfilename += "_"+fSample+".root";
		//else tmpfilename = "results.root";
		fProofFile = new TProofOutputFile(tmpfilename,TProofOutputFile::kMerge);

		fProofFile->SetOutputFileName(dir + tmpfilename);
		out = (TNamed *) fInput->FindObject("PROOF_OUTPUTFILE");
		if (out) fProofFile->SetOutputFileName(out->GetTitle());
	}

	// Open the file
	fFile = fProofFile->OpenFile("RECREATE");
	if (fFile && fFile->IsZombie()) SafeDelete(fFile);
	// Cannot continue
	if (!fFile) {
		Info("SlaveBegin", "could not create '%s': instance is invalid!", fProofFile->GetName());
		return;
	}*//////////////////////////////////////////////////////////////////////

	TString dir = "file:/cms/gomez/Stops/Results/";				// Hexfarm
	//TString dir = "file:/uscms_data/d3/algomez/files/Stops/Results/";	// LPC
	TString tmpfilename;
	//if ( fSample != "" ) tmpfilename = fSample+"_plots.root";
	if ( fSample != "" ) tmpfilename = fSample+"_4jet80_6jet60_plots.root";
	//else if ( fJES == "jesup" ) tmpfilename = fSample+"jesup_4jet80_6jet60_plots.root";
	//else if ( fJES == "jesdown" ) tmpfilename = fSample+"jesdown_4jet80_6jet60_plots.root";
	//if ( fSample != "" ) tmpfilename = fSample+"_4jet80_plots.root";
	//if ( fSample != "" ) tmpfilename = fSample+"_4jet120_plots.root";
	else tmpfilename = "results.root";
	cout << "|----- Saving into the file: " << tmpfilename << endl;
	fFile = new TFile(dir+tmpfilename,"RECREATE");


	//////////////////////////////////////////
	/////////create histograms    ////////////
	//////////////////////////////////////////
	
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

	int bigNbinPt = 200;
	double bigMinPt = 0;
	double bigMaxPt = 2000;

	/////////////////////////////// Basic Plots
	h1test = new TH1F( "h1test", "jet p_{T}", nbinPt, minPt, maxPt );
	basicPlots["recoJets_num"] = new TH1D("recoJets_num" , "Number of Reco Jets", nbinNum , minNum, maxNum );
	basicPlots["recoJets_pt"] = new TH1D("recoJets_pt" , "Reco Jets p_{T}", nbinPt , minPt, maxPt );
	basicPlots["recoJets_1pt"] = new TH1D("recoJets_1pt" , "Leading Jet p_{T}", nbinPt , minPt, maxPt );
	basicPlots["recoJets_2pt"] = new TH1D("recoJets_2pt" , "Second Leading Jet p_{T}", nbinPt , minPt, maxPt );
	basicPlots["recoJets_4pt"] = new TH1D("recoJets_4pt" , "4th Leading Jet p_{T}", nbinPt , minPt, maxPt );
	basicPlots["recoJets_6pt"] = new TH1D("recoJets_6pt" , "6th Leading Jet p_{T}", nbinPt , minPt, maxPt );
	basicPlots["recoJets_HT"] = new TH1D("recoJets_HT" , "H_{T}", bigNbinPt , bigMinPt, bigMaxPt );
	basicPlots["recoJets_eta"] = new TH1D("recoJets_eta" , "Reco Jets #eta", nbinEta , minEta, maxEta );
	basicPlots["recoJets_phi"] = new TH1D("recoJets_phi" , "Reco Jets #phi", nbinEta , minEta, maxEta );
	basicPlots["recoBjets_num"] = new TH1D("recoBjets_num" , "Number of Reco Bjets", nbinNum , minNum, maxNum );
	basicPlots["recoBjets_pt"] = new TH1D("recoBjets_pt" , "Reco Bjets p_{T}", nbinPt , minPt, maxPt );
	basicPlots["recoBjets_sumpt"] = new TH1D("recoBjets_sumpt" , "#sum Reco Bjets p_{T}", nbinPt , minPt, maxPt );
	basicPlots["recoBjets_eta"] = new TH1D("recoBjets_eta" , "Reco Bjets #eta", nbinEta , minEta, maxEta );
	basicPlots["recoBjets_phi"] = new TH1D("recoBjets_phi" , "Reco Bjets #phi", nbinEta , minEta, maxEta );

	basicPlots["recoJets_num_Step1"] = new TH1D("recoJets_num_Step1" , "Number of Reco Jets after Step1", nbinNum , minNum, maxNum );
	basicPlots["recoBjets_num_Step1"] = new TH1D("recoBjets_num_Step1" , "Number of Reco Bjets after Step1", nbinNum , minNum, maxNum );
	basicPlots["recoJets_num_Step2"] = new TH1D("recoJets_num_Step2" , "Number of Reco Jets after Step2", nbinNum , minNum, maxNum );
	basicPlots["recoBjets_num_Step2"] = new TH1D("recoBjets_num_Step2" , "Number of Reco Bjets after Step2", nbinNum , minNum, maxNum );
	basicPlots["recoJets_num_Step2_diag50"] = new TH1D("recoJets_num_Step2_diag50" , "Number of Reco Jets after Step2_diag50", nbinNum , minNum, maxNum );
	basicPlots["recoBjets_num_Step2_diag50"] = new TH1D("recoBjets_num_Step2_diag50" , "Number of Reco Bjets after Step2_diag50", nbinNum , minNum, maxNum );
	basicPlots["recoJets_num_Step2_diag100"] = new TH1D("recoJets_num_Step2_diag100" , "Number of Reco Jets after Step2_diag100", nbinNum , minNum, maxNum );
	basicPlots["recoBjets_num_Step2_diag100"] = new TH1D("recoBjets_num_Step2_diag100" , "Number of Reco Bjets after Step2_diag100", nbinNum , minNum, maxNum );
	basicPlots["recoJets_num_Step2_diag150"] = new TH1D("recoJets_num_Step2_diag150" , "Number of Reco Jets after Step2_diag150", nbinNum , minNum, maxNum );
	basicPlots["recoBjets_num_Step2_diag150"] = new TH1D("recoBjets_num_Step2_diag150" , "Number of Reco Bjets after Step2_diag150", nbinNum , minNum, maxNum );
	basicPlots["recoJets_num_Step2_diag200"] = new TH1D("recoJets_num_Step2_diag200" , "Number of Reco Jets after Step2_diag200", nbinNum , minNum, maxNum );
	basicPlots["recoBjets_num_Step2_diag200"] = new TH1D("recoBjets_num_Step2_diag200" , "Number of Reco Bjets after Step2_diag200", nbinNum , minNum, maxNum );

	//////////////////////////////// Step1 Plots 1D
	// Reco Jets (TEST)
	step1plots1D["massRecoJetsHiggsCandWOBjets"] = new TH1D("massRecoJetsHiggsCandWOBjets" , "Mass of RecoJetsHiggsCandWOBjets" , nbinPt, minPt, maxPt );
	step1plots1D["avgMassRecoJetsHiggsCandWOBjets"] = new TH1D("avgMassRecoJetsHiggsCandWOBjets" , "Average Mass of RecoJetsHiggsCandWOBjets" , nbinPt, minPt, maxPt );
	step1plots2D["scalarPtRecoJetsHiggsCandWOBjets"] = new TH2D("scalarPtRecoJetsHiggsCandWOBjets" , "Mass of RecoJetsHiggsCandWOBjets vs scalar #sum p_{T}^{RecoJetsHiggsCandWOBjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["avgPtRecoJetsHiggsCandWOBjets"] = new TH2D("avgPtRecoJetsHiggsCandWOBjets" , "Average Mass of RecoJetsHiggsCandWOBjets vs scalar #sum p_{T}^{RecoJetsHiggsCandWOBjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots1D["massRecoJetsHiggsCandW1Bjets"] = new TH1D("massRecoJetsHiggsCandW1Bjets" , "Mass of RecoJetsHiggsCandW1Bjets" , nbinPt, minPt, maxPt );
	step1plots1D["avgMassRecoJetsHiggsCandW1Bjets"] = new TH1D("avgMassRecoJetsHiggsCandW1Bjets" , "Average Mass of RecoJetsHiggsCandW1Bjets" , nbinPt, minPt, maxPt );
	step1plots2D["scalarPtRecoJetsHiggsCandW1Bjets"] = new TH2D("scalarPtRecoJetsHiggsCandW1Bjets" , "Mass of RecoJetsHiggsCandW1Bjets vs scalar #sum p_{T}^{RecoJetsHiggsCandW1Bjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["avgPtRecoJetsHiggsCandW1Bjets"] = new TH2D("avgPtRecoJetsHiggsCandW1Bjets" , "Average Mass of RecoJetsHiggsCandW1Bjets vs scalar #sum p_{T}^{RecoJetsHiggsCandW1Bjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots1D["massRecoJetsHiggsCandW2Bjets"] = new TH1D("massRecoJetsHiggsCandW2Bjets" , "Mass of RecoJetsHiggsCandW2Bjets" , nbinPt, minPt, maxPt );
	step1plots1D["avgMassRecoJetsHiggsCandW2Bjets"] = new TH1D("avgMassRecoJetsHiggsCandW2Bjets" , "Average Mass of RecoJetsHiggsCandW2Bjets" , nbinPt, minPt, maxPt );
	step1plots2D["scalarPtRecoJetsHiggsCandW2Bjets"] = new TH2D("scalarPtRecoJetsHiggsCandW2Bjets" , "Mass of RecoJetsHiggsCandW2Bjets vs scalar #sum p_{T}^{RecoJetsHiggsCandW2Bjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["avgPtRecoJetsHiggsCandW2Bjets"] = new TH2D("avgPtRecoJetsHiggsCandW2Bjets" , "Average Mass of RecoJetsHiggsCandW2Bjets vs scalar #sum p_{T}^{RecoJetsHiggsCandW2Bjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots1D["massRecoJetsHiggsCandW3Bjets"] = new TH1D("massRecoJetsHiggsCandW3Bjets" , "Mass of RecoJetsHiggsCandW3Bjets" , nbinPt, minPt, maxPt );
	step1plots1D["avgMassRecoJetsHiggsCandW3Bjets"] = new TH1D("avgMassRecoJetsHiggsCandW3Bjets" , "Average Mass of RecoJetsHiggsCandW3Bjets" , nbinPt, minPt, maxPt );
	step1plots2D["scalarPtRecoJetsHiggsCandW3Bjets"] = new TH2D("scalarPtRecoJetsHiggsCandW3Bjets" , "Mass of RecoJetsHiggsCandW3Bjets vs scalar #sum p_{T}^{RecoJetsHiggsCandW3Bjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["avgPtRecoJetsHiggsCandW3Bjets"] = new TH2D("avgPtRecoJetsHiggsCandW3Bjets" , "Average Mass of RecoJetsHiggsCandW3Bjets vs scalar #sum p_{T}^{RecoJetsHiggsCandW3Bjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots1D["massRecoJetsHiggsCandW4Bjets"] = new TH1D("massRecoJetsHiggsCandW4Bjets" , "Mass of RecoJetsHiggsCandW4Bjets" , nbinPt, minPt, maxPt );
	step1plots1D["avgMassRecoJetsHiggsCandW4Bjets"] = new TH1D("avgMassRecoJetsHiggsCandW4Bjets" , "Average Mass of RecoJetsHiggsCandW4Bjets" , nbinPt, minPt, maxPt );
	step1plots2D["scalarPtRecoJetsHiggsCandW4Bjets"] = new TH2D("scalarPtRecoJetsHiggsCandW4Bjets" , "Mass of RecoJetsHiggsCandW4Bjets vs scalar #sum p_{T}^{RecoJetsHiggsCandW4Bjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["avgPtRecoJetsHiggsCandW4Bjets"] = new TH2D("avgPtRecoJetsHiggsCandW4Bjets" , "Average Mass of RecoJetsHiggsCandW4Bjets vs scalar #sum p_{T}^{RecoJetsHiggsCandW4Bjets}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	

	// Reco Bjets
	step1plots1D["massRecoBjetsCSVM"] = new TH1D("massRecoBjetsCSVM" , "Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["massRecoBjetsCSVM_cutDiagHiggsbb50"] = new TH1D("massRecoBjetsCSVM_cutDiagHiggsbb50" , "Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["massRecoBjetsCSVM_cutDiagHiggsbb100"] = new TH1D("massRecoBjetsCSVM_cutDiagHiggsbb100" , "Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["massRecoBjetsCSVM_cutDiagHiggsbb150"] = new TH1D("massRecoBjetsCSVM_cutDiagHiggsbb150" , "Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["massRecoBjetsCSVM_cutDiagHiggsbb200"] = new TH1D("massRecoBjetsCSVM_cutDiagHiggsbb200" , "Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["avgMassRecoBjetsCSVM"] = new TH1D("avgMassRecoBjetsCSVM" , "Average Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["avgMassRecoBjetsCSVM_cutDiagHiggsbb50"] = new TH1D("avgMassRecoBjetsCSVM_cutDiagHiggsbb50" , "Average Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["avgMassRecoBjetsCSVM_cutDiagHiggsbb100"] = new TH1D("avgMassRecoBjetsCSVM_cutDiagHiggsbb100" , "Average Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["avgMassRecoBjetsCSVM_cutDiagHiggsbb150"] = new TH1D("avgMassRecoBjetsCSVM_cutDiagHiggsbb150" , "Average Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["avgMassRecoBjetsCSVM_cutDiagHiggsbb200"] = new TH1D("avgMassRecoBjetsCSVM_cutDiagHiggsbb200" , "Average Mass of RecoBjetsCSVM" , nbinPt, minPt, maxPt );


	// Reco Bjets + Parton Flavor
	step1plots1D["massRecoPartonFlavorBjetsCSVM"] = new TH1D("massRecoPartonFlavorBjetsCSVM" , "Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50"] = new TH1D("massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50" , "Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100"] = new TH1D("massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100" , "Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150"] = new TH1D("massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150" , "Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200"] = new TH1D("massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200" , "Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["avgMassRecoPartonFlavorBjetsCSVM"] = new TH1D("avgMassRecoPartonFlavorBjetsCSVM" , "Average Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50"] = new TH1D("avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50" , "Average Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100"] = new TH1D("avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100" , "Average Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150"] = new TH1D("avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150" , "Average Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );
	step1plots1D["avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200"] = new TH1D("avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200" , "Average Mass of RecoPartonFlavorBjetsCSVM" , nbinPt, minPt, maxPt );

	// Step1 Plots 2D
	// Reco Bjets
	step1plots2D["scalarPtRecoBjetsCSVM"] = new TH2D("scalarPtRecoBjetsCSVM" , "Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["scalarPtRecoBjetsCSVM_cutDiagHiggsbb50"] = new TH2D("scalarPtRecoBjetsCSVM_cutDiagHiggsbb50" , "Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["scalarPtRecoBjetsCSVM_cutDiagHiggsbb100"] = new TH2D("scalarPtRecoBjetsCSVM_cutDiagHiggsbb100" , "Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["scalarPtRecoBjetsCSVM_cutDiagHiggsbb150"] = new TH2D("scalarPtRecoBjetsCSVM_cutDiagHiggsbb150" , "Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["scalarPtRecoBjetsCSVM_cutDiagHiggsbb200"] = new TH2D("scalarPtRecoBjetsCSVM_cutDiagHiggsbb200" , "Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["avgPtRecoBjetsCSVM"] = new TH2D("avgPtRecoBjetsCSVM" , "Average Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["avgPtRecoBjetsCSVM_cutDiagHiggsbb50"] = new TH2D("avgPtRecoBjetsCSVM_cutDiagHiggsbb50" , "Average Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["avgPtRecoBjetsCSVM_cutDiagHiggsbb100"] = new TH2D("avgPtRecoBjetsCSVM_cutDiagHiggsbb100" , "Average Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["avgPtRecoBjetsCSVM_cutDiagHiggsbb150"] = new TH2D("avgPtRecoBjetsCSVM_cutDiagHiggsbb150" , "Average Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["avgPtRecoBjetsCSVM_cutDiagHiggsbb200"] = new TH2D("avgPtRecoBjetsCSVM_cutDiagHiggsbb200" , "Average Mass of RecoBjetsCSVM vs scalar #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["PtRecoBjetsCSVM"] = new TH2D("PtRecoBjetsCSVM" , "Mass of RecoBjetsCSVM vs #sum p_{T}^{RecoBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	// Reco Bjets + Parton Flavor
	step1plots2D["scalarPtRecoPartonFlavorBjetsCSVM"] = new TH2D("scalarPtRecoPartonFlavorBjetsCSVM" , "Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50"] = new TH2D("scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50" , "Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100"] = new TH2D("scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100" , "Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150"] = new TH2D("scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150" , "Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200"] = new TH2D("scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200" , "Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["avgPtRecoPartonFlavorBjetsCSVM"] = new TH2D("avgPtRecoPartonFlavorBjetsCSVM" , "Average Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50"] = new TH2D("avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50" , "Average Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100"] = new TH2D("avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100" , "Average Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150"] = new TH2D("avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150" , "Average Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200"] = new TH2D("avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200" , "Average Mass of RecoPartonFlavorBjetsCSVM vs scalar #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step1plots2D["PtRecoPartonFlavorBjetsCSVM"] = new TH2D("PtRecoPartonFlavorBjetsCSVM" , "Mass of RecoPartonFlavorBjetsCSVM vs #sum p_{T}^{RecoPartonFlavorBjetsCSVM}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt ); 


	////// Step 2 plots 2D
	step2plots2D["recoDijet_masspt"] = new TH2D("recoDijet_masspt" , "Mass of dijet vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["recoPartonFlavorDijet_masspt"] = new TH2D("recoPartonFlavorDijet_masspt" , "Mass of dijet vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	// Reco Bjets
	step2plots1D["massdijetWORecoBjetsCSVM"] = new TH1D("massdijetWORecoBjetsCSVM" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_WOStop1"] = new TH1D("massdijetWORecoBjetsCSVM_WOStop1" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj10"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj10" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj20"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj20" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj30"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj30" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj40"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj40" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj60"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj60" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj70"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj70" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj80"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj80" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj90"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj90" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj50"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj50_WOStop1"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj50_WOStop1" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj100"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj100_WOStop1"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj100_WOStop1" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj150"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj150_WOStop1"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj150_WOStop1" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj200"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	// Reco Bjets + Parton Flavor
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj50"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj100"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj150"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj200"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	////// Step 2 plots 2D
	//  Reco Bjets
	step2plots2D["dijetWORecoBjetsCSVM_masspt"] = new TH2D("dijetWORecoBjetsCSVM_masspt" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj10"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj10" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj20"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj20" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj30"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj30" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj40"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj40" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj50"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj60"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj60" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj70"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj70" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj80"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj80" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj90"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj90" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj100"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj150"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj200"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	// Reco jets + Parton Flavor
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj50"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj100"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj150"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj200"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );


	///// STEP 3
	// Reco Jets
	step3plots2D["recoDiBjetDiJet_masspt"] = new TH2D("recoDiBjetDiJet_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_MassPt"] = new TH2D("recoDiBjetDiJet_MassPt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet"] = new TH1D("massRecoDiBjetDiJet" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj0_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop2bbjj0_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj10_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop2bbjj10_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj20_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop2bbjj20_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj30_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop2bbjj30_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj40_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop2bbjj40_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj50_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop2bbjj50_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj60_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop2bbjj60_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj70_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop2bbjj70_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj80_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop2bbjj80_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj90_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop2bbjj90_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj0"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop2bbjj0" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj10"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop2bbjj10" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj20"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop2bbjj20" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj30"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop2bbjj30" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj40"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop2bbjj40" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj60"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop2bbjj60" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj70"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop2bbjj70" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj80"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop2bbjj80" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj90"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop2bbjj90" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj50"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop2bbjj50" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj50_MassPt"] = new TH2D("recoDiBjetDiJet_cutDiagStop2bbjj50_MassPt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massPtRecoDiBjetDiJet_cutDiagStop2bbjj50"] = new TH1D("massPtRecoDiBjetDiJet_cutDiagStop2bbjj50" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj100_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop2bbjj100_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj100"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop2bbjj100" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop1jj10_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop1jj10_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop1jj20_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop1jj20_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop1jj30_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop1jj30_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop1jj40_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop1jj40_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop1jj50_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop1jj50_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop1jj60_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop1jj60_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop1jj70_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop1jj70_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop1jj80_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop1jj80_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop1jj90_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop1jj90_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj10"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop1jj10" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj20"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop1jj20" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj30"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop1jj30" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj40"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop1jj40" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj50"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop1jj50" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj60"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop1jj60" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj70"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop1jj70" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj80"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop1jj80" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj90"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop1jj90" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop1jj100_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop1jj100_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj100"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop1jj100" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );
	step3plots2D["recoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100_masspt"] = new TH2D("recoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets}" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100"] = new TH1D("massRecoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100" , "Mass of RECO (diBjet + diJet)" , bigNbinPt, bigMinPt, bigMaxPt );


	//////////// RESOLUTION BINNING PLOTS
	Double_t dijetMassBins[39] = {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 74, 88, 103, 119, 137, 156, 176, 197, 220, 244, 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000};
	step2plots1D["massdijetWORecoBjetsCSVM_resoBasedBin"] = step2plots1D["massdijetWORecoBjetsCSVM"]->Rebin(38, "massdijetWORecoBjetsCSVM_resoBasedBin", dijetMassBins);

	Double_t quadjetMassBins[32] = {30, 60, 90, 120, 150, 180, 210, 250, 290, 330, 370, 410, 460, 510, 560, 610, 670, 730, 790, 860, 930, 1000, 1080, 1160, 1240, 1330, 1420, 1520, 1620, 1730, 1840, 2000};
	step3plots1D["massRecoDiBjetDiJet_resoBasedBin"] = step3plots1D["massRecoDiBjetDiJet"]->Rebin(31, "massRecoDiBjetDiJet_resoBasedBin", quadjetMassBins);
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj0_resoBasedBin"] = step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj0"]->Rebin(31, "massRecoDiBjetDiJet_cutDiagStop2bbjj0_resoBasedBin", quadjetMassBins);
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj10_resoBasedBin"] = step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj10"]->Rebin(31, "massRecoDiBjetDiJet_cutDiagStop2bbjj10_resoBasedBin", quadjetMassBins);
	step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj20_resoBasedBin"] = step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj20"]->Rebin(31, "massRecoDiBjetDiJet_cutDiagStop2bbjj20_resoBasedBin", quadjetMassBins);

	////////////////// Plots after all cuts
	step3plots1D["avgMassHiggsCandidate_cutDiagStop2bbjj0"] = new TH1D("avgMassHiggsCandidate_cutDiagStop2bbjj0" , "Average Mass of Higgs Candidates" , nbinPt, minPt, maxPt );
	step3plots1D["avgMassHiggsCandidate_cutDiagStop2bbjj0_resoBasedBin"] = step1plots1D["avgMassRecoBjetsCSVM"]->Rebin(38, "avgMassHiggsCandidate_cutDiagStop2bbjj0_resoBasedBin", dijetMassBins);
	step3plots1D["massHiggsCandidate_cutDiagStop2bbjj0"] = new TH1D("massHiggsCandidate_cutDiagStop2bbjj0" , "Mass of Higgs Candidates" , nbinPt, minPt, maxPt );
	step3plots1D["massStop1Candidate_cutDiagStop2bbjj0"] = new TH1D("massStop1Candidate_cutDiagStop2bbjj0" , "Mass of Stop1 Candidates" , nbinPt, minPt, maxPt );
	step3plots1D["massStop1Candidate_cutDiagStop2bbjj0_resoBasedBin"] = step2plots1D["massdijetWORecoBjetsCSVM"]->Rebin(38, "massStop1Candidate_cutDiagStop2bbjj0_resoBasedBin", dijetMassBins);



	///// STEP 4
	step4plots2D["recoDiBjetDiJetSmallestDeltaM_masspt"] = new TH2D("recoDiBjetDiJetSmallestDeltaM_masspt" , "Mass of RECO (diBjets + diJets) vs scalar #sum p_{T}^{diBjets + diJets} SmallestDeltaM" , bigNbinPt, bigMinPt, bigMaxPt, bigNbinPt, bigMinPt, bigMaxPt );
	step4plots1D["massRecoDiBjetDiJetSmallestDeltaM"] = new TH1D("massRecoDiBjetDiJetSmallestDeltaM" , "Mass of RECO (diBjet + diJet) SmallestDeltaM" , bigNbinPt, bigMinPt, bigMaxPt );

	map<string,TH1* > allhistos1D = basicPlots;
	allhistos1D.insert( step1plots1D.begin(), step1plots1D.end() );
	allhistos1D.insert( step2plots1D.begin(), step2plots1D.end() );
	allhistos1D.insert( step3plots1D.begin(), step3plots1D.end() );
	allhistos1D.insert( step4plots1D.begin(), step4plots1D.end() );
	map<string,TH2* > allhistos2D = step1plots2D;
	allhistos2D.insert( step2plots2D.begin(), step2plots2D.end() );
	allhistos2D.insert( step3plots2D.begin(), step3plots2D.end() );
	allhistos2D.insert( step4plots2D.begin(), step4plots2D.end() );

	for ( std::map<string,TH1* >::const_iterator imap=allhistos1D.begin(); imap!=allhistos1D.end(); ++imap ) {
		TH1 *temp = imap->second;
		temp->Sumw2();
		//temp->SetXTitle( temp->GetTitle() );
	}

	for ( std::map<string,TH2* >::const_iterator imap=allhistos2D.begin(); imap!=allhistos2D.end(); ++imap ) {
		TH2 *temp = imap->second;
		temp->Sumw2();
		//temp->SetXTitle( temp->GetTitle() );
	}


	fCutLabels.push_back("Processed");
	fCutLabels.push_back("Simple");
	fCutLabels.push_back("4jetTrigger");
	fCutLabels.push_back("Step1");
	fCutLabels.push_back("Step2");
	fCutLabels.push_back("Step2_diag50");
	fCutLabels.push_back("Step2_diag100");
	fCutLabels.push_back("Step2_diag150");
	fCutLabels.push_back("Step2_diag200");
	fCutLabels.push_back("Step3");
	fCutLabels.push_back("Step3_diagStop2_0");

	hcutflow = new TH1D("cutflow","cut flow", fCutLabels.size(), 0.5, fCutLabels.size() +0.5 );

	for ( vector<string>::const_iterator ivec= fCutLabels.begin(); ivec!=fCutLabels.end(); ++ivec) {
		cutmap[ *ivec ] = 0;
	}

}

Bool_t MyTreeAnalyzer::Process(Long64_t Entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either MyTreeAnalyzer::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

	//cout << st2mass << " " << weight << endl;
	//cout << " 0 " << endl;

	//..Just read the full event
	fChain->GetTree()->GetEntry(Entry);

	if ( Entry % 2000 == 0 ) cout<< "process Entry " << Entry << endl;

	Double_t nEvents = 1.;
	cutmap["Processed"] += nEvents;

	/// TLorentzVectors
	std::vector< TLorentzVector > p4RecoJets;
	std::vector< TLorentzVector > p4RecoBjetsCSVM;
	std::vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM;
	std::vector< double > dummyCounter1;

	///////////////////////////////////////////////////
	/////////// Store TLorentzVecots RECO jets and bjets
	//////////////////////////////////////////////////

	for (int i=0; i<nPFJets; i++){

		h1test->Fill( jet_PF_pt[i] );
		if ( jet_PF_pt[i] < 35.0 || fabs( jet_PF_eta[i] ) > 2.5) continue;
		dummyCounter1.push_back( jet_PF_pt[i] );

		//if ( nPFJets > 5 ){
			//if ( jet_PF_pt[3] < 60.0 ) continue;				/// 6jet Trigger
			if ( jet_PF_pt[3] < 80.0 || jet_PF_pt[5] < 60.0 ) continue;				/// 6jet Trigger
			//if ( jet_PF_pt[3] < 80 ) continue;				/// dijet trigger
			//if ( jet_PF_pt[3] < 120 ) continue;				/// dijet trigger

			TLorentzVector p4Jets;
			p4Jets.SetPxPyPzE( jet_PF_px[i], jet_PF_py[i], jet_PF_pz[i], jet_PF_e[i] );
			p4RecoJets.push_back( p4Jets );

			//bool isTagged = false;
			//CSVL > 0.244, CSVM > 0.679, CSVT > 0.898, JPM > 
			//if (!fSample.Contains("HT")){
				if (bdiscCSV_PF[i] > 0.679 ) p4RecoBjetsCSVM.push_back( p4Jets );
			//} else {
			//	if (bdiscCSV_PF[i] > -999 ) p4RecoBjetsCSVM.push_back( p4Jets );
			//}
			/*if ( abs( jet_PF_PartonFlav[i] ) = 5 ) continue;
			if ( bdiscCSV_PF < 0.679  ) continue;
			p4RecoPartonFlavorBjetsCSVM.push_back( p4Jets );
			RecoBjetsCSVM.push_back( bdiscCSV_PF );*/

		//}
	}

	if ( dummyCounter1.size() > 0 ) cutmap["Simple"] += nEvents;
	if ( p4RecoJets.size() > 0 ) cutmap["4jetTrigger"] += nEvents;

	//cout << " 1 " << endl;
	///////////////////////////////////////////////////////
	//   Basic Plots RECO Level                          //
	///////////////////////////////////////////////////////
	
	//////// Basic plots for All jets
	double sumRecoJetsPt = 0;
	for(unsigned int j = 0; j < p4RecoJets.size(); ++j) {
		sumRecoJetsPt += p4RecoJets[j].Pt();
		basicPlots["recoJets_pt"]->Fill( p4RecoJets[j].Pt() );	
		basicPlots["recoJets_eta"]->Fill( p4RecoJets[j].Eta() );
		basicPlots["recoJets_phi"]->Fill(p4RecoJets[j].Phi());
	} 
	if ( p4RecoJets.size() > 0 ) basicPlots["recoJets_1pt"]->Fill( p4RecoJets[0].Pt() );	
	if ( p4RecoJets.size() > 1 ) basicPlots["recoJets_2pt"]->Fill( p4RecoJets[1].Pt() );	
	if ( p4RecoJets.size() > 3 ) basicPlots["recoJets_4pt"]->Fill( p4RecoJets[3].Pt() );	
	if ( p4RecoJets.size() > 4 ) basicPlots["recoJets_6pt"]->Fill( p4RecoJets[4].Pt() );	
	basicPlots["recoJets_HT"]->Fill( sumRecoJetsPt );	
	basicPlots["recoJets_num"]->Fill( p4RecoJets.size() );	

	//////// Basic plots for bs 
	double sumRecoBjetsPt = 0;
	if ( p4RecoBjetsCSVM.size() > 0 ){
		for(unsigned int k = 0; k < p4RecoBjetsCSVM.size(); ++k) {
			sumRecoBjetsPt += p4RecoBjetsCSVM[k].Pt();
			basicPlots["recoBjets_pt"]->Fill( p4RecoBjetsCSVM[k].Pt() );	
			basicPlots["recoBjets_eta"]->Fill( p4RecoBjetsCSVM[k].Eta() );
			basicPlots["recoBjets_phi"]->Fill( p4RecoBjetsCSVM[k].Phi() );
		}
	}
	basicPlots["recoBjets_num"]->Fill( p4RecoBjetsCSVM.size() );	
	basicPlots["recoBjets_sumpt"]->Fill( sumRecoBjetsPt );	
	//////////////////////////////////////////////////////////


	//cout << " 2 " << endl;
	////////////////////////////////////////////////////////////////////
	//////       STEP 1                                         ////////
	//////  Best combination of bjets with smallest delta Mass  ////////
	////////////////////////////////////////////////////////////////////

	//////// Dummy counter
	if ( p4RecoBjetsCSVM.size() > 3 ){
	       cutmap["Step1"] += nEvents;
	       basicPlots["recoJets_num_Step1"]->Fill( p4RecoJets.size() );	
	       basicPlots["recoBjets_num_Step1"]->Fill( p4RecoBjetsCSVM.size() );	
	}

	///////////////// NO Bjets requierement
	//// Structure function smallMassDiff
	smallMassDiffVectors RecoJetsHiggsCandWOBjets_Vectors;
	RecoJetsHiggsCandWOBjets_Vectors = smallMassDiff( p4RecoJets );
	std::vector< double > massRecoJetsHiggsCandWOBjets = RecoJetsHiggsCandWOBjets_Vectors.mass;
	std::vector< double > ptRecoJetsHiggsCandWOBjets = RecoJetsHiggsCandWOBjets_Vectors.pt;
	std::vector< double > scalarPtRecoJetsHiggsCandWOBjets = RecoJetsHiggsCandWOBjets_Vectors.scalarPt;
	std::vector< TLorentzVector > p4RecoJetsHiggsCandWOBjets_SmallMassDiff = RecoJetsHiggsCandWOBjets_Vectors.p4SmallMassDiff;
	double avgMassRecoJetsHiggsCandWOBjets = RecoJetsHiggsCandWOBjets_Vectors.avgMass;
	double avgPtRecoJetsHiggsCandWOBjets = RecoJetsHiggsCandWOBjets_Vectors.avgPt;

	for(unsigned int tii = 0; tii < massRecoJetsHiggsCandWOBjets.size(); ++tii) {
		///// Simple plots
		step1plots1D["massRecoJetsHiggsCandWOBjets"]->Fill( massRecoJetsHiggsCandWOBjets[tii] );
		if (tii==0) step1plots1D["avgMassRecoJetsHiggsCandWOBjets"]->Fill( avgMassRecoJetsHiggsCandWOBjets );
		if (tii==0) step1plots2D["avgPtRecoJetsHiggsCandWOBjets"]->Fill( avgPtRecoJetsHiggsCandWOBjets, avgMassRecoJetsHiggsCandWOBjets );
		step1plots2D["scalarPtRecoJetsHiggsCandWOBjets"]->Fill( scalarPtRecoJetsHiggsCandWOBjets[tii], massRecoJetsHiggsCandWOBjets[tii] );
	}
	//////////////////////////////////////////////////////////////////////////////////

	///////////////// 1Bjets requierement
	//// Structure function smallMassDiff
	if ( p4RecoBjetsCSVM.size() > 0 ){
		smallMassDiffVectors RecoJetsHiggsCandW1Bjets_Vectors;
		RecoJetsHiggsCandW1Bjets_Vectors = smallMassDiff( p4RecoJets );
		std::vector< double > massRecoJetsHiggsCandW1Bjets = RecoJetsHiggsCandW1Bjets_Vectors.mass;
		std::vector< double > ptRecoJetsHiggsCandW1Bjets = RecoJetsHiggsCandW1Bjets_Vectors.pt;
		std::vector< double > scalarPtRecoJetsHiggsCandW1Bjets = RecoJetsHiggsCandW1Bjets_Vectors.scalarPt;
		std::vector< TLorentzVector > p4RecoJetsHiggsCandW1Bjets_SmallMassDiff = RecoJetsHiggsCandW1Bjets_Vectors.p4SmallMassDiff;
		double avgMassRecoJetsHiggsCandW1Bjets = RecoJetsHiggsCandW1Bjets_Vectors.avgMass;
		double avgPtRecoJetsHiggsCandW1Bjets = RecoJetsHiggsCandW1Bjets_Vectors.avgPt;

		for(unsigned int tii = 0; tii < massRecoJetsHiggsCandW1Bjets.size(); ++tii) {
			///// Simple plots
			step1plots1D["massRecoJetsHiggsCandW1Bjets"]->Fill( massRecoJetsHiggsCandW1Bjets[tii] );
			if (tii==0) step1plots1D["avgMassRecoJetsHiggsCandW1Bjets"]->Fill( avgMassRecoJetsHiggsCandW1Bjets );
			if (tii==0) step1plots2D["avgPtRecoJetsHiggsCandW1Bjets"]->Fill( avgPtRecoJetsHiggsCandW1Bjets, avgMassRecoJetsHiggsCandW1Bjets );
			step1plots2D["scalarPtRecoJetsHiggsCandW1Bjets"]->Fill( scalarPtRecoJetsHiggsCandW1Bjets[tii], massRecoJetsHiggsCandW1Bjets[tii] );
		}
	}
	//////////////////////////////////////////////////////////////////////////////////
	
	///////////////// 2Bjets requierement
	//// Structure function smallMassDiff
	if ( p4RecoBjetsCSVM.size() > 1 ){
		smallMassDiffVectors RecoJetsHiggsCandW2Bjets_Vectors;
		RecoJetsHiggsCandW2Bjets_Vectors = smallMassDiff( p4RecoJets );
		std::vector< double > massRecoJetsHiggsCandW2Bjets = RecoJetsHiggsCandW2Bjets_Vectors.mass;
		std::vector< double > ptRecoJetsHiggsCandW2Bjets = RecoJetsHiggsCandW2Bjets_Vectors.pt;
		std::vector< double > scalarPtRecoJetsHiggsCandW2Bjets = RecoJetsHiggsCandW2Bjets_Vectors.scalarPt;
		std::vector< TLorentzVector > p4RecoJetsHiggsCandW2Bjets_SmallMassDiff = RecoJetsHiggsCandW2Bjets_Vectors.p4SmallMassDiff;
		double avgMassRecoJetsHiggsCandW2Bjets = RecoJetsHiggsCandW2Bjets_Vectors.avgMass;
		double avgPtRecoJetsHiggsCandW2Bjets = RecoJetsHiggsCandW2Bjets_Vectors.avgPt;

		for(unsigned int tii = 0; tii < massRecoJetsHiggsCandW2Bjets.size(); ++tii) {
			///// Simple plots
			step1plots1D["massRecoJetsHiggsCandW2Bjets"]->Fill( massRecoJetsHiggsCandW2Bjets[tii] );
			if (tii==0) step1plots1D["avgMassRecoJetsHiggsCandW2Bjets"]->Fill( avgMassRecoJetsHiggsCandW2Bjets );
			if (tii==0) step1plots2D["avgPtRecoJetsHiggsCandW2Bjets"]->Fill( avgPtRecoJetsHiggsCandW2Bjets, avgMassRecoJetsHiggsCandW2Bjets );
			step1plots2D["scalarPtRecoJetsHiggsCandW2Bjets"]->Fill( scalarPtRecoJetsHiggsCandW2Bjets[tii], massRecoJetsHiggsCandW2Bjets[tii] );
		}
	}
	//////////////////////////////////////////////////////////////////////////////////
	
	///////////////// 3Bjets requierement
	//// Structure function smallMassDiff
	if ( p4RecoBjetsCSVM.size() > 2 ){
		smallMassDiffVectors RecoJetsHiggsCandW3Bjets_Vectors;
		RecoJetsHiggsCandW3Bjets_Vectors = smallMassDiff( p4RecoJets );
		std::vector< double > massRecoJetsHiggsCandW3Bjets = RecoJetsHiggsCandW3Bjets_Vectors.mass;
		std::vector< double > ptRecoJetsHiggsCandW3Bjets = RecoJetsHiggsCandW3Bjets_Vectors.pt;
		std::vector< double > scalarPtRecoJetsHiggsCandW3Bjets = RecoJetsHiggsCandW3Bjets_Vectors.scalarPt;
		std::vector< TLorentzVector > p4RecoJetsHiggsCandW3Bjets_SmallMassDiff = RecoJetsHiggsCandW3Bjets_Vectors.p4SmallMassDiff;
		double avgMassRecoJetsHiggsCandW3Bjets = RecoJetsHiggsCandW3Bjets_Vectors.avgMass;
		double avgPtRecoJetsHiggsCandW3Bjets = RecoJetsHiggsCandW3Bjets_Vectors.avgPt;

		for(unsigned int tii = 0; tii < massRecoJetsHiggsCandW3Bjets.size(); ++tii) {
			///// Simple plots
			step1plots1D["massRecoJetsHiggsCandW3Bjets"]->Fill( massRecoJetsHiggsCandW3Bjets[tii] );
			if (tii==0) step1plots1D["avgMassRecoJetsHiggsCandW3Bjets"]->Fill( avgMassRecoJetsHiggsCandW3Bjets );
			if (tii==0) step1plots2D["avgPtRecoJetsHiggsCandW3Bjets"]->Fill( avgPtRecoJetsHiggsCandW3Bjets, avgMassRecoJetsHiggsCandW3Bjets );
			step1plots2D["scalarPtRecoJetsHiggsCandW3Bjets"]->Fill( scalarPtRecoJetsHiggsCandW3Bjets[tii], massRecoJetsHiggsCandW3Bjets[tii] );
		}
	}
	//////////////////////////////////////////////////////////////////////////////////
	
	///////////////// 4Bjets requierement
	//// Structure function smallMassDiff
	if ( p4RecoBjetsCSVM.size() > 3 ){
		smallMassDiffVectors RecoJetsHiggsCandW4Bjets_Vectors;
		RecoJetsHiggsCandW4Bjets_Vectors = smallMassDiff( p4RecoJets );
		std::vector< double > massRecoJetsHiggsCandW4Bjets = RecoJetsHiggsCandW4Bjets_Vectors.mass;
		std::vector< double > ptRecoJetsHiggsCandW4Bjets = RecoJetsHiggsCandW4Bjets_Vectors.pt;
		std::vector< double > scalarPtRecoJetsHiggsCandW4Bjets = RecoJetsHiggsCandW4Bjets_Vectors.scalarPt;
		std::vector< TLorentzVector > p4RecoJetsHiggsCandW4Bjets_SmallMassDiff = RecoJetsHiggsCandW4Bjets_Vectors.p4SmallMassDiff;
		double avgMassRecoJetsHiggsCandW4Bjets = RecoJetsHiggsCandW4Bjets_Vectors.avgMass;
		double avgPtRecoJetsHiggsCandW4Bjets = RecoJetsHiggsCandW4Bjets_Vectors.avgPt;

		for(unsigned int tii = 0; tii < massRecoJetsHiggsCandW4Bjets.size(); ++tii) {
			///// Simple plots
			step1plots1D["massRecoJetsHiggsCandW4Bjets"]->Fill( massRecoJetsHiggsCandW4Bjets[tii] );
			if (tii==0) step1plots1D["avgMassRecoJetsHiggsCandW4Bjets"]->Fill( avgMassRecoJetsHiggsCandW4Bjets );
			if (tii==0) step1plots2D["avgPtRecoJetsHiggsCandW4Bjets"]->Fill( avgPtRecoJetsHiggsCandW4Bjets, avgMassRecoJetsHiggsCandW4Bjets );
			step1plots2D["scalarPtRecoJetsHiggsCandW4Bjets"]->Fill( scalarPtRecoJetsHiggsCandW4Bjets[tii], massRecoJetsHiggsCandW4Bjets[tii] );
		}
	}
	//////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////  RecoBjets with Btagging
	
	//// Structure function smallMassDiff
	smallMassDiffVectors RecoBjetsCSVM_Vectors;
	RecoBjetsCSVM_Vectors = smallMassDiff( p4RecoBjetsCSVM );
	std::vector< double > massRecoBjetsCSVM = RecoBjetsCSVM_Vectors.mass;
	std::vector< double > ptRecoBjetsCSVM = RecoBjetsCSVM_Vectors.pt;
	std::vector< double > scalarPtRecoBjetsCSVM = RecoBjetsCSVM_Vectors.scalarPt;
	std::vector< TLorentzVector > p4RecoBjetsCSVM_SmallMassDiff = RecoBjetsCSVM_Vectors.p4SmallMassDiff;
	double avgMassRecoBjetsCSVM = RecoBjetsCSVM_Vectors.avgMass;
	double avgPtRecoBjetsCSVM = RecoBjetsCSVM_Vectors.avgPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb50;
	std::vector< TLorentzVector > p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb100;
	std::vector< TLorentzVector > p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb150;
	std::vector< TLorentzVector > p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb200;

	for(unsigned int tii = 0; tii < massRecoBjetsCSVM.size(); ++tii) {
		///// Simple plots
		step1plots1D["massRecoBjetsCSVM"]->Fill( massRecoBjetsCSVM[tii] );
		if (tii==0) step1plots1D["avgMassRecoBjetsCSVM"]->Fill( avgMassRecoBjetsCSVM );
		if (tii==0) step1plots2D["avgPtRecoBjetsCSVM"]->Fill( avgPtRecoBjetsCSVM, avgMassRecoBjetsCSVM );
		step1plots2D["PtRecoBjetsCSVM"]->Fill( ptRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );
		step1plots2D["scalarPtRecoBjetsCSVM"]->Fill( scalarPtRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );

		////////////// Mass Window Cut - NO Included - Test Only
		//cout << " 1 " << massRecoBjetsCSVM[0] << " " << massRecoBjetsCSVM[1] << endl;
		//	if ( massRecoBjetsCSVM[0] < 130 && massRecoBjetsCSVM[0] > 120 ) continue;
		//	if ( massRecoBjetsCSVM[1] < 130 && massRecoBjetsCSVM[1] > 120 ) continue;
		//if ( massRecoBjetsCSVM[0] < 130 ) continue;
		//if ( massRecoBjetsCSVM[1] > 120 ) continue;
		//if ( massRecoBjetsCSVM[0] < 130 ) continue;
		//if ( massRecoBjetsCSVM[1] > 120 ) continue;
		//cout << " 2 " << massRecoBjetsCSVM[0] << " " << massRecoBjetsCSVM[1] << endl;
		////////////////////////////////////////////////////////////////////////////////////////

		/*//// Diagonal cuts
		double iDiag=(double)tii*10.0+50.0;
		if( massRecoBjetsCSVM[tii] < ( scalarPtRecoBjetsCSVM[tii]-iDiag ) ) {
			step1plots1D["massRecoBjetsCSVM_cutDiagHiggsbb50"]->Fill( massRecoBjetsCSVM[tii] );
			if (tii==0) step1plots1D["avgMassRecoBjetsCSVM_cutDiagHiggsbb50"]->Fill( avgMassRecoBjetsCSVM );
			step1plots2D["scalarPtRecoBjetsCSVM_cutDiagHiggsbb50"]->Fill( scalarPtRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );
			if (tii==0) step1plots2D["avgPtRecoBjetsCSVM_cutDiagHiggsbb50"]->Fill( avgPtRecoBjetsCSVM, avgMassRecoBjetsCSVM);
			p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb50 = p4RecoBjetsCSVM_SmallMassDiff;
		}
							
		double iiDiag=(double)tii*10.0+100.0;
		if( massRecoBjetsCSVM[tii] < ( scalarPtRecoBjetsCSVM[tii]-iiDiag ) ) {
			step1plots1D["massRecoBjetsCSVM_cutDiagHiggsbb100"]->Fill( massRecoBjetsCSVM[tii] );
			if (tii==0) step1plots1D["avgMassRecoBjetsCSVM_cutDiagHiggsbb100"]->Fill( avgMassRecoBjetsCSVM );
			step1plots2D["scalarPtRecoBjetsCSVM_cutDiagHiggsbb100"]->Fill( scalarPtRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );
			if (tii==0) step1plots2D["avgPtRecoBjetsCSVM_cutDiagHiggsbb100"]->Fill( avgPtRecoBjetsCSVM,avgMassRecoBjetsCSVM );
			p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb100 = p4RecoBjetsCSVM_SmallMassDiff;
		}
		
		double iiiDiag=(double)tii*10.0+150.0;
		if( massRecoBjetsCSVM[tii] < ( scalarPtRecoBjetsCSVM[tii]-iiiDiag ) ) {
			step1plots1D["massRecoBjetsCSVM_cutDiagHiggsbb150"]->Fill( massRecoBjetsCSVM[tii] );
			if (tii==0) step1plots1D["avgMassRecoBjetsCSVM_cutDiagHiggsbb150"]->Fill( avgMassRecoBjetsCSVM );
			step1plots2D["scalarPtRecoBjetsCSVM_cutDiagHiggsbb150"]->Fill( scalarPtRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );
			if (tii==0) step1plots2D["avgPtRecoBjetsCSVM_cutDiagHiggsbb150"]->Fill( avgPtRecoBjetsCSVM, avgMassRecoBjetsCSVM );
			p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb150 = p4RecoBjetsCSVM_SmallMassDiff;
		}

		double ivDiag=(double)tii*10.0+200.0;
		if( massRecoBjetsCSVM[tii] < ( scalarPtRecoBjetsCSVM[tii]-ivDiag ) ){
			step1plots1D["massRecoBjetsCSVM_cutDiagHiggsbb200"]->Fill( massRecoBjetsCSVM[tii] );
			if (tii==0) step1plots1D["avgMassRecoBjetsCSVM_cutDiagHiggsbb200"]->Fill( avgMassRecoBjetsCSVM );
			step1plots2D["scalarPtRecoBjetsCSVM_cutDiagHiggsbb200"]->Fill( scalarPtRecoBjetsCSVM[tii], massRecoBjetsCSVM[tii] );
			if (tii==0) step1plots2D["avgPtRecoBjetsCSVM_cutDiagHiggsbb200"]->Fill( avgPtRecoBjetsCSVM, avgMassRecoBjetsCSVM );
			p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb200 = p4RecoBjetsCSVM_SmallMassDiff;
		}*/

	} 
	//////////////////////////////////////////////////////////

	///// PartonFlavor and  RecoBjets with Btagging
	//// Structure function smallMassDiff
	smallMassDiffVectors RecoPartonFlavorBjetsCSVM_Vectors;
	RecoPartonFlavorBjetsCSVM_Vectors = smallMassDiff( p4RecoPartonFlavorBjetsCSVM );
	std::vector< double > massRecoPartonFlavorBjetsCSVM = RecoPartonFlavorBjetsCSVM_Vectors.mass;
	std::vector< double > ptRecoPartonFlavorBjetsCSVM = RecoPartonFlavorBjetsCSVM_Vectors.pt;
	std::vector< double > scalarPtRecoPartonFlavorBjetsCSVM = RecoPartonFlavorBjetsCSVM_Vectors.scalarPt;
	std::vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM_SmallMassDiff = RecoPartonFlavorBjetsCSVM_Vectors.p4SmallMassDiff;
	double avgMassRecoPartonFlavorBjetsCSVM = RecoPartonFlavorBjetsCSVM_Vectors.avgMass;
	double avgPtRecoPartonFlavorBjetsCSVM = RecoPartonFlavorBjetsCSVM_Vectors.avgPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb50;
	std::vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb100;
	std::vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb150;
	std::vector< TLorentzVector > p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb200;

	for(unsigned int tiii = 0; tiii < massRecoPartonFlavorBjetsCSVM.size(); ++tiii) {
		///// Simple plots
		step1plots1D["massRecoPartonFlavorBjetsCSVM"]->Fill( massRecoPartonFlavorBjetsCSVM[tiii] );
		if (tiii==0) step1plots1D["avgMassRecoPartonFlavorBjetsCSVM"]->Fill( avgMassRecoPartonFlavorBjetsCSVM );
		step1plots2D["PtRecoPartonFlavorBjetsCSVM"]->Fill( ptRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
		step1plots2D["scalarPtRecoPartonFlavorBjetsCSVM"]->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
		if (tiii==0) step1plots2D["avgPtRecoPartonFlavorBjetsCSVM"]->Fill( avgPtRecoPartonFlavorBjetsCSVM, avgMassRecoPartonFlavorBjetsCSVM );

		/*//// Diagonal cuts
		double iDiag=(double)tiii*10.0+50.0;
		if( massRecoPartonFlavorBjetsCSVM[tiii] < ( scalarPtRecoPartonFlavorBjetsCSVM[tiii]-iDiag ) ) {
			step1plots1D["massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50"]->Fill( massRecoPartonFlavorBjetsCSVM[tiii] );
			if (tiii==0) step1plots1D["avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50"]->Fill( avgMassRecoPartonFlavorBjetsCSVM );
			step1plots2D["scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50"]->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			if (tiii==0) step1plots2D["avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50"]->Fill( avgPtRecoPartonFlavorBjetsCSVM, avgMassRecoPartonFlavorBjetsCSVM );
			p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb50 = p4RecoPartonFlavorBjetsCSVM_SmallMassDiff;
		}
		
		double iiDiag=(double)tiii*10.0+100.0;
		if( massRecoPartonFlavorBjetsCSVM[tiii] < ( scalarPtRecoPartonFlavorBjetsCSVM[tiii]-iiDiag ) ) {
			step1plots1D["massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100"]->Fill( massRecoPartonFlavorBjetsCSVM[tiii] );
			if (tiii==0) step1plots1D["avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100"]->Fill( avgMassRecoPartonFlavorBjetsCSVM );
			step1plots2D["scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100"]->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			if (tiii==0) step1plots2D["avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100"]->Fill( avgPtRecoPartonFlavorBjetsCSVM, avgMassRecoPartonFlavorBjetsCSVM );
			p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb100 = p4RecoPartonFlavorBjetsCSVM_SmallMassDiff;
		}
		
		double iiiDiag=(double)tiii*10.0+150.0;
		if( massRecoPartonFlavorBjetsCSVM[tiii] < ( scalarPtRecoPartonFlavorBjetsCSVM[tiii]-iiiDiag ) ) {
			step1plots1D["massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150"]->Fill( massRecoPartonFlavorBjetsCSVM[tiii] );
			if (tiii==0) step1plots1D["avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150"]->Fill( avgMassRecoPartonFlavorBjetsCSVM );
			step1plots2D["scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150"]->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			if (tiii==0) step1plots2D["avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150"]->Fill( avgPtRecoPartonFlavorBjetsCSVM, avgMassRecoPartonFlavorBjetsCSVM );
			p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb150 = p4RecoPartonFlavorBjetsCSVM_SmallMassDiff;
		}

		double ivDiag=(double)tiii*10.0+200.0;
		if( massRecoPartonFlavorBjetsCSVM[tiii] < ( scalarPtRecoPartonFlavorBjetsCSVM[tiii]-ivDiag ) ) {
			step1plots1D["massRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200"]->Fill( massRecoPartonFlavorBjetsCSVM[tiii] );
			if (tiii==0) step1plots1D["avgMassRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200"]->Fill( avgMassRecoPartonFlavorBjetsCSVM );
			step1plots2D["scalarPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200"]->Fill( scalarPtRecoPartonFlavorBjetsCSVM[tiii], massRecoPartonFlavorBjetsCSVM[tiii] );
			if (tiii==0) step1plots2D["avgPtRecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200"]->Fill( avgPtRecoPartonFlavorBjetsCSVM, avgMassRecoPartonFlavorBjetsCSVM );
			p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb200 = p4RecoPartonFlavorBjetsCSVM_SmallMassDiff;
		}*/

	} 
	//////////////////////////////////////////////////////////

	//cout << " 3 " << endl;
	///////////////////////////////////////
	/////////// STEP 2  ////////////////
	/////// Mass dijet vs sum pt dijet //////////
	///////////////////////////////////////

	///////// Maximum and MInimum values for histo without resonance
	int maxStop1 = st1mass+40;
	int minStop1 = st1mass-40;

	///////// RECO jets + Btagging CSVM
	std::vector< TLorentzVector > p4jetsWORecoJetsCSVM;
	std::vector< TLorentzVector > p4bWORecoBjetsCSVM;
	std::vector< TLorentzVector > p4jetsWORecoBjetsCSVM;
	std::vector< TLorentzVector > p4jetsWORecoBjetsCSVM_cutDiagHiggsbb50;
	std::vector< TLorentzVector > p4jetsWORecoBjetsCSVM_cutDiagHiggsbb100;
	std::vector< TLorentzVector > p4jetsWORecoBjetsCSVM_cutDiagHiggsbb150;
	std::vector< TLorentzVector > p4jetsWORecoBjetsCSVM_cutDiagHiggsbb200;

	if ( p4RecoJets.size() > 5 && p4RecoBjetsCSVM.size() > 3 ){

		cutmap["Step2"] += nEvents;
		basicPlots["recoJets_num_Step2"]->Fill( p4RecoJets.size() );	
		basicPlots["recoBjets_num_Step2"]->Fill( p4RecoBjetsCSVM.size() );	

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
				step2plots2D["recoDijet_masspt"]->Fill( sumRecoBtagJetsdijetpt, p4RecoBtagJetsdijet.M() );
			}
			
			////// Step 2 - dijet mass vs sum pt of jets without bs from step 1
			//p4jetsWORecoJetsCSVM = compare2TLVReturnNoEqual( p4RecoJets, p4RecoBjetsCSVM );
			//if ( p4RecoBjetsCSVM_SmallMassDiff.size() == 4 ) p4bWORecoBjetsCSVM = compare2TLVReturnNoEqual( p4RecoBjetsCSVM, p4RecoBjetsCSVM_SmallMassDiff );
			if ( p4RecoBjetsCSVM_SmallMassDiff.size() == 4 ) p4jetsWORecoBjetsCSVM = compare2TLVReturnNoEqual( p4RecoJets, p4RecoBjetsCSVM_SmallMassDiff );
			/*if ( p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb50.size() == 4 ) p4jetsWORecoBjetsCSVM_cutDiagHiggsbb50 = compare2TLVReturnNoEqual( p4RecoJets, p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb50 );
			if ( p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb100.size() == 4 ) p4jetsWORecoBjetsCSVM_cutDiagHiggsbb100 = compare2TLVReturnNoEqual( p4RecoJets, p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb100 );
			if ( p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb150.size() == 4 ) p4jetsWORecoBjetsCSVM_cutDiagHiggsbb150 = compare2TLVReturnNoEqual( p4RecoJets, p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb150 );
			if ( p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb200.size() == 4 ) p4jetsWORecoBjetsCSVM_cutDiagHiggsbb200 = compare2TLVReturnNoEqual( p4RecoJets, p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb200 ); */
		}
	}
			
	/////////////////////// STEP 2 - NO cutDiagHiggsbb 
	//// Structure function s2Vectors
	s2VecMassSumPt vectorsjWORecoBjetsCSVM;
	vectorsjWORecoBjetsCSVM = s2Vectors( p4jetsWORecoBjetsCSVM );
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM = vectorsjWORecoBjetsCSVM.s2VectorTL;
	std::vector< double > massdijetWORecoBjetsCSVM = vectorsjWORecoBjetsCSVM.s2Mass;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM = vectorsjWORecoBjetsCSVM.s2ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagStop1jj10;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagStop1jj20;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagStop1jj30;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagStop1jj40;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagStop1jj60;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagStop1jj70;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagStop1jj80;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagStop1jj90;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagStop1jj200;

	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj10;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj20;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj30;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj40;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj50;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj60;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj70;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj80;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj90;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj100;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj150;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj200;


	for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM.size(); ++iim) {
		step2plots2D["dijetWORecoBjetsCSVM_masspt"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
		step2plots1D["massdijetWORecoBjetsCSVM"]->Fill( massdijetWORecoBjetsCSVM[iim] );
		step2plots1D["massdijetWORecoBjetsCSVM_resoBasedBin"]->Fill( massdijetWORecoBjetsCSVM[iim] );
		if ( massdijetWORecoBjetsCSVM[iim] < minStop1 ||  massdijetWORecoBjetsCSVM[iim] > maxStop1) {
			step2plots1D["massdijetWORecoBjetsCSVM_WOStop1"]->Fill( massdijetWORecoBjetsCSVM[iim] );
		}

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+10.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-iDiag ) ) {
			//step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj10"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj10"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagStop1jj10 = p4dijetWORecoBjetsCSVM;
			scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj10 = scalarSumPtdijetWORecoBjetsCSVM;
			/*if ( massdijetWORecoBjetsCSVM[iim] < minStop1 ||  massdijetWORecoBjetsCSVM[iim] > maxStop1) {
				step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj10_WOStop1"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			}*/
		}

		double iiDiag=(double)iim*10.0+20.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-iiDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj20"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj20"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagStop1jj20 = p4dijetWORecoBjetsCSVM;
			scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj20 = scalarSumPtdijetWORecoBjetsCSVM;
		}

		double iiiDiag=(double)iim*10.0+30.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-iiiDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj30"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj30"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagStop1jj30 = p4dijetWORecoBjetsCSVM;
			scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj30 = scalarSumPtdijetWORecoBjetsCSVM;
		}

		double ivDiag=(double)iim*10.0+40.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-ivDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj40"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj40"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagStop1jj40 = p4dijetWORecoBjetsCSVM;
			scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj40 = scalarSumPtdijetWORecoBjetsCSVM;
		}

		double vDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-vDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj50"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagStop1jj50 = p4dijetWORecoBjetsCSVM;
			scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj50 = scalarSumPtdijetWORecoBjetsCSVM;
			if ( massdijetWORecoBjetsCSVM[iim] < minStop1 ||  massdijetWORecoBjetsCSVM[iim] > maxStop1) {
				step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj50_WOStop1"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			}
		}

		double viDiag=(double)iim*10.0+60.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-viDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj60"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj60"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagStop1jj60 = p4dijetWORecoBjetsCSVM;
			scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj60 = scalarSumPtdijetWORecoBjetsCSVM;
		}

		double viiDiag=(double)iim*10.0+70.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-viiDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj70"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj70"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagStop1jj70 = p4dijetWORecoBjetsCSVM;
			scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj70 = scalarSumPtdijetWORecoBjetsCSVM;
		}

		double viiiDiag=(double)iim*10.0+80.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-viiiDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj80"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj80"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagStop1jj80 = p4dijetWORecoBjetsCSVM;
			scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj80 = scalarSumPtdijetWORecoBjetsCSVM;
		}

		double ixDiag=(double)iim*10.0+90.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-ixDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj90"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj90"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagStop1jj90 = p4dijetWORecoBjetsCSVM;
			scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj90 = scalarSumPtdijetWORecoBjetsCSVM;
		}

		double xDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-xDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj100"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagStop1jj100 = p4dijetWORecoBjetsCSVM;
			scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj100 = scalarSumPtdijetWORecoBjetsCSVM;
			if ( massdijetWORecoBjetsCSVM[iim] < minStop1 ||  massdijetWORecoBjetsCSVM[iim] > maxStop1) {
				step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj100_WOStop1"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			}
		}

		double xvDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-xvDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj150"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagStop1jj150 = p4dijetWORecoBjetsCSVM;
			scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj150 = scalarSumPtdijetWORecoBjetsCSVM;
			if ( massdijetWORecoBjetsCSVM[iim] < minStop1 ||  massdijetWORecoBjetsCSVM[iim] > maxStop1) {
				step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj150_WOStop1"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			}
		}

		double xxDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-xxDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj200"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagStop1jj200 = p4dijetWORecoBjetsCSVM;
			scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj200 = scalarSumPtdijetWORecoBjetsCSVM;
		}

	}

	//////// Check number of bjets and events
	if ( p4dijetWORecoBjetsCSVM_cutDiagStop1jj50.size() > 0 ){
	       cutmap["Step2_diag50"] += nEvents;
	       basicPlots["recoJets_num_Step2_diag50"]->Fill( p4RecoJets.size() );	
	       basicPlots["recoBjets_num_Step2_diag50"]->Fill( p4RecoBjetsCSVM.size() );	
	}
	if ( p4dijetWORecoBjetsCSVM_cutDiagStop1jj100.size() > 0 ){
	       cutmap["Step2_diag100"] += nEvents;
	       basicPlots["recoJets_num_Step2_diag100"]->Fill( p4RecoJets.size() );	
	       basicPlots["recoBjets_num_Step2_diag100"]->Fill( p4RecoBjetsCSVM.size() );	
	}
	if ( p4dijetWORecoBjetsCSVM_cutDiagStop1jj150.size() > 0 ){
	       cutmap["Step2_diag150"] += nEvents;
	       basicPlots["recoJets_num_Step2_diag150"]->Fill( p4RecoJets.size() );	
	       basicPlots["recoBjets_num_Step2_diag150"]->Fill( p4RecoBjetsCSVM.size() );	
	}
	if ( p4dijetWORecoBjetsCSVM_cutDiagStop1jj200.size() > 0 ){
	       cutmap["Step2_diag200"] += nEvents;
	       basicPlots["recoJets_num_Step2_diag200"]->Fill( p4RecoJets.size() );	
	       basicPlots["recoBjets_num_Step2_diag200"]->Fill( p4RecoBjetsCSVM.size() );	
	}


	/*////////////////////// STEP 2 - cutDiagHiggsbb50 
	s2VecMassSumPt vectorsjWORecoBjetsCSVM_cutDiagHiggsbb50;
	vectorsjWORecoBjetsCSVM_cutDiagHiggsbb50 = s2Vectors( p4jetsWORecoBjetsCSVM_cutDiagHiggsbb50 );
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb50.s2VectorTL;
	std::vector< double > massdijetWORecoBjetsCSVM_cutDiagHiggsbb50 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb50.s2Mass;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb50.s2ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM_cutDiagHiggsbb50.size(); ++iim) {
		step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
		step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim]-iDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim]-iiDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim]-iiiDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim]-ivDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50;
		}

	}

	/////////////////////// STEP 2 - cutDiagHiggsbb100 
	s2VecMassSumPt vectorsjWORecoBjetsCSVM_cutDiagHiggsbb100;
	vectorsjWORecoBjetsCSVM_cutDiagHiggsbb100 = s2Vectors( p4jetsWORecoBjetsCSVM_cutDiagHiggsbb100 );
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb100.s2VectorTL;
	std::vector< double > massdijetWORecoBjetsCSVM_cutDiagHiggsbb100 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb100.s2Mass;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb100.s2ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM_cutDiagHiggsbb100.size(); ++iim) {
		step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
		step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim]-iDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim]-iiDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim]-iiiDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim]-ivDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100;
		}

	}

	/////////////////////// STEP 2 - cutDiagHiggsbb150 
	s2VecMassSumPt vectorsjWORecoBjetsCSVM_cutDiagHiggsbb150;
	vectorsjWORecoBjetsCSVM_cutDiagHiggsbb150 = s2Vectors( p4jetsWORecoBjetsCSVM_cutDiagHiggsbb150 );
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb150.s2VectorTL;
	std::vector< double > massdijetWORecoBjetsCSVM_cutDiagHiggsbb150 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb150.s2Mass;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb150.s2ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM_cutDiagHiggsbb150.size(); ++iim) {
		step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
		step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim]-iDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim]-iiDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim]-iiiDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim]-ivDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150;
		}
	}

	/////////////////////// STEP 2 - cutDiagHiggsbb200 
	s2VecMassSumPt vectorsjWORecoBjetsCSVM_cutDiagHiggsbb200;
	vectorsjWORecoBjetsCSVM_cutDiagHiggsbb200 = s2Vectors( p4jetsWORecoBjetsCSVM_cutDiagHiggsbb200 );
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb200.s2VectorTL;
	std::vector< double > massdijetWORecoBjetsCSVM_cutDiagHiggsbb200 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb200.s2Mass;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb200.s2ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM_cutDiagHiggsbb200.size(); ++iim) {
		step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
		step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim]-iDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim]-iiDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim]-iiiDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim]-ivDiag ) ) {
			step2plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			step2plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200;
		}
	}
	////////////////////////////////////////////////////*/

	///////// RECO jets + PartonFlavor + Btagging CSVM
	std::vector< TLorentzVector > p4jetsWORecoPartonFlavorBjetsCSVM;
	std::vector< TLorentzVector > p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
	std::vector< TLorentzVector > p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
	std::vector< TLorentzVector > p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
	std::vector< TLorentzVector > p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;

	if ( p4RecoJets.size() > 5 && p4RecoPartonFlavorBjetsCSVM.size() > 3 ){
		for(unsigned int iii = 0; iii < p4RecoJets.size(); ++iii) {

			////// Step 2 - dijet mass vs sum pt of jets
			for(unsigned int iij = 0; iij < p4RecoJets.size(); ++iij) {
				if ( iii>=iij ) continue;
				TLorentzVector p4RecoPartonFlavorBtagJetsdijet = p4RecoJets[iii] + p4RecoJets[iij];
				double sumRecoPartonFlavorBtagJetsdijetpt = p4RecoJets[iii].Pt() + p4RecoJets[iij].Pt();
				step2plots2D["recoPartonFlavorDijet_masspt"]->Fill( sumRecoPartonFlavorBtagJetsdijetpt, p4RecoPartonFlavorBtagJetsdijet.M() );
			}
			
			////// Step 2 - dijet mass vs sum pt of jets without bs from step 1
			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff.size() == 4 ) p4jetsWORecoPartonFlavorBjetsCSVM = compare2TLVReturnNoEqual( p4RecoJets, p4RecoPartonFlavorBjetsCSVM_SmallMassDiff );
			/*if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb50.size() == 4 ) p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 = compare2TLVReturnNoEqual( p4RecoJets, p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb50 );
			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb100.size() == 4 ) p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 = compare2TLVReturnNoEqual( p4RecoJets, p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb100 );
			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb150.size() == 4 ) p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 = compare2TLVReturnNoEqual( p4RecoJets, p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb150 );
			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb200.size() == 4 ) p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 = compare2TLVReturnNoEqual( p4RecoJets, p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb200 ); */
		}
	}

	/////////////////////// STEP 2 - NO cutDiagHiggsbb 
	//// Structure function s2Vectors
	s2VecMassSumPt vectorsjWORecoPartonFlavorBjetsCSVM;
	vectorsjWORecoPartonFlavorBjetsCSVM = s2Vectors( p4jetsWORecoPartonFlavorBjetsCSVM );
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM = vectorsjWORecoPartonFlavorBjetsCSVM.s2VectorTL;
	std::vector< double > massdijetWORecoPartonFlavorBjetsCSVM = vectorsjWORecoPartonFlavorBjetsCSVM.s2Mass;
	std::vector< double > scalarSumPtdijetWORecoPartonFlavorBjetsCSVM = vectorsjWORecoPartonFlavorBjetsCSVM.s2ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoPartonFlavorBjetsCSVM.size(); ++iim) {
		step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim], massdijetWORecoPartonFlavorBjetsCSVM[iim] );
		step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim]-iDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim], massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj50"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj50 = p4dijetWORecoPartonFlavorBjetsCSVM;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim]-iiDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim], massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj100"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj100 = p4dijetWORecoPartonFlavorBjetsCSVM;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim]-iiiDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim], massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj150"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj150 = p4dijetWORecoPartonFlavorBjetsCSVM;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim]-ivDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim], massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj200"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj200 = p4dijetWORecoPartonFlavorBjetsCSVM;
		}

	}

	/*////////////////////// STEP 2 - cutDiagHiggsbb50 
	s2VecMassSumPt vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
	vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 = s2Vectors( p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 );
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50.s2VectorTL;
	std::vector< double > massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50.s2Mass;
	std::vector< double > scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50.s2ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50.size(); ++iim) {
		step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
		step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim]-iDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim]-iiDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim]-iiiDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim]-ivDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
		}

	}

	/////////////////////// STEP 2 - cutDiagHiggsbb100 
	s2VecMassSumPt vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
	vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 = s2Vectors( p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 );
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100.s2VectorTL;
	std::vector< double > massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100.s2Mass;
	std::vector< double > scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100.s2ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100.size(); ++iim) {
		step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
		step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim]-iDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim]-iiDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim]-iiiDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim]-ivDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
		}

	}

	/////////////////////// STEP 2 - cutDiagHiggsbb150 
	s2VecMassSumPt vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
	vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 = s2Vectors( p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 );
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150.s2VectorTL;
	std::vector< double > massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150.s2Mass;
	std::vector< double > scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150.s2ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150.size(); ++iim) {
		step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
		step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim]-iDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim]-iiDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim]-iiiDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim]-ivDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
		}
	}

	/////////////////////// STEP 2 - cutDiagHiggsbb200 
	s2VecMassSumPt vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;
	vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 = s2Vectors( p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 );
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200.s2VectorTL;
	std::vector< double > massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200.s2Mass;
	std::vector< double > scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200.s2ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200.size(); ++iim) {
		step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
		step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim]-iDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim]-iiDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim]-iiiDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim]-ivDiag ) ) {
			step2plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			step2plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;
		}
	}
			
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

	//cout << " 4 " << endl;
	////////////////////////////////////////////////////////////////
	//////           STEP 3                                    /////
	//////  Mass of MatchBjets from 1 and dijet from 3 vs sum pt of MatchBjetsdijet  /////
	////////////////////////////////////////////////////////////////
	
	/////// Reco Jets
	// NO diagCutHiggs neither diagCutStop1
	//// TLorentzVectors for cuts
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop2bbjj10;
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop2bbjj20;
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop2bbjj30;
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop2bbjj40;
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop2bbjj50;
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop2bbjj60;
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop2bbjj70;
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop2bbjj80;
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop2bbjj90;
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop2bbjj100;
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop2bbjj150;

	//// Structure function s3Vectors
	s3Vectors vectorsStep3;
	vectorsStep3 = step3Vectors( p4dijetWORecoBjetsCSVM, p4RecoBjetsCSVM_SmallMassDiff, scalarSumPtdijetWORecoBjetsCSVM );
	vector< TLorentzVector > p4RecoDiBjetDiJet = vectorsStep3.s3TLV;
	vector< double > massRecoDiBjetDiJet = vectorsStep3.s3Mass;
	vector< double > scalarSumPtRecoDiBjetDiJet = vectorsStep3.s3ScalarPt;

	int dummyCounter = 0;

	for(unsigned int aa = 0; aa < p4RecoDiBjetDiJet.size(); ++aa) {
		step3plots2D["recoDiBjetDiJet_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet[aa], massRecoDiBjetDiJet[aa] );
		//step3plots2D["recoDiBjetDiJet_MassPt"]->Fill( p4RecoDiBjetDiJet[aa].Pt(), p4RecoDiBjetDiJet[aa].M() );
		step3plots1D["massRecoDiBjetDiJet"]->Fill( massRecoDiBjetDiJet[aa] );
		step3plots1D["massRecoDiBjetDiJet_resoBasedBin"]->Fill( massRecoDiBjetDiJet[aa] );

		double zDiag2=(double)aa*10.0;
		if( massRecoDiBjetDiJet[aa] < ( scalarSumPtRecoDiBjetDiJet[aa]-zDiag2 ) ) {
			step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj0_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet[aa], massRecoDiBjetDiJet[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj0"]->Fill( massRecoDiBjetDiJet[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj0_resoBasedBin"]->Fill( massRecoDiBjetDiJet[aa] );
			dummyCounter++;
		}
		double iDiag2=(double)aa*10.0+10.0;
		if( massRecoDiBjetDiJet[aa] < ( scalarSumPtRecoDiBjetDiJet[aa]-iDiag2 ) ) {
			step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj10_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet[aa], massRecoDiBjetDiJet[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj10"]->Fill( massRecoDiBjetDiJet[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj10_resoBasedBin"]->Fill( massRecoDiBjetDiJet[aa] );
		}
		double iiDiag2=(double)aa*10.0+20.0;
		if( massRecoDiBjetDiJet[aa] < ( scalarSumPtRecoDiBjetDiJet[aa]-iiDiag2 ) ) {
			step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj20_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet[aa], massRecoDiBjetDiJet[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj20"]->Fill( massRecoDiBjetDiJet[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj20_resoBasedBin"]->Fill( massRecoDiBjetDiJet[aa] );
		}
		double iiiDiag2=(double)aa*10.0+30.0;
		if( massRecoDiBjetDiJet[aa] < ( scalarSumPtRecoDiBjetDiJet[aa]-iiiDiag2 ) ) {
			step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj30_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet[aa], massRecoDiBjetDiJet[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj30"]->Fill( massRecoDiBjetDiJet[aa] );
		}
		double ivDiag2=(double)aa*10.0+40.0;
		if( massRecoDiBjetDiJet[aa] < ( scalarSumPtRecoDiBjetDiJet[aa]-ivDiag2 ) ) {
			step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj40_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet[aa], massRecoDiBjetDiJet[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj40"]->Fill( massRecoDiBjetDiJet[aa] );
		}
		double vDiag2=(double)aa*10.0+50.0;
		if( massRecoDiBjetDiJet[aa] < ( scalarSumPtRecoDiBjetDiJet[aa]-vDiag2 ) ) {
			step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj50_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet[aa], massRecoDiBjetDiJet[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj50"]->Fill( massRecoDiBjetDiJet[aa] );
		}
		double viDiag2=(double)aa*10.0+60.0;
		if( massRecoDiBjetDiJet[aa] < ( scalarSumPtRecoDiBjetDiJet[aa]-viDiag2 ) ) {
			step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj60_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet[aa], massRecoDiBjetDiJet[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj60"]->Fill( massRecoDiBjetDiJet[aa] );
		}
		double viiDiag2=(double)aa*10.0+70.0;
		if( massRecoDiBjetDiJet[aa] < ( scalarSumPtRecoDiBjetDiJet[aa]-viiDiag2 ) ) {
			step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj70_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet[aa], massRecoDiBjetDiJet[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj70"]->Fill( massRecoDiBjetDiJet[aa] );
		}
		double viiiDiag2=(double)aa*10.0+80.0;
		if( massRecoDiBjetDiJet[aa] < ( scalarSumPtRecoDiBjetDiJet[aa]-viiiDiag2 ) ) {
			step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj80_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet[aa], massRecoDiBjetDiJet[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj80"]->Fill( massRecoDiBjetDiJet[aa] );
		}
		double ixDiag2=(double)aa*10.0+90.0;
		if( massRecoDiBjetDiJet[aa] < ( scalarSumPtRecoDiBjetDiJet[aa]-ixDiag2 ) ) {
			step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj90_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet[aa], massRecoDiBjetDiJet[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj90"]->Fill( massRecoDiBjetDiJet[aa] );
		}
		double xDiag2=(double)aa*10.0+100.0;
		if( massRecoDiBjetDiJet[aa] < ( scalarSumPtRecoDiBjetDiJet[aa]-xDiag2 ) ) {
			step3plots2D["recoDiBjetDiJet_cutDiagStop2bbjj100_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet[aa], massRecoDiBjetDiJet[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop2bbjj100"]->Fill( massRecoDiBjetDiJet[aa] );
		}
	}

	if ( p4RecoDiBjetDiJet.size() > 0 ) cutmap["Step3"] += nEvents;
	if ( dummyCounter > 0 ) {
		cutmap["Step3_diagStop2_0"] += nEvents;
		step3plots1D["avgMassHiggsCandidate_cutDiagStop2bbjj0"]->Fill( avgMassRecoBjetsCSVM );
		step3plots1D["avgMassHiggsCandidate_cutDiagStop2bbjj0_resoBasedBin"]->Fill( avgMassRecoBjetsCSVM );
		step3plots1D["massHiggsCandidate_cutDiagStop2bbjj0"]->Fill( massRecoBjetsCSVM[0] );
		step3plots1D["massHiggsCandidate_cutDiagStop2bbjj0"]->Fill( massRecoBjetsCSVM[1] );
		for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM.size(); ++iim) {
			step3plots1D["massStop1Candidate_cutDiagStop2bbjj0"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			step3plots1D["massStop1Candidate_cutDiagStop2bbjj0_resoBasedBin"]->Fill( massdijetWORecoBjetsCSVM[iim] );
		}
	}




	/*/ NO diagCutHiggs, diagCutStop1jj10
	//// Structure function s2Vectors
	s3Vectors vectorsStep3_cutDiagStop1jj10;
	vectorsStep3_cutDiagStop1jj10 = step3Vectors( p4dijetWORecoBjetsCSVM_cutDiagStop1jj10, p4RecoBjetsCSVM_SmallMassDiff, scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj10 );
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop1jj10 = vectorsStep3_cutDiagStop1jj10.s3TLV;
	vector< double > massRecoDiBjetDiJet_cutDiagStop1jj10 = vectorsStep3_cutDiagStop1jj10.s3Mass;
	vector< double > scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj10 = vectorsStep3_cutDiagStop1jj10.s3ScalarPt;

	for(unsigned int aa = 0; aa < p4RecoDiBjetDiJet_cutDiagStop1jj10.size(); ++aa) {
		step3plots2D["recoDiBjetDiJet_cutDiagStop1jj10_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj10[aa], massRecoDiBjetDiJet_cutDiagStop1jj10[aa] );
		step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj10"]->Fill( massRecoDiBjetDiJet_cutDiagStop1jj10[aa] );
	}

	// NO diagCutHiggs, diagCutStop1jj20
	//// Structure function s2Vectors
	s3Vectors vectorsStep3_cutDiagStop1jj20;
	vectorsStep3_cutDiagStop1jj20 = step3Vectors( p4dijetWORecoBjetsCSVM_cutDiagStop1jj20, p4RecoBjetsCSVM_SmallMassDiff, scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj20 );
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop1jj20 = vectorsStep3_cutDiagStop1jj20.s3TLV;
	vector< double > massRecoDiBjetDiJet_cutDiagStop1jj20 = vectorsStep3_cutDiagStop1jj20.s3Mass;
	vector< double > scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj20 = vectorsStep3_cutDiagStop1jj20.s3ScalarPt;

	for(unsigned int aa = 0; aa < p4RecoDiBjetDiJet_cutDiagStop1jj20.size(); ++aa) {
		step3plots2D["recoDiBjetDiJet_cutDiagStop1jj20_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj20[aa], massRecoDiBjetDiJet_cutDiagStop1jj20[aa] );
		step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj20"]->Fill( massRecoDiBjetDiJet_cutDiagStop1jj20[aa] );
	}

	// NO diagCutHiggs, diagCutStop1jj30
	//// Structure function s2Vectors
	s3Vectors vectorsStep3_cutDiagStop1jj30;
	vectorsStep3_cutDiagStop1jj30 = step3Vectors( p4dijetWORecoBjetsCSVM_cutDiagStop1jj30, p4RecoBjetsCSVM_SmallMassDiff, scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj30 );
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop1jj30 = vectorsStep3_cutDiagStop1jj30.s3TLV;
	vector< double > massRecoDiBjetDiJet_cutDiagStop1jj30 = vectorsStep3_cutDiagStop1jj30.s3Mass;
	vector< double > scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj30 = vectorsStep3_cutDiagStop1jj30.s3ScalarPt;

	for(unsigned int aa = 0; aa < p4RecoDiBjetDiJet_cutDiagStop1jj30.size(); ++aa) {
		step3plots2D["recoDiBjetDiJet_cutDiagStop1jj30_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj30[aa], massRecoDiBjetDiJet_cutDiagStop1jj30[aa] );
		step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj30"]->Fill( massRecoDiBjetDiJet_cutDiagStop1jj30[aa] );
	}

	// NO diagCutHiggs, diagCutStop1jj40
	//// Structure function s2Vectors
	s3Vectors vectorsStep3_cutDiagStop1jj40;
	vectorsStep3_cutDiagStop1jj40 = step3Vectors( p4dijetWORecoBjetsCSVM_cutDiagStop1jj40, p4RecoBjetsCSVM_SmallMassDiff, scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj40 );
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop1jj40 = vectorsStep3_cutDiagStop1jj40.s3TLV;
	vector< double > massRecoDiBjetDiJet_cutDiagStop1jj40 = vectorsStep3_cutDiagStop1jj40.s3Mass;
	vector< double > scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj40 = vectorsStep3_cutDiagStop1jj40.s3ScalarPt;

	for(unsigned int aa = 0; aa < p4RecoDiBjetDiJet_cutDiagStop1jj40.size(); ++aa) {
		step3plots2D["recoDiBjetDiJet_cutDiagStop1jj40_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj40[aa], massRecoDiBjetDiJet_cutDiagStop1jj40[aa] );
		step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj40"]->Fill( massRecoDiBjetDiJet_cutDiagStop1jj40[aa] );
	}

	// NO diagCutHiggs, diagCutStop1jj60
	//// Structure function s2Vectors
	s3Vectors vectorsStep3_cutDiagStop1jj60;
	vectorsStep3_cutDiagStop1jj60 = step3Vectors( p4dijetWORecoBjetsCSVM_cutDiagStop1jj60, p4RecoBjetsCSVM_SmallMassDiff, scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj60 );
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop1jj60 = vectorsStep3_cutDiagStop1jj60.s3TLV;
	vector< double > massRecoDiBjetDiJet_cutDiagStop1jj60 = vectorsStep3_cutDiagStop1jj60.s3Mass;
	vector< double > scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj60 = vectorsStep3_cutDiagStop1jj60.s3ScalarPt;

	for(unsigned int aa = 0; aa < p4RecoDiBjetDiJet_cutDiagStop1jj60.size(); ++aa) {
		step3plots2D["recoDiBjetDiJet_cutDiagStop1jj60_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj60[aa], massRecoDiBjetDiJet_cutDiagStop1jj60[aa] );
		step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj60"]->Fill( massRecoDiBjetDiJet_cutDiagStop1jj60[aa] );
	}

	// NO diagCutHiggs, diagCutStop1jj70
	//// Structure function s2Vectors
	s3Vectors vectorsStep3_cutDiagStop1jj70;
	vectorsStep3_cutDiagStop1jj70 = step3Vectors( p4dijetWORecoBjetsCSVM_cutDiagStop1jj70, p4RecoBjetsCSVM_SmallMassDiff, scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj70 );
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop1jj70 = vectorsStep3_cutDiagStop1jj70.s3TLV;
	vector< double > massRecoDiBjetDiJet_cutDiagStop1jj70 = vectorsStep3_cutDiagStop1jj70.s3Mass;
	vector< double > scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj70 = vectorsStep3_cutDiagStop1jj70.s3ScalarPt;

	for(unsigned int aa = 0; aa < p4RecoDiBjetDiJet_cutDiagStop1jj70.size(); ++aa) {
		step3plots2D["recoDiBjetDiJet_cutDiagStop1jj70_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj70[aa], massRecoDiBjetDiJet_cutDiagStop1jj70[aa] );
		step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj70"]->Fill( massRecoDiBjetDiJet_cutDiagStop1jj70[aa] );
	}

	// NO diagCutHiggs, diagCutStop1jj80
	//// Structure function s2Vectors
	s3Vectors vectorsStep3_cutDiagStop1jj80;
	vectorsStep3_cutDiagStop1jj80 = step3Vectors( p4dijetWORecoBjetsCSVM_cutDiagStop1jj80, p4RecoBjetsCSVM_SmallMassDiff, scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj80 );
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop1jj80 = vectorsStep3_cutDiagStop1jj80.s3TLV;
	vector< double > massRecoDiBjetDiJet_cutDiagStop1jj80 = vectorsStep3_cutDiagStop1jj80.s3Mass;
	vector< double > scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj80 = vectorsStep3_cutDiagStop1jj80.s3ScalarPt;

	for(unsigned int aa = 0; aa < p4RecoDiBjetDiJet_cutDiagStop1jj80.size(); ++aa) {
		step3plots2D["recoDiBjetDiJet_cutDiagStop1jj80_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj80[aa], massRecoDiBjetDiJet_cutDiagStop1jj80[aa] );
		step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj80"]->Fill( massRecoDiBjetDiJet_cutDiagStop1jj80[aa] );
	}

	// NO diagCutHiggs, diagCutStop1jj90
	//// Structure function s2Vectors
	s3Vectors vectorsStep3_cutDiagStop1jj90;
	vectorsStep3_cutDiagStop1jj90 = step3Vectors( p4dijetWORecoBjetsCSVM_cutDiagStop1jj90, p4RecoBjetsCSVM_SmallMassDiff, scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj90 );
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop1jj90 = vectorsStep3_cutDiagStop1jj90.s3TLV;
	vector< double > massRecoDiBjetDiJet_cutDiagStop1jj90 = vectorsStep3_cutDiagStop1jj90.s3Mass;
	vector< double > scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj90 = vectorsStep3_cutDiagStop1jj90.s3ScalarPt;

	for(unsigned int aa = 0; aa < p4RecoDiBjetDiJet_cutDiagStop1jj90.size(); ++aa) {
		step3plots2D["recoDiBjetDiJet_cutDiagStop1jj90_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj90[aa], massRecoDiBjetDiJet_cutDiagStop1jj90[aa] );
		step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj90"]->Fill( massRecoDiBjetDiJet_cutDiagStop1jj90[aa] );
	}

	// NO diagCutHiggs, diagCutStop1jj50
	//// Structure function s2Vectors
	s3Vectors vectorsStep3_cutDiagStop1jj50;
	vectorsStep3_cutDiagStop1jj50 = step3Vectors( p4dijetWORecoBjetsCSVM_cutDiagStop1jj50, p4RecoBjetsCSVM_SmallMassDiff, scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj50 );
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop1jj50 = vectorsStep3_cutDiagStop1jj50.s3TLV;
	vector< double > massRecoDiBjetDiJet_cutDiagStop1jj50 = vectorsStep3_cutDiagStop1jj50.s3Mass;
	vector< double > scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj50 = vectorsStep3_cutDiagStop1jj50.s3ScalarPt;

	//// TLorentzVectors for cuts
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50;
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100;
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj150;

	for(unsigned int aa = 0; aa < p4RecoDiBjetDiJet_cutDiagStop1jj50.size(); ++aa) {
		step3plots2D["recoDiBjetDiJet_cutDiagStop1jj50_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj50[aa], massRecoDiBjetDiJet_cutDiagStop1jj50[aa] );
		step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj50"]->Fill( massRecoDiBjetDiJet_cutDiagStop1jj50[aa] );

		double iDiag=(double)aa*10.0+50.0;
		if( massRecoDiBjetDiJet_cutDiagStop1jj50[aa] < ( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj50[aa]-iDiag ) ) {
			step3plots2D["recoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj50[aa], massRecoDiBjetDiJet_cutDiagStop1jj50[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj50"]->Fill( massRecoDiBjetDiJet_cutDiagStop1jj50[aa] );
		}

		double iiDiag=(double)aa*10.0+100.0;
		if( massRecoDiBjetDiJet_cutDiagStop1jj50[aa] < ( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj50[aa]-iiDiag ) ) {
			step3plots2D["recoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj50[aa], massRecoDiBjetDiJet_cutDiagStop1jj50[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj50_cutDiagStop2bbjj100"]->Fill( massRecoDiBjetDiJet_cutDiagStop1jj50[aa] );
		}
	}

	if ( p4RecoDiBjetDiJet_cutDiagStop1jj50.size() > 0 ) cutmap["Step3_diagStop150"] += nEvents;

	// NO diagCutHiggs, diagCutStop1jj100
	//// Structure function s2Vectors
	s3Vectors vectorsStep3_cutDiagStop1jj100;
	vectorsStep3_cutDiagStop1jj100 = step3Vectors( p4dijetWORecoBjetsCSVM_cutDiagStop1jj100, p4RecoBjetsCSVM_SmallMassDiff, scalarSumPtdijetWORecoBjetsCSVM_cutDiagStop1jj100 );
	vector< TLorentzVector > p4RecoDiBjetDiJet_cutDiagStop1jj100 = vectorsStep3_cutDiagStop1jj100.s3TLV;
	vector< double > massRecoDiBjetDiJet_cutDiagStop1jj100 = vectorsStep3_cutDiagStop1jj100.s3Mass;
	vector< double > scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj100 = vectorsStep3_cutDiagStop1jj100.s3ScalarPt;

	for(unsigned int aa = 0; aa < p4RecoDiBjetDiJet_cutDiagStop1jj100.size(); ++aa) {
		step3plots2D["recoDiBjetDiJet_cutDiagStop1jj100_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj100[aa], massRecoDiBjetDiJet_cutDiagStop1jj100[aa] );
		step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj100"]->Fill( massRecoDiBjetDiJet_cutDiagStop1jj100[aa] );

		double iDiag=(double)aa*10.0+50.0;
		if( massRecoDiBjetDiJet_cutDiagStop1jj100[aa] < ( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj100[aa]-iDiag ) ) {
			step3plots2D["recoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj100[aa], massRecoDiBjetDiJet_cutDiagStop1jj100[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj50"]->Fill( massRecoDiBjetDiJet_cutDiagStop1jj100[aa] );
		}

		double iiDiag=(double)aa*10.0+100.0;
		if( massRecoDiBjetDiJet_cutDiagStop1jj100[aa] < ( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj100[aa]-iiDiag ) ) {
			step3plots2D["recoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100_masspt"]->Fill( scalarSumPtRecoDiBjetDiJet_cutDiagStop1jj100[aa], massRecoDiBjetDiJet_cutDiagStop1jj100[aa] );
			step3plots1D["massRecoDiBjetDiJet_cutDiagStop1jj100_cutDiagStop2bbjj100"]->Fill( massRecoDiBjetDiJet_cutDiagStop1jj100[aa] );
		}
	}

	if ( p4RecoDiBjetDiJet_cutDiagStop1jj100.size() > 0 ) cutmap["Step3_diagStop1100"] += nEvents;*/

	//cout << " 5 " << endl;
	////////////////////////////////////////////////////////////////
	//////           STEP 4                                    /////
	//////  Smallest mass MatchBjetsdijet                                 /////
	////////////////////////////////////////////////////////////////
	
	/////// Reco Jets
	//
	// NO diagCutHiggs neither diagCutStop1
	double tmpMassDiffStop2 = 9999999;
	Int_t bestMassIndexStop2[2] = {-1, -1};
	if ( p4dijetWORecoBjetsCSVM.size() > 1 ){
		for(unsigned int iiii = 0; iiii < p4dijetWORecoBjetsCSVM.size(); ++iiii) {
			for(unsigned int iiij = 0; iiij < p4dijetWORecoBjetsCSVM.size(); ++iiij) {
				TLorentzVector cand1Stop2 = p4dijetWORecoBjetsCSVM[iiii] + p4RecoBjetsCSVM_SmallMassDiff[0] + p4RecoBjetsCSVM_SmallMassDiff[1];
				TLorentzVector cand2Stop2 = p4dijetWORecoBjetsCSVM[iiij] + p4RecoBjetsCSVM_SmallMassDiff[2] + p4RecoBjetsCSVM_SmallMassDiff[3];
				double massDiff = abs( cand1Stop2.M() - cand2Stop2.M() );
				if( tmpMassDiffStop2 > massDiff ){
					tmpMassDiffStop2 = massDiff;
					if ( iiii != iiij ) continue;
					bestMassIndexStop2[0] = iiii;
					bestMassIndexStop2[1] = iiij;
				}
			}
		}
		TLorentzVector tmpCand1Stop2 = p4dijetWORecoBjetsCSVM[bestMassIndexStop2[0]] + p4RecoBjetsCSVM_SmallMassDiff[0] + p4RecoBjetsCSVM_SmallMassDiff[1];
		TLorentzVector tmpCand2Stop2 = p4dijetWORecoBjetsCSVM[bestMassIndexStop2[1]] + p4RecoBjetsCSVM_SmallMassDiff[2] + p4RecoBjetsCSVM_SmallMassDiff[3];
			
		step4plots2D["recoDiBjetDiJetSmallestDeltaM_masspt"]->Fill( tmpCand1Stop2.Pt(), tmpCand1Stop2.M() );
		step4plots2D["recoDiBjetDiJetSmallestDeltaM_masspt"]->Fill( tmpCand2Stop2.Pt(), tmpCand2Stop2.M() );
		step4plots1D["massRecoDiBjetDiJetSmallestDeltaM"]->Fill( tmpCand1Stop2.M() );
		step4plots1D["massRecoDiBjetDiJetSmallestDeltaM"]->Fill( tmpCand2Stop2.M() );
	}
	///////////////////////////*/

   return kTRUE;
}

void MyTreeAnalyzer::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
	
	int ibin = 1;
	for ( vector<string>::const_iterator ivec= fCutLabels.begin(); ivec != fCutLabels.end(); ++ivec ) {
		hcutflow->SetBinContent( ibin, cutmap[ *ivec ] );
		ibin++;
	}

	if (fFile) {
		TDirectory *savedir = gDirectory;
		if(h1test->GetEntries() > 0){
			fFile->cd();
			h1test->Write();
			hcutflow->Write();
			fFile->mkdir("basicPlots");
			fFile->cd("basicPlots");
			for ( std::map<string,TH1* >::const_iterator imap=basicPlots.begin(); imap!=basicPlots.end(); ++imap ) {
				TH1 *temp = imap->second;
				if ( temp->GetEntries() > 0 ) temp->Write();
				//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
			}
			fFile->cd();
			fFile->mkdir("scaleBasicPlots");
			fFile->cd("scaleBasicPlots");
			for ( std::map<string,TH1* >::const_iterator imap=basicPlots.begin(); imap!=basicPlots.end(); ++imap ) {
				TH1 *temp = imap->second;
				temp->Scale( weight );
				if ( temp->GetEntries() > 0 ) temp->Write();
				//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
			}
			fFile->cd();
			fFile->mkdir("step1plots1D");
			fFile->cd("step1plots1D");
			for ( std::map<string,TH1* >::const_iterator imap=step1plots1D.begin(); imap!=step1plots1D.end(); ++imap ) {
				TH1 *temp = imap->second;
				if ( temp->GetEntries() > 0 ) temp->Write();
				//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
			}
			fFile->cd();
			fFile->mkdir("scaleStep1plots1D");
			fFile->cd("scaleStep1plots1D");
			for ( std::map<string,TH1* >::const_iterator imap=step1plots1D.begin(); imap!=step1plots1D.end(); ++imap ) {
				TH1 *temp = imap->second;
				temp->Scale( weight );
				if ( temp->GetEntries() > 0 ) temp->Write();
				//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
			}
			fFile->cd();
			fFile->mkdir("step1plots2D");
			fFile->cd("step1plots2D");
			for ( std::map<string,TH2* >::const_iterator imap=step1plots2D.begin(); imap!=step1plots2D.end(); ++imap ) {
				TH1 *temp = imap->second;
				if ( temp->GetEntries() > 0 ) temp->Write();
				//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
			}
			fFile->cd();
			fFile->mkdir("step2plots1D");
			fFile->cd("step2plots1D");
			for ( std::map<string,TH1* >::const_iterator imap=step2plots1D.begin(); imap!=step2plots1D.end(); ++imap ) {
				TH1 *temp = imap->second;
				if ( temp->GetEntries() > 0 ) temp->Write();
				//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
			}
			fFile->cd();
			fFile->mkdir("scaleStep2plots1D");
			fFile->cd("scaleStep2plots1D");
			for ( std::map<string,TH1* >::const_iterator imap=step2plots1D.begin(); imap!=step2plots1D.end(); ++imap ) {
				TH1 *temp = imap->second;
				temp->Scale( weight );
				if ( temp->GetEntries() > 0 ) temp->Write();
				//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
			}
			fFile->cd();
			fFile->mkdir("step2plots2D");
			fFile->cd("step2plots2D");
			for ( std::map<string,TH2* >::const_iterator imap=step2plots2D.begin(); imap!=step2plots2D.end(); ++imap ) {
				TH1 *temp = imap->second;
				if ( temp->GetEntries() > 0 ) temp->Write();
				//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
			}
			fFile->cd();
			fFile->mkdir("step3plots1D");
			fFile->cd("step3plots1D");
			for ( std::map<string,TH1* >::const_iterator imap=step3plots1D.begin(); imap!=step3plots1D.end(); ++imap ) {
				TH1 *temp = imap->second;
				if ( temp->GetEntries() > 0 ) temp->Write();
				//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
			}
			fFile->cd();
			fFile->mkdir("scaleStep3plots1D");
			fFile->cd("scaleStep3plots1D");
			for ( std::map<string,TH1* >::const_iterator imap=step3plots1D.begin(); imap!=step3plots1D.end(); ++imap ) {
				TH1 *temp = imap->second;
				temp->Scale( weight );
				if ( temp->GetEntries() > 0 ) temp->Write();
				//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
			}
			fFile->cd();
			fFile->mkdir("step3plots2D");
			fFile->cd("step3plots2D");
			for ( std::map<string,TH2* >::const_iterator imap=step3plots2D.begin(); imap!=step3plots2D.end(); ++imap ) {
				TH1 *temp = imap->second;
				if ( temp->GetEntries() > 0 ) temp->Write();
				//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
			}
			fFile->cd();
			fFile->mkdir("step4plots1D");
			fFile->cd("step4plots1D");
			for ( std::map<string,TH1* >::const_iterator imap=step4plots1D.begin(); imap!=step4plots1D.end(); ++imap ) {
				TH1 *temp = imap->second;
				if ( temp->GetEntries() > 0 ) temp->Write();
				//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
			}
			fFile->cd();
			fFile->mkdir("scaleStep4plots1D");
			fFile->cd("scaleStep4plots1D");
			for ( std::map<string,TH1* >::const_iterator imap=step4plots1D.begin(); imap!=step4plots1D.end(); ++imap ) {
				TH1 *temp = imap->second;
				temp->Scale( weight );
				if ( temp->GetEntries() > 0 ) temp->Write();
				//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
			}
			fFile->cd();
			fFile->mkdir("step4plots2D");
			fFile->cd("step4plots2D");
			for ( std::map<string,TH2* >::const_iterator imap=step4plots2D.begin(); imap!=step4plots2D.end(); ++imap ) {
				TH1 *temp = imap->second;
				if ( temp->GetEntries() > 0 ) temp->Write();
				//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
			}
			fFile->cd();

		}


		h1test->SetDirectory(0);
		hcutflow->SetDirectory(0);
		//MyStoreTree->GetStoreTree()->SetDirectory(0);
		gDirectory = savedir;
		fFile->Close();
	}
}

void MyTreeAnalyzer::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

	  Info("Terminate","Analyzer done.");
}
