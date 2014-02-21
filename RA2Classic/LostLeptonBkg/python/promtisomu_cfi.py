import FWCore.ParameterSet.Config as cms

promtIsoMu = cms.EDProducer('PromtIsoMu',
              MuonIDISOTag = cms.InputTag("patMuonsPFIDIso"),
              ElecIDISOTag = cms.InputTag("selectedPatElectronsPF"),
              GenTag = cms.InputTag("genParticles"),  
)
