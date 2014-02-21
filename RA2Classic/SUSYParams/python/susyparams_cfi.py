import FWCore.ParameterSet.Config as cms

susyparams = cms.EDProducer('SUSYParams',
                            Model = cms.string('T1'),
                            Debug = cms.bool(False)
)
