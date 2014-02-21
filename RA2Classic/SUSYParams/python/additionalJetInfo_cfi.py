import FWCore.ParameterSet.Config as cms

AdditionalJetInfo = cms.EDProducer(
    'AdditionalJetInfo',
    JetSource = cms.InputTag(''),
    bTagName  = cms.string  ("combinedSecondaryVertexBJetTags")
    )
