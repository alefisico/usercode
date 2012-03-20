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
      if (fdoJERunc && fdoJERup==true) fSample += "_JERUP";
      if (fdoJERunc && fdoJERdown==true) fSample += "_JERDOWN";

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
      TString dir = "file:/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/results17/";
      //TString dir = "file:/uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/";
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
   
   hmuons["N_cut0"] = new TH1F("N_muons_cut0"+hname,"Number of Muons",6,-0.5,5.5);
   hmuons["N"] = new TH1F("N_muons"+hname,"Number of Muons",6,-0.5,5.5);
   hmuons["N_1btag"] = new TH1F("N_muons_1btag"+hname,"Number of Muons",6,-0.5,5.5);
   hmuons["Nelectrons_cut0"] = new TH1F("Nelectrons_cut0"+hname,"Number of Loose Electrons",6,-0.5,5.5);
   hmuons["Nelectrons"] = new TH1F("Nelectrons"+hname,"Number of Loose Electrons",6,-0.5,5.5);
   hmuons["Nelectrons_1btag"] = new TH1F("Nelectrons_1btag"+hname,"Number of Loose Electrons",6,-0.5,5.5);
   hmuons["pt_1jet"] = new TH1F("muon_pt_1jet"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["pt_2jet"] = new TH1F("muon_pt_2jet"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["pt_fin"] = new TH1F("muon_pt_fin"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["eta"] = new TH1F("muon_eta"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["eta_1jet"] = new TH1F("muon_eta_1jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["eta_2jet"] = new TH1F("muon_eta_2jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["eta_fin"] = new TH1F("muon_eta_fin"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   hmuons["phi"] = new TH1F("muon_phi"+hname,"#phi^{#mu}", 30, -3.15, 3.15);
   hmuons["phi_1jet"] = new TH1F("muon_phi_1jet"+hname,"#phi^{#mu}", 30, -3.15, 3.15);
   hmuons["phi_2jet"] = new TH1F("muon_phi_2jet"+hname,"#phi^{#mu}", 30, -3.15, 3.15);
   hmuons["phi_fin"] = new TH1F("muon_phi_fin"+hname,"#phi^{#mu}", 30, -3.15, 3.15);
   hmuons["reliso"] = new TH1F("muon_reliso"+hname,"Relative Isolation", 40, 0, 0.2);
   hmuons["reliso_fin"] = new TH1F("muon_reliso_fin"+hname,"Relative Isolation", 40, 0, 0.2);
   hmuons["deltaR_cut0"] = new TH1F("deltaR_cut0"+hname,"#DeltaR(#mu,jet)",80, 0, 4);
   hmuons["deltaR"] = new TH1F("deltaR"+hname,"#DeltaR(#mu,jet)", 80, 0, 4);
   hmuons["deltaR_1btag"] = new TH1F("deltaR_1btag"+hname,"#DeltaR(#mu,jet)", 80, 0, 4);
   hmuons["d0_cut1"] = new TH1F("d0_cut1"+hname,"#mu Impact Parameter [cm]",20,-0.1,0.1);
   hmuons["pt_cut1"] = new TH1F("muon_pt_cut1"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["pt_cut2"] = new TH1F("muon_pt_cut2"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["pt_cut3"] = new TH1F("muon_pt_cut3"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["pt"] = new TH1F("muon_pt"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["pt_1btag"] = new TH1F("muon_pt_1btag"+hname,"p_{T}^{#mu} [GeV/c]", 50, 0, 500);
   hmuons["dz"] = new TH1F("dz"+hname,"|z(#mu) - z_{PV}| [cm]", 25, 0, 1.);
   hmuons["Niso"] = new TH1F("Niso"+hname,"Number of Primary Vertices", 25,-0.5,24.5);
   hmuons["Ngood"] = new TH1F("Ngood"+hname,"Number of Primary Vertices",25,-0.5,24.5);
   hmuons["charge_tiso"] = new TH1F("charge_tiso"+hname,"#mu Charge", 5,-2.5,2.5);

#
   hPVs["N"] = new TH1F("NPV"+hname,"Number of PVs",25,-0.5,24.5);
   hPVs["Nreweight"] = new TH1F("NPVreweight"+hname,"Number of PVs",25,-0.5,24.5);
   hPVs["Nreweight_2jet"] = new TH1F("NPVreweight_2jet"+hname,"Number of PVs",25,-0.5,24.5);
   hPVs["Nreweight_2jet_1btag"] = new TH1F("NPVreweight_2jet_1btag"+hname,"Number of PVs",25,-0.5,24.5);
      
   hMET["MET"] = new TH1F("MET"+hname,"Missing Transverse Energy [GeV]", 50, 0, 500);
   hMET["MET_1btag"] = new TH1F("MET_1btag"+hname,"Missing Transverse Energy [GeV]", 50, 0, 500);
   hMET["MET_cut0"] = new TH1F("MET_cut0"+hname,"Missing Transverse Energy [GeV]", 50, 0, 300);
   hMET["MET_2jet"] = new TH1F("MET_2jet"+hname,"Missing Transverse Energy [GeV]", 50, 0, 300);
   hMET["MET_fin"] = new TH1F("MET_fin"+hname,"Missing Transverse Energy [GeV]", 50, 0, 300);
   hMET["genMET_2jet"] = new TH1F("genMET_2jet"+hname,"Missing Transverse Energy [GeV]", 50, 0, 300);
   hMET["deltaMET_2jet"] = new TH1F("deltaMET_2jet"+hname,"Missing Transverse Energy [GeV]", 50, -200, 200);
   hMET["phi"] = new TH1F("MET_phi"+hname,"#phi Missing Transverse Energy [GeV]", 20, 0, 3.15);
   hMET["Ht0"] = new TH1F("Ht0"+hname,"H_{T} [GeV]", 60, 0, 3000);
   hMET["Ht"] = new TH1F("Ht"+hname,"H_{T} [GeV]", 56, 200, 3000);
   hMET["Ht_1btag"] = new TH1F("Ht_1btag"+hname,"H_{T} [GeV]", 56, 200, 3000);
   hMET["Htlep"] = new TH1F("Htlep"+hname,"H_{T,lep} [GeV]", 100, 0, 1000);
   hMET["Stlep"] = new TH1F("Stlep"+hname,"S_{t}^{lep} [GeV]", 100, 0, 1000);
   hMET["Stlep_1btag"] = new TH1F("Stlep_1btag"+hname,"S_{t}^{lep} [GeV]", 100, 0, 1000);
   hMET["Stjet"] = new TH1F("Stjet"+hname,"S_{t}^{jet} [GeV]", 56, 200, 3000);
   hMET["Stjet_1btag"] = new TH1F("Stjet_1btag"+hname,"S_{t}^{jet} [GeV]", 56, 200, 3000);
   hMET["PzNu"] = new TH1F("PzNu"+hname,"p_{z} #nu [GeV]", 40, -300,300);
   hMET["EtaNu"] = new TH1F("EtaNu"+hname,"#eta",50,-2.2,2.2);
   hMET["LepWmass"] = new TH1F("LepWmass"+hname,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",15, 50, 200);
   hMET["LepWmass_topcut"] = new TH1F("LepWmass_topcut"+hname,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150);
   hMET["LepWmassNoPt"]=new TH1F("LepWmassNoPt"+hname,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150);
   hMET["LepWPt"]=new TH1F("LepWPt"+hname,"W#rightarrow#mu#nu p_{t} [GeV/c^{2}]",100, 0, 1000);
   hMET["deltaPhiWW"] = new TH1F("deltaPhiWW"+hname,"#Delta #phi(W(#mu,#nu),W(j,j))",50, -3.15, 3.15);
   hMET["deltaRWW"] = new TH1F("deltaRWW"+hname,"#Delta #phi(W(#mu,#nu),W(j,j))",50, 0, 5.);
   hMET["deltaPhiMETWlep"] = new TH1F("deltaPhiMETWlep"+hname,"#Delta #phi(Missing Transverse Energy,W(#mu,#nu))",50, -3.15, 3.15);
   hMET["deltaPhiMETlep"] = new TH1F("deltaPhiMETlep"+hname,"#Delta #phi(Missing Transverse Energy,#mu)",50, -3.15, 3.15);
   hMET["deltaPhiNulep"] = new TH1F("deltaPhiNulep"+hname,"#Delta #phi(#mu, #nu)",50, -3.15, 3.15);

   hM["WMt"] = new TH1F("Mt"+hname,"M_{T}(W) [GeV/c^{2}]", 50, 0, 500);        // Transverse Mass sqrt(Wpt*Wpt - Wpx*Wpx - Wpy*Wpy)
   hM["WMt_2jet"] = new TH1F("Mt_2jet"+hname,"M_{T}(W) [GeV/c^{2}]", 50, 0, 300);
   hM["WMt_2jet_1btag"] = new TH1F("Mt_2jet_1btag"+hname,"M_{T}(W) [GeV/c^{2}]", 50, 0, 300);
   hM["WMt_fin"] = new TH1F("Mt_fin"+hname,"M_{T}(W) [GeV/c^{2}]", 50, 0, 300);
   hM["dileadjet"] = new TH1F("dileadjet"+hname,"leading (jj) mass [GeV/c^{2}]", 100, 0, 1000);
   hM["dijet"] = new TH1F("dijet"+hname,"(jj) mass [GeV/c^{2}]", 50, 0, 500);
   hM["trijet"] = new TH1F("trijet"+hname,"(jjj) mass [GeV/c^{2}]", 100, 0, 1000);
   hM["top_1btag"] = new TH1F("top_1btag"+hname,"top mass [GeV/c^{2}]",50,0,500);
   hM["top_2btag"] = new TH1F("top_2btag"+hname,"top mass [GeV/c^{2}]",50,0,500);

   hjets["pt"] = new TH1F("jet_pt"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["pt_b_mc"] = new TH1F("jet_pt_b_mc"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["pt_c_mc"] = new TH1F("jet_pt_c_mc"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["pt_l_mc"] = new TH1F("jet_pt_l_mc"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["pt_btag"] = new TH1F("jet_pt_btag"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["pt_btag_b"] = new TH1F("jet_pt_btag_b"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["pt_btag_c"] = new TH1F("jet_pt_btag_c"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["pt_btag_l"] = new TH1F("jet_pt_btag_l"+hname,"jet p_{T} [GeV/c]", 60, 30, 800);
   hjets["1st_pt_cut0"] = new TH1F("jet1_pt_cut0"+hname,"Leading jet p_{T} [GeV/c]",100, 0, 1000);
   hjets["1st_pt"] = new TH1F("jet1_pt"+hname,"Leading jet p_{T} [GeV/c]",100, 0, 1000);
   hjets["1st_pt_1btag"] = new TH1F("jet1_pt_1btag"+hname,"Leading jet p_{T} [GeV/c]",100, 0, 1000);
   hjets["2nd_pt_cut0"] = new TH1F("jet2_pt_cut0"+hname,"2nd jet p_{T} [GeV/c]",100, 0, 1000);
   hjets["2nd_pt"] = new TH1F("jet2_pt"+hname,"2nd jet p_{T} [GeV/c]",60, 30, 1000);
   hjets["2nd_pt_1btag"] = new TH1F("jet2_pt_1btag"+hname,"2nd jet p_{T} [GeV/c]",100, 0, 1000);
   hjets["3rd_pt_cut0"] = new TH1F("jet3_pt_cut0"+hname,"3rd jet p_{T} [GeV/c]", 50, 0, 500);
   hjets["3rd_pt"] = new TH1F("jet3_pt"+hname,"3rd jet p_{T} [GeV/c]",50, 0, 500);
   hjets["3rd_pt_1btag"] = new TH1F("jet3_pt_1btag"+hname,"3rd jet p_{T} [GeV/c]",50, 0, 500);
   hjets["4th_pt_cut0"] = new TH1F("jet4_pt_cut0"+hname,"4th jet p_{T} [GeV/c]",50, 0, 500);
   hjets["4th_pt"] = new TH1F("jet4_pt"+hname,"4th jet p_{T} [GeV/c]",50, 0, 500);
   hjets["4th_pt_1btag"] = new TH1F("jet4_pt_1btag"+hname,"4th jet p_{T} [GeV/c]",50, 0, 500);
   hjets["5th_pt_cut0"] = new TH1F("jet5_pt_cut0"+hname,"5th jet p_{T} [GeV/c]",60, 30, 800);
   hjets["5th_pt"] = new TH1F("jet5_pt"+hname,"5th jet p_{T} [GeV/c]",60, 30, 800);
   hjets["5th_pt_1btag"] = new TH1F("jet5_pt_1btag"+hname,"5th jet p_{T} [GeV/c]",60, 30, 800);
   hjets["6th_pt_cut0"] = new TH1F("jet6_pt_cut0"+hname,"6th jet p_{T} [GeV/c]",60, 30, 800);
   hjets["6th_pt"] = new TH1F("jet6_pt"+hname,"6th jet p_{T} [GeV/c]",60, 30, 800);
   hjets["6th_pt_1btag"] = new TH1F("jet6_pt_1btag"+hname,"6th jet p_{T} [GeV/c]",60, 30, 800);
   hjets["7th_pt_cut0"] = new TH1F("jet7_pt_cut0"+hname,"7th jet p_{T} [GeV/c]",60, 30, 800);
   hjets["7th_pt"] = new TH1F("jet7_pt"+hname,"7th jet p_{T} [GeV/c]",60, 30, 800); 
   hjets["7th_pt_1btag"] = new TH1F("jet7_pt_1btag"+hname,"7th jet p_{T} [GeV/c]",60, 30, 800); 
   hjets["1st_pt_fin"] = new TH1F("jet1_pt_fin"+hname,"Leading jet p_{T} [GeV/c]",60, 30, 800);
   hjets["2nd_pt_fin"] = new TH1F("jet2_pt_fin"+hname,"2nd jet p_{T} [GeV/c]",60, 30, 800);
   hjets["eta"] = new TH1F("jet_eta"+hname,"jet #eta",50, -2.4, 2.4);
   hjets["1st_eta_cut0"] = new TH1F("jet1_eta_cut0"+hname,"Leading jet #eta",50, -2.4, 2.4);
   hjets["1st_eta"] = new TH1F("jet1_eta"+hname,"Leading jet #eta",50, -2.4, 2.4);
   hjets["1st_eta_1btag"] = new TH1F("jet1_eta_1btag"+hname,"Leading jet #eta",50, -2.4, 2.4);
   hjets["2nd_eta_cut0"] = new TH1F("jet2_eta_cut0"+hname,"2nd jet #eta",50, -2.4, 2.4);
   hjets["2nd_eta"] = new TH1F("jet2_eta"+hname,"2nd jet #eta",50, -2.4, 2.4);
   hjets["2nd_eta_1btag"] = new TH1F("jet2_eta_1btag"+hname,"2nd jet #eta",50, -2.4, 2.4);
   hjets["1st_eta_fin"] = new TH1F("jet1_eta_fin"+hname,"Leading jet #eta",50, -2.4, 2.4);
   hjets["2nd_eta_fin"] = new TH1F("jet2_eta_fin"+hname,"2nd jet #eta",50, -2.4, 2.4);
   hjets["phi"] = new TH1F("jet_phi"+hname,"jet #phi",50, 0, 3.15);
   hjets["Njets"] = new TH1F("Njets"+hname,"jet multiplicity",13,-0.5,12.5);
   hjets["Njets_cut0"] = new TH1F("Njets_cut0"+hname,"jet multiplicity",12, 0.5,12.5);
   hjets["Njets_cut1"] = new TH1F("Njets_cut1"+hname,"jet multiplicity",12, 0.5, 12.5);
   hjets["Njets_1btag"] = new TH1F("Njets_1tag"+hname,"jet multiplicity",12, 0.5,12.5);
   hjets["Njets_2btag"] = new TH1F("Njets_2tag"+hname,"jet multiplicity",6,0.5,6.5);
   hjets["Nbtags_TCHPM"] = new TH1F("Nbjets_TCHPM"+hname,"Tagged b-jets",3,-0.5,2.5);
   hjets["Nbtags_CSVL"] = new TH1F("Nbjets_CSVL"+hname,"Tagged b-jets",8,-0.5,7.5);
   hjets["Nbtags_CSVM"] = new TH1F("Nbjets_CSVM"+hname,"Tagged b-jets",8,-0.5,7.5);
   hjets["Nbtags_CSVT"] = new TH1F("Nbjets_CSVT"+hname,"Tagged b-jets",8,-0.5,7.5);
   hjets["Dijet_deltaPhi"] = new TH1F("Dijet_deltaPhi"+hname,"#Delta #phi(j,j)",30,-3.15,3.15);
   hjets["Dijet_deltaR_cut0"] = new TH1F("Dijet_deltaR_cut0"+hname,"#DeltaR(j,j)",80,0.,4.);
   hjets["Dijet_deltaR"] = new TH1F("Dijet_deltaR"+hname,"#DeltaR(j,j)",80,0.,4.);
   hjets["Dijet_deltaR_1btag"] = new TH1F("Dijet_deltaR_1btag"+hname,"#DeltaR(j,j)",80,0.,4.);
   hjets["tb_deltaPhi"] = new TH1F("tb_deltaPhi"+hname,"#Delta #phi(t,b)",30,0.,3.15);
   hjets["tb_deltaEta"] = new TH1F("tb_deltaEta"+hname,"#Delta #eta(t,b)",30,-5,5);
   hjets["pt_top"]  = new TH1F("pt_top"+hname,"top p_{T} [GeV]",60,0,1500);
   hjets["pt_b"]  = new TH1F("pt_b"+hname,"b-jet p_{T} [GeV]",60,0,1500);
   hjets["gen_deltaR_mub"] = new TH1F("gen_deltaR_mub"+hname,"#Delta R(#mu,b)",40,0,4);
   hjets["jet1_CSVM_pt"] = new TH1F("jet1_CSVM_pt"+hname,"leading btag jet p_{T} (CSVM) [GeV/c]",80, 0, 800); 
   hjets["jet2_CSVM_pt"] = new TH1F("jet2_CSVM_pt"+hname,"2 leading btag jet p_{T} (CSVM) [GeV/c]",50, 0, 500); 
   hjets["jet3_CSVM_pt"] = new TH1F("jet3_CSVM_pt"+hname,"3 leading btag jet p_{T} (CSVM) [GeV/c]",20, 0, 200); 
   hjets["jet4_CSVM_pt"] = new TH1F("jet4_CSVM_pt"+hname,"4 leading btag jet p_{T} (CSVM) [GeV/c]",20, 0, 200); 
   hjets["jet1_CSVL_pt"] = new TH1F("jet1_CSVL_pt"+hname,"leading btag jet p_{T} (CSVL) [GeV/c]",80, 0, 800); 
   hjets["jet2_CSVL_pt"] = new TH1F("jet2_CSVL_pt"+hname,"2 leading btag jet p_{T} (CSVL) [GeV/c]",50, 0, 500); 
   hjets["jet3_CSVL_pt"] = new TH1F("jet3_CSVL_pt"+hname,"3 leading btag jet p_{T} (CSVL) [GeV/c]",20, 0, 200); 
   hjets["jet4_CSVL_pt"] = new TH1F("jet4_CSVL_pt"+hname,"4 leading btag jet p_{T} (CSVL) [GeV/c]",20, 0, 200); 


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
      MyStoreTree->SetMuonFalse();
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
       //fCutLabels.push_back("LooseMuVeto");
       fCutLabels.push_back("ElectronVeto");
       fCutLabels.push_back("4Jet");
       fCutLabels.push_back("Ht");
       fCutLabels.push_back("4Jet0b");
       fCutLabels.push_back("4Jet1b");
       fCutLabels.push_back("4JetCut");
       fCutLabels.push_back("Stjet");
     }
   else
     { //electron+jets
       fCutLabels.push_back("Processed");
       fCutLabels.push_back("OneIsoMu");
       fCutLabels.push_back("LooseMuVeto");
       fCutLabels.push_back("ElectronVeto");
       fCutLabels.push_back("4Jet");
       fCutLabels.push_back("Ht");
       fCutLabels.push_back("4Jet1b");
       fCutLabels.push_back("4JetCut");
       fCutLabels.push_back("Stjet");
     }
   hcutflow = new TH1D("cutflow","cut flow", fCutLabels.size(), 0.5, fCutLabels.size() +0.5 );

   for ( vector<string>::const_iterator ivec= fCutLabels.begin(); ivec!=fCutLabels.end(); ++ivec)
     {
       cutmap[ *ivec ] = 0;
     }


   if(fIsMC){
   LumiWeights_ =
     edm::Lumi3DReWeighting("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/PUMC_dist.root",
			     "/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/NewPileUp/MyCert_160404_180252_pileupTruth_finebin.root",
			    "pileup",
			    "pileup",
			    "/uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/Weight_3Dnew.root");
   LumiWeights_.weight3D_init( 1.08 );
   LumiWeightsup_ =
     edm::Lumi3DReWeighting("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/PUMC_dist.root",
			    "/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/NewPileUp/MyCert_160404_180252_pileupTruth_finebin.root",
			    "pileup",
			    "pileup",
			    "/uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/Weight_3Dup.root");
   LumiWeightsup_.weight3D_init( 1.167 );//up
   LumiWeightsdown_ =
     edm::Lumi3DReWeighting("/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/PUMC_dist.root",
			    "/uscms/home/weizou/work/NtupleMaker/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test_4top/NewPileUp/MyCert_160404_180252_pileupTruth_finebin.root",
			    "pileup",
			    "pileup",
			    "/uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/Weight_3Ddown.root");
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
	  
	  hjets["gen_deltaR_mub"]->Fill( p4genLepton.DeltaR( p4genb ) ); 
	}
    }

   //////////Seperate the Wbb, Wcc and Wlight///////////////////////////
   //before the selection, We should seperate the samples
   //if(fSample.Contains("WJets"))
   {
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
   Double_t PUweightdown = 1.0;
   Double_t PUweightup   = 1.0;


   if(fIsMC && !fSample.Contains("4Top")){

      Int_t mc_npvminus1 = ntuple->gen.Bx_minus1;
      Int_t mc_npv0      = ntuple->gen.Bx_0;
      Int_t mc_npvplus1  = ntuple->gen.Bx_plus1;

      PUweight = LumiWeights_.weight3D(mc_npvminus1,mc_npv0,mc_npvplus1);
      PUweightdown = LumiWeightsdown_.weight3D(mc_npvminus1,mc_npv0,mc_npvplus1);
      PUweightup = LumiWeightsup_.weight3D(mc_npvminus1,mc_npv0,mc_npvplus1);
      //PUweight = 1.0;//This is just for test_4top
   }

   if(fIsMC && fSample.Contains("4Top")){

      Int_t mc_npvminus1 = ntuple->gen.Bx_0;
      Int_t mc_npv0      = ntuple->gen.Bx_0;
      Int_t mc_npvplus1  = ntuple->gen.Bx_0;

      PUweight = LumiWeights_.weight3D(mc_npvminus1,mc_npv0,mc_npvplus1);
      PUweightdown = LumiWeightsdown_.weight3D(mc_npvminus1,mc_npv0,mc_npvplus1);
      PUweightup = LumiWeightsup_.weight3D(mc_npvminus1,mc_npv0,mc_npvplus1);
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
  
  hPVs["Nreweight"]->Fill( total_pvs, PUweight );

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
	hmuons["pt_cut1"]->Fill( muon.pt, PUweight );
 
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
			//hmuons["pt_cut2"]->Fill( muon.pt, PUweight );
			//hmuons["N_tisomuons"]->Fill( nloosemuons );  
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
		/*
		if (nmuonID[0] == 1) cutmap["MuID1"] += PUweight;
		if (nmuonID[1] == 1) cutmap["MuID2"] +=PUweight;
		if (nmuonID[2] == 1) cutmap["MuID3"] +=PUweight;
		if (nmuonID[3] == 1) cutmap["MuID4"] +=PUweight;
		if (nmuonID[4] == 1) cutmap["MuID5"] +=PUweight;
		if (nmuonID[5] == 1) cutmap["MuID6"] +=PUweight;
		if (nmuonID[6] == 1) cutmap["MuID7"] +=PUweight;
		if (nmuonID[7] == 1) cutmap["MuID8"] +=PUweight;
		if (ngoodIDmuons == 1) cutmap["MuID"] += PUweight;*/  // TEMP

		if ( ngoodIDmuons > 0 ) hmuons["Ngood"]->Fill( total_pvs, PUweight);
		if ( ntightmuons > 0 ) hmuons["Niso"]->Fill( total_pvs, PUweight);
		if ( ntightmuons != 1 ) return kTRUE;
		cutmap["OneIsoMu"] += PUweight;

		if ( nloosemuons > 1 ) return kTRUE;
		//cutmap["LooseMuVeto"] += PUweight;
	
		if ( nlooseelectrons > 0 ) return kTRUE;
		cutmap["ElectronVeto"] += PUweight;
	
		p4lepton = p4muon;
		if (fVerbose) cout << "got a good lepton" << endl;
	}

	/*
	hmuons["pt"]->Fill( p4lepton.Pt(), PUweight );
	hmuons["eta"]->Fill( p4lepton.Eta(), PUweight );
	hmuons["phi"]->Fill( p4lepton.Phi(), PUweight );
	hmuons["reliso"]->Fill( RelIso, PUweight );
	hmuons["deltaR"]->Fill( deltaR, PUweight ); */

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


  //cutmap["MET"] += PUweight;
  //hMET["MET"]->Fill( p4MET.Pt(), PUweight );
  //hMET["phi"]->Fill( p4MET.Phi(), PUweight );
  //hMET["Ht0"]->Fill( ntuple->PFHt, PUweight );
  //hMET["Htlep"]->Fill( ntuple->PFHt + p4lepton.Pt(), PUweight );


  double Wpt = p4lepton.Pt() + p4MET.Pt();
  double Wpx = p4lepton.Px() + p4MET.Px();
  double Wpy = p4lepton.Py() + p4MET.Py();
  double WMt = sqrt(Wpt*Wpt-Wpx*Wpx-Wpy*Wpy);
  
  //if ( WMt <= 40. ) return kTRUE; 
  //cutmap["MET"] += PUweight;

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

  ////////// put back corr MET JER
  //if (ntuple->run == 1 && ntuple->lumi == 4 && ntuple->event == 3611988 ) {
  //  TString outstring = "MET before ";
  //  outstring += TString(Form("%f", p4MET.Pt() ));
  //  Info("MET", outstring);
  //}

  p4MET.SetPtEtaPhiE( p4MET.Pt() + totJERptadd + totJERptminus, p4MET.Eta(), p4MET.Phi(), p4MET.Pt() + totJERptadd + totJERptminus );

  //if (ntuple->run == 1 && ntuple->lumi == 4 && ntuple->event == 3611988 ) {
  //  TString outstring ="MET after ";
  //  outstring += TString(Form("%f", p4MET.Pt() ));
  //  Info("MET", outstring);
  //  outstring = "tot add = "+TString(Form("%f", totJERptadd))+ " tot minus = "+TString(Form("%f", totJERptminus));
  //  Info("MET", outstring );
  //}

  jets = tmp_corrjets;
  sort( jets.begin(), jets.end(), mysortPt );
  
  for ( size_t ijet=0; ijet < jets.size() ; ++ijet) {

	TopJetEvent jet = jets[ijet];

	bool jetpass = jets.size()>1 && jet.pt > 40. && fabs(jet.eta) < 2.4 && jets[0].pt > 100. && jets[1].pt > 60.;
	//bool jetpass = jets.size()>1 && jet.pt > 30. && fabs(jet.eta) < 2.4;

	// was 35 for all, leading 120
	//if ( total_jets>1 && SF_JEC*jet.pt > 30. && fabs(jet.eta) < 2.4 && SF_JEC*jets[0].pt > 100. && SF_JEC*jets[1].pt > 40. ) 
	if ( jetpass ) {
		
		//if (fVerbose) cout << " jet pt " << SF_JEC*jet.pt << endl;
	
		//hjets["pt"]->Fill( jet.pt, PUweight );
		//hjets["eta"]->Fill( jet.eta, PUweight );
		//hjets["phi"]->Fill( jet.phi, PUweight );

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
		//bdisc["TCHP"].push_back( jet.btag_TCHP );
		bdisc["CSV"].push_back( jet.btag_CSV );
		if (fVerbose) cout << "store bdisc" << endl;
		if ( jet.btag_TCHP > 1.93 ) isTagb["TCHPM"].push_back(true);
		else isTagb["TCHPM"].push_back(false);
		if (fVerbose) cout << "done tchpl" << endl;
		if ( jet.btag_CSV > 0.679){ 
			isTagb["CSVM"].push_back(true); 
			tmpbjetcsvm = jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e); 
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
	if ( fabs( Wm1 - 80.4) < fabs( Wm2 - 80.4) ) p4Nu = p4Nu1tmp;
	else p4Nu = p4Nu2tmp;

	p4OtherNu = p4Nu; // since we chose the real part, the two solutions are the same.
  }


  hMET["PzNu"]->Fill(pzNu, PUweight ); //change this to 2d with two sol and as a function of jets
                                                                                                                       
  TLorentzVector p4LepW = p4lepton + p4Nu;
  TLorentzVector p4OtherLepW = p4lepton + p4OtherNu;

  //if ( fzCalculator.IsComplex() ) hMET["LepWmassComplex"]->Fill( p4LepW.M(), PUweight );


  if (njets>0) {
	/////////// plots without cuts 
	hM["WMt"]->Fill( WMt, PUweight ); 
	hjets["Njets"]->Fill( njets, PUweight );
   	hMET["Ht0"]->Fill( ntuple->PFHt, PUweight );
	hjets["Njets_cut0"]->Fill( njets, PUweight ); 
	hjets["1st_pt_cut0"]->Fill( p4jets[0].Pt(), PUweight );
	hjets["1st_eta_cut0"]->Fill( p4jets[0].Eta(), PUweight );
	hjets["2nd_pt_cut0"]->Fill( p4jets[1].Pt(), PUweight );
	hjets["2nd_eta_cut0"]->Fill( p4jets[1].Eta(), PUweight );
	hjets["3rd_pt_cut0"]->Fill( p4jets[2].Pt(), PUweight );
	hjets["4th_pt_cut0"]->Fill( p4jets[3].Pt(), PUweight );
	hjets["5th_pt_cut0"]->Fill( p4jets[4].Pt(), PUweight );
	hjets["6th_pt_cut0"]->Fill( p4jets[5].Pt(), PUweight );
	hjets["7th_pt_cut0"]->Fill( p4jets[6].Pt(), PUweight );
	hmuons["N_cut0"]->Fill( total_muons, PUweight );
	hmuons["Nelectrons_cut0"]->Fill( nlooseelectrons, PUweight  );
	hMET["MET_cut0"]->Fill( p4MET.Pt(), PUweight );
	hmuons["deltaR_cut0"]->Fill( deltaR, PUweight ); 
	//hjets["Dijet_deltaR_cut0"]->Fill( deltaRjj, PUweight );
	/////////////////////////////////////////////////////////////////////
  }	

  if (fVerbose) cout << "done jets" << endl;

  /*if (njets > 0 ) cutmap["1Jet"] += PUweight;
  if (njets > 1 ) cutmap["2Jet"] += PUweight;
  if (njets > 2 ) cutmap["3Jet"] += PUweight;
  if (njets > 3 ) cutmap["4Jet"] += PUweight;*/

  //if ( njets < 4 ) return kTRUE; 
  if ( njets < 6 ) return kTRUE; 

  //if (njets > 3) {
  if (njets > 5) {

	cutmap["4Jet"] += PUweight;
	double Ht = 0; 
	double Stlep = 0; 
	double Stjet = 0; 
	double deltaRjj = 999.; 


	for ( size_t kkk=0; kkk < p4jets.size(); ++kkk) {
		// Ht calculation
		Ht += p4jets[kkk].Pt();

		// St Calculation
		Stlep = p4MET.Pt() + p4lepton.Pt() ;
		Stjet = p4MET.Pt() + p4lepton.Pt() + Ht ;
		
		 
		// deltaR(jet,jet)
        	for ( vector< TopJetEvent>::iterator ijet=jets.begin(); ijet != jets.end(); ++ijet) {

			TopJetEvent jet = *ijet;
                	TLorentzVector tmpp4Jet;
	                tmpp4Jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e );
        	        double tmpdeltaR = p4jets[kkk].DeltaR(tmpp4Jet);
			if ( tmpdeltaR < 0.3 ) continue;
                	if ( tmpdeltaR < deltaRjj ) deltaRjj = tmpdeltaR;
		}
	}	

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
	}


	vector<double>mvaInputVal;

	//   Cuts
	//bool passcut = true;
	if ( Ht < 300. ) return kTRUE;

	if ( Ht >= 300.0 ) {
		cutmap["Ht"] += PUweight;
		//if(Nbtags_CSVM == 5) cout <<"Run : "<< ntuple->run <<", Lumi : "<< ntuple->lumi << ", Event: "<< ntuple->event << endl; 
		hPVs["Nreweight_2jet"]->Fill( total_pvs, PUweight );
		hMET["Ht"]->Fill( Ht, PUweight );
		hMET["MET"]->Fill( p4MET.Pt(), PUweight );
		hMET["Stlep"]->Fill( Stlep , PUweight );
		hMET["Stjet"]->Fill( Stjet , PUweight );
		hjets["Njets_cut1"]->Fill( njets, PUweight ); 
		hjets["1st_pt"]->Fill( p4jets[0].Pt(), PUweight );
		hjets["1st_eta"]->Fill( p4jets[0].Eta(), PUweight );
		hjets["2nd_pt"]->Fill( p4jets[1].Pt(), PUweight );
		hjets["2nd_eta"]->Fill( p4jets[1].Eta(), PUweight );
		hjets["3rd_pt"]->Fill( p4jets[2].Pt(), PUweight );
		hjets["4th_pt"]->Fill( p4jets[3].Pt(), PUweight );
		hjets["5th_pt"]->Fill( p4jets[4].Pt(), PUweight );
		hjets["6th_pt"]->Fill( p4jets[5].Pt(), PUweight );
		hjets["7th_pt"]->Fill( p4jets[6].Pt(), PUweight );
		hjets["Dijet_deltaR"]->Fill( deltaRjj, PUweight );
		hmuons["N"]->Fill( total_muons, PUweight );
		hmuons["Nelectrons"]->Fill( nlooseelectrons, PUweight  );
		hmuons["pt"]->Fill( p4lepton.Pt(), PUweight );
		hmuons["deltaR"]->Fill( deltaR, PUweight );
		hM["WMt_2jet"]->Fill( WMt, PUweight ); 

		// B-tagging
		hjets["Nbtags_CSVL"]->Fill( Nbtags_CSVL, PUweight ); 
		hjets["Nbtags_CSVM"]->Fill( Nbtags_CSVM, PUweight ); 
		hjets["Nbtags_CSVT"]->Fill( Nbtags_CSVT, PUweight ); 


		// calculate dijet mass closest to W mass
	
		double dileadjet_mass = 0;
		double dijet_mass = 0;
		double DeltaPhiWW =0;
		double DeltaRWW =0;
		double DeltaPhiMETWlep = 0;
		double DeltaPhiMETlep = 0;
		double DeltaPhiNulep = 0;
		double sigma2 = 13.*13.;
		double tmpchi2 = 9999.;

		TLorentzVector p4dileadjet, p4HadW;
		p4dileadjet = p4jets[0] + p4jets[1];
		dileadjet_mass = p4dileadjet.M();

		for ( size_t iijet = 0; iijet < p4jets.size(); ++iijet ) {
			for ( size_t jjjet = iijet+1; jjjet < p4jets.size(); ++jjjet ) {
				TLorentzVector tmpvv = p4jets[iijet] + p4jets[jjjet];
				double tmpmass = tmpvv.M();
				if ( (tmpmass - 83.8 )*(tmpmass - 83.8)/sigma2 < tmpchi2 ) { 
					//tmpchi2 = (tmpmass - 83.8 )*(tmpmass - 83.8)/sigma2; 
					dijet_mass = tmpmass; 
					p4HadW = tmpvv;
				}
			}
		} // dijet mass 

		DeltaPhiWW = p4HadW.DeltaPhi(p4LepW);
		DeltaRWW = p4HadW.DeltaR(p4LepW);
		DeltaPhiMETWlep = p4LepW.DeltaPhi(p4MET);
		DeltaPhiMETlep = p4lepton.DeltaPhi(p4MET);
		DeltaPhiNulep = p4lepton.DeltaPhi(p4Nu);

		double LepWPt = p4LepW.Pt();

		hM["dileadjet"]->Fill( dileadjet_mass, PUweight );
		hM["dijet"]->Fill( dijet_mass, PUweight );
		hMET["LepWmass"]->Fill(p4LepW.M(), PUweight );
		hMET["deltaPhiWW"]->Fill( DeltaPhiWW , PUweight );
		hMET["deltaRWW"]->Fill( DeltaRWW , PUweight );
		hMET["LepWPt"]->Fill( p4LepW.Pt(), PUweight );
		hMET["deltaPhiMETWlep"]->Fill( DeltaPhiMETWlep , PUweight );
		hMET["deltaPhiMETlep"]->Fill( DeltaPhiMETlep , PUweight );
		hMET["deltaPhiNulep"]->Fill( DeltaPhiNulep , PUweight );

		double jet1CSVMpt = 0;
		double jet2CSVMpt = 0;
		double jet3CSVMpt = 0;
		double jet4CSVMpt = 0;
		double jet1CSVLpt = 0;
		double jet2CSVLpt = 0;
		double jet3CSVLpt = 0;
		double jet4CSVLpt = 0;

		if ( Nbtags_CSVM < 1 ){cutmap["4Jet0b"] += PUweight; return kTRUE;}
		//if ( Nbtags_CSVM < 1 ) return kTRUE;

/*		if ( Nbtags_CSVL > 0 ){ jet1CSVLpt = p4bjetscsvl[0].Pt(); hjets["jet1_CSVL_pt"]->Fill ( jet1CSVLpt, PUweight ); }
		if ( Nbtags_CSVL > 1 ){ jet2CSVLpt = p4bjetscsvl[1].Pt(); hjets["jet2_CSVL_pt"]->Fill ( jet2CSVLpt, PUweight ); }
		if ( Nbtags_CSVL > 2 ){ jet3CSVLpt = p4bjetscsvl[2].Pt(); hjets["jet3_CSVL_pt"]->Fill ( jet3CSVLpt, PUweight ); }
		if ( Nbtags_CSVL > 3 ){ jet4CSVLpt = p4bjetscsvl[3].Pt(); hjets["jet4_CSVL_pt"]->Fill ( jet4CSVLpt, PUweight ); }
		if ( Nbtags_CSVM > 0 ){ jet1CSVMpt = p4bjetscsvm[0].Pt(); hjets["jet1_CSVM_pt"]->Fill ( jet1CSVMpt, PUweight ); }
		if ( Nbtags_CSVM > 1 ){ jet2CSVMpt = p4bjetscsvm[1].Pt(); hjets["jet2_CSVM_pt"]->Fill ( jet2CSVMpt, PUweight ); }
		if ( Nbtags_CSVM > 2 ){ jet3CSVMpt = p4bjetscsvl[2].Pt(); hjets["jet3_CSVM_pt"]->Fill ( jet3CSVMpt, PUweight ); }
		if ( Nbtags_CSVM > 3 ){ jet4CSVMpt = p4bjetscsvl[3].Pt(); hjets["jet4_CSVM_pt"]->Fill ( jet4CSVMpt, PUweight ); }
*/		
		if ( Nbtags_CSVM > 0){ 
			cutmap["4Jet1b"] += PUweight;
			if ( p4jets[3].Pt() > 50.){ 
				cutmap["4JetCut"] += PUweight;
				if ( Stjet >= 500 ) {
					cutmap["Stjet"] += PUweight;
					hPVs["Nreweight_2jet_1btag"]->Fill( total_pvs, PUweight );
					hMET["Ht_1btag"]->Fill( Ht, PUweight );
					hMET["MET_1btag"]->Fill( p4MET.Pt(), PUweight );
					hMET["Stlep_1btag"]->Fill( Stlep , PUweight );
					hMET["Stjet_1btag"]->Fill( Stjet , PUweight );
					hjets["Njets_1btag"]->Fill(njets, PUweight );
					hjets["1st_pt_1btag"]->Fill( p4jets[0].Pt(), PUweight );
					hjets["1st_eta_1btag"]->Fill( p4jets[0].Eta(), PUweight );
					hjets["2nd_pt_1btag"]->Fill( p4jets[1].Pt(), PUweight );
					hjets["2nd_eta_1btag"]->Fill( p4jets[1].Eta(), PUweight );
					hjets["3rd_pt_1btag"]->Fill( p4jets[2].Pt(), PUweight );
					hjets["4th_pt_1btag"]->Fill( p4jets[3].Pt(), PUweight );
					hjets["Dijet_deltaR_1btag"]->Fill( deltaRjj, PUweight );
					hmuons["N_1btag"]->Fill( total_muons, PUweight );
					hmuons["Nelectrons_1btag"]->Fill( nlooseelectrons, PUweight  );
					hmuons["pt_1btag"]->Fill( p4lepton.Pt(), PUweight );
					hmuons["deltaR_1btag"]->Fill( deltaR, PUweight );
					hM["WMt_2jet_1btag"]->Fill( WMt, PUweight ); 
				}  // Stjet cut
			}  // 4 leading jet
		}  // 1 btag */

		// Variables in 4Tree
		MyStoreTree->GetGeneralVariable()->PUWeight = PUweight;
		MyStoreTree->GetMetVariable()->Ht = Ht;
		MyStoreTree->GetMetVariable()->MET = p4MET.Pt();
		MyStoreTree->GetMetVariable()->Stlep = Stlep;
		MyStoreTree->GetMetVariable()->Stjet = Stjet;
		MyStoreTree->GetMetVariable()->DeltaPhiWW = DeltaPhiWW;
		MyStoreTree->GetMetVariable()->DeltaPhiMETWlep = DeltaPhiMETWlep;
		MyStoreTree->GetMetVariable()->DeltaPhiMETlep = DeltaPhiMETlep;
		MyStoreTree->GetMetVariable()->DeltaPhiNulep = DeltaPhiNulep;
		MyStoreTree->GetMetVariable()->LepWPt = LepWPt;
		if (DeltaRWW < 100) MyStoreTree->GetMetVariable()->DeltaRWW = DeltaRWW;
                MyStoreTree->GetJetVariable()->numjets= njets;
                MyStoreTree->GetJetVariable()->numBjets_csvl= Nbtags_CSVL;
                MyStoreTree->GetJetVariable()->numBjets_csvm= Nbtags_CSVM;
                MyStoreTree->GetJetVariable()->numBjets_csvt= Nbtags_CSVT;
                MyStoreTree->GetJetVariable()->jet1pt = p4jets[0].Pt() ;
                MyStoreTree->GetJetVariable()->jet2pt = p4jets[1].Pt() ;
                MyStoreTree->GetJetVariable()->jet3pt = p4jets[2].Pt() ;
                MyStoreTree->GetJetVariable()->jet4pt = p4jets[3].Pt() ;
                MyStoreTree->GetJetVariable()->deltaRdijet = deltaRjj ;
                /*if (p4bjetscsvl[0].Pt() < 1000 && p4bjetscsvl[0].Pt() > 10 ) MyStoreTree->GetJetVariable()->jet1CSVLpt = p4bjetscsvl[0].Pt() ;
                if (p4bjetscsvl[1].Pt() < 1000 && p4bjetscsvl[1].Pt() > 10 ) MyStoreTree->GetJetVariable()->jet2CSVLpt = p4bjetscsvl[1].Pt() ;
                if (p4bjetscsvl[2].Pt() < 1000 && p4bjetscsvl[2].Pt() > 10 ) MyStoreTree->GetJetVariable()->jet3CSVLpt = p4bjetscsvl[2].Pt() ;
                if (p4bjetscsvl[3].Pt() < 1000 && p4bjetscsvl[3].Pt() > 10 ) MyStoreTree->GetJetVariable()->jet4CSVLpt = p4bjetscsvl[3].Pt() ;
                if (p4bjetscsvm[0].Pt() < 1000 && p4bjetscsvm[0].Pt() > 10 ) MyStoreTree->GetJetVariable()->jet1CSVMpt = p4bjetscsvm[0].Pt() ;
                if (p4bjetscsvm[1].Pt() < 1000 && p4bjetscsvm[1].Pt() > 10 ) MyStoreTree->GetJetVariable()->jet2CSVMpt = p4bjetscsvm[1].Pt() ;
                if (p4bjetscsvm[2].Pt() < 1000 && p4bjetscsvm[2].Pt() > 10 ) MyStoreTree->GetJetVariable()->jet3CSVMpt = p4bjetscsvm[2].Pt() ;
                if (p4bjetscsvm[3].Pt() < 1000 && p4bjetscsvm[3].Pt() > 10 ) MyStoreTree->GetJetVariable()->jet4CSVMpt = p4bjetscsvm[3].Pt() ; */
		//////////////////////////////////////////////////////////////////////////////////
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
