import FWCore.ParameterSet.Config as cms

prescaleweightProducer = cms.EDProducer('PrescaleWeightProducer',
   #Option 1:
   weight = cms.double( -1.0 ),          # If larger 1 use this weight

   #Option 2:
   weightName  = cms.InputTag('weight'), # Name of an existing weight-variable in the event

   #The final calculated weight is scaled by the following factor.
   #This can be used e.g. to scale the luminosity by +-1sigma.
   LumiScale = cms.double( 1.0 ),
   PrescaleCut = cms.double( -1. ),
   
   HLTProcess = cms.string("HLT"),
   hltTag = cms.InputTag("TriggerResults","","HLT"),
   trgEvtObj = cms.InputTag("hltTriggerSummaryAOD::HLT"),

   # if you want to use prescale weights for 7 TeV --> set to false
   PFHTWeights = cms.bool( True )
                                        
)
