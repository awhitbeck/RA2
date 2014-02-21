# $Id: makeTreeFromSignalScan_cff.py,v 1.2 2012/11/19 21:25:22 lungu Exp $
#
# If using this function in a .py config, you need to make
# 'from PhysicsTools.PatAlgos.patTemplate_cfg import *' in
# that file first. Can't do it here


#import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.patTemplate_cfg import *
process.outpath.remove(process.out)
def makeTreeFromSignalScan(process,
                           globalTag,
                           smsModel,
                           smsMotherMass,
                           smsLSPMass,
                           SMStopol,
                           xs=1,
                           numEvtsInSample=1,
                           lumi=5.088,
                           reportEveryEvt=5000,
                           testFileName="/store/mc/Summer12/WJetsToLNu_HT-400ToInf_8TeV-madgraph/AODSIM/PU_S7_START52_V9-v1/0000/041CF749-57A0-E111-B6DD-0026189438DA.root",
                           numProcessedEvt=20):
    

    #-- Meta data to be logged in DBS ---------------------------------------------
    process.configurationMetadata = cms.untracked.PSet(
        version = cms.untracked.string('$Revision: 1.2 $'),
        name = cms.untracked.string('$Source: /local/reps/CMSSW/UserCode/kheine/RA2Classic/TreeMaker/python/makeTreeFromSignalScan_cff.py,v $'),
        annotation = cms.untracked.string('SUSY pattuple definition')
        )


    #-- Message Logger ------------------------------------------------------------
    process.MessageLogger.categories.append('PATSummaryTables')
    process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(
        limit = cms.untracked.int32(-1),
        reportEvery = cms.untracked.int32(1)
        )
    process.MessageLogger.cerr.FwkReport.reportEvery = reportEveryEvt


    #-- Input Source --------------------------------------------------------------
    process.maxEvents.input = numProcessedEvt
    process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
    process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring(testFileName))


    #-- SUSYPAT specifics ---------------------------------------------------------
    from PhysicsTools.Configuration.SUSY_pattuple_cff import addDefaultSUSYPAT, getSUSY_pattuple_outputCommands

    hltMenu = 'REDIGI'

    theJetColls = ['AK5PF']

    jetMetCorr = ['L1FastJet', 'L2Relative', 'L3Absolute']

    process.GlobalTag.globaltag = globalTag

    addDefaultSUSYPAT(process,
                      mcInfo=True,
                      HLTMenu=hltMenu,
                      jetMetCorrections=jetMetCorr,
                      mcVersion='',
                      theJetNames=theJetColls,
                      doSusyTopProjection=False)

    # Remove the PAT cleaning and filtering sequences
    process.patDefaultSequence.remove(process.selectedPatCandidates)
    process.patDefaultSequence.remove(process.cleanPatCandidates)
    process.patDefaultSequence.remove(process.countPatCandidates)
    
    # Disable embedment so that lepton cleaning method works
    process.patJetsAK5PF.embedCaloTowers = False
    process.patJetsAK5PF.embedPFCandidates = False
    process.patJetsPF.embedCaloTowers = False
    process.patJetsPF.embedPFCandidates = False 
    
    #-- Adjust collections to use PFNoPU jets -------------------------------------
    
    # do not use Z-mass window for PU subtraction
    # such that JEC works properly
    process.pfPileUpPF.checkClosestZVertex = cms.bool(False)
    
    # do not remove muons and electrons from the jet clustering input
    # (patMuons and patElectrons use relaxed pfMuon and pfElectron collections)
    process.pfIsolatedElectronsPF.isolationCut = -1
    process.pfIsolatedMuonsPF.isolationCut = -1

    # make loose clones of the original electron collection
    process.pfRelaxedElectronsPF = process.pfIsolatedElectronsPF.clone()
    process.pfRelaxedElectronsPF.isolationCut = 9999
    process.pfElectronsFromVertexPF.dzCut = 9999.0
    process.pfElectronsFromVertexPF.d0Cut = 9999.0
    process.pfSelectedElectronsPF.cut = ""
    process.patElectronsPF.pfElectronSource  = "pfRelaxedElectronsPF"
    process.pfElectronSequencePF.replace(process.pfIsolatedElectronsPF,
                                         process.pfIsolatedElectronsPF + 
                                         process.pfRelaxedElectronsPF)
    
    # make loose clones of the original muon collection
    process.pfRelaxedMuonsPF = process.pfIsolatedMuonsPF.clone()
    process.pfRelaxedMuonsPF.isolationCut = 9999
    process.pfMuonsFromVertexPF.dzCut = 9999.0
    process.pfMuonsFromVertexPF.d0Cut = 9999.0
    process.pfSelectedMuonsPF.cut = ""
    process.patMuonsPF.pfMuonSource  = "pfRelaxedMuonsPF"
    process.pfMuonSequencePF.replace(process.pfIsolatedMuonsPF,
                                     process.pfIsolatedMuonsPF + 
                                     process.pfRelaxedMuonsPF)
    

    # do not remove taus from the jet collection
    process.pfTausPF.discriminators = cms.VPSet()
    process.pfUnclusteredTausPF = process.pfTausPF.clone(
        cut = cms.string("pt < 0")
    )
    process.pfTauSequencePF.replace(process.pfTausPF, process.pfTausPF+ process.pfUnclusteredTausPF)
    process.pfNoTauPF.topCollection = "pfUnclusteredTausPF"
    #------------------------------------------------------------------------------

    #-- Overwrite default output content ------------------------------------------
    from SandBox.Skims.RA2Content_cff import getRA2PATOutput
    process.out.outputCommands = getRA2PATOutput(process)
    process.out.dropMetaData = cms.untracked.string('DROPPED')
    

    #-- RA2 filtering -------------------------------------------------------------
    process.prefilterCounter        = cms.EDProducer("EventCountProducer")
    process.postStdCleaningCounter  = cms.EDProducer("EventCountProducer")
    process.postPostCleaningCounter = cms.EDProducer("EventCountProducer")
    process.postPFchsJetsCounter    = cms.EDProducer("EventCountProducer")
    process.postPFchsHTCounter      = cms.EDProducer("EventCountProducer")
    process.postPFchsMHTCounter     = cms.EDProducer("EventCountProducer")

    process.load('SandBox.Skims.RA2Objects_cff')
    process.load('SandBox.Skims.RA2Selection_cff')
    process.load('SandBox.Skims.RA2Cleaning_cff')

    # Adjust object filters for signal-scan values
    process.htPFchsFilter.MinHT               = cms.double(300.0)
    process.mhtPFchsFilter.MinMHT             = cms.double(100.0)
    process.countJetsPFchsPt50Eta25.minNumber = cms.uint32(2)

    ## please comment this block to remove tagging mode of
    ##filters and reject events failing any of following filters
    process.eeNoiseFilter.taggingMode         = False
    process.trackingFailureFilter.taggingMode = False
    process.beamHaloFilter.taggingMode        = False
    process.ra2NoiseCleaning.remove(process.HBHENoiseFilter)
    process.inconsistentMuons.taggingMode     = False
    process.greedyMuons.taggingMode           = False
    process.ra2EcalTPFilter.taggingMode       = False
    process.ra2EcalBEFilter.taggingMode       = False
    process.hcalLaserEventFilter.taggingMode  = False
    process.eeBadScFilter.taggingMode         = False

    process.load("SandBox.Skims.provInfoMuons_cfi")
    process.load("SandBox.Skims.provInfoElectrons_cfi")

    process.cleanpatseq = cms.Sequence(
        process.susyPatDefaultSequence *
        process.prefilterCounter *
        process.ra2StdCleaning *
        process.postStdCleaningCounter *
        process.ra2Objects *
        process.provInfoMuons *
        process.provInfoElectrons *
        process.ra2PostCleaning *
        process.postPostCleaningCounter
        )


    ## --- Setup WeightProducer --------------------------------------------
    from RA2Classic.WeightProducer.weightProducer_cfi import weightProducer
    process.WeightProducer = weightProducer.clone(
        weight     = cms.double(-1.0),
        Method     = cms.string("Constant"),
        XS         = cms.double(xs),
        NumberEvts = cms.double(numEvtsInSample),
        Lumi       = cms.double(lumi),
        LumiScale  = cms.double(1.0),
        FileNamePUDataDistribution = cms.string("NONE"),
        PU         = cms.int32(0)
        )


    ## --- SMS Model Filter ------------------------------------------------
    # Filter files for specified signal point out of dataset
    # Code from UserCode/seema/SusyAnalysis/AnalysisUtils
    from SusyAnalysis.AnalysisUtils.smsModelFilter_cfi import smsModelFilter
    process.SMSModelFilter = smsModelFilter.clone(
        SusyScanTopology   = cms.string(smsModel),
        SusyScanMotherMass = cms.double(smsMotherMass),
        SusyScanLSPMass    = cms.double(smsLSPMass),
        SusyScanFracLSP    = cms.double(0.0)
        )
    
    
    #---JL
    #--for PDF sys
    process.pdfWeight = cms.EDProducer("PdfWeightProducer",
                                       PdfInfoTag = cms.untracked.InputTag("generator"),
                                       PdfSetNames = cms.untracked.vstring("cteq66.LHgrid","MSTW2008nlo68cl.LHgrid","NNPDF20_100.LHgrid")
                                       #PdfSetNames = cms.untracked.vstring("NNPDF20_100.LHgrid")
                                       )
    #---for ISR sys
    process.lastParticles = cms.EDProducer("ISRProducer")
    process.ISRsys = cms.Sequence(process.lastParticles)
    
    #---put the susy params
    process.load("RA2Classic.SUSYParams.susyparams_cfi")
    process.susyparams.Model = cms.string(SMStopol)
    #-- end JL

    ## --- Setup of TreeMaker ----------------------------------------------
    process.TFileService = cms.Service(
        "TFileService",
        fileName = cms.string("RA2Selection.root")
        )

    from RA2Classic.TreeMaker.treemaker_cfi import TreeMaker
    process.RA2TreeMaker = TreeMaker.clone(
        TreeName          = cms.string("RA2PreSelection"),
        VertexCollection  = cms.InputTag('goodVertices'),
        HT                = cms.InputTag('htPFchs'),
        HTJets            = cms.InputTag('patJetsPFchsPt50Eta25'),
        MHT               = cms.InputTag('mhtPFchs'),
        MHTJets           = cms.InputTag('patJetsPFchsPt30'),
        VarsDouble        = cms.VInputTag(cms.InputTag('WeightProducer:weight'),"susyparams:m0","susyparams:m12","susyparams:evtProcID"),
        VarsDoubleNamesInTree = cms.vstring('Weight',"massMom","massDau","procID"),
        CandidateCollections  = cms.VInputTag('patJetsPFchsPt30','patMuonsPFIDIso','patElectronsIDIso'),
        CandidateNamesInTree  = cms.vstring('Jets','PATMuonsPFIDIso','PATElectronsIDIso'),
        VarsDoubleV = cms.VInputTag("pdfWeight:cteq66","pdfWeight:MSTW2008nlo68cl","pdfWeight:NNPDF20"),
        VarsDoubleNamesInTreeV = cms.vstring("cteq66","MSTW2008nlo68cl","NNPDF20")
        )

    #process.dump   = cms.EDAnalyzer("EventContentAnalyzer")

    process.ppfchs = cms.Path(
        process.SMSModelFilter *
        process.cleanpatseq *
        process.countJetsPFchsPt50Eta25 *
        process.postPFchsJetsCounter *
        process.htPFchsFilter *
        process.postPFchsHTCounter *
        process.mhtPFchsFilter *
        process.postPFchsMHTCounter *
        process.WeightProducer *
        #--JL
        process.pdfWeight*
        process.ISRsys*
        process.susyparams*
        #--end JL
        process.RA2TreeMaker
        )

    process.schedule = cms.Schedule(process.ppfchs)
    
   
