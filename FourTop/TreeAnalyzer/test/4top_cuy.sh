#!/bin/bash 
########################################################
###  AUTHOR: Alejandro Gomez
###  email: alejandro.gomez@cern.ch
###  Little script to create xml files for cuy.py
###  
###  Setup propierly the following lines:

args=("$@")
declare -a var

MAIN_Dir="/uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/"
SAMPLE_DIR="/uscms/home/algomez/nobackup/files/fourtop/resultsTreeAnalyzer/results14/"
#SAMPLE_DIR="/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results16/"
#sample=(data 4Top1100 4Top900 4Top500 4TopSM 4TopUED6 ttbar WJets ZJets STtch STtch_bar STsch STsch_bar STtWch STtWch_bar WW WZ)
sample=(data 4Top1100 4Top900 4Top500 4TopSM 4TopUED6 ttbar WJets ZJets STtch STtch_bar STsch STtWch STtWch_bar WW WZ)
#OUTFILE='test_'$Source'_cuy.xml'
OUTFILEPLOTS='/uscms/home/algomez/public_html/plots/BDTv4/'

Lumi="4678.0"
min='0.001'
max='10e4'

############### Some setup variables
Source=""
var=("")
bin=("")

if [ -n $1 ]; then
	if [ $1 = 'muons' ]; then
		Source='muons'
		var=("N_muons" "N_muons_1btag" "N_muons_cut0" "Nelectrons" "Nelectrons_1btag" "Nelectrons_cut0" "Ngood" "Niso" "deltaR" "deltaR_1btag" "deltaR_cut0" "muon_pt" "muon_pt_1btag" "muon_pt_cut1")
		bin=("0" "0" "0" "0" "0" "0" "0" "0" "0" "0" "0" "0" "10" "10")
		OUTFILE=$Source'_cuy.xml'
	elif [ $1 = 'PV' ]; then
		Source='PVs'
		var=("NPVreweight") 
		bin=("0")
		OUTFILE=$Source'_cuy.xml'
	elif [ $1 = 'jets' ]; then
		Source='jets'
		#var=("jet1_eta" "jet1_eta_1btag" "jet1_eta_cut0" "jet1_pt" "jet1_pt_1btag" "jet1_pt_cut0" "jet2_eta" "jet2_eta_1btag" "jet2_eta_cut0" "jet2_pt" "jet2_pt_1btag" "jet2_pt_cut0" "jet3_pt" "jet3_pt_1btag" "jet3_pt_cut0" "jet4_pt" "jet4_pt_1btag" "jet4_pt_cut0" "jet5_pt" "jet5_pt_cut0" "jet6_pt" "jet6_pt_cut0" "jet7_pt" "jet7_pt_cut0" "Dijet_deltaR" "Dijet_deltaR_1btag" "Nbjets_CSVL" "Nbjets_CSVM" "Nbjets_CSVT" "Njets" "Njets_1tag" "Njets_cut0" "Njets_cut1" "jet1_CSVL_pt" "jet1_CSVM_pt" "jet2_CSVL_pt" "jet2_CSVM_pt" "jet3_CSVL_pt" "jet3_CSVM_pt" "jet4_CSVL_pt" "jet4_CSVM_pt")
		var=("jet1_eta" "jet1_eta_1btag" "jet1_eta_cut0" "jet1_pt" "jet1_pt_1btag" "jet1_pt_cut0" "jet2_eta" "jet2_eta_1btag" "jet2_eta_cut0" "jet2_pt" "jet2_pt_1btag" "jet2_pt_cut0" "jet3_pt" "jet3_pt_1btag" "jet3_pt_cut0" "jet4_pt" "jet4_pt_1btag" "jet4_pt_cut0" "jet5_pt" "jet5_pt_cut0" "jet6_pt" "jet6_pt_cut0" "jet7_pt" "jet7_pt_cut0" "Dijet_deltaR" "Dijet_deltaR_1btag" "Nbjets_CSVL" "Nbjets_CSVM" "Nbjets_CSVT" "Njets" "Njets_1tag" "Njets_cut0" "Njets_cut1")
		bin=("0" "0" "0" "10" "10" "10" "0" "0" "0" "10" "10" "10" "10" "10" "10" "10" "10" "10" "12" "12" "12" "12" "12" "12" "0" "0" "0" "0" "0" "0" "0" "0" "0")
		#bin=("0" "0" "0" "10" "10" "10" "0" "0" "0" "10" "10" "10" "10" "10" "10" "10" "10" "10" "12" "12" "12" "12" "12" "12" "0" "0" "0" "0" "0" "0" "0" "0" "0" "10" "10" "10" "10" "10" "10" "10" "10" )
		OUTFILE=$Source'_cuy.xml'
	elif [ $1 = 'mass' ]; then
		Source='mass'
		var=("Mt" "Mt_2jet" "Mt_2jet_1btag" "dijet" "dileadjet")
		bin=("10" "6" "6" "10" "10")
		OUTFILE=$Source'_cuy.xml'
	elif [ $1 = 'MET' ]; then
		Source='MET'
		var=("Ht" "Ht0" "Ht_1btag" "LepWPt" "LepWmass" "MET" "MET_1btag" "MET_cut0" "PzNu" "Stjet" "Stjet_1btag" "Stlep" "Stlep_1btag" "deltaPhiMETlep" "deltaPhiWW" "deltaRWW" "deltaPhiNulep" "deltaPhiMETWlep")
		bin=("10" "10" "10" "10" "10" "10" "10" "10" "15" "10" "10" "10" "10" "0" "0" "0" "0" "0")
		OUTFILE=$Source'_cuy.xml'
	else
		echo "option not found."
	fi
