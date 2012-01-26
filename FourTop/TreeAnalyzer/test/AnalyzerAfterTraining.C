
// The class definition in AnalyzerAfterTraining.h has been generated automatically
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
// Root > T->Process("AnalyzerAfterTraining.C")
// Root > T->Process("AnalyzerAfterTraining.C","some options")
// Root > T->Process("AnalyzerAfterTraining.C+")
//

#include "AnalyzerAfterTraining.h"
//#include "BTagWeight.h"
#include "BTagSFUtil_lite.h"
//#include "Yumiceva/TreeAnalyzer/interface/JetCombinatorics.h"

#include <TStyle.h>
#include <TSystem.h>

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#define NMAX 100

void AnalyzerAfterTraining::ParseInput()
{

  if (fMyOpt.Contains("muon")) 
    {
      fChannel = 1;      
    }
  if (fMyOpt.Contains("electron"))
    {
      fChannel = 2;
    }
  if (fMyOpt.Contains("verbose"))
    {
      fVerbose = true;
    }
  if (fMyOpt.Contains("JECUP")) { fdoJECunc = true; fdoJECup = true; }
  if (fMyOpt.Contains("JECDOWN")) { fdoJECunc = true; fdoJECup = false; }
  if (fMyOpt.Contains("PUUP")) { fpuhistogram = "WHistUp";}
  if (fMyOpt.Contains("PUDOWN")) { fpuhistogram = "WHistDown";}
  if (fMyOpt.Contains("QCD1")) fdoQCD1SideBand = true;//anti-isolation
  if (fMyOpt.Contains("QCD2")) fdoQCD2SideBand = true;//MET < 20 GeV
  if (fMyOpt.Contains("mtop")) fdoMtopCut = true;
  if (fMyOpt.Contains("outdir")) {
    TString tmp = fMyOpt;
    tmp = tmp.Remove(0,fMyOpt.Index("outdir")+7);
    fOutdir = tmp+"/";
    Info("Begin","output files will be written to directory: %s", fOutdir.Data());
  }
  if (fMyOpt.Contains("sample"))
    {
      TString tmp = fMyOpt;
      tmp = tmp.Remove(0,fMyOpt.Index("sample")+7);
      fSample = tmp;
      if (fdoJECunc && fdoJECup==true) fSample += "_JECUP";
      if (fdoJECunc && fdoJECup==false) fSample += "_JECDOWN";
      if (fMyOpt.Contains("PUUP")) fSample += "_PUUP";
      if (fMyOpt.Contains("PUDOWN")) fSample += "_PUDOWN";

      Info("Begin","Histogram names will have suffix: %s", fSample.Data());

      if ( fSample.Contains("data") )
	{
	  fIsMC = false;
	  Info("Begin","This sample is treated as DATA");
	  if (fdoQCD1SideBand) fSample = "dataQCD1";
	  if (fdoQCD2SideBand) fSample = "dataQCD2";
	}
      else
	{
	  Info("Begin","This sample is treated as MC");
	  Info("Begin","we will use pileup set: %s", fpuhistogram.Data());
	}
      
      
    }
}

void AnalyzerAfterTraining::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   fMyOpt = option;
   ParseInput();

   Info("Begin", "starting with process option: %s", option.Data());

}

