import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

#===================== Message Logger =============================
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(
        limit = cms.untracked.int32(10),
            reportEvery = cms.untracked.int32(1)
            )
process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(True)
            )
process.MessageLogger.cerr.FwkReport.reportEvery = 1


#================= configure poolsource module ===================
process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring("/store/mc/Summer12/SMS-T2tt_Mgluino-225to1200_mLSP-0to1000_8TeV-Pythia6Z/AODSIM/START52_V9_FSIM-v7/0002/06C82BA0-95EC-E111-848D-001EC9D7F697.root")
)
process.source.skipEvents = cms.untracked.uint32(0)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

#======= define SMS model filter =============
process.load("SusyAnalysis.AnalysisUtils.smsModelFilter_cfi")
process.smsModelFilter.SusyScanTopology   = "T2tt"
process.smsModelFilter.SusyScanMotherMass = 1175
process.smsModelFilter.SusyScanLSPMass    =  750
process.smsModelFilter.Debug              = False


process.p = cms.Path(process.smsModelFilter)

process.schedule = cms.Schedule(process.p)