fi

#########################################################


########################################################
###   Here begins the code
###   Modify it if you need something extra
###

############################
## Simple function to compare elements in an array
in_array(){
	local val1 val2=$1
	shift
	for val1; do
		[[ $val1 == $val2 ]] && return 0
	done
	return 1
}
##########################
### Some extra definitions
ndata="data"
nttbar="ttbar"
nWjets="WJets"
nZjets="ZJets"
nDibosons="WW"
nST="STtch"
n4TopSM="4TopSM"
n4TopUED6="4TopUED6"
n4Top1100="4Top1100"
n4Top900="4Top900"
n4Top500="4Top500"

/bin/rm -f $OUTFILE
echo "<!-- xml file for cuy.py to create nice plots. More details please contact: Alejandro Gomez (alejandro.gomez@cern.ch) -->

<cuy>
" >> $OUTFILE

sample_elements=${#sample[@]}
var_elements=${#var[@]}
bin_elements=${#bin[@]}
for ((i=0;i<$sample_elements;i++));
do 
	echo "<validation type=\"${sample[${i}]}\" file=\"${SAMPLE_DIR}results_${sample[${i}]}.root\" weight=\"MCScaleFactors.txt:${sample[${i}]}\">" >> $OUTFILE 

	for ((j=0;j<$var_elements;j++));
	do
		echo "<TH1 name=\"${var[${j}]}_${sample[${i}]}\" source=\"/${Source}/${var[${j}]}_${sample[${i}]}\"/>" >> $OUTFILE
	done
	echo "</validation>" >> $OUTFILE
	echo '' >> $OUTFILE
done
echo '<additionArray name="ST" title="ST"> 
<additionArrayItem array="STtch"/>
<additionArrayItem array="STtWch"/>
<additionArrayItem array="STsch"/>
<additionArrayItem array="STtch_bar"/>
<additionArrayItem array="STtWch_bar"/>
</additionArray>

<additionArray name="Dibosons" title="Dibosons">
<additionArrayItem array="WW"/>
<additionArrayItem array="WZ"/>
</additionArray>
'>> $OUTFILE

#<additionArrayItem array="STsch_bar"/>
for ((j=0;j<$var_elements;j++));
do
	if [ ${bin[${j}]} -eq 0 ]; then
		echo "<superimpose name=\"${var[${j}]}\" title=\"${var[${j}]}\" YTitle=\"Events\" Fill=\"true\" Weight=\"true\" Lumi=\"${Lumi}\" Stack=\"true\" SetLogy=\"true\" Minimum=\"${min}\" Maximum=\"${max}\" PlotDiff=\"true\" doKS=\"true\">" >> $OUTFILE
	else
		echo "<superimpose name=\"${var[${j}]}\" title=\"${var[${j}]}\" YTitle=\"Events/${bin[${j}]} GeV\" Fill=\"true\" Weight=\"true\" Lumi=\"${Lumi}\" Stack=\"true\" SetLogy=\"true\" Minimum=\"${min}\" Maximum=\"${max}\" PlotDiff=\"true\" doKS=\"true\">" >> $OUTFILE 
	fi
	in_array $ndata "${sample[@]}" && echo "<superimposeItem name=\"${var[${j}]}_$ndata\" color=\"top\" legend=\"Data\"/>" >> $OUTFILE
	in_array $nttbar "${sample[@]}" && echo "<superimposeItem name=\"${var[${j}]}_$nttbar\" color=\"top\" legend=\"TTbar\"/>" >> $OUTFILE
	in_array $nWjets "${sample[@]}" && echo "<superimposeItem name=\"${var[${j}]}_$nWjets\" color=\"top\" legend=\"Wjets\"/>" >> $OUTFILE
	in_array $nZjets "${sample[@]}" && echo "<superimposeItem name=\"${var[${j}]}_$nZjets\" color=\"top\" legend=\"Zjets\"/>" >> $OUTFILE
	in_array $nDibosons "${sample[@]}" && echo "<superimposeItem name=\"${var[${j}]}_Dibosons\" color=\"top\" legend=\"Dibosons\"/>" >> $OUTFILE
	in_array $nST "${sample[@]}" && echo "<superimposeItem name=\"${var[${j}]}_ST\" color=\"top\" legend=\"ST\"/>" >> $OUTFILE
	in_array $n4TopSM "${sample[@]}" && echo "<superimposeItem name=\"${var[${j}]}_$n4TopSM\" color=\"1\" legend=\"t#bar{t}t#bar{t} SM\" NoStack=\"true\" />" >> $OUTFILE
	in_array $n4TopUED6 "${sample[@]}" && echo "<superimposeItem name=\"${var[${j}]}_$n4TopUED6\" color=\"1\" legend=\"t#bar{t}t#bar{t} UED6\" NoStack=\"true\" />" >> $OUTFILE
	in_array $n4Top1100 "${sample[@]}" && echo "<superimposeItem name=\"${var[${j}]}_$n4Top1100\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh 1.1 TeV\" NoStack=\"true\" />" >> $OUTFILE
	in_array $n4Top900 "${sample[@]}" && echo "<superimposeItem name=\"${var[${j}]}_$n4Top900\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh 0.9 TeV\" NoStack=\"true\" />" >> $OUTFILE
	in_array $n4Top500 "${sample[@]}" && echo "<superimposeItem name=\"${var[${j}]}_$n4Top500\" color=\"1\" legend=\"t#bar{t}t#bar{t} Gh 0.5 TeV\" NoStack=\"true\" />" >> $OUTFILE
echo '</superimpose>
' >> $OUTFILE
done
echo '</cuy>' >> $OUTFILE


if [ -z $2 ]; then 
	echo "
$OUTFILE was created. Please check manually some setup for each variable, like the size of the bins, and rerun:

../../cuy/scripts/cuy.py -x $OUTFILE -f \"#splitline{CMS Preliminary - 4.7 fb^{-1} at #sqrt{s}=7TeV}{#mu+jets, Njets #geq 4}\" -o $OUTFILEPLOTS -p \"png\" -b -q
	"
else
	../../cuy/scripts/cuy.py -x $OUTFILE -f " #splitline{CMS Preliminary - 4.7 fb^{-1} at #sqrt{s}=7TeV}{#mu+jets, Njets #geq 4}" -o $OUTFILEPLOTS -p "png" -b -q 
	echo "To run again: 
../../cuy/scripts/cuy.py -x $OUTFILE -f \"#splitline{CMS Preliminary - 4.7 fb^{-1} at #sqrt{s}=7TeV}{#mu+jets, Njets #geq 4}\" -o $OUTFILEPLOTS -p \"png\" -b -q
"
fi
