//void runAnalysis(TString sample="all", bool JEC=true, bool down=true, bool BDT=true, bool getLogs=false)  
void runAnalysis(TString sample="all", TString sys="all", bool down=true, bool BDT=true, bool getLogs=false)  
{
  TString desdir = "/uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/";
  TProof *p = TProof::Open("lite://", desdir ,desdir);

  //p->AddDynamicPath("");
  p->Exec("gSystem->Load(\"/uscms/home/algomez/work/CMSSW_4_2_4/lib/slc5_amd64_gcc434/libYumicevaTop7TeV.so\")");
  p->Exec("gSystem->Load(\"/uscms/home/algomez/work/CMSSW_4_2_4/lib/slc5_amd64_gcc434/libCondFormatsJetMETObjects.so\")");
  p->AddIncludePath("/uscms/home/algomez/work/CMSSW_4_2_4/src/");

  p->Archive(" ",desdir);

  //p->AddInput(new TNamed("PROOF_OUTPUTFILE_LOCATION", "LOCAL"));
 
  if (sample=="Gh"||sample=="4Top1100"||sample=="all")
    {
      TDSet *mc_4Top_1100 = new TDSet("top","*","/PATNtupleMaker");
      mc_4Top_1100->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/4Top_1100_Tuple-PATskim.root");
      if (sys =="all" ) {
	if (!BDT) mc_4Top_1100->Process("Analyzer.C+","sample=4Top1100");
	else mc_4Top_1100->Process("Analyzer.C+","BDT sample=4Top1100");
      } else {
	if ( !down ) {
	   if (!BDT) mc_4Top_1100->Process("Analyzer.C+", sys+"UP sample=4Top1100");
	   else mc_4Top_1100->Process("Analyzer.C+", sys+"UP BDT sample=4Top1100");
	} else {
	   if (!BDT) mc_4Top_1100->Process("Analyzer.C+", sys+"DOWN sample=4Top1100");
	   else mc_4Top_1100->Process("Analyzer.C+", sys+"DOWN BDT sample=4Top1100");
	}
      }
    }
  if (sample=="Gh"||sample=="4Top1000"||sample=="all")
    {
      TDSet *mc_4Top_1000 = new TDSet("top","*","/PATNtupleMaker");
      mc_4Top_1000->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/4Top_1000_new_Tuple-PATskim.root");
      if (sys =="all" ) {
	if (!BDT) mc_4Top_1000->Process("Analyzer.C+","sample=4Top1000");
	else mc_4Top_1000->Process("Analyzer.C+","BDT sample=4Top1000");
      } else {
	if (!down) {
	   if (!BDT) mc_4Top_1000->Process("Analyzer.C+", sys+"UP sample=4Top1000");
	   else mc_4Top_1000->Process("Analyzer.C+", sys+"UP BDT sample=4Top1000");
	} else {
	   if (!BDT) mc_4Top_1000->Process("Analyzer.C+", sys+"DOWN sample=4Top1000");
	   else mc_4Top_1000->Process("Analyzer.C+", sys+"DOWN BDT sample=4Top1000");
	}
      }
    }

  if (sample=="Gh"||sample=="4Top900"||sample=="all")
    {
      TDSet *mc_4Top_900 = new TDSet("top","*","/PATNtupleMaker");
      mc_4Top_900->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/4Top_900_Tuple-PATskim.root");
      if (sys =="all" ) {
	if (!BDT) mc_4Top_900->Process("Analyzer.C+","sample=4Top900");
	else mc_4Top_900->Process("Analyzer.C+","BDT sample=4Top900");
      } else {
	if (!down) {
	   if (!BDT) mc_4Top_900->Process("Analyzer.C+", sys+"UP sample=4Top900");
	   else mc_4Top_900->Process("Analyzer.C+", sys+"UP BDT sample=4Top900");
	} else {
	   if (!BDT) mc_4Top_900->Process("Analyzer.C+", sys+"DOWN sample=4Top900");
	   else mc_4Top_900->Process("Analyzer.C+", sys+"DOWN BDT sample=4Top900");
	}
      }
    }

  if (sample=="Gh"||sample=="4Top700"||sample=="all")
    {
      TDSet *mc_4Top_700 = new TDSet("top","*","/PATNtupleMaker");
      mc_4Top_700->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/4Top_700_TUple-PATskim.root");
      if (sys=="all") {
	if (!BDT) mc_4Top_700->Process("Analyzer.C+","sample=4Top700");
	else mc_4Top_700->Process("Analyzer.C+","BDT sample=4Top700");
      } else {
	if (!down) {
	   if (!BDT) mc_4Top_700->Process("Analyzer.C+", sys+"UP sample=4Top700");
	   else mc_4Top_700->Process("Analyzer.C+", sys+"UP BDT sample=4Top700");
	} else {
	   if (!BDT) mc_4Top_700->Process("Analyzer.C+", sys+"DOWN sample=4Top700");
	   else mc_4Top_700->Process("Analyzer.C+", sys+"DOWN BDT sample=4Top700");
	}
      }
    } 

  if (sample=="Gh"||sample=="4Top500"||sample=="all")
    {
      TDSet *mc_4Top_500 = new TDSet("top","*","/PATNtupleMaker");
      mc_4Top_500->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/4Top_500_new_Tuple-PATskim.root");
      if (sys=="all") {
	if (!BDT) mc_4Top_500->Process("Analyzer.C+","sample=4Top500");
	else mc_4Top_500->Process("Analyzer.C+","BDT sample=4Top500");
      } else {
	if ( !down ) {
	   if (!BDT) mc_4Top_500->Process("Analyzer.C+", sys+"UP sample=4Top500");
	   else mc_4Top_500->Process("Analyzer.C+", sys+"UP BDT sample=4Top500");
	} else {
	   if (!BDT) mc_4Top_500->Process("Analyzer.C+", sys+"DOWN sample=4Top500");
	   else mc_4Top_500->Process("Analyzer.C+", sys+"DOWN BDT sample=4Top500");
	}
      }
    }

  if (sample=="MC"||sample=="ttbar"||sample=="all")
    {
      TDSet *mc_ttbar = new TDSet("top","*","/PATNtupleMaker");
      mc_ttbar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/TTbar_Mu.root");
      if (sys=="all") {
	if (!BDT) mc_ttbar->Process("Analyzer.C+","sample=ttbar");
	else mc_ttbar->Process("Analyzer.C+","BDT sample=ttbar");
      } else {
	if (!down) {
	   if (!BDT) mc_ttbar->Process("Analyzer.C+", sys+"UP sample=ttbar");
	   else mc_ttbar->Process("Analyzer.C+", sys+"UP BDT sample=ttbar");
	} else {
	   if (!BDT) mc_ttbar->Process("Analyzer.C+", sys+"DOWN sample=ttbar");
	   else mc_ttbar->Process("Analyzer.C+", sys+"DOWN BDT sample=ttbar");
	}
      }
    }

  if (sample=="MC"||sample=="WJets"||sample=="all")
    {
      TDSet *mc_WJets = new TDSet("top","*","/PATNtupleMaker");
      mc_WJets->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/WJets_Mu.root");
      if (sys=="all") {
	if (!BDT) mc_WJets->Process("Analyzer.C+","sample=WJets");
	else mc_WJets->Process("Analyzer.C+","BDT sample=WJets");
      } else {
	if (!down) {
	   if (!BDT) mc_WJets->Process("Analyzer.C+", sys+"UP sample=WJets");
	   else mc_WJets->Process("Analyzer.C+", sys+"UP BDT sample=WJets");
	} else {
	   if (!BDT) mc_WJets->Process("Analyzer.C+", sys+"DOWN sample=WJets");
	   else mc_WJets->Process("Analyzer.C+", sys+"DOWN BDT sample=WJets");
	}
      }
    }

  if (sample=="MC"||sample=="QCD"||sample=="all")
    {
      TDSet *mc_QCD = new TDSet("top","*","/PATNtupleMaker");
      mc_QCD->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/QCD_Mu.root");
      if (sys=="all") {
	if (!BDT) mc_QCD->Process("Analyzer.C+","sample=QCD");
	else mc_QCD->Process("Analyzer.C+","BDT sample=QCD");
      } else {
	if (!down) {
	   if (!BDT) mc_QCD->Process("Analyzer.C+", sys+"UP sample=QCD");
	   else mc_QCD->Process("Analyzer.C+", sys+"UP BDT sample=QCD");
	} else {
	   if (!BDT) mc_QCD->Process("Analyzer.C+", sys+"DOWN sample=QCD");
	   else mc_QCD->Process("Analyzer.C+", sys+"DOWN BDT sample=QCD");
	}
      }
    }

  if (sample=="MC"||sample=="STsch"||sample=="all")
    {
      TDSet *mc_STsch = new TDSet("top","*","/PATNtupleMaker");
      mc_STsch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/STsch_Mu.root");
      if (sys=="all") {
	if (!BDT) mc_STsch->Process("Analyzer.C+","sample=STsch");
	else mc_STsch->Process("Analyzer.C+","BDT sample=STsch");
      } else {
	if (!down) {
	   if (!BDT) mc_STsch->Process("Analyzer.C+", sys+"UP sample=STsch");
	   else mc_STsch->Process("Analyzer.C+", sys+"UP BDT sample=STsch");
	} else {
	   if (!BDT) mc_STsch->Process("Analyzer.C+", sys+"DOWN sample=STsch");
	   else mc_STsch->Process("Analyzer.C+", sys+"DOWN BDT sample=STsch");
	}
      }
    }

  if (sample=="MC"||sample=="STtch"||sample=="all")
    {
      TDSet *mc_STtch = new TDSet("top","*","/PATNtupleMaker");
      mc_STtch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/STtch_Mu.root");
      if (sys=="all") {
	if (!BDT) mc_STtch->Process("Analyzer.C+","sample=STtch");
	else mc_STtch->Process("Analyzer.C+","BDT sample=STtch");
      } else {
	if (!down) {
	   if (!BDT) mc_STtch->Process("Analyzer.C+", sys+"UP sample=STtch");
	   else mc_STtch->Process("Analyzer.C+", sys+"UP BDT sample=STtch");
	} else {
	   if (!BDT) mc_STtch->Process("Analyzer.C+", sys+"DOWN sample=STtch");
	   else mc_STtch->Process("Analyzer.C+", sys+"DOWN BDT sample=STtch");
	}
      }
    }

  if (sample=="MC"||sample=="STtWch"||sample=="all")
    {
      TDSet *mc_STtWch = new TDSet("top","*","/PATNtupleMaker");
      mc_STtWch->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/STtWch_Mu.root");
      if (sys=="all") {
	if (!BDT) mc_STtWch->Process("Analyzer.C+","sample=STtWch");
	else mc_STtWch->Process("Analyzer.C+","BDT sample=STtWch");
      } else {
	if (!down) {
	   if (!BDT) mc_STtWch->Process("Analyzer.C+", sys+"UP sample=STtWch");
	   else mc_STtWch->Process("Analyzer.C+", sys+"UP BDT sample=STtWch");
	} else {
	   if (!BDT) mc_STtWch->Process("Analyzer.C+", sys+"DOWN sample=STtWch");
	   else mc_STtWch->Process("Analyzer.C+", sys+"DOWN BDT sample=STtWch");
	}
      }
    }

  if (sample=="MC"||sample=="STsch_bar"||sample=="all")
    {
      TDSet *mc_STsch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STsch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/STsch_bar_Mu.root");
      if (sys=="all") {
	if (!BDT) mc_STsch_bar->Process("Analyzer.C+","sample=STsch_bar");
	else mc_STsch_bar->Process("Analyzer.C+","BDT sample=STsch_bar");
      } else {
	if (!down) {
	   if (!BDT) mc_STsch_bar->Process("Analyzer.C+", sys+"UP sample=STsch_bar");
	   else mc_STsch_bar->Process("Analyzer.C+", sys+"UP BDT sample=STsch_bar");
	} else {
	   if (!BDT) mc_STsch_bar->Process("Analyzer.C+", sys+"DOWN sample=STsch_bar");
	   else mc_STsch_bar->Process("Analyzer.C+", sys+"DOWN BDT sample=STsch_bar");
	}
      }
    }

  if (sample=="MC"||sample=="STtch_bar"||sample=="all")
    {
      TDSet *mc_STtch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STtch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/STtch_bar_Mu.root");
      if (sys=="all") {
	if (!BDT) mc_STtch_bar->Process("Analyzer.C+","sample=STtch_bar");
	else mc_STtch_bar->Process("Analyzer.C+","BDT sample=STtch_bar");
      } else {
	if (!down) {
	   if (!BDT) mc_STtch_bar->Process("Analyzer.C+", sys+"UP sample=STtch_bar");
	   else mc_STtch_bar->Process("Analyzer.C+", sys+"UP BDT sample=STtch_bar");
	} else {
	   if (!BDT) mc_STtch_bar->Process("Analyzer.C+", sys+"DOWN sample=STtch_bar");
	   else mc_STtch_bar->Process("Analyzer.C+", sys+"DOWN BDT sample=STtch_bar");
	}
      }
    }

  if (sample=="MC"||sample=="STtWch_bar"||sample=="all")
    {
      TDSet *mc_STtWch_bar = new TDSet("top","*","/PATNtupleMaker");
      mc_STtWch_bar->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/STtWch_bar_Mu.root");
      if (sys=="all") {
	if (!BDT) mc_STtWch_bar->Process("Analyzer.C+","sample=STtWch_bar");
	else mc_STtWch_bar->Process("Analyzer.C+","BDT sample=STtWch_bar");
      } else {
	if (!down) {
	   if (!BDT) mc_STtWch_bar->Process("Analyzer.C+", sys+"UP sample=STtWch_bar");
	   else mc_STtWch_bar->Process("Analyzer.C+", sys+"UP BDT sample=STtWch_bar");
	} else {
	   if (!BDT) mc_STtWch_bar->Process("Analyzer.C+", sys+"DOWN sample=STtWch_bar");
	   else mc_STtWch_bar->Process("Analyzer.C+", sys+"DOWN BDT sample=STtWch_bar");
	}
      }
    }

  if (sample=="MC"||sample=="WW"||sample=="all")
    {
      TDSet *mc_WW = new TDSet("top","*","/PATNtupleMaker");
      mc_WW->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/WW_Mu.root");
      if (sys=="all") {
	if (!BDT) mc_WW->Process("Analyzer.C+","sample=WW");
	else mc_WW->Process("Analyzer.C+","BDT sample=WW");
      } else {
	if (!down) {
	   if (!BDT) mc_WW->Process("Analyzer.C+", sys+"UP sample=WW");
	   else mc_WW->Process("Analyzer.C+", sys+"UP BDT sample=WW");
	} else {
	   if (!BDT) mc_WW->Process("Analyzer.C+", sys+"DOWN sample=WW");
	   else mc_WW->Process("Analyzer.C+", sys+"DOWN BDT sample=WW");
	}
      }
    }

  if (sample=="MC"||sample=="WZ"||sample=="all")
    {
      TDSet *mc_WZ = new TDSet("top","*","/PATNtupleMaker");
      mc_WZ->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/WZ_Mu.root");
      if (sys=="all") {
	if (!BDT) mc_WZ->Process("Analyzer.C+","sample=WZ");
	else mc_WZ->Process("Analyzer.C+","BDT sample=WZ");
      } else {
	if (!down) {
	   if (!BDT) mc_WZ->Process("Analyzer.C+", sys+"UP sample=WZ");
	   else mc_WZ->Process("Analyzer.C+", sys+"UP BDT sample=WZ");
	} else {
	   if (!BDT) mc_WZ->Process("Analyzer.C+", sys+"DOWN sample=WZ");
	   else mc_WZ->Process("Analyzer.C+", sys+"DOWN BDT sample=WZ");
	}
      }
    }

  if (sample=="MC"||sample=="ZJets"||sample=="all")
    {
      TDSet *mc_ZJets = new TDSet("top","*","/PATNtupleMaker");
      mc_ZJets->Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/v9_14/ZJets_Mu.root");
      if (sys=="all") {
	if (!BDT) mc_ZJets->Process("Analyzer.C+","sample=ZJets");
	else mc_ZJets->Process("Analyzer.C+","BDT sample=ZJets");
      } else {
	if (!down) {
	   if (!BDT) mc_ZJets->Process("Analyzer.C+", sys+"UP sample=ZJets");
	   else mc_ZJets->Process("Analyzer.C+", sys+"UP BDT sample=ZJets");
	} else {
	   if (!BDT) mc_ZJets->Process("Analyzer.C+", sys+"DOWN sample=ZJets");
	   else mc_ZJets->Process("Analyzer.C+", sys+"DOWN BDT sample=ZJets");
	}
      }
    }

  if (sample=="MC"||sample=="4TopSM"||sample=="all")
    {
      TDSet *mc_4TopSM = new TDSet("top","*","/PATNtupleMaker");
      mc_4TopSM->Add("/eos/uscms/store/user/algomez/FourTop/Ntuples/4TopSM-Tuple-PATskim.root");
      if (sys =="all" ) {
	if (!BDT) mc_4TopSM->Process("Analyzer.C+","sample=4TopSM");
	else mc_4TopSM->Process("Analyzer.C+","BDT sample=4TopSM");
      } else {
	if ( !down ) {
	   if (!BDT) mc_4TopSM->Process("Analyzer.C+", sys+"UP sample=4TopSM");
	   else mc_4TopSM->Process("Analyzer.C+", sys+"UP BDT sample=4TopSM");
	} else {
	   if (!BDT) mc_4TopSM->Process("Analyzer.C+", sys+"DOWN sample=4TopSM");
	   else mc_4TopSM->Process("Analyzer.C+", sys+"DOWN BDT sample=4TopSM");
	}
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
      
      if (!BDT) data->Process("Analyzer.C+","sample=data");
      else data->Process("Analyzer.C+","BDT sample=data");
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

      if (!BDT) data->Process("Analyzer.C+","sample=dataMissing");
      else data->Process("Analyzer.C+","BDT sample=dataMissing");
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
      if (!BDT) dataQCD2->Process("Analyzer.C+","QCD2 sample=data");
      else dataQCD2->Process("Analyzer.C+","BDT QCD2 sample=data");
    }
}
