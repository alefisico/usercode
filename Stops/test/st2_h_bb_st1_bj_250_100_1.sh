#!/bin/tcsh -f

date
source /uscmst1/prod/sw/cms/setup/cshrc prod
setenv SCRAM_ARCH slc5_amd64_gcc462

set MAIN_Dir = /uscms/home/algomez/work/CMSSW_5_3_3/src/TopQuarkAnalysis/TopPairBSM/test/Stops/st2_h_bb_st1_bj_250_100/

cd $MAIN_Dir

eval `scram runtime -csh`

echo "SCRAM_ARCH " $SCRAM_ARCH
pwd

cmsRun st2_h_bb_st1_bj_250_100_junk_1.py 

date
end

#echo \'$WZ_File\'
