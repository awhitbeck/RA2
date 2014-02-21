import FWCore.ParameterSet.Config as cms

nCandFilter = cms.EDFilter(
    "NCandFilter",
    CandidateCollection = cms.InputTag(''),
    PtMin  = cms.double(0.),
    EtaMax = cms.double(5.),
    NumMin = cms.int32(0),
)
