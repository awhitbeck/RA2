import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.load("FWCore.Framework.test.cmsExceptionsFatal_cff")
process.load("Configuration.StandardSequences.Services_cff")
                                   
# The following three lines reduce the clutter of repeated printouts
# of the same exception message.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.destinations = ['cerr']
process.MessageLogger.statistics = []
process.MessageLogger.fwkJobReports = []
process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
    #"dcap://pnfs/cms/WAX/11/store/mc/Summer11/SMS-T1tttt_Mgluino-450to1200_mLSP-50to800_7TeV-Pythia6Z/AODSIM/PU_START42_V11_FSIM-v2/0000/FE652173-B8F8-E011-BF87-0025B3E05D74.root"]
  "dcap://pnfs/cms/WAX/11/store/results/susy/SMS-T2tt-smallScan_FastSim-Mstop-200to500_mLSP-0to200_8TeV-Pythia6Z/StoreResults-SMS-T2tt-smallScan_FastSim-Mstop-200to500_mLSP-0to200_8TeV-Pythia6Z-30d416a648a359e8b7f67df0bc7170fc/SMS-T2tt-smallScan_FastSim-Mstop-200to500_mLSP-0to200_8TeV-Pythia6Z/USER/StoreResults-SMS-T2tt-smallScan_FastSim-Mstop-200to500_mLSP-0to200_8TeV-Pythia6Z-30d416a648a359e8b7f67df0bc7170fc/0000/00C5C505-6EDC-E111-B23F-003048FFD71E.root"
  )
)

process.source.skipEvents = cms.untracked.uint32(0)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.PARSE = cms.EDAnalyzer("TestPrintEvent"
                               #SusyScanTopology   = cms.string("T1tttt"),
                               #SusyScanMotherMass = cms.double(1000),
                               #SusyScanLSPMass    = cms.double(400),
                               #SusyScanFracLSP    = cms.double(0.5),
                               #Debug              = cms.bool(False)
                             )


process.particle = cms.EDAnalyzer("ParticleDecayDrawer",
                                  src = cms.InputTag("genParticles")
                                  )

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.pyList = cms.EDAnalyzer("ParticleListDrawer",
                                maxEventsToPrint = cms.untracked.int32(5),
                                printVertex = cms.untracked.bool(False),
                                src = cms.InputTag("genParticles")
                                )
process.p = cms.Path(process.pyList * process.PARSE * process.particle)
#process.p = cms.Path(process.PARSE * process.particle)
#process.p = cms.Path(process.PARSE)

process.schedule = cms.Schedule(process.p)
