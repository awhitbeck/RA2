# $Id: $
#

import FWCore.ParameterSet.Config as cms
from RA2Classic.Utils.hltDecisionProducer_cfi import hltDecisionProducer

# Pathes in HT PD (Run2012A)
HLTCleanPFHT300Ele15CaloIdTCaloIsoVLTrkIdTTrkIsoVLPFMET45 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_CleanPFHT300_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45")
)
HLTCleanPFHT300Ele15CaloIdTCaloIsoVLTrkIdTTrkIsoVLPFMET50 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_CleanPFHT300_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET50")
    )
HLTCleanPFHT300Ele40CaloIdVTTrkIdT = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_CleanPFHT300_Ele40_CaloIdVT_TrkIdT")
    )
HLTCleanPFHT300Ele60CaloIdVTTrkIdT = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_CleanPFHT300_Ele60_CaloIdVT_TrkIdT")
   )
HLTCleanPFHT350Ele5CaloIdTCaloIsoVLTrkIdTTrkIsoVLPFMET45 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_CleanPFHT350_Ele5_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET45")
   )
HLTCleanPFHT350Ele5CaloIdTCaloIsoVLTrkIdTTrkIsoVLPFMET50 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_CleanPFHT350_Ele5_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_PFMET50")
   )
HLTFatDiPFJetMass750DR1p1Deta1p5 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_FatDiPFJetMass750_DR1p1_Deta1p5")
   )
HLTHT200AlphaT0p57 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT200_AlphaT0p57")
   )
HLTHT200 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT200")
   )
HLTHT250AlphaT0p55 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT250_AlphaT0p55")
   )
HLTHT250AlphaT0p57 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT250_AlphaT0p57")
   )
HLTHT250DoubleDisplacedPFJet60ChgFraction10 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT250_DoubleDisplacedPFJet60_ChgFraction10")
   )
HLTHT250DoubleDisplacedPFJet60 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT250_DoubleDisplacedPFJet60")
   )
HLTHT250SingleDisplacedPFJet60ChgFraction10 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT250_SingleDisplacedPFJet60_ChgFraction10")
   )
HLTHT250SingleDisplacedPFJet60 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT250_SingleDisplacedPFJet60")
   )
HLTHT250 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT250")
   )
HLTHT300AlphaT0p53 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT300_AlphaT0p53")
   )
HLTHT300AlphaT0p54 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT300_AlphaT0p54")
   )
HLTHT300DoubleDisplacedPFJet60ChgFraction10 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT300_DoubleDisplacedPFJet60_ChgFraction10")
   )
HLTHT300DoubleDisplacedPFJet60 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT300_DoubleDisplacedPFJet60")
   )
HLTHT300SingleDisplacedPFJet60 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT300_SingleDisplacedPFJet60")
   )
HLTHT300SingleDisplacedPFJet60 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT300_SingleDisplacedPFJet60")
   )
HLTHT300 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT300")
   )
HLTHT350AlphaT0p52 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT350_AlphaT0p52")
   )
HLTHT350AlphaT0p53 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT350_AlphaT0p53")
   )
HLTHT350 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT350")
   )
HLTHT400AlphaT0p51 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT400_AlphaT0p51")
   )
HLTHT400AlphaT0p52 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT400_AlphaT0p52")
   )
HLTHT400 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT400")
   )
HLTHT450AlphaT0p51 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT450_AlphaT0p51")
   )
HLTHT450 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT450")
   )
HLTHT500 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT500")
   )
HLTHT550 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT550")
   )
HLTHT650 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT650")
   )
HLTHT750 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT750")
   )
HLTPFHT350PFMET100 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_PFHT350_PFMET100")
   )
HLTPFHT350 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_PFHT350")
   )
HLTPFHT400PFMET100 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_PFHT400_PFMET100")
   )
HLTPFHT650DiCentralPFJet80CenPFJet40 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_PFHT650_DiCentralPFJet80_CenPFJet40")
   )
HLTPFHT650 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_PFHT650")
   )
HLTPFHT700 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_PFHT700")
   )
HLTPFHT750 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_PFHT750")
   )
HLTRsqMR40Rsq0p04 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_RsqMR40_Rsq0p04")
   )
HLTRsqMR45Rsq0p09 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_RsqMR45_Rsq0p09")
   )
