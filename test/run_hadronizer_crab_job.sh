#!/bin/tcsh -f
####################################################
###
###  Simple tcsh script to send many Hadronization
###  and FASTSim jobs to crab.
###
###  Alejandro Gomez Espinosa
###  gomez@physics.rutgers.edu
###
###  How to run: 
###  source run_hadronizer_crab_job.sh 100 250
###  100 is the mass of the daugher particle and 250 of the mother
###  (If it is an executable (chmod +x run_hadronizer_crab_job.sh):
###  ./run_hadronizer_crab_job.sh 100 250
###
####################################################


######################################
### PARAMETERS
#####################################

foreach Process ("bj" "jj")							# Here I have two different final states
set stop1 = $1									# Mass of the stop1	
set stop2 = $2									# Mass of the stop2

set Main_Dir = /uscms/home/algomez/work/CMSSW_5_2_6/src/			# Main Dir
set Name = st2_h_bb_st1_${Process}_${stop2}_${stop1}				# Name of the process
set Sample_Dir = ${Main_Dir}/${Name}_AOD/					# Name of directory for each process
set LHE_Dir = /uscms_data/d3/algomez/files/stops/lhe/				# Name of lhe directory


#####################################################
#### Here is where the code starts.. Do NOT change
#####################################################

######## Create Sample Dir
/bin/rm -rf $Sample_Dir
if ( -d $Sample_Dir ) then
else
mkdir $Sample_Dir 
endif
cd $Sample_Dir

