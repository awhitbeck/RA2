# Test TreeMaker on AOD i.e. run SUSY-PAT first and produce
# RA2 specific objects, and produce trees for nominal and varied JES.
#

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

runningOnMC = False

#-- Message Logger ------------------------------------------------------------
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(
    limit = cms.untracked.int32(100),
    reportEvery = cms.untracked.int32(1)
    )


#-- Input Source --------------------------------------------------------------
process.maxEvents.input = 100

############################# START SUSYPAT specifics ####################################
from PhysicsTools.Configuration.SUSY_pattuple_cff import addDefaultSUSYPAT, getSUSY_pattuple_outputCommands

hltMenu = 'HLT'

theJetColls = ['AK5PF']

jetMetCorr = ['L1FastJet', 'L2Relative', 'L3Absolute']
if runningOnMC == False: jetMetCorr.append('L2L3Residual')  

process.GlobalTag.globaltag = "START52_V11C::All"
if runningOnMC == False:
    process.GlobalTag.globaltag = "GR_R_52_V9D::All"

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
    '/store/data/Run2012B/HTMHT/AOD/PromptReco-v1/000/197/044/56E6003A-00BE-E111-9AF5-0019B9F72F97.root'    
    )
                            )

# Due to problem in production of LM samples: same event number appears multiple times
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

addDefaultSUSYPAT(process,mcInfo=runningOnMC,HLTMenu=hltMenu,jetMetCorrections=jetMetCorr,mcVersion='',theJetNames=theJetColls, doSusyTopProjection=False)

#-- Adjust collections to use PFNoPU jets -------------------------------------
    
# do not use Z-mass window for PU subtraction
# such that JEC works properly
process.pfPileUpPF.checkClosestZVertex = cms.bool(False)

# do not remove muons and electrons from the jet clustering input
# (patMuons and patElectrons use relaxed pfMuon and pfElectron collections)
process.pfIsolatedElectrons.isolationCut = -1
process.pfIsolatedMuons.isolationCut = -1

# do not remove taus from the jet collection
process.pfTausPF.discriminators = cms.VPSet()
process.pfUnclusteredTausPF = process.pfTausPF.clone(
    cut = cms.string("pt < 0")
    )
process.pfTauSequencePF.replace(process.pfTausPF, process.pfTausPF+ process.pfUnclusteredTausPF)
process.pfNoTauPF.topCollection = "pfUnclusteredTausPF"

############################## END SUSYPAT specifics ####################################

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("RA2TestTree.root")
                                   )

process.dump = cms.EDAnalyzer("EventContentAnalyzer")

## --- Vary JES and recompute HT, MHT accordingly ----------------------

# Use PFchs jets and vary JES by +/-1 sigma
from RA2Classic.Utils.jesUncertaintyVariation_cfi import jesUncertaintyVariation
process.patJetsPFJESUp = jesUncertaintyVariation.clone(
    Jets       = cms.InputTag('patJetsPF'),
    JetTypeId  = cms.string('AK5PFchs'),
    Variation  = cms.string('UP')
    )
process.patJetsPFJESDn = process.patJetsPFJESUp.clone(
    Variation  = cms.string('DN')
    )

# Jets with varied JES to compute HT and MHT
process.load('SandBox.Skims.RA2Objects_cff')
process.patJetsPFchsPt30JESUp = process.patJetsPFchsPt30.clone(
    src = cms.InputTag('patJetsPFJESUp')
    )
process.patJetsPFchsPt50Eta25JESUp = process.patJetsPFchsPt50Eta25.clone(
    src = cms.InputTag('patJetsPFchsPt30JESUp')
    )
process.patJetsPFchsPt30JESDn = process.patJetsPFchsPt30.clone(
    src = cms.InputTag('patJetsPFJESDn')
    )
process.patJetsPFchsPt50Eta25JESDn = process.patJetsPFchsPt50Eta25.clone(
    src = cms.InputTag('patJetsPFchsPt30JESDn')
    )

# HT and MHT from varied JES
from SandBox.Skims.htProducer_cfi import ht
process.htPFchsJESUp = ht.clone(
    JetCollection = cms.InputTag('patJetsPFchsPt50Eta25JESUp')
    )
process.htPFchsJESDn = ht.clone(
    JetCollection = cms.InputTag('patJetsPFchsPt50Eta25JESDn')
    )
from SandBox.Skims.mhtProducer_cfi import mht
process.mhtPFchsJESUp = mht.clone(
    JetCollection = cms.InputTag('patJetsPFchsPt30JESUp')
    )
process.mhtPFchsJESDn = mht.clone(
    JetCollection = cms.InputTag('patJetsPFchsPt30JESDn')
    )

process.VaryJESUp = cms.Sequence(
    process.patJetsPFJESUp *
    process.patJetsPFchsPt30JESUp *
    process.patJetsPFchsPt50Eta25JESUp *
    process.htPFchsJESUp *
    process.mhtPFchsJESUp
    )
process.VaryJESDn = cms.Sequence(
    process.patJetsPFJESDn *
    process.patJetsPFchsPt30JESDn *
    process.patJetsPFchsPt50Eta25JESDn *
    process.htPFchsJESDn *
    process.mhtPFchsJESDn
    )



## --- Setup of TreeMaker ----------------------------------------------
from RA2Classic.TreeMaker.treemaker_cfi import TreeMaker
process.RA2TreeMaker = TreeMaker.clone(
    TreeName          = cms.string("RA2PreSelection"),
    VertexCollection  = cms.InputTag('goodVertices'),
    HT                = cms.InputTag('htPFchs'),
    HTJets            = cms.InputTag('patJetsPFchsPt50Eta25'),
    MHT               = cms.InputTag('mhtPFchs'),
    MHTJets           = cms.InputTag('patJetsPFchsPt30'),
    )
process.RA2TreeMakerJESUp = process.RA2TreeMaker.clone(
    HT                = cms.InputTag('htPFchsJESUp'),
    HTJets            = cms.InputTag('patJetsPFchsPt50Eta25JESUp'),
    MHT               = cms.InputTag('mhtPFchsJESUp'),
    MHTJets           = cms.InputTag('patJetsPFchsPt30JESUp'),
    )
process.RA2TreeMakerJESDn = process.RA2TreeMaker.clone(
    HT                = cms.InputTag('htPFchsJESDn'),
    HTJets            = cms.InputTag('patJetsPFchsPt50Eta25JESDn'),
    MHT               = cms.InputTag('mhtPFchsJESDn'),
    MHTJets           = cms.InputTag('patJetsPFchsPt30JESDn'),
    )


process.demo = cms.Sequence(
    process.susyPatDefaultSequence *
    process.ra2Objects *
    process.VaryJESUp *
    process.VaryJESDn *
#    process.dump *
    process.RA2TreeMaker *
    process.RA2TreeMakerJESUp *
    process.RA2TreeMakerJESDn
    )


process.p = cms.Path(process.demo)
