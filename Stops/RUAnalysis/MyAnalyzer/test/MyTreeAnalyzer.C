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
		if (!fMyOpt.Contains("data") || !fMyOpt.Contains("HT")) tmp = tmp.Remove(23,8);
		//cout << tmp << endl;
		fSample = tmp;
		//if (fdoJECunc && fdoJECup==true) fSample += "_JECUP";
		//if (fdoJECunc && fdoJECup==false) fSample += "_JECDOWN";
		//if (fMyOpt.Contains("PUUP")) fSample += "_PUUP";
		//if (fMyOpt.Contains("PUDOWN")) fSample += "_PUDOWN";
	}

	if (fMyOpt.Contains("st1")){
		
		TString tmp = fMyOpt;
		tmp = tmp.Remove(0,fMyOpt.Index("sample="+fSample+"st1")+36);
		fSt1 = tmp.Atoi();
	}
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

///////////////////////////////////////////////////
/////////// Function Compare two TLorentzVectors
std::vector< TLorentzVector > compare2VectorsTLorentzVectors( std::vector< TLorentzVector > vec1, std::vector< TLorentzVector > vec2 ){

	std::vector< TLorentzVector > diffVec;

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
s3VecMassSumPt s3Vectors( std::vector< TLorentzVector > p4Store ){

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
	s3VecMassSumPt tmpVectors;
	tmpVectors.s3VectorTL = p4Vec;
	tmpVectors.s3Mass = massVec;
	tmpVectors.s3ScalarPt = scalarSumPtVec;


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

	TString option = GetOption();
	fMyOpt = option;
	ParseInput();
		
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
	//if ( fSample != "" ) tmpfilename = fSample+"_4jet80_6jet60_plots.root";
	if ( fSample != "" ) tmpfilename = fSample+"_4jet80_plots.root";
	else tmpfilename = "results.root";
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

	/////////////////////////////// Basic Plots
	h1test = new TH1F( "h1test", "jet p_{T}", nbinPt, minPt, maxPt );
	basicPlots["recoJets_num"] = new TH1D("recoJets_num" , "Number of Reco Jets", nbinNum , minNum, maxNum );
	basicPlots["recoJets_pt"] = new TH1D("recoJets_pt" , "Reco Jets p_{T}", nbinPt , minPt, maxPt );
	basicPlots["recoJets_sumpt"] = new TH1D("recoJets_sumpt" , "#sum Reco Jets p_{T}", nbinPt , minPt, maxPt );
	basicPlots["recoJets_eta"] = new TH1D("recoJets_eta" , "Reco Jets #eta", nbinEta , minEta, maxEta );
	basicPlots["recoJets_phi"] = new TH1D("recoJets_phi" , "Reco Jets #phi", nbinEta , minEta, maxEta );
	basicPlots["recoBjets_num"] = new TH1D("recoBjets_num" , "Number of Reco Bjets", nbinNum , minNum, maxNum );
	basicPlots["recoBjets_pt"] = new TH1D("recoBjets_pt" , "Reco Bjets p_{T}", nbinPt , minPt, maxPt );
	basicPlots["recoBjets_sumpt"] = new TH1D("recoBjets_sumpt" , "#sum Reco Bjets p_{T}", nbinPt , minPt, maxPt );
	basicPlots["recoBjets_eta"] = new TH1D("recoBjets_eta" , "Reco Bjets #eta", nbinEta , minEta, maxEta );
	basicPlots["recoBjets_phi"] = new TH1D("recoBjets_phi" , "Reco Bjets #phi", nbinEta , minEta, maxEta );

	basicPlots["recoJets_num_Step1"] = new TH1D("recoJets_num_Step1" , "Number of Reco Jets after Step1", nbinNum , minNum, maxNum );
	basicPlots["recoBjets_num_Step1"] = new TH1D("recoBjets_num_Step1" , "Number of Reco Bjets after Step1", nbinNum , minNum, maxNum );
	basicPlots["recoJets_num_Step3"] = new TH1D("recoJets_num_Step3" , "Number of Reco Jets after Step3", nbinNum , minNum, maxNum );
	basicPlots["recoBjets_num_Step3"] = new TH1D("recoBjets_num_Step3" , "Number of Reco Bjets after Step3", nbinNum , minNum, maxNum );
	basicPlots["recoJets_num_Step3_diag50"] = new TH1D("recoJets_num_Step3_diag50" , "Number of Reco Jets after Step3_diag50", nbinNum , minNum, maxNum );
	basicPlots["recoBjets_num_Step3_diag50"] = new TH1D("recoBjets_num_Step3_diag50" , "Number of Reco Bjets after Step3_diag50", nbinNum , minNum, maxNum );
	basicPlots["recoJets_num_Step3_diag100"] = new TH1D("recoJets_num_Step3_diag100" , "Number of Reco Jets after Step3_diag100", nbinNum , minNum, maxNum );
	basicPlots["recoBjets_num_Step3_diag100"] = new TH1D("recoBjets_num_Step3_diag100" , "Number of Reco Bjets after Step3_diag100", nbinNum , minNum, maxNum );
	basicPlots["recoJets_num_Step3_diag150"] = new TH1D("recoJets_num_Step3_diag150" , "Number of Reco Jets after Step3_diag150", nbinNum , minNum, maxNum );
	basicPlots["recoBjets_num_Step3_diag150"] = new TH1D("recoBjets_num_Step3_diag150" , "Number of Reco Bjets after Step3_diag150", nbinNum , minNum, maxNum );
	basicPlots["recoJets_num_Step3_diag200"] = new TH1D("recoJets_num_Step3_diag200" , "Number of Reco Jets after Step3_diag200", nbinNum , minNum, maxNum );
	basicPlots["recoBjets_num_Step3_diag200"] = new TH1D("recoBjets_num_Step3_diag200" , "Number of Reco Bjets after Step3_diag200", nbinNum , minNum, maxNum );

	//////////////////////////////// Step1 Plots 1D
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


	// Step3 Plots 1D
	// Reco Bjets
	step3plots1D["massdijetWORecoBjetsCSVM"] = new TH1D("massdijetWORecoBjetsCSVM" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_WOStop1"] = new TH1D("massdijetWORecoBjetsCSVM_WOStop1" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj50"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj50_WOStop1"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj50_WOStop1" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj100"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj100_WOStop1"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj100_WOStop1" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj150"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj150_WOStop1"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj150_WOStop1" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj200"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200"] = new TH1D("massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	// Reco Bjets + Parton Flavor
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj50"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj100"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj150"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj200"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );
	step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200"] = new TH1D("massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of dijet w/o RecoBjetsCSVM" , nbinPt, minPt, maxPt );

	////// Step 3 plots 2D
	//  Reco Bjets
	step3plots2D["dijetWORecoBjetsCSVM_masspt"] = new TH2D("dijetWORecoBjetsCSVM_masspt" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj50"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj100"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj150"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj200"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200"] = new TH2D("dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	// Reco jets + Parton Flavor
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj50"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj100"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj150"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj200"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );

	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );
	step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200"] = new TH2D("dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200" , "Mass of Match dijet w/o RecoBjetsCSVM vs scalar #sum p_{T}^{dijet}" , nbinPt, minPt, maxPt, nbinPt, minPt, maxPt );


	map<string,TH1* > allhistos1D = basicPlots;
	allhistos1D.insert( step1plots1D.begin(), step1plots1D.end() );
	allhistos1D.insert( step3plots1D.begin(), step3plots1D.end() );
	map<string,TH2* > allhistos2D = step1plots2D;
	allhistos2D.insert( step2plots2D.begin(), step2plots2D.end() );
	allhistos2D.insert( step3plots2D.begin(), step3plots2D.end() );

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
	fCutLabels.push_back("Step3");
	fCutLabels.push_back("Step3_diag50");
	fCutLabels.push_back("Step3_diag100");
	fCutLabels.push_back("Step3_diag150");
	fCutLabels.push_back("Step3_diag200");

	hcutflow = new TH1D("cutflow","cut flow", fCutLabels.size(), 0.5, fCutLabels.size() +0.5 );

	for ( vector<string>::const_iterator ivec= fCutLabels.begin(); ivec!=fCutLabels.end(); ++ivec) {
		cutmap[ *ivec ] = 0;
	}

}

