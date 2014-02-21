import FWCore.ParameterSet.Config as cms

hltDecisionProducer = cms.EDProducer(
    "HLTDecisionProducer",
    HLTResultsTag = cms.InputTag("TriggerResults::HLT"),
    HLTName       = cms.string("")
    )
