# Test TreeMaker on PAT skim
#

import FWCore.ParameterSet.Config as cms

process = cms.Process("RA2TreeMaker")

# --- Log output ------------------------------------------------------
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr = cms.untracked.PSet(
    placeholder = cms.untracked.bool(True)
    )
process.MessageLogger.statistics.append('cout')
process.MessageLogger.cout = cms.untracked.PSet(
    INFO = cms.untracked.PSet(reportEvery = cms.untracked.int32(10))
    )


# --- Files to process ------------------------------------------------
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-10)
    )
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/user/kheine/HT/RA2PreSelectionOnData_Run2012A_HT_PromptReco-v1_v3/873011f2367b772f5966dc0ae93602f4/RA2SkimsOnData_157_1_iMY.root',
    '/store/user/kheine/HT/RA2PreSelectionOnData_Run2012A_HT_PromptReco-v1_v3/873011f2367b772f5966dc0ae93602f4/RA2SkimsOnData_134_1_fwg.root',
    '/store/user/kheine/HT/RA2PreSelectionOnData_Run2012A_HT_PromptReco-v1_v3/873011f2367b772f5966dc0ae93602f4/RA2SkimsOnData_207_1_seb.root',
    '/store/user/kheine/HT/RA2PreSelectionOnData_Run2012A_HT_PromptReco-v1_v3/873011f2367b772f5966dc0ae93602f4/RA2SkimsOnData_294_1_SDf.root',
    '/store/user/kheine/HT/RA2PreSelectionOnData_Run2012A_HT_PromptReco-v1_v3/873011f2367b772f5966dc0ae93602f4/RA2SkimsOnData_118_1_Ii1.root',
    '/store/user/kheine/HT/RA2PreSelectionOnData_Run2012A_HT_PromptReco-v1_v3/873011f2367b772f5966dc0ae93602f4/RA2SkimsOnData_63_1_slU.root',
    '/store/user/kheine/HT/RA2PreSelectionOnData_Run2012A_HT_PromptReco-v1_v3/873011f2367b772f5966dc0ae93602f4/RA2SkimsOnData_487_1_4dF.root'
    )
    )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("RA2TestTree.root")
                                   )

process.load('RA2Classic.TreeMaker.treemaker_cfi')
process.demo = cms.Sequence(
    process.TreeMaker
    )


process.p = cms.Path(process.demo)
