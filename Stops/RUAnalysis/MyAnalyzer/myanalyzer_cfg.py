import FWCore.ParameterSet.Config as cms
import glob,sys

st1mass = int (sys.argv[2])
st2mass = int (sys.argv[3])
decay = sys.argv[4]
numEvents = int (sys.argv[5])
print sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5]

suffix_list = glob.glob('/cms/gomez/Stops/PATTuples/st2_h_bb_st1_'+decay+'_'+str(st2mass)+'_'+str(st1mass)+'/*root')
this_fin_nocfi = [i if i.startswith('file:') else 'file:' + i for i in suffix_list]

# this_fout0 = "/cms/clseitz/ThreeJet/RUNtuple/Nov6ReRun_JES/Stealth/StealthSbottomTo3b_MSb_"+str(mass)+"/tealthSbottomTo3b_MSb_"+str(mass)+"_FullSimSummer12_TLBSM53xv5_"+str(newInd)+"_JESnone_plots.root"
# this_fout1 = "/cms/clseitz/ThreeJet/RUNtuple/Nov6ReRun_JES/Stealth/StealthSbottomTo3b_MSb_"+str(mass)+"/tealthSbottomTo3b_MSb_"+str(mass)+"_FullSimSummer12_TLBSM53xv5_"+str(newInd)+"_JESnone_tree.root"
#this_fout0 = "ntuples/ntpl_"+str(index)+"_Allplots.root"
#this_fout1 = "ntuples/ntpl_"+str(index)+"_Alltree.root"

outputs = "/cms/gomez/Stops/Results/st2_h_bb_st1_"+decay+"_"+str(st2mass)+"_"+str(st1mass)+"_plots.root"
#this_fout1 = "/cms/gomez/Stops/Ntuples/st2_h_bb_st1_"+process+"_"+str(st2mass)+"_"+str(st1mass)+"/st2_h_bb_st1_"+process+"_"+str(st2mass)+"_"+str(st1mass)+"_tree.root"


print this_fin_nocfi
print outputs
#print this_fout1

process = cms.Process("plots")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = cms.string( 'START53_V15::All' )
process.GlobalTag.globaltag = cms.string( 'START52_V10::All' )

# START53_V7E is the old tag used by TLBSM53x
# process.GlobalTag.globaltag = cms.string( 'START53_V7E::All' )
# For data use an AN3 GT like the following
# process.GlobalTag.globaltag = cms.string( 'FT_53_V10_AN3::All' )

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 2000

# JetCorrectionService ak5PFLchs1FastL2L3 for MC
# JetCorrectionService ak5PFchsL1FastL2L3Residual  for data
# jetcorrserv = 'ak5PFchsL1FastL2L3Residual'
jetcorrserv = 'ak5PFchsL1FastL2L3'

## load jet corrections
##### process.load("JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff")
process.load("RUAnalysis.MyAnalyzer.ak5PFchsJetCorrectionServices_cff")
process.prefer(jetcorrserv)

if numEvents == 0:
	process.maxEvents = cms.untracked.PSet(
		input = cms.untracked.int32(-1)
		)
else:
	process.maxEvents = cms.untracked.PSet(
		input = cms.untracked.int32(numEvents)
		)

process.source = cms.Source("PoolSource",
		fileNames = cms.untracked.vstring(this_fin_nocfi),
		duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
		)

#############################
###### Output module  #######
#############################
process.TFileService = cms.Service("TFileService",
		fileName = cms.string (outputs)
)


#############################
###### ANALYZER #############
#############################

