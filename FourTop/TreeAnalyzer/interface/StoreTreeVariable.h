#ifndef StoreTreeVariable_h
#define StoreTreeVariable_h

#include <TFile.h>
#include <TTree.h>
#include <iostream>

#define NMAX 100

class StoreTreeVariable {

   public:

      StoreTreeVariable();

      virtual ~StoreTreeVariable();

      void SetMuonFalse(){StoreMuon = kFALSE;}
      void SetElectronFalse(){StoreElectron = kFALSE;}
      void SetJetFalse(){StoreJet = kFALSE;}
      void SetVertexFalse(){StoreVertex = kFALSE;}
      void SetTriggerFalse(){StoreTrigger = kFALSE;}
      void SetMetFalse(){StoreMet = kFALSE;}
      void SetGeneralFalse(){StoreGeneral = kFALSE;}

      //typedef struct MuonVariables{
      typedef struct {

         //Muon ID Variables
         Int_t nummuons;
         Double_t relisolation[NMAX];
         Double_t d0[NMAX];
         Int_t hits[NMAX];
         Int_t trackhits[NMAX];
         Double_t normalchi2[NMAX];
         Int_t stations[NMAX];
         Int_t pixelhits[NMAX];
         Double_t muonvzwithPV[NMAX];
         Int_t tightmuon[NMAX];
         Int_t nloosemuon;
         Int_t ntightmuon;

         //Muon Kinematic
         Double_t pt[NMAX];
         Double_t eta[NMAX];
         Double_t phi[NMAX];
         Double_t e[NMAX];

         //Muon Jet deltaR
         Double_t deltaR[NMAX];

      } MuonVariables;
     

      struct ElectronVariables{

         Int_t elecnum;
         Int_t nlooseelec;
         Int_t ntightelec;

         Double_t d0[NMAX];
         Double_t elecvzwrtPV[NMAX];
         Double_t relisolation[NMAX];
         Double_t pfrelisolation[NMAX];//pfisolation
         //Electron ID
         Double_t sigmaIetaIeta[NMAX];
         Double_t HoE[NMAX];
         Double_t deltaphisc[NMAX];
         Double_t deltaetasc[NMAX];

         //Kinematic
         Double_t pt[NMAX];
         Double_t eta[NMAX];
         Double_t phi[NMAX];
         Double_t e[NMAX];
         //deltaR with Electron and jet
         Double_t deltaR[NMAX];

      };

      struct JetVariables{

         Int_t numjets;
         //Jet Kinematic
         Double_t jet1pt;
         Double_t jet2pt;
         Double_t jet3pt;
         Double_t jet4pt;
         Double_t deltaRdijet;
         /*Double_t jeteta[NMAX];
         Double_t jetphi[NMAX];
         Double_t jete[NMAX];*/

         //mc
	 /*
         Int_t jetparton[NMAX];

         //b tag
         Double_t jettchp[NMAX];
         Double_t jettche[NMAX];
         Double_t jetsvhe[NMAX];
         Double_t jetsvhp[NMAX];
         Double_t jetcsv[NMAX]; */
	 Int_t numBjets_csvl;
	 Int_t numBjets_csvm;
	 Int_t numBjets_csvt;

         Double_t jet1CSVLpt;
         Double_t jet2CSVLpt;
         Double_t jet3CSVLpt;
         Double_t jet4CSVLpt;
         Double_t jet1CSVMpt;
         Double_t jet2CSVMpt;
         Double_t jet3CSVMpt;
         Double_t jet4CSVMpt;
         //Jet Track number
         //Int_t   jettracksize[NMAX];
	 
      };


      struct VertexVariables{

         Int_t numpvs;

      };

      struct TriggerVariables{

         Int_t triggername;
         Int_t accept;
         Int_t prescale;

      };

      struct MetVariables{

