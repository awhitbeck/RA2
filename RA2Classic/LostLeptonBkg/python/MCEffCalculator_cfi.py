import FWCore.ParameterSet.Config as cms

MCEffCalculator = cms.EDAnalyzer(
	'MCEffCalculator',
	      TreeName          = cms.string('LostLeptonMCEff'),
	      EventWeightTag = cms.InputTag("WeightProducer:weight"),           
              MuonIDTag = cms.InputTag("patMuonsPFID"),
              MuonIDISOTag = cms.InputTag("patMuonsPFIDIso"),
	      MuonPromtMatchedTag = cms.InputTag("promtLeptons:PromtMuon"),
              ElecIDTag = cms.InputTag("patElectronsIDIso"), 
	      ElecIDISOTag = cms.InputTag("patElectronsIDIso"),
	      CaloJetTag = cms.InputTag("cleanPatJetsAK5Calo"),
              METTag = cms.InputTag("patMETsPF"),
              GenTag = cms.InputTag("genParticles"),                  
              minMuonPt = cms.double(10),
	      maxMuonEta = cms.double(2.4),
              minElecPt = cms.double(10),
	      maxElecEta = cms.double(2.5),
              minJetPt = cms.double(30),
              HTJetsTag   = cms.InputTag('patJetsAK5PFPt50Eta25'),
	      MhtJetsTag   = cms.InputTag('mhtPF'),
	      HTTag	   = cms.InputTag('htInputCol'),
 	      MHTTag	   = cms.InputTag('mhtInputCol'),
	      VertexCollection  = cms.InputTag('goodVertices'),


	)