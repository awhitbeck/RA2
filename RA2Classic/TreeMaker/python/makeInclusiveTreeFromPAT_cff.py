# $Id: $
# Write inclusive tree for long-time storate. Filter decisions are
# stored in tree.

import FWCore.ParameterSet.Config as cms

def makeTreeFromPAT(process,
                    outFileName,
                    NJetsMin=2,
                    HTMin=0.,
                    MHTMin=0.,
                    globalTag="none",
                    isData=True,
                    hltPath=[],
                    reportEveryEvt=10,
                    testFileName="",
                    numProcessedEvt=100):


    ## --- Log output ------------------------------------------------------
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.MessageLogger.cerr = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
        )
    process.MessageLogger.cout = cms.untracked.PSet(
        INFO = cms.untracked.PSet(reportEvery = cms.untracked.int32(reportEveryEvt))
        )
    process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(True)
        )


    ## --- Files to process ------------------------------------------------
    process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(numProcessedEvt)
        )
    process.source = cms.Source(
        "PoolSource",
        fileNames = cms.untracked.vstring(testFileName)
        )
        
        
    ## --- Output file -----------------------------------------------------
    process.TFileService = cms.Service(
        "TFileService",
        fileName = cms.string(outFileName+".root")
        )
    

    ## --- Selection sequences ---------------------------------------------

    # HLT
    process.load('HLTrigger.HLTfilters.hltHighLevel_cfi')
    process.hltHighLevel.HLTPaths = cms.vstring(hltPath)
    process.hltHighLevel.andOr = cms.bool(True)
    process.hltHighLevel.throw = cms.bool(False)

    process.HLTSelection = cms.Sequence(
        process.hltHighLevel
        )
    if not isData:
        print "Running over MC: removing HLT selection"
        process.HLTSelection.remove(process.hltHighLevel)
    elif not hltPath:
        print "Empty list of HLT paths: removing HLT selection"
        process.HLTSelection.remove(process.hltHighLevel)

        
    # Filter-related selection
    from RecoMET.METFilters.jetIDFailureFilter_cfi import jetIDFailure
    process.PBNRFilter = jetIDFailure.clone(
        JetSource = cms.InputTag('patJetsPF'),
        MinJetPt      = cms.double(30.0),
        taggingMode   = cms.bool(True)
        )

    from RecoMET.METFilters.multiEventFilter_cfi import multiEventFilter
    process.HCALLaserEvtFilterList2012 = multiEventFilter.clone(
        file        = cms.FileInPath('RA2Classic/AdditionalInputFiles/data/HCALLaserEventList_20Nov2012-v2_HT-HTMHT.txt'),
        taggingMode = cms.bool(True)
        )

    process.ResidualNoiseEventFilter = multiEventFilter.clone(
        file        = cms.FileInPath('RA2Classic/AdditionalInputFiles/data/NoiseEventList.txt'),
        taggingMode = cms.bool(True)
        )

    from SandBox.Skims.hoNoiseFilter_cfi import hoNoiseFilter
    process.RA2HONoiseFilter = hoNoiseFilter.clone(
        patJetsInputTag = cms.InputTag('patJetsPF'),
        jetPtMin        = cms.double(30),
        jetEtaMax       = cms.double(5),
        maxHOEfrac      = cms.double(0.4),
        taggingMode     = cms.bool(True)
        )

    # Produce RA2 jets (produces the collections HTJets and MHTJets)
    process.load('RA2Classic.Utils.produceRA2JetsPFCHS_cff')
    process.ProduceRA2Jets = cms.Sequence(
        process.produceRA2JetsPFCHS
        )

    # Select events with at least 'NJetsMin' of the above jets
    from PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi import countPatJets
    process.NumJetSelection = countPatJets.clone(
        src       = cms.InputTag('HTJets'),
        minNumber = cms.uint32(NJetsMin)
        )

    # HT selection
    htInputCol = 'htPFchs'
    from SandBox.Skims.RA2HT_cff import htPFFilter
    process.HTSelection = htPFFilter.clone(
        HTSource = cms.InputTag(htInputCol),
        MinHT = cms.double(HTMin)
        )

    # MHT selection
    mhtInputCol = 'mhtPFchs'
    from SandBox.Skims.RA2MHT_cff import mhtPFFilter
    process.MHTSelection = mhtPFFilter.clone(
        MHTSource = cms.InputTag(mhtInputCol),
        MinMHT = cms.double(MHTMin)
        )


    ## --- Compute muon-MT ------------------------------------------------
    from RA2Classic.Utils.mtProducer_cfi import mtProducer 
    process.MuonMT = mtProducer.clone(
        CandidateCollectionTag = cms.InputTag("patMuonsPFIDIso"),
        METTag                 = cms.InputTag("patMETsPF")
        )


    ## --- Setup WeightProducer -------------------------------------------
    from RA2Classic.WeightProducer.getWeightProducer_cff import getWeightProducer
    process.WeightProducer = getWeightProducer(testFileName)
    process.WeightProducer.Lumi                       = cms.double(19466)
    process.WeightProducer.PU                         = cms.int32(3) # PU S10
    process.WeightProducer.FileNamePUDataDistribution = cms.string("RA2Classic/WeightProducer/data/DataPileupHistogram_RA2Summer12_190456-208686_ABCD.root")
    
    


    ## --- Setup of TreeMaker ----------------------------------------------
    FilterNames = cms.VInputTag()
    FilterNames.append(cms.InputTag("HBHENoiseFilterRA2","HBHENoiseFilterResult","PAT"))
    FilterNames.append(cms.InputTag("beamHaloFilter"))
    FilterNames.append(cms.InputTag("trackingFailureFilter"))
    FilterNames.append(cms.InputTag("inconsistentMuons"))
    FilterNames.append(cms.InputTag("greedyMuons"))
    FilterNames.append(cms.InputTag("ra2EcalTPFilter"))
    FilterNames.append(cms.InputTag("ra2EcalBEFilter"))
    FilterNames.append(cms.InputTag("hcalLaserEventFilter"))
    FilterNames.append(cms.InputTag("ecalLaserCorrFilter"))
    FilterNames.append(cms.InputTag("eeBadScFilter"))
    FilterNames.append(cms.InputTag("PBNRFilter"))
    FilterNames.append(cms.InputTag("HCALLaserEvtFilterList2012"))
    FilterNames.append(cms.InputTag("manystripclus53X"))
    FilterNames.append(cms.InputTag("toomanystripclus53X"))
    FilterNames.append(cms.InputTag("logErrorTooManyClusters"))
    FilterNames.append(cms.InputTag("RA2CaloVsPFMHTFilter"))
    FilterNames.append(cms.InputTag("RA2HONoiseFilter"))
    FilterNames.append(cms.InputTag("ResidualNoiseEventFilter"))

    from RA2Classic.TreeMaker.treemaker_cfi import TreeMaker
    process.RA2TreeMaker = TreeMaker.clone(
        TreeName              = cms.string("RA2PreSelection"),
        VertexCollection      = cms.InputTag('goodVertices'),
        HT                    = cms.InputTag(htInputCol),
        HTJets                = cms.InputTag('HTJets'),
        MHT                   = cms.InputTag(mhtInputCol),
        MHTJets               = cms.InputTag('MHTJets'),
        VarsDouble            = cms.VInputTag(cms.InputTag('WeightProducer:weight')),
        VarsDoubleNamesInTree = cms.vstring('Weight'),
        METs                  = cms.VInputTag(mhtInputCol,'patMETsPF'),
        METNamesInTree        = cms.vstring('PFMHT','patMETsPF'),
        PatJetCollInputTag    = cms.InputTag('patJetsPF'),
        PatJetsMinPt          = cms.double(30.),
        PatJetsNameInTree     = cms.string('MHTJets'),
        CandidateCollections  = cms.VInputTag('patElectronsIDIso','patMuonsPFIDIso'),
        CandidateNamesInTree  = cms.vstring('IsoElectrons','IsoMuons'),
        VarsDoubleV           = cms.VInputTag(cms.InputTag('MuonMT')),
        VarsDoubleNamesInTreeV = cms.vstring('MTIsoMuons'),
        Filters               = FilterNames
        )



    ## --- Final paths ----------------------------------------------------

    process.WriteTree = cms.Path(
        process.HLTSelection *
        process.ProduceRA2Jets *
        process.PBNRFilter * process.HCALLaserEvtFilterList2012 * process.RA2HONoiseFilter * process.ResidualNoiseEventFilter *
        process.NumJetSelection *
        process.HTSelection *
        process.MHTSelection *
        process.MuonMT *
        process.WeightProducer *
        process.RA2TreeMaker
        )