process.plots = cms.EDAnalyzer('MyAnalyzer',
		stop1Mass = cms.double(st1mass),
		stop2Mass = cms.double(st2mass),
		st1decay = cms.untracked.string(decay),
		Gensrc = cms.InputTag("genParticles"),
#		sumPtMin       = cms.untracked.double(0.0),
#		debug          = cms.untracked.bool(False),
#		NtuplePlots = cms.untracked.string(this_fout0),
#		NtupleTree= cms.untracked.string(this_fout1),
#		#PatJetType     = cms.untracked.vstring('goodPatJetsCA8PF'),
		PatJetType     = cms.untracked.string('goodPatJetsPFlow'),
#		#PatJetType     = cms.untracked.vstring('goodPatJetsCA8PF'),
#		PatJetType = cms.untracked.vstring('goodPatJetsPFlow','goodPatJetsCA8PF','goodPatJetsCA8PrunedPF'),
#		PrimaryVertex = cms.untracked.string('goodOfflinePrimaryVertices'),
#		METtype = cms.untracked.string('patMETsPFlow'),
#		htTrigger      = cms.untracked.string('HLT_HT360_v2'),
#		IsoMu12_v1     = cms.untracked.string('HLT_IsoMu12_v1'),
#		IsoMu17_v6     = cms.untracked.string('HLT_IsoMu17_v6'),
#		eeta           = cms.untracked.double(2.1),
#		ept            = cms.untracked.double(20.0),
#		meta           =  cms.untracked.double(2.1),
#		mpt            = cms.untracked.double(20.0),
#		isData         =  cms.untracked.bool(False),
#		NjetsMin       = cms.untracked.int32(0),
#		NjetsMax       = cms.untracked.int32(1000),
#		jetptcut       = cms.untracked.double(20),
#		etacut         = cms.untracked.double(3.),
#		jecAdj = cms.untracked.string('none'), # Can also be "up" or "down
		jetCorrectionService = cms.untracked.string(jetcorrserv),
#
#		#selection trigger
#		#Qua60_Di20
#		#TriggerNamesSel = cms.untracked.vstring('HLT_QuadJet60_DiJet20_v1','HLT_QuadJet60_DiJet20_v2','HLT_QuadJet60_DiJet20_v3','HLT_QuadJet60_DiJet20_v4',
#		#'HLT_QuadJet60_DiJet20_v5','HLT_QuadJet60_DiJet20_v6','HLT_QuadJet60_DiJet20_v7'),
#		#Quad70
#		#TriggerNamesSel = cms.untracked.vstring('HLT_QuadJet80_v1','HLT_QuadJet80_v2','HLT_QuadJet80_v3','HLT_QuadJet80_v4',
#		#'HLT_QuadJet80_v5','HLT_QuadJet80_v6','HLT_QuadJet80_v7'),
#		TriggerNamesSel = cms.untracked.vstring('HLT_SixJet45_v1','HLT_SixJet45_v2','HLT_SixJet45_v3','HLT_SixJet45_v4'),
#		
#		#base trigger
#		#TriggerNamesBase = cms.untracked.vstring('HLT_QuadJet50_v9','HLT_QuadJet50_v8'),
#		#,'HLT_QuadJet50_v3','HLT_QuadJet50_v4',
#		#'HLT_QuadJet50_v5','HLT_QuadJet50_v6','HLT_QuadJet50_v7'),
#		#TriggerNamesBase = cms.untracked.vstring('HLT_QuadJet70_v1','HLT_QuadJet70_v2','HLT_QuadJet70_v3','HLT_QuadJet70_v4',
#		#'HLT_QuadJet70_v5','HLT_QuadJet70_v6','HLT_QuadJet70_v7'),
#		TriggerNamesBase = cms.untracked.vstring('HLT_SixJet35_v1','HLT_SixJet35_v2','HLT_SixJet35_v3','HLT_SixJet35_v4'),
#		#JSON file 900/pb May18
#		#JSONFilename = cms.untracked.string('/home/clseitz/MyCMS/RU/CMSSW_5_2_2/src/RUAnalysis/MyAnalyzer/submit/Cert_190456-194076_8TeV_PromptReco_Collisions12_JSON.txt'),
#		#JSON File 2.42/fb Jun01
#		JSONFilename = cms.untracked.string('/home/clseitz/MyCMS/RU/CMSSW_5_2_2/src/RUAnalysis/Ntupler/submit/Cert_190456-195016_8TeV_PromptReco_Collisions12_JSON.txt'),
		)


#process.p = cms.Path(process.myPartons * process.AK5Flavour *process.data2)
process.p = cms.Path(process.plots)
