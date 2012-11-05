#!/bin/tcsh -f

date
source /uscmst1/prod/sw/cms/setup/cshrc prod
setenv SCRAM_ARCH slc5_amd64_gcc434

set MAIN_Dir = /uscms/home/algomez/work/CMSSW_4_2_4/src/theta/utils/examples/fourtop//results110_Nbjets3_v2//

cd $MAIN_Dir

eval `scram runtime -csh`
echo "SCRAM_ARCH " $SCRAM_ARCH
pwd

../../../theta-auto.py analysis_v2_tttt_Gh700.py 
