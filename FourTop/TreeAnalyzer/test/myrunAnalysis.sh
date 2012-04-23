##!/bin/tcsh -f
#
#date
#source /uscmst1/prod/sw/cms/setup/cshrc prod
#setenv SCRAM_ARCH slc5_amd64_gcc434
#
#set MAIN_Dir =/uscms/home/algomez/work/CMSSW_4_2_4/src/Yumiceva/TreeAnalyzer/test/
#
#cd $MAIN_Dir
#
#eval `scram runtime -csh`
#
#echo "SCRAM_ARCH " $SCRAM_ARCH
#pwd
#
root -l loadLibraries.C -b runAnalysis.C\(\"tttt\",\"all\"\) -q
root -l loadLibraries.C -b runAnalysis.C\(\"MC\",\"all\"\) -q
root -l loadLibraries.C -b runAnalysis.C\(\"data\",\"all\"\) -q
root -l loadLibraries.C -b runAnalysis.C\(\"tttt\",\"sys\",\"JEC\"\) -q
root -l loadLibraries.C -b runAnalysis.C\(\"tttt\",\"sys\",\"JER\"\) -q
root -l loadLibraries.C -b runAnalysis.C\(\"tttt\",\"sys\",\"PU\"\) -q
root -l loadLibraries.C -b runAnalysis.C\(\"tttt\",\"sys\",\"BTAG\"\) -q
root -l loadLibraries.C -b runAnalysis.C\(\"MC\",\"sys\",\"JEC\"\) -q
root -l loadLibraries.C -b runAnalysis.C\(\"MC\",\"sys\",\"JER\"\) -q
root -l loadLibraries.C -b runAnalysis.C\(\"MC\",\"sys\",\"PU\"\) -q
root -l loadLibraries.C -b runAnalysis.C\(\"MC\",\"sys\",\"BTAG\"\) -q
#echo Done.
#date
#end
##
