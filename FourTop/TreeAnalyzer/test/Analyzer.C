// The class definition in Analyzer.h has been generated automatically
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
// Root > T->Process("Analyzer.C")
// Root > T->Process("Analyzer.C","some options")
// Root > T->Process("Analyzer.C+")
//

#include "Analyzer.h"
#include "BTagWeight.h"
#include "BTagSFUtil_lite.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
//#include "Yumiceva/TreeAnalyzer/interface/JetCombinatorics.h"
#include "PhysicsTools/Utilities/interface/Lumi3DReWeighting.h"

#include <TStyle.h>
#include <TSystem.h>

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
//#define NMAX 100

struct sortPt
{
  bool operator()(TopJetEvent s1, TopJetEvent s2) const
  {
    return s1.pt >= s2.pt;
  }
} mysortPt;

void Analyzer::ParseInput()
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
  if (fMyOpt.Contains("JERUP")) { fdoJERup = true; }
  if (fMyOpt.Contains("JERDOWN")) { fdoJERdown = true; }
  if (fMyOpt.Contains("PUUP")) { fpu_up = true;}
  if (fMyOpt.Contains("PUDOWN")) { fpu_down = true;}
  if (fMyOpt.Contains("BTAGUP")) { fbtag_up = true;}
  if (fMyOpt.Contains("BTAGDOWN")) { fbtag_down = true;}
  if (fMyOpt.Contains("QCD1")) fdoQCD1SideBand = true;//anti-isolation
  if (fMyOpt.Contains("QCD2")) fdoQCD2SideBand = true;//MET < 20 GeV
  if (fMyOpt.Contains("diLepton")) fdodiLepton = true;
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
      if (fdoJERunc && fdoJERup==true) fSample += "_JERUP";
      if (fdoJERunc && fdoJERdown==true) fSample += "_JERDOWN";
      if (fMyOpt.Contains("BTAGUP")) fSample += "_BTAGUP";
      if (fMyOpt.Contains("BTAGDOWN")) fSample += "_BTAGDOWN";

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
	}
      
      
    }
}

void Analyzer::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   fMyOpt = option;
   ParseInput();

   Info("Begin", "starting with process option: %s", option.Data());

}

