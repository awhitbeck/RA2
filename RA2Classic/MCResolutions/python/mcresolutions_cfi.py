import FWCore.ParameterSet.Config as cms

MCResolutions = cms.EDAnalyzer('MCResolutions',
	jetTag				= cms.InputTag('patJetsAK5PF'),
	muonTag				= cms.InputTag('muons'),
	genJetTag			= cms.InputTag('ak5GenJets'),
	weightName			= cms.InputTag('weightProducer','weight','PAT'),
	EBRecHits			= cms.InputTag('ecalRecHit','EcalRecHitsEB'),
	EERecHits			= cms.InputTag('ecalRecHit','EcalRecHitsEE'),
	jetMultPtCut      = cms.double(50.),
	jetMultEtaCut     = cms.double(2.5),
	deltaPhiDiJet		= cms.double(2.7),
	absCut3rdJet		= cms.double(30.),
	relCut3rdJet		= cms.double(0.2),
	deltaRMatch			= cms.double(0.1),
	deltaRMatchVeto	= cms.double(0.7),
	absPtVeto			= cms.double(30.),
	relPtVeto			= cms.double(0.05),
	deltaRDeadECal		= cms.double(0.2),
	GenJetPtCut			= cms.double(0.),
   maskedEcalChannelStatusThreshold	= cms.int32(13),
   fileName				= cms.string('MCJetResolution.root'),
)
