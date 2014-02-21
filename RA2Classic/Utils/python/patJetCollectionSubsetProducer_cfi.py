import FWCore.ParameterSet.Config as cms

patJetCollectionSubsetProducer = cms.EDProducer(
    'PATJetCollectionSubsetProducer',
    Jets   = cms.InputTag('patJetsAK5PF'),
    PtMin  = cms.double(50.),
    EtaMax = cms.double(10.)
)