void AnalyzerAfterTraining::SlaveBegin(TTree * tree)
{
  
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   fMyOpt = option;
   ParseInput();

   Info("SlaveBegin",
        "starting with process option: %s (tree: %p)", option.Data(), tree);

   //initialize the Tree branch addresses
   Init(tree);

   // We may be creating a dataset or a merge file: check it
   TNamed *nm = dynamic_cast<TNamed *>(fInput->FindObject("SimpleNtuple.root"));
   if (nm) {
     // Just create the object
     UInt_t opt = TProofOutputFile::kRegister | TProofOutputFile::kOverwrite | TProofOutputFile::kVerify;
     fProofFile = new TProofOutputFile("SimpleNtuple.root", TProofOutputFile::kDataset, opt, nm->GetTitle());
   } else {
     // For the ntuple, we use the automatic file merging facility
     // Check if an output URL has been given
     TNamed *out = (TNamed *) fInput->FindObject("PROOF_OUTPUTFILE_LOCATION");
     Info("SlaveBegin", "PROOF_OUTPUTFILE_LOCATION: %s", (out ? out->GetTitle() : "undef"));
     TString tmpfilename = "results";
     if ( fSample != "" ) tmpfilename += "_"+fSample+".root";
     else tmpfilename = "results.root";
     fProofFile = new TProofOutputFile(tmpfilename, (out ? out->GetTitle() : "M"));
     out = (TNamed *) fInput->FindObject("PROOF_OUTPUTFILE");
     if (out) fProofFile->SetOutputFileName(fOutdir + out->GetTitle());
   }

   // Open the file
   //TDirectory *savedir = gDirectory;
   if (!(fFile = fProofFile->OpenFile("RECREATE"))) {
     Warning("SlaveBegin", "problems opening file: %s/%s",
	     fProofFile->GetDir(), fProofFile->GetFileName());
   }

   // Get PU weights
   TString weightfilename = "/uscms/home/yumiceva/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/Weight3Dfinebin4p7.root"; //Weight3D.root";
   fweightfile =  new TFile(weightfilename,"read");
   f3Dweight = (TH1D*) fweightfile->Get(fpuhistogram);

   //create histograms
   h1test = new TH1F("h1test","muon p_{T}",100,10.,400);
   //fHist = new HistoManager(string(fSample));
   TString hname = "_"+fSample;
   
   hMET["bdtresponse"] = new TH1F("bdtresponse"+hname,"BDT response",50, -0.5, 0.3);

   map<string,TH1* > allhistos = hMET;

   for ( map<string,TH1* >::const_iterator imap=allhistos.begin(); imap!=allhistos.end(); ++imap )
     {
       TH1 *temp = imap->second;
       temp->Sumw2();
       temp->SetXTitle( temp->GetTitle() );
     }

   // cut flow
   if (fChannel==1) 
     { //muon +jets
       fCutLabels.push_back("Processed");
       fCutLabels.push_back("OneIsoMu");
       fCutLabels.push_back("LooseMuVeto");
       fCutLabels.push_back("ElectronVeto");
       fCutLabels.push_back("MET");
       fCutLabels.push_back("1Jet");
       fCutLabels.push_back("2Jet");
       fCutLabels.push_back("3Jet");
       fCutLabels.push_back("4Jet");
       fCutLabels.push_back("2Jet1b");
       fCutLabels.push_back("2Jet2b");
       fCutLabels.push_back("MaxJets");
       fCutLabels.push_back("phi");
       fCutLabels.push_back("topmass");
     }
   else
     { //electron+jets
       fCutLabels.push_back("Processed");
       fCutLabels.push_back("OneIsoElectron");
       fCutLabels.push_back("LooseMuVeto");
       fCutLabels.push_back("ZVeto");
       fCutLabels.push_back("ConversionVeto");
       fCutLabels.push_back("MET");
       fCutLabels.push_back("1Jet");
       fCutLabels.push_back("2Jet");
       fCutLabels.push_back("3Jet");
       fCutLabels.push_back("4Jet");
       fCutLabels.push_back("2Jet1b");
       fCutLabels.push_back("2Jet2b");
     }
   hcutflow = new TH1D("cutflow","cut flow", fCutLabels.size(), 0.5, fCutLabels.size() +0.5 );

   for ( vector<string>::const_iterator ivec= fCutLabels.begin(); ivec!=fCutLabels.end(); ++ivec)
     {
       cutmap[ *ivec ] = 0;
     }

   double pu_weights[35] = {
     0.0255506,
     0.251923,
     0.549605,
     0.924918,
     1.25977,
     1.48142,
     1.57923,
     1.57799,
     1.52152,
     1.4414,
     1.35889,
     1.2841,
     1.21927,
     1.16125,
     1.11244,
     1.06446,
     1.01666,
     0.966989,
     0.913378,
     0.85774,
     0.799258,
     0.734225,
     0.670242,
     0.607641,
     0.54542,
     0.484084,
     0.427491,
     0.369787,
     0.321454,
     0.280706,
     0.238499,
     0.198961,
     0.166742,
     0.146428,
     0.224425
   };
   
   fpu_weights_vec.assign( pu_weights, pu_weights + 35 );

   // For JEC uncertainties
   if (fdoJECunc) fJECunc = new JetCorrectionUncertainty("/uscms/home/yumiceva/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/GR_R_42_V19_AK5PF_Uncertainty.txt");

   // LOT for b-tagging SF
   //bSF_table.LoadTable("/uscms/home/yumiceva/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/Table_CSVM_beff_SF.txt");
   lSF_table.LoadTable("/uscms/home/yumiceva/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/Table_CSVM_lmistag_SF.txt");
   leff_table.LoadTable("/uscms/home/yumiceva/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/Table_CSVM_lmistag_Eff.txt");
 
   //leff_tablebtag efficiency file and 2D histogram
   //TString btagefffilename = "/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/BtagEff.root";
   //btagefffile = new TFile(btagefffilename,"read");
   //ttbar
   //f2Dttbarbtag = (TH2D*) btagefffile->Get("ttbar_csv_btageff");
   //f2Dttbarctag = (TH2D*) btagefffile->Get("ttbar_csv_ctageff");
   //f2Dttbarlighttag = (TH2D*) btagefffile->Get("ttbar_csv_lighttageff");
   //wprime
   //f2Dwprimebtag = (TH2D*) btagefffile->Get("wprime_csv_btageff");
   //f2Dwprimectag = (TH2D*) btagefffile->Get("wprime_csv_ctageff");
   //f2Dwprimelighttag = (TH2D*) btagefffile->Get("wprime_csv_lighttageff");



}

