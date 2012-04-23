#!/bin/bash 
#sample=(400 600 800)
sample=(1200)
#num=(0.4 0.6 0.8 )
num=(1.2 )
version='v1'
results='resultsFinal'
OUTDIR='~/public_html/plots/BDTFinal/'

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

  <validation type=\"STtWch\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch\" source=\"results_STtWch\"/>
  </validation>

  <validation type=\"STtWch_bar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_STtWch_bar_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_bar\" source=\"results_STtWch_bar\"/>
  </validation>

  <validation type=\"WZ\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_WZ_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${sample[${i}]}__WZ\" source=\"results_WZ\"/>
  </validation>

  <validation type=\"tttt_Gh${sample[${i}]}\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/BDTResponse${version}/${sample[${i}]}/results_tttt_Gh${sample[${i}]}_BDT_ClassApplication.root\" weight=\"MCScaleFactors.txt:tttt_Gh${sample[${i}]}\">
    <TH1 name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}\" source=\"results_tttt_Gh${sample[${i}]}\"/>
  </validation>

  <additionArray name=\"singletop\" title=\"singletop\">
    <additionArrayItem array=\"STtWch\"/>
    <additionArrayItem array=\"STtWch_bar\"/>
  </additionArray>

  <additionArray name=\"dibosons\" title=\"dibosons\">
    <additionArrayItem array=\"WZ\"/>
  </additionArray>

  <additionArray name=\"wjets\" title=\"wjets\">
    <additionArrayItem array=\"Wbb\" weight=\"1.21\"/>
    <additionArrayItem array=\"Wcc\" weight=\"1.66\"/>
    <additionArrayItem array=\"Wqq\" weight=\"0.58\"/> 
  </additionArray>

  <superimpose name=\"mu_disc${sample[${i}]}\" title=\"BDT_response\" XTitle=\"BDT Discriminator\" YTitle=\"Events/0.02\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" SetLogy=\"true\" PlotDiff=\"true\" Minimum=\"10e-4\" Maximum=\"10e3\"  >
    <superimposeItem name=\"mu_disc${sample[${i}]}__DATA\" color=\"top\" legend=\"Data\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar\" color=\"top\" legend=\"TTbar\" Error=\"0.30\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets\" color=\"top\" legend=\"Wjets\" Error=\"0.30\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}\" color=\"1\" legend=\"t#bar{t}t#bar{t} gh ${num[${i}]} TeV (x20)\" NoStack=\"true\" SF=\"20\" />
  </superimpose>

</cuy>" >> $OUTFILE

../../cuy/scripts/cuy.py -x $OUTFILE  -f "#splitline{CMS Preliminary - 4.7 fb^{-1} at #sqrt{s}=7TeV}{#mu+jets, Njets #geq 4, Ht > 300, Nbtag #geq 1}" -o $OUTDIR -p "pdf"  
rm $OUTFILE
done
