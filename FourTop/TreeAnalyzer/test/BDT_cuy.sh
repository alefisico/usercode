#!/bin/bash 
sample=(500 900 1100)
num=(0.5 0.9 1.1)
version='v1'
results='results18'
#OUTDIR='~/public_html/plots/BDTv22/'
OUTDIR='~/public_html/plots/tmp/'

sample_elements=${#sample[@]}
num_elements=${#num[@]}
for ((i=0;i<$sample_elements;i++));
do
	OUTFILE="BDT_cuy_tmp_${sample[${i}]}.xml"
	echo "<cuy>
  <validation type=\"data\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_data_BDT_ClassApplication.root\" weight=\"1\">
    <TH1 name=\"mu_disc${sample[${i}]}__DATA\" source=\"results_data\"/>
  </validation>

  <validation type=\"4Top${sample[${i}]}\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_4Top${sample[${i}]}_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:4Top${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}\" source=\"results_4Top${sample[${i}]}\"/>
  </validation>

  <validation type=\"ttbar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar\" source=\"results_ttbar\"/>
  </validation>

  <validation type=\"WJets\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WJets_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__wjets\" source=\"results_WJets\"/>
  </validation>

  <validation type=\"ZJets\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets\" source=\"results_ZJets\"/>
  </validation>

  <validation type=\"STtch\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch\" source=\"results_STtch\"/>
  </validation>

  <validation type=\"STtch_bar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_bar\" source=\"results_STtch_bar\"/>
  </validation>

  <validation type=\"STtWch\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch\" source=\"results_STtWch\"/>
  </validation>

  <validation type=\"STtWch_bar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_bar\" source=\"results_STtWch_bar\"/>
  </validation>

  <validation type=\"STsch\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch\" source=\"results_STsch\"/>
  </validation>

  <validation type=\"STsch_bar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_bar\" source=\"results_STsch_bar\"/>
  </validation>

  <validation type=\"WW\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WW\" source=\"results_WW\"/>
  </validation>

  <validation type=\"WZ\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${sample[${i}]}__WZ\" source=\"results_WZ\"/>
  </validation>

  <validation type=\"4Top${sample[${i}]}\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_4Top${sample[${i}]}_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:4Top${sample[${i}]}\">
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}\" source=\"results_4Top${sample[${i}]}\"/>
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


  <superimpose name=\"mu_disc${sample[${i}]}\" title=\"BDT_response\" YTitle=\"Events\" Fill=\"true\" Weight=\"true\" Lumi=\"4678.0\" Stack=\"true\" SetLogy=\"true\" Maximum=\"10e4\"  Minimum=\"10e-3\" PlotDiff=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__DATA\" color=\"top\" legend=\"Data\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\" />
  </superimpose>

  <!-- JERDOWN -->
  <validation type=\"4Top${sample[${i}]}JERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_4Top${sample[${i}]}_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:4Top${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jer__minus\" source=\"results_4Top${sample[${i}]}_JERDOWN\"/>
  </validation>

  <validation type=\"ttbarJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__jer__minus\" source=\"results_ttbar_JERDOWN\"/>
  </validation>

  <validation type=\"WJetsJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WJets_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__wjets__jer__minus\" source=\"results_WJets_JERDOWN\"/>
  </validation>

  <validation type=\"ZJetsJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets__jer__minus\" source=\"results_ZJets_JERDOWN\"/>
  </validation>

  <validation type=\"STtchJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtchJERDOWN\" source=\"results_STtch_JERDOWN\"/>
  </validation>

  <validation type=\"STtch_barJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_barJERDOWN\" source=\"results_STtch_bar_JERDOWN\"/>
  </validation>

  <validation type=\"STtWchJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWchJERDOWN\" source=\"results_STtWch_JERDOWN\"/>
  </validation>

  <validation type=\"STtWch_barJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_barJERDOWN\" source=\"results_STtWch_bar_JERDOWN\"/>
  </validation>

  <validation type=\"STschJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STschJERDOWN\" source=\"results_STsch_JERDOWN\"/>
  </validation>

  <validation type=\"STsch_barJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_barJERDOWN\" source=\"results_STsch_bar_JERDOWN\"/>
  </validation>

  <validation type=\"WWJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WWJERDOWN\" source=\"results_WW_JERDOWN\"/>
  </validation>

  <validation type=\"WZJERDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_JERDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
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


  <superimpose name=\"mu_disc${sample[${i}]}__jer__minus\" title=\"BDT_response\" YTitle=\"Events\" Fill=\"true\" Weight=\"true\" Lumi=\"4678.0\" SetLogy=\"true\" Maximum=\"10e4\"  Minimum=\"10e-3\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__jer__minus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets__jer__minus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets__jer__minus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons__jer__minus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop__jer__minus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jer__minus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- JERUP -->
  <validation type=\"4Top${sample[${i}]}JERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_4Top${sample[${i}]}_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:4Top${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jer__plus\" source=\"results_4Top${sample[${i}]}_JERUP\"/>
  </validation>

  <validation type=\"ttbarJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__jer__plus\" source=\"results_ttbar_JERUP\"/>
  </validation>

  <validation type=\"WJetsJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WJets_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__wjets__jer__plus\" source=\"results_WJets_JERUP\"/>
  </validation>

  <validation type=\"ZJetsJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets__jer__plus\" source=\"results_ZJets_JERUP\"/>
  </validation>

  <validation type=\"STtchJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtchJERUP\" source=\"results_STtch_JERUP\"/>
  </validation>

  <validation type=\"STtch_barJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_barJERUP\" source=\"results_STtch_bar_JERUP\"/>
  </validation>

  <validation type=\"STtWchJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWchJERUP\" source=\"results_STtWch_JERUP\"/>
  </validation>

  <validation type=\"STtWch_barJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_barJERUP\" source=\"results_STtWch_bar_JERUP\"/>
  </validation>

  <validation type=\"STschJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STschJERUP\" source=\"results_STsch_JERUP\"/>
  </validation>

  <validation type=\"STsch_barJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_barJERUP\" source=\"results_STsch_bar_JERUP\"/>
  </validation>

  <validation type=\"WWJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WWJERUP\" source=\"results_WW_JERUP\"/>
  </validation>

  <validation type=\"WZJERUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_JERUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
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


  <superimpose name=\"mu_disc${sample[${i}]}__jer__plus\" title=\"BDT_response\" YTitle=\"Events\" Fill=\"true\" Weight=\"true\" Lumi=\"4678.0\" SetLogy=\"true\" Maximum=\"10e4\"  Minimum=\"10e-3\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__jer__plus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets__jer__plus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets__jer__plus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons__jer__plus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop__jer__plus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jer__plus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- JECDOWN -->
  <validation type=\"4Top${sample[${i}]}JECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_4Top${sample[${i}]}_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:4Top${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jes__minus\" source=\"results_4Top${sample[${i}]}_JECDOWN\"/>
  </validation>

  <validation type=\"ttbarJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__jes__minus\" source=\"results_ttbar_JECDOWN\"/>
  </validation>

  <validation type=\"WJetsJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WJets_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__wjets__jes__minus\" source=\"results_WJets_JECDOWN\"/>
  </validation>

  <validation type=\"ZJetsJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets__jes__minus\" source=\"results_ZJets_JECDOWN\"/>
  </validation>

  <validation type=\"STtchJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtchJECDOWN\" source=\"results_STtch_JECDOWN\"/>
  </validation>

  <validation type=\"STtch_barJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_barJECDOWN\" source=\"results_STtch_bar_JECDOWN\"/>
  </validation>

  <validation type=\"STtWchJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWchJECDOWN\" source=\"results_STtWch_JECDOWN\"/>
  </validation>

  <validation type=\"STtWch_barJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_barJECDOWN\" source=\"results_STtWch_bar_JECDOWN\"/>
  </validation>

  <validation type=\"STschJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STschJECDOWN\" source=\"results_STsch_JECDOWN\"/>
  </validation>

  <validation type=\"STsch_barJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_barJECDOWN\" source=\"results_STsch_bar_JECDOWN\"/>
  </validation>

  <validation type=\"WWJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WWJECDOWN\" source=\"results_WW_JECDOWN\"/>
  </validation>

  <validation type=\"WZJECDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_JECDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
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


  <superimpose name=\"mu_disc${sample[${i}]}__jes__minus\" title=\"BDT_response\" YTitle=\"Events\" Fill=\"true\" Weight=\"true\" Lumi=\"4678.0\" SetLogy=\"true\" Maximum=\"10e4\"  Minimum=\"10e-3\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__jes__minus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets__jes__minus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets__jes__minus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons__jes__minus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop__jes__minus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jes__minus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- JECUP -->
  <validation type=\"4Top${sample[${i}]}JECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_4Top${sample[${i}]}_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:4Top${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jes__plus\" source=\"results_4Top${sample[${i}]}_JECUP\"/>
  </validation>

  <validation type=\"ttbarJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__jes__plus\" source=\"results_ttbar_JECUP\"/>
  </validation>

  <validation type=\"WJetsJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WJets_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__wjets__jes__plus\" source=\"results_WJets_JECUP\"/>
  </validation>

  <validation type=\"ZJetsJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets__jes__plus\" source=\"results_ZJets_JECUP\"/>
  </validation>

  <validation type=\"STtchJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtchJECUP\" source=\"results_STtch_JECUP\"/>
  </validation>

  <validation type=\"STtch_barJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_barJECUP\" source=\"results_STtch_bar_JECUP\"/>
  </validation>

  <validation type=\"STtWchJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWchJECUP\" source=\"results_STtWch_JECUP\"/>
  </validation>

  <validation type=\"STtWch_barJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_barJECUP\" source=\"results_STtWch_bar_JECUP\"/>
  </validation>

  <validation type=\"STschJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STschJECUP\" source=\"results_STsch_JECUP\"/>
  </validation>

  <validation type=\"STsch_barJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_barJECUP\" source=\"results_STsch_bar_JECUP\"/>
  </validation>

  <validation type=\"WWJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WWJECUP\" source=\"results_WW_JECUP\"/>
  </validation>

  <validation type=\"WZJECUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_JECUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
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


  <superimpose name=\"mu_disc${sample[${i}]}__jes__plus\" title=\"BDT_response\" YTitle=\"Events\" Fill=\"true\" Weight=\"true\" Lumi=\"4678.0\" SetLogy=\"true\" Maximum=\"10e4\"  Minimum=\"10e-3\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__jes__plus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets__jes__plus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets__jes__plus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons__jes__plus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop__jes__plus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__jes__plus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- PUDOWN -->
  <validation type=\"4Top${sample[${i}]}PUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_4Top${sample[${i}]}_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:4Top${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__pu__minus\" source=\"results_4Top${sample[${i}]}_PUDOWN\"/>
  </validation>

  <validation type=\"ttbarPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__pu__minus\" source=\"results_ttbar_PUDOWN\"/>
  </validation>

  <validation type=\"WJetsPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WJets_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__wjets__pu__minus\" source=\"results_WJets_PUDOWN\"/>
  </validation>

  <validation type=\"ZJetsPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets__pu__minus\" source=\"results_ZJets_PUDOWN\"/>
  </validation>

  <validation type=\"STtchPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtchPUDOWN\" source=\"results_STtch_PUDOWN\"/>
  </validation>

  <validation type=\"STtch_barPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_barPUDOWN\" source=\"results_STtch_bar_PUDOWN\"/>
  </validation>

  <validation type=\"STtWchPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWchPUDOWN\" source=\"results_STtWch_PUDOWN\"/>
  </validation>

  <validation type=\"STtWch_barPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_barPUDOWN\" source=\"results_STtWch_bar_PUDOWN\"/>
  </validation>

  <validation type=\"STschPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STschPUDOWN\" source=\"results_STsch_PUDOWN\"/>
  </validation>

  <validation type=\"STsch_barPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_barPUDOWN\" source=\"results_STsch_bar_PUDOWN\"/>
  </validation>

  <validation type=\"WWPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WWPUDOWN\" source=\"results_WW_PUDOWN\"/>
  </validation>

  <validation type=\"WZPUDOWN\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_PUDOWN_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
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


  <superimpose name=\"mu_disc${sample[${i}]}__pu__minus\" title=\"BDT_response\" YTitle=\"Events\" Fill=\"true\" Weight=\"true\" Lumi=\"4678.0\" SetLogy=\"true\" Maximum=\"10e4\"  Minimum=\"10e-3\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__pu__minus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets__pu__minus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets__pu__minus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons__pu__minus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop__pu__minus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__pu__minus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

  <!-- PUUP -->
  <validation type=\"4Top${sample[${i}]}PUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_4Top${sample[${i}]}_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:4Top${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__pu__plus\" source=\"results_4Top${sample[${i}]}_PUUP\"/>
  </validation>

  <validation type=\"ttbarPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar__pu__plus\" source=\"results_ttbar_PUUP\"/>
  </validation>

  <validation type=\"WJetsPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WJets_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__wjets__pu__plus\" source=\"results_WJets_PUUP\"/>
  </validation>

  <validation type=\"ZJetsPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ZJets_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ZJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__zjets__pu__plus\" source=\"results_ZJets_PUUP\"/>
  </validation>

  <validation type=\"STtchPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtchPUUP\" source=\"results_STtch_PUUP\"/>
  </validation>

  <validation type=\"STtch_barPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtch_bar_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtch_barPUUP\" source=\"results_STtch_bar_PUUP\"/>
  </validation>

  <validation type=\"STtWchPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWchPUUP\" source=\"results_STtWch_PUUP\"/>
  </validation>

  <validation type=\"STtWch_barPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_barPUUP\" source=\"results_STtWch_bar_PUUP\"/>
  </validation>

  <validation type=\"STschPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STschPUUP\" source=\"results_STsch_PUUP\"/>
  </validation>

  <validation type=\"STsch_barPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STsch_bar_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:sch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STsch_barPUUP\" source=\"results_STsch_bar_PUUP\"/>
  </validation>

  <validation type=\"WWPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WW_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WW\">
    <TH1 name=\"mu_disc${sample[${i}]}__WWPUUP\" source=\"results_WW_PUUP\"/>
  </validation>

  <validation type=\"WZPUUP\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_PUUP_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
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


  <superimpose name=\"mu_disc${sample[${i}]}__pu__plus\" title=\"BDT_response\" YTitle=\"Events\" Fill=\"true\" Weight=\"true\" Lumi=\"4678.0\" SetLogy=\"true\" Maximum=\"10e4\"  Minimum=\"10e-3\" Stack=\"true\" >
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar__pu__plus\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets__pu__plus\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets__pu__plus\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons__pu__plus\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop__pu__plus\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}__pu__plus\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\"/>
  </superimpose>

</cuy>" >> $OUTFILE

../../cuy/scripts/cuy.py -x $OUTFILE  -f "#splitline{CMS Preliminary - 4.7 fb^{-1} at #sqrt{s}=7TeV}{#mu+jets, Njets #geq 4}" -o $OUTDIR -p "png" -O mu_disc${sample[${i}]}_templates.root -b -q
rm $OUTFILE
done

hadd mu_disc_templates.root mu_disc*