	 Double_t MET;
         /*
	Double_t metphi;
         Double_t mht;
         Double_t neupt;
         Double_t rho;
         Double_t sigmarho;
         Double_t scalefactor;
         Double_t eventpuweight;
         Double_t eventpuweightdown;
         Double_t eventpuweightup;
         Double_t eventbtagweight; //Event B tag weight
         Double_t event2btagweight_up; //Event B tag scale factor UP at least 2 TCHPM bjet
         Double_t event2btagweight_down; //Event B tag scale factor DOWN
         Double_t event1btagweight_up; //Event B tag scale factor UP at least 1 TCHPM bjet
         Double_t event1btagweight_down;
         Double_t eventonly1btagweight_up; //Event B tag scale factor UP at least 1 TCHPM bjet
         Double_t eventonly1btagweight_down;
         Double_t event1btagweight; //Event at least one TCHPM btag weight
         Double_t eventonly1btagweight; //Only have one TCHPM btag weight
         Double_t eventonly0btagweight; //Only have zero TCHPM btag weight

         Int_t csvbtagjetnum_up;
         Int_t csvbtagjetnum_down;
         Int_t    flavorhistory;
	 */
         Double_t Ht;
         Double_t Stlep;
         Double_t Stjet;
         Double_t DeltaPhiWW;
         Double_t DeltaRWW;
         Double_t DeltaPhiMETWlep;
         Double_t DeltaPhiMETlep;
         Double_t DeltaPhiNulep;
         Double_t LepWPt;
      };

      struct GeneralVariables {

         Int_t Run;
         Int_t Lumi;
         Int_t Event;
	 Double_t PUWeight;

      };

      TTree* GetStoreTree(){return Top;}

      MuonVariables *GetMuonVariable(){return &VStoreMuon;}
      ElectronVariables *GetElectronVariable(){return &VStoreElectron;}
      JetVariables *GetJetVariable(){return &VStoreJet;}
      VertexVariables *GetVertexVariable(){return &VStoreVertex;}
      TriggerVariables *GetTriggerVariable(){return &VStoreTrigger;}
      MetVariables *GetMetVariable(){return &VStoreMet;}
      GeneralVariables *GetGeneralVariable(){return &VStoreGeneral;}

      void InitialAll();
      void InitialVertex();
      void InitialMuon();
      void InitialElectron();
      void InitialJet();
      void InitialTrigger();
      void InitialMet();
      void InitialGeneral();

   private:

      TTree *Top;

      bool StoreMuon;
      bool StoreVertex;
      bool StoreElectron;
      bool StoreJet;
      bool StoreTrigger;
      bool StoreMet;
      bool StoreGeneral;

      //V mean variables
      MuonVariables VStoreMuon;
      ElectronVariables VStoreElectron;
      JetVariables  VStoreJet;
      VertexVariables VStoreVertex;
      TriggerVariables VStoreTrigger;
      MetVariables VStoreMet;
      GeneralVariables VStoreGeneral;

};

#endif

StoreTreeVariable::StoreTreeVariable(){

  Top = new TTree("4Tree","4Tree");
  Top->SetDirectory(0);
 
  StoreMuon = kTRUE;
  StoreVertex = kTRUE;
  StoreElectron = kTRUE;
  StoreJet = kTRUE;
  StoreTrigger = kTRUE;
  StoreMet = kTRUE;
  StoreGeneral = kTRUE;

}

StoreTreeVariable::~StoreTreeVariable(){

}

