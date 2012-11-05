#!/bin/tcsh -f

date
source /uscmst1/prod/sw/cms/setup/cshrc prod
setenv SCRAM_ARCH slc5_amd64_gcc434

set MAIN_Dir = /uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/Top7TeV/test//tttt_Gh700_50k/

cd $MAIN_Dir

eval `scram runtime -csh`

echo "SCRAM_ARCH " $SCRAM_ARCH
pwd
cmsRun TuplesFromPAT_tttt_Gh700_1.py >& TuplesFromPAT_tttt_Gh700_1.runlog

echo Done.
date
end

