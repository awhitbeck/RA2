## Example: cmsRun makeDASTree_cfg.py data_set=/store/data/Run2012D/HTMHT/AOD/PromptReco-v1/000/208/940/D0D3371D-A046-E211-B762-5404A6388697.root, is_mc=false, global_tag=FT_P_V42_AN3, hlt_path=HLT_PFNoPUHT350_PFMET100
## Example: cmsRun makeDASTree_cfg.py data_set=/store/mc/Summer12_DR53X/WJetsToLNu_HT-400ToInf_8TeV-madgraph/AODSIM/PU_S10_START53_V7A-v1/0000/024E6680-2DDB-E111-BDB9-002354EF3BD2.root is_mc=true, global_tag=START53_V7G

## --- GLOBAL PARAMETERS -----------------------------------------------------

# Read parameters
from RA2Classic.Utils.CommandLineParams import CommandLineParams
parameters = CommandLineParams()

dataSet_   = parameters.value("data_set","")
globalTag_ = parameters.value("global_tag","")+"::All"
isMC_      = parameters.value("is_mc",False)
isSUSY_    = parameters.value("is_susy",False)
hltPath_   = parameters.value("hlt_path","none")


print "***** SETUP ************************************"
print "    dataSet_ : "+dataSet_
print "    hltPath_ : "+hltPath_
print "  globalTag_ : "+globalTag_
print "       isMC_ : "+str(isMC_)
print "     isSUSY_ : "+str(isSUSY_)
print "************************************************"


jets_ = "patJetsPF"



## --- GENERAL SETUP ---------------------------------------------------------
from PhysicsTools.PatAlgos.patTemplate_cfg import *
process.outpath.remove(process.out)     # We only want the ROOT tree

process.GlobalTag.globaltag = globalTag_

#-- Message Logger -----------------------------------------------------------
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(
    limit = cms.untracked.int32(10),
    )
process.MessageLogger.cerr.FwkReport.reportEvery = 5000


#-- Input Source -------------------------------------------------------------
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(dataSet_)
    )
# Due to problem in production of LM samples: same event number appears multiple times
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
process.maxEvents.input = -100


## --- Output file -----------------------------------------------------
process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("DASRA2Tree.root")
    )


## --- SUSY PAT --------------------------------------------------------------
from PhysicsTools.Configuration.SUSY_pattuple_cff import addDefaultSUSYPAT, getSUSY_pattuple_outputCommands

if isMC_:
    hltMenu = 'REDIGI'
else:    
    hltMenu = 'HLT'
theJetColls = ['AK5PF']
jetMetCorr = ['L1FastJet', 'L2Relative', 'L3Absolute']
if not isMC_:
    jetMetCorr.append('L2L3Residual')  


addDefaultSUSYPAT(process,
                  mcInfo=isMC_,
                  HLTMenu=hltMenu,
                  jetMetCorrections=jetMetCorr,
                  mcVersion='',
                  theJetNames=theJetColls,
                  doSusyTopProjection=False)

# Remove the PAT cleaning and filtering sequences
process.patDefaultSequence.remove(process.selectedPatCandidates)
process.patDefaultSequence.remove(process.cleanPatCandidates)
process.patDefaultSequence.remove(process.countPatCandidates)
    
# Disable embedment so that lepton cleaning method works
process.patJetsAK5PF.embedCaloTowers = False
process.patJetsAK5PF.embedPFCandidates = False
process.patJetsPF.embedCaloTowers = False
process.patJetsPF.embedPFCandidates = False 

#-- Adjust collections to use PFNoPU jets -------------------------------------
    
# do not use Z-mass window for PU subtraction
# such that JEC works properly
process.pfPileUpPF.checkClosestZVertex = cms.bool(False)
    
# do not remove muons and electrons from the jet clustering input
process.pfIsolatedElectronsPF.isolationCut = -1.0
process.pfIsolatedMuonsPF.isolationCut = -1.0

