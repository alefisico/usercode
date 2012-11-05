#!/bin/bash 
sample=(400 500 600 700 800 900 1000)
#sample=(800 900 1000)
num=(0.4 0.5 0.6 0.7 0.8 0.9 1.0)
version='v1'
#results='results108'
#results='results108/CSVM1/' #Nbjets3/' ##
results='results108/Nbjets3/' 

sample_elements=${#sample[@]}
num_elements=${#num[@]}
for ((i=0;i<$sample_elements;i++));
do
	OUTFILE="BDT_cuy_tmp_${sample[${i}]}.xml"
	echo "<cuy>
  <validation type=\"data\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_data_BDT_ClassApplication.root\" weight=\"1\">
    <TH1 name=\"mu_disc${sample[${i}]}__DATA\" source=\"results_data\"/>
  </validation>

  <validation type=\"tttt_Gh${sample[${i}]}\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_tttt_Gh${sample[${i}]}_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_Gh${sample[${i}]}\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}\" source=\"results_tttt_Gh${sample[${i}]}\"/>
  </validation>

  <validation type=\"ttbar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_ttbar_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:ttbar\"> 
    <TH1 name=\"mu_disc${sample[${i}]}__ttbar\" source=\"results_ttbar\"/>
  </validation>

  <!--<validation type=\"WJets\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WJets_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WJets\">
    <TH1 name=\"mu_disc${sample[${i}]}__wjets\" source=\"results_WJets\"/>
  </validation>-->

  <validation type=\"Wbb\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wbb_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wbb\">
    <TH1 name=\"mu_disc${sample[${i}]}__Wbb\" source=\"results_Wbb\"/>
  </validation>

  <validation type=\"Wcc\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wcc_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wcc\">
    <TH1 name=\"mu_disc${sample[${i}]}__Wcc\" source=\"results_Wcc\"/>
  </validation>

  <validation type=\"Wqq\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_Wqq_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:Wqq\">
    <TH1 name=\"mu_disc${sample[${i}]}__Wqq\" source=\"results_Wqq\"/>
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

  <superimpose name=\"mu_disc${sample[${i}]}\" title=\"BDT_response\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\"  >
    <superimposeItem name=\"mu_disc${sample[${i}]}__DATA\" color=\"top\" legend=\"Data\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar\" color=\"top\" legend=\"TTbar\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets\" color=\"top\" legend=\"Wjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh ${num[${i}]} TeV\" NoStack=\"true\" />
  </superimpose>

</cuy>" >> $OUTFILE

../../cuy/scripts/cuy.py -x $OUTFILE  -O mu_disc${sample[${i}]}_templates_v0.root -b -q
rm $OUTFILE
done

hadd mu_disc_templates_v0.root mu_disc*
mv mu_disc* /uscms/home/algomez/nobackup/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/
