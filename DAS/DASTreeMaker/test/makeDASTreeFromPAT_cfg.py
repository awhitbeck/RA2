# Example: cmsRun makeDASTreeFromPAT_cfg.py data_set=/store/user/mschrode/WJetsToLNu_HT-400ToInf_8TeV-madgraph/RA2PreSelection_Summer12_DR53X-PU_S10_START53_V7A-v1_V4/6c50609e978ba7d5388d5439fc628605/RA2Skim_100_1_6zP.root, is_mc=true

# Read parameters
from RA2Classic.Utils.CommandLineParams import CommandLineParams
parameters = CommandLineParams()

dataSet_   = parameters.value("data_set","")
isMC_      = parameters.value("is_mc",False)
isSUSY_    = parameters.value("is_susy",False)
hltPath_   = parameters.value("hlt_path","none")


print "***** SETUP ************************************"
print "    dataSet_ : "+dataSet_
print "    hltPath_ : "+hltPath_
print "       isMC_ : "+str(isMC_)
print "     isSUSY_ : "+str(isSUSY_)
print "************************************************"

jets_ = "patJetsPF"



## --- GENERAL SETUP ---------------------------------------------------------
import FWCore.ParameterSet.Config as cms
process = cms.Process("DASTreeMaker")



## --- Log output ------------------------------------------------------
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr = cms.untracked.PSet(
    placeholder = cms.untracked.bool(True)
    )
process.MessageLogger.cout = cms.untracked.PSet(
    INFO = cms.untracked.PSet(reportEvery = cms.untracked.int32(1))
    )
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
    )


## --- Files to process ------------------------------------------------
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
    )
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(dataSet_)
    )


## --- Output file -----------------------------------------------------
process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("DASRA2Tree.root")
    )
    

## --- Selection sequences ---------------------------------------------
process.load('HLTrigger.HLTfilters.hltHighLevel_cfi')
process.hltHighLevel.HLTPaths = cms.vstring(hltPath_+"*")
process.hltHighLevel.andOr = cms.bool(True)
process.hltHighLevel.throw = cms.bool(False)

process.HLTSelection = cms.Sequence(process.hltHighLevel)
if isMC_:
    process.HLTSelection.remove(process.hltHighLevel)

        
# Filter-related selection
process.load('RA2Classic.TreeMaker.filterSelection_cff')

# adapt for signal samples
if "SUSY_LM" in dataSet_:
    print "Removing tracking-POG-filters from sequence"
    process.filterSelection.remove(process.manyStripClustersFilter)
    process.filterSelection.remove(process.tooManyStripClustersFilter)
    process.filterSelection.remove(process.logErrorTooManyClustersFilter)

from RecoMET.METFilters.jetIDFailureFilter_cfi import jetIDFailure
process.PBNRFilter = jetIDFailure.clone(
    JetSource = cms.InputTag(jets_),
    MinJetPt      = cms.double(30.0),
    taggingMode   = cms.bool(False)
    )
process.filterSelection += process.PBNRFilter

from RecoMET.METFilters.multiEventFilter_cfi import multiEventFilter
process.HCALLaserEvtFilterList2012 = multiEventFilter.clone(
    file        = cms.FileInPath('RA2Classic/AdditionalInputFiles/data/HCALLaserEventList_20Nov2012-v2_HT-HTMHT.txt'),
    taggingMode = cms.bool(False)
    )
process.filterSelection += process.HCALLaserEvtFilterList2012

process.NoiseEventFilterList = multiEventFilter.clone(
    file        = cms.FileInPath('RA2Classic/AdditionalInputFiles/data/NoiseEventList.txt'),
    taggingMode = cms.bool(False)
    )
process.filterSelection += process.NoiseEventFilterList

from SandBox.Skims.hoNoiseFilter_cfi import hoNoiseFilter
process.RA2HONoiseFilter = hoNoiseFilter.clone(
    patJetsInputTag = cms.InputTag(jets_),
    jetPtMin        = cms.double(30),
    jetEtaMax       = cms.double(5),
    maxHOEfrac      = cms.double(0.4),
    taggingMode     = cms.bool(False)
    )
process.filterSelection += process.RA2HONoiseFilter


# Select events with at least 2 "HT jets"
from DAS.DASTreeMaker.nCandFilter_cfi import nCandFilter
process.NJetFilter = nCandFilter.clone(
    CandidateCollection = cms.InputTag(jets_),
    PtMin  = cms.double(50),
    EtaMax = cms.double(2.5),
    NumMin = cms.int32(2),
)
# MHT preselection
from SandBox.Skims.RA2MHT_cff import mhtPFFilter
process.MHTSelection = mhtPFFilter.clone(
    MHTSource = cms.InputTag('mhtPFchs'),
    MinMHT = cms.double(100.)
    )




## --- Setup WeightProducer -------------------------------------------
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


## --- Setup of TreeMaker ----------------------------------------------
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
    IsoEletrons       = cms.InputTag('patElectronsIDIso'),
    IsoMuons          = cms.InputTag('patMuonsIDIso'),
    MET               = cms.InputTag('patMETsPF'),    
    SampleId          = cms.int32(sampleID),
    )


## --- Final paths ----------------------------------------------------

#    process.dump = cms.EDAnalyzer("EventContentAnalyzer")
process.WriteTree = cms.Path(
    process.HLTSelection *
    process.filterSelection *
    process.PBNRFilter * process.HCALLaserEvtFilterList2012 * process.RA2HONoiseFilter *
    process.MHTSelection *
    ##        process.dump
    process.NJetFilter *
    process.Weights *
    process.dasTree
    )
