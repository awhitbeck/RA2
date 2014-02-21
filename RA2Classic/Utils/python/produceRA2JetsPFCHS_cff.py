# $Id: produceRA2JetsPFCHS_cff.py,v 1.1 2012/08/08 10:13:57 mschrode Exp $


import FWCore.ParameterSet.Config as cms

from RA2Classic.Utils.patJetCollectionSubsetProducer_cfi import patJetCollectionSubsetProducer

# Create collection of jets with pt > 30 (for MHT)
MHTJets = patJetCollectionSubsetProducer.clone(
    Jets   = cms.InputTag("patJetsPF"),
    PtMin  = cms.double(30.),
    EtaMax = cms.double(99999.)
    )

# Create collection of jets with pt > 50 and eta < 2.5 (for NJet, HT)
HTJets = patJetCollectionSubsetProducer.clone(
    Jets   = cms.InputTag("patJetsPF"),
    PtMin  = cms.double(50.),
    EtaMax = cms.double(2.5)
    )

# Sequences to produce RA2 jet collections
produceRA2JetsPFCHS = cms.Sequence(
    MHTJets *
    HTJets
    )
