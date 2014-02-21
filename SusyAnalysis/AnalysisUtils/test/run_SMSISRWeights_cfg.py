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
     fileNames = cms.untracked.vstring('/store/mc/Summer12/SMS-T1tttt_Mgluino-350to2000_mLSP-0to1650_8TeV-Pythia6Z/AODSIM/START52_V9_FSIM-v3/0002/46A8899B-39ED-E111-97C7-00221981B44C.root')
   #fileNames = cms.untracked.vstring("/store/mc/Summer12/SMS-T2tt_Mgluino-225to1200_mLSP-0to1000_8TeV-Pythia6Z/AODSIM/START52_V9_FSIM-v7/0002/06C82BA0-95EC-E111-848D-001EC9D7F697.root")
)
process.source.skipEvents = cms.untracked.uint32(0)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

#======= define SMS model filter =============
process.load("SusyAnalysis.AnalysisUtils.smsModelFilter_cfi")
process.smsModelFilter.SusyScanTopology   = "T2tt"
process.smsModelFilter.SusyScanMotherMass = 1175
process.smsModelFilter.SusyScanLSPMass    =  750
process.smsModelFilter.Debug              = False

process.lastParticles   = cms.EDProducer("ISRProducer")
process.myProducerLabel = cms.EDProducer('ISRHistoWeightProducer',
                                         src       = cms.InputTag('lastParticles'),
                                         inputFile = cms.untracked.FileInPath("SusyAnalysis/AnalysisUtils/data/ISRWeight_PtDistributions_T1.root"),
                                         #inputFile = cms.untracked.FileInPath("SusyAnalysis/RA2SusyScanAnalysis/data/ISRWeight_PtDistributions_T2.root"),
                                         SusyScanTopology = cms.string("T1"),
                                         #SusyScanTopology = cms.string("T2"),
                                         Debug            = cms.bool(False),
                                         reweightVariable=cms.untracked.string('pt')
                                         )

#process.p = cms.Path(process.smsModelFilter)
process.ppf = cms.Path(process.lastParticles * process.myProducerLabel)

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('/uscms/home/seema/3DayLifetime/testISRWeight.root'),
                               # save only events passing the full path
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('ppf') ),
                               # save PAT Layer 1 output; you need a '*' to
                               # unpack the list of commands 'patEventContent'
                               outputCommands = cms.untracked.vstring('keep *' )
                               )
process.outpath = cms.EndPath(process.out)

#process.schedule = cms.Schedule(process.p)
