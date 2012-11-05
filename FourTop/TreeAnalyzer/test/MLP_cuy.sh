#!/bin/bash 
#sample=(400 500 600 700 800 900 1000)
sample=(600)
#num=(0.4 0.5 0.6 0.7 0.8 0.9 1.0)
num=(0.6 )
version='v1'
#results='results109'
#results='results109/CSVM1/' #Nbjets3/' ##
results='results110/Nbjets3/' 

sample_elements=${#sample[@]}
num_elements=${#num[@]}
for ((i=0;i<$sample_elements;i++));
do
	OUTFILE="MLP_cuy_tmp_${sample[${i}]}.xml"
	echo "<cuy>
  <validation type=\"data\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_data_MLP_ClassApplication.root\" weight=\"1\">
    <TH1 name=\"mu_disc${sample[${i}]}__DATA\" source=\"results_data\"/>
  </validation>

  <validation type=\"tttt_Gh${sample[${i}]}\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_tttt_Gh${sample[${i}]}_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_Gh${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}\" source=\"results_tttt_Gh${sample[${i}]}\"/>
  </validation>

  <validation type=\"ttbar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ttbar_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar\" source=\"results_ttbar\"/>
  </validation>

  <!--<validation type=\"WJets\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WJets_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__wjets\" source=\"results_WJets\"/>
  </validation>-->

  <validation type=\"Wbb\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wbb_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${sample[${i}]}__Wbb\" source=\"results_Wbb\"/>
  </validation>

  <validation type=\"Wcc\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wcc_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${sample[${i}]}__Wcc\" source=\"results_Wcc\"/>
  </validation>

  <validation type=\"Wqq\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wqq_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${sample[${i}]}__Wqq\" source=\"results_Wqq\"/>
  </validation>

  <validation type=\"ZJets\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ZJets_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets\" source=\"results_ZJets\"/>
  </validation>

  <validation type=\"STtch\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch\" source=\"results_STtch\"/>
  </validation>

  <validation type=\"STtch_bar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_bar_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_bar\" source=\"results_STtch_bar\"/>
  </validation>

  <validation type=\"STtWch\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch\" source=\"results_STtWch\"/>
  </validation>

  <validation type=\"STtWch_bar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_bar_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_bar\" source=\"results_STtWch_bar\"/>
  </validation>

  <validation type=\"STsch\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch\" source=\"results_STsch\"/>
  </validation>

  <validation type=\"STsch_bar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_bar_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_bar\" source=\"results_STsch_bar\"/>
  </validation>

  <validation type=\"WW\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WW_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WW\" source=\"results_WW\"/>
  </validation>

  <validation type=\"WZ\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WZ_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${sample[${i}]}__WZ\" source=\"results_WZ\"/>
  </validation>

  <additionArray name=\"singletop\" title=\"singletop\">
    <additionArrayItem array=\"STtch\"/>
    <additionArrayItem array=\"STtWch\"/>
    <additionArrayItem array=\"STsch\"/>
    <additionArrayItem array=\"STtch_bar\"/>
    <additionArrayItem array=\"STtWch_bar\"/>
    <additionArrayItem array=\"STsch_bar\"/>
  </additionArray>

  <additionArray name=\"dibosons\" title=\"dibosons\">
    <additionArrayItem array=\"WW\"/>
    <additionArrayItem array=\"WZ\"/>
  </additionArray>

  <additionArray name=\"wjets\" title=\"wjets\">
    <additionArrayItem array=\"Wbb\" weight=\"1.21\"/>
    <additionArrayItem array=\"Wcc\" weight=\"1.66\"/>
    <additionArrayItem array=\"Wqq\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_disc${sample[${i}]}\" title=\"MLP_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\"  >
    <superimposeItem name=\"mu_disc${sample[${i}]}__DATA\" color=\"top\" legend=\"Data\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\" />
  </superimpose>

  <!-- JERDOWN -->
  <validation type=\"tttt_Gh${sample[${i}]}JERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_tttt_Gh${sample[${i}]}_JERDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_Gh${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jer__minus\" source=\"results_tttt_Gh${sample[${i}]}_JERDOWN\"/>
  </validation>

  <validation type=\"ttbarJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ttbar_JERDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__jer__minus\" source=\"results_ttbar_JERDOWN\"/>
  </validation>

  <validation type=\"WbbJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wbb_JERDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${sample[${i}]}__WbbJERDOWN\" source=\"results_Wbb_JERDOWN\"/>
  </validation>

  <validation type=\"WccJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wcc_JERDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${sample[${i}]}__WccJERDOWN\" source=\"results_Wcc_JERDOWN\"/>
  </validation>

  <validation type=\"WqqJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wqq_JERDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${sample[${i}]}__WqqJERDOWN\" source=\"results_Wqq_JERDOWN\"/>
  </validation>

  <validation type=\"ZJetsJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ZJets_JERDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets__jer__minus\" source=\"results_ZJets_JERDOWN\"/>
  </validation>

  <validation type=\"STtchJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_JERDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtchJERDOWN\" source=\"results_STtch_JERDOWN\"/>
  </validation>

  <validation type=\"STtch_barJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_bar_JERDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_barJERDOWN\" source=\"results_STtch_bar_JERDOWN\"/>
  </validation>

  <validation type=\"STtWchJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_JERDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWchJERDOWN\" source=\"results_STtWch_JERDOWN\"/>
  </validation>

  <validation type=\"STtWch_barJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_bar_JERDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_barJERDOWN\" source=\"results_STtWch_bar_JERDOWN\"/>
  </validation>

  <validation type=\"STschJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_JERDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STschJERDOWN\" source=\"results_STsch_JERDOWN\"/>
  </validation>

  <validation type=\"STsch_barJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_bar_JERDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_barJERDOWN\" source=\"results_STsch_bar_JERDOWN\"/>
  </validation>

  <validation type=\"WWJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WW_JERDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WWJERDOWN\" source=\"results_WW_JERDOWN\"/>
  </validation>

  <validation type=\"WZJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WZ_JERDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${sample[${i}]}__WZJERDOWN\" source=\"results_WZ_JERDOWN\"/>
  </validation>

  <additionArray name=\"singletop__jer__minus\" title=\"singletop__jer__minus\">
    <additionArrayItem array=\"STtchJERDOWN\"/>
    <additionArrayItem array=\"STtWchJERDOWN\"/>
    <additionArrayItem array=\"STschJERDOWN\"/>
    <additionArrayItem array=\"STtch_barJERDOWN\"/>
    <additionArrayItem array=\"STtWch_barJERDOWN\"/>
    <additionArrayItem array=\"STsch_barJERDOWN\"/>
  </additionArray>

  <additionArray name=\"dibosons__jer__minus\" title=\"dibosons__jer__minus\">
    <additionArrayItem array=\"WWJERDOWN\"/>    
    <additionArrayItem array=\"WZJERDOWN\"/>
  </additionArray>

  <additionArray name=\"wjets__jer__minus\" title=\"wjets__jer__minus\">
    <additionArrayItem array=\"WbbJERDOWN\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccJERDOWN\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqJERDOWN\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_disc${sample[${i}]}__jer__minus\" title=\"MLP_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__jer__minus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets__jer__minus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets__jer__minus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons__jer__minus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop__jer__minus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jer__minus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- JERUP -->
  <validation type=\"tttt_Gh${sample[${i}]}JERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_tttt_Gh${sample[${i}]}_JERUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_Gh${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jer__plus\" source=\"results_tttt_Gh${sample[${i}]}_JERUP\"/>
  </validation>

  <validation type=\"ttbarJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ttbar_JERUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__jer__plus\" source=\"results_ttbar_JERUP\"/>
  </validation>

  <validation type=\"WbbJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wbb_JERUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${sample[${i}]}__WbbJERUP\" source=\"results_Wbb_JERUP\"/>
  </validation>

  <validation type=\"WccJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wcc_JERUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${sample[${i}]}__WccJERUP\" source=\"results_Wcc_JERUP\"/>
  </validation>

  <validation type=\"WqqJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wqq_JERUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${sample[${i}]}__WqqJERUP\" source=\"results_Wqq_JERUP\"/>
  </validation>

  <validation type=\"ZJetsJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ZJets_JERUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets__jer__plus\" source=\"results_ZJets_JERUP\"/>
  </validation>

  <validation type=\"STtchJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_JERUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtchJERUP\" source=\"results_STtch_JERUP\"/>
  </validation>

  <validation type=\"STtch_barJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_bar_JERUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_barJERUP\" source=\"results_STtch_bar_JERUP\"/>
  </validation>

  <validation type=\"STtWchJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_JERUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWchJERUP\" source=\"results_STtWch_JERUP\"/>
  </validation>

  <validation type=\"STtWch_barJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_bar_JERUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_barJERUP\" source=\"results_STtWch_bar_JERUP\"/>
  </validation>

  <validation type=\"STschJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_JERUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STschJERUP\" source=\"results_STsch_JERUP\"/>
  </validation>

  <validation type=\"STsch_barJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_bar_JERUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_barJERUP\" source=\"results_STsch_bar_JERUP\"/>
  </validation>

  <validation type=\"WWJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WW_JERUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WWJERUP\" source=\"results_WW_JERUP\"/>
  </validation>

  <validation type=\"WZJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WZ_JERUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${sample[${i}]}__WZJERUP\" source=\"results_WZ_JERUP\"/>
  </validation>

  <additionArray name=\"singletop__jer__plus\" title=\"singletop__jer__plus\">
    <additionArrayItem array=\"STtchJERUP\"/>
    <additionArrayItem array=\"STtWchJERUP\"/>
    <additionArrayItem array=\"STschJERUP\"/>
    <additionArrayItem array=\"STtch_barJERUP\"/>
    <additionArrayItem array=\"STtWch_barJERUP\"/>
    <additionArrayItem array=\"STsch_barJERUP\"/>
  </additionArray>

  <additionArray name=\"dibosons__jer__plus\" title=\"dibosons__jer__plus\">
    <additionArrayItem array=\"WWJERUP\"/>    
    <additionArrayItem array=\"WZJERUP\"/>
  </additionArray>

  <additionArray name=\"wjets__jer__plus\" title=\"wjets__jer__plus\">
    <additionArrayItem array=\"WbbJERUP\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccJERUP\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqJERUP\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_disc${sample[${i}]}__jer__plus\" title=\"MLP_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__jer__plus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets__jer__plus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets__jer__plus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons__jer__plus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop__jer__plus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jer__plus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- JECDOWN -->
  <validation type=\"tttt_Gh${sample[${i}]}JECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_tttt_Gh${sample[${i}]}_JECDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_Gh${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jes__minus\" source=\"results_tttt_Gh${sample[${i}]}_JECDOWN\"/>
  </validation>

  <validation type=\"ttbarJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ttbar_JECDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__jes__minus\" source=\"results_ttbar_JECDOWN\"/>
  </validation>

  <validation type=\"WbbJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wbb_JECDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${sample[${i}]}__WbbJECDOWN\" source=\"results_Wbb_JECDOWN\"/>
  </validation>

  <validation type=\"WccJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wcc_JECDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${sample[${i}]}__WccJECDOWN\" source=\"results_Wcc_JECDOWN\"/>
  </validation>

  <validation type=\"WqqJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wqq_JECDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${sample[${i}]}__WqqJECDOWN\" source=\"results_Wqq_JECDOWN\"/>
  </validation>

  <validation type=\"ZJetsJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ZJets_JECDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets__jes__minus\" source=\"results_ZJets_JECDOWN\"/>
  </validation>

  <validation type=\"STtchJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_JECDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtchJECDOWN\" source=\"results_STtch_JECDOWN\"/>
  </validation>

  <validation type=\"STtch_barJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_bar_JECDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_barJECDOWN\" source=\"results_STtch_bar_JECDOWN\"/>
  </validation>

  <validation type=\"STtWchJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_JECDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWchJECDOWN\" source=\"results_STtWch_JECDOWN\"/>
  </validation>

  <validation type=\"STtWch_barJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_bar_JECDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_barJECDOWN\" source=\"results_STtWch_bar_JECDOWN\"/>
  </validation>

  <validation type=\"STschJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_JECDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STschJECDOWN\" source=\"results_STsch_JECDOWN\"/>
  </validation>

  <validation type=\"STsch_barJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_bar_JECDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_barJECDOWN\" source=\"results_STsch_bar_JECDOWN\"/>
  </validation>

  <validation type=\"WWJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WW_JECDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WWJECDOWN\" source=\"results_WW_JECDOWN\"/>
  </validation>

  <validation type=\"WZJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WZ_JECDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${sample[${i}]}__WZJECDOWN\" source=\"results_WZ_JECDOWN\"/>
  </validation>

  <additionArray name=\"singletop__jes__minus\" title=\"singletop__jes__minus\">
    <additionArrayItem array=\"STtchJECDOWN\"/>
    <additionArrayItem array=\"STtWchJECDOWN\"/>
    <additionArrayItem array=\"STschJECDOWN\"/>
    <additionArrayItem array=\"STtch_barJECDOWN\"/>
    <additionArrayItem array=\"STtWch_barJECDOWN\"/>
    <additionArrayItem array=\"STsch_barJECDOWN\"/>
  </additionArray>

  <additionArray name=\"dibosons__jes__minus\" title=\"dibosons__jes__minus\">
    <additionArrayItem array=\"WWJECDOWN\"/>    
    <additionArrayItem array=\"WZJECDOWN\"/>
  </additionArray>

  <additionArray name=\"wjets__jes__minus\" title=\"wjets__jes__minus\">
    <additionArrayItem array=\"WbbJECDOWN\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccJECDOWN\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqJECDOWN\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_disc${sample[${i}]}__jes__minus\" title=\"MLP_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__jes__minus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets__jes__minus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets__jes__minus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons__jes__minus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop__jes__minus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jes__minus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- JECUP -->
  <validation type=\"tttt_Gh${sample[${i}]}JECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_tttt_Gh${sample[${i}]}_JECUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_Gh${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jes__plus\" source=\"results_tttt_Gh${sample[${i}]}_JECUP\"/>
  </validation>

  <validation type=\"ttbarJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ttbar_JECUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__jes__plus\" source=\"results_ttbar_JECUP\"/>
  </validation>

  <validation type=\"WbbJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wbb_JECUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${sample[${i}]}__WbbJECUP\" source=\"results_Wbb_JECUP\"/>
  </validation>

  <validation type=\"WccJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wcc_JECUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${sample[${i}]}__WccJECUP\" source=\"results_Wcc_JECUP\"/>
  </validation>

  <validation type=\"WqqJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wqq_JECUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${sample[${i}]}__WqqJECUP\" source=\"results_Wqq_JECUP\"/>
  </validation>

  <validation type=\"ZJetsJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ZJets_JECUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets__jes__plus\" source=\"results_ZJets_JECUP\"/>
  </validation>

  <validation type=\"STtchJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_JECUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtchJECUP\" source=\"results_STtch_JECUP\"/>
  </validation>

  <validation type=\"STtch_barJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_bar_JECUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_barJECUP\" source=\"results_STtch_bar_JECUP\"/>
  </validation>

  <validation type=\"STtWchJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_JECUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWchJECUP\" source=\"results_STtWch_JECUP\"/>
  </validation>

  <validation type=\"STtWch_barJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_bar_JECUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_barJECUP\" source=\"results_STtWch_bar_JECUP\"/>
  </validation>

  <validation type=\"STschJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_JECUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STschJECUP\" source=\"results_STsch_JECUP\"/>
  </validation>

  <validation type=\"STsch_barJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_bar_JECUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_barJECUP\" source=\"results_STsch_bar_JECUP\"/>
  </validation>

  <validation type=\"WWJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WW_JECUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WWJECUP\" source=\"results_WW_JECUP\"/>
  </validation>

  <validation type=\"WZJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WZ_JECUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${sample[${i}]}__WZJECUP\" source=\"results_WZ_JECUP\"/>
  </validation>

  <additionArray name=\"singletop__jes__plus\" title=\"singletop__jes__plus\">
    <additionArrayItem array=\"STtchJECUP\"/>
    <additionArrayItem array=\"STtWchJECUP\"/>
    <additionArrayItem array=\"STschJECUP\"/>
    <additionArrayItem array=\"STtch_barJECUP\"/>
    <additionArrayItem array=\"STtWch_barJECUP\"/>
    <additionArrayItem array=\"STsch_barJECUP\"/>
  </additionArray>

  <additionArray name=\"dibosons__jes__plus\" title=\"dibosons__jes__plus\">
    <additionArrayItem array=\"WWJECUP\"/>    
    <additionArrayItem array=\"WZJECUP\"/>
  </additionArray>

  <additionArray name=\"wjets__jes__plus\" title=\"wjets__jes__plus\">
    <additionArrayItem array=\"WbbJECUP\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccJECUP\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqJECUP\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_disc${sample[${i}]}__jes__plus\" title=\"MLP_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__jes__plus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets__jes__plus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets__jes__plus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons__jes__plus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop__jes__plus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jes__plus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- PUDOWN -->
  <validation type=\"tttt_Gh${sample[${i}]}PUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_tttt_Gh${sample[${i}]}_PUDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_Gh${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__pu__minus\" source=\"results_tttt_Gh${sample[${i}]}_PUDOWN\"/>
  </validation>

  <validation type=\"ttbarPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ttbar_PUDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__pu__minus\" source=\"results_ttbar_PUDOWN\"/>
  </validation>

  <validation type=\"WbbPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wbb_PUDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${sample[${i}]}__WbbPUDOWN\" source=\"results_Wbb_PUDOWN\"/>
  </validation>

  <validation type=\"WccPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wcc_PUDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${sample[${i}]}__WccPUDOWN\" source=\"results_Wcc_PUDOWN\"/>
  </validation>

  <validation type=\"WqqPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wqq_PUDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${sample[${i}]}__WqqPUDOWN\" source=\"results_Wqq_PUDOWN\"/>
  </validation>

  <validation type=\"ZJetsPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ZJets_PUDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets__pu__minus\" source=\"results_ZJets_PUDOWN\"/>
  </validation>

  <validation type=\"STtchPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_PUDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtchPUDOWN\" source=\"results_STtch_PUDOWN\"/>
  </validation>

  <validation type=\"STtch_barPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_bar_PUDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_barPUDOWN\" source=\"results_STtch_bar_PUDOWN\"/>
  </validation>

  <validation type=\"STtWchPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_PUDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWchPUDOWN\" source=\"results_STtWch_PUDOWN\"/>
  </validation>

  <validation type=\"STtWch_barPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_bar_PUDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_barPUDOWN\" source=\"results_STtWch_bar_PUDOWN\"/>
  </validation>

  <validation type=\"STschPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_PUDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STschPUDOWN\" source=\"results_STsch_PUDOWN\"/>
  </validation>

  <validation type=\"STsch_barPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_bar_PUDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_barPUDOWN\" source=\"results_STsch_bar_PUDOWN\"/>
  </validation>

  <validation type=\"WWPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WW_PUDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WWPUDOWN\" source=\"results_WW_PUDOWN\"/>
  </validation>

  <validation type=\"WZPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WZ_PUDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${sample[${i}]}__WZPUDOWN\" source=\"results_WZ_PUDOWN\"/>
  </validation>

  <additionArray name=\"singletop__pu__minus\" title=\"singletop__pu__minus\">
    <additionArrayItem array=\"STtchPUDOWN\"/>
    <additionArrayItem array=\"STtWchPUDOWN\"/>
    <additionArrayItem array=\"STschPUDOWN\"/>
    <additionArrayItem array=\"STtch_barPUDOWN\"/>
    <additionArrayItem array=\"STtWch_barPUDOWN\"/>
    <additionArrayItem array=\"STsch_barPUDOWN\"/>
  </additionArray>

  <additionArray name=\"dibosons__pu__minus\" title=\"dibosons__pu__minus\">
    <additionArrayItem array=\"WWPUDOWN\"/>    
    <additionArrayItem array=\"WZPUDOWN\"/>
  </additionArray>

  <additionArray name=\"wjets__pu__minus\" title=\"wjets__pu__minus\">
    <additionArrayItem array=\"WbbPUDOWN\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccPUDOWN\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqPUDOWN\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_disc${sample[${i}]}__pu__minus\" title=\"MLP_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__pu__minus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets__pu__minus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets__pu__minus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons__pu__minus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop__pu__minus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__pu__minus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- PUUP -->
  <validation type=\"tttt_Gh${sample[${i}]}PUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_tttt_Gh${sample[${i}]}_PUUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_Gh${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__pu__plus\" source=\"results_tttt_Gh${sample[${i}]}_PUUP\"/>
  </validation>

  <validation type=\"ttbarPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ttbar_PUUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__pu__plus\" source=\"results_ttbar_PUUP\"/>
  </validation>

  <validation type=\"WbbPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wbb_PUUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${sample[${i}]}__WbbPUUP\" source=\"results_Wbb_PUUP\"/>
  </validation>

  <validation type=\"WccPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wcc_PUUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${sample[${i}]}__WccPUUP\" source=\"results_Wcc_PUUP\"/>
  </validation>

  <validation type=\"WqqPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wqq_PUUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${sample[${i}]}__WqqPUUP\" source=\"results_Wqq_PUUP\"/>
  </validation>

  <validation type=\"ZJetsPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ZJets_PUUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets__pu__plus\" source=\"results_ZJets_PUUP\"/>
  </validation>

  <validation type=\"STtchPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_PUUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtchPUUP\" source=\"results_STtch_PUUP\"/>
  </validation>

  <validation type=\"STtch_barPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_bar_PUUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_barPUUP\" source=\"results_STtch_bar_PUUP\"/>
  </validation>

  <validation type=\"STtWchPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_PUUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWchPUUP\" source=\"results_STtWch_PUUP\"/>
  </validation>

  <validation type=\"STtWch_barPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_bar_PUUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_barPUUP\" source=\"results_STtWch_bar_PUUP\"/>
  </validation>

  <validation type=\"STschPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_PUUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STschPUUP\" source=\"results_STsch_PUUP\"/>
  </validation>

  <validation type=\"STsch_barPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_bar_PUUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_barPUUP\" source=\"results_STsch_bar_PUUP\"/>
  </validation>

  <validation type=\"WWPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WW_PUUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WWPUUP\" source=\"results_WW_PUUP\"/>
  </validation>

  <validation type=\"WZPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WZ_PUUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${sample[${i}]}__WZPUUP\" source=\"results_WZ_PUUP\"/>
  </validation>

  <additionArray name=\"singletop__pu__plus\" title=\"singletop__pu__plus\">
    <additionArrayItem array=\"STtchPUUP\"/>
    <additionArrayItem array=\"STtWchPUUP\"/>
    <additionArrayItem array=\"STschPUUP\"/>
    <additionArrayItem array=\"STtch_barPUUP\"/>
    <additionArrayItem array=\"STtWch_barPUUP\"/>
    <additionArrayItem array=\"STsch_barPUUP\"/>
  </additionArray>

  <additionArray name=\"dibosons__pu__plus\" title=\"dibosons__pu__plus\">
    <additionArrayItem array=\"WWPUUP\"/>    
    <additionArrayItem array=\"WZPUUP\"/>
  </additionArray>

  <additionArray name=\"wjets__pu__plus\" title=\"wjets__pu__plus\">
    <additionArrayItem array=\"WbbPUUP\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccPUUP\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqPUUP\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_disc${sample[${i}]}__pu__plus\" title=\"MLP_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__pu__plus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets__pu__plus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets__pu__plus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons__pu__plus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop__pu__plus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__pu__plus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- ttbar Match and Scale -->
  <validation type=\"ttbarmatchup\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ttbarMatchup_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbarmatchup\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__match__plus\" source=\"results_ttbarMatchup\"/>
  </validation>

  <validation type=\"ttbarmatchdown\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ttbarMatchdown_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbarmatchdown\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__match__minus\" source=\"results_ttbarMatchdown\"/>
  </validation>

  <validation type=\"ttbarscaleup\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ttbarScaleup_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbarscaleup\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__scale__plus\" source=\"results_ttbarScaleup\"/>
  </validation>

  <validation type=\"ttbarscaledown\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ttbarScaledown_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbarscaledown\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__scale__minus\" source=\"results_ttbarScaledown\"/>
  </validation>

  <superimpose name=\"mu_disc${sample[${i}]}__ttbarsyst\" title=\"MLP_response_ttbarsyst\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__match__plus\" color=\"1\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__match__minus\" color=\"1\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__scale__plus\" color=\"1\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__scale__minus\" color=\"1\" legend=\"TTbar\"/>
  </superimpose>


  <!-- BTAGDOWN -->
  <validation type=\"tttt_Gh${sample[${i}]}BTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_tttt_Gh${sample[${i}]}_BTAGDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_Gh${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__btag__minus\" source=\"results_tttt_Gh${sample[${i}]}_BTAGDOWN\"/>
  </validation>

  <validation type=\"ttbarBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ttbar_BTAGDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__btag__minus\" source=\"results_ttbar_BTAGDOWN\"/>
  </validation>

  <validation type=\"WbbBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wbb_BTAGDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${sample[${i}]}__WbbBTAGDOWN\" source=\"results_Wbb_BTAGDOWN\"/>
  </validation>

  <validation type=\"WccBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wcc_BTAGDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${sample[${i}]}__WccBTAGDOWN\" source=\"results_Wcc_BTAGDOWN\"/>
  </validation>

  <validation type=\"WqqBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wqq_BTAGDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${sample[${i}]}__WqqBTAGDOWN\" source=\"results_Wqq_BTAGDOWN\"/>
  </validation>

  <validation type=\"ZJetsBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ZJets_BTAGDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets__btag__minus\" source=\"results_ZJets_BTAGDOWN\"/>
  </validation>

  <validation type=\"STtchBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_BTAGDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtchBTAGDOWN\" source=\"results_STtch_BTAGDOWN\"/>
  </validation>

  <validation type=\"STtch_barBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_bar_BTAGDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_barBTAGDOWN\" source=\"results_STtch_bar_BTAGDOWN\"/>
  </validation>

  <validation type=\"STtWchBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_BTAGDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWchBTAGDOWN\" source=\"results_STtWch_BTAGDOWN\"/>
  </validation>

  <validation type=\"STtWch_barBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_bar_BTAGDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_barBTAGDOWN\" source=\"results_STtWch_bar_BTAGDOWN\"/>
  </validation>

  <validation type=\"STschBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_BTAGDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STschBTAGDOWN\" source=\"results_STsch_BTAGDOWN\"/>
  </validation>

  <validation type=\"STsch_barBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_bar_BTAGDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_barBTAGDOWN\" source=\"results_STsch_bar_BTAGDOWN\"/>
  </validation>

  <validation type=\"WWBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WW_BTAGDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WWBTAGDOWN\" source=\"results_WW_BTAGDOWN\"/>
  </validation>

  <validation type=\"WZBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WZ_BTAGDOWN_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${sample[${i}]}__WZBTAGDOWN\" source=\"results_WZ_BTAGDOWN\"/>
  </validation>

  <additionArray name=\"singletop__btag__minus\" title=\"singletop__btag__minus\">
    <additionArrayItem array=\"STtchBTAGDOWN\"/>
    <additionArrayItem array=\"STtWchBTAGDOWN\"/>
    <additionArrayItem array=\"STschBTAGDOWN\"/>
    <additionArrayItem array=\"STtch_barBTAGDOWN\"/>
    <additionArrayItem array=\"STtWch_barBTAGDOWN\"/>
    <additionArrayItem array=\"STsch_barBTAGDOWN\"/>
  </additionArray>

  <additionArray name=\"dibosons__btag__minus\" title=\"dibosons__btag__minus\">
    <additionArrayItem array=\"WWBTAGDOWN\"/>    
    <additionArrayItem array=\"WZBTAGDOWN\"/>
  </additionArray>

  <additionArray name=\"wjets__btag__minus\" title=\"wjets__btag__minus\">
    <additionArrayItem array=\"WbbBTAGDOWN\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccBTAGDOWN\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqBTAGDOWN\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_disc${sample[${i}]}__btag__minus\" title=\"MLP_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__btag__minus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets__btag__minus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets__btag__minus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons__btag__minus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop__btag__minus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__btag__minus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- BTAGUP -->
  <validation type=\"tttt_Gh${sample[${i}]}BTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_tttt_Gh${sample[${i}]}_BTAGUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_Gh${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__btag__plus\" source=\"results_tttt_Gh${sample[${i}]}_BTAGUP\"/>
  </validation>

  <validation type=\"ttbarBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ttbar_BTAGUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__btag__plus\" source=\"results_ttbar_BTAGUP\"/>
  </validation>

  <validation type=\"WbbBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wbb_BTAGUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${sample[${i}]}__WbbBTAGUP\" source=\"results_Wbb_BTAGUP\"/>
  </validation>

  <validation type=\"WccBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wcc_BTAGUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${sample[${i}]}__WccBTAGUP\" source=\"results_Wcc_BTAGUP\"/>
  </validation>

  <validation type=\"WqqBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_Wqq_BTAGUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${sample[${i}]}__WqqBTAGUP\" source=\"results_Wqq_BTAGUP\"/>
  </validation>

  <validation type=\"ZJetsBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_ZJets_BTAGUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets__btag__plus\" source=\"results_ZJets_BTAGUP\"/>
  </validation>

  <validation type=\"STtchBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_BTAGUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtchBTAGUP\" source=\"results_STtch_BTAGUP\"/>
  </validation>

  <validation type=\"STtch_barBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtch_bar_BTAGUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_barBTAGUP\" source=\"results_STtch_bar_BTAGUP\"/>
  </validation>

  <validation type=\"STtWchBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_BTAGUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWchBTAGUP\" source=\"results_STtWch_BTAGUP\"/>
  </validation>

  <validation type=\"STtWch_barBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_bar_BTAGUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_barBTAGUP\" source=\"results_STtWch_bar_BTAGUP\"/>
  </validation>

  <validation type=\"STschBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_BTAGUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STschBTAGUP\" source=\"results_STsch_BTAGUP\"/>
  </validation>

  <validation type=\"STsch_barBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STsch_bar_BTAGUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_barBTAGUP\" source=\"results_STsch_bar_BTAGUP\"/>
  </validation>

  <validation type=\"WWBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WW_BTAGUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WWBTAGUP\" source=\"results_WW_BTAGUP\"/>
  </validation>

  <validation type=\"WZBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WZ_BTAGUP_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${sample[${i}]}__WZBTAGUP\" source=\"results_WZ_BTAGUP\"/>
  </validation>

  <additionArray name=\"singletop__btag__plus\" title=\"singletop__btag__plus\">
    <additionArrayItem array=\"STtchBTAGUP\"/>
    <additionArrayItem array=\"STtWchBTAGUP\"/>
    <additionArrayItem array=\"STschBTAGUP\"/>
    <additionArrayItem array=\"STtch_barBTAGUP\"/>
    <additionArrayItem array=\"STtWch_barBTAGUP\"/>
    <additionArrayItem array=\"STsch_barBTAGUP\"/>
  </additionArray>

  <additionArray name=\"dibosons__btag__plus\" title=\"dibosons__btag__plus\">
    <additionArrayItem array=\"WWBTAGUP\"/>    
    <additionArrayItem array=\"WZBTAGUP\"/>
  </additionArray>

  <additionArray name=\"wjets__btag__plus\" title=\"wjets__btag__plus\">
    <additionArrayItem array=\"WbbBTAGUP\" weight=\"1.21\"/>
    <additionArrayItem array=\"WccBTAGUP\" weight=\"1.66\"/>
    <additionArrayItem array=\"WqqBTAGUP\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_disc${sample[${i}]}__btag__plus\" title=\"MLP_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__btag__plus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets__btag__plus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets__btag__plus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons__btag__plus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop__btag__plus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__btag__plus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

</cuy>" >> $OUTFILE

../../cuy/scripts/cuy.py -x $OUTFILE  -O mu_disc${sample[${i}]}_templates.root -b -q
rm $OUTFILE
done

#hadd mu_disc_templates.root mu_disc*
mv mu_disc* /uscms/home/algomez/nobackup/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/
