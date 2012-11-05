#!/bin/bash 
#sample=(0050 0075 0100 0125 0150 0175 0200)
sample=(0225 0250 0275 0300 0325 0350)
#num=(0.05 0.075 0.1 0.125 0.150 0.175 0.2)
num=(0.225 0.250 0.275 0.3 0.325 0.35)
version='UED6'
results='results110/Nbjets3/'

sample_elements=${#sample[@]}
num_elements=${#num[@]}
for ((i=0;i<$sample_elements;i++));
do
	OUTFILE="BDT_cuy_tmp_${sample[${i}]}.xml"
	echo "<cuy>
  <validation type=\"data\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_data.root\" weight=\"1\">
    <TH1 name=\"mu_stjet${sample[${i}]}__DATA\" source=\"/MET/Stjet_cut2_data\"/>
  </validation>

  <validation type=\"tttt_UED6${sample[${i}]}\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_tttt_UED6.root\" weight=\"MCScaleFactors.txt:tttt_UED6${sample[${i}]}\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}\" source=\"/MET/Stjet_cut2_ttttUED6\"/>
  </validation>

  <validation type=\"ttbar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ttbar.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__ttbar\" source=\"/MET/Stjet_cut2_ttbar\"/>
  </validation>

  <!--<validation type=\"WJets\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_WJets.root\" weight=\"MCScaleFactors.txt:WJets\">
    <TH1 name=\"mu_stjet${sample[${i}]}__wjets\" source=\"/MET/Stjet_cut2_WJets\"/>
  </validation>-->

  <validation type=\"Wbb\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wbb.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_stjet${sample[${i}]}__Wbb\" source=\"/MET/Stjet_cut2_Wbb\"/>
  </validation>

  <validation type=\"Wcc\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wcc.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_stjet${sample[${i}]}__Wcc\" source=\"/MET/Stjet_cut2_Wcc\"/>
  </validation>

  <validation type=\"Wqq\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wqq.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_stjet${sample[${i}]}__Wqq\" source=\"/MET/Stjet_cut2_Wqq\"/>
  </validation>

  <validation type=\"ZJets\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ZJets.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_stjet${sample[${i}]}__zjets\" source=\"/MET/Stjet_cut2_ZJets\"/>
  </validation>

  <validation type=\"STtch\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtch\" source=\"/MET/Stjet_cut2_STtch\"/>
  </validation>

  <validation type=\"STtch_bar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_bar.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtch_bar\" source=\"/MET/Stjet_cut2_STtch_bar\"/>
  </validation>

  <validation type=\"STtWch\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWch\" source=\"/MET/Stjet_cut2_STtWch\"/>
  </validation>

  <validation type=\"STtWch_bar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_bar.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWch_bar\" source=\"/MET/Stjet_cut2_STtWch_bar\"/>
  </validation>

  <validation type=\"STsch\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STsch.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STsch\" source=\"/MET/Stjet_cut2_STsch\"/>
  </validation>


  <additionArray name=\"singletop\" title=\"singletop\">
    <additionArrayItem array=\"STtch\"/>
    <additionArrayItem array=\"STtWch\"/>
    <additionArrayItem array=\"STsch\"/>
    <additionArrayItem array=\"STtch_bar\"/>
    <additionArrayItem array=\"STtWch_bar\"/>
  </additionArray>


  <additionArray name=\"wjets\" title=\"wjets\">
    <additionArrayItem array=\"Wbb\" weight=\"1.21\"/>
    <additionArrayItem array=\"Wcc\" weight=\"1.66\"/>
    <additionArrayItem array=\"Wqq\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_stjet${sample[${i}]}\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\"  >
    <superimposeItem name=\"mu_stjet${sample[${i}]}__DATA\" color=\"top\" legend=\"Data\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__ttbar\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__wjets\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__zjets\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__singletop\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\" />
  </superimpose>

  <!-- JERDOWN -->
  <validation type=\"tttt_UED6${sample[${i}]}JERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_tttt_UED6_JERDOWN.root\" weight=\"MCScaleFactors.txt:tttt_UED6${sample[${i}]}\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__jer__minus\" source=\"/MET/Stjet_cut2_ttttUED6_JERDOWN\"/>
  </validation>

  <validation type=\"ttbarJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ttbar_JERDOWN.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__ttbar__jer__minus\" source=\"/MET/Stjet_cut2_ttbar_JERDOWN\"/>
  </validation>

  <validation type=\"WbbJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wbb_JERDOWN.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WbbJERDOWN\" source=\"/MET/Stjet_cut2_Wbb_JERDOWN\"/>
  </validation>

  <validation type=\"WccJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wcc_JERDOWN.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WccJERDOWN\" source=\"/MET/Stjet_cut2_Wcc_JERDOWN\"/>
  </validation>

  <validation type=\"WqqJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wqq_JERDOWN.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WqqJERDOWN\" source=\"/MET/Stjet_cut2_Wqq_JERDOWN\"/>
  </validation>

  <validation type=\"ZJetsJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ZJets_JERDOWN.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_stjet${sample[${i}]}__zjets__jer__minus\" source=\"/MET/Stjet_cut2_ZJets_JERDOWN\"/>
  </validation>

  <validation type=\"STtchJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_JERDOWN.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtchJERDOWN\" source=\"/MET/Stjet_cut2_STtch_JERDOWN\"/>
  </validation>

  <validation type=\"STtch_barJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_bar_JERDOWN.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtch_barJERDOWN\" source=\"/MET/Stjet_cut2_STtch_bar_JERDOWN\"/>
  </validation>

  <validation type=\"STtWchJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_JERDOWN.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWchJERDOWN\" source=\"/MET/Stjet_cut2_STtWch_JERDOWN\"/>
  </validation>

  <validation type=\"STtWch_barJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_bar_JERDOWN.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWch_barJERDOWN\" source=\"/MET/Stjet_cut2_STtWch_bar_JERDOWN\"/>
  </validation>

  <validation type=\"STschJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STsch_JERDOWN.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STschJERDOWN\" source=\"/MET/Stjet_cut2_STsch_JERDOWN\"/>
  </validation>

  <additionArray name=\"singletop__jer__minus\" title=\"singletop__jer__minus\">
    <additionArrayItem array=\"STtchJERDOWN\"/>
    <additionArrayItem array=\"STtWchJERDOWN\"/>
    <additionArrayItem array=\"STschJERDOWN\"/>
    <additionArrayItem array=\"STtch_barJERDOWN\"/>
    <additionArrayItem array=\"STtWch_barJERDOWN\"/>
  </additionArray>


  <additionArray name=\"wjets__jer__minus\" title=\"wjets__jer__minus\">
    <additionArrayItem array=\"WbbJERDOWN\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccJERDOWN\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqJERDOWN\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_stjet${sample[${i}]}__jer__minus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_stjet${sample[${i}]}__ttbar__jer__minus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__wjets__jer__minus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__zjets__jer__minus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__singletop__jer__minus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__jer__minus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- JERUP -->
  <validation type=\"tttt_UED6${sample[${i}]}JERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_tttt_UED6_JERUP.root\" weight=\"MCScaleFactors.txt:tttt_UED6${sample[${i}]}\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__jer__plus\" source=\"/MET/Stjet_cut2_ttttUED6_JERUP\"/>
  </validation>

  <validation type=\"ttbarJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ttbar_JERUP.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__ttbar__jer__plus\" source=\"/MET/Stjet_cut2_ttbar_JERUP\"/>
  </validation>

  <validation type=\"WbbJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wbb_JERUP.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WbbJERUP\" source=\"/MET/Stjet_cut2_Wbb_JERUP\"/>
  </validation>

  <validation type=\"WccJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wcc_JERUP.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WccJERUP\" source=\"/MET/Stjet_cut2_Wcc_JERUP\"/>
  </validation>

  <validation type=\"WqqJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wqq_JERUP.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WqqJERUP\" source=\"/MET/Stjet_cut2_Wqq_JERUP\"/>
  </validation>

  <validation type=\"ZJetsJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ZJets_JERUP.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_stjet${sample[${i}]}__zjets__jer__plus\" source=\"/MET/Stjet_cut2_ZJets_JERUP\"/>
  </validation>

  <validation type=\"STtchJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_JERUP.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtchJERUP\" source=\"/MET/Stjet_cut2_STtch_JERUP\"/>
  </validation>

  <validation type=\"STtch_barJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_bar_JERUP.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtch_barJERUP\" source=\"/MET/Stjet_cut2_STtch_bar_JERUP\"/>
  </validation>

  <validation type=\"STtWchJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_JERUP.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWchJERUP\" source=\"/MET/Stjet_cut2_STtWch_JERUP\"/>
  </validation>

  <validation type=\"STtWch_barJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_bar_JERUP.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWch_barJERUP\" source=\"/MET/Stjet_cut2_STtWch_bar_JERUP\"/>
  </validation>

  <validation type=\"STschJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STsch_JERUP.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STschJERUP\" source=\"/MET/Stjet_cut2_STsch_JERUP\"/>
  </validation>

  <additionArray name=\"singletop__jer__plus\" title=\"singletop__jer__plus\">
    <additionArrayItem array=\"STtchJERUP\"/>
    <additionArrayItem array=\"STtWchJERUP\"/>
    <additionArrayItem array=\"STschJERUP\"/>
    <additionArrayItem array=\"STtch_barJERUP\"/>
    <additionArrayItem array=\"STtWch_barJERUP\"/>
  </additionArray>


  <additionArray name=\"wjets__jer__plus\" title=\"wjets__jer__plus\">
    <additionArrayItem array=\"WbbJERUP\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccJERUP\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqJERUP\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_stjet${sample[${i}]}__jer__plus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_stjet${sample[${i}]}__ttbar__jer__plus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__wjets__jer__plus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__zjets__jer__plus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__singletop__jer__plus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__jer__plus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- JECDOWN -->
  <validation type=\"tttt_UED6${sample[${i}]}JECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_tttt_UED6_JECDOWN.root\" weight=\"MCScaleFactors.txt:tttt_UED6${sample[${i}]}\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__jes__minus\" source=\"/MET/Stjet_cut2_ttttUED6_JECDOWN\"/>
  </validation>

  <validation type=\"ttbarJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ttbar_JECDOWN.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__ttbar__jes__minus\" source=\"/MET/Stjet_cut2_ttbar_JECDOWN\"/>
  </validation>

  <validation type=\"WbbJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wbb_JECDOWN.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WbbJECDOWN\" source=\"/MET/Stjet_cut2_Wbb_JECDOWN\"/>
  </validation>

  <validation type=\"WccJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wcc_JECDOWN.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WccJECDOWN\" source=\"/MET/Stjet_cut2_Wcc_JECDOWN\"/>
  </validation>

  <validation type=\"WqqJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wqq_JECDOWN.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WqqJECDOWN\" source=\"/MET/Stjet_cut2_Wqq_JECDOWN\"/>
  </validation>

  <validation type=\"ZJetsJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ZJets_JECDOWN.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_stjet${sample[${i}]}__zjets__jes__minus\" source=\"/MET/Stjet_cut2_ZJets_JECDOWN\"/>
  </validation>

  <validation type=\"STtchJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_JECDOWN.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtchJECDOWN\" source=\"/MET/Stjet_cut2_STtch_JECDOWN\"/>
  </validation>

  <validation type=\"STtch_barJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_bar_JECDOWN.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtch_barJECDOWN\" source=\"/MET/Stjet_cut2_STtch_bar_JECDOWN\"/>
  </validation>

  <validation type=\"STtWchJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_JECDOWN.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWchJECDOWN\" source=\"/MET/Stjet_cut2_STtWch_JECDOWN\"/>
  </validation>

  <validation type=\"STtWch_barJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_bar_JECDOWN.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWch_barJECDOWN\" source=\"/MET/Stjet_cut2_STtWch_bar_JECDOWN\"/>
  </validation>

  <validation type=\"STschJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STsch_JECDOWN.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STschJECDOWN\" source=\"/MET/Stjet_cut2_STsch_JECDOWN\"/>
  </validation>


  <additionArray name=\"singletop__jes__minus\" title=\"singletop__jes__minus\">
    <additionArrayItem array=\"STtchJECDOWN\"/>
    <additionArrayItem array=\"STtWchJECDOWN\"/>
    <additionArrayItem array=\"STschJECDOWN\"/>
    <additionArrayItem array=\"STtch_barJECDOWN\"/>
    <additionArrayItem array=\"STtWch_barJECDOWN\"/>
  </additionArray>

  <additionArray name=\"wjets__jes__minus\" title=\"wjets__jes__minus\">
    <additionArrayItem array=\"WbbJECDOWN\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccJECDOWN\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqJECDOWN\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_stjet${sample[${i}]}__jes__minus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_stjet${sample[${i}]}__ttbar__jes__minus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__wjets__jes__minus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__zjets__jes__minus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__singletop__jes__minus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__jes__minus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- JECUP -->
  <validation type=\"tttt_UED6${sample[${i}]}JECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_tttt_UED6_JECUP.root\" weight=\"MCScaleFactors.txt:tttt_UED6${sample[${i}]}\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__jes__plus\" source=\"/MET/Stjet_cut2_ttttUED6_JECUP\"/>
  </validation>

  <validation type=\"ttbarJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ttbar_JECUP.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__ttbar__jes__plus\" source=\"/MET/Stjet_cut2_ttbar_JECUP\"/>
  </validation>

  <validation type=\"WbbJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wbb_JECUP.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WbbJECUP\" source=\"/MET/Stjet_cut2_Wbb_JECUP\"/>
  </validation>

  <validation type=\"WccJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wcc_JECUP.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WccJECUP\" source=\"/MET/Stjet_cut2_Wcc_JECUP\"/>
  </validation>

  <validation type=\"WqqJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wqq_JECUP.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WqqJECUP\" source=\"/MET/Stjet_cut2_Wqq_JECUP\"/>
  </validation>

  <validation type=\"ZJetsJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ZJets_JECUP.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_stjet${sample[${i}]}__zjets__jes__plus\" source=\"/MET/Stjet_cut2_ZJets_JECUP\"/>
  </validation>

  <validation type=\"STtchJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_JECUP.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtchJECUP\" source=\"/MET/Stjet_cut2_STtch_JECUP\"/>
  </validation>

  <validation type=\"STtch_barJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_bar_JECUP.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtch_barJECUP\" source=\"/MET/Stjet_cut2_STtch_bar_JECUP\"/>
  </validation>

  <validation type=\"STtWchJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_JECUP.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWchJECUP\" source=\"/MET/Stjet_cut2_STtWch_JECUP\"/>
  </validation>

  <validation type=\"STtWch_barJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_bar_JECUP.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWch_barJECUP\" source=\"/MET/Stjet_cut2_STtWch_bar_JECUP\"/>
  </validation>

  <validation type=\"STschJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STsch_JECUP.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STschJECUP\" source=\"/MET/Stjet_cut2_STsch_JECUP\"/>
  </validation>


  <additionArray name=\"singletop__jes__plus\" title=\"singletop__jes__plus\">
    <additionArrayItem array=\"STtchJECUP\"/>
    <additionArrayItem array=\"STtWchJECUP\"/>
    <additionArrayItem array=\"STschJECUP\"/>
    <additionArrayItem array=\"STtch_barJECUP\"/>
    <additionArrayItem array=\"STtWch_barJECUP\"/>
  </additionArray>


  <additionArray name=\"wjets__jes__plus\" title=\"wjets__jes__plus\">
    <additionArrayItem array=\"WbbJECUP\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccJECUP\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqJECUP\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_stjet${sample[${i}]}__jes__plus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_stjet${sample[${i}]}__ttbar__jes__plus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__wjets__jes__plus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__zjets__jes__plus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__singletop__jes__plus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__jes__plus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- PUDOWN -->
  <validation type=\"tttt_UED6${sample[${i}]}PUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_tttt_UED6_PUDOWN.root\" weight=\"MCScaleFactors.txt:tttt_UED6${sample[${i}]}\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__pu__minus\" source=\"/MET/Stjet_cut2_ttttUED6_PUDOWN\"/>
  </validation>

  <validation type=\"ttbarPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ttbar_PUDOWN.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__ttbar__pu__minus\" source=\"/MET/Stjet_cut2_ttbar_PUDOWN\"/>
  </validation>

  <validation type=\"WbbPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wbb_PUDOWN.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WbbPUDOWN\" source=\"/MET/Stjet_cut2_Wbb_PUDOWN\"/>
  </validation>

  <validation type=\"WccPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wcc_PUDOWN.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WccPUDOWN\" source=\"/MET/Stjet_cut2_Wcc_PUDOWN\"/>
  </validation>

  <validation type=\"WqqPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wqq_PUDOWN.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WqqPUDOWN\" source=\"/MET/Stjet_cut2_Wqq_PUDOWN\"/>
  </validation>

  <validation type=\"ZJetsPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ZJets_PUDOWN.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_stjet${sample[${i}]}__zjets__pu__minus\" source=\"/MET/Stjet_cut2_ZJets_PUDOWN\"/>
  </validation>

  <validation type=\"STtchPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_PUDOWN.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtchPUDOWN\" source=\"/MET/Stjet_cut2_STtch_PUDOWN\"/>
  </validation>

  <validation type=\"STtch_barPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_bar_PUDOWN.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtch_barPUDOWN\" source=\"/MET/Stjet_cut2_STtch_bar_PUDOWN\"/>
  </validation>

  <validation type=\"STtWchPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_PUDOWN.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWchPUDOWN\" source=\"/MET/Stjet_cut2_STtWch_PUDOWN\"/>
  </validation>

  <validation type=\"STtWch_barPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_bar_PUDOWN.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWch_barPUDOWN\" source=\"/MET/Stjet_cut2_STtWch_bar_PUDOWN\"/>
  </validation>

  <validation type=\"STschPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STsch_PUDOWN.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STschPUDOWN\" source=\"/MET/Stjet_cut2_STsch_PUDOWN\"/>
  </validation>


  <additionArray name=\"singletop__pu__minus\" title=\"singletop__pu__minus\">
    <additionArrayItem array=\"STtchPUDOWN\"/>
    <additionArrayItem array=\"STtWchPUDOWN\"/>
    <additionArrayItem array=\"STschPUDOWN\"/>
    <additionArrayItem array=\"STtch_barPUDOWN\"/>
    <additionArrayItem array=\"STtWch_barPUDOWN\"/>
  </additionArray>


  <additionArray name=\"wjets__pu__minus\" title=\"wjets__pu__minus\">
    <additionArrayItem array=\"WbbPUDOWN\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccPUDOWN\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqPUDOWN\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_stjet${sample[${i}]}__pu__minus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_stjet${sample[${i}]}__ttbar__pu__minus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__wjets__pu__minus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__zjets__pu__minus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__singletop__pu__minus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__pu__minus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- PUUP -->
  <validation type=\"tttt_UED6${sample[${i}]}PUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_tttt_UED6_PUUP.root\" weight=\"MCScaleFactors.txt:tttt_UED6${sample[${i}]}\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__pu__plus\" source=\"/MET/Stjet_cut2_ttttUED6_PUUP\"/>
  </validation>

  <validation type=\"ttbarPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ttbar_PUUP.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__ttbar__pu__plus\" source=\"/MET/Stjet_cut2_ttbar_PUUP\"/>
  </validation>

  <validation type=\"WbbPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wbb_PUUP.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WbbPUUP\" source=\"/MET/Stjet_cut2_Wbb_PUUP\"/>
  </validation>

  <validation type=\"WccPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wcc_PUUP.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WccPUUP\" source=\"/MET/Stjet_cut2_Wcc_PUUP\"/>
  </validation>

  <validation type=\"WqqPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wqq_PUUP.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WqqPUUP\" source=\"/MET/Stjet_cut2_Wqq_PUUP\"/>
  </validation>

  <validation type=\"ZJetsPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ZJets_PUUP.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_stjet${sample[${i}]}__zjets__pu__plus\" source=\"/MET/Stjet_cut2_ZJets_PUUP\"/>
  </validation>

  <validation type=\"STtchPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_PUUP.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtchPUUP\" source=\"/MET/Stjet_cut2_STtch_PUUP\"/>
  </validation>

  <validation type=\"STtch_barPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_bar_PUUP.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtch_barPUUP\" source=\"/MET/Stjet_cut2_STtch_bar_PUUP\"/>
  </validation>

  <validation type=\"STtWchPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_PUUP.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWchPUUP\" source=\"/MET/Stjet_cut2_STtWch_PUUP\"/>
  </validation>

  <validation type=\"STtWch_barPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_bar_PUUP.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWch_barPUUP\" source=\"/MET/Stjet_cut2_STtWch_bar_PUUP\"/>
  </validation>

  <validation type=\"STschPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STsch_PUUP.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STschPUUP\" source=\"/MET/Stjet_cut2_STsch_PUUP\"/>
  </validation>

  <additionArray name=\"singletop__pu__plus\" title=\"singletop__pu__plus\">
    <additionArrayItem array=\"STtchPUUP\"/>
    <additionArrayItem array=\"STtWchPUUP\"/>
    <additionArrayItem array=\"STschPUUP\"/>
    <additionArrayItem array=\"STtch_barPUUP\"/>
    <additionArrayItem array=\"STtWch_barPUUP\"/>
  </additionArray>

  <additionArray name=\"wjets__pu__plus\" title=\"wjets__pu__plus\">
    <additionArrayItem array=\"WbbPUUP\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccPUUP\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqPUUP\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_stjet${sample[${i}]}__pu__plus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_stjet${sample[${i}]}__ttbar__pu__plus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__wjets__pu__plus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__zjets__pu__plus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__singletop__pu__plus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__pu__plus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- ttbar Match and Scale -->
  <validation type=\"ttbarmatchup\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ttbarMatchup.root\" weight=\"MCScaleFactors.txt:ttbarmatchup\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__ttbar__match__plus\" source=\"/MET/Stjet_cut2_ttbarMatchup\"/>
  </validation>

  <validation type=\"ttbarmatchdown\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ttbarMatchdown.root\" weight=\"MCScaleFactors.txt:ttbarmatchdown\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__ttbar__match__minus\" source=\"/MET/Stjet_cut2_ttbarMatchdown\"/>
  </validation>

  <validation type=\"ttbarscaleup\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ttbarScaleup.root\" weight=\"MCScaleFactors.txt:ttbarscaleup\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__ttbar__scale__plus\" source=\"/MET/Stjet_cut2_ttbarScaleup\"/>
  </validation>

  <validation type=\"ttbarscaledown\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ttbarScaledown.root\" weight=\"MCScaleFactors.txt:ttbarscaledown\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__ttbar__scale__minus\" source=\"/MET/Stjet_cut2_ttbarScaledown\"/>
  </validation>

  <superimpose name=\"mu_stjet${sample[${i}]}__ttbarsyst\" title=\"BDT_response_ttbarsyst\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_stjet${sample[${i}]}__ttbar__match__plus\" color=\"1\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__ttbar__match__minus\" color=\"1\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__ttbar__scale__plus\" color=\"1\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__ttbar__scale__minus\" color=\"1\" legend=\"TTbar\"/>
  </superimpose>


  <!-- BTAGDOWN -->
  <validation type=\"tttt_UED6${sample[${i}]}BTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_tttt_UED6_BTAGDOWN.root\" weight=\"MCScaleFactors.txt:tttt_UED6${sample[${i}]}\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__btag__minus\" source=\"/MET/Stjet_cut2_ttttUED6_BTAGDOWN\"/>
  </validation>

  <validation type=\"ttbarBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ttbar_BTAGDOWN.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__ttbar__btag__minus\" source=\"/MET/Stjet_cut2_ttbar_BTAGDOWN\"/>
  </validation>

  <validation type=\"WbbBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wbb_BTAGDOWN.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WbbBTAGDOWN\" source=\"/MET/Stjet_cut2_Wbb_BTAGDOWN\"/>
  </validation>

  <validation type=\"WccBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wcc_BTAGDOWN.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WccBTAGDOWN\" source=\"/MET/Stjet_cut2_Wcc_BTAGDOWN\"/>
  </validation>

  <validation type=\"WqqBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wqq_BTAGDOWN.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WqqBTAGDOWN\" source=\"/MET/Stjet_cut2_Wqq_BTAGDOWN\"/>
  </validation>

  <validation type=\"ZJetsBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ZJets_BTAGDOWN.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_stjet${sample[${i}]}__zjets__btag__minus\" source=\"/MET/Stjet_cut2_ZJets_BTAGDOWN\"/>
  </validation>

  <validation type=\"STtchBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_BTAGDOWN.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtchBTAGDOWN\" source=\"/MET/Stjet_cut2_STtch_BTAGDOWN\"/>
  </validation>

  <validation type=\"STtch_barBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_bar_BTAGDOWN.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtch_barBTAGDOWN\" source=\"/MET/Stjet_cut2_STtch_bar_BTAGDOWN\"/>
  </validation>

  <validation type=\"STtWchBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_BTAGDOWN.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWchBTAGDOWN\" source=\"/MET/Stjet_cut2_STtWch_BTAGDOWN\"/>
  </validation>

  <validation type=\"STtWch_barBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_bar_BTAGDOWN.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWch_barBTAGDOWN\" source=\"/MET/Stjet_cut2_STtWch_bar_BTAGDOWN\"/>
  </validation>

  <validation type=\"STschBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STsch_BTAGDOWN.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STschBTAGDOWN\" source=\"/MET/Stjet_cut2_STsch_BTAGDOWN\"/>
  </validation>

  <additionArray name=\"singletop__btag__minus\" title=\"singletop__btag__minus\">
    <additionArrayItem array=\"STtchBTAGDOWN\"/>
    <additionArrayItem array=\"STtWchBTAGDOWN\"/>
    <additionArrayItem array=\"STschBTAGDOWN\"/>
    <additionArrayItem array=\"STtch_barBTAGDOWN\"/>
    <additionArrayItem array=\"STtWch_barBTAGDOWN\"/>
  </additionArray>

  <additionArray name=\"wjets__btag__minus\" title=\"wjets__btag__minus\">
    <additionArrayItem array=\"WbbBTAGDOWN\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccBTAGDOWN\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqBTAGDOWN\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_stjet${sample[${i}]}__btag__minus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_stjet${sample[${i}]}__ttbar__btag__minus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__wjets__btag__minus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__zjets__btag__minus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__singletop__btag__minus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__btag__minus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- BTAGUP -->
  <validation type=\"tttt_UED6${sample[${i}]}BTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_tttt_UED6_BTAGUP.root\" weight=\"MCScaleFactors.txt:tttt_UED6${sample[${i}]}\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__btag__plus\" source=\"/MET/Stjet_cut2_ttttUED6_BTAGUP\"/>
  </validation>

  <validation type=\"ttbarBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ttbar_BTAGUP.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_stjet${sample[${i}]}__ttbar__btag__plus\" source=\"/MET/Stjet_cut2_ttbar_BTAGUP\"/>
  </validation>

  <validation type=\"WbbBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wbb_BTAGUP.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WbbBTAGUP\" source=\"/MET/Stjet_cut2_Wbb_BTAGUP\"/>
  </validation>

  <validation type=\"WccBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wcc_BTAGUP.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WccBTAGUP\" source=\"/MET/Stjet_cut2_Wcc_BTAGUP\"/>
  </validation>

  <validation type=\"WqqBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_Wqq_BTAGUP.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_stjet${sample[${i}]}__WqqBTAGUP\" source=\"/MET/Stjet_cut2_Wqq_BTAGUP\"/>
  </validation>

  <validation type=\"ZJetsBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_ZJets_BTAGUP.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_stjet${sample[${i}]}__zjets__btag__plus\" source=\"/MET/Stjet_cut2_ZJets_BTAGUP\"/>
  </validation>

  <validation type=\"STtchBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_BTAGUP.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtchBTAGUP\" source=\"/MET/Stjet_cut2_STtch_BTAGUP\"/>
  </validation>

  <validation type=\"STtch_barBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtch_bar_BTAGUP.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtch_barBTAGUP\" source=\"/MET/Stjet_cut2_STtch_bar_BTAGUP\"/>
  </validation>

  <validation type=\"STtWchBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_BTAGUP.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWchBTAGUP\" source=\"/MET/Stjet_cut2_STtWch_BTAGUP\"/>
  </validation>

  <validation type=\"STtWch_barBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STtWch_bar_BTAGUP.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STtWch_barBTAGUP\" source=\"/MET/Stjet_cut2_STtWch_bar_BTAGUP\"/>
  </validation>

  <validation type=\"STschBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}//results_STsch_BTAGUP.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_stjet${sample[${i}]}__STschBTAGUP\" source=\"/MET/Stjet_cut2_STsch_BTAGUP\"/>
  </validation>

  <additionArray name=\"singletop__btag__plus\" title=\"singletop__btag__plus\">
    <additionArrayItem array=\"STtchBTAGUP\"/>
    <additionArrayItem array=\"STtWchBTAGUP\"/>
    <additionArrayItem array=\"STschBTAGUP\"/>
    <additionArrayItem array=\"STtch_barBTAGUP\"/>
    <additionArrayItem array=\"STtWch_barBTAGUP\"/>
  </additionArray>


  <additionArray name=\"wjets__btag__plus\" title=\"wjets__btag__plus\">
    <additionArrayItem array=\"WbbBTAGUP\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccBTAGUP\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqBTAGUP\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_stjet${sample[${i}]}__btag__plus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_stjet${sample[${i}]}__ttbar__btag__plus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__wjets__btag__plus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__zjets__btag__plus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__singletop__btag__plus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_stjet${sample[${i}]}__fourtop${sample[${i}]}__btag__plus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

</cuy>" >> $OUTFILE

../../cuy/scripts/cuy.py -x $OUTFILE  -O mu_stjet${sample[${i}]}_templates.root -b -q
rm $OUTFILE
done

hadd mu_stjet_templates.root mu_stjet*
mv mu_stjet* /uscms/home/algomez/nobackup/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/
