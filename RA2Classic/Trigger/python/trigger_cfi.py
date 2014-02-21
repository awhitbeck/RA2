import FWCore.ParameterSet.Config as cms

trigger = cms.EDAnalyzer('Trigger',

   jetCollection = cms.InputTag("patJetsAK5PF"),
  	metCollection = cms.InputTag("patMETsPF"),
 	vertexCollection = cms.InputTag("offlinePrimaryVertices"),
   TriggerTag = cms.InputTag("TriggerResults","","HLT"),
   
	muon_pt_cut = cms.double( 15. ),
	muon_eta_cut = cms.double( 2.4 ),

	elec_pt_cut = cms.double( 10. ),
	elec_eta_cut = cms.double( 2.5 ),

	jet_pt_cut_MHT = cms.double( 30. ),
	jet_eta_cut_MHT = cms.double( 5.0 ),
   jet_pt_cut_HT = cms.double( 50. ),
	jet_eta_cut_HT = cms.double( 2.5 ),            
	jet_useJetID = cms.bool( True )                     
)