Bool_t MyTreeAnalyzer::Process(Long64_t entry)
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

	int st1mass;
	if ( fSt1 != 0 ) st1mass = fSt1;

	//..Just read the full event
	fChain->GetTree()->GetEntry(entry);

	if ( entry % 2000 == 0 ) cout<< "process entry " << entry << endl;

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
		if ( jet_PF_pt[i] < 20.0 || fabs( jet_PF_eta[i] ) > 3.0) continue;
		dummyCounter1.push_back( jet_PF_pt[i] );

		//if ( nPFJets > 5 ){
			//if ( jet_PF_pt[3] < 80.0 || jet_PF_pt[5] < 60.0 ) continue;	/// 6jet Trigger
			if ( jet_PF_pt[3] < 80 ) continue;				/// dijet trigger

			TLorentzVector p4Jets;
			p4Jets.SetPxPyPzE( jet_PF_px[i], jet_PF_py[i], jet_PF_pz[i], jet_PF_e[i] );
			p4RecoJets.push_back( p4Jets );

			//bool isTagged = false;
			//CSVL > 0.244, CSVM > 0.679, CSVT > 0.898, JPM > 
			if (bdiscCSV_PF[i] > 0.679 ) p4RecoBjetsCSVM.push_back( p4Jets );
			/*if ( abs( jet_PF_PartonFlav[i] ) = 5 ) continue;
			if ( bdiscCSV_PF < 0.679  ) continue;
			p4RecoPartonFlavorBjetsCSVM.push_back( p4Jets );
			RecoBjetsCSVM.push_back( bdiscCSV_PF );*/

		//}
	}

	if ( dummyCounter1.size() > 0 ) cutmap["Simple"] += nEvents;
	if ( p4RecoJets.size() > 0 ) cutmap["4jetTrigger"] += nEvents;

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
	basicPlots["recoJets_sumpt"]->Fill( sumRecoJetsPt );	
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

	///////////////////////////////////////
	/////////// STEP 2 - 3 ////////////////
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

		cutmap["Step3"] += nEvents;
		basicPlots["recoJets_num_Step3"]->Fill( p4RecoJets.size() );	
		basicPlots["recoBjets_num_Step3"]->Fill( p4RecoBjetsCSVM.size() );	

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
			
			////// Step 3 - dijet mass vs sum pt of jets without bs from step 1
			//p4jetsWORecoJetsCSVM = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoBjetsCSVM );
			//if ( p4RecoBjetsCSVM_SmallMassDiff.size() == 4 ) p4bWORecoBjetsCSVM = compare2VectorsTLorentzVectors( p4RecoBjetsCSVM, p4RecoBjetsCSVM_SmallMassDiff );
			if ( p4RecoBjetsCSVM_SmallMassDiff.size() == 4 ) p4jetsWORecoBjetsCSVM = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoBjetsCSVM_SmallMassDiff );
			/*if ( p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb50.size() == 4 ) p4jetsWORecoBjetsCSVM_cutDiagHiggsbb50 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb50 );
			if ( p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb100.size() == 4 ) p4jetsWORecoBjetsCSVM_cutDiagHiggsbb100 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb100 );
			if ( p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb150.size() == 4 ) p4jetsWORecoBjetsCSVM_cutDiagHiggsbb150 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb150 );
			if ( p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb200.size() == 4 ) p4jetsWORecoBjetsCSVM_cutDiagHiggsbb200 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoBjetsCSVM_SmallMassDiff_cutDiagHiggsbb200 ); */
		}
	}
			
	/////////////////////// STEP 3 - NO cutDiagHiggsbb 
	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWORecoBjetsCSVM;
	vectorsjWORecoBjetsCSVM = s3Vectors( p4jetsWORecoBjetsCSVM );
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM = vectorsjWORecoBjetsCSVM.s3VectorTL;
	std::vector< double > massdijetWORecoBjetsCSVM = vectorsjWORecoBjetsCSVM.s3Mass;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM = vectorsjWORecoBjetsCSVM.s3ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM.size(); ++iim) {
		step3plots2D["dijetWORecoBjetsCSVM_masspt"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
		step3plots1D["massdijetWORecoBjetsCSVM"]->Fill( massdijetWORecoBjetsCSVM[iim] );
		if ( massdijetWORecoBjetsCSVM[iim] < minStop1 ||  massdijetWORecoBjetsCSVM[iim] > maxStop1) {
			step3plots1D["massdijetWORecoBjetsCSVM_WOStop1"]->Fill( massdijetWORecoBjetsCSVM[iim] );
		}

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-iDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj50"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4jetWORecoBjetsCSVM_cutDiagStop1jj50 = p4dijetWORecoBjetsCSVM;
			if ( massdijetWORecoBjetsCSVM[iim] < minStop1 ||  massdijetWORecoBjetsCSVM[iim] > maxStop1) {
				step3plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj50_WOStop1"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			}
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-iiDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj100"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4jetWORecoBjetsCSVM_cutDiagStop1jj100 = p4dijetWORecoBjetsCSVM;
			if ( massdijetWORecoBjetsCSVM[iim] < minStop1 ||  massdijetWORecoBjetsCSVM[iim] > maxStop1) {
				step3plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj100_WOStop1"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			}
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-iiiDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj150"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4jetWORecoBjetsCSVM_cutDiagStop1jj150 = p4dijetWORecoBjetsCSVM;
			if ( massdijetWORecoBjetsCSVM[iim] < minStop1 ||  massdijetWORecoBjetsCSVM[iim] > maxStop1) {
				step3plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj150_WOStop1"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			}
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoBjetsCSVM[iim] < ( scalarSumPtdijetWORecoBjetsCSVM[iim]-ivDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoBjetsCSVM[iim], massdijetWORecoBjetsCSVM[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagStop1jj200"]->Fill( massdijetWORecoBjetsCSVM[iim] );
			p4jetWORecoBjetsCSVM_cutDiagStop1jj200 = p4dijetWORecoBjetsCSVM;
		}

	}

	//////// Check number of bjets and events
	if ( p4jetWORecoBjetsCSVM_cutDiagStop1jj50.size() > 0 ){
	       cutmap["Step3_diag50"] += nEvents;
	       basicPlots["recoJets_num_Step3_diag50"]->Fill( p4RecoJets.size() );	
	       basicPlots["recoBjets_num_Step3_diag50"]->Fill( p4RecoBjetsCSVM.size() );	
	}
	if ( p4jetWORecoBjetsCSVM_cutDiagStop1jj100.size() > 0 ){
	       cutmap["Step3_diag100"] += nEvents;
	       basicPlots["recoJets_num_Step3_diag100"]->Fill( p4RecoJets.size() );	
	       basicPlots["recoBjets_num_Step3_diag100"]->Fill( p4RecoBjetsCSVM.size() );	
	}
	if ( p4jetWORecoBjetsCSVM_cutDiagStop1jj150.size() > 0 ){
	       cutmap["Step3_diag150"] += nEvents;
	       basicPlots["recoJets_num_Step3_diag150"]->Fill( p4RecoJets.size() );	
	       basicPlots["recoBjets_num_Step3_diag150"]->Fill( p4RecoBjetsCSVM.size() );	
	}
	if ( p4jetWORecoBjetsCSVM_cutDiagStop1jj200.size() > 0 ){
	       cutmap["Step3_diag200"] += nEvents;
	       basicPlots["recoJets_num_Step3_diag200"]->Fill( p4RecoJets.size() );	
	       basicPlots["recoBjets_num_Step3_diag200"]->Fill( p4RecoBjetsCSVM.size() );	
	}




	/*////////////////////// STEP 3 - cutDiagHiggsbb50 
	s3VecMassSumPt vectorsjWORecoBjetsCSVM_cutDiagHiggsbb50;
	vectorsjWORecoBjetsCSVM_cutDiagHiggsbb50 = s3Vectors( p4jetsWORecoBjetsCSVM_cutDiagHiggsbb50 );
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb50.s3VectorTL;
	std::vector< double > massdijetWORecoBjetsCSVM_cutDiagHiggsbb50 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb50.s3Mass;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb50.s3ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM_cutDiagHiggsbb50.size(); ++iim) {
		step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
		step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim]-iDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim]-iiDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim]-iiiDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim]-ivDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb50;
		}

	}

	/////////////////////// STEP 3 - cutDiagHiggsbb100 
	s3VecMassSumPt vectorsjWORecoBjetsCSVM_cutDiagHiggsbb100;
	vectorsjWORecoBjetsCSVM_cutDiagHiggsbb100 = s3Vectors( p4jetsWORecoBjetsCSVM_cutDiagHiggsbb100 );
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb100.s3VectorTL;
	std::vector< double > massdijetWORecoBjetsCSVM_cutDiagHiggsbb100 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb100.s3Mass;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb100.s3ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM_cutDiagHiggsbb100.size(); ++iim) {
		step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
		step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim]-iDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim]-iiDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim]-iiiDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim]-ivDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb100;
		}

	}

	/////////////////////// STEP 3 - cutDiagHiggsbb150 
	s3VecMassSumPt vectorsjWORecoBjetsCSVM_cutDiagHiggsbb150;
	vectorsjWORecoBjetsCSVM_cutDiagHiggsbb150 = s3Vectors( p4jetsWORecoBjetsCSVM_cutDiagHiggsbb150 );
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb150.s3VectorTL;
	std::vector< double > massdijetWORecoBjetsCSVM_cutDiagHiggsbb150 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb150.s3Mass;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb150.s3ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM_cutDiagHiggsbb150.size(); ++iim) {
		step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
		step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim]-iDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim]-iiDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim]-iiiDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim]-ivDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb150;
		}
	}

	/////////////////////// STEP 3 - cutDiagHiggsbb200 
	s3VecMassSumPt vectorsjWORecoBjetsCSVM_cutDiagHiggsbb200;
	vectorsjWORecoBjetsCSVM_cutDiagHiggsbb200 = s3Vectors( p4jetsWORecoBjetsCSVM_cutDiagHiggsbb200 );
	std::vector< TLorentzVector > p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb200.s3VectorTL;
	std::vector< double > massdijetWORecoBjetsCSVM_cutDiagHiggsbb200 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb200.s3Mass;
	std::vector< double > scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200 = vectorsjWORecoBjetsCSVM_cutDiagHiggsbb200.s3ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4jetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoBjetsCSVM_cutDiagHiggsbb200.size(); ++iim) {
		step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
		step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim]-iDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim]-iiDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim]-iiiDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim]-ivDiag ) ) {
			step3plots2D["dijetWORecoBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			step3plots1D["massdijetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200"]->Fill( massdijetWORecoBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4jetWORecoBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200 = p4dijetWORecoBjetsCSVM_cutDiagHiggsbb200;
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
			
			////// Step 3 - dijet mass vs sum pt of jets without bs from step 1
			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff.size() == 4 ) p4jetsWORecoPartonFlavorBjetsCSVM = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoPartonFlavorBjetsCSVM_SmallMassDiff );
			/*if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb50.size() == 4 ) p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb50 );
			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb100.size() == 4 ) p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb100 );
			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb150.size() == 4 ) p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb150 );
			if ( p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb200.size() == 4 ) p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 = compare2VectorsTLorentzVectors( p4RecoJets, p4RecoPartonFlavorBjetsCSVM_SmallMassDiff_cutDiagHiggsbb200 ); */
		}
	}

	/////////////////////// STEP 3 - NO cutDiagHiggsbb 
	//// Structure function s3Vectors
	s3VecMassSumPt vectorsjWORecoPartonFlavorBjetsCSVM;
	vectorsjWORecoPartonFlavorBjetsCSVM = s3Vectors( p4jetsWORecoPartonFlavorBjetsCSVM );
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM = vectorsjWORecoPartonFlavorBjetsCSVM.s3VectorTL;
	std::vector< double > massdijetWORecoPartonFlavorBjetsCSVM = vectorsjWORecoPartonFlavorBjetsCSVM.s3Mass;
	std::vector< double > scalarSumPtdijetWORecoPartonFlavorBjetsCSVM = vectorsjWORecoPartonFlavorBjetsCSVM.s3ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoPartonFlavorBjetsCSVM.size(); ++iim) {
		step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim], massdijetWORecoPartonFlavorBjetsCSVM[iim] );
		step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim]-iDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim], massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj50"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj50 = p4dijetWORecoPartonFlavorBjetsCSVM;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim]-iiDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim], massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj100"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj100 = p4dijetWORecoPartonFlavorBjetsCSVM;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim]-iiiDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim], massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj150"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj150 = p4dijetWORecoPartonFlavorBjetsCSVM;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim]-ivDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM[iim], massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj200"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagStop1jj200 = p4dijetWORecoPartonFlavorBjetsCSVM;
		}

	}

	/*////////////////////// STEP 3 - cutDiagHiggsbb50 
	s3VecMassSumPt vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
	vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 = s3Vectors( p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 );
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50.s3VectorTL;
	std::vector< double > massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50.s3Mass;
	std::vector< double > scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50.s3ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50.size(); ++iim) {
		step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
		step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim]-iDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj50 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim]-iiDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj100 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim]-iiiDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj150 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim]-ivDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb50_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50_cutDiagStop1jj200 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb50;
		}

	}

	/////////////////////// STEP 3 - cutDiagHiggsbb100 
	s3VecMassSumPt vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
	vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 = s3Vectors( p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 );
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100.s3VectorTL;
	std::vector< double > massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100.s3Mass;
	std::vector< double > scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100.s3ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100.size(); ++iim) {
		step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
		step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim]-iDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj50 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim]-iiDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj100 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim]-iiiDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj150 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim]-ivDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb100_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100_cutDiagStop1jj200 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb100;
		}

	}

	/////////////////////// STEP 3 - cutDiagHiggsbb150 
	s3VecMassSumPt vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
	vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 = s3Vectors( p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 );
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150.s3VectorTL;
	std::vector< double > massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150.s3Mass;
	std::vector< double > scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150.s3ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150.size(); ++iim) {
		step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
		step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim]-iDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj50 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim]-iiDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj100 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim]-iiiDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj150 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim]-ivDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb150_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150_cutDiagStop1jj200 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb150;
		}
	}

	/////////////////////// STEP 3 - cutDiagHiggsbb200 
	s3VecMassSumPt vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;
	vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 = s3Vectors( p4jetsWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 );
	std::vector< TLorentzVector > p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200.s3VectorTL;
	std::vector< double > massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200.s3Mass;
	std::vector< double > scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200 = vectorsjWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200.s3ScalarPt;

	//// TLorentzVectors for cuts
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50;
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100;
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150;
	std::vector< TLorentzVector > p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200;

	for(unsigned int iim = 0; iim < massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200.size(); ++iim) {
		step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
		step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );

		///// Diagonal cuts
		double iDiag=(double)iim*10.0+50.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim]-iDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj50"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj50 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;
		}

		double iiDiag=(double)iim*10.0+100.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim]-iiDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj100"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj100 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;
		}

		double iiiDiag=(double)iim*10.0+150.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim]-iiiDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj150"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj150 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;
		}

		double ivDiag=(double)iim*10.0+200.0;
		if( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] < ( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim]-ivDiag ) ) {
			step3plots2D["dijetWORecoPartonFlavorBjetsCSVM_masspt_cutDiagHiggsbb200_cutDiagStop1jj200"]->Fill( scalarSumPtdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim], massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			step3plots1D["massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200"]->Fill( massdijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200[iim] );
			p4jetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200_cutDiagStop1jj200 = p4dijetWORecoPartonFlavorBjetsCSVM_cutDiagHiggsbb200;
		}
	}
			
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


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
			fFile->mkdir("step1plots1D");
			fFile->cd("step1plots1D");
			for ( std::map<string,TH1* >::const_iterator imap=step1plots1D.begin(); imap!=step1plots1D.end(); ++imap ) {
				TH1 *temp = imap->second;
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
			fFile->mkdir("step3plots2D");
			fFile->cd("step3plots2D");
			for ( std::map<string,TH2* >::const_iterator imap=step3plots2D.begin(); imap!=step3plots2D.end(); ++imap ) {
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