void StoreTreeVariable::InitialMuon(){

    if(StoreMuon){
       cout<<"We are initial the Muon Branch"<<endl;

       Top->Branch("muon_number",&VStoreMuon.nummuons,"nummuons/I");
       Top->Branch("muon_isolation",VStoreMuon.relisolation,"reisolation[nummuons]/D");
       Top->Branch("muon_d0",VStoreMuon.d0,"d0[nummuons]/D");
       Top->Branch("muon_muonhist",VStoreMuon.hits,"hits[nummuons]/I");
       Top->Branch("muon_trackhits",VStoreMuon.trackhits,"trackhits[nummuons]/I");
       Top->Branch("muon_normalchi2",VStoreMuon.normalchi2,"normalchi2[nummuons]/D");
       Top->Branch("muon_muonstations",VStoreMuon.stations,"stations[nummuons]/I");
       Top->Branch("muon_pixelhits",VStoreMuon.pixelhits,"pixelhits[nummuons]/I");
       Top->Branch("muon_muonvzwithPV",VStoreMuon.muonvzwithPV,"muonvzwithPV[nummuons]/D");
       Top->Branch("muon_tightmuon",VStoreMuon.tightmuon,"tightmuon[nummuons]/I");
       Top->Branch("muon_nloosemuon",&VStoreMuon.nloosemuon,"nloosemuon/I");
       Top->Branch("muon_ntightmuon",&VStoreMuon.ntightmuon,"ntightmuon/I");
       Top->Branch("muon_jet_deltaR",VStoreMuon.deltaR,"deltaR[nummuons]/D");

       //kinematic
       Top->Branch("muon_pt",VStoreMuon.pt,"pt[nummuons]/D");
       //Top->Branch("muon_pt",muonpt,"pt[nummuons]/D");
       Top->Branch("muon_eta",VStoreMuon.eta,"eta[nummuons]/D");
       Top->Branch("muon_phi",VStoreMuon.phi,"phi[nummuons]/D");
       Top->Branch("muon_energy",VStoreMuon.e,"e[nummuons]/D");

    }

}

void StoreTreeVariable::InitialElectron(){

   if(StoreElectron){

      cout<<"We are initial the Electron Branch"<<endl;

      Top->Branch("electronnum",&VStoreElectron.elecnum,"elecnum/I");
      //Electrom ID
      Top->Branch("elec_d0",VStoreElectron.d0,"d0[elecnum]/D");
      Top->Branch("elec_elecvzwrtPV",VStoreElectron.elecvzwrtPV,"elecivzwrtPV[elecnum]/D");
      Top->Branch("elec_isolation",VStoreElectron.relisolation,"relisolation[elecnum]/D");
      Top->Branch("elec_pfisolation",VStoreElectron.pfrelisolation,"pfrelisolation[elecnum]/D");
      Top->Branch("elec_sigmaIetaIeta",VStoreElectron.sigmaIetaIeta,"sigmaIetaIeta[elecnum]/D");
      Top->Branch("elec_HoE",VStoreElectron.HoE,"HoE[elecnum]/D");
      Top->Branch("elec_deltaphisc",VStoreElectron.deltaphisc,"deltaphisc[elecnum]/D");
      Top->Branch("elec_deltaetasc",VStoreElectron.deltaetasc,"deltaetasc[elecnum]/D");
      //Electron kinematic
      Top->Branch("elec_pt",VStoreElectron.pt,"pt[elecnum]/D");
      Top->Branch("elec_eta",VStoreElectron.eta,"eta[elecnum]/D");
      Top->Branch("elec_phi",VStoreElectron.phi,"phi[elecnum]/D");
      Top->Branch("elec_energy",VStoreElectron.e,"e[elecnum]/D");
      Top->Branch("elec_jet_deltaR",VStoreElectron.deltaR,"deltaR[elecnum]/D");

   }

}

