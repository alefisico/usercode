//void runAnalysis(TString sample="all", TString proc="all", TString sys="all", TString updown="both", bool getLogs=false)  
#include "TChain.h"

void runAnalyzerLPC(TString sample="all", TString st2="250", TString st1="100", TString proc="bj"){

	if (sample=="signal"){
		TString inputFiles = "st2_h_bb_st1_"+proc+"_"+st2+"_"+st1;
		TChain chain("EvTree");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/Signal/"+inputFiles+"_tree.root");
		cout << "||-------- Processing the File: " << inputFiles << endl;
		chain.Process("/uscms/home/algomez/work/CMSSW_5_3_3/src/RUAnalysis/MyAnalyzer/test/MyTreeAnalyzer.C++","sample="+inputFiles+" st1="+st1);
	}


	if (sample=="HT250-500"){
		TChain chain("EvTree");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_0/HT250-500_0_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_0/HT250-500_1_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_0/HT250-500_2_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_0/HT250-500_3_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_0/HT250-500_4_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_0/HT250-500_5_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_0/HT250-500_6_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_0/HT250-500_7_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_0/HT250-500_8_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_0/HT250-500_9_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_0/HT250-500_40_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_1/HT250-500_10_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_1/HT250-500_11_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_1/HT250-500_12_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_1/HT250-500_13_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_1/HT250-500_14_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_1/HT250-500_15_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_1/HT250-500_16_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_1/HT250-500_17_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_1/HT250-500_18_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_1/HT250-500_19_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_1/HT250-500_41_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_2/HT250-500_20_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_2/HT250-500_21_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_2/HT250-500_22_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_2/HT250-500_23_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_2/HT250-500_24_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_2/HT250-500_25_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_2/HT250-500_26_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_2/HT250-500_27_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_2/HT250-500_28_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_2/HT250-500_29_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_2/HT250-500_42_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_3/HT250-500_30_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_3/HT250-500_31_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_3/HT250-500_32_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_3/HT250-500_33_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_3/HT250-500_34_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_3/HT250-500_35_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_3/HT250-500_36_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_3/HT250-500_37_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_3/HT250-500_38_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_3/HT250-500_39_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT250-500/Part_3/HT250-500_43_tree.root");
		chain.Process("/users/h2/gomez/work/CMSSW_5_3_3/src/RUAnalysis/MyAnalyzer/test/MyTreeAnalyzer.C++", "sample="+sample);
	}

	if (sample=="HT500-1000"){
		TChain chain("EvTree");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_0/HT500-1000_0_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_0/HT500-1000_10_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_0/HT500-1000_11_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_0/HT500-1000_12_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_0/HT500-1000_13_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_1/HT500-1000_14_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_1/HT500-1000_15_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_1/HT500-1000_16_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_1/HT500-1000_17_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_1/HT500-1000_18_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_2/HT500-1000_19_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_2/HT500-1000_1_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_2/HT500-1000_20_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_2/HT500-1000_21_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_2/HT500-1000_22_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_3/HT500-1000_23_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_3/HT500-1000_24_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_3/HT500-1000_25_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_3/HT500-1000_26_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_3/HT500-1000_27_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_4/HT500-1000_28_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_4/HT500-1000_29_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_4/HT500-1000_2_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_4/HT500-1000_30_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_4/HT500-1000_31_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_5/HT500-1000_32_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_5/HT500-1000_33_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_5/HT500-1000_34_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_5/HT500-1000_35_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_5/HT500-1000_36_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_6/HT500-1000_37_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_6/HT500-1000_38_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_6/HT500-1000_39_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_6/HT500-1000_3_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_6/HT500-1000_40_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_7/HT500-1000_41_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_7/HT500-1000_42_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_7/HT500-1000_43_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_7/HT500-1000_44_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_7/HT500-1000_45_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_8/HT500-1000_4_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_8/HT500-1000_5_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_8/HT500-1000_6_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_8/HT500-1000_7_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_8/HT500-1000_8_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT500-1000/Part_9/HT500-1000_9_tree.root");
		chain.Process("/users/h2/gomez/work/CMSSW_5_3_3/src/RUAnalysis/MyAnalyzer/test/MyTreeAnalyzer.C++", "sample="+sample);
	}


	if (sample=="HT1000-Inf"){
		TChain chain("EvTree");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_0/HT1000-Inf_0_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_0/HT1000-Inf_10_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_0/HT1000-Inf_11_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_0/HT1000-Inf_12_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_0/HT1000-Inf_13_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_0/HT1000-Inf_14_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_0/HT1000-Inf_15_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_0/HT1000-Inf_16_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_0/HT1000-Inf_17_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_0/HT1000-Inf_18_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_1/HT1000-Inf_19_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_1/HT1000-Inf_1_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_1/HT1000-Inf_20_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_1/HT1000-Inf_21_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_1/HT1000-Inf_22_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_1/HT1000-Inf_23_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_1/HT1000-Inf_24_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_1/HT1000-Inf_25_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_1/HT1000-Inf_26_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_1/HT1000-Inf_27_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_2/HT1000-Inf_28_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_2/HT1000-Inf_29_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_2/HT1000-Inf_2_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_2/HT1000-Inf_30_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_2/HT1000-Inf_31_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_2/HT1000-Inf_32_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_2/HT1000-Inf_33_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_2/HT1000-Inf_34_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_2/HT1000-Inf_35_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_2/HT1000-Inf_36_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_3/HT1000-Inf_37_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_3/HT1000-Inf_38_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_3/HT1000-Inf_39_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_3/HT1000-Inf_3_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_3/HT1000-Inf_40_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_3/HT1000-Inf_41_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_3/HT1000-Inf_42_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_3/HT1000-Inf_43_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_3/HT1000-Inf_44_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_3/HT1000-Inf_45_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_4/HT1000-Inf_4_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_4/HT1000-Inf_5_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_4/HT1000-Inf_6_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_4/HT1000-Inf_7_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_4/HT1000-Inf_8_tree.root");
		chain.Add("/eos/uscms/store/user/algomez/Stops/Ntuples/HT1000-Inf/Part_4/HT1000-Inf_9_tree.root");
		chain.Process("/users/h2/gomez/work/CMSSW_5_3_3/src/RUAnalysis/MyAnalyzer/test/MyTreeAnalyzer.C++", "sample="+sample);
	}
	//////////////// TEST PROOF
	//TString desdir = "/cms/gomez/Stops/Results/";
	//TProof *p = TProof::Open("lite://", desdir ,desdir);
	//TProof::AddEnvVar("PROOF_WRAPPERCMD", "valgrind_opts:--leak-check=full");
	//TProof *p = TProof::Open("workers=2","valgrind");

	//p->AddDynamicPath("");
	//      p->Exec("gSystem->Load(\"/uscms/home/algomez/work/CMSSW_4_2_4/lib/slc5_amd64_gcc434/libYumicevaTop7TeV.so\")");
	//        p->Exec("gSystem->Load(\"/uscms/home/algomez/work/CMSSW_4_2_4/lib/slc5_amd64_gcc434/libCondFormatsJetMETObjects.so\")");
	//p->AddIncludePath("/users/h2/gomez/work/CMSSW_5_3_3/src/");

	//p->Archive(" ",desdir);

	//p->AddInput(new TNamed("PROOF_OUTPUTFILE_LOCATION", "LOCAL"));

	//if (sample=="tttt"|| sample=="ttttSM"||sample=="all"){

		//TDSet *mc_st2_h_bb_st1_bj_450_200 = new TDSet("EvTree","*","EvTree");
		//TDSet *mc_st2_h_bb_st1_bj_450_200 = new TDSet("EvTree");
		//mc_st2_h_bb_st1_bj_450_200->Add("/cms/gomez/Stops/Ntuples/st2_h_bb_st1_bj_450_200_tree.root");
		//mc_st2_h_bb_st1_bj_450_200->Process("/users/h2/gomez/work/CMSSW_5_3_3/src/RUAnalysis/MyAnalyzer/src/MyTreeAnalyzer.C++");
		/*if (proc =="all" ) mc_tttt_SM->Process("Analyzer.C+","sample=tttt_SM");
		if ( proc =="sys" ) {
			if (updown == "both" || updown == "up") mc_tttt_SM->Process("Analyzer.C+", sys+"UP sample=tttt_SM");
			if (updown == "both" || updown == "down") mc_tttt_SM->Process("Analyzer.C+", sys+"DOWN sample=tttt_SM");
		}*/
	//}
}
