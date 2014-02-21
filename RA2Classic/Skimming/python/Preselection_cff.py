# $Id: Preselection_cff.py,v 1.5 2012/12/27 16:36:42 mschrode Exp $
#
# Process setup for RA2 skims

import FWCore.ParameterSet.Config as cms

def runRA2Preselection(process,
                       globalTag,
                       isData=True,
                       reportEveryEvt=5000,
                       testFileName="",
                       numProcessedEvt=10):


    print "*** SETUP OF 'runRA2Preselection' ********************************"
    if isData :
        print " Run on data"
    else:
        print " Run on MC"
    print " GlobalTag = "+globalTag
    print "******************************************************************"



    #-- Meta data to be logged in DBS ---------------------------------------------
    process.configurationMetadata = cms.untracked.PSet(
        version = cms.untracked.string('$Revision: 1.5 $'),
        name = cms.untracked.string('$Source: /local/reps/CMSSW/UserCode/kheine/RA2Classic/Skimming/python/Preselection_cff.py,v $'),
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

    if isData: hltMenu = 'HLT'
    else: hltMenu = 'REDIGI'

    theJetColls = ['AK5PF']

    jetMetCorr = ['L1FastJet', 'L2Relative', 'L3Absolute']
    if isData: jetMetCorr.append('L2L3Residual')

    process.GlobalTag.globaltag = globalTag

    addDefaultSUSYPAT(process,
                      mcInfo=not isData,
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
    process.load('SandBox.Skims.RA2Objects_cff')
    process.load('SandBox.Skims.RA2Selection_cff')
    process.load('SandBox.Skims.RA2Cleaning_cff')

    ## Modify selection and filters for skims
    process.countJetsPFchsPt50Eta25.minNumber = cms.uint32(2)
    process.htPFchsFilter.MinHT               = cms.double(350)

    ## Set filters in tagging mode; stores filter decision as boolean:
    ## - true  : event passes filter
    ## - false : event gets rejected by filter
    process.eeNoiseFilter.taggingMode         = True
    process.trackingFailureFilter.taggingMode = True
    process.beamHaloFilter.taggingMode        = True
    process.inconsistentMuons.taggingMode     = True
    process.greedyMuons.taggingMode           = True
    process.ra2EcalTPFilter.taggingMode       = True
    process.ra2EcalBEFilter.taggingMode       = True
    process.hcalLaserEventFilter.taggingMode  = True
    process.eeBadScFilter.taggingMode         = True
    process.ecalLaserCorrFilter.taggingMode   = True

    # Rejects the event, replaced by HBHENoiseFilterRA2 that
    # stores booleans
    process.ra2NoiseCleaning.remove(process.HBHENoiseFilter)

    # Tracking-POG Cleaning (booleans have inverted meaning
    # compared to the other filters!)
    process.ra2PostCleaning += process.trackingPOGCleaning
    
    process.load("SandBox.Skims.provInfoMuons_cfi")
    process.load("SandBox.Skims.provInfoElectrons_cfi")

    process.cleanpatseq = cms.Sequence(
        process.susyPatDefaultSequence *
        process.ra2StdCleaning *
        process.ra2Objects *
        process.provInfoMuons *
        process.provInfoElectrons *
        process.ra2PostCleaning
        )

    process.dump   = cms.EDAnalyzer("EventContentAnalyzer")

    process.ppfchs = cms.Path(
        process.cleanpatseq *
        process.countJetsPFchsPt50Eta25 *
        process.htPFchsFilter
        )


    #-- Output module configuration -----------------------------------------------

    process.out.fileName = cms.untracked.string('RA2Skim.root')
    process.out.SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('ppfchs')
        )
    process.outpath = cms.EndPath( process.out )
    
   
