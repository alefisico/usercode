#!/bin/bash
####################################################################
##
## Simple script to count the number of events of several root files
##
## Alejandro Gomez Espinosa
## gomez@physics.rutgers.edu
##
## How to run: ./countNumberOfEvents.sh Stop1Mass Stop2Mass Process
## 
## This script counts the number of events and move events to HexFarm
##
#####################################################################

st1=$1
st2=$2
process=$3

directory='/pnfs/cms/WAX/11/store/user/algomez/st2_h_bb_st1_'${process}'_'$st2'_'$st1'_AOD'    # Directory from argument 1
tmp_dir='/uscms_data/d3/algomez/tmp/'
toHexfarm='/cms/karen/algomez/Stops/AOD/st2_h_bb_st1_'${process}'_'$st2'_'$st1'/'

ls -1 $directory | while read line                                                          # List the files in folder and while each line
do
	dccp ${directory}/${line} $tmp_dir
	echo 'Copying '${directory}/${line} 'to '$tmp_dir
done

ls -1 $tmp_dir | while read line                                                          # List the files in folder and while each line
do
	events=$(edmEventSize -v ${tmp_dir}/${line} | grep Events | awk '{ print $4 }' )  
	# edmEventsSize is CMSSW to create a list out of a root file, then search for the line displaying the Events, finally print only number of events
	echo $events >> tmp.txt                                                             # create a tmp file with numbers
done 

#Move files to my Rutgers area
scp ${tmp_dir}/*root gomez@hexcms.rutgers/edu:${toHexfarm}

# Sum each number in the column (with awk) and print the total number of events
echo "Total Number of events in " $1 " :" $(cat tmp.txt | awk '{ sum+=$1} END { print sum}')
rm tmp.txt     									             ## remove the tmp file
rm $tmp_dir/*
