#!/usr/bin/env python

'''
File: myRooFitWorkspace.py
Author: Alejandro Gomez Espinosa
Email: gomez@physics.rutgers.edu
Description: Read workspace from data and signal files and create Data Cards for Combination Tool Limits
'''

import sys
import os
from ROOT import *
from ROOT import RooFit,RooStats



################################################
### Create Data Card                        ####
################################################
def MakeDataCard( file, jesFile, Mass, dir ):
	
	nSigDataCard =  findNumExpecSignalInFile( file, Mass )
	nuisanceJesUnc = findUncInFile( jesFile, 2 ) ### 2 is the position of the jes unc in the file

	######### Create DataCard
	DataCard = open(dir+'DataCard_jj_'+str(Mass)+'_200.txt',"w")
	text = ['imax 1 channels \n',
			'jmax 1 backgrounds \n',
			'kmax * systematics \n',
			'------------------------------- \n',
			'shapes data_obs x /uscms_data/d3/algomez/files/Stops/Limits/workspace_dataPlusP4.root myDataWS:$PROCESS \n',
			'shapes bkgP4 x /uscms_data/d3/algomez/files/Stops/Limits/workspace_dataPlusP4.root myDataWS:$PROCESS \n',
			'shapes finalSignal_pdf x /uscms_data/d3/algomez/files/Stops/Limits/workspace_jj_'+str(Mass)+'_200.root mySignalWS:$PROCESS \n',
			'-------------------------------\n',
			'bin           x\n',
			'observation  -1\n',
			'-------------------------------\n',
			'bin           x          x\n',
			'process       0          1\n',
			'process     finalSignal_pdf     bkgP4\n',
			'rate        '+str(nSigDataCard)+'   1108.0001\n',
			'-------------------------------\n',
			'SigNormJes	lnN	'+str(nuisanceJesUnc)+'	-']	# I took 1108 from where I create the data workspace
	DataCard.writelines(text)
	DataCard.close()
	print ' Well done '
	
###############################################
##### Read Info in file                    ####
###############################################
def readInfo():
	with open('signalInfo.txt') as f:
		arrays = [map(float, line.split()) for line in f]
	for subarray in arrays:
		if subarray[0] == Mass:
			print subarray
			XS = subarray[1]
			Acceptance = subarray[2]
			Width = subarray[3]
	#print XS, Acceptance, Width


######################################################
##### Find number of expected signal from file    ####
######################################################
def findNumExpecSignalInFile( filename, mass  ):
	for line in open( filename ):
		columns = line.split()
		li=line.strip()
		if not li.startswith("#"):
			if ( mass in line ) and ( 'nom' in line ):
				return columns[8]

######################################################
##### Find uncertainty from file                  ####
######################################################
def findUncInFile( filename, unc ):
	for line in open( filename ):
		columns = line.split()
		li=line.strip()
		if not li.startswith("#"):
			return columns[ unc ]


################################################################################
if __name__ == "__main__":

	##### Initialize some arguments
	dir = '/uscms_data/d3/algomez/files/Stops/Limits/'
	txtfile = 'signalInfo.txt'
	txtJESfile = 'signalJESInfo.txt'
	st2mass = [ '450', '550', '650', '750' ]

	for mass in st2mass:
		MakeDataCard( dir+txtfile, dir+txtJESfile, mass, dir )