Bool_t AnalyzerAfterTraining::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either AnalyzerAfterTraining::GetEntry() or TBranch::GetEntry()
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

   //TString option = GetOption();

  if ( entry % 100 == 0 )
  cout<< "process entry " << entry << endl;

  //TString sEntry = Form("%f", float(entry) );
  //   Info("Process",
  //"entry # %s", Form("%f", float(entry) ) );

  //fChain->GetTree()->GetEntry(entry);
  fChain->GetEntry(entry);

  //if (entry>10) return kTRUE;

  // event info
  //cout << "run: " << ntuple->run << " lumi: " << ntuple->lumi << endl;

  // get collections
  vector< TopVertexEvent > primaryVertices = ntuple->vertices;
  vector< TopMuonEvent > muons = ntuple->muons;
  vector< TopElectronEvent > electrons = ntuple->PFelectrons; // use PF electrons (gsf collection is called "electrons")
  vector< TopJetEvent > jets = ntuple->PFjets;

  // USE PF Isolation
  fMuSelector.UsePFIsolation(true);
  fEleSelector.UsePFIsolation(true);

  size_t total_pvs   = primaryVertices.size();
  size_t total_muons = muons.size();
  size_t total_electrons = electrons.size();
  size_t total_jets  = jets.size();

  float PVz = 0.;
  TLorentzVector p4muon, p4ele, p4lepton, p4MET;
  TLorentzVector p4Nu, p4OtherNu;
  TLorentzVector p4QCDmuon;

  vector< TLorentzVector > p4jets;
  vector< TLorentzVector > p4Othermuon;    // leading muon

   //--------- For MVA analysis
   const char* inputVars[] = { "Ht", "Stlep", "Stjet", "jet_number", "numBjets_csvl" };
   std::vector<std::string> inputVarsMVA;
   for (int i=0; i<5; ++i) inputVarsMVA.push_back( inputVars[i] );
   ReadBDT mvaReader( inputVarsMVA );  
   float bdtresponse = 0.;
   //////////////////////////////////////////////////

  ////////////////////
  // GENERATOR
  ///////////////////
  if (fIsMC && fSample.Contains("4Top") )
    {
      TLorentzVector p4genLepton;
      TLorentzVector p4genNu;
      TLorentzVector p4genb;
      if (ntuple->gen.bLep_pt>0)
	{
	  p4genLepton.SetPtEtaPhiE(ntuple->gen.mu_pt,ntuple->gen.mu_eta,ntuple->gen.mu_phi,ntuple->gen.mu_e);
	  p4genNu.SetPtEtaPhiE(ntuple->gen.nu_pt,ntuple->gen.nu_eta,ntuple->gen.nu_phi,ntuple->gen.nu_e);
	  p4genb.SetPtEtaPhiE(ntuple->gen.bLep_pt,ntuple->gen.bLep_eta,ntuple->gen.bLep_phi,ntuple->gen.bLep_e);
	}
    }
  ////////////////////////////////////
  // PRIMARY VERTICES
  ///////////////////////////////////

  for ( size_t ipv=0; ipv < total_pvs; ++ipv) 
    {
      if (ipv==0) PVz = primaryVertices[ipv].vz;
    }

  // calculate PU weight
  double PUweight = 1.;

  if (fIsMC) {

    Int_t      mc_npvminus1 = TMath::Min(ntuple->gen.Bx_minus1,34);
    Int_t      mc_npv0   = TMath::Min(ntuple->gen.Bx_0,34);
    Int_t      mc_npvplus1  = TMath::Min(ntuple->gen.Bx_plus1,34);

    PUweight = f3Dweight->GetBinContent(mc_npvminus1,mc_npv0,mc_npvplus1);

  }

  // For 4 tops
  if (fIsMC && fSample.Contains("4Top") ) {
	  PUweight = 1;
  }

  /////////////
  // HLT scale factor for MC
  ////////////
  double SF_hlt = 1.;
  if (fIsMC) SF_hlt = 0.97; //0.966;
  double SF_iso = 1.;
  if (fIsMC) SF_iso = 0.996;
  PUweight = PUweight*SF_hlt*SF_iso;  // LETS INCLUDE THE TRIGGER SF INTO THE PU WEIGHTS
  
  //TEMP
  //PUweight = 1.;

  cutmap["Processed"] += PUweight;

  if (fVerbose) cout << "done pv" << endl;

  //////////////////////////////////
  // MUONS
  //////////////////////////////////
  int ngoodIDmuons = 0;
  int nloosemuons = 0;
  int ntightmuons = 0;
  int nqcdmuons = 0;
  int nmuonID[8] = {0};

  double RelIso = -1.;
  double deltaR = -1.;
  double QCDRelIso = -1.;
  double QCDdeltaR = -1;


  for ( size_t imu=0; imu < total_muons; ++imu) {
     
	TopMuonEvent muon = muons[imu];
     
	h1test->Fill( muon.pt );
 
	if (muon.pt > 32. && fabs(muon.eta)<2.1 && muon.IsTrackerMuon==1 ) {
  		nmuonID[0] += 1;
		if (fabs(muon.d0)<0.02 ) {
			nmuonID[1] += 1;
			if (muon.trackerhits>=11 ) {
				nmuonID[2] += 1;
				if (muon.normchi2<10 ) {
					nmuonID[3] +=1;
					if (muon.muonhits>0 ) {
						nmuonID[4] += 1;
						if (muon.pixelhits >= 1 ) {
							nmuonID[5] += 1;
							if (muon.muonstations > 1 ) {
								nmuonID[6] +=1;
								if (fabs(muon.vz - PVz) < 1.) {
									nmuonID[7] +=1;
								}
							}
						}
					}
				}
			}
		}
	}

	if ( fMuSelector.MuonID( muon, PVz ) ) ngoodIDmuons++;
 
	// select only good muons
     
	if ( fMuSelector.MuonLoose( muon ) ) {
  
		nloosemuons++;
  
		if ( fMuSelector.MuonTightDeltaR( muon, PVz, jets) ) {
			ntightmuons++;
			deltaR = fMuSelector.GetDeltaR();
		}

		p4muon.SetPtEtaPhiE( muon.pt, muon.eta, muon.phi, muon.e );
		RelIso = muon.pfreliso; //muon.reliso03;
		p4Othermuon.push_back( p4muon ); // for leading muon
 	}

	// check muon in QCD control region
	if ( fMuSelector.MuonRelax02IsoQCD( muon, PVz, jets ) ) {
		nqcdmuons++;

		// keep the leading muon for selection
		if (nqcdmuons==1) {
			p4QCDmuon.SetPtEtaPhiE( muon.pt, muon.eta, muon.phi, muon.e );
			QCDRelIso = muon.pfreliso; //muon.reliso03;
			QCDdeltaR = fMuSelector.GetDeltaR();
		}
	}
  } 

  if (fVerbose) cout << "done muons" << endl;

  //////////////////////////////////
  // ELECTRONS
  //////////////////////////////////                                                                                                                                                                              
  int nlooseelectrons = 0;
  int ntightelectrons = 0;
  bool IsConversion = false;

  for ( size_t iele=0; iele < total_electrons; ++iele) {

	TopElectronEvent electron = electrons[iele];
    
	if ( fEleSelector.ElectronLoose(electron) ) nlooseelectrons++;

	if ( fEleSelector.ElectronTight(electron, PVz ) ) {

		if (ntightelectrons == 0) {
			IsConversion = electron.IsConversion;
			p4ele.SetPtEtaPhiE( electron.pt, electron.eta, electron.phi, electron.e );
		}
		ntightelectrons++;
	}
  }
  if (fVerbose) cout << "done electron" << endl;

  /////////////////////////////////////
  // MUON/ELECTRON + JETS
  ///////////////////////////////////// 

  if ( fChannel == 1 ) {

	if (fdoQCD2SideBand) {

		if (nqcdmuons  == 0) return kTRUE;
		cutmap["OneIsoMu"] += PUweight;

		p4lepton = p4QCDmuon;
		RelIso = QCDRelIso;
		deltaR = QCDdeltaR;
	}
       	else {
		if ( ntightmuons != 1 ) return kTRUE;
		cutmap["OneIsoMu"] += PUweight;

		if ( nloosemuons > 1 ) return kTRUE;
		cutmap["LooseMuVeto"] += PUweight;
	
		if ( nlooseelectrons > 0 ) return kTRUE;
		cutmap["ElectronVeto"] += PUweight;
	
		p4lepton = p4muon;
		if (fVerbose) cout << "got a good lepton" << endl;
	}
  }
  else // electron+jets
    {
      // pending ...
    }
  
  if (fVerbose) cout << "done lepton selection " << endl;

  /////////////////////////////////
  // MET + Ht
  /////////////////////////////////
  
  p4MET.SetPtEtaPhiE( ntuple->PFMET,
  		      0.,
  		      ntuple->PFMETphi,
  		      ntuple->PFMET );

  //temporal check using genMET
  //p4MET.SetPtEtaPhiE( ntuple->gen.MET,
  //                    0.,
  //                    ntuple->gen.METPhi,
  //                    ntuple->gen.MET );

  if (fdoQCD1SideBand && p4MET.Et() > 20.) return kTRUE;
  else if ( p4MET.Et() <= 20. && fdoQCD2SideBand==false ) return kTRUE;
  if (fVerbose) cout << "pass MET cut" << endl;

  double Wpt = p4lepton.Pt() + p4MET.Pt();
  double Wpx = p4lepton.Px() + p4MET.Px();
  double Wpy = p4lepton.Py() + p4MET.Py();
  double WMt = sqrt(Wpt*Wpt-Wpx*Wpx-Wpy*Wpy);
  
  //if ( WMt <= 40. ) return kTRUE; 
  cutmap["MET"] += PUweight;

  if (fVerbose) cout << "pass W MT cut " << endl;

  /////////////////////////////////
  // estimate Pz of neutrino
  ////////////////////////////////

  fzCalculator.SetMET(p4MET);
  fzCalculator.SetLepton(p4lepton);
  if (fChannel==2) {
	fzCalculator.SetLeptonType("electron");
  }
  
  double pzNu = fzCalculator.Calculate();
  double pzOtherNu = fzCalculator.getOther();
  if (fabs(pzNu) > fabs(pzOtherNu) ) {
	double tmppzNu = pzNu;
	double tmppzOtherNu = pzOtherNu;
	pzNu = tmppzOtherNu;
	pzOtherNu = tmppzNu;
  }

  p4Nu = TLorentzVector();
  p4OtherNu = TLorentzVector();

  p4Nu.SetPxPyPzE(p4MET.Px(), p4MET.Py(), pzNu, sqrt(p4MET.Px()*p4MET.Px()+p4MET.Py()*p4MET.Py()+pzNu*pzNu));
  //print "pzNu = " +str(pzNu)
  //print "p4Nu =("+str(p4Nu.Px())+","+str(p4Nu.Py())+","+str(p4Nu.Pz())+","+str(p4Nu.E())
  p4OtherNu.SetPxPyPzE( p4MET.Px(), p4MET.Py(),pzOtherNu,sqrt(p4MET.Px()*p4MET.Px()+p4MET.Py()*p4MET.Py()+pzOtherNu*pzOtherNu));

  //double WmassNoPt = (p4Nu+p4lepton).M();

  if ( fzCalculator.IsComplex() )  {
	double ptNu1 = fzCalculator.getPtneutrino(1);
	double ptNu2 = fzCalculator.getPtneutrino(2);
	TLorentzVector p4Nu1tmp;
	TLorentzVector p4Nu2tmp;

	p4Nu1tmp.SetPxPyPzE( ptNu1*p4MET.Px()/p4MET.Pt(), ptNu1*p4MET.Py()/p4MET.Pt(), pzNu, sqrt(ptNu1*ptNu1+pzNu*pzNu));
	p4Nu2tmp.SetPxPyPzE( ptNu2*p4MET.Px()/p4MET.Pt(), ptNu2*p4MET.Py()/p4MET.Pt(), pzNu, sqrt(ptNu2*ptNu2+pzNu*pzNu));
      
	TLorentzVector Wtmp;
	Wtmp = p4lepton + p4Nu1tmp;
	double Wm1 = 0;
	double Wm2 = 0;
	Wm1 = Wtmp.M();
	Wtmp = p4lepton + p4Nu2tmp;
	Wm2 = Wtmp.M();
	if ( fabs( Wm1 - 80.) < fabs( Wm2 - 80.) ) p4Nu = p4Nu1tmp;
	else p4Nu = p4Nu2tmp;

	p4OtherNu = p4Nu; // since we chose the real part, the two solutions are the same.
  }

  TLorentzVector p4LepW = p4lepton + p4Nu;
  TLorentzVector p4OtherLepW = p4lepton + p4OtherNu;

  /////////////////////////////////
  // JETS
  ////////////////////////////////
  
  //JetCombinatorics myCombi = JetCombinatoric();

  int njets = 0;
  map< string, vector<float> > bdisc;
  map< string, vector<bool> >  isTagb;
  vector<int> listflavor;

  for ( size_t ijet=0; ijet < total_jets; ++ijet)  {

	TopJetEvent jet = jets[ijet];
	double SF_JEC = 1.;
	if (fdoJECunc){
		fJECunc->setJetEta( jet.eta);
		fJECunc->setJetPt( jet.pt);
		double jec_unc = fJECunc->getUncertainty(true);
		if (fVerbose) cout << "JEC uncertainty is " << jec_unc << endl;
		if (fdoJECup) SF_JEC = 1.+jec_unc;
		else SF_JEC = 1.-jec_unc;
	}

	if ( SF_JEC*jet.pt > 40. && fabs(jet.eta) < 2.4 && SF_JEC*jets[0].pt > 100. && SF_JEC*jets[1].pt > 60. ) {    //first cut
		
		TLorentzVector tmpjet;
		tmpjet.SetPtEtaPhiE(SF_JEC*jet.pt, jet.eta, jet.phi, SF_JEC*jet.e);
		p4jets.push_back( tmpjet);
		listflavor.push_back( jet.mc.flavor );

		if (fVerbose) {
			cout << "done storing njets " << njets << endl;
			//cout << " bdisc " << jet.btag_TCHP << endl;
			cout << " bdisc " << jet.btag_CSV << endl;
		}

		
		// store discriminators
		//bdisc["TCHP"].push_back( jet.btag_TCHP );
		bdisc["CSV"].push_back( jet.btag_CSV );
		if (fVerbose) cout << "store bdisc" << endl;
		// TCHPL cut at 1.19
		// check TCHPM cut at 1.93
		//if ( jet.btag_TCHP > 1.93 ) isTagb["TCHPM"].push_back(true);
		//else isTagb["TCHPM"].push_back(false);
		//if (fVerbose) cout << "done tchpl" << endl;
		// check SSVHEM cut at 1.74
		if ( jet.btag_CSV > 0.244) {
			isTagb["CSVL"].push_back(true);
		} else {
			isTagb["CSVL"].push_back(false);
		}
		// CSVM cut at 0.244 LOOSE 
		if ( jet.btag_CSV > 0.679) {
			isTagb["CSVM"].push_back(true);
		} else {
			isTagb["CSVM"].push_back(false);
		}
		if (fVerbose) cout << "done csvm" << endl;
		// CSVM cut at 0.679 MEDIUM
		
		njets++;
	}
  }

  if (fVerbose) cout << "done jets" << endl;

  if (njets > 0 ) cutmap["1Jet"] += PUweight;
  if (njets > 1 ) cutmap["2Jet"] += PUweight;
  if (njets > 2 ) cutmap["3Jet"] += PUweight;
  if (njets > 3 ) cutmap["4Jet"] += PUweight;

  if (njets >= 4) {

	double Ht = 0; 
	double Stlep = 0; 
	double Stjet = 0; 

	for ( size_t kk=0; kk < p4jets.size(); ++kk) {
		// Ht calculation
		Ht += p4jets[kk].Pt();

		// St Calculation
		Stlep = p4MET.Pt() + p4lepton.Pt() ;
		Stjet = p4MET.Pt() + p4lepton.Pt() + Ht ;
	}

	// count number of b-tags
	int Nbtags_CSVL = 0;
	int Nbtags_CSVM = 0;

	for ( size_t itag=0; itag< isTagb["CSVM"].size(); ++itag ) {
		if ( isTagb["CSVL"][itag] ) Nbtags_CSVL++;
		if ( isTagb["CSVM"][itag] ) Nbtags_CSVM++;
	}
      
	// for BDT
	double jet_number = 0.;
	double numBjets_csvl = -999.;
	vector<double>mvaInputVal;

	//   Cuts
	bool passcut = true;
	if ( Ht <= 300. ) passcut = false;

	if (passcut) {
		mvaInputVal.push_back( Ht ); 
		mvaInputVal.push_back( Stlep ); 
		mvaInputVal.push_back( Stjet ); 
		jet_number = njets;
		mvaInputVal.push_back( jet_number ); 
		numBjets_csvl = Nbtags_CSVL;
		mvaInputVal.push_back( numBjets_csvl ); 


		bdtresponse = mvaReader.GetMvaValue( mvaInputVal );
		hMET["bdtresponse"]->Fill( bdtresponse, PUweight );
		//////////////////////////////////////////////////////////////////////////////////
	}
  }

  if (fVerbose) cout << "done analysis" << endl;
  return kTRUE;
}

