#!/bin/bash
####################################################################
##
## Simple script to count the number of events of several root files
##
## Alejandro Gomez Espinosa
## gomez@physics.rutgers.edu
##
## How to run: ./countNumberOfEvents.sh path/to/folder/
##
#####################################################################


directory=$1 #'/uscms/home/algomez/nobackup/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD'    # Directory from argument 1

ls -1 $directory | while read line                                                          # List the files in folder and while each line
do
	events=$(edmEventSize -v ${directory}/${line} | grep Events | awk '{ print $4 }' )  
	# edmEventsSize is CMSSW to create a list out of a root file, then search for the line displaying the Events, finally print only number of events
	echo $events >> tmp.txt                                                             # create a tmp file with numbers
done 
# Sum each number in the column (with awk) and print the total number of events
echo "Total Number of events in " $1 " :" $(cat tmp.txt | awk '{ sum+=$1} END { print sum}')
rm tmp.txt     									             ## remove the tmp file