void StoreTreeVariable::InitialJet(){

 if(StoreJet){
      
      cout<<"We are initial Jet Branch"<<endl;

      Top->Branch("jet_number",&VStoreJet.numjets,"numjets/I");
      
      Top->Branch("jet1_pt",&VStoreJet.jet1pt,"jet1pt/D");
      Top->Branch("jet2_pt",&VStoreJet.jet2pt,"jet2pt/D");
      Top->Branch("jet3_pt",&VStoreJet.jet3pt,"jet3pt/D");
      Top->Branch("jet4_pt",&VStoreJet.jet4pt,"jet4pt/D");
      Top->Branch("deltaRdijet",&VStoreJet.deltaRdijet,"deltaRdijet/D");
      /*Top->Branch("jet_eta",VStoreJet.jeteta,"jeteta[numjets]/D");
      Top->Branch("jet_phi",VStoreJet.jetphi,"jetphi[numjets]/D");
      Top->Branch("jet_energy",VStoreJet.jete,"jete[numjets]/D");
      Top->Branch("jet_partonflavor",VStoreJet.jetparton,"jetparton[numjets]/I");
      //btag
      Top->Branch("jet_btag_trackhighputy",VStoreJet.jettchp,"jettchp[numjets]/D");
      Top->Branch("jet_btag_trackhigheff",VStoreJet.jettche,"jettche[numjets]/D");
      Top->Branch("jet_btag_SVhigheff",VStoreJet.jetsvhe,"jetsvhe[numjets]/D");
      Top->Branch("jet_btag_SVhighputy",VStoreJet.jetsvhp,"jetsvhp[numjets]/D");
      Top->Branch("jet_btag_csv",VStoreJet.jetcsv,"jetcsv[numjets]/D");
      */
      Top->Branch("numBjets_csvl",&VStoreJet.numBjets_csvl,"numBjets_csvl/I");
      Top->Branch("numBjets_csvm",&VStoreJet.numBjets_csvm,"numBjets_csvm/I");
      Top->Branch("numBjets_csvt",&VStoreJet.numBjets_csvt,"numBjets_csvt/I");

      Top->Branch("jet1CSVLpt",&VStoreJet.jet1CSVLpt,"jet1CSVLpt/D");
      Top->Branch("jet2CSVLpt",&VStoreJet.jet2CSVLpt,"jet2CSVLpt/D");
      Top->Branch("jet3CSVLpt",&VStoreJet.jet3CSVLpt,"jet3CSVLpt/D");
      Top->Branch("jet4CSVLpt",&VStoreJet.jet4CSVLpt,"jet4CSVLpt/D");
      Top->Branch("jet1CSVMpt",&VStoreJet.jet1CSVMpt,"jet1CSVMpt/D");
      Top->Branch("jet2CSVMpt",&VStoreJet.jet2CSVMpt,"jet2CSVMpt/D");
      Top->Branch("jet3CSVMpt",&VStoreJet.jet3CSVMpt,"jet3CSVMpt/D");
      Top->Branch("jet4CSVMpt",&VStoreJet.jet4CSVMpt,"jet4CSVMpt/D");
      //Jet track size
      //Top->Branch("jet_tracksize",VStoreJet.jettracksize,"jettracksize[numjets]/I");
 }

}

