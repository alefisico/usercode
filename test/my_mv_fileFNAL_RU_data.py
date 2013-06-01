#! /bin/env python
import os
import sys
import string

####################################
##     Parameters to Modify        ##
#####################################
#right now we copy from OSU to Rutgers
#FromSRM      =  'srm://cmssrm.fnal.gov:8443/srm/managerv2?SFN=/11/store/user/clseitz/TLBM52xData/'
#FromSRM      =  'srm://cmssrm.fnal.gov:8443/srm/managerv2?SFN=/11/store/user/lpctlbsm/crsilk/'
FromSRM      =  'srm://cmssrm.fnal.gov:8443/srm/managerv2?SFN=/11/store/user/algomez/'
Fromls      =  '/pnfs/cms/WAX/11/store/user/algomez/'

#ToSRM      = 'srm://ruhex-osgce.rutgers.edu:8443/srm/v2/server?SFN=/cms/data24/algomez/'
ToSRM      = '/uscms_data/d3/algomez/files/RPV_M300_112_ISRFSRdown_1/'
ToHexfarm  = '/cms/data24/algomez/RPV_M300_112_ISRFSRdown_1/'

#define which dataset to copu
#dataset = 'MultiJet/'
#dataset = 'SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_v9_Summer11-PU_S4_START42_V11-v1/'
dataset = 'RPV_M300_112_ISRFSRdown_2/'


#run = 'Run2012A-PromptReco-v1_Json0601'
#run = 'Run2012B-PromptReco-v1_Json0601'
#run = '2f45e7f986681f9c0bbcc7cc31425dac'


#####################################
##            Main Code            ##
#####################################
#make list of all the files in a directory
files=os.popen("ls -1 "+Fromls+dataset)
for FileName in files.readlines():
	#print "myresult:",i,
	#cmd = 'lcg-cp -b -D srmv2 "' + FromSRM +dataset+ FileName.rstrip('\r\n') +'" "file://' + ToSRM + FileName.rstrip('\r\n')+ '"'
	#cmd = 'dccp "' + Fromls +dataset+ FileName.rstrip('\r\n') +'" .' 
	cmd = 'dccp "' + FileName.rstrip('\r\n') +'" .' 
	print 'Performing: '+cmd
	#execute the command, uncomment line below
        os.popen(cmd)
print 'Finished copy to d3. Starting copy to hexfarm:'
os.popen('scp '+ToSRM+'*.root gomez@hexcms.rutgers.edu:'+ToHexfarm)
print 'Waiting...'
os.popen('rm *root')
print 'Finish'

