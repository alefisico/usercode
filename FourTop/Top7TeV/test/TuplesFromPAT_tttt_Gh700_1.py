import string
import os

####### Parameters ############
###############################
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')

options.register ('useData',
                  False,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.int,
                  "Run this on real data")

options.register ('channel',
                  'muon',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  "channel muon or electron")

options.register ('events',
                  -1,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.int,
                  "number of events to process")

options.register ('verbose',
                  False,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.int,
                  "verbose")

options.parseArguments()

inputType = "DATA"
if not options.useData :
    inputType = "MC"

events= options.events
channel = options.channel 
eventtype = "tttt_Gh700_100k"

fileaddix = "_Mu.root"
if channel == "electron":
    fileaddix = "_El.root"
       
#jsonfile = "json/Cert_160404-163369_7TeV_PromptReco_Collisions11_JSON_MuonPhys.txt"
jsonfile = ''

out = "%s-*-PATskim.root"%eventtype
outfile = string.replace(out,"*","PAT")
outntuple = string.replace(out,"*","Tuple")

import FWCore.ParameterSet.Config as cms
process = cms.Process("ttttGh700")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff')
process.MessageLogger.suppressWarning.append('patTrigger')
process.MessageLogger.cerr.FwkJob.limit=1
process.MessageLogger.cerr.ERROR = cms.untracked.PSet( limit = cms.untracked.int32(0) )

## Ntuple maker
process.load("Yumiceva.Top7TeV.PATNtupleMaker_cfi")
process.PATNtupleMaker.inputType = inputType
process.PATNtupleMaker.ntupleFile = outntuple

process.PATNtupleMaker.hltList = 'HLT_IsoMu24_v1','HLT_IsoMu24_v1'
#process.PATNtupleMaker.hltList = 'HLT_Mu24_v1','HLT_Mu24_v2','HLT_Mu24_v3','HLT_Mu24_v4','HLT_Mu24_v7'
if channel=="electron":
    #process.PATNtupleMaker.hltList = 'HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT','HLT_Ele32_CaloIdL_CaloIsoVL_SC17'
    process.PATNtupleMaker.hltList = 'HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2','HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1'
process.PATNtupleMaker.hltTag = cms.InputTag("patTriggerEvent")
process.PATNtupleMaker.MuonTag = 'selectedPatMuonsPFlow'
process.PATNtupleMaker.ElectronTag = 'selectedPatElectrons'
process.PATNtupleMaker.PFJetTag = 'goodPatJetsPFlow'
process.PATNtupleMaker.PFMETTag = 'patMETsPFlow'
process.PATNtupleMaker.PVTag = 'goodOfflinePrimaryVertices'
if options.verbose:
    process.PATNtupleMaker.Verbose = True

## Input PAT files for testing
if inputType=="DATA":
    process.source = cms.Source("PoolSource",
                                fileNames = cms.untracked.vstring(
	 'file:/uscms_data/d3/algomez/files/fourtop/Ntuples/4tbsm_700_42x_data.root'				
        ))
    process.GlobalTag.globaltag = 'GR_R_42_V12::All'
if inputType=="MC":
    process.source = cms.Source("PoolSource",
                                fileNames = cms.untracked.vstring(
	 'file:/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh700_50k_1_42x_mc.root',
	 'file:/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh700_50k_2_42x_mc.root',
	 'file:/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh700_50k_3_42x_mc.root',
	 'file:/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh700_50k_4_42x_mc.root'
	 'file:/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh700_20k_42x_mc.root',
	 'file:/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh700_30k_1_42x_mc.root',
	  'file:/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh700_30k_2_42x_mc.root',
	 'file:/eos/uscms/store/user/algomez/FourTop/Ntuples/tttt_Gh700_30k_3_42x_mc.root'
        ))
    process.GlobalTag.globaltag = 'START42_V12::All'
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( events ))

## JSON Filter
import PhysicsTools.PythonAnalysis.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes
if os.path.exists(jsonfile) and inputType=="DATA":
    print "Using JSON file "+jsonfile
    myLumis = LumiList.LumiList(filename = jsonfile).getCMSSWString().split(',')
    process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    process.source.lumisToProcess.extend(myLumis)

    
process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(outntuple)
)


### GEN decay channel, for tT MC events only ###
process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")
process.load("PhysicsTools.HepMCCandAlgos.flavorHistoryPaths_cfi")
process.cFlavorHistoryProducer.matchedSrc = "ak5GenJets"
process.bFlavorHistoryProducer.matchedSrc = "ak5GenJets"

process.load('PhysicsTools.PatAlgos.triggerLayer1.triggerEventProducer_cfi')


### The process path ###
process.p = cms.Path(

    process.patTriggerDefaultSequence * process.PATNtupleMaker

)
