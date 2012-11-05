#!/bin/bash 
########################################################
###  AUTHOR: Alejandro Gomez
###  email: alejandro.gomez@cern.ch
###  Little script to create xml files for cuy.py
###  
###  Setup propierly the following lines:

args=("$@")
#declare -a var

VARFILE="varcuy1.txt"
SAMPLE_DIR="/uscms/home/algomez/nobackup/files/fourtop/resultsTreeAnalyzer/results110/Nbjets3/"
sample=(tttt_Gh400 tttt_Gh500 tttt_Gh600 tttt_Gh700 tttt_Gh800  tttt_Gh900 tttt_Gh1000 tttt_SM ttbar )
#OUTFILEPLOTS='/uscms/home/algomez/public_html/plots/BDTv107/'

#Lumi="4678.0"
Lumi="5000.0"

############### Some setup variables
#Source=""
#var=("")
#bin=("")

##########################
### Some extra definitions
ndata="data"
nttbar="ttbar"
#nWjets="WJets"
nWjets="Wbb"
nZjets="ZJets"
nDibosons="WW"
nST="STtch"
n4TopSM="tttt_SM"
n4TopUED6="4TopUED6"
n4Top1200="tttt_Gh1200"
n4Top1000="tttt_Gh1000"
n4Top800="tttt_Gh800"
n4Top700="tttt_Gh700"
n4Top600="tttt_Gh600"
n4Top500="tttt_Gh500"
n4Top400="tttt_Gh400"
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


	echo "<superimpose name=\"${var}\" title=\"${var}\" YTitle=\"Unit Area\" Lumi=\"${Lumi}\" Normalized=\"true\" >" >> $OUTFILE
	in_array $ndata "${sample[@]}" && echo "<superimposeItem name=\"${var}_$ndata\" color=\"top\" legend=\"Data\"/>" >> $OUTFILE
	in_array $nttbar "${sample[@]}" && echo "<superimposeItem name=\"${var}_$nttbar\" color=\"top\" legend=\"TTbar\"/>" >> $OUTFILE
	in_array $n4Top400 "${sample[@]}" && echo "<superimposeItem name=\"${var}_${n4Top400}\" color=\"1\" legend=\"t#bar{t}t#bar{t} gh 0.4 TeV \" />" >> $OUTFILE
	in_array $n4Top500 "${sample[@]}" && echo "<superimposeItem name=\"${var}_${n4Top500}\" color=\"1\" legend=\"t#bar{t}t#bar{t} gh 0.5 TeV \" />" >> $OUTFILE
	in_array $n4Top600 "${sample[@]}" && echo "<superimposeItem name=\"${var}_${n4Top600}\" color=\"1\" legend=\"t#bar{t}t#bar{t} gh 0.6 TeV \"  />" >> $OUTFILE
	in_array $n4Top700 "${sample[@]}" && echo "<superimposeItem name=\"${var}_${n4Top700}\" color=\"1\" legend=\"t#bar{t}t#bar{t} gh 0.7 TeV \" />" >> $OUTFILE
	in_array $n4Top800 "${sample[@]}" && echo "<superimposeItem name=\"${var}_${n4Top800}\" color=\"1\" legend=\"t#bar{t}t#bar{t} gh 0.8 TeV \" />" >> $OUTFILE
	in_array $n4Top1000 "${sample[@]}" && echo "<superimposeItem name=\"${var}_${n4Top1000}\" color=\"1\" legend=\"t#bar{t}t#bar{t} gh 1.0 TeV \" />" >> $OUTFILE
	in_array $n4TopSM "${sample[@]}" && echo "<superimposeItem name=\"${var}_${n4TopSM}\" color=\"1\" legend=\"t#bar{t}t#bar{t} SM \" />" >> $OUTFILE
	echo '</superimpose>' >> $OUTFILE
	echo '</cuy>' >> $OUTFILE

	../../cuy/scripts/cuy.py -x $OUTFILE -O tmp_${var}.root -b -q 
	rm -rf $OUTFILE
done

#hadd variables.root tmp_*
#hadd Stjets2.root tmp_*
#rm tmp_*
