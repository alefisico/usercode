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

#ToSRM      = 'srm://ruhex-osgce.rutgers.edu:8443/srm/v2/server?SFN=/cms/data24/algomez/'
ToSRM      = 'file:///uscms_data/d3/algomez/files/RPV_M300_112_ISRFSRdown_1/'

#define which dataset to copu
#dataset = 'MultiJet/'
#dataset = 'SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/ttbsm_v9_Summer11-PU_S4_START42_V11-v1/'
dataset = 'RPV_M300_112_ISRFSRdown_1/'


#run = 'Run2012A-PromptReco-v1_Json0601'
#run = 'Run2012B-PromptReco-v1_Json0601'
#run = '2f45e7f986681f9c0bbcc7cc31425dac'
#####################################
##            Main Code            ##
#####################################
#make list of all the files in a directory
#print 'srmls "'+FromSRM+dataset+run+'"'
print 'srmls "'+FromSRM+dataset+'"'
#outlist = os.popen('srmls "'+FromSRM+dataset+run+'"').readlines()
outlist = os.popen('srmls "'+FromSRM+dataset+'"').readlines()
#loop through all these files
count=0
for myfile in outlist:
    #find the position in the string for a certain dataset
    startD = int(string.find(myfile,dataset))
    endD = startD+int(len(dataset))
    #runD = endD+int(len(run))
    print startD, endD, myfile[startD:endD]#, myfile[endD:runD]
    if startD > 0:
        if count < 550:
            print count
            FileName = myfile[startD:-1]
            #OutName = myfile[runD+1:-1]
            OutName = myfile[endD+1:-1]
            print FileName 
            #command to acutally copy stuff
            cmd = 'lcg-cp -b -D srmv2 "' + FromSRM + FileName + '" "' + ToSRM + OutName + '"'
            print 'Performing: '+cmd
            #execute the command, uncomment line below
            os.popen(cmd)
        count=count+1
#print count
#Performing: lcg-cp -b -D srmv2 "srm://cmssrm.fnal.gov:8443/srm/managerv2?SFN=/11/store/user/algomez/RPV_M300_112_ISRFSRdown_1//RPVgluino_M_300_8TeV_pythia6D6THC_112_cff_less_ISRFSR_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_RAW2DIGI_L1Reco_RECO_PU_149_1_xFz.root" "srm://ruhex-osgce.rutgers.edu:8443/srm/v2/server?SFN=/cms/data24/algomez/RPVgluino_M_300_8TeV_pythia6D6THC_112_cff_less_ISRFSR_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_RAW2DIGI_L1Reco_RECO_PU_149_1_xFz.root"
#38 64 RPV_M300_112_ISRFSRdown_1/
#371
#RPV_M300_112_ISRFSRdown_1//RPVgluino_M_300_8TeV_pythia6D6THC_112_cff_less_ISRFSR_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_RAW2DIGI_L1Reco_RECO_PU_300_1_ZA8.root


