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
     fProofFile = new TProofOutputFile("SimpleNtuple.root", TProofOutputFile::kDataset, opt, nm->GetTitle());
   } else {
     // For the ntuple, we use the automatic file merging facility
     // Check if an output URL has been given
     TNamed *out = (TNamed *) fInput->FindObject("PROOF_OUTPUTFILE_LOCATION");
     Info("SlaveBegin", "PROOF_OUTPUTFILE_LOCATION: %s", (out ? out->GetTitle() : "undef"));
     TString tmpfilename = "preresults";
     if ( fSample != "" ) tmpfilename += "_"+fSample+".root";
     else tmpfilename = "preresults.root";
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

   hPVs["N"] = new TH1F("NPV"+hname,"Number of PVs",25,-0.5,24.5);
   hPVs["Nreweight"] = new TH1F("NPVreweight"+hname,"Number of PVs",25,-0.5,24.5);
   hPVs["Nreweight_2jet"] = new TH1F("NPVreweight_2jet"+hname,"Number of PVs",25,-0.5,24.5);
   hPVs["Nreweight_2jet_1btag"] = new TH1F("NPVreweight_2jet_1btag"+hname,"Number of PVs",25,-0.5,24.5);

   helectrons["pt"] = new TH1F("electron_pt"+hname,"p_{T}^{#mu} [GeV/c]", 50, 20, 500);
   helectrons["pt_cut2"] = new TH1F("electron_pt_cut2"+hname,"p_{T}^{#mu} [GeV/c]", 50, 20, 500);
   helectrons["pt_1jet"] = new TH1F("electron_pt_1jet"+hname,"p_{T}^{#mu} [GeV/c]", 50, 20, 500);
   helectrons["pt_2jet"] = new TH1F("electron_pt_2jet"+hname,"p_{T}^{#mu} [GeV/c]", 50, 20, 500);
   helectrons["pt_3jet"] = new TH1F("electron_pt_3jet"+hname,"p_{T}^{#mu} [GeV/c]", 50, 20, 500);
   helectrons["pt_4jet"] = new TH1F("electron_pt_4jet"+hname,"p_{T}^{#mu} [GeV/c]", 50, 20, 500);
   helectrons["eta"] = new TH1F("electron_eta"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   helectrons["eta_cut2"] = new TH1F("electron_eta_cut2"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   helectrons["eta_1jet"] = new TH1F("electron_eta_1jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   helectrons["eta_2jet"] = new TH1F("electron_eta_2jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   helectrons["eta_3jet"] = new TH1F("electron_eta_3jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   helectrons["eta_4jet"] = new TH1F("electron_eta_4jet"+hname,"#eta^{#mu}", 20, -2.1, 2.1);
   helectrons["phi"] = new TH1F("electron_phi"+hname,"#phi^{#mu}", 20, 0, 3.15);
   helectrons["phi_cut2"] = new TH1F("electron_phi_cut2"+hname,"#phi^{#mu}", 20, 0, 3.15);
   helectrons["phi_1jet"] = new TH1F("electron_phi_1jet"+hname,"#phi^{#mu}", 20, 0, 3.15);
   helectrons["phi_2jet"] = new TH1F("electron_phi_2jet"+hname,"#phi^{#mu}", 20, 0, 3.15);
   helectrons["phi_3jet"] = new TH1F("electron_phi_3jet"+hname,"#phi^{#mu}", 20, 0, 3.15);
   helectrons["phi_4jet"] = new TH1F("electron_phi_4jet"+hname,"#phi^{#mu}", 20, 0, 3.15);
   helectrons["reliso"] = new TH1F("electron_reliso"+hname,"Relative Isolation", 40, 0, 0.2);
   helectrons["reliso_1jet"] = new TH1F("electron_reliso_1jet"+hname,"Relative Isolation", 40, 0, 0.2);
   helectrons["reliso_2jet"] = new TH1F("electron_reliso_2jet"+hname,"Relative Isolation", 40, 0, 0.2);
   helectrons["reliso_3jet"] = new TH1F("electron_reliso_3jet"+hname,"Relative Isolation", 40, 0, 0.2);
   helectrons["reliso_4jet"] = new TH1F("electron_reliso_4jet"+hname,"Relative Isolation", 40, 0, 0.2);
   helectrons["deltaR_cut0"] = new TH1F("electron_deltaR_cut0"+hname,"#DeltaR(#mu,jet)",80, 0, 4);
   helectrons["deltaR"] = new TH1F("electron_deltaR"+hname,"#DeltaR(#mu,jet)",80, 0, 4);
   helectrons["d0_cut1"] = new TH1F("electron_d0_cut1"+hname,"#mu Impact Parameter [cm]",20,-0.1,0.1);
   helectrons["dz"] = new TH1F("electron_dz"+hname,"|z(#mu) - z_{PV}| [cm]", 25, 0, 1.);
      
   hMET["MET"] = new TH1F("MET"+hname,"Missing Transverse Energy [GeV]", 50, 0, 500);
   hMET["MET_1btag"] = new TH1F("MET_1btag"+hname,"Missing Transverse Energy [GeV]", 50, 0, 500);
   hMET["MET_cut0"] = new TH1F("MET_cut0"+hname,"Missing Transverse Energy [GeV]", 50, 0, 300);
   hMET["MET_2jet"] = new TH1F("MET_2jet"+hname,"Missing Transverse Energy [GeV]", 50, 0, 300);
   hMET["MET_fin"] = new TH1F("MET_fin"+hname,"Missing Transverse Energy [GeV]", 50, 0, 300);
   hMET["genMET_2jet"] = new TH1F("genMET_2jet"+hname,"Missing Transverse Energy [GeV]", 50, 0, 300);
   hMET["deltaMET_2jet"] = new TH1F("deltaMET_2jet"+hname,"Missing Transverse Energy [GeV]", 50, -200, 200);
   hMET["phi"] = new TH1F("MET_phi"+hname,"#phi Missing Transverse Energy [GeV]", 20, 0, 3.15);
   hMET["Ht0"] = new TH1F("Ht0"+hname,"H_{T} [GeV]", 50, 0, 3000);
   hMET["Ht"] = new TH1F("Ht"+hname,"H_{T} [GeV]", 50, 0, 3000);
   hMET["Ht_1btag"] = new TH1F("Ht_1btag"+hname,"H_{T} [GeV]", 50, 0, 3000);
   hMET["Htlep"] = new TH1F("Htlep"+hname,"H_{T,lep} [GeV]", 100, 0, 1000);
   hMET["Stlep"] = new TH1F("Stlep"+hname,"S_{t}^{lep} [GeV]", 100, 0, 1000);
   hMET["Stlep_1btag"] = new TH1F("Stlep_1btag"+hname,"S_{t}^{lep} [GeV]", 100, 0, 1000);
   hMET["Stjet"] = new TH1F("Stjet"+hname,"S_{t}^{jet} [GeV]", 100, 0, 3000);
   hMET["Stjet_1btag"] = new TH1F("Stjet_1btag"+hname,"S_{t}^{jet} [GeV]", 100, 0, 3000);
   hMET["PzNu"] = new TH1F("PzNu"+hname,"p_{z} #nu [GeV]", 40, -300,300);
   hMET["EtaNu"] = new TH1F("EtaNu"+hname,"#eta",50,-2.2,2.2);
   hMET["LepWmass"] = new TH1F("LepWmass"+hname,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150);
   hMET["LepWmass_topcut"] = new TH1F("LepWmass_topcut"+hname,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150);
   hMET["LepWmassNoPt"]=new TH1F("LepWmassNoPt"+hname,"W#rightarrow#mu#nu Mass [GeV/c^{2}]",20, 0, 150);
   hMET["deltaPhi"] = new TH1F("deltaPhi"+hname,"#Delta #phi(#mu,MET)",50, -3.15, 3.15);

   hM["WMt"] = new TH1F("Mt"+hname,"M_{T}(W) [GeV/c^{2}]", 50, 0, 500);        // Transverse Mass sqrt(Wpt*Wpt - Wpx*Wpx - Wpy*Wpy)
   hM["WMt_2jet"] = new TH1F("Mt_2jet"+hname,"M_{T}(W) [GeV/c^{2}]", 50, 0, 300);
   hM["WMt_2jet_1btag"] = new TH1F("Mt_2jet_1btag"+hname,"M_{T}(W) [GeV/c^{2}]", 50, 0, 300);
   hM["WMt_fin"] = new TH1F("Mt_fin"+hname,"M_{T}(W) [GeV/c^{2}]", 50, 0, 300);
   hM["dijet"] = new TH1F("dijet"+hname,"(jj) mass [GeV/c^{2}]", 50, 0, 1000);
   hM["trijet"] = new TH1F("trijet"+hname,"(jjj) mass [GeV/c^{2}]", 50, 0, 1000);
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
   hjets["1st_pt_cut0"] = new TH1F("jet1_pt_cut0"+hname,"Leading jet p_{T} [GeV/c]",60, 30, 1000);
   hjets["1st_pt"] = new TH1F("jet1_pt"+hname,"Leading jet p_{T} [GeV/c]",60, 30, 1000);
   hjets["1st_pt_1btag"] = new TH1F("jet1_pt_1btag"+hname,"Leading jet p_{T} [GeV/c]",60, 30, 1000);
   hjets["2nd_pt_cut0"] = new TH1F("jet2_pt_cut0"+hname,"2nd jet p_{T} [GeV/c]",60, 30, 1000);
   hjets["2nd_pt"] = new TH1F("jet2_pt"+hname,"2nd jet p_{T} [GeV/c]",60, 30, 1000);
   hjets["2nd_pt_1btag"] = new TH1F("jet2_pt_1btag"+hname,"2nd jet p_{T} [GeV/c]",60, 30, 1000);
   hjets["3rd_pt_cut0"] = new TH1F("jet3_pt_cut0"+hname,"3rd jet p_{T} [GeV/c]",60, 30, 800);
   hjets["3rd_pt"] = new TH1F("jet3_pt"+hname,"3rd jet p_{T} [GeV/c]",60, 30, 800);
   hjets["3rd_pt_1btag"] = new TH1F("jet3_pt_1btag"+hname,"3rd jet p_{T} [GeV/c]",60, 30, 800);
   hjets["4th_pt_cut0"] = new TH1F("jet4_pt_cut0"+hname,"4th jet p_{T} [GeV/c]",60, 30, 800);
   hjets["4th_pt"] = new TH1F("jet4_pt"+hname,"4th jet p_{T} [GeV/c]",60, 30, 800);
   hjets["4th_pt_1btag"] = new TH1F("jet4_pt_1btag"+hname,"4th jet p_{T} [GeV/c]",60, 30, 800);
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
   hjets["Njets_cut0"] = new TH1F("Njets_cut0"+hname,"jet multiplicity",13,-0.5,12.5);
   hjets["Njets_cut1"] = new TH1F("Njets_cut1"+hname,"jet multiplicity",13,-0.5,12.5);
   hjets["Njets_1btag"] = new TH1F("Njets_1tag"+hname,"jet multiplicity",13,-0.5,12.5);
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
   hjets["1st_bdisc"] = new TH1F("1st_bdisc"+hname,"b discriminator Leading jet", 50, -0.01, 1.01);
   hjets["1st_bdisc_1btag"] = new TH1F("1st_bdisc_1btag"+hname,"b discriminator Leading jet", 50, -0.01, 1.01);
   hjets["2nd_bdisc"] = new TH1F("2nd_bdisc"+hname,"b discriminator 2nd jet", 50, -0.01, 1.01);
   hjets["2nd_bdisc_1btag"] = new TH1F("2nd_bdisc_1btag"+hname,"b discriminator 2nd jet", 50, -0.01, 1.01);
   hjets["3rd_bdisc"] = new TH1F("3rd_bdisc"+hname,"b discriminator 3rd jet", 50, -0.01, 1.01);
   hjets["3rd_bdisc_1btag"] = new TH1F("3rd_bdisc_1btag"+hname,"b discriminator 3rd jet", 50, -0.01, 1.01);
   hjets["4th_bdisc"] = new TH1F("4th_bdisc"+hname,"b discriminator 4th jet", 50, -0.01, 1.01);
   hjets["4th_bdisc_1btag"] = new TH1F("4th_bdisc_1btag"+hname,"b discriminator 4th jet", 50, -0.01, 1.01);
   hjets["5th_bdisc"] = new TH1F("5th_bdisc"+hname,"b discriminator 5th jet", 50, -0.01, 1.01);
   hjets["6th_bdisc"] = new TH1F("6th_bdisc"+hname,"b discriminator 6th jet", 50, -0.01, 1.01);
   hjets["7th_bdisc"] = new TH1F("7th_bdisc"+hname,"b discriminator 7th jet", 50, -0.01, 1.01);
   hjets["prod_bdisc"] = new TH1F("prod_bdisc"+hname,"discriminators product", 50, -0.01, 1.01);
   hjets["prod_bdisc_1btag"] = new TH1F("prod_bdisc_1btag"+hname,"discriminators product", 50, -0.01, 1.01);



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

   // cut flow
   if (fChannel==1) 
     { //muon +jets
       fCutLabels.push_back("Processed");
       fCutLabels.push_back("OneIsoMu");
       fCutLabels.push_back("LooseMuVeto");
       fCutLabels.push_back("ElectronVeto");
       //fCutLabels.push_back("MET");
       //fCutLabels.push_back("1Jet");
       //fCutLabels.push_back("2Jet");
       //fCutLabels.push_back("3Jet");
       //fCutLabels.push_back("4Jet");
       fCutLabels.push_back("5Jet");
       fCutLabels.push_back("Ht");
       fCutLabels.push_back("5Jet1b");
     }
   else
     { //electron+jets
       fCutLabels.push_back("Processed");
       fCutLabels.push_back("OneIsoMu");
       fCutLabels.push_back("LooseMuVeto");
       fCutLabels.push_back("ElectronVeto");
       //fCutLabels.push_back("MET");
       //fCutLabels.push_back("1Jet");
       //fCutLabels.push_back("2Jet");
       //fCutLabels.push_back("3Jet");
       //fCutLabels.push_back("4Jet");
       fCutLabels.push_back("5Jet");
       fCutLabels.push_back("Ht");
       fCutLabels.push_back("5Jet1b");
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
  ////////////////////////////////////
  // PRIMARY VERTICES
  ///////////////////////////////////

  for ( size_t ipv=0; ipv < total_pvs; ++ipv) 
    {
      if (ipv==0) PVz = primaryVertices[ipv].vz;
    }

  //hPVs["N"]->Fill( total_pvs );

  // calculate PU weight
  double PUweight = 1.;

  if (fIsMC) {

    Int_t      mc_npvminus1 = TMath::Min(ntuple->gen.Bx_minus1,34);
    Int_t      mc_npv0   = TMath::Min(ntuple->gen.Bx_0,34);
    Int_t      mc_npvplus1  = TMath::Min(ntuple->gen.Bx_plus1,34);

    PUweight = f3Dweight->GetBinContent(mc_npvminus1,mc_npv0,mc_npvplus1);

    //int iibin = 0;
    //for ( vector<double>::iterator ivec = fpu_weights_vec.begin(); ivec != fpu_weights_vec.end(); ++ivec )
    // {
    //	int mc_npvs = ntuple->gen.Bx_0; // in-time pile up
    ////int mc_npvs = (int)total_pvs;
    //if ( mc_npvs >= iibin+1 ) PUweight = *ivec; // use the last weight for last bin
    //if ( ( iibin <= mc_npvs ) && ( mc_npvs < iibin + 1 ) ) PUweight = *ivec; 
    //iibin++;
    //}
  }

  // For 4 tops
  if (fIsMC && fSample.Contains("4Top") ) {
	  PUweight = 1;
  }

  hPVs["Nreweight"]->Fill( total_pvs, PUweight );

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
  
		//if ( fMuSelector.MuonTight( muon, PVz) )  
			//hmuons["pt_cut2"]->Fill( muon.pt, PUweight );
			//hmuons["N_tisomuons"]->Fill( nloosemuons );  
			//hmuons["charge_tiso"]->Fill( muon.charge, PUweight );
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
		cutmap["LooseMuVeto"] += PUweight;
	
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


  //hMET["PzNu"]->Fill(pzNu, PUweight ); //change this to 2d with two sol and as a function of jets
                                                                                                                       
  TLorentzVector p4LepW = p4lepton + p4Nu;
  TLorentzVector p4OtherLepW = p4lepton + p4OtherNu;

  //hMET["LepWmass"]->Fill(p4LepW.M(), PUweight );
  //if ( fzCalculator.IsComplex() ) hMET["LepWmassComplex"]->Fill( p4LepW.M(), PUweight );


  /////////////////////////////////
  // JETS
  ////////////////////////////////
  
  //JetCombinatorics myCombi = JetCombinatoric();

  int njets = 0;
  //double prod_bdisc =0;
  MyStoreTree->GetJetVariable()->numjets = 0;
  MyStoreTree->GetJetVariable()->numBjets_csvl = 0;
  MyStoreTree->GetJetVariable()->numBjets_csvm = 0;
  MyStoreTree->GetJetVariable()->numBjets_csvt = 0; 
  map< string, vector<float> > bdisc;
  map< string, vector<bool> >  isTagb;
  map< string, vector<bool> >  isTagbUp;
  map< string, vector<bool> >  isTagbDown;
  vector<int> listflavor;
  vector<float> bdiscriminator;


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
		
		//if (fVerbose) cout << " jet pt " << SF_JEC*jet.pt << endl;
	
		//hjets["pt"]->Fill( jet.pt, PUweight );
		//hjets["eta"]->Fill( jet.eta, PUweight );
		//hjets["phi"]->Fill( jet.phi, PUweight );

		TLorentzVector tmpjet;
		tmpjet.SetPtEtaPhiE(SF_JEC*jet.pt, jet.eta, jet.phi, SF_JEC*jet.e);
		p4jets.push_back( tmpjet);
		listflavor.push_back( jet.mc.flavor );
		if (jet.btag_CSV > 0) bdiscriminator.push_back( jet.btag_CSV );      // for bdiscriminator
		else bdiscriminator.push_back( 0 );

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
			//isTagbUp["CSVL"].push_back(true);
			//isTagbDown["CSVL"].push_back(true);
		} else {
			isTagb["CSVL"].push_back(false);
			//isTagbUp["CSVL"].push_back(false);
			//isTagbDown["CSVL"].push_back(false);
		}
		// CSVM cut at 0.244 LOOSE 
		if ( jet.btag_CSV > 0.679) {
			isTagb["CSVM"].push_back(true);
			isTagbUp["CSVM"].push_back(true);
			isTagbDown["CSVM"].push_back(true);
		} else {
			isTagb["CSVM"].push_back(false);
			isTagbUp["CSVM"].push_back(false);
			isTagbDown["CSVM"].push_back(false);
		}
		// CSVM cut at 0.679 MEDIUM

		if ( jet.btag_CSV > 0.898) {
			isTagb["CSVT"].push_back(true);
			//isTagbUp["CSVT"].push_back(true);
			//isTagbDown["CSVT"].push_back(true);
		} else {
			isTagb["CSVT"].push_back(false);
			//isTagbUp["CSVT"].push_back(false);
			//isTagbDown["CSVT"].push_back(false);
		}
		// CSVM cut at 0.898 TIGHT 
		
		if (fVerbose) cout << "done csv" << endl;
		
		njets++;
	}
  }

//  if (njets>0) {
//	/////////// plots without cuts 
//	hM["WMt"]->Fill( WMt, PUweight ); 
//	hjets["Njets"]->Fill( njets, PUweight );
//   	hMET["Ht0"]->Fill( ntuple->PFHt, PUweight );
//	hjets["Njets_cut0"]->Fill( njets, PUweight ); 
//	hjets["1st_pt_cut0"]->Fill( p4jets[0].Pt(), PUweight );
//	hjets["1st_eta_cut0"]->Fill( p4jets[0].Eta(), PUweight );
//	hjets["2nd_pt_cut0"]->Fill( p4jets[1].Pt(), PUweight );
//	hjets["2nd_eta_cut0"]->Fill( p4jets[1].Eta(), PUweight );
//	hjets["3rd_pt_cut0"]->Fill( p4jets[2].Pt(), PUweight );
//	hjets["4th_pt_cut0"]->Fill( p4jets[3].Pt(), PUweight );
//	hjets["5th_pt_cut0"]->Fill( p4jets[4].Pt(), PUweight );
//	hjets["6th_pt_cut0"]->Fill( p4jets[5].Pt(), PUweight );
//	hjets["7th_pt_cut0"]->Fill( p4jets[6].Pt(), PUweight );
//	hmuons["N_cut0"]->Fill( total_muons, PUweight );
//	hmuons["Nelectrons_cut0"]->Fill( nlooseelectrons, PUweight  );
//	hMET["MET_cut0"]->Fill( p4MET.Pt(), PUweight );
//	hmuons["deltaR_cut0"]->Fill( deltaR, PUweight ); 
//	//hjets["Dijet_deltaR_cut0"]->Fill( deltaRjj, PUweight );
//	/////////////////////////////////////////////////////////////////////
//  }	

  if (fVerbose) cout << "done jets" << endl;

  if (njets > 0 ) cutmap["1Jet"] += PUweight;
  if (njets > 1 ) cutmap["2Jet"] += PUweight;
  if (njets > 2 ) cutmap["3Jet"] += PUweight;
  if (njets > 3 ) cutmap["4Jet"] += PUweight;

  if (njets >= 4) {

	// count partons
	int number_of_b = 0;
	int number_of_c = 0;
	int number_of_l = 0;

	double Ht = 0; 
	double Stlep = 0; 
	double Stjet = 0; 
	double deltaRjj = 999.; 


	for ( size_t kk=0; kk < p4jets.size(); ++kk) {
		// Ht calculation
		Ht += p4jets[kk].Pt();

		// St Calculation
		Stlep = p4MET.Pt() + p4lepton.Pt() ;
		Stjet = p4MET.Pt() + p4lepton.Pt() + Ht ;
		
		 
		// deltaR(jet,jet)
        	for ( vector< TopJetEvent>::iterator ijet=jets.begin(); ijet != jets.end(); ++ijet) {

			TopJetEvent jet = *ijet;
                	TLorentzVector tmpp4Jet;
	                tmpp4Jet.SetPtEtaPhiE(jet.pt, jet.eta, jet.phi, jet.e );
        	        double tmpdeltaR = p4jets[kk].DeltaR(tmpp4Jet);
			if ( tmpdeltaR < 0.3 ) continue;
                	if ( tmpdeltaR < deltaRjj ) deltaRjj = tmpdeltaR;
		}	

		// b-tag
		if ( abs(listflavor[kk])==5 && p4jets[kk].Pt()<=240 ) { number_of_b++;} // hjets["pt_b_mc"]->Fill( p4jets[kk].Pt(), PUweight );}
		if ( abs(listflavor[kk])==4 && p4jets[kk].Pt()<=240 ) { number_of_c++;} // hjets["pt_c_mc"]->Fill( p4jets[kk].Pt(), PUweight );}
		if ( abs(listflavor[kk])==1 || abs(listflavor[kk])==2 || abs(listflavor[kk])==3 || abs(listflavor[kk])==21 ) { number_of_l++;} // hjets["pt_l_mc"]->Fill( p4jets[kk].Pt(), PUweight );}
		//if ( abs(listflavor[kk])==5 && p4jets[kk].Pt()>240 ) { number_of_b_highpt++; hjets["pt_b_mc"]->Fill( p4jets[kk].Pt(), PUweight );}
		//if ( abs(listflavor[kk])==4 && p4jets[kk].Pt()>240 ) { number_of_c_highpt++; hjets["pt_c_mc"]->Fill( p4jets[kk].Pt(), PUweight );}


		//hjets["pt"]->Fill( p4jets[kk].Pt(), PUweight );
		//float b_mc_eff = f2Dttbarbtag->GetBinContent(f2Dttbarbtag->FindBin(p4jets[kk].Pt(),fabs(p4jets[kk].Eta())));
		//float c_mc_eff = f2Dttbarctag->GetBinContent(f2Dttbarctag->FindBin(p4jets[kk].Pt(),fabs(p4jets[kk].Eta())));
		//float l_mc_eff = f2Dttbarlighttag->GetBinContent(f2Dttbarlighttag->FindBin(p4jets[kk].Pt(),fabs(p4jets[kk].Eta())));
		float b_data_eff = 0.705;
		float c_data_eff = b_data_eff/5.;

		float b_SF = 0.972; //bSF_table.GetValue(  p4jets[kk].Pt(), fabs(p4jets[kk].Eta()) );
		float l_SF = lSF_table.GetValue(  p4jets[kk].Pt(), fabs(p4jets[kk].Eta()) );
		float l_data_eff = leff_table.GetValue( p4jets[kk].Pt(), fabs(p4jets[kk].Eta()) );

		double seed = abs(static_cast<int>(sin(p4jets[kk].Phi()*1000000)*100000));
		BTagSFUtil btsfutil = BTagSFUtil( seed );

		if ( fIsMC && abs(listflavor[kk])==5 ) { 
			number_of_b++; hjets["pt_b_mc"]->Fill( p4jets[kk].Pt(), PUweight );
			//Info("Process",TString("btag SF= ")+TString(Form("%f",b_SF)));

			bool tmp_IsTag = isTagb["CSVM"][kk];
			isTagb["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag, abs(listflavor[kk]), b_SF, b_data_eff, l_SF, l_data_eff);
	    
			if (p4jets[kk].Pt()<=240 ) {
				tmp_IsTag = isTagbUp["CSVM"][kk];
				isTagbUp["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag  , abs(listflavor[kk]), b_SF*1.04, b_data_eff, l_SF, l_data_eff);
				tmp_IsTag = isTagbDown["CSVM"][kk];
				isTagbDown["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag, abs(listflavor[kk]), b_SF*0.96, b_data_eff, l_SF, l_data_eff);
			} else {
				tmp_IsTag = isTagbUp["CSVM"][kk];
				isTagbUp["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag  , abs(listflavor[kk]), b_SF*1.15, b_data_eff, l_SF, l_data_eff);
				tmp_IsTag = isTagbDown["CSVM"][kk];
				isTagbDown["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag, abs(listflavor[kk]), b_SF*0.85, b_data_eff, l_SF, l_data_eff);
			}
		}

		if ( fIsMC && abs(listflavor[kk])==4 ) { 
			number_of_c++; hjets["pt_c_mc"]->Fill( p4jets[kk].Pt(), PUweight );
			bool tmp_IsTag = isTagb["CSVM"][kk];
			isTagb["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag, abs(listflavor[kk]), b_SF, c_data_eff, l_SF, l_data_eff);
			if (p4jets[kk].Pt()<=240 ) {
				tmp_IsTag = isTagbUp["CSVM"][kk];
				isTagbUp["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag  , abs(listflavor[kk]), b_SF*1.08, c_data_eff, l_SF, l_data_eff);
				tmp_IsTag = isTagbDown["CSVM"][kk];
				isTagbDown["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag, abs(listflavor[kk]), b_SF*0.92, c_data_eff, l_SF, l_data_eff);
			} else {
				tmp_IsTag = isTagbUp["CSVM"][kk];
				isTagbUp["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag  , abs(listflavor[kk]), b_SF*1.15, c_data_eff, l_SF, l_data_eff);
				tmp_IsTag = isTagbDown["CSVM"][kk];
				isTagbDown["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag, abs(listflavor[kk]), b_SF*0.85, c_data_eff, l_SF, l_data_eff);
			}
		}

		if ( fIsMC && (abs(listflavor[kk])==1 || abs(listflavor[kk])==2 || abs(listflavor[kk])==3 || abs(listflavor[kk])==21 )) { 
			number_of_l++; hjets["pt_l_mc"]->Fill( p4jets[kk].Pt(), PUweight );
			bool tmp_IsTag = isTagb["CSVM"][kk];
			isTagb["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag, abs(listflavor[kk]), b_SF, b_data_eff, l_SF, l_data_eff);
			if (p4jets[kk].Pt()<=240 ) {
				tmp_IsTag = isTagbUp["CSVM"][kk];
				isTagbUp["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag  , abs(listflavor[kk]), b_SF, b_data_eff, l_SF*1.13, l_data_eff);
				tmp_IsTag = isTagbDown["CSVM"][kk];
				isTagbDown["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag, abs(listflavor[kk]), b_SF, b_data_eff, l_SF*1.13, l_data_eff);
			} else {
				tmp_IsTag = isTagbUp["CSVM"][kk];
				isTagbUp["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag  , abs(listflavor[kk]), b_SF, b_data_eff, l_SF*1.20, l_data_eff);
				tmp_IsTag = isTagbDown["CSVM"][kk];
				isTagbDown["CSVM"][kk] = btsfutil.modifyBTagsWithSF( tmp_IsTag, abs(listflavor[kk]), b_SF, b_data_eff, l_SF*1.20, l_data_eff);
			}
		}
	  
		/*if ( isTagb["CSVM"][kk] ) {
			hjets["pt_btag"]->Fill( p4jets[kk].Pt(), PUweight );
			if ( abs(listflavor[kk])==5 ) hjets["pt_btag_b"]->Fill( p4jets[kk].Pt(), PUweight );
			if ( abs(listflavor[kk])==4 ) hjets["pt_btag_c"]->Fill( p4jets[kk].Pt(), PUweight );
			if ( abs(listflavor[kk])==1 || abs(listflavor[kk])==2 || abs(listflavor[kk])==3 || abs(listflavor[kk])==21 ) hjets["pt_btag_l"]->Fill( p4jets[kk].Pt(), PUweight );
		}	*/
	}

	// W+jets h.f. corrections
	int FH = 2; // 0=Wbb, 1=Wcc, 2=Wqq
	if ( number_of_b > 0 ) FH = 0;
	else if ( number_of_b == 0 && number_of_c > 0 ) FH = 1;
	else FH = 2;
	float SF_W = 1.;
	if (fSample=="WJets") {
		// from TOP-11-003
		if ( FH == 0 ) SF_W = 1.21;
		if ( FH == 1 ) SF_W = 1.66;
	}

	// Split W+jets if requested
	if (fSample=="Wbb" && FH != 0 ) return kTRUE;
	if (fSample=="Wcc" && FH != 1 ) return kTRUE;
	if (fSample=="Wqq" && FH != 2 ) return kTRUE;

	// count number of b-tags
	//int Nbtags_TCHPM = 0;
	int Nbtags_CSVL = 0;
	int Nbtags_CSVT = 0;
	int Nbtags_CSVM = 0;
	int NbtagsUp_CSVM = 0;
	int NbtagsDown_CSVM = 0;
	//float SFb_0tag = 1.;
	//float SFb_only1tag = 1.;
	float SFb_1tag = 1.;
	//float SFb_2tag = 1.;
	//float SFb_0tag_syst[2] = {1.}; // for systematics
	//float SFb_1tag_syst[2] = {1.};
	//float SFb_2tag_syst[2] = {1.};

	for ( size_t itag=0; itag< isTagb["CSVM"].size(); ++itag ) {
		//if ( isTagb["TCHPM"][itag] ) Nbtags_TCHPM++;
		if ( isTagb["CSVL"][itag] ) Nbtags_CSVL++;
		if ( isTagb["CSVM"][itag] ) Nbtags_CSVM++;
		if ( isTagbUp["CSVM"][itag] ) NbtagsUp_CSVM++;
		if ( isTagbDown["CSVM"][itag] ) NbtagsDown_CSVM++;
		if ( isTagb["CSVT"][itag] ) Nbtags_CSVT++;
	}
      
	// store b tags
	//hjets["Nbtags_CSVL"]->Fill( Nbtags_CSVL, PUweight*SF_W ); 
	//hjets["Nbtags_CSVM"]->Fill( Nbtags_CSVM, PUweight*SF_W ); 
	//hjets["Nbtags_CSVT"]->Fill( Nbtags_CSVT, PUweight*SF_W ); 

	/*
	// compute b-tag event weight
	if ( fIsMC ) {
		hMET["genMET_2jet"]->Fill( ntuple->gen.MET, PUweight*SF_W );
		hMET["deltaMET_2jet"]->Fill( p4MET.Pt() - ntuple->gen.MET, PUweight*SF_W );
	}
		// zeto tag
		BTagWeight b0(0,0); // number of tags 
		//BTagWeight::JetInfo bj(0.63,0.91); // mean MC eff and mean SF. For TCHPM=0.91\pm0.09, CSVM=0.96\pm0.096
		//BTagWeight::JetInfo cj(0.15,0.91);
		BTagWeight::JetInfo bj(0.63,0.96); 
		BTagWeight::JetInfo cj(0.15,0.96);
		double light_mceff = 0.017; //CHECK
		if ( 100 < p4jets[0].Pt() && p4jets[0].Pt() <= 200 ) light_mceff = 0.04;
		if ( 200 < p4jets[0].Pt() && p4jets[0].Pt() <= 300 ) light_mceff = 0.08;
		if ( 300 < p4jets[0].Pt() && p4jets[0].Pt() <= 400 ) light_mceff = 0.12;
		if ( 400 < p4jets[0].Pt() ) light_mceff = 0.14;

		//BTagWeight::JetInfo lj(light_mceff,1.22); //for TCHPM=1.22, CSVM=1.08 \pm 0.13
		BTagWeight::JetInfo lj(light_mceff,1.08); 

		// b-tag systematic UP 9% for b, 18% for c
		// for CSVM 5% for b, and 10% for c
		//BTagWeight::JetInfo bjUP(0.63,0.99);
		//BTagWeight::JetInfo cjUP(0.15,1.07);
		BTagWeight::JetInfo bjUP(0.63,1.008); 
		BTagWeight::JetInfo cjUP(0.15,1.056);

		// b-tag systemacit DOWN 9% for b, 18% for c
		//BTagWeight::JetInfo bjDOWN(0.63,0.83);
		//BTagWeight::JetInfo cjDOWN(0.15,0.75);
		BTagWeight::JetInfo bjDOWN(0.63,0.912);
		BTagWeight::JetInfo cjDOWN(0.15,0.864);

		// for high pt jets > 240 UP 50% for b and c
		// for CSVM 
		//BTagWeight::JetInfo bjUPhighpt(0.63,1.36);
		//BTagWeight::JetInfo cjUPhighpt(0.15,1.36);
		BTagWeight::JetInfo bjUPhighpt(0.63,1.104);
		BTagWeight::JetInfo cjUPhighpt(0.15,1.104);
		// for high pt jets > 240 DOWN 50% for b and c
		//BTagWeight::JetInfo bjDOWNhighpt(0.63,0.46);
		//BTagWeight::JetInfo cjDOWNhighpt(0.15,0.46);
		BTagWeight::JetInfo bjDOWNhighpt(0.63,0.816);
		BTagWeight::JetInfo cjDOWNhighpt(0.15,0.816);

		vector<BTagWeight::JetInfo> j;
		for(int i=0;i<number_of_b;i++)j.push_back(bj);
		for(int i=0;i<number_of_b_highpt;i++)j.push_back(bj);
		for(int i=0;i<number_of_c;i++)j.push_back(cj);
		for(int i=0;i<number_of_c_highpt;i++)j.push_back(cj);
		for(int i=0;i<number_of_l;i++)j.push_back(lj);
          
		// changed to CSVM from TCHPM
		if (Nbtags_CSVM==0) {
			SFb_0tag = b0.weight(j,0);
			hjets["Nbtags_CSVM"]->Fill( Nbtags_CSVM, PUweight*SFb_0tag ); // fill bin 0
		}
		
		// only one tag
		BTagWeight b11(1,1); // number of tags
		if (Nbtags_CSVM==1) {
			SFb_only1tag = b11.weight(j,1);
			hjets["Nbtags_CSVM"]->Fill( Nbtags_CSVM, PUweight*SFb_only1tag ); // fill bin 1
		}

		// at least one tag
		BTagWeight b1(1,Nbtags_CSVM); // number of tags
		if (Nbtags_CSVM>=1) {
			SFb_1tag = b1.weight(j,1);

			// UP
			vector<BTagWeight::JetInfo> jj;
			for(int i=0;i<number_of_b;i++)jj.push_back(bjUP);
			for(int i=0;i<number_of_b_highpt;i++)jj.push_back(bjUPhighpt);
			for(int i=0;i<number_of_c;i++)jj.push_back(cjUP);
			for(int i=0;i<number_of_c_highpt;i++)jj.push_back(cjUPhighpt);
			for(int i=0;i<number_of_l;i++)jj.push_back(lj);
			SFb_1tag_syst[0] = b1.weight(jj,1); 

			// DOWN
			vector<BTagWeight::JetInfo> jk;
			for(int i=0;i<number_of_b;i++)jk.push_back(bjDOWN);
			for(int i=0;i<number_of_b_highpt;i++)jk.push_back(bjDOWNhighpt);
			for(int i=0;i<number_of_c;i++)jk.push_back(cjDOWN);
			for(int i=0;i<number_of_c_highpt;i++)jk.push_back(cjDOWNhighpt);
			for(int i=0;i<number_of_l;i++)jk.push_back(lj);
			SFb_1tag_syst[1] = b1.weight(jk,1);

		}

		// at least two tags
		BTagWeight b2(2,Nbtags_CSVM); // number of tags
		if (Nbtags_CSVM>=2) {
			SFb_2tag = b2.weight(j,2);
			hjets["Nbtags_CSVM"]->Fill( Nbtags_CSVM, PUweight*SFb_2tag ); // fill bin >=2
	    
			// UP
			vector<BTagWeight::JetInfo> jj;
			for(int i=0;i<number_of_b;i++)jj.push_back(bjUP);
			for(int i=0;i<number_of_b_highpt;i++)jj.push_back(bjUPhighpt);
			for(int i=0;i<number_of_c;i++)jj.push_back(cjUP);
			for(int i=0;i<number_of_c_highpt;i++)jj.push_back(cjUPhighpt);
			for(int i=0;i<number_of_l;i++)jj.push_back(lj);
			SFb_2tag_syst[0] = b2.weight(jj,2); 

			// DOWN
			vector<BTagWeight::JetInfo> jk;
			for(int i=0;i<number_of_b;i++)jk.push_back(bjDOWN);
			for(int i=0;i<number_of_b_highpt;i++)jk.push_back(bjDOWNhighpt);
			for(int i=0;i<number_of_c;i++)jk.push_back(cjDOWN);
			for(int i=0;i<number_of_c_highpt;i++)jk.push_back(cjDOWNhighpt);
			for(int i=0;i<number_of_l;i++)jk.push_back(lj);
			SFb_2tag_syst[1] = b2.weight(jk,2);
		}*/
	//else hjets["Nbtags_CSVM"]->Fill( Nbtags_CSVM );

	//hjets["Nbtags_TCHPM"]->Fill( Nbtags_TCHPM, PUweight*SFb );
	//hjets["Nbtags_CSVM"]->Fill( Nbtags_CSVM, PUweight*SFb );

	cutmap["5Jet"] += PUweight;

	//   Cuts
	bool passcut = true;
	if ( Ht <= 300. ) passcut = false;

	if (passcut) {
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
		//hjets["5th_pt"]->Fill( p4jets[4].Pt(), PUweight );
		//hjets["6th_pt"]->Fill( p4jets[5].Pt(), PUweight );
		//hjets["7th_pt"]->Fill( p4jets[6].Pt(), PUweight );
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

		// plot bdiscriminator
       		if ( bdiscriminator[0] >= 0 ) { MyStoreTree->GetJetVariable()->bdisc_1st = bdiscriminator[0]; hjets["1st_bdisc"]->Fill( bdiscriminator[0], PUweight );}
		if ( bdiscriminator[1] >= 0 ) { MyStoreTree->GetJetVariable()->bdisc_2nd = bdiscriminator[1]; hjets["2nd_bdisc"]->Fill( bdiscriminator[1], PUweight );}
		if ( bdiscriminator[2] >= 0 ) { MyStoreTree->GetJetVariable()->bdisc_3rd = bdiscriminator[2]; hjets["3rd_bdisc"]->Fill( bdiscriminator[2], PUweight );}
		if ( bdiscriminator[3] >= 0 ) { MyStoreTree->GetJetVariable()->bdisc_4th = bdiscriminator[3]; hjets["4th_bdisc"]->Fill( bdiscriminator[3], PUweight );}
		//prod_bdisc = bdiscriminator[0]*bdiscriminator[1]*bdiscriminator[2]*bdiscriminator[3];
		//hjets["prod_bdisc"]->Fill( prod_bdisc, PUweight );
		
		// Variables in 4Tree
		MyStoreTree->GetGeneralVariable()->PUWeight = PUweight;
		MyStoreTree->GetMetVariable()->Ht = Ht;
		MyStoreTree->GetMetVariable()->Stlep = Stlep;
		MyStoreTree->GetMetVariable()->Stjet = Stjet;
                MyStoreTree->GetJetVariable()->numjets= njets;
                MyStoreTree->GetJetVariable()->numBjets_csvl= Nbtags_CSVL;
                MyStoreTree->GetJetVariable()->numBjets_csvm= Nbtags_CSVM;
                MyStoreTree->GetJetVariable()->numBjets_csvt= Nbtags_CSVT;
		//////////////////////////////////////////////////////////////////////////////////

		cutmap["Ht"] += PUweight;

		if ( Nbtags_CSVM >= 1 ) {
			cutmap["5Jet1b"] += PUweight;
			/*hPVs["Nreweight_2jet_1btag"]->Fill( total_pvs, PUweight );
			hMET["Ht_1btag"]->Fill( Ht, PUweight );
			hMET["MET_1btag"]->Fill( p4MET.Pt(), PUweight );
			hMET["Stlep_1btag"]->Fill( Stlep , PUweight );
			hMET["Stjet_1btag"]->Fill( Stjet , PUweight );
			hjets["Njets_1btag"]->Fill(njets, PUweight*SFb_1tag );
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

			// plot bdiscriminator
	       		//if ( bdiscriminator[0] >= 0 ) hjets["1st_bdisc"]->Fill( bdiscriminator[0], PUweight );
	       		hjets["1st_bdisc_1btag"]->Fill( bdiscriminator[0], PUweight );
			if ( bdiscriminator[1] >= 0 ) hjets["2nd_bdisc_1btag"]->Fill( bdiscriminator[1], PUweight );
			if ( bdiscriminator[2] >= 0 ) hjets["3rd_bdisc_1btag"]->Fill( bdiscriminator[2], PUweight );
			if ( bdiscriminator[3] >= 0 ) hjets["4th_bdisc_1btag"]->Fill( bdiscriminator[3], PUweight );
			//hjets["prod_bdisc_1btag"]->Fill( prod_bdisc, PUweight ); 
			////////////////////////////////////////////////////////////////////////////////// */
		}
	}
  }

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
	if ( h1test->GetEntries() > 0) {
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
