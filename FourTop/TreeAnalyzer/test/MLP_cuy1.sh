#!/bin/bash 
#sample=(400 500 600 700 800 900 1000)
sample=(  600)
#num=(0.4 0.5 0.6 0.7 0.8 0.9 1.0 )
num=( 0.6 )
Minimum='10e-1'
Maximum='10e3'
XMinimum='-0.3'
XMaximum='1.0'

version='v1'
#results='resultsFinal'
results='results110/Nbjets3'
#OUTDIR='~/public_html/plots/MLPFinal/'
OUTDIR='~/public_html/plots/BDTv110/Nbjets3/'

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

  <validation type=\"STtWch\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch\" source=\"results_STtWch\"/>
  </validation>

  <validation type=\"STtWch_bar\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_STtWch_bar_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:tWch_bar\">
    <TH1 name=\"mu_disc${sample[${i}]}__STtWch_bar\" source=\"results_STtWch_bar\"/>
  </validation>

  <validation type=\"WZ\" file=\"/uscms_data/d3/algomez/files/fourtop/resultsTreeAnalyzer/${results}/MLPResponse${version}/${sample[${i}]}/results_WZ_MLP_ClassApplication.root\" weight=\"MCScaleFactors.txt:WZ\">
    <TH1 name=\"mu_disc${sample[${i}]}__WZ\" source=\"results_WZ\"/>
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

  <superimpose name=\"MLPoutput${sample[${i}]}\" title=\"MLP_response\" XTitle=\"MLP Discriminator\" YTitle=\"Events/0.02\" Fill=\"true\" Weight=\"true\" Lumi=\"5000.0\" Stack=\"true\" SetLogy=\"true\" PlotDiff=\"true\" Minimum=\"$Minimum\" Maximum=\"$Maximum\" XMinimum=\"$XMinimum\" XMaximum=\"$XMaximum\"  > 
    <superimposeItem name=\"mu_disc${sample[${i}]}__DATA\" color=\"top\" legend=\"Data\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__ttbar\" color=\"top\" legend=\"TTbar\" Error=\"0.30\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__wjets\" color=\"top\" legend=\"Wjets\" Error=\"0.30\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__zjets\" color=\"top\" legend=\"Zjets\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__dibosons\" color=\"top\" legend=\"Dibosons\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__singletop\" color=\"top\" legend=\"ST\"/>
    <superimposeItem name=\"mu_disc${sample[${i}]}__fourtop${sample[${i}]}\" color=\"1\" legend=\"t#bar{t}t#bar{t} G_{H} ${num[${i}]} TeV (x5)\" NoStack=\"true\" SF=\"5\" />
  </superimpose>

</cuy>" >> $OUTFILE

../../cuy/scripts/cuy.py -x $OUTFILE  -f "#splitline{CMS Preliminary - 5.0 fb^{-1} at #sqrt{s}=7TeV}{#mu+jets, Njets #geq 4, Ht > 300, Nbtag #geq 3}" -o $OUTDIR -p "pdf"  -b -q

#../../cuy/scripts/cuy.py -x $OUTFILE  -f "#splitline{CMS Preliminary 2011}{5.0 fb^{-1} at #sqrt{s}=7TeV #mu+jets}" -o $OUTDIR -p "pdf" -b -q 
rm $OUTFILE
done
