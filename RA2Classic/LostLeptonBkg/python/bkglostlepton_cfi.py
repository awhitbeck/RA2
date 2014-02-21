
import FWCore.ParameterSet.Config as cms
# all corrections must be in % entered relative to the mean value of prediction
# allunceratinties are relative to the underlaying correction factor or the total weight in % to be entered


bkglostlepton = cms.EDProducer(
    'LostLeptonBkg',
    	HTJets          = cms.InputTag('patJetsAK5PFPt50Eta25'),
	MetTag		= cms.InputTag('patMETsPF'),
	MuonTag		= cms.InputTag('patMuonsPFIDIso'),
	MuonRecoTag	= cms.InputTag('patMuonsPFID'),
	ElecTag		= cms.InputTag('patElectronsIDIso'),
	EventWeightTag	= cms.InputTag("WeightProducer:weight"),
	MhtTag		= cms.InputTag('mhtPF'),
	CaloJetTag	= cms.InputTag('cleanPatJetsAK5Calo'),
	EfficiencyFileName = cms.string('MCEff.root'),
	MTWMax		= cms.double('100'),
	MTWCut		= cms.bool(True),
	MinMuPT		= cms.double('10'),
	MinElecPT	= cms.double('10'),
        HTTag	   	= cms.InputTag('htInputCol'),
        MHTTag	   	= cms.InputTag('mhtInputCol'),
        VertexCollection= cms.InputTag('goodVertices'),
	DiLepCorrection = cms.double('-1.5'),
	DiLepCorrectionUp = cms.double('50'), 
	DiLepCorrectionDown = cms.double('50'),
	MTWUncertaintyUp = cms.double('40'),
	MTWUncertaintyDown = cms.double('40'),
	TagAndProbeUncertainty	= cms.bool(True),
	TAPUncertaintiesHTNJET = cms.bool(False),
	TagAndProbeConstUncertainty	= cms.bool(True),
	MetJetTagUp = cms.InputTag('jesUp:METs'),
	MetJetTagDown = cms.InputTag('jesDown:METs'),
	muonAccUncertaintyUp = cms.double('9'),
	muonAccUncertaintyDown = cms.double('9'),
	elecAccUncertaintyUp = cms.double('9'),
	elecAccUncertaintyDown = cms.double('9'),
	diBosonDown = cms.double('3'),
	nonClosureLowNJet = cms.double('15'),
	nonClosureHighNJet = cms.double('40'),
	IsoPlots = cms.bool(False),
	statErrorEffmaps = cms.bool(True),


)