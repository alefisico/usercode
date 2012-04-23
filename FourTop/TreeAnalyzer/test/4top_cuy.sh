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
SAMPLE_DIR="/uscms/home/algomez/nobackup/files/fourtop/resultsTreeAnalyzer/resultsFinal/"
#SAMPLE_DIR="/eos/uscms/store/user/algomez/FourTop/resultsTreeAnalyzer/results16/"
#sample=(data 4Top1100 4Top900 4Top500 4TopSM 4TopUED6 ttbar WJets ZJets STtch STtch_bar STsch STsch_bar STtWch STtWch_bar WW WZ)
#sample=(data tttt_Gh1200 tttt_Gh1000 tttt_Gh800 tttt_Gh600 tttt_Gh400 ttbar WJets ZJets STtch STtch_bar STsch_bar STsch STtWch STtWch_bar WW WZ)
sample=(data tttt_Gh400 tttt_Gh600 tttt_Gh800 ttbar Wbb Wcc Wqq ZJets STtch STtch_bar STsch_bar STsch STtWch STtWch_bar WW WZ)
OUTFILEPLOTS='/uscms/home/algomez/public_html/plots/BDTFinal/tmp/'
#OUTFILEPLOTS='/uscms/home/algomez/public_html/plots/tmp/'
#BANNER='#mu+jets'
#BANNER='#mu+jets, Njets #geq 4'
#BANNER='#mu+jets, Njets #geq 4, #geq 1 btag CSVL'
BANNER='#mu+jets, sel + S_{T}^{jet} > 500GeV'

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
n4TopSM="4TopSM"
n4TopUED6="4TopUED6"
n4Top1200="tttt_Gh1200"
n4Top1000="tttt_Gh1000"
n4Top800="tttt_Gh800"
n4Top600="tttt_Gh600"
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

	echo '<additionArray name="ST" title="ST"> 
<additionArrayItem array="STtch"/>
<additionArrayItem array="STtWch"/>
<additionArrayItem array="STsch"/>
<additionArrayItem array="STsch_bar"/>
<additionArrayItem array="STtch_bar"/>
<additionArrayItem array="STtWch_bar"/>
</additionArray>

<additionArray name="wjets" title="wjets">
<additionArrayItem array="Wbb" weight="1.21"/>
<additionArrayItem array="Wcc" weight="1.66"/>
<additionArrayItem array="Wqq" weight="0.58"/> 
</additionArray>

