# Import configurations
import FWCore.ParameterSet.Config as cms
process = cms.Process("Demo")

###############################
####### Parameters ############
###############################
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('standard')			### For a simple set of parameters
#options = VarParsing ('analysis')			### For a complete set of parameters

options.register ('st1',
                  0,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.int,
                  'Mass for the Stop1')

options.register ('st2',
                  0,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.int,
                  'Mass for the Stop2')

options.register ('use',
                  '',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.string,
                  'Stop1 decay in')
options.parseArguments()
print options

###############################
########## Gen Setup ##########
###############################

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
## Report every ..
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(2000)

## Source
stop2 = str(options.st2)			# Convert it into string only for inputfile names
stop1 = str(options.st1)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
	    ##### For normal process
	    'file:/eos/uscms/store/user/algomez/Stops/PATTuples/st2_h_bb_st1_'+options.use+'_'+stop2+'_'+stop1+'/st2_h_bb_st1_'+options.use+'_'+stop2+'_'+stop1+'_tlbsm_53x_v2_mc_1.root',
	    'file:/eos/uscms/store/user/algomez/Stops/PATTuples/st2_h_bb_st1_'+options.use+'_'+stop2+'_'+stop1+'/st2_h_bb_st1_'+options.use+'_'+stop2+'_'+stop1+'_tlbsm_53x_v2_mc_2.root',
	    'file:/eos/uscms/store/user/algomez/Stops/PATTuples/st2_h_bb_st1_'+options.use+'_'+stop2+'_'+stop1+'/st2_h_bb_st1_'+options.use+'_'+stop2+'_'+stop1+'_tlbsm_53x_v2_mc_3.root',
	    'file:/eos/uscms/store/user/algomez/Stops/PATTuples/st2_h_bb_st1_'+options.use+'_'+stop2+'_'+stop1+'/st2_h_bb_st1_'+options.use+'_'+stop2+'_'+stop1+'_tlbsm_53x_v2_mc_4.root'

    )
)

## Output Dir
output_Dir='/uscms_data/d3/algomez/files/stops/GenPlots/st2_h_bb_st1_'+options.use+'_'+stop2+'_'+stop1+'/'


###############################
########## Gen Setup ##########
###############################

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")


############################# The following EDAnalyzer are usefull for simple analysis,
############################# but I am not using them for the current GenAnalysis.
## Print Tree
process.printGenParticle = cms.EDAnalyzer("ParticleListDrawer",
		#src = cms.InputTag("prunedGenParticles"),
		src = cms.InputTag("genParticles"),
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

dummyprocess = -999
if options.use=='bj': dummyprocess = 1
else: dummyprocess = 0
############################
###### My Analyzer  ########
############################
process.genHistos = cms.EDAnalyzer('GenAnalyzer',
		#src = cms.InputTag("prunedGenParticles"),
		src = cms.InputTag("genParticles"),
		stop1Mass = cms.double(options.st1),
		stop2Mass = cms.double(options.st2),
		st1decay = cms.double(dummyprocess)
)

#############################
###### Output module  #######
#############################
process.TFileService = cms.Service("TFileService",
		fileName = cms.string (output_Dir+options.output)
)


#############################
########## Run ##############
#############################
process.p = cms.Path(process.printGenParticle*process.genHistos)
#process.p = cms.Path(process.genPartonClone*process.partonHistos*process.genBClone*process.bHistos*process.genPartonPlusBClone*process.partonPlusBHistos*process.genHistos)

#process.outpath = cms.EndPath(process.out)