########################################################
######### Python script for Hadronization and FASTSim
######### This part is generic for most of the analysis
########################################################
/bin/rm -f Hadronizer_TuneZ2star_8TeV_generic_LHE_pythia_cff_py_GEN_FASTSIM_HLT_PU_${Name}_AOD.py
cat > Hadronizer_TuneZ2star_8TeV_generic_LHE_pythia_cff_py_GEN_FASTSIM_HLT_PU_${Name}_AOD.py << EOF
# Auto generated configuration file
# using: 
# Revision: 1.372.2.14 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/EightTeV/Hadronizer_TuneZ2star_8TeV_generic_LHE_pythia_cff.py -s GEN,FASTSIM,HLT:7E33v2 --beamspot Realistic8TeVCollision --pileup 2012_Startup_inTimeOnly --geometry DB --conditions START52_V10::All --eventcontent AODSIM --datatier AODSIM --filein file:/uscms_data/d3/algomez/files/stops/lhe/${Name}.lhe --fileout file:/uscms_data/d3/algomez/files/stops/AOD/${Name}_AOD.root --filetype LHE -n 10 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('HLT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('FastSimulation.Configuration.EventContent_cff')
process.load('FastSimulation.PileUpProducer.PileUpSimulator_2012_Startup_inTimeOnly_cff')
process.load('FastSimulation.Configuration.Geometries_START_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('FastSimulation.Configuration.FamosSequences_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedParameters_cfi')
process.load('HLTrigger.Configuration.HLT_7E33v2_Famos_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("LHESource",
    fileNames = cms.untracked.vstring('file:${LHE_Dir}${Name}.lhe')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('\$Revision: 1.1 \$'),
    annotation = cms.untracked.string('runs Z2* Pythia6'),
    name = cms.untracked.string('\$Source: /local/reps/CMSSW/CMSSW/Configuration/GenProduction/python/EightTeV/Hadronizer_TuneZ2star_8TeV_generic_LHE_pythia_cff.py,v \$')
)

# Output definition

process.AODSIMoutput = cms.OutputModule("PoolOutputModule",
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    outputCommands = process.AODSIMEventContent.outputCommands,
    fileName = cms.untracked.string('file:${Name}_AOD.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('AODSIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
process.simulation = cms.Sequence(process.simulationWithFamos)
process.HLTEndSequence = cms.Sequence(process.reconstructionWithFamos)
process.Realistic8TeVCollisionVtxSmearingParameters.type = cms.string("BetaFunc")
process.famosSimHits.VertexGenerator = process.Realistic8TeVCollisionVtxSmearingParameters
process.famosPileUp.VertexGenerator = process.Realistic8TeVCollisionVtxSmearingParameters
# customise the HLT menu for running on MC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforMC
process = customizeHLTforMC(process)

process.GlobalTag.globaltag = 'START52_V10::All'

process.generator = cms.EDFilter("Pythia6HadronizerFilter",
    pythiaHepMCVerbosity = cms.untracked.bool(True),
    maxEventsToPrint = cms.untracked.int32(0),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    comEnergy = cms.double(8000.0),
    PythiaParameters = cms.PSet(
        pythiaUESettings = cms.vstring('MSTU(21)=1     ! Check on possible errors during program execution', 
            'MSTJ(22)=2     ! Decay those unstable particles', 
            'PARJ(71)=10 .  ! for which ctau  10 mm', 
            'MSTP(33)=0     ! no K factors in hard cross sections', 
            'MSTP(2)=1      ! which order running alphaS', 
            'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)', 
            'MSTP(52)=2     ! work with LHAPDF', 
            'PARP(82)=1.921 ! pt cutoff for multiparton interactions', 
            'PARP(89)=1800. ! sqrts for which PARP82 is set', 
            'PARP(90)=0.227 ! Multiple interactions: rescaling power', 
            'MSTP(95)=6     ! CR (color reconnection parameters)', 
            'PARP(77)=1.016 ! CR', 
            'PARP(78)=0.538 ! CR', 
            'PARP(80)=0.1   ! Prob. colored parton from BBR', 
            'PARP(83)=0.356 ! Multiple interactions: matter distribution parameter', 
            'PARP(84)=0.651 ! Multiple interactions: matter distribution parameter', 
            'PARP(62)=1.025 ! ISR cutoff', 
            'MSTP(91)=1     ! Gaussian primordial kT', 
            'PARP(93)=10.0  ! primordial kT-max', 
            'MSTP(81)=21    ! multiple parton interactions 1 is Pythia default', 
            'MSTP(82)=4     ! Defines the multi-parton model'),
        processParameters = cms.vstring('MSEL=0          ! User defined processes', 
            'PMAS(5,1)=4.4   ! b quark mass', 
            'PMAS(6,1)=172.4 ! t quark mass'),
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters')
    )
)


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen_genonly)
process.reconstruction = cms.Path(process.reconstructionWithFamos)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.reconstruction,process.AODSIMoutput_step])
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

EOF

#########################################
########## Crab file
#########################################
/bin/rm -f crab_${Name}_AOD.cfg
cat > crab_${Name}_AOD.cfg << EOF
[CRAB]
jobtype 		= cmssw
scheduler 		= condor

[CMSSW]
datasetpath 		= None
generator 		= lhe
pset 			= Hadronizer_TuneZ2star_8TeV_generic_LHE_pythia_cff_py_GEN_FASTSIM_HLT_PU_${Name}_AOD.py
output_file 		= ${Name}_AOD.root
total_number_of_events 	= 100000
events_per_job 		= 5000
get_edm_output 		= 1

[USER]
return_data 		= 0
copy_data 		= 1
storage_element 	= cmssrm.fnal.gov
storage_path 		= /srm/managerv2?SFN=11/store/user/algomez/
user_remote_dir 	= ${Name}_AOD
dbs_url_for_publication = http://cmsdbsprod.cern.ch:8643/cms_dbs_ph_analysis_02_writer/servlet/DBSServlet
ui_working_dir 		= ${Name}_AOD
check_user_remote_dir 	= 0
eMail 			= alejandro.gomez@cern.ch
EOF

#################################
##### Make it run
#################################
crab -create -submit -cfg crab_${Name}_AOD.cfg

cd ${Main_Dir}

echo Done.
date
end