<additionArray name="Dibosons" title="Dibosons">
<additionArrayItem array="WW"/>
<additionArrayItem array="WZ"/>
</additionArray>
'>> $OUTFILE

	if [ $setlog != "false" ]; then
		if [ $bin != "0" ]; then
			echo "<superimpose name=\"${var}_log\" title=\"${var}_log\" YTitle=\"Events/${bin}\" Fill=\"true\" Weight=\"true\" Lumi=\"${Lumi}\" Stack=\"true\" SetLogy=\"${setlog}\" Minimum=\"${min}\" Maximum=\"${max}\" PlotDiff=\"true\" doKS=\"true\" >" >> $OUTFILE
		else
			echo "<superimpose name=\"${var}_log\" title=\"${var}_log\" Fill=\"true\" Weight=\"true\" Lumi=\"${Lumi}\" Stack=\"true\" SetLogy=\"${setlog}\" Minimum=\"${min}\" Maximum=\"${max}\" PlotDiff=\"true\" doKS=\"true\" >" >> $OUTFILE
		fi
       else
		if [ $bin != "0" ]; then
	       		echo "<superimpose name=\"${var}\" title=\"${var}\" YTitle=\"Events/${bin}\" Fill=\"true\" Weight=\"true\" Lumi=\"${Lumi}\" Stack=\"true\" PlotDiff=\"true\" doKS=\"true\" >" >> $OUTFILE
		else
	       		echo "<superimpose name=\"${var}\" title=\"${var}\" Fill=\"true\" Weight=\"true\" Lumi=\"${Lumi}\" Stack=\"true\" PlotDiff=\"true\" doKS=\"true\" >" >> $OUTFILE
		fi
       fi
	in_array $ndata "${sample[@]}" && echo "<superimposeItem name=\"${var}_$ndata\" color=\"1\" legend=\"Data\"/>" >> $OUTFILE
	in_array $nttbar "${sample[@]}" && echo "<superimposeItem name=\"${var}_$nttbar\" color=\"top\" legend=\"TTbar\" Error=\"0.30\"/>" >> $OUTFILE
	in_array $nWjets "${sample[@]}" && echo "<superimposeItem name=\"${var}_wjets\" color=\"top\" legend=\"Wjets\" Error=\"0.30\"/>" >> $OUTFILE
	in_array $nZjets "${sample[@]}" && echo "<superimposeItem name=\"${var}_$nZjets\" color=\"top\" legend=\"Zjets\" />" >> $OUTFILE
	in_array $nDibosons "${sample[@]}" && echo "<superimposeItem name=\"${var}_Dibosons\" color=\"top\" legend=\"Dibosons\"/>" >> $OUTFILE
	in_array $nQCD "${sample[@]}" && echo "<superimposeItem name=\"${var}_QCD\" color=\"top\" legend=\"QCD\"/>" >> $OUTFILE
	in_array $nST "${sample[@]}" && echo "<superimposeItem name=\"${var}_ST\" color=\"top\" legend=\"ST\" />" >> $OUTFILE
	#in_array $n4TopSM "${sample[@]}" && echo "<superimposeItem name=\"${var}_$n4TopSM\" color=\"1\" legend=\"t#bar{t}t#bar{t} SM\" NoStack=\"true\" />" >> $OUTFILE
	#in_array $n4TopUED6 "${sample[@]}" && echo "<superimposeItem name=\"${var}_$n4TopUED6\" color=\"1\" legend=\"t#bar{t}t#bar{t} UED6\" NoStack=\"true\" />" >> $OUTFILE
	in_array $n4Top400 "${sample[@]}" && echo "<superimposeItem name=\"${var}_$n4Top400\" color=\"1\" legend=\"t#bar{t}t#bar{t} gh 0.4 TeV (x20)\" NoStack=\"true\" SF=\"20\" />" >> $OUTFILE
	in_array $n4Top600 "${sample[@]}" && echo "<superimposeItem name=\"${var}_$n4Top600\" color=\"1\" legend=\"t#bar{t}t#bar{t} gh 0.6 TeV (x20)\" NoStack=\"true\" SF=\"20\" />" >> $OUTFILE
	in_array $n4Top800 "${sample[@]}" && echo "<superimposeItem name=\"${var}_$n4Top800\" color=\"1\" legend=\"t#bar{t}t#bar{t} gh 0.8 TeV (x20)\" NoStack=\"true\"  SF=\"20\"/>" >> $OUTFILE
	in_array $n4Top1000 "${sample[@]}" && echo "<superimposeItem name=\"${var}_$n4Top1000\" color=\"1\" legend=\"t#bar{t}t#bar{t} gh 1.0 TeV (x20)\" NoStack=\"true\"  SF=\"20\"/>" >> $OUTFILE
	in_array $n4Top1200 "${sample[@]}" && echo "<superimposeItem name=\"${var}_$n4Top1200\" color=\"1\" legend=\"t#bar{t}t#bar{t} gh 1.2 TeV (x20)\" NoStack=\"true\"  SF=\"20\"/>" >> $OUTFILE
	echo '</superimpose>' >> $OUTFILE
	echo '</cuy>' >> $OUTFILE

	../../cuy/scripts/cuy.py -x $OUTFILE -f " #splitline{CMS Preliminary - 5.0 fb^{-1} at #sqrt{s}=7TeV}{$BANNER}" -o $OUTFILEPLOTS -p "pdf" -b -q 
	rm -rf $OUTFILE
done
