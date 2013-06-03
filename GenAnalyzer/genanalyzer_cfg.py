# Import configurations
import FWCore.ParameterSet.Config as cms
process = cms.Process("Demo")

###############################
####### Parameters ############
###############################
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('standard')			### For a simple set of parameters
#options = VarParsing ('analysis')			### For a complete set of parameters

options.register ('stop1mass',
                  0,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.int,
                  'Mass for the Stop1')

options.register ('stop2mass',
                  0,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.int,
                  'Mass for the Stop2')

#options.register ('useData',
#                  False,
#                  VarParsing.multiplicity.singleton,
#                  VarParsing.varType.int,
#                  'Run this on real data')
options.parseArguments()
print options

###############################
########## Gen Setup ##########
###############################

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
## Report every ..
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

## Source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#	    'file:/eos/uscms/store/user/algomez/Stops/PATTuples/st2_h_bb_st1_bj_250_100/st2_h_bb_st1_bj_250_100_tlbsm_53x_v2_mcst2_h_bb_st1_bj_250_100_1_tlbsm_53x_v2_mc.root',
#	    'file:/eos/uscms/store/user/algomez/Stops/PATTuples/st2_h_bb_st1_bj_250_100/st2_h_bb_st1_bj_250_100_tlbsm_53x_v2_mcst2_h_bb_st1_bj_250_100_2_tlbsm_53x_v2_mc.root',
#	    'file:/eos/uscms/store/user/algomez/Stops/PATTuples/st2_h_bb_st1_bj_250_100/st2_h_bb_st1_bj_250_100_tlbsm_53x_v2_mcst2_h_bb_st1_bj_250_100_3_tlbsm_53x_v2_mc.root',
#	    'file:/eos/uscms/store/user/algomez/Stops/PATTuples/st2_h_bb_st1_bj_250_100/st2_h_bb_st1_bj_250_100_tlbsm_53x_v2_mcst2_h_bb_st1_bj_250_100_4_tlbsm_53x_v2_mc.root'
 'file:/eos/uscms/store/user/algomez/Stops/PATTuples/st2_h_bb_st1_jj_250_100/st2_h_bb_st1_jj_250_100_tlbsm_53x_v2_mc_2.root',
 'file:/eos/uscms/store/user/algomez/Stops/PATTuples/st2_h_bb_st1_jj_250_100/st2_h_bb_st1_jj_250_100_tlbsm_53x_v2_mc_3.root',
 'file:/eos/uscms/store/user/algomez/Stops/PATTuples/st2_h_bb_st1_jj_250_100/st2_h_bb_st1_jj_250_100_tlbsm_53x_v2_mc_4.root',
 'file:/eos/uscms/store/user/algomez/Stops/PATTuples/st2_h_bb_st1_jj_250_100/st2_h_bb_st1_jj_250_100_tlbsm_53x_v2_mc.root'

    )
)


###############################
########## Gen Setup ##########
###############################

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")


## Print Tree
process.printGenParticle = cms.EDAnalyzer("ParticleListDrawer",
		src = cms.InputTag("prunedGenParticles"),
		maxEventsToPrint = cms.untracked.int32(1)
		)


## Some selections
process.genPartonClone = cms.EDFilter("CandViewShallowCloneProducer",
		src = cms.InputTag("prunedGenParticles"),
		cut = cms.string("(abs(pdgId) = 1 | abs(pdgId) = 2 | abs(pdgId) = 3 | abs(pdgId)=4) & status=3")
		)


process.genBClone = cms.EDFilter("CandViewShallowCloneProducer",
		src = cms.InputTag("prunedGenParticles"),
		cut = cms.string("abs(pdgId) = 5 & status=3")
		)

process.genPartonPlusBClone = cms.EDFilter("CandViewShallowCloneProducer",
		src = cms.InputTag("prunedGenParticles"),
		cut = cms.string("(abs(pdgId) = 1 | abs(pdgId) = 2 | abs(pdgId) = 3 | abs(pdgId)=4 | abs(pdgId)=5) & status=3")
		)

## Simple Histos
process.partonHistos= cms.EDAnalyzer("CandViewHistoAnalyzer",
		src = cms.InputTag("genPartonClone"),
		histograms = cms.VPSet(
			cms.PSet(
				min = cms.untracked.double(0.0),
				max = cms.untracked.double(300.0),
				nbins = cms.untracked.int32(30),
				name = cms.untracked.string("jets_Pt"),
				description = cms.untracked.string("jets pt [GeV/c^{2}]"),
				plotquantity = cms.untracked.string("pt")
				)
			)
		)

process.bHistos= cms.EDAnalyzer("CandViewHistoAnalyzer",
		src = cms.InputTag("genBClone"),
		histograms = cms.VPSet(
			cms.PSet(
				min = cms.untracked.double(0.0),
				max = cms.untracked.double(300.0),
				nbins = cms.untracked.int32(30),
				name = cms.untracked.string("b_Pt"),
				description = cms.untracked.string("b pt [GeV/c^{2}]"),
				plotquantity = cms.untracked.string("pt")
				)
			)
		)

process.partonPlusBHistos= cms.EDAnalyzer("CandViewHistoAnalyzer",
		src = cms.InputTag("genPartonPlusBClone"),
		histograms = cms.VPSet(
			cms.PSet(
				min = cms.untracked.double(0.0),
				max = cms.untracked.double(300.0),
				nbins = cms.untracked.int32(30),
				name = cms.untracked.string("jets_b_Pt"),
				description = cms.untracked.string("jets + b pt [GeV/c^{2}]"),
				plotquantity = cms.untracked.string("pt")
				)
			)
		)

############################
###### My Analyzer  ########
############################
process.genHistos = cms.EDAnalyzer('GenAnalyzer',
		src = cms.InputTag("prunedGenParticles"),
		stop1Mass = cms.double(options.stop1mass),
		stop2Mass = cms.double(options.stop2mass)
)

#############################
###### Output module  #######
#############################
process.TFileService = cms.Service("TFileService",
		fileName = cms.string (options.output)
)


#############################
########## Run ##############
#############################
process.p = cms.Path(process.printGenParticle*process.genHistos)
#process.p = cms.Path(process.genPartonClone*process.partonHistos*process.genBClone*process.bHistos*process.genPartonPlusBClone*process.partonPlusBHistos*process.genHistos)

#process.outpath = cms.EndPath(process.out)
