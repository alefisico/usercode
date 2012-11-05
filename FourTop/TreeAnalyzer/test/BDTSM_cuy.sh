#!/bin/bash 
sample=(SM)
num=(sm)
version='SM'
results='results110/Nbjets3'

sample_elements=${#sample[@]}
num_elements=${#num[@]}
for ((i=0;i<$sample_elements;i++));
do
	OUTFILE="BDT_cuy_tmp_${sample[${i}]}.xml"
	echo "<cuy>
  <validation type=\"data\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_data_BDT_ClassApplication.root\" weight=\"1\">
    <TH1 name=\"mu_disc${num[${i}]}__DATA\" source=\"results_data\"/>
  </validation>

  <validation type=\"tttt_${sample[${i}]}\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_tttt_${sample[${i}]}_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}\" source=\"results_tttt_${sample[${i}]}\"/>
  </validation>

  <validation type=\"ttbar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${num[${i}]}__ttbar\" source=\"results_ttbar\"/>
  </validation>

  <!--<validation type=\"WJets\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WJets_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WJets\">
    <TH1 name=\"mu_disc${num[${i}]}__wjets\" source=\"results_WJets\"/>
  </validation>-->

  <validation type=\"Wbb\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wbb_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${num[${i}]}__Wbb\" source=\"results_Wbb\"/>
  </validation>

  <validation type=\"Wcc\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wcc_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${num[${i}]}__Wcc\" source=\"results_Wcc\"/>
  </validation>

  <validation type=\"Wqq\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wqq_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${num[${i}]}__Wqq\" source=\"results_Wqq\"/>
  </validation>

  <validation type=\"ZJets\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${num[${i}]}__zjets\" source=\"results_ZJets\"/>
  </validation>

  <validation type=\"STtch\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtch\" source=\"results_STtch\"/>
  </validation>

  <validation type=\"STtch_bar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtch_bar\" source=\"results_STtch_bar\"/>
  </validation>

  <validation type=\"STtWch\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWch\" source=\"results_STtWch\"/>
  </validation>

  <validation type=\"STtWch_bar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWch_bar\" source=\"results_STtWch_bar\"/>
  </validation>

  <validation type=\"STsch\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${num[${i}]}__STsch\" source=\"results_STsch\"/>
  </validation>

  <validation type=\"STsch_bar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STsch_bar\" source=\"results_STsch_bar\"/>
  </validation>

  <validation type=\"WW\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${num[${i}]}__WW\" source=\"results_WW\"/>
  </validation>

  <validation type=\"WZ\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${num[${i}]}__WZ\" source=\"results_WZ\"/>
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

  <superimpose name=\"mu_disc${num[${i}]}\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\"  >
    <superimposeItem name=\"mu_disc${num[${i}]}__DATA\" color=\"top\" legend=\"Data\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__ttbar\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__wjets\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__zjets\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__dibosons\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__singletop\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}\" color=\"1\" legend=\"t#bar{t}t#bar{t}  ${num[${i}]} TeV\" NoStack=\"true\" />
  </superimpose>

  <!-- JERDOWN -->
  <validation type=\"tttt_${sample[${i}]}JERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_tttt_${sample[${i}]}_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__jer__minus\" source=\"results_tttt_${sample[${i}]}_JERDOWN\"/>
  </validation>

  <validation type=\"ttbarJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${num[${i}]}__ttbar__jer__minus\" source=\"results_ttbar_JERDOWN\"/>
  </validation>

  <validation type=\"WbbJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wbb_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${num[${i}]}__WbbJERDOWN\" source=\"results_Wbb_JERDOWN\"/>
  </validation>

  <validation type=\"WccJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wcc_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${num[${i}]}__WccJERDOWN\" source=\"results_Wcc_JERDOWN\"/>
  </validation>

  <validation type=\"WqqJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wqq_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${num[${i}]}__WqqJERDOWN\" source=\"results_Wqq_JERDOWN\"/>
  </validation>

  <validation type=\"ZJetsJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${num[${i}]}__zjets__jer__minus\" source=\"results_ZJets_JERDOWN\"/>
  </validation>

  <validation type=\"STtchJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtchJERDOWN\" source=\"results_STtch_JERDOWN\"/>
  </validation>

  <validation type=\"STtch_barJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtch_barJERDOWN\" source=\"results_STtch_bar_JERDOWN\"/>
  </validation>

  <validation type=\"STtWchJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWchJERDOWN\" source=\"results_STtWch_JERDOWN\"/>
  </validation>

  <validation type=\"STtWch_barJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWch_barJERDOWN\" source=\"results_STtWch_bar_JERDOWN\"/>
  </validation>

  <validation type=\"STschJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${num[${i}]}__STschJERDOWN\" source=\"results_STsch_JERDOWN\"/>
  </validation>

  <validation type=\"STsch_barJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STsch_barJERDOWN\" source=\"results_STsch_bar_JERDOWN\"/>
  </validation>

  <validation type=\"WWJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${num[${i}]}__WWJERDOWN\" source=\"results_WW_JERDOWN\"/>
  </validation>

  <validation type=\"WZJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${num[${i}]}__WZJERDOWN\" source=\"results_WZ_JERDOWN\"/>
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

  <superimpose name=\"mu_disc${num[${i}]}__jer__minus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${num[${i}]}__ttbar__jer__minus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__wjets__jer__minus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__zjets__jer__minus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__dibosons__jer__minus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__singletop__jer__minus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__jer__minus\" color=\"1\" legend=\"t#bar{t}t#bar{t}  ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- JERUP -->
  <validation type=\"tttt_${sample[${i}]}JERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_tttt_${sample[${i}]}_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__jer__plus\" source=\"results_tttt_${sample[${i}]}_JERUP\"/>
  </validation>

  <validation type=\"ttbarJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${num[${i}]}__ttbar__jer__plus\" source=\"results_ttbar_JERUP\"/>
  </validation>

  <validation type=\"WbbJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wbb_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${num[${i}]}__WbbJERUP\" source=\"results_Wbb_JERUP\"/>
  </validation>

  <validation type=\"WccJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wcc_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${num[${i}]}__WccJERUP\" source=\"results_Wcc_JERUP\"/>
  </validation>

  <validation type=\"WqqJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wqq_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${num[${i}]}__WqqJERUP\" source=\"results_Wqq_JERUP\"/>
  </validation>

  <validation type=\"ZJetsJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${num[${i}]}__zjets__jer__plus\" source=\"results_ZJets_JERUP\"/>
  </validation>

  <validation type=\"STtchJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtchJERUP\" source=\"results_STtch_JERUP\"/>
  </validation>

  <validation type=\"STtch_barJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtch_barJERUP\" source=\"results_STtch_bar_JERUP\"/>
  </validation>

  <validation type=\"STtWchJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWchJERUP\" source=\"results_STtWch_JERUP\"/>
  </validation>

  <validation type=\"STtWch_barJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWch_barJERUP\" source=\"results_STtWch_bar_JERUP\"/>
  </validation>

  <validation type=\"STschJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${num[${i}]}__STschJERUP\" source=\"results_STsch_JERUP\"/>
  </validation>

  <validation type=\"STsch_barJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STsch_barJERUP\" source=\"results_STsch_bar_JERUP\"/>
  </validation>

  <validation type=\"WWJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${num[${i}]}__WWJERUP\" source=\"results_WW_JERUP\"/>
  </validation>

  <validation type=\"WZJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${num[${i}]}__WZJERUP\" source=\"results_WZ_JERUP\"/>
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

  <superimpose name=\"mu_disc${num[${i}]}__jer__plus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${num[${i}]}__ttbar__jer__plus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__wjets__jer__plus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__zjets__jer__plus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__dibosons__jer__plus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__singletop__jer__plus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__jer__plus\" color=\"1\" legend=\"t#bar{t}t#bar{t}  ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- JECDOWN -->
  <validation type=\"tttt_${sample[${i}]}JECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_tttt_${sample[${i}]}_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__jes__minus\" source=\"results_tttt_${sample[${i}]}_JECDOWN\"/>
  </validation>

  <validation type=\"ttbarJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${num[${i}]}__ttbar__jes__minus\" source=\"results_ttbar_JECDOWN\"/>
  </validation>

  <validation type=\"WbbJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wbb_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${num[${i}]}__WbbJECDOWN\" source=\"results_Wbb_JECDOWN\"/>
  </validation>

  <validation type=\"WccJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wcc_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${num[${i}]}__WccJECDOWN\" source=\"results_Wcc_JECDOWN\"/>
  </validation>

  <validation type=\"WqqJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wqq_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${num[${i}]}__WqqJECDOWN\" source=\"results_Wqq_JECDOWN\"/>
  </validation>

  <validation type=\"ZJetsJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${num[${i}]}__zjets__jes__minus\" source=\"results_ZJets_JECDOWN\"/>
  </validation>

  <validation type=\"STtchJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtchJECDOWN\" source=\"results_STtch_JECDOWN\"/>
  </validation>

  <validation type=\"STtch_barJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtch_barJECDOWN\" source=\"results_STtch_bar_JECDOWN\"/>
  </validation>

  <validation type=\"STtWchJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWchJECDOWN\" source=\"results_STtWch_JECDOWN\"/>
  </validation>

  <validation type=\"STtWch_barJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWch_barJECDOWN\" source=\"results_STtWch_bar_JECDOWN\"/>
  </validation>

  <validation type=\"STschJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${num[${i}]}__STschJECDOWN\" source=\"results_STsch_JECDOWN\"/>
  </validation>

  <validation type=\"STsch_barJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STsch_barJECDOWN\" source=\"results_STsch_bar_JECDOWN\"/>
  </validation>

  <validation type=\"WWJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${num[${i}]}__WWJECDOWN\" source=\"results_WW_JECDOWN\"/>
  </validation>

  <validation type=\"WZJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${num[${i}]}__WZJECDOWN\" source=\"results_WZ_JECDOWN\"/>
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

  <superimpose name=\"mu_disc${num[${i}]}__jes__minus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${num[${i}]}__ttbar__jes__minus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__wjets__jes__minus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__zjets__jes__minus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__dibosons__jes__minus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__singletop__jes__minus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__jes__minus\" color=\"1\" legend=\"t#bar{t}t#bar{t}  ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- JECUP -->
  <validation type=\"tttt_${sample[${i}]}JECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_tttt_${sample[${i}]}_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__jes__plus\" source=\"results_tttt_${sample[${i}]}_JECUP\"/>
  </validation>

  <validation type=\"ttbarJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${num[${i}]}__ttbar__jes__plus\" source=\"results_ttbar_JECUP\"/>
  </validation>

  <validation type=\"WbbJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wbb_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${num[${i}]}__WbbJECUP\" source=\"results_Wbb_JECUP\"/>
  </validation>

  <validation type=\"WccJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wcc_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${num[${i}]}__WccJECUP\" source=\"results_Wcc_JECUP\"/>
  </validation>

  <validation type=\"WqqJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wqq_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${num[${i}]}__WqqJECUP\" source=\"results_Wqq_JECUP\"/>
  </validation>

  <validation type=\"ZJetsJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${num[${i}]}__zjets__jes__plus\" source=\"results_ZJets_JECUP\"/>
  </validation>

  <validation type=\"STtchJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtchJECUP\" source=\"results_STtch_JECUP\"/>
  </validation>

  <validation type=\"STtch_barJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtch_barJECUP\" source=\"results_STtch_bar_JECUP\"/>
  </validation>

  <validation type=\"STtWchJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWchJECUP\" source=\"results_STtWch_JECUP\"/>
  </validation>

  <validation type=\"STtWch_barJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWch_barJECUP\" source=\"results_STtWch_bar_JECUP\"/>
  </validation>

  <validation type=\"STschJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${num[${i}]}__STschJECUP\" source=\"results_STsch_JECUP\"/>
  </validation>

  <validation type=\"STsch_barJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STsch_barJECUP\" source=\"results_STsch_bar_JECUP\"/>
  </validation>

  <validation type=\"WWJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${num[${i}]}__WWJECUP\" source=\"results_WW_JECUP\"/>
  </validation>

  <validation type=\"WZJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${num[${i}]}__WZJECUP\" source=\"results_WZ_JECUP\"/>
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

  <superimpose name=\"mu_disc${num[${i}]}__jes__plus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${num[${i}]}__ttbar__jes__plus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__wjets__jes__plus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__zjets__jes__plus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__dibosons__jes__plus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__singletop__jes__plus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__jes__plus\" color=\"1\" legend=\"t#bar{t}t#bar{t}  ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- PUDOWN -->
  <validation type=\"tttt_${sample[${i}]}PUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_tttt_${sample[${i}]}_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__pu__minus\" source=\"results_tttt_${sample[${i}]}_PUDOWN\"/>
  </validation>

  <validation type=\"ttbarPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${num[${i}]}__ttbar__pu__minus\" source=\"results_ttbar_PUDOWN\"/>
  </validation>

  <validation type=\"WbbPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wbb_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${num[${i}]}__WbbPUDOWN\" source=\"results_Wbb_PUDOWN\"/>
  </validation>

  <validation type=\"WccPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wcc_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${num[${i}]}__WccPUDOWN\" source=\"results_Wcc_PUDOWN\"/>
  </validation>

  <validation type=\"WqqPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wqq_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${num[${i}]}__WqqPUDOWN\" source=\"results_Wqq_PUDOWN\"/>
  </validation>

  <validation type=\"ZJetsPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${num[${i}]}__zjets__pu__minus\" source=\"results_ZJets_PUDOWN\"/>
  </validation>

  <validation type=\"STtchPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtchPUDOWN\" source=\"results_STtch_PUDOWN\"/>
  </validation>

  <validation type=\"STtch_barPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtch_barPUDOWN\" source=\"results_STtch_bar_PUDOWN\"/>
  </validation>

  <validation type=\"STtWchPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWchPUDOWN\" source=\"results_STtWch_PUDOWN\"/>
  </validation>

  <validation type=\"STtWch_barPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWch_barPUDOWN\" source=\"results_STtWch_bar_PUDOWN\"/>
  </validation>

  <validation type=\"STschPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${num[${i}]}__STschPUDOWN\" source=\"results_STsch_PUDOWN\"/>
  </validation>

  <validation type=\"STsch_barPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STsch_barPUDOWN\" source=\"results_STsch_bar_PUDOWN\"/>
  </validation>

  <validation type=\"WWPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${num[${i}]}__WWPUDOWN\" source=\"results_WW_PUDOWN\"/>
  </validation>

  <validation type=\"WZPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${num[${i}]}__WZPUDOWN\" source=\"results_WZ_PUDOWN\"/>
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

  <superimpose name=\"mu_disc${num[${i}]}__pu__minus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${num[${i}]}__ttbar__pu__minus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__wjets__pu__minus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__zjets__pu__minus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__dibosons__pu__minus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__singletop__pu__minus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__pu__minus\" color=\"1\" legend=\"t#bar{t}t#bar{t}  ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- PUUP -->
  <validation type=\"tttt_${sample[${i}]}PUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_tttt_${sample[${i}]}_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__pu__plus\" source=\"results_tttt_${sample[${i}]}_PUUP\"/>
  </validation>

  <validation type=\"ttbarPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${num[${i}]}__ttbar__pu__plus\" source=\"results_ttbar_PUUP\"/>
  </validation>

  <validation type=\"WbbPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wbb_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${num[${i}]}__WbbPUUP\" source=\"results_Wbb_PUUP\"/>
  </validation>

  <validation type=\"WccPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wcc_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${num[${i}]}__WccPUUP\" source=\"results_Wcc_PUUP\"/>
  </validation>

  <validation type=\"WqqPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wqq_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${num[${i}]}__WqqPUUP\" source=\"results_Wqq_PUUP\"/>
  </validation>

  <validation type=\"ZJetsPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${num[${i}]}__zjets__pu__plus\" source=\"results_ZJets_PUUP\"/>
  </validation>

  <validation type=\"STtchPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtchPUUP\" source=\"results_STtch_PUUP\"/>
  </validation>

  <validation type=\"STtch_barPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtch_barPUUP\" source=\"results_STtch_bar_PUUP\"/>
  </validation>

  <validation type=\"STtWchPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWchPUUP\" source=\"results_STtWch_PUUP\"/>
  </validation>

  <validation type=\"STtWch_barPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWch_barPUUP\" source=\"results_STtWch_bar_PUUP\"/>
  </validation>

  <validation type=\"STschPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${num[${i}]}__STschPUUP\" source=\"results_STsch_PUUP\"/>
  </validation>

  <validation type=\"STsch_barPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STsch_barPUUP\" source=\"results_STsch_bar_PUUP\"/>
  </validation>

  <validation type=\"WWPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${num[${i}]}__WWPUUP\" source=\"results_WW_PUUP\"/>
  </validation>

  <validation type=\"WZPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${num[${i}]}__WZPUUP\" source=\"results_WZ_PUUP\"/>
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

  <superimpose name=\"mu_disc${num[${i}]}__pu__plus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${num[${i}]}__ttbar__pu__plus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__wjets__pu__plus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__zjets__pu__plus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__dibosons__pu__plus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__singletop__pu__plus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__pu__plus\" color=\"1\" legend=\"t#bar{t}t#bar{t}  ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- ttbar Match and Scale -->
  <validation type=\"ttbarmatchup\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbarMatchup_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbarmatchup\"> 
    <TH1 name=\"mu_disc${num[${i}]}__ttbar__match__plus\" source=\"results_ttbarMatchup\"/>
  </validation>

  <validation type=\"ttbarmatchdown\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbarMatchdown_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbarmatchdown\"> 
    <TH1 name=\"mu_disc${num[${i}]}__ttbar__match__minus\" source=\"results_ttbarMatchdown\"/>
  </validation>

  <validation type=\"ttbarscaleup\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbarScaleup_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbarscaleup\"> 
    <TH1 name=\"mu_disc${num[${i}]}__ttbar__scale__plus\" source=\"results_ttbarScaleup\"/>
  </validation>

  <validation type=\"ttbarscaledown\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbarScaledown_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbarscaledown\"> 
    <TH1 name=\"mu_disc${num[${i}]}__ttbar__scale__minus\" source=\"results_ttbarScaledown\"/>
  </validation>

  <superimpose name=\"mu_disc${num[${i}]}__ttbarsyst\" title=\"BDT_response_ttbarsyst\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${num[${i}]}__ttbar__match__plus\" color=\"1\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__ttbar__match__minus\" color=\"1\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__ttbar__scale__plus\" color=\"1\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__ttbar__scale__minus\" color=\"1\" legend=\"TTbar\"/>
  </superimpose>


  <!-- BTAGDOWN -->
  <validation type=\"tttt_${sample[${i}]}BTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_tttt_${sample[${i}]}_BTAGDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__btag__minus\" source=\"results_tttt_${sample[${i}]}_BTAGDOWN\"/>
  </validation>

  <validation type=\"ttbarBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_BTAGDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${num[${i}]}__ttbar__btag__minus\" source=\"results_ttbar_BTAGDOWN\"/>
  </validation>

  <validation type=\"WbbBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wbb_BTAGDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${num[${i}]}__WbbBTAGDOWN\" source=\"results_Wbb_BTAGDOWN\"/>
  </validation>

  <validation type=\"WccBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wcc_BTAGDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${num[${i}]}__WccBTAGDOWN\" source=\"results_Wcc_BTAGDOWN\"/>
  </validation>

  <validation type=\"WqqBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wqq_BTAGDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${num[${i}]}__WqqBTAGDOWN\" source=\"results_Wqq_BTAGDOWN\"/>
  </validation>

  <validation type=\"ZJetsBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_BTAGDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${num[${i}]}__zjets__btag__minus\" source=\"results_ZJets_BTAGDOWN\"/>
  </validation>

  <validation type=\"STtchBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_BTAGDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtchBTAGDOWN\" source=\"results_STtch_BTAGDOWN\"/>
  </validation>

  <validation type=\"STtch_barBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_BTAGDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtch_barBTAGDOWN\" source=\"results_STtch_bar_BTAGDOWN\"/>
  </validation>

  <validation type=\"STtWchBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_BTAGDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWchBTAGDOWN\" source=\"results_STtWch_BTAGDOWN\"/>
  </validation>

  <validation type=\"STtWch_barBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_BTAGDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWch_barBTAGDOWN\" source=\"results_STtWch_bar_BTAGDOWN\"/>
  </validation>

  <validation type=\"STschBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_BTAGDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${num[${i}]}__STschBTAGDOWN\" source=\"results_STsch_BTAGDOWN\"/>
  </validation>

  <validation type=\"STsch_barBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_BTAGDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STsch_barBTAGDOWN\" source=\"results_STsch_bar_BTAGDOWN\"/>
  </validation>

  <validation type=\"WWBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_BTAGDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${num[${i}]}__WWBTAGDOWN\" source=\"results_WW_BTAGDOWN\"/>
  </validation>

  <validation type=\"WZBTAGDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_BTAGDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${num[${i}]}__WZBTAGDOWN\" source=\"results_WZ_BTAGDOWN\"/>
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

  <superimpose name=\"mu_disc${num[${i}]}__btag__minus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${num[${i}]}__ttbar__btag__minus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__wjets__btag__minus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__zjets__btag__minus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__dibosons__btag__minus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__singletop__btag__minus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__btag__minus\" color=\"1\" legend=\"t#bar{t}t#bar{t}  ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- BTAGUP -->
  <validation type=\"tttt_${sample[${i}]}BTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_tttt_${sample[${i}]}_BTAGUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__btag__plus\" source=\"results_tttt_${sample[${i}]}_BTAGUP\"/>
  </validation>

  <validation type=\"ttbarBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_BTAGUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${num[${i}]}__ttbar__btag__plus\" source=\"results_ttbar_BTAGUP\"/>
  </validation>

  <validation type=\"WbbBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wbb_BTAGUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${num[${i}]}__WbbBTAGUP\" source=\"results_Wbb_BTAGUP\"/>
  </validation>

  <validation type=\"WccBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wcc_BTAGUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${num[${i}]}__WccBTAGUP\" source=\"results_Wcc_BTAGUP\"/>
  </validation>

  <validation type=\"WqqBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wqq_BTAGUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${num[${i}]}__WqqBTAGUP\" source=\"results_Wqq_BTAGUP\"/>
  </validation>

  <validation type=\"ZJetsBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_BTAGUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${num[${i}]}__zjets__btag__plus\" source=\"results_ZJets_BTAGUP\"/>
  </validation>

  <validation type=\"STtchBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_BTAGUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtchBTAGUP\" source=\"results_STtch_BTAGUP\"/>
  </validation>

  <validation type=\"STtch_barBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_BTAGUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtch_barBTAGUP\" source=\"results_STtch_bar_BTAGUP\"/>
  </validation>

  <validation type=\"STtWchBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_BTAGUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWchBTAGUP\" source=\"results_STtWch_BTAGUP\"/>
  </validation>

  <validation type=\"STtWch_barBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_BTAGUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STtWch_barBTAGUP\" source=\"results_STtWch_bar_BTAGUP\"/>
  </validation>

  <validation type=\"STschBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_BTAGUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${num[${i}]}__STschBTAGUP\" source=\"results_STsch_BTAGUP\"/>
  </validation>

  <validation type=\"STsch_barBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_BTAGUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${num[${i}]}__STsch_barBTAGUP\" source=\"results_STsch_bar_BTAGUP\"/>
  </validation>

  <validation type=\"WWBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_BTAGUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${num[${i}]}__WWBTAGUP\" source=\"results_WW_BTAGUP\"/>
  </validation>

  <validation type=\"WZBTAGUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_BTAGUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${num[${i}]}__WZBTAGUP\" source=\"results_WZ_BTAGUP\"/>
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

  <superimpose name=\"mu_disc${num[${i}]}__btag__plus\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${num[${i}]}__ttbar__btag__plus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__wjets__btag__plus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__zjets__btag__plus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__dibosons__btag__plus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__singletop__btag__plus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}__btag__plus\" color=\"1\" legend=\"t#bar{t}t#bar{t}  ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

</cuy>" >> $OUTFILE

../../cuy/scripts/cuy.py -x $OUTFILE  -O mu_disc${num[${i}]}_templates.root -b -q
rm $OUTFILE
done

mv mu_disc* /uscms/home/algomez/nobackup/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/
