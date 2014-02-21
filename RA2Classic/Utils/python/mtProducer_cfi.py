import FWCore.ParameterSet.Config as cms

mtProducer = cms.EDProducer(
    "MTProducer",
    CandidateCollectionTag = cms.InputTag("patMuonsPFIDIso"),
    METTag                 = cms.InputTag("patMETsPF")
    )