void Analyzer::SlaveBegin(TTree * tree)
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
      fProofFile = new TProofOutputFile( "SimpleNtuple.root",TProofOutputFile::kDataset, opt, nm->GetTitle());
   } else {
      // For the ntuple, we use the automatic file merging facility
      // Check if an output URL has been given
      TNamed *out = (TNamed *) fInput->FindObject("PROOF_OUTPUTFILE_LOCATION");
      Info("SlaveBegin", "PROOF_OUTPUTFILE_LOCATION: %s", (out ? out->GetTitle() : "undef"));
      //TString dir = "file:/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/resultsFinal/";
      TString dir = "file:/uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/";
      Info("SlaveBegin","We will store the sample under the directory: %s", dir.Data());
      TString tmpfilename = "results";
      if ( fSample != "" ) tmpfilename += "_"+fSample+".root";
      else tmpfilename = "results.root";
      //TString filename ="SimpleNtuple.root";
      fProofFile = new TProofOutputFile(tmpfilename,TProofOutputFile::kMerge);

      //fProofFile->SetOutputFileName(dir + "SimpleNtuple.root");
      fProofFile->SetOutputFileName(dir + tmpfilename);
      out = (TNamed *) fInput->FindObject("PROOF_OUTPUTFILE");

      if (out) fProofFile->SetOutputFileName(out->GetTitle());
   }

   // Open the file
   //TDirectory *savedir = gDirectory;
   if (!(fFile = fProofFile->OpenFile("RECREATE"))) {
     Warning("SlaveBegin", "problems opening file: %s/%s",
	     fProofFile->GetDir(), fProofFile->GetFileName());
   }

   //create histograms
   h1test = new TH1F("h1test","muon p_{T}",100,10.,400);
   TString hname = "_"+fSample;
   
   hmuons["N0"] = new TH1F("N_muons_cut0"+hname,"Number of Muons",6, -0.5, 5.5);
   hmuons["N"] = new TH1F("N_muons"+hname,"Number of Muons",6, -0.5, 5.5);
   hmuons["Nelectrons_cut0"] = new TH1F("Nelectrons_cut0"+hname,"Number of Loose Electrons", 6, -0.5, 5.5);
   hmuons["eta_cut0"] = new TH1F("muon_eta_cut0"+hname,"#eta^{#mu}", 42, -2.1, 2.1);
   hmuons["eta"] = new TH1F("muon_eta"+hname,"#eta^{#mu}", 42, -2.1, 2.1);
   hmuons["phi_cut0"] = new TH1F("muon_phi_cut0"+hname,"#phi^{#mu}", 32, -3.15, 3.15);
   hmuons["phi"] = new TH1F("muon_phi"+hname,"#phi^{#mu}", 32, -3.15, 3.15);
   hmuons["reliso"] = new TH1F("muon_reliso"+hname,"Relative Isolation", 20, 0, 0.2);
   hmuons["deltaR_cut0"] = new TH1F("deltaR_cut0"+hname,"#DeltaR(#mu,jet)",20, 0, 4);
   hmuons["deltaR"] = new TH1F("deltaR"+hname,"#DeltaR(#mu,jet)", 20, 0, 4);
   hmuons["deltaR_cut2"] = new TH1F("deltaR_cut2"+hname,"#DeltaR(#mu,jet)", 20, 0, 4);
   hmuons["d0_cut1"] = new TH1F("d0_cut1"+hname,"#mu Impact Parameter [cm]",22,-0.1,0.1);
   hmuons["pt_cut0"] = new TH1F("muon_pt_cut0"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["pt"] = new TH1F("muon_pt"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["pt_cut2"] = new TH1F("muon_pt_cut2"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   //hmuons["dz"] = new TH1F("dz"+hname,"|z(#mu) - z_{PV}| [cm]", 25, 0, 1.);
   hmuons["Niso"] = new TH1F("Niso"+hname,"Number of Primary Vertices", 25, -0.5, 24.5);
   hmuons["Ngood"] = new TH1F("Ngood"+hname,"Number of Primary Vertices",25, -0.5,24.5);
   hmuons["charge_tiso"] = new TH1F("charge_tiso"+hname,"#mu Charge", 5,-2.5,2.5);
   hmuons["j4m0"] = new TH1F("j4m0"+hname,"Number of Muons",6, 0.5,6.5);
   hmuons["j5m0"] = new TH1F("j5m0"+hname,"Number of Muons",6, 0.5,6.5);
   hmuons["j6m0"] = new TH1F("j6m0"+hname,"Number of Muons",6, 0.5,6.5);
   hmuons["j4m1"] = new TH1F("j4m1"+hname,"Number of Muons",6, 0.5,6.5);
   hmuons["j5m1"] = new TH1F("j5m1"+hname,"Number of Muons",6, 0.5,6.5);
   hmuons["j6m1"] = new TH1F("j6m1"+hname,"Number of Muons",6, 0.5,6.5);
   hmuons["j4m2"] = new TH1F("j4m2"+hname,"Number of Muons",6, 0.5,6.5);
   hmuons["j5m2"] = new TH1F("j5m2"+hname,"Number of Muons",6, 0.5,6.5);
   hmuons["j6m2"] = new TH1F("j6m2"+hname,"Number of Muons",6, 0.5,6.5);


   hPVs["Nreweight_cut0"] = new TH1F("NPVreweight_cut0"+hname,"Number of PVs",25, -0.5,24.5);
   hPVs["Nreweight"] = new TH1F("NPVreweight"+hname,"Number of PVs",25, -0.5, 24.5);
   hPVs["Nreweight_cut2"] = new TH1F("NPVreweight_cut2"+hname,"Number of PVs",25, -0.5, 24.5);
      
   hMET["MET_cut0"] = new TH1F("MET_cut0"+hname,"Missing Transverse Energy [GeV]", 50, 0, 500);
   hMET["MET"] = new TH1F("MET"+hname,"Missing Transverse Energy [GeV]", 50, 0, 500);
   hMET["MET_cut2"] = new TH1F("MET_cut2"+hname,"Missing Transverse Energy [GeV]", 50, 0, 500);
   hMET["phi_cut0"] = new TH1F("MET_phi_cut0"+hname,"#phi Missing Transverse Energy [GeV]", 32, -3.15, 3.15);
   hMET["phi"] = new TH1F("MET_phi"+hname,"#phi Missing Transverse Energy [GeV]", 32, -3.15, 3.15);
   hMET["Ht_cut0"] = new TH1F("Ht_cut0"+hname,"H_{T} [GeV]", 125, 0, 2500);
   hMET["Ht"] = new TH1F("Ht"+hname,"H_{T} [GeV]", 125, 0, 2500);
   hMET["Ht_cut2"] = new TH1F("Ht_cut2"+hname,"H_{T} [GeV]", 125, 0, 2500);
   hMET["Stlep_cut0"] = new TH1F("Stlep_cut0"+hname,"S_{T}^{lep} [Gev/c]", 50, 0, 1000);
   hMET["Stlep"] = new TH1F("Stlep"+hname,"S_{T}^{lep} [Gev/c]", 50, 0, 1000);
   hMET["Stlep_cut2"] = new TH1F("Stlep_cut2"+hname,"S_{T}^{lep} [Gev/c]", 50, 0, 1000);
   hMET["Stjet_cut0"] = new TH1F("Stjet_cut0"+hname,"S_{T}^{jet} [Gev/c]", 100, 0, 3000);
   hMET["Stjet"] = new TH1F("Stjet"+hname,"S_{T}^{jet} [Gev/c]", 100, 0, 3000);
   hMET["Stjet_cut2"] = new TH1F("Stjet_cut2"+hname,"S_{T}^{jet} [Gev/c]", 100, 0, 3000);
   hMET["PzNu"] = new TH1F("PzNu"+hname,"p_{z} #nu [GeV]", 60, -300,300);
   //hMET["EtaNu"] = new TH1F("EtaNu"+hname,"#eta",44,-2.2,2.2);
   hMET["deltaPhiMETWlep"] = new TH1F("deltaPhiMETWlep"+hname,"#Delta #phi(Missing Transverse Energy,W(#mu,#nu))",32, -3.15, 3.15);
   hMET["deltaPhiMETlep"] = new TH1F("deltaPhiMETlep"+hname,"#Delta #phi(Missing Transverse Energy,#mu)",32, -3.15, 3.15);
   hMET["deltaPhiNulep"] = new TH1F("deltaPhiNulep"+hname,"#Delta #phi(#mu, #nu)",32, -3.15, 3.15);
   hMET["deltaPhiMETWlep_cut2"] = new TH1F("deltaPhiMETWlep_cut2"+hname,"#Delta #phi(Missing Transverse Energy,W(#mu,#nu))",32, -3.15, 3.15);
   hMET["deltaPhiMETlep_cut2"] = new TH1F("deltaPhiMETlep_cut2"+hname,"#Delta #phi(Missing Transverse Energy,#mu)",32, -3.15, 3.15);
   hMET["deltaPhiNulep_cut2"] = new TH1F("deltaPhiNulep_cut2"+hname,"#Delta #phi(#mu, #nu)",32, -3.15, 3.15);

   hM["WMt"] = new TH1F("Mt"+hname,"M_{T}(W) [GeV/c^{2}]", 50, 0, 500);        // Transverse Mass sqrt(Wpt*Wpt - Wpx*Wpx - Wpy*Wpy)
   hM["WMt_cut0"] = new TH1F("Mt_cut0"+hname,"M_{T}(W) [GeV/c^{2}]", 50, 0, 500);
   hM["WMt_cut2"] = new TH1F("Mt_cut2"+hname,"M_{T}(W) [GeV/c^{2}]", 50, 0, 500);
   hM["dileadjet_cut0"] = new TH1F("dileadjet_cut0"+hname,"leading (jj) mass [GeV/c^{2}]", 100, 0, 1000);
   hM["dileadjet"] = new TH1F("dileadjet"+hname,"leading (jj) mass [GeV/c^{2}]", 30, 0, 600);
   hM["dileadjet_cut2"] = new TH1F("dileadjet_cut2"+hname,"leading (jj) mass [GeV/c^{2}]", 25, 0, 500);
   hM["hadW_mass_cut0"] = new TH1F("hadW_mass_cut0"+hname,"Hadronic W mass [GeV/c^{2}]", 24, 10, 500);
   hM["hadW_mass"] = new TH1F("hadW_mass"+hname,"Hadronic W mass [GeV/c^{2}]", 49, 10, 500);
   hM["hadW_mass_cut2"] = new TH1F("hadW_mass_cut2"+hname,"Hadronic W mass [GeV/c^{2}]", 49, 10, 500);
   hM["top_mass_cut0"] = new TH1F("top_mass_cut0"+hname,"Hadronic Top mass [GeV/c^{2}]", 25, 0, 500);
   hM["top_mass"] = new TH1F("top_mass"+hname,"Hadronic Top mass [GeV/c^{2}]", 25, 0, 500);
   hM["top_mass_cut2"] = new TH1F("top_mass_cut2"+hname,"Hadronic Top mass [GeV/c^{2}]", 25, 0, 500);
   hM["top_pt"] = new TH1F("top_pt"+hname,"Hadronic Top p_{T} [GeV/c]", 50, 0, 1000);
   hM["top_pt_cut0"] = new TH1F("top_pt_cut0"+hname,"Hadronic Top p_{T} [GeV/c]", 50, 0, 1000);
   hM["top_pt_cut2"] = new TH1F("top_pt_cut2"+hname,"Hadronic Top p_{T} [GeV/c]", 50, 0, 500);
   hM["Leptop_mass"] = new TH1F("Leptop"+hname,"Leptonic Top mass [GeV/c^{2}]", 25, 0, 500);
   hM["Leptop_pt"] = new TH1F("Leptop_pt"+hname,"Leptonic Top p_{T} [GeV/c]", 25, 0, 500);
   hM["lepW_mass"] = new TH1F("lepW_mass"+hname,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",15, 50, 200);
   hM["lepW_mass_cut2"] = new TH1F("lepW_mass_cut2"+hname,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",15, 50, 200);
   hM["lepW_pt"]=new TH1F("lepW_pt"+hname,"W#rightarrow#mu#nu p_{t} [GeV/c^{2}]",100, 0, 1000);
   hM["lepW_pt_cut2"]=new TH1F("lepW_pt_cut2"+hname,"W#rightarrow#mu#nu p_{t} [GeV/c^{2}]",100, 0, 1000);
   hM["diWdeltaphi"] = new TH1F("diWdeltaphi"+hname,"#Delta #phi(W(j,j), W(#mu,#nu))",34, -3.40, 3.40);
   hM["diWdeltaR"] = new TH1F("diWdeltaR"+hname,"#Delta R (W(j,j), W(#mu,#nu))",30, 0, 6.);
   hM["chi2"] = new TH1F("chi2"+hname,"#chi^{2}", 50, 0, 500);

   hjets["pt"] = new TH1F("jet_pt"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   //hjets["pt_b_mc"] = new TH1F("jet_pt_b_mc"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   //hjets["pt_c_mc"] = new TH1F("jet_pt_c_mc"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   //hjets["pt_l_mc"] = new TH1F("jet_pt_l_mc"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   //hjets["pt_btag"] = new TH1F("jet_pt_btag"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   //hjets["pt_btag_b"] = new TH1F("jet_pt_btag_b"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   //hjets["pt_btag_c"] = new TH1F("jet_pt_btag_c"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   //hjets["pt_btag_l"] = new TH1F("jet_pt_btag_l"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["1st_pt_cut0"] = new TH1F("jet1_pt_cut0"+hname,"1st Jet p_{T} [GeV/c]",120, 0, 1200);
   hjets["1st_pt"] = new TH1F("jet1_pt"+hname,"1st Jet p_{T} [GeV/c]", 120, 0, 1200);
   hjets["1st_pt_cut2"] = new TH1F("jet1_pt_cut2"+hname,"1st Jet p_{T} [GeV/c]",120, 0, 1200);
   hjets["2nd_pt_cut0"] = new TH1F("jet2_pt_cut0"+hname,"2nd Jet p_{T} [GeV/c]", 80, 0, 800);
   hjets["2nd_pt"] = new TH1F("jet2_pt"+hname,"2nd Jet p_{T} [GeV/c]", 80, 0, 800);
   hjets["2nd_pt_cut2"] = new TH1F("jet2_pt_cut2"+hname,"2nd Jet p_{T} [GeV/c]", 80, 0, 800);
   hjets["3rd_pt_cut0"] = new TH1F("jet3_pt_cut0"+hname,"3rd Jet p_{T} [GeV/c]", 50, 0, 500);
   hjets["3rd_pt"] = new TH1F("jet3_pt"+hname,"3rd Jet p_{T} [GeV/c]", 50, 0, 500);
   hjets["3rd_pt_cut2"] = new TH1F("jet3_pt_cut2"+hname,"3rd Jet p_{T} [GeV/c]", 50, 0, 500);
   hjets["4th_pt_cut0"] = new TH1F("jet4_pt_cut0"+hname,"4th Jet p_{T} [GeV/c]", 35, 0, 350);
   hjets["4th_pt"] = new TH1F("jet4_pt"+hname,"4th Jet p_{T} [GeV/c]", 35, 0, 350);
   hjets["4th_pt_cut2"] = new TH1F("jet4_pt_cut2"+hname,"4th Jet p_{T} [GeV/c]", 35, 0, 350);
   hjets["5th_pt_cut0"] = new TH1F("jet5_pt_cut0"+hname,"5th Jet p_{T} [GeV/c]", 29, 10, 300);
   hjets["5th_pt"] = new TH1F("jet5_pt"+hname,"5th Jet p_{T} [GeV/c]", 29, 10, 300);
   hjets["5th_pt_cut2"] = new TH1F("jet5_pt_cut2"+hname,"5th Jet p_{T} [GeV/c]", 29, 10, 300);
   hjets["6th_pt_cut0"] = new TH1F("jet6_pt_cut0"+hname,"6th Jet p_{T} [GeV/c]", 29, 10, 300);
   hjets["6th_pt"] = new TH1F("jet6_pt"+hname,"6th Jet p_{T} [GeV/c]", 29, 10, 300);
   hjets["6th_pt_cut2"] = new TH1F("jet6_pt_cut2"+hname,"6th Jet p_{T} [GeV/c]", 29, 10, 300);
   hjets["7th_pt_cut0"] = new TH1F("jet7_pt_cut0"+hname,"7th Jet p_{T} [GeV/c]", 29, 10, 300);
   hjets["7th_pt"] = new TH1F("jet7_pt"+hname,"7th Jet p_{T} [GeV/c]", 29, 10, 300); 
   hjets["7th_pt_cut2"] = new TH1F("jet7_pt_cut2"+hname,"7th Jet p_{T} [GeV/c]", 29, 10, 300); 
   hjets["eta"] = new TH1F("jet_eta"+hname,"jet #eta",48, -2.4, 2.4);
   hjets["1st_eta_cut0"] = new TH1F("jet1_eta_cut0"+hname,"1st Jet #eta",48, -2.4, 2.4);
   hjets["1st_eta"] = new TH1F("jet1_eta"+hname,"1st Jet #eta",48, -2.4, 2.4);
   hjets["1st_eta_cut2"] = new TH1F("jet1_eta_cut2"+hname,"1st Jet #eta",48, -2.4, 2.4);
   hjets["2nd_eta_cut0"] = new TH1F("jet2_eta_cut0"+hname,"2nd Jet #eta",48, -2.4, 2.4);
   hjets["2nd_eta"] = new TH1F("jet2_eta"+hname,"2nd Jet #eta",48, -2.4, 2.4);
   hjets["2nd_eta_cut2"] = new TH1F("jet2_eta_cut2"+hname,"2nd Jet #eta",48, -2.4, 2.4);
   hjets["3rd_eta"] = new TH1F("jet3_eta"+hname,"3rd Jet #eta",48, -2.4, 2.4);
   hjets["4th_eta"] = new TH1F("jet4_eta"+hname,"4th Jet #eta",48, -2.4, 2.4);
   hjets["phi"] = new TH1F("jet_phi"+hname,"jet #phi",32, -3.15, 3.15);
   hjets["Njets_cut0"] = new TH1F("Njets_cut0"+hname,"Jet Multiplicity",12, 0.5,12.5);
   hjets["Njets"] = new TH1F("Njets"+hname,"jet multiplicity",12,0.5,12.5);
   hjets["Njets_cut2"] = new TH1F("Njets_cut2"+hname,"jet multiplicity",12, 0.5,12.5);
   hjets["Nbtags_TCHPM_cut0"] = new TH1F("Nbjets_TCHPM_cut0"+hname,"Tagged b-jets",8,-0.5,7.5);
   hjets["Nbtags_TCHPM"] = new TH1F("Nbjets_TCHPM"+hname,"Tagged b-jets",8,-0.5,7.5);
   hjets["Nbtags_CSVL_cut0"] = new TH1F("Nbjets_CSVL_cut0"+hname,"Tagged b-jets",8,-0.5,7.5);
   hjets["Nbtags_CSVL"] = new TH1F("Nbjets_CSVL"+hname,"CSV Loose Tagged b-jets",8,-0.5,7.5);
   hjets["Nbtags_CSVM_cut0"] = new TH1F("Nbjets_CSVM_cut0"+hname,"Tagged b-jets",8, -0.5, 7.5);
   hjets["Nbtags_CSVM"] = new TH1F("Nbjets_CSVM"+hname,"Tagged b-jets",8,-0.5,7.5);
   hjets["Nbtags_CSVT_cut0"] = new TH1F("Nbjets_CSVT_cut0"+hname,"Tagged b-jets",8,-0.5,7.5);
   hjets["Nbtags_CSVT"] = new TH1F("Nbjets_CSVT"+hname,"Tagged b-jets",8,-0.5,7.5);
   hjets["deltaPhijj_cut0"] = new TH1F("deltaPhijj_cut0"+hname,"#Delta #phi(j1,j2)",34,-3.40,3.40);
   hjets["deltaPhijj"] = new TH1F("deltaPhijj"+hname,"#Delta #phi(j1,j2)",34, -3.40, 3.40);
   hjets["deltaRjj_cut0"] = new TH1F("deltaRjj_cut0"+hname,"#DeltaR(j1,j2)",20,0.,4.);
   hjets["mindeltaRjj"] = new TH1F("mindeltaRjj"+hname,"min #DeltaR(j,j)",15,0.,3.);
   hjets["deltaRjj"] = new TH1F("deltaRjj"+hname,"#DeltaR(j1,j2)",30,0.,6.);
   hjets["deltaRjj_cut2"] = new TH1F("deltaRjj_cut2"+hname,"#DeltaR(j1,j2)",30,0.,6.);
   hjets["deltaRmu2j"] = new TH1F("deltaRmu2j"+hname,"#DeltaR(#mu,j2)",30,0.,6.);
   hjets["deltaRmu3j"] = new TH1F("deltaRmu3j"+hname,"#DeltaR(#mu,j2)",30,0.,6.);
   hjets["deltaPhimu2j"] = new TH1F("deltaPhimu2j"+hname,"#Delta #phi(#mu,j2)",17, -3.40, 3.40);
   hjets["deltaPhimu3j"] = new TH1F("deltaPhimu3j"+hname,"#Delta #phi(#mu,j3)",68, -3.40, 3.40);
   //hjets["tb_deltaPhi"] = new TH1F("tb_deltaPhi"+hname,"#Delta #phi(t,b)",30,0.,3.15);
   //hjets["tb_deltaEta"] = new TH1F("tb_deltaEta"+hname,"#Delta #eta(t,b)",30,-5,5);
   //hjets["pt_b"]  = new TH1F("pt_b"+hname,"b-jet p_{T} [GeV]",60,0,1500);
   //hjets["jet1_CSVM_pt"] = new TH1F("jet1_CSVM_pt"+hname,"leading btag jet p_{T} (CSVM) [GeV/c]",80, 0, 800); 
   //hjets["jet2_CSVM_pt"] = new TH1F("jet2_CSVM_pt"+hname,"2 leading btag jet p_{T} (CSVM) [GeV/c]",50, 0, 500); 
   //hjets["jet3_CSVM_pt"] = new TH1F("jet3_CSVM_pt"+hname,"3 leading btag jet p_{T} (CSVM) [GeV/c]",20, 0, 200); 
   //hjets["jet4_CSVM_pt"] = new TH1F("jet4_CSVM_pt"+hname,"4 leading btag jet p_{T} (CSVM) [GeV/c]",20, 0, 200); 
   hjets["jet1_CSVL_pt"] = new TH1F("jet1_CSVL_pt"+hname,"leading btag jet p_{T} (CSVL) [GeV/c]",80, 0, 800); 
   //hjets["jet2_CSVL_pt"] = new TH1F("jet2_CSVL_pt"+hname,"2 leading btag jet p_{T} (CSVL) [GeV/c]",50, 0, 500); 
   //hjets["jet3_CSVL_pt"] = new TH1F("jet3_CSVL_pt"+hname,"3 leading btag jet p_{T} (CSVL) [GeV/c]",20, 0, 200); 
   //hjets["jet4_CSVL_pt"] = new TH1F("jet4_CSVL_pt"+hname,"4 leading btag jet p_{T} (CSVL) [GeV/c]",20, 0, 200); 
   //
   //
   //
   //
   //////////////////////////////////// Additional Plots
   //
   hjets["Stjetm1j4b0"] = new TH1F("Stjetm1j4b0"+hname,"S_{T}^{jet} (1 #mu & 4 jets & 0 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm1j4b0"] = new TH1F("DeltaRdijetm1j4b0"+hname,"#DeltaR(j,j) (1 #mu & 4 jets & 0 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm1j4b0"] = new TH1F("DeltaPhiMETWlepm1j4b0"+hname,"#Delta #phi(MET,W(#mu,#nu)) (1 #mu & 4 jets & 0 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm1j4b0"] = new TH1F("trijetmassm1j4b0"+hname,"top mass (1 #mu & 4 jets & 0 btag)", 100, 0, 1000 ); 
   hjets["trijetptm1j4b0"] = new TH1F("trijetptm1j4b0"+hname,"top p_{T} (1 #mu & 4 jets & 0 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm1j4b0"] = new TH1F("deltaRmindijetm1j4b0"+hname,"min #DeltaR(j,j) (1 #mu & 4 jets & 0 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm1j4b0"] = new TH1F("deltaRmu2jetm1j4b0"+hname,"#DeltaR(#mu,2nd j) (1 #mu & 4 jets & 0 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm1j4b0"] = new TH1F("deltaPhidijetm1j4b0"+hname,"#Delta #phi(j,j) (1 #mu & 4 jets & 0 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm1j4b0"] = new TH1F("deltaPhimu2jetm1j4b0"+hname,"#Delta #phi(#mu,2nd j) (1 #mu & 4 jets & 0 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm1j4b0"] = new TH1F("Stlepm1j4b0"+hname,"S_{t}^{lep} (1 #mu & 4 jets & 0 btag)",  100, 0, 1000); 
   hjets["Htm1j4b0"] = new TH1F("Htm1j4b0"+hname,"H_{T} (1 #mu & 4 jets & 0 btag)",  125, 0, 2500); 
   hjets["Njetsm1j4b0"] = new TH1F("Njetsm1j4b0"+hname,"Njets (1 #mu & 4 jets & 0 btag)", 12,0.5,12.5); 
   hjets["Stjetm1j5b0"] = new TH1F("Stjetm1j5b0"+hname,"S_{T}^{jet} (1 #mu &  5 jets & 0 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm1j5b0"] = new TH1F("DeltaRdijetm1j5b0"+hname,"#DeltaR(j,j) (1 #mu &  5 jets & 0 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm1j5b0"] = new TH1F("DeltaPhiMETWlepm1j5b0"+hname,"#Delta #phi(MET,W(#mu,#nu)) (1 #mu &  5 jets & 0 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm1j5b0"] = new TH1F("trijetmassm1j5b0"+hname,"top mass (1 #mu &  5 jets & 0 btag)", 100, 0, 1000 ); 
   hjets["trijetptm1j5b0"] = new TH1F("trijetptm1j5b0"+hname,"top p_{T} (1 #mu &  5 jets & 0 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm1j5b0"] = new TH1F("deltaRmindijetm1j5b0"+hname,"min #DeltaR(j,j) (1 #mu &  5 jets & 0 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm1j5b0"] = new TH1F("deltaRmu2jetm1j5b0"+hname,"#DeltaR(#mu,2nd j) (1 #mu &  5 jets & 0 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm1j5b0"] = new TH1F("deltaPhidijetm1j5b0"+hname,"#Delta #phi(j,j) (1 #mu &  5 jets & 0 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm1j5b0"] = new TH1F("deltaPhimu2jetm1j5b0"+hname,"#Delta #phi(#mu,2nd j) (1 #mu &  5 jets & 0 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm1j5b0"] = new TH1F("Stlepm1j5b0"+hname,"S_{t}^{lep} (1 #mu &  5 jets & 0 btag)",  100, 0, 1000); 
   hjets["Htm1j5b0"] = new TH1F("Htm1j5b0"+hname,"H_{T} (1 #mu &  5 jets & 0 btag)",  125, 0, 2500); 
   hjets["Njetsm1j5b0"] = new TH1F("Njetsm1j5b0"+hname,"Njets (1 #mu &  5 jets & 0 btag)", 12,0.5,12.5); 
   hjets["Stjetm1j6b0"] = new TH1F("Stjetm1j6b0"+hname,"S_{T}^{jet} (1 #mu &  #geq 6 jets & 0 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm1j6b0"] = new TH1F("DeltaRdijetm1j6b0"+hname,"#DeltaR(j,j) (1 #mu &  #geq 6 jets & 0 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm1j6b0"] = new TH1F("DeltaPhiMETWlepm1j6b0"+hname,"#Delta #phi(MET,W(#mu,#nu)) (1 #mu &  #geq 6 jets & 0 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm1j6b0"] = new TH1F("trijetmassm1j6b0"+hname,"top mass (1 #mu &  #geq 6 jets & 0 btag)", 100, 0, 1000 ); 
   hjets["trijetptm1j6b0"] = new TH1F("trijetptm1j6b0"+hname,"top p_{T} (1 #mu &  #geq 6 jets & 0 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm1j6b0"] = new TH1F("deltaRmindijetm1j6b0"+hname,"min #DeltaR(j,j) (1 #mu &  #geq 6 jets & 0 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm1j6b0"] = new TH1F("deltaRmu2jetm1j6b0"+hname,"#DeltaR(#mu,2nd j) (1 #mu &  #geq 6 jets & 0 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm1j6b0"] = new TH1F("deltaPhidijetm1j6b0"+hname,"#Delta #phi(j,j) (1 #mu &  #geq 6 jets & 0 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm1j6b0"] = new TH1F("deltaPhimu2jetm1j6b0"+hname,"#Delta #phi(#mu,2nd j) (1 #mu &  #geq 6 jets & 0 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm1j6b0"] = new TH1F("Stlepm1j6b0"+hname,"S_{t}^{lep} (1 #mu &  #geq 6 jets & 0 btag)",  100, 0, 1000); 
   hjets["Htm1j6b0"] = new TH1F("Htm1j6b0"+hname,"H_{T} (1 #mu &  #geq 6 jets & 0 btag)",  125, 0, 2500); 
   hjets["Njetsm1j6b0"] = new TH1F("Njetsm1j6b0"+hname,"Njets (1 #mu &  #geq 6 jets & 0 btag)", 12,0.5,12.5); 


   hjets["Stjetm1j4b1"] = new TH1F("Stjetm1j4b1"+hname,"S_{T}^{jet} (1 #mu & 4 jets & 1 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm1j4b1"] = new TH1F("DeltaRdijetm1j4b1"+hname,"#DeltaR(j,j) (1 #mu & 4 jets & 1 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm1j4b1"] = new TH1F("DeltaPhiMETWlepm1j4b1"+hname,"#Delta #phi(MET,W(#mu,#nu)) (1 #mu & 4 jets & 1 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm1j4b1"] = new TH1F("trijetmassm1j4b1"+hname,"top mass (1 #mu & 4 jets & 1 btag)", 100, 0, 1000 ); 
   hjets["trijetptm1j4b1"] = new TH1F("trijetptm1j4b1"+hname,"top p_{T} (1 #mu & 4 jets & 1 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm1j4b1"] = new TH1F("deltaRmindijetm1j4b1"+hname,"min #DeltaR(j,j) (1 #mu & 4 jets & 1 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm1j4b1"] = new TH1F("deltaRmu2jetm1j4b1"+hname,"#DeltaR(#mu,2nd j) (1 #mu & 4 jets & 1 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm1j4b1"] = new TH1F("deltaPhidijetm1j4b1"+hname,"#Delta #phi(j,j) (1 #mu & 4 jets & 1 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm1j4b1"] = new TH1F("deltaPhimu2jetm1j4b1"+hname,"#Delta #phi(#mu,2nd j) (1 #mu & 4 jets & 1 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm1j4b1"] = new TH1F("Stlepm1j4b1"+hname,"S_{t}^{lep} (1 #mu & 4 jets & 1 btag)",  100, 0, 1000); 
   hjets["Htm1j4b1"] = new TH1F("Htm1j4b1"+hname,"H_{T} (1 #mu & 4 jets & 1 btag)",  125, 0, 2500); 
   hjets["Njetsm1j4b1"] = new TH1F("Njetsm1j4b1"+hname,"Njets (1 #mu & 4 jets & 1 btag)", 12,0.5,12.5); 
   hjets["Stjetm1j5b1"] = new TH1F("Stjetm1j5b1"+hname,"S_{T}^{jet} (1 #mu &  5 jets & 1 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm1j5b1"] = new TH1F("DeltaRdijetm1j5b1"+hname,"#DeltaR(j,j) (1 #mu &  5 jets & 1 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm1j5b1"] = new TH1F("DeltaPhiMETWlepm1j5b1"+hname,"#Delta #phi(MET,W(#mu,#nu)) (1 #mu &  5 jets & 1 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm1j5b1"] = new TH1F("trijetmassm1j5b1"+hname,"top mass (1 #mu &  5 jets & 1 btag)", 100, 0, 1000 ); 
   hjets["trijetptm1j5b1"] = new TH1F("trijetptm1j5b1"+hname,"top p_{T} (1 #mu &  5 jets & 1 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm1j5b1"] = new TH1F("deltaRmindijetm1j5b1"+hname,"min #DeltaR(j,j) (1 #mu &  5 jets & 1 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm1j5b1"] = new TH1F("deltaRmu2jetm1j5b1"+hname,"#DeltaR(#mu,2nd j) (1 #mu &  5 jets & 1 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm1j5b1"] = new TH1F("deltaPhidijetm1j5b1"+hname,"#Delta #phi(j,j) (1 #mu &  5 jets & 1 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm1j5b1"] = new TH1F("deltaPhimu2jetm1j5b1"+hname,"#Delta #phi(#mu,2nd j) (1 #mu &  5 jets & 1 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm1j5b1"] = new TH1F("Stlepm1j5b1"+hname,"S_{t}^{lep} (1 #mu &  5 jets & 1 btag)",  100, 0, 1000); 
   hjets["Htm1j5b1"] = new TH1F("Htm1j5b1"+hname,"H_{T} (1 #mu &  5 jets & 1 btag)",  125, 0, 2500); 
   hjets["Njetsm1j5b1"] = new TH1F("Njetsm1j5b1"+hname,"Njets (1 #mu &  5 jets & 1 btag)", 12,0.5,12.5); 
   hjets["Stjetm1j6b1"] = new TH1F("Stjetm1j6b1"+hname,"S_{T}^{jet} (1 #mu &  #geq 6 jets & 1 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm1j6b1"] = new TH1F("DeltaRdijetm1j6b1"+hname,"#DeltaR(j,j) (1 #mu &  #geq 6 jets & 1 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm1j6b1"] = new TH1F("DeltaPhiMETWlepm1j6b1"+hname,"#Delta #phi(MET,W(#mu,#nu)) (1 #mu &  #geq 6 jets & 1 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm1j6b1"] = new TH1F("trijetmassm1j6b1"+hname,"top mass (1 #mu &  #geq 6 jets & 1 btag)", 100, 0, 1000 ); 
   hjets["trijetptm1j6b1"] = new TH1F("trijetptm1j6b1"+hname,"top p_{T} (1 #mu &  #geq 6 jets & 1 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm1j6b1"] = new TH1F("deltaRmindijetm1j6b1"+hname,"min #DeltaR(j,j) (1 #mu &  #geq 6 jets & 1 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm1j6b1"] = new TH1F("deltaRmu2jetm1j6b1"+hname,"#DeltaR(#mu,2nd j) (1 #mu &  #geq 6 jets & 1 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm1j6b1"] = new TH1F("deltaPhidijetm1j6b1"+hname,"#Delta #phi(j,j) (1 #mu &  #geq 6 jets & 1 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm1j6b1"] = new TH1F("deltaPhimu2jetm1j6b1"+hname,"#Delta #phi(#mu,2nd j) (1 #mu &  #geq 6 jets & 1 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm1j6b1"] = new TH1F("Stlepm1j6b1"+hname,"S_{t}^{lep} (1 #mu &  #geq 6 jets & 1 btag)",  100, 0, 1000); 
   hjets["Htm1j6b1"] = new TH1F("Htm1j6b1"+hname,"H_{T} (1 #mu &  #geq 6 jets & 1 btag)",  125, 0, 2500); 
   hjets["Njetsm1j6b1"] = new TH1F("Njetsm1j6b1"+hname,"Njets (1 #mu &  #geq 6 jets & 1 btag)", 12,0.5,12.5); 

   hjets["Stjetm1j4b2"] = new TH1F("Stjetm1j4b2"+hname,"S_{T}^{jet} (1 #mu & 4 jets & #geq 2 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm1j4b2"] = new TH1F("DeltaRdijetm1j4b2"+hname,"#DeltaR(j,j) (1 #mu & 4 jets & #geq 2 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm1j4b2"] = new TH1F("DeltaPhiMETWlepm1j4b2"+hname,"#Delta #phi(MET,W(#mu,#nu)) (1 #mu & 4 jets & #geq 2 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm1j4b2"] = new TH1F("trijetmassm1j4b2"+hname,"top mass (1 #mu & 4 jets & #geq 2 btag)", 100, 0, 1000 ); 
   hjets["trijetptm1j4b2"] = new TH1F("trijetptm1j4b2"+hname,"top p_{T} (1 #mu & 4 jets & #geq 2 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm1j4b2"] = new TH1F("deltaRmindijetm1j4b2"+hname,"min #DeltaR(j,j) (1 #mu & 4 jets & #geq 2 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm1j4b2"] = new TH1F("deltaRmu2jetm1j4b2"+hname,"#DeltaR(#mu,2nd j) (1 #mu & 4 jets & #geq 2 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm1j4b2"] = new TH1F("deltaPhidijetm1j4b2"+hname,"#Delta #phi(j,j) (1 #mu & 4 jets & #geq 2 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm1j4b2"] = new TH1F("deltaPhimu2jetm1j4b2"+hname,"#Delta #phi(#mu,2nd j) (1 #mu & 4 jets & #geq 2 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm1j4b2"] = new TH1F("Stlepm1j4b2"+hname,"S_{t}^{lep} (1 #mu & 4 jets & #geq 2 btag)",  100, 0, 1000); 
   hjets["Htm1j4b2"] = new TH1F("Htm1j4b2"+hname,"H_{T} (1 #mu & 4 jets & #geq 2 btag)",  125, 0, 2500); 
   hjets["Njetsm1j4b2"] = new TH1F("Njetsm1j4b2"+hname,"Njets (1 #mu & 4 jets & #geq 2 btag)", 12,0.5,12.5); 
   hjets["Stjetm1j5b2"] = new TH1F("Stjetm1j5b2"+hname,"S_{T}^{jet} (1 #mu &  5 jets & #geq 2 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm1j5b2"] = new TH1F("DeltaRdijetm1j5b2"+hname,"#DeltaR(j,j) (1 #mu &  5 jets & #geq 2 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm1j5b2"] = new TH1F("DeltaPhiMETWlepm1j5b2"+hname,"#Delta #phi(MET,W(#mu,#nu)) (1 #mu &  5 jets & #geq 2 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm1j5b2"] = new TH1F("trijetmassm1j5b2"+hname,"top mass (1 #mu &  5 jets & #geq 2 btag)", 100, 0, 1000 ); 
   hjets["trijetptm1j5b2"] = new TH1F("trijetptm1j5b2"+hname,"top p_{T} (1 #mu &  5 jets & #geq 2 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm1j5b2"] = new TH1F("deltaRmindijetm1j5b2"+hname,"min #DeltaR(j,j) (1 #mu &  5 jets & #geq 2 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm1j5b2"] = new TH1F("deltaRmu2jetm1j5b2"+hname,"#DeltaR(#mu,2nd j) (1 #mu &  5 jets & #geq 2 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm1j5b2"] = new TH1F("deltaPhidijetm1j5b2"+hname,"#Delta #phi(j,j) (1 #mu &  5 jets & #geq 2 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm1j5b2"] = new TH1F("deltaPhimu2jetm1j5b2"+hname,"#Delta #phi(#mu,2nd j) (1 #mu &  5 jets & #geq 2 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm1j5b2"] = new TH1F("Stlepm1j5b2"+hname,"S_{t}^{lep} (1 #mu &  5 jets & #geq 2 btag)",  100, 0, 1000); 
   hjets["Htm1j5b2"] = new TH1F("Htm1j5b2"+hname,"H_{T} (1 #mu &  5 jets & #geq 2 btag)",  125, 0, 2500); 
   hjets["Njetsm1j5b2"] = new TH1F("Njetsm1j5b2"+hname,"Njets (1 #mu &  5 jets & #geq 2 btag)", 12,0.5,12.5); 
   hjets["Stjetm1j6b2"] = new TH1F("Stjetm1j6b2"+hname,"S_{T}^{jet} (1 #mu &  #geq 6 jets & #geq 2 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm1j6b2"] = new TH1F("DeltaRdijetm1j6b2"+hname,"#DeltaR(j,j) (1 #mu &  #geq 6 jets & #geq 2 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm1j6b2"] = new TH1F("DeltaPhiMETWlepm1j6b2"+hname,"#Delta #phi(MET,W(#mu,#nu)) (1 #mu &  #geq 6 jets & #geq 2 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm1j6b2"] = new TH1F("trijetmassm1j6b2"+hname,"top mass (1 #mu &  #geq 6 jets & #geq 2 btag)", 100, 0, 1000 ); 
   hjets["trijetptm1j6b2"] = new TH1F("trijetptm1j6b2"+hname,"top p_{T} (1 #mu &  #geq 6 jets & #geq 2 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm1j6b2"] = new TH1F("deltaRmindijetm1j6b2"+hname,"min #DeltaR(j,j) (1 #mu &  #geq 6 jets & #geq 2 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm1j6b2"] = new TH1F("deltaRmu2jetm1j6b2"+hname,"#DeltaR(#mu,2nd j) (1 #mu &  #geq 6 jets & #geq 2 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm1j6b2"] = new TH1F("deltaPhidijetm1j6b2"+hname,"#Delta #phi(j,j) (1 #mu &  #geq 6 jets & #geq 2 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm1j6b2"] = new TH1F("deltaPhimu2jetm1j6b2"+hname,"#Delta #phi(#mu,2nd j) (1 #mu &  #geq 6 jets & #geq 2 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm1j6b2"] = new TH1F("Stlepm1j6b2"+hname,"S_{t}^{lep} (1 #mu &  #geq 6 jets & #geq 2 btag)",  100, 0, 1000); 
   hjets["Htm1j6b2"] = new TH1F("Htm1j6b2"+hname,"H_{T} (1 #mu &  #geq 6 jets & #geq 2 btag)",  125, 0, 2500); 
   hjets["Njetsm1j6b2"] = new TH1F("Njetsm1j6b2"+hname,"Njets (1 #mu &  #geq 6 jets & #geq 2 btag)", 12,0.5,12.5); 

   hjets["Stjetm2j4b0"] = new TH1F("Stjetm2j4b0"+hname,"S_{T}^{jet} (2 #mu & 4 jets & 0 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm2j4b0"] = new TH1F("DeltaRdijetm2j4b0"+hname,"#DeltaR(j,j) (2 #mu & 4 jets & 0 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm2j4b0"] = new TH1F("DeltaPhiMETWlepm2j4b0"+hname,"#Delta #phi(MET,W(#mu,#nu)) (2 #mu & 4 jets & 0 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm2j4b0"] = new TH1F("trijetmassm2j4b0"+hname,"top mass (2 #mu & 4 jets & 0 btag)", 100, 0, 1000 ); 
   hjets["trijetptm2j4b0"] = new TH1F("trijetptm2j4b0"+hname,"top p_{T} (2 #mu & 4 jets & 0 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm2j4b0"] = new TH1F("deltaRmindijetm2j4b0"+hname,"min #DeltaR(j,j) (2 #mu & 4 jets & 0 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm2j4b0"] = new TH1F("deltaRmu2jetm2j4b0"+hname,"#DeltaR(#mu,2nd j) (2 #mu & 4 jets & 0 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm2j4b0"] = new TH1F("deltaPhidijetm2j4b0"+hname,"#Delta #phi(j,j) (2 #mu & 4 jets & 0 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm2j4b0"] = new TH1F("deltaPhimu2jetm2j4b0"+hname,"#Delta #phi(#mu,2nd j) (2 #mu & 4 jets & 0 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm2j4b0"] = new TH1F("Stlepm2j4b0"+hname,"S_{t}^{lep} (2 #mu & 4 jets & 0 btag)",  100, 0, 1000); 
   hjets["Htm2j4b0"] = new TH1F("Htm2j4b0"+hname,"H_{T} (2 #mu & 4 jets & 0 btag)",  125, 0, 2500); 
   hjets["Njetsm2j4b0"] = new TH1F("Njetsm2j4b0"+hname,"Njets (2 #mu & 4 jets & 0 btag)", 12,0.5,12.5); 
   hjets["Stjetm2j5b0"] = new TH1F("Stjetm2j5b0"+hname,"S_{T}^{jet} (2 #mu &  5 jets & 0 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm2j5b0"] = new TH1F("DeltaRdijetm2j5b0"+hname,"#DeltaR(j,j) (2 #mu &  5 jets & 0 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm2j5b0"] = new TH1F("DeltaPhiMETWlepm2j5b0"+hname,"#Delta #phi(MET,W(#mu,#nu)) (2 #mu &  5 jets & 0 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm2j5b0"] = new TH1F("trijetmassm2j5b0"+hname,"top mass (2 #mu &  5 jets & 0 btag)", 100, 0, 1000 ); 
   hjets["trijetptm2j5b0"] = new TH1F("trijetptm2j5b0"+hname,"top p_{T} (2 #mu &  5 jets & 0 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm2j5b0"] = new TH1F("deltaRmindijetm2j5b0"+hname,"min #DeltaR(j,j) (2 #mu &  5 jets & 0 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm2j5b0"] = new TH1F("deltaRmu2jetm2j5b0"+hname,"#DeltaR(#mu,2nd j) (2 #mu &  5 jets & 0 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm2j5b0"] = new TH1F("deltaPhidijetm2j5b0"+hname,"#Delta #phi(j,j) (2 #mu &  5 jets & 0 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm2j5b0"] = new TH1F("deltaPhimu2jetm2j5b0"+hname,"#Delta #phi(#mu,2nd j) (2 #mu &  5 jets & 0 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm2j5b0"] = new TH1F("Stlepm2j5b0"+hname,"S_{t}^{lep} (2 #mu &  5 jets & 0 btag)",  100, 0, 1000); 
   hjets["Htm2j5b0"] = new TH1F("Htm2j5b0"+hname,"H_{T} (2 #mu &  5 jets & 0 btag)",  125, 0, 2500); 
   hjets["Njetsm2j5b0"] = new TH1F("Njetsm2j5b0"+hname,"Njets (2 #mu &  5 jets & 0 btag)", 12,0.5,12.5); 
   hjets["Stjetm2j6b0"] = new TH1F("Stjetm2j6b0"+hname,"S_{T}^{jet} (2 #mu &  #geq 6 jets & 0 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm2j6b0"] = new TH1F("DeltaRdijetm2j6b0"+hname,"#DeltaR(j,j) (2 #mu &  #geq 6 jets & 0 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm2j6b0"] = new TH1F("DeltaPhiMETWlepm2j6b0"+hname,"#Delta #phi(MET,W(#mu,#nu)) (2 #mu &  #geq 6 jets & 0 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm2j6b0"] = new TH1F("trijetmassm2j6b0"+hname,"top mass (2 #mu &  #geq 6 jets & 0 btag)", 100, 0, 1000 ); 
   hjets["trijetptm2j6b0"] = new TH1F("trijetptm2j6b0"+hname,"top p_{T} (2 #mu &  #geq 6 jets & 0 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm2j6b0"] = new TH1F("deltaRmindijetm2j6b0"+hname,"min #DeltaR(j,j) (2 #mu &  #geq 6 jets & 0 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm2j6b0"] = new TH1F("deltaRmu2jetm2j6b0"+hname,"#DeltaR(#mu,2nd j) (2 #mu &  #geq 6 jets & 0 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm2j6b0"] = new TH1F("deltaPhidijetm2j6b0"+hname,"#Delta #phi(j,j) (2 #mu &  #geq 6 jets & 0 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm2j6b0"] = new TH1F("deltaPhimu2jetm2j6b0"+hname,"#Delta #phi(#mu,2nd j) (2 #mu &  #geq 6 jets & 0 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm2j6b0"] = new TH1F("Stlepm2j6b0"+hname,"S_{t}^{lep} (2 #mu &  #geq 6 jets & 0 btag)",  100, 0, 1000); 
   hjets["Htm2j6b0"] = new TH1F("Htm2j6b0"+hname,"H_{T} (2 #mu &  #geq 6 jets & 0 btag)",  125, 0, 2500); 
   hjets["Njetsm2j6b0"] = new TH1F("Njetsm2j6b0"+hname,"Njets (2 #mu &  #geq 6 jets & 0 btag)", 12,0.5,12.5); 


   hjets["Stjetm2j4b1"] = new TH1F("Stjetm2j4b1"+hname,"S_{T}^{jet} (2 #mu & 4 jets & 1 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm2j4b1"] = new TH1F("DeltaRdijetm2j4b1"+hname,"#DeltaR(j,j) (2 #mu & 4 jets & 1 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm2j4b1"] = new TH1F("DeltaPhiMETWlepm2j4b1"+hname,"#Delta #phi(MET,W(#mu,#nu)) (2 #mu & 4 jets & 1 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm2j4b1"] = new TH1F("trijetmassm2j4b1"+hname,"top mass (2 #mu & 4 jets & 1 btag)", 100, 0, 1000 ); 
   hjets["trijetptm2j4b1"] = new TH1F("trijetptm2j4b1"+hname,"top p_{T} (2 #mu & 4 jets & 1 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm2j4b1"] = new TH1F("deltaRmindijetm2j4b1"+hname,"min #DeltaR(j,j) (2 #mu & 4 jets & 1 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm2j4b1"] = new TH1F("deltaRmu2jetm2j4b1"+hname,"#DeltaR(#mu,2nd j) (2 #mu & 4 jets & 1 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm2j4b1"] = new TH1F("deltaPhidijetm2j4b1"+hname,"#Delta #phi(j,j) (2 #mu & 4 jets & 1 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm2j4b1"] = new TH1F("deltaPhimu2jetm2j4b1"+hname,"#Delta #phi(#mu,2nd j) (2 #mu & 4 jets & 1 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm2j4b1"] = new TH1F("Stlepm2j4b1"+hname,"S_{t}^{lep} (2 #mu & 4 jets & 1 btag)",  100, 0, 1000); 
   hjets["Htm2j4b1"] = new TH1F("Htm2j4b1"+hname,"H_{T} (2 #mu & 4 jets & 1 btag)",  125, 0, 2500); 
   hjets["Njetsm2j4b1"] = new TH1F("Njetsm2j4b1"+hname,"Njets (2 #mu & 4 jets & 1 btag)", 12,0.5,12.5); 
   hjets["Stjetm2j5b1"] = new TH1F("Stjetm2j5b1"+hname,"S_{T}^{jet} (2 #mu &  5 jets & 1 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm2j5b1"] = new TH1F("DeltaRdijetm2j5b1"+hname,"#DeltaR(j,j) (2 #mu &  5 jets & 1 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm2j5b1"] = new TH1F("DeltaPhiMETWlepm2j5b1"+hname,"#Delta #phi(MET,W(#mu,#nu)) (2 #mu &  5 jets & 1 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm2j5b1"] = new TH1F("trijetmassm2j5b1"+hname,"top mass (2 #mu &  5 jets & 1 btag)", 100, 0, 1000 ); 
   hjets["trijetptm2j5b1"] = new TH1F("trijetptm2j5b1"+hname,"top p_{T} (2 #mu &  5 jets & 1 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm2j5b1"] = new TH1F("deltaRmindijetm2j5b1"+hname,"min #DeltaR(j,j) (2 #mu &  5 jets & 1 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm2j5b1"] = new TH1F("deltaRmu2jetm2j5b1"+hname,"#DeltaR(#mu,2nd j) (2 #mu &  5 jets & 1 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm2j5b1"] = new TH1F("deltaPhidijetm2j5b1"+hname,"#Delta #phi(j,j) (2 #mu &  5 jets & 1 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm2j5b1"] = new TH1F("deltaPhimu2jetm2j5b1"+hname,"#Delta #phi(#mu,2nd j) (2 #mu &  5 jets & 1 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm2j5b1"] = new TH1F("Stlepm2j5b1"+hname,"S_{t}^{lep} (2 #mu &  5 jets & 1 btag)",  100, 0, 1000); 
   hjets["Htm2j5b1"] = new TH1F("Htm2j5b1"+hname,"H_{T} (2 #mu &  5 jets & 1 btag)",  125, 0, 2500); 
   hjets["Njetsm2j5b1"] = new TH1F("Njetsm2j5b1"+hname,"Njets (2 #mu &  5 jets & 1 btag)", 12,0.5,12.5); 
   hjets["Stjetm2j6b1"] = new TH1F("Stjetm2j6b1"+hname,"S_{T}^{jet} (2 #mu &  #geq 6 jets & 1 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm2j6b1"] = new TH1F("DeltaRdijetm2j6b1"+hname,"#DeltaR(j,j) (2 #mu &  #geq 6 jets & 1 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm2j6b1"] = new TH1F("DeltaPhiMETWlepm2j6b1"+hname,"#Delta #phi(MET,W(#mu,#nu)) (2 #mu &  #geq 6 jets & 1 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm2j6b1"] = new TH1F("trijetmassm2j6b1"+hname,"top mass (2 #mu &  #geq 6 jets & 1 btag)", 100, 0, 1000 ); 
   hjets["trijetptm2j6b1"] = new TH1F("trijetptm2j6b1"+hname,"top p_{T} (2 #mu &  #geq 6 jets & 1 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm2j6b1"] = new TH1F("deltaRmindijetm2j6b1"+hname,"min #DeltaR(j,j) (2 #mu &  #geq 6 jets & 1 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm2j6b1"] = new TH1F("deltaRmu2jetm2j6b1"+hname,"#DeltaR(#mu,2nd j) (2 #mu &  #geq 6 jets & 1 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm2j6b1"] = new TH1F("deltaPhidijetm2j6b1"+hname,"#Delta #phi(j,j) (2 #mu &  #geq 6 jets & 1 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm2j6b1"] = new TH1F("deltaPhimu2jetm2j6b1"+hname,"#Delta #phi(#mu,2nd j) (2 #mu &  #geq 6 jets & 1 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm2j6b1"] = new TH1F("Stlepm2j6b1"+hname,"S_{t}^{lep} (2 #mu &  #geq 6 jets & 1 btag)",  100, 0, 1000); 
   hjets["Htm2j6b1"] = new TH1F("Htm2j6b1"+hname,"H_{T} (2 #mu &  #geq 6 jets & 1 btag)",  125, 0, 2500); 
   hjets["Njetsm2j6b1"] = new TH1F("Njetsm2j6b1"+hname,"Njets (2 #mu &  #geq 6 jets & 1 btag)", 12,0.5,12.5); 

   hjets["Stjetm2j4b2"] = new TH1F("Stjetm2j4b2"+hname,"S_{T}^{jet} (2 #mu & 4 jets & #geq 2 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm2j4b2"] = new TH1F("DeltaRdijetm2j4b2"+hname,"#DeltaR(j,j) (2 #mu & 4 jets & #geq 2 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm2j4b2"] = new TH1F("DeltaPhiMETWlepm2j4b2"+hname,"#Delta #phi(MET,W(#mu,#nu)) (2 #mu & 4 jets & #geq 2 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm2j4b2"] = new TH1F("trijetmassm2j4b2"+hname,"top mass (2 #mu & 4 jets & #geq 2 btag)", 100, 0, 1000 ); 
   hjets["trijetptm2j4b2"] = new TH1F("trijetptm2j4b2"+hname,"top p_{T} (2 #mu & 4 jets & #geq 2 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm2j4b2"] = new TH1F("deltaRmindijetm2j4b2"+hname,"min #DeltaR(j,j) (2 #mu & 4 jets & #geq 2 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm2j4b2"] = new TH1F("deltaRmu2jetm2j4b2"+hname,"#DeltaR(#mu,2nd j) (2 #mu & 4 jets & #geq 2 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm2j4b2"] = new TH1F("deltaPhidijetm2j4b2"+hname,"#Delta #phi(j,j) (2 #mu & 4 jets & #geq 2 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm2j4b2"] = new TH1F("deltaPhimu2jetm2j4b2"+hname,"#Delta #phi(#mu,2nd j) (2 #mu & 4 jets & #geq 2 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm2j4b2"] = new TH1F("Stlepm2j4b2"+hname,"S_{t}^{lep} (2 #mu & 4 jets & #geq 2 btag)",  100, 0, 1000); 
   hjets["Htm2j4b2"] = new TH1F("Htm2j4b2"+hname,"H_{T} (2 #mu & 4 jets & #geq 2 btag)",  125, 0, 2500); 
   hjets["Njetsm2j4b2"] = new TH1F("Njetsm2j4b2"+hname,"Njets (2 #mu & 4 jets & #geq 2 btag)", 12,0.5,12.5); 
   hjets["Stjetm2j5b2"] = new TH1F("Stjetm2j5b2"+hname,"S_{T}^{jet} (2 #mu &  5 jets & #geq 2 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm2j5b2"] = new TH1F("DeltaRdijetm2j5b2"+hname,"#DeltaR(j,j) (2 #mu &  5 jets & #geq 2 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm2j5b2"] = new TH1F("DeltaPhiMETWlepm2j5b2"+hname,"#Delta #phi(MET,W(#mu,#nu)) (2 #mu &  5 jets & #geq 2 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm2j5b2"] = new TH1F("trijetmassm2j5b2"+hname,"top mass (2 #mu &  5 jets & #geq 2 btag)", 100, 0, 1000 ); 
   hjets["trijetptm2j5b2"] = new TH1F("trijetptm2j5b2"+hname,"top p_{T} (2 #mu &  5 jets & #geq 2 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm2j5b2"] = new TH1F("deltaRmindijetm2j5b2"+hname,"min #DeltaR(j,j) (2 #mu &  5 jets & #geq 2 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm2j5b2"] = new TH1F("deltaRmu2jetm2j5b2"+hname,"#DeltaR(#mu,2nd j) (2 #mu &  5 jets & #geq 2 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm2j5b2"] = new TH1F("deltaPhidijetm2j5b2"+hname,"#Delta #phi(j,j) (2 #mu &  5 jets & #geq 2 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm2j5b2"] = new TH1F("deltaPhimu2jetm2j5b2"+hname,"#Delta #phi(#mu,2nd j) (2 #mu &  5 jets & #geq 2 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm2j5b2"] = new TH1F("Stlepm2j5b2"+hname,"S_{t}^{lep} (2 #mu &  5 jets & #geq 2 btag)",  100, 0, 1000); 
   hjets["Htm2j5b2"] = new TH1F("Htm2j5b2"+hname,"H_{T} (2 #mu &  5 jets & #geq 2 btag)",  125, 0, 2500); 
   hjets["Njetsm2j5b2"] = new TH1F("Njetsm2j5b2"+hname,"Njets (2 #mu &  5 jets & #geq 2 btag)", 12,0.5,12.5); 
   hjets["Stjetm2j6b2"] = new TH1F("Stjetm2j6b2"+hname,"S_{T}^{jet} (2 #mu &  #geq 6 jets & #geq 2 btag)",70, 200, 3000 ); 
   hjets["DeltaRdijetm2j6b2"] = new TH1F("DeltaRdijetm2j6b2"+hname,"#DeltaR(j,j) (2 #mu &  #geq 6 jets & #geq 2 btag)",40,0.,4.); 
   hjets["DeltaPhiMETWlepm2j6b2"] = new TH1F("DeltaPhiMETWlepm2j6b2"+hname,"#Delta #phi(MET,W(#mu,#nu)) (2 #mu &  #geq 6 jets & #geq 2 btag)",32, -3.15, 3.15 ); 
   hjets["trijetmassm2j6b2"] = new TH1F("trijetmassm2j6b2"+hname,"top mass (2 #mu &  #geq 6 jets & #geq 2 btag)", 100, 0, 1000 ); 
   hjets["trijetptm2j6b2"] = new TH1F("trijetptm2j6b2"+hname,"top p_{T} (2 #mu &  #geq 6 jets & #geq 2 btag)",  50, 0, 500); 
   hjets["deltaRmindijetm2j6b2"] = new TH1F("deltaRmindijetm2j6b2"+hname,"min #DeltaR(j,j) (2 #mu &  #geq 6 jets & #geq 2 btag)", 40,0.,4.); 
   hjets["deltaRmu2jetm2j6b2"] = new TH1F("deltaRmu2jetm2j6b2"+hname,"#DeltaR(#mu,2nd j) (2 #mu &  #geq 6 jets & #geq 2 btag)",  40,0.,4.); 
   hjets["deltaPhidijetm2j6b2"] = new TH1F("deltaPhidijetm2j6b2"+hname,"#Delta #phi(j,j) (2 #mu &  #geq 6 jets & #geq 2 btag)", 32, -3.15, 3.15); 
   hjets["deltaPhimu2jetm2j6b2"] = new TH1F("deltaPhimu2jetm2j6b2"+hname,"#Delta #phi(#mu,2nd j) (2 #mu &  #geq 6 jets & #geq 2 btag)", 32, -3.15, 3.15); 
   hjets["Stlepm2j6b2"] = new TH1F("Stlepm2j6b2"+hname,"S_{t}^{lep} (2 #mu &  #geq 6 jets & #geq 2 btag)",  100, 0, 1000); 
   hjets["Htm2j6b2"] = new TH1F("Htm2j6b2"+hname,"H_{T} (2 #mu &  #geq 6 jets & #geq 2 btag)",  125, 0, 2500); 
   hjets["Njetsm2j6b2"] = new TH1F("Njetsm2j6b2"+hname,"Njets (2 #mu &  #geq 6 jets & #geq 2 btag)", 12,0.5,12.5); 

   hjets["Stjet41"] = new TH1F("Stjet41"+hname,"#geq 4 jets, Stjet < 1000", 60, 400, 1000 ); 
   hjets["Stjet51"] = new TH1F("Stjet51"+hname,"#geq 5 jets, Stjet < 1000", 60, 400, 1000 ); 
   hjets["Stjet61"] = new TH1F("Stjet61"+hname,"#geq 6 jets, Stjet < 1000", 60, 400, 1000 ); 
   hjets["Stjet42"] = new TH1F("Stjet42"+hname,"#geq 4 jets, Stjet > 1000", 50, 1000, 3000 ); 
   hjets["Stjet52"] = new TH1F("Stjet52"+hname,"#geq 5 jets, Stjet > 1000", 50, 1000, 3000 ); 
   hjets["Stjet62"] = new TH1F("Stjet62"+hname,"#geq 6 jets, Stjet > 1000", 50, 1000, 3000 ); 


   map<string,TH1* > allhistos = hmuons;
   allhistos.insert( helectrons.begin(), helectrons.end() );
   allhistos.insert( hMET.begin(), hMET.end() );
   allhistos.insert( hM.begin(), hM.end() );
   allhistos.insert( hjets.begin(), hjets.end() );

   for ( map<string,TH1* >::const_iterator imap=allhistos.begin(); imap!=allhistos.end(); ++imap )
     {
       TH1 *temp = imap->second;
       temp->Sumw2();
       temp->SetXTitle( temp->GetTitle() );
     }

   //------- Store information in a Tree
   MyStoreTree = new StoreTreeVariable();

   if(fChannel == 1){
      MyStoreTree->SetElectronFalse();
      //MyStoreTree->SetJetFalse();
      MyStoreTree->SetVertexFalse();
      MyStoreTree->SetTriggerFalse();
      //MyStoreTree->SetMetFalse();
      //MyStoreTree->SetMuonFalse();
   }

   MyStoreTree->InitialAll();
   //Get the Store Tree
   MyStoreTree->GetStoreTree()->SetDirectory(fFile);
   MyStoreTree->GetStoreTree()->AutoSave();
   //////////////////////////////////////////

   // cut flow
   if (fChannel==1) 
     { //muon +jets
       fCutLabels.push_back("Processed");
       fCutLabels.push_back("OneIsoMu");
       fCutLabels.push_back("LooseMuVeto");
       //fCutLabels.push_back("ZMassVeto");
       fCutLabels.push_back("ElectronVeto");
       fCutLabels.push_back("MET");
       fCutLabels.push_back("4Jet");
       fCutLabels.push_back("Ht");
       fCutLabels.push_back("4Jet0b");
       fCutLabels.push_back("4Jet1b");
       fCutLabels.push_back("4JetCut");
       fCutLabels.push_back("Stjet");
     }
   else
     { //electron+jets
     }
   hcutflow = new TH1D("cutflow","cut flow", fCutLabels.size(), 0.5, fCutLabels.size() +0.5 );

   for ( vector<string>::const_iterator ivec= fCutLabels.begin(); ivec!=fCutLabels.end(); ++ivec)
     {
       cutmap[ *ivec ] = 0;
     }


   if(fIsMC){
      LumiWeights_ = edm::Lumi3DReWeighting("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/PUMC_dist.root",
		      "/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/NewPileUp/MyCert_160404_180252_pileupTruth_finebin.root",
		      "pileup", 
		      "pileup",
		      "/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Weight_3Dnew.root");
      LumiWeights_.weight3D_init( 1.08 );
      LumiWeightsup_ = edm::Lumi3DReWeighting("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/PUMC_dist.root", 
		      "/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/NewPileUp/MyCert_160404_180252_pileupTruth_finebin.root",
		      "pileup",
		      "pileup",
		      "/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Weight_3Dup.root");
      LumiWeightsup_.weight3D_init( 1.167 );//up
      LumiWeightsdown_ = edm::Lumi3DReWeighting("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/PUMC_dist.root", 
		      "/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/NewPileUp/MyCert_160404_180252_pileupTruth_finebin.root",
		      "pileup", 
		      "pileup",
		      "/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Weight_3Ddown.root");
      LumiWeightsdown_.weight3D_init( 0.994 );//down
   }

   //btag efficiency file and 2D histogram
   TString btagefffilename = "/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/BtagEff.root";
   btagefffile = new TFile(btagefffilename,"read");
   //ttbar
   f2Dttbarbtag = (TH2D*) btagefffile->Get("ttbar_csv_btageff");
   f2Dttbarctag = (TH2D*) btagefffile->Get("ttbar_csv_ctageff");
   f2Dttbarlighttag = (TH2D*) btagefffile->Get("ttbar_csv_lighttageff");

   // For JEC uncertainties
   JetCorrectorParameters *jcp = new JetCorrectorParameters("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/JEC11_V10_AK5PF_UncertaintySources.txt","Total");
   if (fdoJECunc) fJECunc = new JetCorrectionUncertainty( *jcp );

   //Medium
   bSF_table.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVM_beff_SF.txt");
   bSF_table_error.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVM_beff_SF_Error.txt");
   lSF_table.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVM_lmistag_SF.txt");
   lSF_table_error_down.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVM_lmistag_SF_Error_Down.txt");
   lSF_table_error_up.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVM_lmistag_SF_Error_Up.txt");
   leff_table.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVM_lmistag_Eff.txt");

   //Loose
   bSF_table_loose.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVL_beff_SF.txt");
   bSF_table_error_loose.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVL_beff_SF_Error.txt");
   lSF_table_loose.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVL_lmistag_SF.txt");
   lSF_table_error_down_loose.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVL_lmistag_SF_Error_Down.txt");
   lSF_table_error_up_loose.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVL_lmistag_SF_Error_Up.txt");
   leff_table_loose.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVL_lmistag_Eff.txt");
   
   //Tight
   bSF_table_tight.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVT_beff_SF.txt");
   bSF_table_error_tight.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVT_beff_SF_Error.txt");
   lSF_table_tight.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVT_lmistag_SF.txt");
   lSF_table_error_down_tight.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVT_lmistag_SF_Error_Down.txt");
   lSF_table_error_up_tight.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVT_lmistag_SF_Error_Up.txt");
   leff_table_tight.LoadTable("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/Table_CSVT_lmistag_Eff.txt");
}

Bool_t Analyzer::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either Analyzer::GetEntry() or TBranch::GetEntry()
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

  //if ( entry % 100 == 0 )
  //cout<< "process entry " << entry << endl;

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
  vector< TLorentzVector > p4bjetscsvl;
  vector< TLorentzVector > p4bjetscsvm;
  vector< TLorentzVector > p4Othermuon;    // leading muon

  // FILTER OUT DATA events
  int testrun = ntuple->run; 
  if ( fSample =="data" && ( testrun == 176765 || testrun == 176771 || testrun == 176795 || testrun == 176796 || testrun == 176545 || testrun == 176547 || testrun == 176928 || testrun == 176982 ) ) return kTRUE;
  if ( fSample =="dataMissing" && ( testrun != 176765 && testrun != 176771 && testrun != 176795 && testrun != 176796 && testrun != 176545 && testrun != 176547 && testrun != 176928 && testrun != 176982 ) ) return kTRUE;


  ////////////////////
  // GENERATOR
  ///////////////////
  if (fIsMC && fSample.Contains("tttt") )
    {
      TLorentzVector p4genLepton;
      TLorentzVector p4genNu;
      TLorentzVector p4genb;
      if (ntuple->gen.bLep_pt>0)
	{
	  p4genLepton.SetPtEtaPhiE(ntuple->gen.mu_pt,ntuple->gen.mu_eta,ntuple->gen.mu_phi,ntuple->gen.mu_e);
	  p4genNu.SetPtEtaPhiE(ntuple->gen.nu_pt,ntuple->gen.nu_eta,ntuple->gen.nu_phi,ntuple->gen.nu_e);
	  p4genb.SetPtEtaPhiE(ntuple->gen.bLep_pt,ntuple->gen.bLep_eta,ntuple->gen.bLep_phi,ntuple->gen.bLep_e);
	  
	  hjets["gen_deltaR_mub"]->Fill( p4genLepton.DeltaR( p4genb ) ); 
	}
    }

   //////////Seperate the Wbb, Wcc and Wlight///////////////////////////
   //before the selection, We should seperate the samples
   //if(fSample.Contains("WJets"))
      if(fSample.Contains("Wbb") && !fSample.Contains("Wbbpartonflavor") )
      {
         int FH = ntuple->flavorHistory;
         if(FH != 1 && FH!= 2 && FH != 5 && FH != 7 && FH != 9)
            return kTRUE;
      }

      if(fSample.Contains("Wcc") && !fSample.Contains("Wccpartonflavor") )
      {
         int FH = ntuple->flavorHistory;
         if(FH != 3 && FH != 4 && FH !=6 && FH !=8 && FH != 10)
            return kTRUE;
      }

      if(fSample.Contains("Wlight") && !fSample.Contains("Wlightpartonflavor") )
      {
         int FH = ntuple->flavorHistory;
         //if(FH != 11) //Wlight = Wjet - Wcc - Wbb
         if(FH == 1 || FH == 2 || FH == 5 || FH == 7 || FH == 9 || FH == 3 || FH == 4 || FH ==6 || FH ==8 || FH == 10)
            return kTRUE;
      }

      int numberb = 0;
      int numberc = 0;
      int numberl = 0;

      for ( size_t ijet=0; ijet < total_jets; ++ijet)
      {
         TopJetEvent jet = jets[ijet];
         int listflavor = jet.mc.flavor; 
         if(abs(listflavor) == 5){numberb++;}
         if(abs(listflavor) == 4){numberc++;}
         if(abs(listflavor) == 1 || abs(listflavor) == 2 || abs(listflavor) == 3 || abs(listflavor) == 21){numberl++;}

      }

      //begin this we are using the jet parton flovar to determine the Wbb Wcc and Wlight
      if(fSample.Contains("Wlightpartonflavor"))
      {
         if((numberb > 0 || numberc > 0))
            return kTRUE;
      }

      if(fSample.Contains("Wccpartonflavor"))
      {
         if(numberc == 0 || numberb > 0 )
            return kTRUE;
      }

      if(fSample.Contains("Wbbpartonflavor"))
      {
         if(numberb == 0)
            return kTRUE;
      }

   //////////Seperate the Wbb, Wcc and Wlight///////////////////////////
  ////////////////////////////////////
  // PRIMARY VERTICES
  ///////////////////////////////////

  Int_t npvs = 0;
  for ( size_t ipv=0; ipv < total_pvs; ++ipv) 
    {
      if (ipv==0) PVz = primaryVertices[ipv].vz;
      if (primaryVertices[ipv].IsGood == 1) {
         npvs++;
      }
    }


   Double_t PUweight = 1.;

   if(fIsMC && !fSample.Contains("tttt")){

      Int_t mc_npvminus1 = ntuple->gen.Bx_minus1;
      Int_t mc_npv0      = ntuple->gen.Bx_0;
      Int_t mc_npvplus1  = ntuple->gen.Bx_plus1;

      PUweight = LumiWeights_.weight3D(mc_npvminus1,mc_npv0,mc_npvplus1);
      if (fpu_down) PUweight = LumiWeightsdown_.weight3D(mc_npvminus1,mc_npv0,mc_npvplus1);
      if (fpu_up) PUweight = LumiWeightsup_.weight3D(mc_npvminus1,mc_npv0,mc_npvplus1);
      //PUweight = 1.0;//This is just for test_4top
   }

   if(fIsMC && fSample.Contains("tttt")){

      Int_t mc_npvminus1 = ntuple->gen.Bx_0;
      Int_t mc_npv0      = ntuple->gen.Bx_0;
      Int_t mc_npvplus1  = ntuple->gen.Bx_0;

      PUweight = LumiWeights_.weight3D(mc_npvminus1,mc_npv0,mc_npvplus1);
      if (fpu_down) PUweight = LumiWeightsdown_.weight3D(mc_npvminus1,mc_npv0,mc_npvplus1);
      if (fpu_up) PUweight = LumiWeightsup_.weight3D(mc_npvminus1,mc_npv0,mc_npvplus1);
      //PUweight = 1.0;//This is just for test_4top
   }

  /////////////
  // HLT scale factor for MC
  ////////////
  double SF_hlt = 1.;
  if (fIsMC) SF_hlt = 0.97; //0.966;
  double SF_iso = 1.;
  if (fIsMC) SF_iso = 0.996;
  PUweight = PUweight*SF_hlt*SF_iso;  // LETS INCLUDE THE TRIGGER SF INTO THE PU WEIGHTS
  
  hPVs["Nreweight_cut0"]->Fill( total_pvs, PUweight );

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

  //bool IsZevent = false;

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
  
		if ( fMuSelector.MuonTight( muon, PVz) ) hmuons["charge_tiso"]->Fill( muon.charge, PUweight );
			//hmuons["N_tisomuons"]->Fill( nloosemuons );  
		if ( fMuSelector.MuonTightDeltaR( muon, PVz, jets) ) {
			ntightmuons++;
			deltaR = fMuSelector.GetDeltaR();
		}

		
		p4muon.SetPtEtaPhiE( muon.pt, muon.eta, muon.phi, muon.e );
		p4Othermuon.push_back( p4muon ); // for leading muon
		RelIso = muon.pfreliso; //muon.reliso03;
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
			/*helectrons["pt_cut2"]->Fill( p4ele.Pt(), PUweight );
			helectrons["eta_cut2"]->Fill( p4ele.Eta(), PUweight );
			helectrons["phi_cut2"]->Fill( p4ele.Phi(), PUweight );*/
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

		if ( ngoodIDmuons > 0 ) hmuons["Ngood"]->Fill( total_pvs, PUweight);
		if ( ntightmuons > 0 ) hmuons["Niso"]->Fill( total_pvs, PUweight);

		//if ( ntightmuons != 2 ) return kTRUE;
		if ( ntightmuons != 1 ) return kTRUE;
		//if ( ntightmuons < 1 ) return kTRUE;
		cutmap["OneIsoMu"] += PUweight;

		if ( nloosemuons > 1 ) return kTRUE;
		//if ( nloosemuons > 2 ) return kTRUE;
		cutmap["LooseMuVeto"] += PUweight;
		hmuons["N0"]->Fill( total_muons, PUweight );
	
		if ( nlooseelectrons > 0 ) return kTRUE;
		hmuons["Nelectrons_cut0"]->Fill( nlooseelectrons, PUweight  );
		cutmap["ElectronVeto"] += PUweight;
	
		/*
		TLorentzVector zboson = p4Othermuon[0] + p4Othermuon[1];
		double zbosonmass = zboson.M();
		if ( abs(zbosonmass - 91.2 ) < 10.0) IsZevent = true;
		if (IsZevent) return kTRUE;
		cutmap["ZMassVeto"] += PUweight;*/

		p4lepton = p4muon;
		if (fVerbose) cout << "got a good lepton" << endl;
	}

	
	hmuons["pt_cut0"]->Fill( p4lepton.Pt(), PUweight );
	hmuons["eta_cut0"]->Fill( p4lepton.Eta(), PUweight );
	hmuons["phi_cut0"]->Fill( p4lepton.Phi(), PUweight );
	hmuons["reliso"]->Fill( RelIso, PUweight );
	hmuons["deltaR_cut0"]->Fill( deltaR, PUweight ); 

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

  //if (fdoQCD1SideBand && p4MET.Et() > 20.) return kTRUE;
  //else if ( p4MET.Et() <= 20. && fdoQCD2SideBand==false ) return kTRUE;
//  if ( p4MET.Et() <= 20.) return kTRUE;
//  if (fVerbose) cout << "pass MET cut" << endl;


  //cutmap["MET"] += PUweight;
  hMET["MET_cut0"]->Fill( p4MET.Pt(), PUweight );
  hMET["phi_cut0"]->Fill( p4MET.Phi(), PUweight );


  double Wpt = p4lepton.Pt() + p4MET.Pt();
  double Wpx = p4lepton.Px() + p4MET.Px();
  double Wpy = p4lepton.Py() + p4MET.Py();
  double WMt = sqrt(Wpt*Wpt-Wpx*Wpx-Wpy*Wpy);
  hM["WMt_cut0"]->Fill( WMt, PUweight ); 
  

  /////////////////////////////////
  // JETS
  ////////////////////////////////
  
  //JetCombinatorics myCombi = JetCombinatoric();

  int njets = 0;
  MyStoreTree->GetJetVariable()->numjets = 0;
  MyStoreTree->GetJetVariable()->numBjets_csvl = 0;
  MyStoreTree->GetJetVariable()->numBjets_csvm = 0;
  MyStoreTree->GetJetVariable()->numBjets_csvt = 0; 
		
  map< string, vector<float> > bdisc;
  map< string, vector<bool> >  isTagb;
  map< string, vector<bool> >  isTagbUp;
  map< string, vector<bool> >  isTagbDown;
  vector<int> listflavor;

  Float_t metcorpx = p4MET.Px();
  Float_t metcorpy = p4MET.Py();
  Float_t totJERptadd = 0.;
  Float_t totJERptminus = 0;

  vector< TopJetEvent > tmp_corrjets;

  ////////////////////////////////
  /////  JES, JER
  //////////////////////////////
  for ( size_t ijet=0; ijet < total_jets; ++ijet)  {

	TopJetEvent jet = jets[ijet];
	double SF_JEC = 1.;

	//JER
	Float_t factor = 1.0;
	Float_t ptscale = 1.0;

	if (fdoJECunc){

		metcorpx = metcorpx + jet.uncorrpx;
		metcorpy = metcorpy + jet.uncorrpy;

		fJECunc->setJetEta( jet.eta);
		fJECunc->setJetPt( jet.pt);
		double jec_unc = 0.;
		if (fdoJECup)  jec_unc = fJECunc->getUncertainty(true);
		if (!fdoJECup) jec_unc = fJECunc->getUncertainty(false);
		if (fVerbose) cout << "JEC uncertainty is " << jec_unc << endl;
		if (fdoJECup) SF_JEC = 1.+jec_unc;
		else SF_JEC = 1.-jec_unc;

		metcorpx = metcorpx -  jet.uncorrpx * SF_JEC;
		metcorpy = metcorpy -  jet.uncorrpy * SF_JEC;
	}
    
	if(fIsMC && fdoJERunc) {
		if(fdoJERunc && !fdoJERup && !fdoJERdown){
			if(fabs(jet.eta) < 1.5) factor = 0.1;
			if(fabs(jet.eta) >= 1.5 && fabs(jet.eta) < 2.0) factor = 0.1;
			if(fabs(jet.eta) >= 2.0) factor = 0.1;
		}

		if(fdoJERunc && fdoJERup){
			if(fabs(jet.eta) < 1.5) factor = 0.2;
			if(fabs(jet.eta) >= 1.5 && fabs(jet.eta) < 2.0) factor = 0.25;
			if(fabs(jet.eta) >= 2.0) factor = 0.3;
		}

		if(fdoJERunc && fdoJERdown){
			if(fabs(jet.eta) < 1.5) factor = 0.0;
			if(fabs(jet.eta) >= 1.5 && fabs(jet.eta) < 2.0) factor = -0.05;
			if(fabs(jet.eta) >= 2.0) factor = -0.1;
		}

		//if (jet.mc.pt < 15) continue;
		if (jet.mc.pt >= 15 ) {
			metcorpx += jet.uncorrpx;
			metcorpy += jet.uncorrpy;
			totJERptadd +=  TMath::Sqrt(jet.uncorrpx*jet.uncorrpx + jet.uncorrpy*jet.uncorrpy);

			//if ( ntuple->run == 1 && ntuple->lumi == 4 && ntuple->event == 3611988 ) {
				//  TString outstring = "Add:";
				//  outstring += TString(Form("%f", TMath::Sqrt(jet.uncorrpx*jet.uncorrpx + jet.uncorrpy*jet.uncorrpy) ));
				//  Info("MET", outstring);
			//}

			Float_t deltapt = (jet.pt - jet.mc.pt) * factor;
			ptscale = TMath::Max( Float_t (0.) , (jet.pt + deltapt)/jet.pt );
	  
			metcorpx -=  jet.uncorrpx * ptscale;
			metcorpy -=  jet.uncorrpy * ptscale;

			totJERptminus -= ptscale*TMath::Sqrt(jet.uncorrpx*jet.uncorrpx + jet.uncorrpy*jet.uncorrpy);

			//if (ntuple->run == 1 && ntuple->lumi == 4 && ntuple->event == 3611988 ) {
				//  TString outstring = "Subtract:";
				//  outstring += TString(Form("%f", ptscale*TMath::Sqrt(jet.uncorrpx*jet.uncorrpx + jet.uncorrpy*jet.uncorrpy)  ));
				//  Info("MET", outstring);
			//}

		}
      
		if (jet.pt > 30 && fabs(jet.eta)< 2.4 ) {
			jet.pt = SF_JEC * ptscale * jet.pt;
			jet.e = SF_JEC * ptscale * jet.e;
		}
	}

	if (jet.pt > 30 && fabs(jet.eta)< 2.4 ) tmp_corrjets.push_back( jet );

  } // jets
  
 //p4MET.SetPtEtaPhiE( tmpmet + totJERptadd + totJERptminus, tmpeta, tmpphi, tmpmet + totJERptadd + totJERptminus ); //// previous way
  p4MET.SetPxPyPzE( metcorpx, metcorpy, 0, TMath::Sqrt((metcorpx * metcorpx ) + ( metcorpy * metcorpy)) );


  //if (ntuple->run == 1 && ntuple->lumi == 4 && ntuple->event == 3611988 ) {
  //  TString outstring ="MET after ";
  //  outstring += TString(Form("%f", p4MET.Pt() ));
  //  Info("MET", outstring);
  //  outstring = "tot add = "+TString(Form("%f", totJERptadd))+ " tot minus = "+TString(Form("%f", totJERptminus));
  //  Info("MET", outstring );
  //}
  //
  if ( p4MET.Et() <= 20.) return kTRUE;
  cutmap["MET"] += PUweight;
  if (fVerbose) cout << "pass MET cut" << endl;


  //////////////////////////////////////////////
  ////// Jets info
  /////////////////////////////////////////////

  jets = tmp_corrjets;
  sort( jets.begin(), jets.end(), mysortPt );
  
  for ( size_t ijet=0; ijet < jets.size() ; ++ijet) {

	TopJetEvent jet = jets[ijet];

	bool jetpass = jets.size()>1 && jet.pt > 40. && fabs(jet.eta) < 2.4 && jets[0].pt > 100. && jets[1].pt > 60.;

	if ( jetpass ) {
		
		//if (fVerbose) cout << " jet pt " << SF_JEC*jet.pt << endl;
	
		hjets["pt"]->Fill( jet.pt, PUweight );
		hjets["eta"]->Fill( jet.eta, PUweight );
		hjets["phi"]->Fill( jet.phi, PUweight );

		TLorentzVector tmpjet, tmpbjetcsvl, tmpbjetcsvm;
		tmpjet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e);
		p4jets.push_back( tmpjet);
		listflavor.push_back( jet.mc.flavor );

		if (fVerbose) {
			cout << "done storing njets " << njets << endl;
			cout << " bdisc " << jet.btag_TCHP << endl;
			cout << " bdisc " << jet.btag_CSV << endl;
		}

		// store discriminators
		bdisc["TCHP"].push_back( jet.btag_TCHP );
		bdisc["CSV"].push_back( jet.btag_CSV );
		if (fVerbose) cout << "store bdisc" << endl;
		if ( jet.btag_TCHP > 1.93 ) isTagb["TCHPM"].push_back(true);
		else isTagb["TCHPM"].push_back(false);
		if (fVerbose) cout << "done tchpl" << endl;
		if ( jet.btag_CSV > 0.679){ 
			isTagb["CSVM"].push_back(true); 
			tmpbjetcsvm.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e); 
			p4bjetscsvm.push_back(tmpbjetcsvm); 
			//p4bjetscsvm.push_back(tmpjet); 
		}
		else isTagb["CSVM"].push_back(false);
		if ( jet.btag_CSV > 0.679) isTagbUp["CSVM"].push_back(true);
		else isTagbUp["CSVM"].push_back(false);
		if ( jet.btag_CSV > 0.679) isTagbDown["CSVM"].push_back(true);
		else isTagbDown["CSVM"].push_back(false);
		//CSV Loose Tagger at 0.244
		if ( jet.btag_CSV > 0.244){
		       	isTagb["CSVL"].push_back(true);
			tmpbjetcsvl.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e); 
			p4bjetscsvl.push_back(tmpbjetcsvl); 
		}
		else isTagb["CSVL"].push_back(false);
		if ( jet.btag_CSV > 0.244) isTagbUp["CSVL"].push_back(true);
		else isTagbUp["CSVL"].push_back(false);
		if ( jet.btag_CSV > 0.244) isTagbDown["CSVL"].push_back(true);
		else isTagbDown["CSVL"].push_back(false);
		//CSV Tight Tagger at 0.898
		if ( jet.btag_CSV > 0.898) isTagb["CSVT"].push_back(true);
		else isTagb["CSVT"].push_back(false);
		if ( jet.btag_CSV > 0.898) isTagbUp["CSVT"].push_back(true);
		else isTagbUp["CSVT"].push_back(false);
		if ( jet.btag_CSV > 0.898) isTagbDown["CSVT"].push_back(true);
		else isTagbDown["CSVT"].push_back(false);
	
		if (fVerbose) cout << "done csv" << endl;
		
		njets++;
	}
  }


  /////////////////////////////////
  //     estimate Pz of neutrino
  ////////////////////////////////

  fzCalculator.SetMET(p4MET);
  fzCalculator.SetLepton(p4lepton);
  
  double pzNu = fzCalculator.Calculate();
  double pzOtherNu = fzCalculator.getOther();
  p4Nu.SetPxPyPzE(p4MET.Px(), p4MET.Py(), pzNu, sqrt(p4MET.Px()*p4MET.Px()+p4MET.Py()*p4MET.Py()+pzNu*pzNu));
  p4OtherNu.SetPxPyPzE( p4MET.Px(), p4MET.Py(),pzOtherNu,sqrt(p4MET.Px()*p4MET.Px()+p4MET.Py()*p4MET.Py()+pzOtherNu*pzOtherNu));
  
  if (fabs(pzNu) > fabs(pzOtherNu) ) {
	double tmppzNu = pzNu;
	double tmppzOtherNu = pzOtherNu;
	pzNu = tmppzOtherNu;
	pzOtherNu = tmppzNu;
  }

  //p4Nu = TLorentzVector();
  //p4OtherNu = TLorentzVector();


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
	if ( fabs( Wm1 - 80.4) < fabs( Wm2 - 80.4) ) p4Nu = p4Nu1tmp;
	else p4Nu = p4Nu2tmp;

	p4OtherNu = p4Nu; // since we chose the real part, the two solutions are the same.
  }


  hMET["PzNu"]->Fill(pzNu, PUweight ); //change this to 2d with two sol and as a function of jets

  TLorentzVector nup4[2];
  nup4[0] = p4Nu;
  nup4[1] = p4OtherNu;
                                                                                                                       

  //if ( fzCalculator.IsComplex() ) hM["lepW_massComplex"]->Fill( p4LepW.M(), PUweight );

  ///////////////////////////////////////
  //   SOME VARIABLES
  ///////////////////////////////////////

  double Ht = 0; 
  double Stlep = 0; 
  double Stjet = 0; 
  double Slep = 0;
  double deltaRminjj = 999.; 
  double deltaRjj = 999.; 
  double deltaRmu2j = 999.; 
  double deltaPhijj = 0.; 
  double deltaPhimu2j = 0.; 

  for (size_t mm=0; mm < p4Othermuon.size(); ++mm){
	Slep += p4Othermuon[mm].Pt();
	for ( size_t kkk=0; kkk < p4jets.size(); ++kkk) {
		deltaRmu2j = p4Othermuon[0].DeltaR(p4jets[1]);
		deltaPhimu2j = p4Othermuon[0].DeltaPhi(p4jets[1]);
	}
  }

  for ( size_t kkk=0; kkk < p4jets.size(); ++kkk) {
	// Ht calculation
	Ht += p4jets[kkk].Pt();

	// deltaR(jet,jet)
       	for ( vector< TopJetEvent>::iterator ijet=jets.begin(); ijet != jets.end(); ++ijet) {

		TopJetEvent jet = *ijet;
               	TLorentzVector tmpp4Jet;
                tmpp4Jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e );
       	        double tmpdeltaR = p4jets[kkk].DeltaR(tmpp4Jet);
		if ( tmpdeltaR < 0.3 ) continue;
               	if ( tmpdeltaR < deltaRminjj ) deltaRminjj = tmpdeltaR;
	}
  deltaRjj = p4jets[0].DeltaR(p4jets[1]);
  deltaPhijj = p4jets[0].DeltaPhi(p4jets[1]);

  }	

  //  St Calculation
  Stlep = p4MET.Pt() + Slep ;
  Stjet = Stlep + Ht ;

  // calculate dijet mass closest to W mass
	
  double dileadjet_mass = 0;
  double hadW_mass = 0;
  double Htop_mass = 0;
  double Leptop_mass = 0;
  double Leptop_pt = 0;
  double diWdeltaphi =0;
  double diWdeltaR =0;
  double DeltaPhiMETWlep = 0;
  double DeltaPhiMETlep = 0;
  double DeltaPhiNulep = 0;
  double sigma2 = 10.5*10.5;
  double sigma2top = 19.2 * 19.2;
  double tmpchi2 = 999999999.;
  double lepW_pt = 0;
  double lepW_mass = 0;
  TLorentzVector p4dileadjet, p4HadW, p4HadTop;

  if ( njets < 4 ) return kTRUE; 
  if ( njets > 3) {

  cutmap["4Jet"] += PUweight;


  	//////////////////////////////////
	//    Btagging
	/////////////////////////////////
	
	Double_t SFb0only = 1.0;
	Double_t SFb1only = 1.0;
	Double_t SFb_2btag = 1.0;// weight factor
	Double_t SFb_1btag = 1.0; // at least one btag TCHPM jet
	Double_t SFb_only1tag_syst[2] = {1.0,1.0};
	Double_t SFb_1tag_syst[2] = {1.0,1.0};
	Double_t SFb_2tag_syst[2] = {1.0,1.0};
	//For Btag Systematic error
	Int_t NbtagsUp_CSVM = 0;
	Int_t NbtagsDown_CSVM = 0;
	Int_t Nbtags_CSVM = 0;
	Int_t myNbtags_CSVM = 0;
	Int_t NbtagsUp_CSVL = 0;
	Int_t NbtagsDown_CSVL = 0;
	Int_t Nbtags_CSVL = 0;
	Int_t NbtagsUp_CSVT = 0;
	Int_t NbtagsDown_CSVT = 0;
	Int_t Nbtags_CSVT = 0;

	// count partons
	Int_t number_of_b = 0;
	Int_t number_of_c = 0;
	//Int_t number_of_l = 0;
	Int_t number_of_b_highpt = 0;
	Int_t number_of_c_highpt = 0;

	Int_t number_of_b_all = 0;
	Int_t number_of_c_all = 0;

	for ( size_t mm=0; mm< isTagb["TCHPM"].size(); ++mm ) {
		if(isTagb["CSVM"][mm]) {
			myNbtags_CSVM = myNbtags_CSVM + 1; //We need to store the btag jey number before applying the scale factor modified
		}
	}

	if(fIsMC){
		//use the new Btag method
		for(size_t kk = 0; kk < p4jets.size(); ++kk){


			//The official Twiki of Btag Efficiency and Scale factor: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG
			//For the CSV Btag efficiency: x is the CSV btag discriminator
			//effb = -1.73338329789*x*x*x*x +  1.26161794785*x*x*x +  0.784721653518*x*x +  -1.03328577451*x +  1.04305075822
			//effc = -1.5734604211*x*x*x*x +  1.52798999269*x*x*x +  0.866697059943*x*x +  -1.66657942274*x +  0.780639301724
			//Tight Tagger has been Commented
	
			//For the Medium
			float b_data_eff = 0.730;
			float c_data_eff = 0.192;

			//For the Loose
			float b_data_eff_loose = 0.850;
			float c_data_eff_loose = 0.442;

			//For the tight
			float b_data_eff_tight = 0.534;
			float c_data_eff_tight = 0.0663;
            
			//Medium
			float b_SF = bSF_table.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta())); 
			float b_SF_error = bSF_table_error.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float l_SF = lSF_table.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float l_SF_up = lSF_table_error_up.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float l_SF_down = lSF_table_error_down.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float l_data_eff = leff_table.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));

			//Loose
			float b_SF_loose = bSF_table_loose.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta())); 
			float b_SF_loose_error = bSF_table_error_loose.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float l_SF_loose = lSF_table_loose.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float l_SF_up_loose = lSF_table_error_up_loose.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float l_SF_down_loose = lSF_table_error_down_loose.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float l_data_eff_loose = leff_table_loose.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));

			//Tight
			float b_SF_tight = bSF_table_tight.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta())); 
			float b_SF_tight_error = bSF_table_error_tight.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float l_SF_tight = lSF_table_tight.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float l_SF_up_tight = lSF_table_error_up_tight.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float l_SF_down_tight = lSF_table_error_down_tight.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float l_data_eff_tight = leff_table_tight.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));

			Double_t ptmax = 670.;
			Double_t times = 2.0;

			if(p4jets[kk].Pt() > ptmax){
				   //btag scale factor error > Ptmax, two times uncerntainty, This has been considered into the Txt Files
				   //b_SF_error = times * b_SF_error;
				   //b_SF_loose_error = times * b_SF_loose_error;
				   //b_SF_tight_error = times * b_SF_tight_error;

				//mistag scale factor error
				l_SF_up = l_SF + times * (l_SF_up - l_SF);
				l_SF_down = l_SF - times * (l_SF - l_SF_down);

				//Loose
				l_SF_up_loose = l_SF_loose + times * (l_SF_up_loose - l_SF_loose);
				l_SF_down_loose = l_SF_loose - times * (l_SF_loose - l_SF_down_loose);

				//Tight
				l_SF_up_tight = l_SF_tight + times * (l_SF_up_tight - l_SF_tight);
				l_SF_down_tight = l_SF_tight - times * (l_SF_tight - l_SF_down_tight);
			}

		
			/*
			//From the twiki: https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFb-mujet_payload.txt 

			float b_SF = bSF_table_medium.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSVMedium"); 
			float b_SF_error = bSF_table_medium.GetError(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));

			float b_SF_loose = bSF_table_loose.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSVLoose"); 
			float b_SF_loose_error = bSF_table_loose.GetError(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));;

			float b_SF_tight = bSF_table_tight.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSVTight"); 
			float b_SF_tight_error = bSF_table_tight.GetError(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));;

			//for Mediem tagger
			float l_SF = leff_SF_table_medium.GetLightSFValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","M");
			float l_SF_up = leff_SF_table_medium.GetLightSFError(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","M","Up");
			float l_SF_down = leff_SF_table_medium.GetLightSFError(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","M","Down");
			float l_data_eff = leff_SF_table_medium.GetMistagValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","M");

			//for Loose tagger
			float l_SF_loose = leff_SF_table_loose.GetLightSFValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","L");
			float l_SF_up_loose = leff_SF_table_loose.GetLightSFError(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","L","Up");
			float l_SF_down_loose = leff_SF_table_loose.GetLightSFError(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","L","Down");
			float l_data_eff_loose = leff_SF_table_loose.GetMistagValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","L");


			//for Tight tagger
			float l_SF_tight = leff_SF_table_tight.GetLightSFValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","T");
			float l_SF_up_tight = leff_SF_table_tight.GetLightSFError(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","T","Up");
			float l_SF_down_tight = leff_SF_table_tight.GetLightSFError(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","T","Down");
			float l_data_eff_tight = leff_SF_table_tight.GetMistagValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","T");
			 */

			double seed = abs(static_cast<int>(sin(p4jets[kk].Phi()*1000000)*100000));
			BTagSFUtil btsfutil = BTagSFUtil( seed );

			//b jet
			if ( fIsMC && abs(listflavor[kk])==5 ){

				//Medium
				bool tmp_IsTag = isTagb["CSVM"][kk];
				isTagb["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag, abs(listflavor[kk]), b_SF, b_data_eff, l_SF, l_data_eff); 
	
				//Loose
				bool tmp_IsTag_loose = isTagb["CSVL"][kk];
				isTagb["CSVL"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_loose, abs(listflavor[kk]), b_SF_loose, b_data_eff_loose, l_SF, l_data_eff); 


				//Tight
				bool tmp_IsTag_tight = isTagb["CSVT"][kk];
				isTagb["CSVT"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_tight, abs(listflavor[kk]), b_SF_tight, b_data_eff_tight, l_SF, l_data_eff); 


				//the uncerntainty has been considered when getting the value
				tmp_IsTag = isTagbUp["CSVM"][kk];
				isTagbUp["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag , abs(listflavor[kk]), b_SF + b_SF_error, b_data_eff, l_SF, l_data_eff);
				tmp_IsTag = isTagbDown["CSVM"][kk];
				isTagbDown["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag , abs(listflavor[kk]), b_SF - b_SF_error, b_data_eff, l_SF, l_data_eff);
	
				tmp_IsTag_loose = isTagbUp["CSVL"][kk];
				isTagbUp["CSVL"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_loose, abs(listflavor[kk]), b_SF_loose + b_SF_loose_error, b_data_eff_loose, l_SF, l_data_eff);
				tmp_IsTag_loose = isTagbDown["CSVL"][kk];
				isTagbDown["CSVL"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_loose, abs(listflavor[kk]), b_SF_loose - b_SF_loose_error, b_data_eff_loose, l_SF, l_data_eff);


				tmp_IsTag_tight = isTagbUp["CSVT"][kk];
				isTagbUp["CSVT"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_tight, abs(listflavor[kk]), b_SF_tight + b_SF_tight_error, b_data_eff_tight, l_SF, l_data_eff);
				tmp_IsTag_tight = isTagbDown["CSVT"][kk];
				isTagbDown["CSVT"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_tight, abs(listflavor[kk]), b_SF_tight - b_SF_tight_error, b_data_eff_tight, l_SF, l_data_eff);
			}

			if ( fIsMC && abs(listflavor[kk])==4 ) {

				bool tmp_IsTag = isTagb["CSVM"][kk];
				isTagb["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag, abs(listflavor[kk]), b_SF, c_data_eff,l_SF, l_data_eff  );
	
				//For the Loose
				bool tmp_IsTag_loose = isTagb["CSVL"][kk];
				isTagb["CSVL"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_loose, abs(listflavor[kk]), b_SF_loose, c_data_eff_loose,l_SF, l_data_eff  );
	
				//For the Tight 
				bool tmp_IsTag_tight = isTagb["CSVT"][kk];
				isTagb["CSVT"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_tight, abs(listflavor[kk]), b_SF_tight, c_data_eff_tight,l_SF, l_data_eff  );


				//use uncerntainty on the cjet, two times of B btag error
				tmp_IsTag = isTagbUp["CSVM"][kk];
				isTagbUp["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag, abs(listflavor[kk]), b_SF + 2.0 * b_SF_error , c_data_eff, l_SF, l_data_eff);
				tmp_IsTag = isTagbDown["CSVM"][kk];
				isTagbDown["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag, abs(listflavor[kk]), b_SF - 2.0 * b_SF_error, c_data_eff, l_SF, l_data_eff);

				//use uncerntainty on the cjet
				tmp_IsTag_loose = isTagbUp["CSVL"][kk];
				isTagbUp["CSVL"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_loose, abs(listflavor[kk]), b_SF_loose + 2.0 * b_SF_loose_error, c_data_eff_loose, l_SF, l_data_eff);
				tmp_IsTag_loose = isTagbDown["CSVL"][kk];
				isTagbDown["CSVL"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_loose, abs(listflavor[kk]), b_SF_loose - 2.0 * b_SF_loose_error, c_data_eff_loose, l_SF, l_data_eff);


				//use uncerntainty on the cjet
				tmp_IsTag_tight = isTagbUp["CSVT"][kk];
				isTagbUp["CSVT"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_tight, abs(listflavor[kk]), b_SF_tight + 2.0 * b_SF_tight_error, c_data_eff_tight, l_SF, l_data_eff);
				tmp_IsTag_tight = isTagbDown["CSVT"][kk];
				isTagbDown["CSVT"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_tight, abs(listflavor[kk]), b_SF_tight - 2.0 * b_SF_tight_error, c_data_eff_tight, l_SF, l_data_eff);
			}

			if ( fIsMC && (abs(listflavor[kk])==1 || abs(listflavor[kk])==2 || abs(listflavor[kk])==3 || abs(listflavor[kk])==21 )){

				bool tmp_IsTag = isTagb["CSVM"][kk];
				isTagb["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag, abs(listflavor[kk]), b_SF, b_data_eff, l_SF, l_data_eff);

				//for loose
				bool tmp_IsTag_loose = isTagb["CSVL"][kk];
				isTagb["CSVL"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_loose, abs(listflavor[kk]), b_SF_loose, b_data_eff_loose, l_SF_loose, l_data_eff_loose);

				//for tight
				bool tmp_IsTag_tight = isTagb["CSVT"][kk];
				isTagb["CSVT"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_tight, abs(listflavor[kk]), b_SF_tight, b_data_eff_tight, l_SF_tight, l_data_eff_tight);

	
				tmp_IsTag = isTagbUp["CSVM"][kk];
				isTagbUp["CSVM"][kk] = btsfutil.modifyBTagsWithSF(  tmp_IsTag, abs(listflavor[kk]), b_SF, b_data_eff, l_SF_up , l_data_eff);
				tmp_IsTag = isTagbDown["CSVM"][kk];
				isTagbDown["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag, abs(listflavor[kk]), b_SF, b_data_eff, l_SF_down, l_data_eff);

				//for Loose
				tmp_IsTag_loose = isTagbUp["CSVL"][kk];
				isTagbUp["CSVL"][kk] = btsfutil.modifyBTagsWithSF(  tmp_IsTag_loose  , abs(listflavor[kk]), b_SF_loose, b_data_eff_loose, l_SF_up_loose, l_data_eff_loose);
				tmp_IsTag_loose = isTagbDown["CSVL"][kk];
				isTagbDown["CSVL"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_loose, abs(listflavor[kk]), b_SF_loose, b_data_eff_loose, l_SF_down_loose, l_data_eff_loose);


				//for tight 
				tmp_IsTag_tight = isTagbUp["CSVT"][kk];
				isTagbUp["CSVT"][kk] = btsfutil.modifyBTagsWithSF(  tmp_IsTag_tight  , abs(listflavor[kk]), b_SF_tight, b_data_eff_tight, l_SF_up_tight, l_data_eff_tight);
				tmp_IsTag_tight = isTagbDown["CSVT"][kk];
				isTagbDown["CSVT"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag_tight, abs(listflavor[kk]), b_SF_tight, b_data_eff_tight, l_SF_down_tight, l_data_eff_tight);
			}
		}
	}


	if( fIsMC ){
		//zero btag
		BTagWeight b0(0,0); // number of tags 1 to 3

		//Using the Pt and Eta dependent Btag efficiency
		vector<BTagWeight::JetInfo> j;
		//for systematic Btag
		vector<BTagWeight::JetInfo> jj;//UP
		vector<BTagWeight::JetInfo> jk;//DOWN

		//New Recommendtion from the Btag Group
		for(size_t kk = 0; kk < p4jets.size(); ++kk){
			//Still Support for the Medium
			//For the Mediem
			float b_data_eff = 0.730;
			float c_data_eff = 0.192;

			float b_SF1 = bSF_table.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float b_SF_error1 = bSF_table_error.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));

			float l_SF1 = lSF_table.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float l_SF_up1 = lSF_table_error_up.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float l_SF_down1 = lSF_table_error_down.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
			float l_data_eff1  = leff_table.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));

			Double_t ptmax = 670.;

			if(p4jets[kk].Pt() > ptmax){
				//btag scale factor error > Ptmax, two times uncerntainty
				b_SF_error1 = 2.0 * b_SF_error1;

				//mistag scale factor error
				l_SF_up1 = l_SF1 + 2.0 * (l_SF_up1 - l_SF1);
				l_SF_down1 = l_SF1 - 2.0 * (l_SF1 - l_SF_down1);
			}

			/*//From the twiki: https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFb-mujet_payload.txt 
			float b_SF1 = bSF_table_medium.GetValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSVMedium"); 
			float b_SF_error1 = bSF_table_medium.GetError(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()));
	
			//for Mediem tagger
			float l_SF1 = leff_SF_table_medium.GetLightSFValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","M");
			float l_SF_up1 = leff_SF_table_medium.GetLightSFError(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","M","Up");
			float l_SF_down1 = leff_SF_table_medium.GetLightSFError(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","M","Down");
			float l_data_eff1 = leff_SF_table_medium.GetMistagValue(p4jets[kk].Pt(),fabs(p4jets[kk].Eta()),"CSV","M");
			 */

			if(abs(listflavor[kk]) == 5) {  
				Double_t tageff = b_data_eff; 
				Double_t b_SF = b_SF1;
				Double_t b_SF_error = b_SF_error1;
	
				BTagWeight::JetInfo bjlow(tageff,b_SF);
				BTagWeight::JetInfo bjlowUP(tageff,b_SF + b_SF_error);
				BTagWeight::JetInfo bjlowDOWN(tageff,b_SF - b_SF_error);
				j.push_back(bjlow);
				jj.push_back(bjlowUP);
				jk.push_back(bjlowDOWN);
			}

			if(abs(listflavor[kk]) == 4){
				Double_t tageff = c_data_eff; 
				Double_t b_SF = b_SF1;
				Double_t b_SF_error = b_SF_error1;

				BTagWeight::JetInfo cjlow(tageff,b_SF);
				BTagWeight::JetInfo cjlowUP(tageff,b_SF + 2.0 * b_SF_error);
				BTagWeight::JetInfo cjlowDOWN(tageff,b_SF - 2.0 * b_SF_error);
				j.push_back(cjlow);
				jj.push_back(cjlowUP);
				jk.push_back(cjlowDOWN);
			}

			if(abs(listflavor[kk]) == 1 || abs(listflavor[kk]) == 2 || abs(listflavor[kk]) == 3 || abs(listflavor[kk]) == 21) {
				Double_t tageff = l_data_eff1;
				Double_t l_SF = l_SF1;
				Double_t l_SF_up = l_SF_up1;
				Double_t l_SF_down = l_SF_down1;

				BTagWeight::JetInfo lightj(tageff,l_SF);
				BTagWeight::JetInfo lightjUP(tageff,l_SF_up);
				BTagWeight::JetInfo lightjDOWN(tageff,l_SF_down);

				j.push_back(lightj);
				jj.push_back(lightjUP);
				jk.push_back(lightjDOWN);
			}
		}//LOOP of Jet

		//changed from TCHPM to CSVM
		if(myNbtags_CSVM == 0){
			SFb0only = b0.weight(j,0);
		}

		if(myNbtags_CSVM == 1){
			BTagWeight b11(1,1);
			SFb1only = b11.weight(j,1);

			SFb_only1tag_syst[0] = b11.weight(jj,1); //UP
			SFb_only1tag_syst[1] = b11.weight(jk,1);//DOWN
		}

		if(myNbtags_CSVM >= 1){
			BTagWeight b1(1,Nbtags_CSVM); 
			SFb_1btag = b1.weight(j,1);

			SFb_1tag_syst[0] = b1.weight(jj,1); //UP
			SFb_1tag_syst[1] = b1.weight(jk,1);//DOWN
       		 }

		if(myNbtags_CSVM >= 2) {
			BTagWeight b2(2,myNbtags_CSVM);
			SFb_2btag = b2.weight(j,2);

			SFb_2tag_syst[0] = b2.weight(jj,2); //UP
			SFb_2tag_syst[1] = b2.weight(jk,2);//DOWN
		}
	}//MC sample

	number_of_b_all = number_of_b + number_of_b_highpt;
	number_of_c_all = number_of_c + number_of_c_highpt;

	// count number of b-tags
	Int_t Nbtags_TCHPM = 0;

	//This number of btag jet has been applied the scale factor, so be careful!!!!!!!!!!!!!!!!!!
	for ( size_t itag=0; itag< isTagb["TCHPM"].size(); ++itag ){
		if ( isTagb["TCHPM"][itag] )  Nbtags_TCHPM++;
		if ( isTagb["CSVM"][itag] ) Nbtags_CSVM++;
		if ( isTagb["CSVL"][itag] ) Nbtags_CSVL++;
		if ( isTagb["CSVT"][itag] ) Nbtags_CSVT++;
		if ( isTagbUp["CSVL"][itag] ) NbtagsUp_CSVL++;
		if ( isTagbDown["CSVL"][itag] ) NbtagsDown_CSVL++;
		if ( isTagbUp["CSVT"][itag] ) NbtagsUp_CSVT++;
		if ( isTagbDown["CSVT"][itag] ) NbtagsDown_CSVT++;
		if ( isTagbUp["CSVM"][itag] ) NbtagsUp_CSVM++;
		if ( isTagbDown["CSVM"][itag] ) NbtagsDown_CSVM++;
	}///////////////////////////////////////////////////////////////// end btaging

  p4dileadjet = p4jets[0] + p4jets[1];
  dileadjet_mass = p4dileadjet.M();

  //calculate cloest di jet mass which is the cloest to W mass 
  Int_t bestjetTopindex[3] = {-1,-1,-1};

         for ( Int_t iijet = 0; iijet < (njets - 2); ++iijet ) {
            for ( Int_t jjjet = iijet + 1; jjjet < njets - 1; ++jjjet ) {
               for(Int_t kkjet = iijet + 2; kkjet < njets; ++kkjet){
                  TLorentzVector tmpvv = p4jets[iijet] + p4jets[jjjet];
                  Double_t tmpmass = tmpvv.M();
                  TLorentzVector tmptop = p4jets[iijet] + p4jets[jjjet] + p4jets[kkjet];
                  Double_t tmptopmass = tmptop.M();
                  Double_t reconstructionchi2 = TMath::Abs(tmpmass - 80.4) * TMath::Abs(tmpmass - 80.4)/sigma2 + TMath::Abs(tmptopmass - 172.5) * TMath::Abs(tmptopmass - 172.5)/sigma2top;
                  if( tmpchi2 > reconstructionchi2){
                     tmpchi2 = reconstructionchi2;
                     bestjetTopindex[0] = iijet;
                     bestjetTopindex[1] = jjjet;
                     bestjetTopindex[2] = kkjet;
                  }
               }
            }
         }

  p4HadW = p4jets[bestjetTopindex[0]] + p4jets[bestjetTopindex[1]];
  p4HadTop = p4jets[bestjetTopindex[0]] + p4jets[bestjetTopindex[1]] + p4jets[bestjetTopindex[2]];
  hadW_mass = p4HadW.M();
  Htop_mass = p4HadTop.M();


  // Leptonic Top
  Int_t index[2] = {-1, -1}; // best index combination cloest to the Top mass
  Double_t mindeltamass = 100000000.;
  vector<Double_t> topmasscombination;
  topmasscombination.clear();

  for(Int_t l = 0; l < 2; l++){
	  for(Int_t m = 0; m < njets; m++){   
		  TLorentzVector sum;
		  sum = p4lepton + nup4[l] + p4jets[m];

		  topmasscombination.push_back(sum.M());

		  if (mindeltamass > TMath::Abs(sum.M()- 172.5)) {   
			  mindeltamass = TMath::Abs(sum.M()- 172.5);
			  Leptop_mass = sum.M();
			  Leptop_pt = sum.Pt();
			  index[0] = l;
			  index[1] = m;
		  }
	  }
  }


  TLorentzVector p4LepW = p4lepton + nup4[index[0]];

  TLorentzVector hadwbosonp4 = p4jets[bestjetTopindex[0]] + p4jets[bestjetTopindex[1]];
  diWdeltaphi = p4LepW.DeltaPhi(hadwbosonp4);
  diWdeltaR = p4LepW.DeltaR(hadwbosonp4);
  DeltaPhiMETWlep = p4LepW.DeltaPhi(p4MET);
  DeltaPhiMETlep = p4lepton.DeltaPhi(p4MET);
  DeltaPhiNulep = p4lepton.DeltaPhi(p4Nu);

  lepW_pt = p4LepW.Pt();
  lepW_mass = p4LepW.M();



  	hMET["Ht_cut0"]->Fill( Ht, PUweight );
	hMET["Stlep_cut0"]->Fill( Stlep , PUweight );
	hMET["Stjet_cut0"]->Fill( Stjet , PUweight );
	hM["dileadjet_cut0"]->Fill( dileadjet_mass, PUweight );
	hM["hadW_mass_cut0"]->Fill( hadW_mass, PUweight );
	hM["top_mass_cut0"]->Fill( Htop_mass, PUweight );
	hM["top_pt_cut0"]->Fill( p4HadTop.Pt(), PUweight );
  	hjets["1st_pt_cut0"]->Fill( p4jets[0].Pt(), PUweight );
	hjets["1st_eta_cut0"]->Fill( p4jets[0].Eta(), PUweight );
	hjets["2nd_pt_cut0"]->Fill( p4jets[1].Pt(), PUweight );
	hjets["2nd_eta_cut0"]->Fill( p4jets[1].Eta(), PUweight );
	hjets["3rd_pt_cut0"]->Fill( p4jets[2].Pt(), PUweight );
	hjets["4th_pt_cut0"]->Fill( p4jets[3].Pt(), PUweight );
	hjets["5th_pt_cut0"]->Fill( p4jets[4].Pt(), PUweight );
	hjets["6th_pt_cut0"]->Fill( p4jets[5].Pt(), PUweight );
	hjets["7th_pt_cut0"]->Fill( p4jets[6].Pt(), PUweight );
	hjets["Njets_cut0"]->Fill( njets, PUweight ); 
	hjets["Nbtags_CSVL_cut0"]->Fill( Nbtags_CSVL, PUweight ); 
	hjets["Nbtags_CSVM_cut0"]->Fill( Nbtags_CSVM, PUweight ); 
	hjets["Nbtags_CSVT_cut0"]->Fill( Nbtags_CSVT, PUweight ); 
	hjets["Nbtags_TCHPM_cut0"]->Fill( Nbtags_TCHPM, PUweight ); 
	hjets["deltaPhijj_cut0"]->Fill( deltaPhijj, PUweight );

	if ( total_muons == 1){
		if ( Nbtags_CSVL == 0 ){
			if (njets == 4){
				hjets["Stjetm1j4b0"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm1j4b0"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm1j4b0"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm1j4b0"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm1j4b0"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm1j4b0"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm1j4b0"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm1j4b0"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm1j4b0"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm1j4b0"]->Fill( Stlep, PUweight );
				hjets["Htm1j4b0"]->Fill( Ht, PUweight );
				hjets["Njetsm1j4b0"]->Fill( njets, PUweight );
			}
			if (njets == 5){
				hjets["Stjetm1j5b0"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm1j5b0"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm1j5b0"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm1j5b0"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm1j5b0"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm1j5b0"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm1j5b0"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm1j5b0"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm1j5b0"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm1j5b0"]->Fill( Stlep, PUweight );
				hjets["Htm1j5b0"]->Fill( Ht, PUweight );
				hjets["Njetsm1j5b0"]->Fill( njets, PUweight );
			}
			if (njets >= 6){
				hjets["Stjetm1j6b0"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm1j6b0"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm1j6b0"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm1j6b0"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm1j6b0"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm1j6b0"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm1j6b0"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm1j6b0"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm1j6b0"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm1j6b0"]->Fill( Stlep, PUweight );
				hjets["Htm1j6b0"]->Fill( Ht, PUweight );
				hjets["Njetsm1j6b0"]->Fill( njets, PUweight );
			}
		}

		if ( Nbtags_CSVL == 1 ){
			if (njets == 4){
				hjets["Stjetm1j4b1"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm1j4b1"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm1j4b1"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm1j4b1"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm1j4b1"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm1j4b1"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm1j4b1"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm1j4b1"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm1j4b1"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm1j4b1"]->Fill( Stlep, PUweight );
				hjets["Htm1j4b1"]->Fill( Ht, PUweight );
				hjets["Njetsm1j4b1"]->Fill( njets, PUweight );
			}
			if (njets == 5){
				hjets["Stjetm1j5b1"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm1j5b1"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm1j5b1"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm1j5b1"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm1j5b1"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm1j5b1"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm1j5b1"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm1j5b1"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm1j5b1"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm1j5b1"]->Fill( Stlep, PUweight );
				hjets["Htm1j5b1"]->Fill( Ht, PUweight );
				hjets["Njetsm1j5b1"]->Fill( njets, PUweight );
			}
			if (njets >= 6){
				hjets["Stjetm1j6b1"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm1j6b1"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm1j6b1"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm1j6b1"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm1j6b1"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm1j6b1"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm1j6b1"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm1j6b1"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm1j6b1"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm1j6b1"]->Fill( Stlep, PUweight );
				hjets["Htm1j6b1"]->Fill( Ht, PUweight );
				hjets["Njetsm1j6b1"]->Fill( njets, PUweight );
			}
		}

		if ( Nbtags_CSVL >= 2 ){
			if (njets == 4){
				hjets["Stjetm1j4b2"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm1j4b2"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm1j4b2"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm1j4b2"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm1j4b2"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm1j4b2"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm1j4b2"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm1j4b2"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm1j4b2"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm1j4b2"]->Fill( Stlep, PUweight );
				hjets["Htm1j4b2"]->Fill( Ht, PUweight );
				hjets["Njetsm1j4b2"]->Fill( njets, PUweight );
			}
			if (njets == 5){
				hjets["Stjetm1j5b2"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm1j5b2"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm1j5b2"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm1j5b2"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm1j5b2"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm1j5b2"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm1j5b2"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm1j5b2"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm1j5b2"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm1j5b2"]->Fill( Stlep, PUweight );
				hjets["Htm1j5b2"]->Fill( Ht, PUweight );
				hjets["Njetsm1j5b2"]->Fill( njets, PUweight );
			}
			if (njets >= 6){
				hjets["Stjetm1j6b2"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm1j6b2"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm1j6b2"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm1j6b2"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm1j6b2"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm1j6b2"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm1j6b2"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm1j6b2"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm1j6b2"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm1j6b2"]->Fill( Stlep, PUweight );
				hjets["Htm1j6b2"]->Fill( Ht, PUweight );
				hjets["Njetsm1j6b2"]->Fill( njets, PUweight );
			}
		}

	}
		
	if ( total_muons == 2){
		if ( Nbtags_CSVL == 0 ){
			if (njets == 4){
				hjets["Stjetm2j4b0"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm2j4b0"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm2j4b0"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm2j4b0"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm2j4b0"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm2j4b0"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm2j4b0"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm2j4b0"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm2j4b0"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm2j4b0"]->Fill( Stlep, PUweight );
				hjets["Htm2j4b0"]->Fill( Ht, PUweight );
				hjets["Njetsm2j4b0"]->Fill( njets, PUweight );
			}
			if (njets == 5){
				hjets["Stjetm2j5b0"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm2j5b0"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm2j5b0"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm2j5b0"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm2j5b0"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm2j5b0"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm2j5b0"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm2j5b0"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm2j5b0"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm2j5b0"]->Fill( Stlep, PUweight );
				hjets["Htm2j5b0"]->Fill( Ht, PUweight );
				hjets["Njetsm2j5b0"]->Fill( njets, PUweight );
			}
			if (njets >= 6){
				hjets["Stjetm2j6b0"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm2j6b0"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm2j6b0"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm2j6b0"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm2j6b0"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm2j6b0"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm2j6b0"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm2j6b0"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm2j6b0"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm2j6b0"]->Fill( Stlep, PUweight );
				hjets["Htm2j6b0"]->Fill( Ht, PUweight );
				hjets["Njetsm2j6b0"]->Fill( njets, PUweight );
			}
		}

		if ( Nbtags_CSVL == 1 ){
			if (njets == 4){
				hjets["Stjetm2j4b1"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm2j4b1"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm2j4b1"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm2j4b1"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm2j4b1"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm2j4b1"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm2j4b1"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm2j4b1"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm2j4b1"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm2j4b1"]->Fill( Stlep, PUweight );
				hjets["Htm2j4b1"]->Fill( Ht, PUweight );
				hjets["Njetsm2j4b1"]->Fill( njets, PUweight );
			}
			if (njets == 5){
				hjets["Stjetm2j5b1"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm2j5b1"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm2j5b1"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm2j5b1"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm2j5b1"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm2j5b1"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm2j5b1"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm2j5b1"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm2j5b1"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm2j5b1"]->Fill( Stlep, PUweight );
				hjets["Htm2j5b1"]->Fill( Ht, PUweight );
				hjets["Njetsm2j5b1"]->Fill( njets, PUweight );
			}
			if (njets >= 6){
				hjets["Stjetm2j6b1"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm2j6b1"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm2j6b1"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm2j6b1"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm2j6b1"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm2j6b1"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm2j6b1"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm2j6b1"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm2j6b1"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm2j6b1"]->Fill( Stlep, PUweight );
				hjets["Htm2j6b1"]->Fill( Ht, PUweight );
				hjets["Njetsm2j6b1"]->Fill( njets, PUweight );
			}
		}

		if ( Nbtags_CSVL >= 2 ){
			if (njets == 4){
				hjets["Stjetm2j4b2"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm2j4b2"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm2j4b2"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm2j4b2"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm2j4b2"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm2j4b2"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm2j4b2"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm2j4b2"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm2j4b2"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm2j4b2"]->Fill( Stlep, PUweight );
				hjets["Htm2j4b2"]->Fill( Ht, PUweight );
				hjets["Njetsm2j4b2"]->Fill( njets, PUweight );
			}
			if (njets == 5){
				hjets["Stjetm2j5b2"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm2j5b2"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm2j5b2"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm2j5b2"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm2j5b2"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm2j5b2"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm2j5b2"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm2j5b2"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm2j5b2"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm2j5b2"]->Fill( Stlep, PUweight );
				hjets["Htm2j5b2"]->Fill( Ht, PUweight );
				hjets["Njetsm2j5b2"]->Fill( njets, PUweight );
			}
			if (njets >= 6){
				hjets["Stjetm2j6b2"]->Fill( Stjet , PUweight );
				hjets["DeltaRdijetm2j6b2"]->Fill( deltaRjj , PUweight );
				hjets["DeltaPhiMETWlepm2j6b2"]->Fill( DeltaPhiMETWlep , PUweight );
				hjets["trijetmassm2j6b2"]->Fill( Htop_mass , PUweight );
				hjets["trijetptm2j6b2"]->Fill( p4HadTop.Pt() , PUweight );
				hjets["deltaRmindijetm2j6b2"]->Fill( deltaRminjj , PUweight );
				hjets["deltaRmu2jetm2j6b2"]->Fill( deltaRmu2j , PUweight );
				hjets["deltaPhidijetm2j6b2"]->Fill( deltaPhijj , PUweight );
				hjets["deltaPhimu2jetm2j6b2"]->Fill( deltaPhimu2j , PUweight );
				hjets["Stlepm2j6b2"]->Fill( Stlep, PUweight );
				hjets["Htm2j6b2"]->Fill( Ht, PUweight );
				hjets["Njetsm2j6b2"]->Fill( njets, PUweight );
			}
		}

	}

	hjets["Stjet41"]->Fill( Stjet , PUweight );
	hjets["Stjet42"]->Fill( Stjet , PUweight );
 	if (njets > 4){
		hjets["Stjet51"]->Fill( Stjet , PUweight );
		hjets["Stjet52"]->Fill( Stjet , PUweight );
	}
 	if (njets > 5){
		hjets["Stjet61"]->Fill( Stjet , PUweight );
		hjets["Stjet62"]->Fill( Stjet , PUweight );
	}
	//   Cuts
	//bool passcut = true;
	if ( Ht < 300. ) return kTRUE;

	if ( Ht >= 300.0 ) {
		cutmap["Ht"] += PUweight;
		if(Nbtags_CSVM == 4){
		       	cout <<"Run : "<< ntuple->run <<", Lumi : "<< ntuple->lumi << ", Event: "<< ntuple->event << endl;
			for (size_t h = 0; h < p4jets.size(); ++h){
				TopJetEvent jet = jets[h];
				cout << h << " " << p4jets[h].Pt() << " " << jet.btag_CSV << endl;
			}
		}


		/*double jet1CSVMpt = 0;
		double jet2CSVMpt = 0;
		double jet3CSVMpt = 0;
		double jet4CSVMpt = 0;
		double jet1CSVLpt = 0;
		double jet2CSVLpt = 0;
		double jet3CSVLpt = 0;
		double jet4CSVLpt = 0;*/

		//if ( Nbtags_CSVM < 1 ) return kTRUE;

		/*if ( Nbtags_CSVL > 0 ){ jet1CSVLpt = p4bjetscsvl[0].Pt(); hjets["jet1_CSVL_pt"]->Fill ( jet1CSVLpt, PUweight ); }
		if ( Nbtags_CSVL > 1 ){ jet2CSVLpt = p4bjetscsvl[1].Pt(); hjets["jet2_CSVL_pt"]->Fill ( jet2CSVLpt, PUweight ); }
		if ( Nbtags_CSVL > 2 ){ jet3CSVLpt = p4bjetscsvl[2].Pt(); hjets["jet3_CSVL_pt"]->Fill ( jet3CSVLpt, PUweight ); }
		if ( Nbtags_CSVL > 3 ){ jet4CSVLpt = p4bjetscsvl[3].Pt(); hjets["jet4_CSVL_pt"]->Fill ( jet4CSVLpt, PUweight ); }
		if ( Nbtags_CSVM > 0 ){ jet1CSVMpt = p4bjetscsvm[0].Pt(); hjets["jet1_CSVM_pt"]->Fill ( jet1CSVMpt, PUweight ); }
		if ( Nbtags_CSVM > 1 ){ jet2CSVMpt = p4bjetscsvm[1].Pt(); hjets["jet2_CSVM_pt"]->Fill ( jet2CSVMpt, PUweight ); }
		if ( Nbtags_CSVM > 2 ){ jet3CSVMpt = p4bjetscsvl[2].Pt(); hjets["jet3_CSVM_pt"]->Fill ( jet3CSVMpt, PUweight ); }
		if ( Nbtags_CSVM > 3 ){ jet4CSVMpt = p4bjetscsvl[3].Pt(); hjets["jet4_CSVM_pt"]->Fill ( jet4CSVMpt, PUweight ); } */
		
		
		if ( !fbtag_up && fbtag_down && NbtagsDown_CSVL < 1 ){ cutmap["4Jet0b"] += PUweight; return kTRUE;}
		if ( !fbtag_up && !fbtag_down && Nbtags_CSVL < 1 ){ cutmap["4Jet0b"] += PUweight; return kTRUE;}
		if ( fbtag_up && !fbtag_down && NbtagsUp_CSVL < 1 ){ cutmap["4Jet0b"] += PUweight; return kTRUE;}
		if ( Nbtags_CSVL > 0){ 
			cutmap["4Jet1b"] += PUweight;

			//double jet1CSVLpt = p4bjetscsvl[0].Pt(); 
			//hjets["jet1_CSVL_pt"]->Fill ( jet1CSVLpt, PUweight ); 

			hPVs["Nreweight"]->Fill( total_pvs, PUweight );
			hMET["Ht"]->Fill( Ht, PUweight );
			hMET["MET"]->Fill( p4MET.Pt(), PUweight );
	  		hMET["phi"]->Fill( p4MET.Phi(), PUweight );
			hMET["Stlep"]->Fill( Stlep , PUweight );
			hMET["Stjet"]->Fill( Stjet , PUweight );
			hjets["Njets"]->Fill( njets, PUweight ); 
			hjets["1st_pt"]->Fill( p4jets[0].Pt(), PUweight );
			hjets["1st_eta"]->Fill( p4jets[0].Eta(), PUweight );
			hjets["2nd_pt"]->Fill( p4jets[1].Pt(), PUweight );
			hjets["2nd_eta"]->Fill( p4jets[1].Eta(), PUweight );
			hjets["3rd_pt"]->Fill( p4jets[2].Pt(), PUweight );
			hjets["3rd_eta"]->Fill( p4jets[2].Eta(), PUweight );
			hjets["4th_pt"]->Fill( p4jets[3].Pt(), PUweight );
			hjets["4th_eta"]->Fill( p4jets[3].Eta(), PUweight );
			hjets["5th_pt"]->Fill( p4jets[4].Pt(), PUweight );
			hjets["6th_pt"]->Fill( p4jets[5].Pt(), PUweight );
			hjets["7th_pt"]->Fill( p4jets[6].Pt(), PUweight );
			hjets["mindeltaRjj"]->Fill( deltaRminjj, PUweight );
			hjets["deltaRjj"]->Fill( deltaRjj, PUweight );
			hjets["deltaPhijj"]->Fill( deltaPhijj, PUweight );
			hjets["deltaRmu2j"]->Fill( deltaRmu2j, PUweight );
			hjets["deltaPhimu2j"]->Fill( deltaPhimu2j, PUweight );
			hmuons["N"]->Fill( total_muons, PUweight );
			hmuons["pt"]->Fill( p4lepton.Pt(), PUweight );
			hmuons["eta"]->Fill( p4lepton.Eta(), PUweight );
			hmuons["phi"]->Fill( p4lepton.Phi(), PUweight );
			hmuons["deltaR"]->Fill( deltaR, PUweight );
			hM["WMt"]->Fill( WMt, PUweight ); 
	
			// B-tagging
			hjets["Nbtags_CSVL"]->Fill( Nbtags_CSVL, PUweight ); 
			hjets["Nbtags_CSVM"]->Fill( Nbtags_CSVM, PUweight ); 
			hjets["Nbtags_CSVT"]->Fill( Nbtags_CSVT, PUweight ); 
			hjets["Nbtags_TCHPM"]->Fill( Nbtags_TCHPM, PUweight ); 


			hM["dileadjet"]->Fill( dileadjet_mass, PUweight );
			hM["hadW_mass"]->Fill( hadW_mass, PUweight );
			hM["top_mass"]->Fill( Htop_mass, PUweight );
			hM["Leptop_mass"]->Fill( Leptop_mass, PUweight );
			hM["top_pt"]->Fill( p4HadTop.Pt(), PUweight );
			hM["Leptop_pt"]->Fill( Leptop_pt, PUweight );
			hM["chi2"]->Fill( tmpchi2, PUweight );
			hM["lepW_mass"]->Fill(lepW_mass, PUweight );
			hM["diWdeltaphi"]->Fill( diWdeltaphi , PUweight );
			hM["diWdeltaR"]->Fill( diWdeltaR , PUweight );
			hM["lepW_pt"]->Fill( lepW_pt, PUweight );
			hMET["deltaPhiMETWlep"]->Fill( DeltaPhiMETWlep , PUweight );
			hMET["deltaPhiMETlep"]->Fill( DeltaPhiMETlep , PUweight );
			hMET["deltaPhiNulep"]->Fill( DeltaPhiNulep , PUweight );

			if ( p4jets[3].Pt() > 50.){ 
				cutmap["4JetCut"] += PUweight;
				if ( Stjet >= 500 ) {
					cutmap["Stjet"] += PUweight;
					hPVs["Nreweight_cut2"]->Fill( total_pvs, PUweight );
					hMET["Ht_cut2"]->Fill( Ht, PUweight );
					hMET["MET_cut2"]->Fill( p4MET.Pt(), PUweight );
					hMET["Stlep_cut2"]->Fill( Stlep , PUweight );
					hMET["Stjet_cut2"]->Fill( Stjet , PUweight );
 					hM["lepW_mass_cut2"]->Fill(p4LepW.M(), PUweight );
 					hMET["deltaPhiMETWlep_cut2"]->Fill( DeltaPhiMETWlep , PUweight );
 					hMET["deltaPhiMETlep_cut2"]->Fill( DeltaPhiMETlep , PUweight );
 					hMET["deltaPhiNulep_cut2"]->Fill( DeltaPhiNulep , PUweight );
					hjets["Njets_cut2"]->Fill(njets, PUweight );
					hjets["1st_pt_cut2"]->Fill( p4jets[0].Pt(), PUweight );
					hjets["1st_eta_cut2"]->Fill( p4jets[0].Eta(), PUweight );
					hjets["2nd_pt_cut2"]->Fill( p4jets[1].Pt(), PUweight );
					hjets["2nd_eta_cut2"]->Fill( p4jets[1].Eta(), PUweight );
					hjets["3rd_pt_cut2"]->Fill( p4jets[2].Pt(), PUweight );
					hjets["4th_pt_cut2"]->Fill( p4jets[3].Pt(), PUweight );
					hjets["5th_pt_cut2"]->Fill( p4jets[4].Pt(), PUweight );
					hjets["6th_pt_cut2"]->Fill( p4jets[5].Pt(), PUweight );
					hjets["deltaRjj_cut2"]->Fill( deltaRminjj, PUweight );
					hmuons["pt_cut2"]->Fill( p4lepton.Pt(), PUweight );
					hmuons["deltaR_cut2"]->Fill( deltaR, PUweight );
					hM["WMt_cut2"]->Fill( WMt, PUweight ); 
 					hM["dileadjet_cut2"]->Fill( dileadjet_mass, PUweight );
 					hM["hadW_mass_cut2"]->Fill( hadW_mass, PUweight );
 					hM["top_mass_cut2"]->Fill( Htop_mass, PUweight );
 					hM["top_pt_cut2"]->Fill( p4HadTop.Pt(), PUweight );
				}  // Stjet cut
			}  // 4 leading jet

  		//if ( njets < 4 ) return kTRUE; 
		// Variables in 4Tree
		MyStoreTree->GetGeneralVariable()->PUWeight = PUweight;
		MyStoreTree->GetMuonVariable()->Muonpt = p4lepton.Pt();
		MyStoreTree->GetMuonVariable()->Muoneta = p4lepton.Eta();
		MyStoreTree->GetMuonVariable()->Muonphi = p4lepton.Phi();
		MyStoreTree->GetMuonVariable()->MuondeltaR = deltaR;
		MyStoreTree->GetMetVariable()->Ht = Ht;
		MyStoreTree->GetMetVariable()->MET = p4MET.Pt();
		MyStoreTree->GetMetVariable()->METphi = p4MET.Phi();
		MyStoreTree->GetMetVariable()->Stlep = Stlep;
		MyStoreTree->GetMetVariable()->Stjet = Stjet;
		MyStoreTree->GetMetVariable()->diWdeltaphi = diWdeltaphi;
		MyStoreTree->GetMetVariable()->diWdeltaR = diWdeltaR;
		MyStoreTree->GetMetVariable()->DeltaPhiMETWlep = DeltaPhiMETWlep;
		MyStoreTree->GetMetVariable()->DeltaPhiMETlep = DeltaPhiMETlep;
		MyStoreTree->GetMetVariable()->DeltaPhiNulep = DeltaPhiNulep;
		MyStoreTree->GetMetVariable()->LepWPt = lepW_pt;
		MyStoreTree->GetMetVariable()->topmass = Htop_mass;
		MyStoreTree->GetMetVariable()->toppt = p4HadTop.Pt();
                MyStoreTree->GetJetVariable()->numjets= njets;
                MyStoreTree->GetJetVariable()->numBjets_csvl= Nbtags_CSVL;
                MyStoreTree->GetJetVariable()->numBjets_csvm= Nbtags_CSVM;
                MyStoreTree->GetJetVariable()->numBjets_csvt= Nbtags_CSVT;
                MyStoreTree->GetJetVariable()->jet1pt = p4jets[0].Pt() ;
                MyStoreTree->GetJetVariable()->jet2pt = p4jets[1].Pt() ;
                MyStoreTree->GetJetVariable()->jet3pt = p4jets[2].Pt() ;
                MyStoreTree->GetJetVariable()->jet4pt = p4jets[3].Pt() ;
                MyStoreTree->GetJetVariable()->deltaRmindijet = deltaRminjj ;
                MyStoreTree->GetJetVariable()->deltaRdijet = deltaRjj ;
                MyStoreTree->GetJetVariable()->deltaPhidijet = deltaPhijj ;
                MyStoreTree->GetJetVariable()->deltaRmu2jet = deltaRmu2j ;
                MyStoreTree->GetJetVariable()->deltaPhimu2jet = deltaPhimu2j ;
                //MyStoreTree->GetJetVariable()->jet1CSVLpt = jet1CSVLpt ;
                /*if (p4bjetscsvl[1].Pt() < 1000 && p4bjetscsvl[1].Pt() > 40 ) MyStoreTree->GetJetVariable()->jet2CSVLpt = p4bjetscsvl[1].Pt() ;
                if (p4bjetscsvl[2].Pt() < 1000 && p4bjetscsvl[2].Pt() > 40 ) MyStoreTree->GetJetVariable()->jet3CSVLpt = p4bjetscsvl[2].Pt() ;
                if (p4bjetscsvl[3].Pt() < 1000 && p4bjetscsvl[3].Pt() > 40 ) MyStoreTree->GetJetVariable()->jet4CSVLpt = p4bjetscsvl[3].Pt() ;
                if (p4bjetscsvm[0].Pt() < 1000 && p4bjetscsvm[0].Pt() > 40 ) MyStoreTree->GetJetVariable()->jet1CSVMpt = p4bjetscsvm[0].Pt() ;
                if (p4bjetscsvm[1].Pt() < 1000 && p4bjetscsvm[1].Pt() > 40 ) MyStoreTree->GetJetVariable()->jet2CSVMpt = p4bjetscsvm[1].Pt() ;
                if (p4bjetscsvm[2].Pt() < 1000 && p4bjetscsvm[2].Pt() > 40 ) MyStoreTree->GetJetVariable()->jet3CSVMpt = p4bjetscsvm[2].Pt() ;
                if (p4bjetscsvm[3].Pt() < 1000 && p4bjetscsvm[3].Pt() > 40 ) MyStoreTree->GetJetVariable()->jet4CSVMpt = p4bjetscsvm[3].Pt() ;*/
		//////////////////////////////////////////////////////////////////////////////////
		
		}  // 1 btag */
	}  // Ht
  }  // jets

   MyStoreTree->GetGeneralVariable()->Run = ntuple->run;
   MyStoreTree->GetGeneralVariable()->Lumi = ntuple->lumi;
   MyStoreTree->GetGeneralVariable()->Event = ntuple->event;

   MyStoreTree->GetStoreTree()->Fill();

  if (fVerbose) cout << "done analysis" << endl;
  return kTRUE;
}

void Analyzer::SlaveTerminate()
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
	if(MyStoreTree->GetStoreTree()->GetEntries() > 0){
		fFile->cd();
		h1test->Write();
		hcutflow->Write();
		MyStoreTree->GetStoreTree()->Write();
		//h2_pt_Wprime->Write();
		fFile->mkdir("muons");
		fFile->cd("muons");
		for ( map<string,TH1* >::const_iterator imap=hmuons.begin(); imap!=hmuons.end(); ++imap ) {
			TH1 *temp = imap->second;
			if ( temp->GetEntries() > 0 )
			temp->Write();
			//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
		}
		fFile->cd();
		fFile->mkdir("PVs");
		fFile->cd("PVs");
		for ( map<string,TH1* >::const_iterator imap=hPVs.begin(); imap!=hPVs.end(); ++imap ) {
			TH1 *temp = imap->second;
			if ( temp->GetEntries() > 0 )
			temp->Write();
			//else cout << "Warning: empty histogram " << temp->GetName() << " will not be written to file." << endl;
		}
		fFile->cd();
		fFile->mkdir("jets");
		fFile->cd("jets");
		for ( map<string,TH1* >::const_iterator imap=hjets.begin(); imap!=hjets.end(); ++imap ) {
			TH1 *temp = imap->second;
			if ( temp->GetEntries() > 0 )
			temp->Write();
		}
		fFile->cd();
		fFile->mkdir("mass");
		fFile->cd("mass");
		for ( map<string,TH1* >::const_iterator imap=hM.begin(); imap!=hM.end(); ++imap ) {
			TH1 *temp = imap->second;
			if ( temp->GetEntries() > 0 )
			temp->Write();
		}
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
    MyStoreTree->GetStoreTree()->SetDirectory(0);
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

void Analyzer::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

  Info("Terminate","Analyzer done.");
}
