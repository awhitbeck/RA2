import FWCore.ParameterSet.Config as cms

PUInfo = cms.EDProducer(
    'PUInfo',
    PileupSummaryInfoTag = cms.InputTag('addPileupInfo')
    )
