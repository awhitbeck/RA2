# $Id: $
#
# Apply cut on ratio of Calo and PF MHT
#
# You need to define a valid global tag and load the
# JEC services before using RA2CaloVsPFMHTFilterSequence,
# i.e. do
#
#  process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#  process.GlobalTag.globaltag = <a valid GT>
#  process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

import FWCore.ParameterSet.Config as cms

# Produce MHT from Calo jets
from SandBox.Skims.mhtProducer_cfi import mht
mhtCalo = mht.clone(
    JetCollection = cms.InputTag('ak5CaloJetsL1L2L3Residual'),
    MinJetPt      = cms.double(30),
    MaxJetEta     = cms.double(5)
    )

# Define filter on MHT ratio
from caloVsPFMetFilter_cfi import caloVsPFMetFilter
RA2CaloVsPFMHTFilter = caloVsPFMetFilter.clone(
    CaloMetInputTag  = cms.InputTag('mhtCalo'),
    PFMetInputTag    = cms.InputTag('mhtPFchs'),
    MinCaloOverPFMet = cms.double(0.5),
    SizeOfDeltaPhiWindowInNPi = cms.double(1.),
    TaggingMode      = cms.bool(True)
)

# Filter sequence
from JetMETCorrections.Configuration.DefaultJEC_cff import *
RA2CaloVsPFMHTFilterSequence = cms.Sequence(
    ak5CaloJetsL1L2L3Residual *         # Produce corrected AK5 Calo jets
    mhtCalo *                           # Produce MHT from those Calo jets
    RA2CaloVsPFMHTFilter                # Apply filter conditions
    )

