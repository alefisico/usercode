#!/bin/bash 
sample=(SM)
num=(sm)
version='SM'
results='results110/Nbjets3'
OUTDIR='~/public_html/plots/BDTv110/Nbjets3/'
Minimum='10e-3'
Maximum='10e4'
XMinimum='-0.8'
XMaximum='0.2'


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

  <superimpose name=\"BDToutput${sample[${i}]}\" title=\"BDT_response\" XTitle=\"BDT Discriminator\" YTitle=\"Events/0.02\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" SetLogy=\"true\" PlotDiff=\"true\" Minimum=\"$Minimum\" Maximum=\"$Maximum\" XMinimum=\"$XMinimum\" XMaximum=\"$XMaximum\"  >
    <superimposeItem name=\"mu_disc${num[${i}]}__DATA\" color=\"top\" legend=\"Data\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__ttbar\" color=\"top\" legend=\"TTbar\" Error=\"0.30\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__wjets\" color=\"top\" legend=\"Wjets\" Error=\"0.30\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__zjets\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__dibosons\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__singletop\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${num[${i}]}__fourtop${num[${i}]}\" color=\"1\" legend=\"t#bar{t}t#bar{t} ${sample[${i}]} TeV (x5)\" NoStack=\"true\" SF=\"5\" />
  </superimpose>

</cuy>" >> $OUTFILE

../../cuy/scripts/cuy.py -x $OUTFILE  -f "#splitline{CMS Preliminary - 5.0 fb^{-1} at #sqrt{s}=7TeV}{#mu+jets, Njets #geq 4, Ht > 300, Nbtag #geq 3}" -o $OUTDIR -p "pdf" -b -q 
#../../cuy/scripts/cuy.py -x $OUTFILE  -f "#splitline{CMS Preliminary 2011}{5.0 fb^{-1} at #sqrt{s}=7TeV #mu+jets}" -o $OUTDIR -p "pdf" -b -q 
rm $OUTFILE
done

