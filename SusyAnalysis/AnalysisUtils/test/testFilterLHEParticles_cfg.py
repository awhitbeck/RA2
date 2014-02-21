import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

#===================== Message Logger =============================
process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(
            limit = cms.untracked.int32(10),
            reportEvery = cms.untracked.int32(100)
            )
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.options = cms.untracked.PSet(
            wantSummary = cms.untracked.bool(True)
            )

#================= configure poolsource module ===================

#process.load('SusyAnalysis.PhotonAnalysis.PhotonRun2011AMay10ReReco_160404to163869_cfi');
#process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(FILELIST ))
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
    'file:/uscms/home/seema/3DayLifetime/MCDBtoEDM_NONE.root'
   )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100 ))

process.source.skipEvents = cms.untracked.uint32(0)

#========================= analysis module =====================================

process.analysis = cms.EDFilter('FilterLHEParticles',
                                LHEProductSrc = cms.InputTag("source"),
                                Debug         = cms.bool(False),
                                FilterPdgId   = cms.int32(22), # for photon
                                FilterStatus  = cms.int32(1), # for photon
                                #FilterPdgId   = cms.int32(23), # for Z
                                #FilterStatus  = cms.int32(2), # for photon
                                MinimumPt     = cms.double(70.0)
)

#================ configure filters and analysis sequence=======================

process.analysisSeq = cms.Sequence(process.analysis
)

#======================= output module configuration ===========================

#process.TFileService = cms.Service("TFileService",
#    fileName = cms.string('test.root')
#)

process.p1 = cms.Path(process.analysisSeq )