HLTRsqMR55Rsq0p09MR150 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_RsqMR55_Rsq0p09_MR150")
   )
HLTRsqMR60Rsq0p09MR150 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_RsqMR60_Rsq0p09_MR150")
   )
HLTRsqMR65Rsq0p09MR150 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_RsqMR65_Rsq0p09_MR150")
   )


# Pathes in HTMHT PD (>Run2012A)
HLTHT250AlphaT0p55 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT250_AlphaT0p55")
   )
HLTHT250AlphaT0p57 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT250_AlphaT0p57")
   )
HLTHT300AlphaT0p53 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT300_AlphaT0p53")
   )
HLTHT300AlphaT0p54 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT300_AlphaT0p54")
   )
HLTHT350AlphaT0p52 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT350_AlphaT0p52")
   )
HLTHT350AlphaT0p53 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT350_AlphaT0p53")
   )
HLTHT400AlphaT0p51 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT400_AlphaT0p51")
   )
HLTHT400AlphaT0p52 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT400_AlphaT0p52")
   )
HLTHT450AlphaT0p51 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_HT450_AlphaT0p51")
   )
HLTPFNoPUHT350PFMET100 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_PFNoPUHT350_PFMET100")
   )
HLTPFNoPUHT400PFMET100 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_PFNoPUHT400_PFMET100")
   )
HLTRsqMR40Rsq0p04 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_RsqMR40_Rsq0p04")
   )
HLTRsqMR55Rsq0p09MR150 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_RsqMR55_Rsq0p09_MR150")
   )
HLTRsqMR60Rsq0p09MR150 = hltDecisionProducer.clone(
   HLTName = cms.string("HLT_RsqMR60_Rsq0p09_MR150")
   )
HLTRsqMR65Rsq0p09MR150 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_RsqMR65_Rsq0p09_MR150")
    )


# Pathes in JetHT PD (>Run2012A)
HLTDiPFJetAve320 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_DiPFJetAve320")
    )
HLTDiPFJetAve400 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_DiPFJetAve400")
    )
HLTFatDiPFJetMass750DR1p1Deta1p5 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_FatDiPFJetMass750_DR1p1_Deta1p5")
    )
HLTHT200 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT200")
    )
HLTHT250 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT250")
    )
HLTHT300DoubleDisplacedPFJet60ChgFraction10 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT300_DoubleDisplacedPFJet60_ChgFraction10")
    )
HLTHT300DoubleDisplacedPFJet60 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT300_DoubleDisplacedPFJet60")
    )
HLTHT300SingleDisplacedPFJet60 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT300_SingleDisplacedPFJet60")
    )
HLTHT300SingleDisplacedPFJet60 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT300_SingleDisplacedPFJet60")
    )
HLTHT300 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT300")
    )
HLTHT350 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT350")
    )
HLTHT400 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT400")
    )
HLTHT450 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT450")
    )
HLTHT500 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT500")
    )
HLTHT550 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT550")
    )
HLTHT650Track50dEdx3p6 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT650_Track50_dEdx3p6")
    )
HLTHT650Track60dEdx3p7 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT650_Track60_dEdx3p7")
    )
HLTHT650 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT650")
    )
HLTHT750 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_HT750")
    )
HLTJet370NoJetID = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_Jet370_NoJetID")
    )
HLTMET80Track50dEdx3p6 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_MET80_Track50_dEdx3p6")
    )
HLTMET80Track60dEdx3p7 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_MET80_Track60_dEdx3p7")
    )
HLTMET80 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_MET80")
    )
HLTPFJet320 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_PFJet320")
    )
HLTPFJet400 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_PFJet400")
    )
HLTPFNoPUHT350 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_PFNoPUHT350")
    )
HLTPFNoPUHT650DiCentralPFNoPUJet80CenPFNoPUJet40 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_PFNoPUHT650_DiCentralPFNoPUJet80_CenPFNoPUJet40")
    )
HLTPFNoPUHT650 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_PFNoPUHT650")
    )
HLTPFNoPUHT700 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_PFNoPUHT700")
    )
HLTPFNoPUHT750 = hltDecisionProducer.clone(
    HLTName = cms.string("HLT_PFNoPUHT750")
    )





