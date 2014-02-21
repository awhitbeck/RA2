import FWCore.ParameterSet.Config as cms

dasTreeMaker = cms.EDAnalyzer(
    'DASTreeMaker',

    # Name of the output tree
    TreeName          = cms.string('DASRA2Tree'),

    # Dataset id
    SampleId          = cms.int32(0),

    # collection from which the tree variable "NumVtx" is determined
    VertexCollection  = cms.InputTag('goodVertices'),

    # jet collection from which "HT" and "NJets" are computed
    HTJets            = cms.InputTag('patJetsAK5PFPt50Eta25'),

    # jet collection from which "MHT" and the "DeltaPhi" are computed
    MHTJets           = cms.InputTag('patJetsAK5PFPt30'),

    # all jets
    Jets              = cms.InputTag('patJetsPF'),

    # leptons
    Electrons         = cms.InputTag('patElectronsID'),
    Muons             = cms.InputTag('patMuonsID'),

    # ra2 leptons, i.e. isolated
    IsoElectrons         = cms.InputTag('patElectronsIDIso'),
    IsoMuons             = cms.InputTag('patMuonsIDIso'),

    # double-precision event weights
    WeightTag         = cms.InputTag(''),
    PrescaleTag       = cms.InputTag(''),

    # MET for MT computation
    MET               = cms.InputTag(''),

    # Generator particles
    GenParticles      = cms.InputTag('genParticles'),
)