void StoreTreeVariable::InitialMet(){

  if(StoreMet){
      cout<<"We are inital Met Branch"<<endl;
     
      Top->Branch("MET",&VStoreMet.MET,"MET/D");
      /*Top->Branch("MET_phi",&VStoreMet.metphi,"metphi/D");
      Top->Branch("MHT_energy",&VStoreMet.mht,"mht/D");
      Top->Branch("Neutrinuo_pt",&VStoreMet.neupt,"neupt/D");
      Top->Branch("pileup_rho",&VStoreMet.rho,"rho/D");
      Top->Branch("pileup_sigmarho",&VStoreMet.sigmarho,"sigmarho/D");
      Top->Branch("scalefactor",&VStoreMet.scalefactor,"scalefactor/D");
      Top->Branch("pileup_reweight",&VStoreMet.eventpuweight,"eventpuweight/D");
      Top->Branch("pileup_reweightdown",&VStoreMet.eventpuweightdown,"eventpuweightdown/D");
      Top->Branch("pileup_reweightup",&VStoreMet.eventpuweightup,"eventpuweightup/D");
      Top->Branch("btagweight",&VStoreMet.eventbtagweight,"eventbtagweight/D");
      Top->Branch("2btagweight_up",&VStoreMet.event2btagweight_up,"event2btagweight_up/D");
      Top->Branch("2btagweight_down",&VStoreMet.event2btagweight_down,"event2btagweight_down/D");
      Top->Branch("1btagweight_up",&VStoreMet.event1btagweight_up,"event1btagweight_up/D");
      Top->Branch("1btagweight_down",&VStoreMet.event1btagweight_down,"event1btagweight_down/D");
      Top->Branch("only1btagweight_up",&VStoreMet.eventonly1btagweight_up,"eventonly1btagweight_up/D");
      Top->Branch("only1btagweight_down",&VStoreMet.eventonly1btagweight_down,"eventonly1btagweight_down/D");
      Top->Branch("1btagweight",&VStoreMet.event1btagweight,"event1btagweight/D");// at least one btag weight factor
      Top->Branch("only1btagweight",&VStoreMet.eventonly1btagweight,"eventonly1btagweight/D");// at least one btag weight factor
      Top->Branch("only0btagweight",&VStoreMet.eventonly0btagweight,"eventonly0btagweight/D");// at least one btag weight factor
      Top->Branch("csvbtagjetnum_up",&VStoreMet.csvbtagjetnum_up,"csvbtagjetnum_up/I");
      Top->Branch("csvbtagjetnum_down",&VStoreMet.csvbtagjetnum_down,"csvbtagjetnum_down/I");
      Top->Branch("flavorhistory",&VStoreMet.flavorhistory,"flavorhistory/I");
    */
      Top->Branch("Ht",&VStoreMet.Ht,"Ht/D");
      Top->Branch("Stlep",&VStoreMet.Stlep,"Stlep/D");
      Top->Branch("Stjet",&VStoreMet.Stjet,"Stjet/D");
      Top->Branch("DeltaPhiWW",&VStoreMet.DeltaPhiWW,"DeltaPhiWW/D");
      Top->Branch("DeltaRWW",&VStoreMet.DeltaRWW,"DeltaRWW/D");
      Top->Branch("DeltaPhiMETWlep",&VStoreMet.DeltaPhiMETWlep,"DeltaPhiMETWlep/D");
      Top->Branch("DeltaPhiMETlep",&VStoreMet.DeltaPhiMETlep,"DeltaPhiMETlep/D");
      Top->Branch("DeltaPhiNulep",&VStoreMet.DeltaPhiNulep,"DeltaPhiNulep/D");
      Top->Branch("LepWPt",&VStoreMet.LepWPt,"LepWPt/D");
  }
}

void StoreTreeVariable::InitialGeneral(){

  if(StoreGeneral){
      cout<<"We are inital General Branch"<<endl;

      Top->Branch("run",&VStoreGeneral.Run,"Run/I");
      Top->Branch("lumi",&VStoreGeneral.Lumi,"Lumi/I");
      Top->Branch("event",&VStoreGeneral.Event,"Event/I");
      Top->Branch("PUWeight",&VStoreGeneral.PUWeight,"PUWeight/D");

  }
}

void StoreTreeVariable::InitialTrigger(){
   
   if(StoreTrigger){
       cout<<"We are nitial Trigger Bracn"<<endl;

      Top->Branch("Trigger_Name",&VStoreTrigger.triggername,"triggername/I");
      Top->Branch("Trigger_accept",&VStoreTrigger.accept,"accept/I");
      Top->Branch("Trigger_prescale",&VStoreTrigger.prescale,"prescale/I");

   }

}

void StoreTreeVariable::InitialVertex(){
     
     if(StoreVertex){
       cout<<"We are inital Vertex Branch"<<endl;

       Top->Branch("nPVs",&VStoreVertex.numpvs,"numpvs/I");

   }

}

void StoreTreeVariable::InitialAll(){

     InitialVertex();
     InitialTrigger();
     InitialMuon();
     InitialElectron();
     InitialJet();
     InitialMet();
     InitialGeneral();
}
