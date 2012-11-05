#!/bin/bash 
########################################################
###  AUTHOR: Alejandro Gomez
###  email: alejandro.gomez@cern.ch
###  Little script to create xml files for cuy.py
###  
###  Setup propierly the following lines:

args=("$@")
#declare -a var

VARFILE="varcuy.txt"
MAIN_Dir="/uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/"
#SAMPLE_DIR="/uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/"
SAMPLE_DIR="/uscms/home/algomez/nobackup/files/fourtop/resultsTreeAnalyzer/results110/Nbjets3/"
#sample=(ttbar ttbar_powheg)
sample=(ttbar TTbar)
OUTFILEPLOTS='/uscms/home/algomez/public_html/plots/BDTv110/Nbjets3/'

Lumi="5000.0"

############### Some setup variables
#Source=""
#var=("")
#bin=("")

##########################
### Some extra definitions
ndata="data"
nttbar="ttbar"
nTTbar="TTbar"
nWjets="WJets"
nZjets="ZJets"
nDibosons="WW"
nST="STtch"
n4TopSM="4TopSM"
n4TopUED6="4TopUED6"
n4Top1100="4Top1100"
n4Top900="4Top900"
n4Top500="4Top500"
nQCD='QCD'

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

grep -v '^[[:space:]]*#' $VARFILE| cat | while read line;
do
	var=`echo $line | awk '{ print $1}'`;
	Source=`echo $line | awk '{ print $2}'`;
	bin=`echo $line | awk '{ print $3}'`;
	max=`echo $line | awk '{ print $4}'`;
	min=`echo $line | awk '{ print $5}'`;
	setlog=`echo $line | awk '{ print $6}'`;
	OUTFILE='test_'$Source'_cuy.xml'

#########################################################


########################################################
###   Here begins the code
###   Modify it if you need something extra
###

	/bin/rm -f $OUTFILE
	echo "<!-- xml file for cuy.py to create nice plots. More details please contact: Alejandro Gomez (alejandro.gomez@cern.ch) -->

<cuy>" >> $OUTFILE

	sample_elements=${#sample[@]}
	for ((i=0;i<$sample_elements;i++));
	do 
		echo "<validation type=\"${sample[${i}]}\" file=\"${SAMPLE_DIR}results_${sample[${i}]}.root\" weight=\"MCScaleFactors.txt:${sample[${i}]}\">" >> $OUTFILE 

		echo "<TH1 name=\"${var}_${sample[${i}]}\" source=\"/${Source}/${var}_${sample[${i}]}\"/>" >> $OUTFILE
	echo "</validation>" >> $OUTFILE
	echo '' >> $OUTFILE
	done

	echo "<superimpose name=\"${var}_ttbardiff\" title=\"${var}_ttbardiff\" Fill=\"true\" YTitle=\"Unit Area\" Lumi=\"${Lumi}\" Normalize=\"false\" >" >> $OUTFILE
	#echo "<superimpose name=\"${var}\" title=\"${var}\" Fill=\"true\" Weight=\"true\" Lumi=\"${Lumi}\" Stack=\"false\"  >" >> $OUTFILE
	echo "<superimposeItem name=\"${var}_TTbar\" color=\"1\" legend=\"TTbarOther\"  />" >> $OUTFILE
	echo "<superimposeItem name=\"${var}_ttbar\" color=\"1\" legend=\"TTbar\"  />" >> $OUTFILE
	echo '</superimpose>' >> $OUTFILE
	echo '</cuy>' >> $OUTFILE

	#../../cuy/scripts/cuy.py -x $OUTFILE -f "CMS 2011 Simulation" -o $OUTFILEPLOTS -p "pdf" -b -q 
	../../cuy/scripts/cuy.py -x $OUTFILE -O tmp_Ttbardiff_${var}.root -b -q 
	rm -rf $OUTFILE
done

hadd TTbardiff.root tmp_Ttbardiff*
rm tmp_Ttbardiff*
