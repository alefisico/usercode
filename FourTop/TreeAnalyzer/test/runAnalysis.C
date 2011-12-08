
void runAnalysis(TString sample="all",bool NoGUI=false, bool getLogs=false) 
{
  TString desdir = "/uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/";
  TProof *p = TProof::Open("lite://", desdir ,desdir);

  //p->AddDynamicPath("");
  p->Exec("gSystem->Load(\"/uscms/home/algomez/work/CMSSW_4_2_4/lib/slc5_amd64_gcc434/libYumicevaTop7TeV.so\")");
  p->Exec("gSystem->Load(\"/uscms/home/algomez/work/CMSSW_4_2_4/lib/slc5_amd64_gcc434/libCondFormatsJetMETObjects.so\")");
  p->AddIncludePath("/uscms/home/algomez/work/CMSSW_4_2_4/src/");

  p->Archive(" ",desdir);

  //p->AddInput(new TNamed("PROOF_OUTPUTFILE_LOCATION", "LOCAL"));
/* 
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="4Top_1000"||sample=="all")
    {
      TDSet *mc_ttbar = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbar->Add("/uscms_data/d3/algomez/files/fourtop/4top-Tuple-PATskim.root");
      mc_ttbar->Process("Analyzer.C+","sample=4Top_1000");
    }
 
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="4Top_1000"||sample=="all")
    {
      TDSet *mc_ttbar = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbar->Add("/uscms_data/d3/algomez/files/fourtop/4Top_1000_Tuple-PATskim.root");
      mc_ttbar->Process("Analyzer.C+","sample=4Top_1000");
    }
*/
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="4Top_500"||sample=="all")
    {
      TDSet *mc_ttbar = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbar->Add("/uscms_data/d3/algomez/files/fourtop/4Top_500_Tuple-PATskim.root");
      mc_ttbar->Process("Analyzer.C+","sample=4Top_500");
    }

  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);

  if (sample=="MC"||sample=="ttbar"||sample=="all")
    {
      TDSet *mc_ttbar = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/TTbar_Mu.root");
      mc_ttbar->Process("Analyzer.C+","sample=ttbar");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
/*
  if (sample=="ttbarM"||sample=="all")
    {
      TDSet *mc_ttbarm = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbarm->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/TTbar_matchup_Mu.root");
      mc_ttbarm->Process("Analyzer.C+","sample=ttbarMatchup");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);

  if (sample=="ttbarM"||sample=="all")
    {
      TDSet *mc_ttbarm = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbarm->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/TTbar_matchdown_Mu.root");
      mc_ttbarm->Process("Analyzer.C+","sample=ttbarMatchdown");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);

  if (sample=="ttbarQ"||sample=="all")
    {
      TDSet *mc_ttbarm = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbarm->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/TTbar_scaleup_Mu.root");
      mc_ttbarm->Process("Analyzer.C+","sample=ttbarScaleup");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);

  if (sample=="ttbarQ"||sample=="all")
    {
      TDSet *mc_ttbarm = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbarm->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/TTbar_scaledown_Mu.root");
      mc_ttbarm->Process("Analyzer.C+","sample=ttbarScaledown");
    }


  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="WJets"||sample=="all")
    {
      TDSet *mc_Wjets = new TDSet("top","*","/PATNtupleMaker");
      mc_Wjets->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WJets_Mu.root");
      mc_Wjets->Process("Analyzer.C+","sample=WJets");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="QCD"||sample=="all")
    {
      TDSet *mc_QCD = new TDSet("top","*","/PATNtupleMaker");
      mc_QCD->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/QCD_Mu.root");
      mc_QCD->Process("Analyzer.C+","sample=QCD");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="STsch"||sample=="all")
    {
      TDSet *mc_STsch = new TDSet("top","*","/PATNtupleMaker");
      mc_STsch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/STsch_Mu.root");
      mc_STsch->Process("Analyzer.C+","sample=STsch");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="STtch"||sample=="all")
    {
      TDSet *mc_STtch = new TDSet("top","*","/PATNtupleMaker");
      mc_STtch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/STtch_Mu.root");
      mc_STtch->Process("Analyzer.C+","sample=STtch");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="STtWch"||sample=="all")
    {
      TDSet *mc_STtWch = new TDSet("top","*","/PATNtupleMaker");
      mc_STtWch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/STtWch_Mu.root");
      mc_STtWch->Process("Analyzer.C+","sample=STtWch");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="STsch_bar"||sample=="all")
    {
      TDSet *mc_STsch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STsch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/STsch_bar_Mu.root");
      mc_STsch_bar->Process("Analyzer.C+","sample=STsch_bar");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="STtch_bar"||sample=="all")
    {
      TDSet *mc_STtch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STtch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/STtch_bar_Mu.root");
      mc_STtch_bar->Process("Analyzer.C+","sample=STtch_bar");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="STtWch_bar"||sample=="all")
    {
      TDSet *mc_STtWch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STtWch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/STtWch_bar_Mu.root");
      mc_STtWch_bar->Process("Analyzer.C+","sample=STtWch_bar");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="WW"||sample=="all")
    {
      TDSet *mc_WW = new TDSet("top","*","/PATNtupleMaker");
      mc_WW->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WW_Mu.root");
      mc_WW->Process("Analyzer.C+","sample=WW");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="WZ"||sample=="all")
    {
      TDSet *mc_WZ = new TDSet("top","*","/PATNtupleMaker");
      mc_WZ->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WZ_Mu.root");
      mc_WZ->Process("Analyzer.C+","sample=WZ");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MC"||sample=="ZJets"||sample=="all")
    {
      TDSet *mc_ZJets = new TDSet("top","*","/PATNtupleMaker");
      mc_ZJets->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/ZJets_Mu.root");
      mc_ZJets->Process("Analyzer.C+","sample=ZJets");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);

  if (sample=="MCsyst"||sample=="WJetsScaleUp"||sample=="all")
    {
      TDSet *mc_WJets_scaleup = new TDSet("top","*","/PATNtupleMaker");
      mc_WJets_scaleup->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WJets_scaledup_Mu.root");
      mc_WJets_scaleup->Process("Analyzer.C+","sample=WJets_scaleup");
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="MCsyst"||sample=="WJetsScaleDown"||sample=="all")
    {
      TDSet *mc_WJets_scaledown = new TDSet("top","*","/PATNtupleMaker");
      mc_WJets_scaledown->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_3/WJets_scaledown_Mu.root");
      mc_WJets_scaledown->Process("Analyzer.C+","sample=WJets_scaledown");
    }
  */
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);

  if (sample=="data"||sample=="all")
    {
      TDSet *data = new TDSet("top","*","/PATNtupleMaker");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_3/SingleMu_May10ReReco_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_3/SingleMu_PromptReco-v4_Run_165088_166967_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_3/SingleMu_PromptReco-v4_Run_167039_167913_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_3/SingleMu_Aug05ReReco_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_3/SingleMu_PromptReco-v6_Run_172620_173198_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_3/SingleMu_PromptReco-v6_Run_173236_173692_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_3/SingleMu_Run2011B-PromptReco-v1_Mu.root");
      data->Process("Analyzer.C+","sample=data");
      // get log files
      if (getLogs)
	{
	  logList = p->GetManager()->GetSessionLogs()->GetListOfLogs();
	  for (int i=1; i< logList->GetSize(); ++i)
	    {
	      logElem = ( TProofLogElem* ) logList->At( i );
	      macro = logElem->GetMacro();
	      macro->SaveSource("data_muons_"+TString(Form("%i",i))+".stdout");
	    }
	}
    }
  if (NoGUI) p->SetBit(TProof::kUsingSessionGui);
  if (sample=="dataQCD2"||sample=="all")
    {
      TDSet *dataQCD2 = new TDSet("top","*","/PATNtupleMaker");
      dataQCD2->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/42x/Run2011_May10ReReco_Mu.root");
      dataQCD2->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/42x/Run2011_PromptReco1_Mu.root");
      dataQCD2->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/42x/Run2011_PromptReco2_Mu.root");
      dataQCD2->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/42x/SingleMu_Aug05ReReco_Mu.root");
      dataQCD2->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011/42x/SingleMu_PromptReco-v6_Mu.root");
      dataQCD2->Process("Analyzer.C+","QCD2 sample=data");
    }

}