hltDecisions = cms.Sequence(
    HLTCleanPFHT300Ele15CaloIdTCaloIsoVLTrkIdTTrkIsoVLPFMET45 *
    HLTCleanPFHT300Ele15CaloIdTCaloIsoVLTrkIdTTrkIsoVLPFMET50 *
    HLTCleanPFHT300Ele40CaloIdVTTrkIdT *
    HLTCleanPFHT300Ele60CaloIdVTTrkIdT *
    HLTCleanPFHT350Ele5CaloIdTCaloIsoVLTrkIdTTrkIsoVLPFMET45 *
    HLTCleanPFHT350Ele5CaloIdTCaloIsoVLTrkIdTTrkIsoVLPFMET50 *
    HLTFatDiPFJetMass750DR1p1Deta1p5 *
    HLTHT200AlphaT0p57 *
    HLTHT200 *
    HLTHT250AlphaT0p55 *
    HLTHT250AlphaT0p57 *
    HLTHT250DoubleDisplacedPFJet60ChgFraction10 *
    HLTHT250DoubleDisplacedPFJet60 *
    HLTHT250SingleDisplacedPFJet60ChgFraction10 *
    HLTHT250SingleDisplacedPFJet60 *
    HLTHT250 *
    HLTHT300AlphaT0p53 *
    HLTHT300AlphaT0p54 *
    HLTHT300DoubleDisplacedPFJet60ChgFraction10 *
    HLTHT300DoubleDisplacedPFJet60 *
    HLTHT300SingleDisplacedPFJet60 *
    HLTHT300SingleDisplacedPFJet60 *
    HLTHT300 *
    HLTHT350AlphaT0p52 *
    HLTHT350AlphaT0p53 *
    HLTHT350 *
    HLTHT400AlphaT0p51 *
    HLTHT400AlphaT0p52 *
    HLTHT400 *
    HLTHT450AlphaT0p51 *
    HLTHT450 *
    HLTHT500 *
    HLTHT550 *
    HLTHT650 *
    HLTHT750 *
    HLTPFHT350PFMET100 *
    HLTPFHT350 *
    HLTPFHT400PFMET100 *
    HLTPFHT650DiCentralPFJet80CenPFJet40 *
    HLTPFHT650 *
    HLTPFHT700 *
    HLTPFHT750 *
    HLTRsqMR40Rsq0p04 *
    HLTRsqMR45Rsq0p09 *
    HLTRsqMR55Rsq0p09MR150 *
    HLTRsqMR60Rsq0p09MR150 *
    HLTRsqMR65Rsq0p09MR150 *
    HLTHT250AlphaT0p55 *
    HLTHT250AlphaT0p57 *
    HLTHT300AlphaT0p53 *
    HLTHT300AlphaT0p54 *
    HLTHT350AlphaT0p52 *
    HLTHT350AlphaT0p53 *
    HLTHT400AlphaT0p51 *
    HLTHT400AlphaT0p52 *
    HLTHT450AlphaT0p51 *
    HLTPFNoPUHT350PFMET100 *
    HLTPFNoPUHT400PFMET100 *
    HLTRsqMR40Rsq0p04 *
    HLTRsqMR55Rsq0p09MR150 *
    HLTRsqMR60Rsq0p09MR150 *
    HLTRsqMR65Rsq0p09MR150 *
    HLTDiPFJetAve320 *
    HLTDiPFJetAve400 *
    HLTFatDiPFJetMass750DR1p1Deta1p5 *
    HLTHT200 *
    HLTHT250 *
    HLTHT300DoubleDisplacedPFJet60ChgFraction10 *
    HLTHT300DoubleDisplacedPFJet60 *
    HLTHT300SingleDisplacedPFJet60 *
    HLTHT300SingleDisplacedPFJet60 *
    HLTHT300 *
    HLTHT350 *
    HLTHT400 *
    HLTHT450 *
    HLTHT500 *
    HLTHT550 *
    HLTHT650Track50dEdx3p6 *
    HLTHT650Track60dEdx3p7 *
    HLTHT650 *
    HLTHT750 *
    HLTJet370NoJetID *
    HLTMET80Track50dEdx3p6 *
    HLTMET80Track60dEdx3p7 *
    HLTMET80 *
    HLTPFJet320 *
    HLTPFJet400 *
    HLTPFNoPUHT350 *
    HLTPFNoPUHT650DiCentralPFNoPUJet80CenPFNoPUJet40 *
    HLTPFNoPUHT650 *
    HLTPFNoPUHT700 *
    HLTPFNoPUHT750
    )
