import FWCore.ParameterSet.Config as cms

caloVsPFMetFilter = cms.EDFilter(
    "CaloVsPFMetFilter",
    CaloMetInputTag           = cms.InputTag(''),
    PFMetInputTag             = cms.InputTag(''),
    MinCaloOverPFMet          = cms.double(0.5),
    SizeOfDeltaPhiWindowInNPi = cms.double(1.),
    TaggingMode               = cms.bool(False)
)
