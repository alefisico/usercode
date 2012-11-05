void runAnalysis(TString sample="all", TString proc="all", TString sys="all", TString updown="both", bool getLogs=false)  
{
  TString desdir = "/uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/backup/";
  //TString desdir = "/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results13/";
  TProof *p = TProof::Open("lite://", desdir ,desdir);

  //p->AddDynamicPath("");
  p->Exec("gSystem->Load(\"/uscms/home/algomez/work/CMSSW_4_2_4/lib/slc5_amd64_gcc434/libYumicevaTop7TeV.so\")");
  p->Exec("gSystem->Load(\"/uscms/home/algomez/work/CMSSW_4_2_4/lib/slc5_amd64_gcc434/libCondFormatsJetMETObjects.so\")");
  p->AddIncludePath("/uscms/home/algomez/work/CMSSW_4_2_4/src/");

  p->Archive(" ",desdir);

  //p->AddInput(new TNamed("PROOF_OUTPUTFILE_LOCATION", "LOCAL"));

  
  if (sample=="tttt"|| sample=="ttttSM"||sample=="all")
    {
      TDSet *mc_tttt_SM = new TDSet("top","*","/PATNtupleMaker");
      mc_tttt_SM->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_SM-Tuple-PATskim.root");
	if (proc =="all" ) {
		mc_tttt_SM->Process("Analyzer.C+","sample=tttt_SM");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_tttt_SM->Process("Analyzer.C+", sys+"UP sample=tttt_SM");
	   if (updown == "both" || updown == "down") mc_tttt_SM->Process("Analyzer.C+", sys+"DOWN sample=tttt_SM");
	}
      } 
    
  if (sample=="tttt"||sample=="tttt400"||sample=="all")
    {
      TDSet *mc_tttt_Gh400 = new TDSet("top","*","/PATNtupleMaker");
      mc_tttt_Gh400->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh400_50k_1-Tuple-PATskim.root");
      mc_tttt_Gh400->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh400_50k_2-Tuple-PATskim.root");
	if (proc =="all" ) {
		mc_tttt_Gh400->Process("Analyzer.C+","sample=tttt_Gh400");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_tttt_Gh400->Process("Analyzer.C+", sys+"UP sample=tttt_Gh400");
	   if (updown == "both" || updown == "down") mc_tttt_Gh400->Process("Analyzer.C+", sys+"DOWN sample=tttt_Gh400");
	}
      }

  if (sample=="tttt"||sample=="tttt500"||sample=="all")
    {
      TDSet *mc_tttt_Gh500 = new TDSet("top","*","/PATNtupleMaker");
      mc_tttt_Gh500->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh500_50k_1-Tuple-PATskim.root");
      mc_tttt_Gh500->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh500_50k_2-Tuple-PATskim.root");
	if (proc =="all" ) {
		mc_tttt_Gh500->Process("Analyzer.C+","sample=tttt_Gh500");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_tttt_Gh500->Process("Analyzer.C+", sys+"UP sample=tttt_Gh500");
	   if (updown == "both" || updown == "down") mc_tttt_Gh500->Process("Analyzer.C+", sys+"DOWN sample=tttt_Gh500");
	}
      }

  if (sample=="tttt"||sample=="tttt600"||sample=="all")
    {
      TDSet *mc_tttt_Gh600 = new TDSet("top","*","/PATNtupleMaker");
      //mc_tttt_Gh600->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh600_50k_1-Tuple-PATskim.root");
      mc_tttt_Gh600->Add("/uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/Top7TeV/test/tttt_Gh600_test_FASTSIM_FULLSIM/tttt_Gh600-Tuple-PATskim.root");
      //mc_tttt_Gh600->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh600_50k_2-Tuple-PATskim.root");
	if (proc =="all" ) {
		mc_tttt_Gh600->Process("Analyzer.C+","sample=tttt_Gh600");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_tttt_Gh600->Process("Analyzer.C+", sys+"UP sample=tttt_Gh600");
	   if (updown == "both" || updown == "down") mc_tttt_Gh600->Process("Analyzer.C+", sys+"DOWN sample=tttt_Gh600");
	}
      }

  if (sample=="tttt"||sample=="tttt700"||sample=="all")
    {
      TDSet *mc_tttt_Gh700 = new TDSet("top","*","/PATNtupleMaker");
      mc_tttt_Gh700->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh700_50k_1-Tuple-PATskim.root");
      mc_tttt_Gh700->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh700_50k_2-Tuple-PATskim.root");
	if (proc =="all" ) {
		mc_tttt_Gh700->Process("Analyzer.C+","sample=tttt_Gh700");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_tttt_Gh700->Process("Analyzer.C+", sys+"UP sample=tttt_Gh700");
	   if (updown == "both" || updown == "down") mc_tttt_Gh700->Process("Analyzer.C+", sys+"DOWN sample=tttt_Gh700");
	}
      }

  if (sample=="tttt"||sample=="tttt800"||sample=="all")
    {
      TDSet *mc_tttt_Gh800 = new TDSet("top","*","/PATNtupleMaker");
      mc_tttt_Gh800->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh800_50k_1-Tuple-PATskim.root");
      mc_tttt_Gh800->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh800_50k_2-Tuple-PATskim.root");
	if (proc =="all" ) {
		mc_tttt_Gh800->Process("Analyzer.C+","sample=tttt_Gh800");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_tttt_Gh800->Process("Analyzer.C+", sys+"UP sample=tttt_Gh800");
	   if (updown == "both" || updown == "down") mc_tttt_Gh800->Process("Analyzer.C+", sys+"DOWN sample=tttt_Gh800");
	}
      }

  if (sample=="tttt"||sample=="tttt900"||sample=="all")
    {
      TDSet *mc_tttt_Gh900 = new TDSet("top","*","/PATNtupleMaker");
      mc_tttt_Gh900->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh900_50k_1-Tuple-PATskim.root");
      mc_tttt_Gh900->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh900_50k_2-Tuple-PATskim.root");
	if (proc =="all" ) {
		mc_tttt_Gh900->Process("Analyzer.C+","sample=tttt_Gh900");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_tttt_Gh900->Process("Analyzer.C+", sys+"UP sample=tttt_Gh900");
	   if (updown == "both" || updown == "down") mc_tttt_Gh900->Process("Analyzer.C+", sys+"DOWN sample=tttt_Gh900");
	}
      }

  if (sample=="tttt"||sample=="tttt1000"||sample=="all")
    {
      TDSet *mc_tttt_Gh1000 = new TDSet("top","*","/PATNtupleMaker");
      mc_tttt_Gh1000->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh1000_50k_1-Tuple-PATskim.root");
      mc_tttt_Gh1000->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh1000_50k_2-Tuple-PATskim.root");
	if (proc =="all" ) {
		mc_tttt_Gh1000->Process("Analyzer.C+","sample=tttt_Gh1000");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_tttt_Gh1000->Process("Analyzer.C+", sys+"UP sample=tttt_Gh1000");
	   if (updown == "both" || updown == "down") mc_tttt_Gh1000->Process("Analyzer.C+", sys+"DOWN sample=tttt_Gh1000");
	}
      }

  if (sample=="tttt"||sample=="ttttUED6"||sample=="all")
    {
      TDSet *mc_tttt_UED6 = new TDSet("top","*","/PATNtupleMaker");
      mc_tttt_UED6->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_UED6-Tuple-PATskim.root");
	if (proc =="all" ) {
		mc_tttt_UED6->Process("Analyzer.C+","sample=ttttUED6");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_tttt_UED6->Process("Analyzer.C+", sys+"UP sample=ttttUED6");
	   if (updown == "both" || updown == "down") mc_tttt_UED6->Process("Analyzer.C+", sys+"DOWN sample=ttttUED6");
	}
      }
  /*if (sample=="tttt"||sample=="tttt1200"||sample=="all")
    {
      TDSet *mc_tttt_Gh1200 = new TDSet("top","*","/PATNtupleMaker");
      mc_tttt_Gh1200->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh1200-Tuple-PATskim.root");
	if (proc =="all" ) {
		mc_tttt_Gh1200->Process("Analyzer.C+","sample=tttt_Gh1200");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_tttt_Gh1200->Process("Analyzer.C+", sys+"UP sample=tttt_Gh1200");
	   if (updown == "both" || updown == "down") mc_tttt_Gh1200->Process("Analyzer.C+", sys+"DOWN sample=tttt_Gh1200");
	}
      }*/

  if (sample=="MC"||sample=="ttbar"||sample=="all")
    {
      TDSet *mc_ttbar = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/TTbar_Fall11_Mu.root"); ///uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/TTbar_Mu.root");
	if (proc =="all" ) {
		mc_ttbar->Process("Analyzer.C+","sample=ttbar");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_ttbar->Process("Analyzer.C+", sys+"UP sample=ttbar");
	   if (updown == "both" || updown == "down") mc_ttbar->Process("Analyzer.C+", sys+"DOWN sample=ttbar");
	}
      }

  if (sample=="TTbar"||sample=="all")
    {
      TDSet *mc_TTbar = new TDSet("top","*","/PATNtupleMaker");
      mc_TTbar->Add("/eos/uscms/store/user/algomez/ttbar/ttbar_pythia8-Tuple-PATskim.root"); 
      mc_TTbar->Add("/eos/uscms/store/user/algomez/ttbar/ttbar_pythia8_100k-Tuple-PATskim.root"); 
	if (proc =="all" ) {
		mc_TTbar->Process("Analyzer.C+","sample=TTbar");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_TTbar->Process("Analyzer.C+", sys+"UP sample=TTbar");
	   if (updown == "both" || updown == "down") mc_TTbar->Process("Analyzer.C+", sys+"DOWN sample=TTbar");
	}
      }

  if (sample=="TTbar_POWHEG"||sample=="all")
    {
      TDSet *mc_ttbar = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/TT_Powheg_Mu.root");
	if (proc =="all" ) mc_ttbar->Process("Analyzer.C+","sample=ttbar_powheg");
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_ttbar->Process("Analyzer.C+", sys+"UP sample=ttbar_powheg");
	   if (updown == "both" || updown == "down") mc_ttbar->Process("Analyzer.C+", sys+"DOWN sample=ttbar_powheg");
	}
      }

  if (sample=="TTbarM"||sample=="MC")
    {
      TDSet *mc_ttbarm = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbarm->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/TTbar_matchup_Mu.root");
      if (proc =="all" ) {mc_ttbarm->Process("Analyzer.C+","sample=ttbarMatchup");}
    }


  if (sample=="TTbarM"||sample=="MC")
    {
      TDSet *mc_ttbarm = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbarm->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/TTbar_matchdown_Mu.root");
      if (proc =="all" ) {mc_ttbarm->Process("Analyzer.C+","sample=ttbarMatchdown");}
    }


  if (sample=="TTbarQ"||sample=="MC")
    {
      TDSet *mc_ttbarm = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbarm->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/TTbar_scaleup_Mu.root");
      if (proc =="all" ) {mc_ttbarm->Process("Analyzer.C+","sample=ttbarScaleup");}
    }


  if (sample=="TTbarQ"||sample=="MC")
    {
      TDSet *mc_ttbarm = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbarm->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/TTbar_scaledown_Mu.root");
      if (proc =="all" ) {mc_ttbarm->Process("Analyzer.C+","sample=ttbarScaledown");}
    }

  if (sample=="WJets"||sample=="all")
    {
      TDSet *mc_WJets = new TDSet("top","*","/PATNtupleMaker");
      mc_WJets->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/WJets_Mu.root");
	if (proc =="all" ) {
		mc_WJets->Process("Analyzer.C+","sample=WJets");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_WJets->Process("Analyzer.C+", sys+"UP sample=WJets");
	   if (updown == "both" || updown == "down") mc_WJets->Process("Analyzer.C+", sys+"DOWN sample=WJets");
	}
      } 

  if (sample=="MC"||sample=="Wjets"||sample=="Wbb"||sample=="all")
    {
      TDSet *mc_Wbb = new TDSet("top","*","/PATNtupleMaker");
      mc_Wbb->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/WJets_Mu.root");
	if (proc =="all" ) {
		mc_Wbb->Process("Analyzer.C+","sample=Wbb");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_Wbb->Process("Analyzer.C+", sys+"UP sample=Wbb");
	   if (updown == "both" || updown == "down") mc_Wbb->Process("Analyzer.C+", sys+"DOWN sample=Wbb");
	}
    }

  if (sample=="MC"||sample=="Wjets"||sample=="Wcc"||sample=="all")
    {
      TDSet *mc_Wcc = new TDSet("top","*","/PATNtupleMaker");
      mc_Wcc->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/WJets_Mu.root");
	if (proc =="all" ) {
		mc_Wcc->Process("Analyzer.C+","sample=Wcc");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_Wcc->Process("Analyzer.C+", sys+"UP sample=Wcc");
	   if (updown == "both" || updown == "down") mc_Wcc->Process("Analyzer.C+", sys+"DOWN sample=Wcc");
	}
    }

  if (sample=="MC"||sample=="Wjets"||sample=="Wqq"||sample=="all")
    {
      TDSet *mc_Wqq = new TDSet("top","*","/PATNtupleMaker");
      mc_Wqq->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/WJets_Mu.root");
	if (proc =="all" ) {
		mc_Wqq->Process("Analyzer.C+","sample=Wqq");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_Wqq->Process("Analyzer.C+", sys+"UP sample=Wqq");
	   if (updown == "both" || updown == "down") mc_Wqq->Process("Analyzer.C+", sys+"DOWN sample=Wqq");
	}
    }

  if (sample=="QCD"||sample=="all")
    {
      TDSet *mc_QCD = new TDSet("top","*","/PATNtupleMaker");
      mc_QCD->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/QCD_Mu.root");
	if (proc =="all" ) {
		mc_QCD->Process("Analyzer.C+","sample=QCD");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_QCD->Process("Analyzer.C+", sys+"UP sample=QCD");
	   if (updown == "both" || updown == "down") mc_QCD->Process("Analyzer.C+", sys+"DOWN sample=QCD");
	}
      }

  if (sample=="MC"||sample=="ST"||sample=="STsch"||sample=="all")
    {
      TDSet *mc_STsch = new TDSet("top","*","/PATNtupleMaker");
      mc_STsch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/STsch_Mu.root");
	if (proc =="all" ) {
		mc_STsch->Process("Analyzer.C+","sample=STsch");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_STsch->Process("Analyzer.C+", sys+"UP sample=STsch");
	   if (updown == "both" || updown == "down") mc_STsch->Process("Analyzer.C+", sys+"DOWN sample=STsch");
	}
      }

  if (sample=="MC"||sample=="ST"||sample=="STtch"||sample=="all")
    {
      TDSet *mc_STtch = new TDSet("top","*","/PATNtupleMaker");
      mc_STtch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/STtch_Mu.root");
	if (proc =="all" ) {
		mc_STtch->Process("Analyzer.C+","sample=STtch");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_STtch->Process("Analyzer.C+", sys+"UP sample=STtch");
	   if (updown == "both" || updown == "down") mc_STtch->Process("Analyzer.C+", sys+"DOWN sample=STtch");
	}
      }

  if (sample=="MC"||sample=="ST"||sample=="STtWch"||sample=="all")
    {
      TDSet *mc_STtWch = new TDSet("top","*","/PATNtupleMaker");
      mc_STtWch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/STtWch_Mu.root");
	if (proc =="all" ) {
		mc_STtWch->Process("Analyzer.C+","sample=STtWch");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_STtWch->Process("Analyzer.C+", sys+"UP sample=STtWch");
	   if (updown == "both" || updown == "down") mc_STtWch->Process("Analyzer.C+", sys+"DOWN sample=STtWch");
	}
      }

  if (sample=="MC"||sample=="ST"||sample=="STsch_bar"||sample=="all")
    {
      TDSet *mc_STsch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STsch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/STsch_bar_Mu.root");
	if (proc =="all" ) {
		mc_STsch_bar->Process("Analyzer.C+","sample=STsch_bar");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_STsch_bar->Process("Analyzer.C+", sys+"UP sample=STsch_bar");
	   if (updown == "both" || updown == "down") mc_STsch_bar->Process("Analyzer.C+", sys+"DOWN sample=STsch_bar");
	}
      }

  if (sample=="MC"||sample=="ST"||sample=="STtch_bar"||sample=="all")
    {
      TDSet *mc_STtch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STtch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/STtch_bar_Mu.root");
	if (proc =="all" ) {
		mc_STtch_bar->Process("Analyzer.C+","sample=STtch_bar");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_STtch_bar->Process("Analyzer.C+", sys+"UP sample=STtch_bar");
	   if (updown == "both" || updown == "down") mc_STtch_bar->Process("Analyzer.C+", sys+"DOWN sample=STtch_bar");
	}
      }

  if (sample=="MC"||sample=="ST"||sample=="STtWch_bar"||sample=="all")
    {
      TDSet *mc_STtWch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STtWch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/STtWch_bar_Mu.root");
	if (proc =="all" ) {
		mc_STtWch_bar->Process("Analyzer.C+","sample=STtWch_bar");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_STtWch_bar->Process("Analyzer.C+", sys+"UP sample=STtWch_bar");
	   if (updown == "both" || updown == "down") mc_STtWch_bar->Process("Analyzer.C+", sys+"DOWN sample=STtWch_bar");
	}
      }

  if (sample=="MC"||sample=="diboson"||sample=="WW"||sample=="all")
    {
      TDSet *mc_WW = new TDSet("top","*","/PATNtupleMaker");
      mc_WW->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/WW_Mu.root");
	if (proc =="all" ) {
		mc_WW->Process("Analyzer.C+","sample=WW");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_WW->Process("Analyzer.C+", sys+"UP sample=WW");
	   if (updown == "both" || updown == "down") mc_WW->Process("Analyzer.C+", sys+"DOWN sample=WW");
	}
      }

  if (sample=="MC"||sample=="diboson"||sample=="WZ"||sample=="all")
    {
      TDSet *mc_WZ = new TDSet("top","*","/PATNtupleMaker");
      mc_WZ->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/WZ_Mu.root");
	if (proc =="all" ) {
		mc_WZ->Process("Analyzer.C+","sample=WZ");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_WZ->Process("Analyzer.C+", sys+"UP sample=WZ");
	   if (updown == "both" || updown == "down") mc_WZ->Process("Analyzer.C+", sys+"DOWN sample=WZ");
	}
      }

  if (sample=="MC"||sample=="ZJets"||sample=="all")
    {
      TDSet *mc_ZJets = new TDSet("top","*","/PATNtupleMaker");
      mc_ZJets->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/ZJets_Mu.root");
	if (proc =="all" ) {
		mc_ZJets->Process("Analyzer.C+","sample=ZJets");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_ZJets->Process("Analyzer.C+", sys+"UP sample=ZJets");
	   if (updown == "both" || updown == "down") mc_ZJets->Process("Analyzer.C+", sys+"DOWN sample=ZJets");
	}
      }
  
  if (sample=="data"||sample=="all")
    {
      TDSet *data = new TDSet("top","*","/PATNtupleMaker");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_12/SingleMu_May10ReReco_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_12/SingleMu_PromptReco-v4_Run_165088_166967_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_12/SingleMu_PromptReco-v4_Run_167039_167913_Mu.root");
	      //data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_12/SingleMu_Aug05ReReco_Mu.root");
	      //data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_12/SingleMu_PromptReco-v6_Run_172620_173198_Mu.root");
	      //data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_12/SingleMu_PromptReco-v6_Run_173236_173692_Mu.root");
	      //data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_12/SingleMu_Run2011B-PromptReco-v1_175860_178380_Mu.root");
	      //data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_12/SingleMu_Run2011B-PromptReco-v1_178420_180252_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_12/SingleMu_Aug05ReReco_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_12/SingleMu_PromptReco-v6_Run_172620_173198_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_12/SingleMu_PromptReco-v6_Run_173236_173692_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_12/SingleMu_Run2011B-PromptReco-v1_175860_177452_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_12/SingleMu_Run2011B-PromptReco-v1_177718_178380_Mu.root");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_12/SingleMu_Run2011B-PromptReco-v1_178420_180252_Mu.root ");
      
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

  if (sample=="dataMissing"||sample=="all")
    {
      TDSet *data = new TDSet("top","*","/PATNtupleMaker");
      data->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/Data/2011Muon/42x_v9_12/missing.root ");

      data->Process("Analyzer.C+","sample=dataMissing");
      // get log files                                                     
                                                                                                                                                 
      if (getLogs)
        {
          logList = p->GetManager()->GetSessionLogs()->GetListOfLogs();
          for (int i=1; i< logList->GetSize(); ++i)
            {
              logElem = ( TProofLogElem* ) logList->At( i );
              macro = logElem->GetMacro();
              macro->SaveSource("data_missing_"+TString(Form("%i",i))+".stdout");
            }
        }
    }

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
 /* 
  if (sample=="4Top"||sample=="4Top1100"||sample=="all")
    {
      TDSet *mc_4Top_1100 = new TDSet("top","*","/PATNtupleMaker");
      mc_4Top_1100->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/4Top_1100_Tuple-PATskim.root");
	if (proc =="all" ) {
		mc_4Top_1100->Process("Analyzer.C+","sample=4Top1100");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_4Top_1100->Process("Analyzer.C+", sys+"UP sample=4Top1100");
	   if (updown == "both" || updown == "down") mc_4Top_1100->Process("Analyzer.C+", sys+"DOWN sample=4Top1100");
	}
      }
    
  if (sample=="4Top"||sample=="4Top900"||sample=="all")
    {
      TDSet *mc_4Top_900 = new TDSet("top","*","/PATNtupleMaker");
      mc_4Top_900->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/4Top_900_Tuple-PATskim.root");
	if (proc =="all" ) {
		mc_4Top_900->Process("Analyzer.C+","sample=4Top900");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_4Top_900->Process("Analyzer.C+", sys+"UP sample=4Top900");
	   if (updown == "both" || updown == "down") mc_4Top_900->Process("Analyzer.C+", sys+"DOWN sample=4Top900");
	}
      }
    
    
  if (sample=="4Top"||sample=="4TopUED6"||sample=="all")
    {
      TDSet *mc_4Top_UED6 = new TDSet("top","*","/PATNtupleMaker");
      mc_4Top_UED6->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/4TopUED6-Tuple-PATskim.root");
	if (proc =="all" ) {
		mc_4Top_UED6->Process("Analyzer.C+","sample=4TopUED6");
	}
	if ( proc =="sys" ) {
	   if (updown == "both" || updown == "up") mc_4Top_UED6->Process("Analyzer.C+", sys+"UP sample=4TopUED6");
	   if (updown == "both" || updown == "down") mc_4Top_UED6->Process("Analyzer.C+", sys+"DOWN sample=4TopUED6");
	}
      }*/
    