# do not remove taus from the jet collection
process.pfTausPF.discriminators = cms.VPSet()
process.pfUnclusteredTausPF = process.pfTausPF.clone(
    cut = cms.string("pt < 0")
    )
process.pfTauSequencePF.replace(process.pfTausPF, process.pfTausPF+ process.pfUnclusteredTausPF)
process.pfNoTauPF.topCollection = "pfUnclusteredTausPF"

# make loose clones of the original electron collection
process.pfRelaxedElectronsPF = process.pfIsolatedElectronsPF.clone()
process.pfRelaxedElectronsPF.isolationCut = 9999
process.pfElectronsFromVertexPF.dzCut = 9999.0
process.pfElectronsFromVertexPF.d0Cut = 9999.0
process.pfSelectedElectronsPF.cut = ""
process.patElectronsPF.pfElectronSource  = "pfRelaxedElectronsPF"
process.pfElectronSequencePF.replace(process.pfIsolatedElectronsPF,
                                     process.pfIsolatedElectronsPF + 
                                     process.pfRelaxedElectronsPF)
    
# make loose clones of the original muon collection
process.pfRelaxedMuonsPF = process.pfIsolatedMuonsPF.clone()
process.pfRelaxedMuonsPF.isolationCut = 9999
process.pfMuonsFromVertexPF.dzCut = 9999.0
process.pfMuonsFromVertexPF.d0Cut = 9999.0
process.pfSelectedMuonsPF.cut = ""
process.patMuonsPF.pfMuonSource  = "pfRelaxedMuonsPF"
process.pfMuonSequencePF.replace(process.pfIsolatedMuonsPF,
                                 process.pfIsolatedMuonsPF + 
                                 process.pfRelaxedMuonsPF)


#-- Overwrite default output content ------------------------------------------
from SandBox.Skims.RA2Content_cff import getRA2PATOutput
process.out.outputCommands = getRA2PATOutput(process)
process.out.dropMetaData = cms.untracked.string('DROPPED')



## --- HLT -------------------------------------------------------------------
process.load('HLTrigger.HLTfilters.hltHighLevel_cfi')
process.hltHighLevel.HLTPaths = cms.vstring(hltPath_+"*")
process.hltHighLevel.andOr = cms.bool(True)
process.hltHighLevel.throw = cms.bool(False)

process.hltSelection = cms.Sequence(process.hltHighLevel)
if isMC_:
    process.hltSelection.remove(process.hltHighLevel)



## --- RA2 OBJECTS AND CLEANING ----------------------------------------------

process.load('SandBox.Skims.RA2Objects_cff')
process.load('SandBox.Skims.RA2Selection_cff')
process.load('SandBox.Skims.RA2Cleaning_cff')
process.load('SandBox.Skims.provInfoMuons_cfi')
process.load('SandBox.Skims.provInfoElectrons_cfi')

# Set filters in 'filter' mode (rejects events)
process.trackingFailureFilter.taggingMode = False
process.beamHaloFilter.taggingMode        = False
process.inconsistentMuons.taggingMode     = False
process.greedyMuons.taggingMode           = False
process.ra2EcalTPFilter.taggingMode       = False
process.ra2EcalBEFilter.taggingMode       = False
process.hcalLaserEventFilter.taggingMode  = False
process.eeBadScFilter.taggingMode         = False
process.ecalLaserCorrFilter.taggingMode   = False

# Remove the 'tagging' mode of the HBHENoiseFilter
# The filtering mode is simply HBHENoiseFilter, and that
# is already in the sequence
process.ra2NoiseCleaning.remove(process.HBHENoiseFilterRA2)

# Replace eeNoiseFilter by ra2PBNR filter
process.ra2NoiseCleaning.remove(process.eeNoiseFilter)
from RecoMET.METFilters.jetIDFailureFilter_cfi import jetIDFailure
process.PBNRFilter = jetIDFailure.clone(
    JetSource = cms.InputTag(jets_),
    MinJetPt      = cms.double(30.0),
    taggingMode   = cms.bool(False)
    )
process.ra2NoiseCleaning += process.PBNRFilter

