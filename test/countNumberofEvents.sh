#!/bin/bash

directory=$1 #'/uscms/home/algomez/nobackup/files/stops/AOD/st2_h_bb_st1_bj_250_100_AOD'

ls -1 $directory | while read line
do
	totalevents=0	
	events=$(edmEventSize -v ${directory}/${line} | grep Events | awk '{ print $4 }' )
	echo $events >> tmp.txt
done 

echo "Total Number of events in " $1 " :" $(cat tmp.txt | awk '{ sum+=$1} END { print sum}')
rm tmp.txt