void AnalyzerAfterTraining::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

  // fill cutflow histogram

  int ibin = 1;
  for ( vector<string>::const_iterator ivec= fCutLabels.begin(); ivec != fCutLabels.end(); ++ivec )
       //  for ( map<string, int >::const_iterator imap=cutmap.begin(); imap!=cutmap.end(); ++imap )
    {
      hcutflow->SetBinContent( ibin, cutmap[ *ivec ] );
      ibin++;
    }

  // Write the ntuple to the file
  if (fFile) {
	Bool_t cleanup = kFALSE;
	TDirectory *savedir = gDirectory;
	if ( h1test->GetEntries() > 0) {
		fFile->cd();
		h1test->Write();
		fFile->cd();
		fFile->mkdir("MET");
		fFile->cd("MET");
		for ( map<string,TH1* >::const_iterator imap=hMET.begin(); imap!=hMET.end(); ++imap ) {
			TH1 *temp = imap->second;
			if ( temp->GetEntries() > 0 )
			temp->Write();
		}
		fFile->cd();

		fProofFile->Print();
		fOutput->Add(fProofFile);
	} else {
		cleanup = kTRUE;
	}


    h1test->SetDirectory(0);
    hcutflow->SetDirectory(0);
    gDirectory = savedir;
    fFile->Close();
    // Cleanup, if needed
    if (cleanup) {
	TUrl uf(*(fFile->GetEndpointUrl()));
	SafeDelete(fFile);
	gSystem->Unlink(uf.GetFile());
	SafeDelete(fProofFile);
    }
  }

}

void AnalyzerAfterTraining::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

  Info("Terminate","AnalyzerAfterTraining done.");
}