# HO-Noise filter
from SandBox.Skims.hoNoiseFilter_cfi import hoNoiseFilter
process.RA2HONoiseFilter = hoNoiseFilter.clone(
    patJetsInputTag = cms.InputTag(jets_),
    jetPtMin        = cms.double(30),
    jetEtaMax       = cms.double(5),
    maxHOEfrac      = cms.double(0.4),
    taggingMode     = cms.bool(False)
    )
process.ra2NoiseCleaning += process.RA2HONoiseFilter

# Tracking-POG Cleaning (decisions have inverted meaning
# compared to the other filters!)
process.manystripclus53X.taggedMode         = cms.untracked.bool(False)
process.toomanystripclus53X.taggedMode      = cms.untracked.bool(False)
process.logErrorTooManyClusters.taggedMode  = cms.untracked.bool(False)
process.trackingPOGCleaningFilters = cms.Sequence(
    ~process.manystripclus53X *
    ~process.toomanystripclus53X *
    ~process.logErrorTooManyClusters
    )
process.ra2PostCleaning += process.trackingPOGCleaningFilters

# HCAL-laser-event-filter list
from RecoMET.METFilters.multiEventFilter_cfi import multiEventFilter
process.HCALLaserEvtFilterList2012 = multiEventFilter.clone(
    file        = cms.FileInPath('RA2Classic/AdditionalInputFiles/data/HCALLaserEventList_20Nov2012-v2_HT-HTMHT.txt'),
    taggingMode = cms.bool(True)
    )
process.ra2PostCleaning += process.HCALLaserEvtFilterList2012


# The final RA2 object + cleaning sequence
process.cleanpatseq = cms.Sequence(
        process.susyPatDefaultSequence *
        process.ra2StdCleaning *
        process.ra2Objects *
        process.provInfoMuons *
        process.provInfoElectrons *
        process.ra2PostCleaning
        )



## --- HT, MHT, NJETS PRESELECTION -------------------------------------------
process.htPFchsFilter.MinHT   = cms.double(300)
process.mhtPFchsFilter.MinMHT = cms.double(200)
process.countJetsPFchsPt50Eta25.minNumber = cms.uint32(2)



## --- WEIGHT PRODUCER -------------------------------------------------------
from RA2Classic.WeightProducer.getWeightProducer_cff import getWeightProducer
process.WeightProducer = getWeightProducer(dataSet_)
process.WeightProducer.Lumi                       = cms.double(19466)
process.WeightProducer.PU                         = cms.int32(3) # PU S10
process.WeightProducer.FileNamePUDataDistribution = cms.string("RA2Classic/WeightProducer/data/DataPileupHistogram_RA2Summer12_190456-208686_ABCD.root")

process.Weights = cms.Sequence(
    process.WeightProducer
    )
if not isMC_:
    process.Weights.remove(process.WeightProducer)




## --- DAS TREE MAKER --------------------------------------------------------
from DAS.DASTreeMaker.dasTreeMaker_cfi import dasTreeMaker

sampleID = 0
if "WJetsToLNu" in dataSet_:
    sampleID = 24
    
process.dasTree = dasTreeMaker.clone(
    WeightTag         = cms.InputTag('WeightProducer:weight'),
    PrescaleTag       = cms.InputTag(''),
    VertexCollection  = cms.InputTag('goodVertices'),
    Jets              = cms.InputTag(jets_),
    Electrons         = cms.InputTag('patElectronsID'),
    Muons             = cms.InputTag('patMuonsID'),
    IsoElectrons      = cms.InputTag('patElectronsIDIso'),
    IsoMuons          = cms.InputTag('patMuonsIDIso'),
    MET               = cms.InputTag('patMETsPF'),    
    SampleId          = cms.int32(sampleID),
    )



#process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.ppfchs = cms.Path(
    process.hltSelection *
    process.cleanpatseq *
    #process.dump *
    process.countJetsPFchsPt50Eta25 *
    process.Weights *
    process.dasTree
    )

process.schedule = cms.Schedule(process.ppfchs)
