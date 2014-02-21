# $Id: makeEffFromMC_cff.py,v 1.11 2013/04/04 09:20:06 adraeger Exp $
#


import FWCore.ParameterSet.Config as cms
def makeTreeFromPAT(process,
                    outFileName,
                    useCHSJets=True,
		    invertLeptonVeto=False,
                    NJetsMin=2,
                    HTMin=500.,
                    MHTMin=200.,
                    reportEveryEvt=10,
		    Global_Tag="",
                    testFileName=["/store/user/kheine/HT/RA2PreSelectionOnData_Run2012A_HT_PromptReco-v1_v5/71cce229addb17644d40a607fa20b5d7/RA2SkimsOnData_99_3_TPC.root"],
                    numProcessedEvt=1000):
    
    
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = Global_Tag
    
    ## --- Log output ------------------------------------------------------
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.MessageLogger.cerr = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
        )
    process.MessageLogger.statistics.append('cout')
    process.MessageLogger.cout = cms.untracked.PSet(
        INFO = cms.untracked.PSet(reportEvery = cms.untracked.int32(reportEveryEvt))
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

    # Filter-related selection
    process.load('RA2Classic.TreeMaker.filterSelection_cff')
    process.load('SandBox.Skims.RA2Leptons_cff')

    process.CleaningSelection = cms.Sequence(
        process.filterSelection
        )

    # Filter-related selection
#    process.load('RA2Classic.TreeMaker.filterSelection_cff')
#    from RecoMET.METFilters.jetIDFailureFilter_cfi import jetIDFailure
#    process.PBNRFilter = jetIDFailure.clone(
#        JetSource = cms.InputTag('MHTJets'),
#        MinJetPt      = cms.double(30.0),
#        taggingMode   = cms.bool(False)
#        )
#    process.filterSelection += process.PBNRFilter
#    from RecoMET.METFilters.multiEventFilter_cfi import multiEventFilter
#    process.HCALLaserEvtFilterList2012 = multiEventFilter.clone(
#        file        = cms.FileInPath('EventFilter/HcalRawToDigi/data/AllBadHCALLaser.txt'),
#        taggingMode = cms.bool(False)
#        )
 #   process.filterSelection += process.HCALLaserEvtFilterList2012




    # Produce RA2 jets
    if useCHSJets:
        process.load('RA2Classic.Utils.produceRA2JetsPFCHS_cff')
        process.ProduceRA2Jets = cms.Sequence(
            process.produceRA2JetsPFCHS
            )
    else:
        process.load('RA2Classic.Utils.produceRA2JetsAK5PF_cff')
        process.ProduceRA2Jets = cms.Sequence(
            process.produceRA2JetsAK5PF
            )
	    
	    


    # Select events with at least 'NJetsMin' of the above jets
    from PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi import countPatJets
    process.NumJetSelection = countPatJets.clone(
        src       = cms.InputTag('HTJets'),
        minNumber = cms.uint32(NJetsMin)
        )

    # HT selection
    htInputCol = 'htPF'
    if useCHSJets:
        htInputCol = 'htPFchs'

    from SandBox.Skims.RA2HT_cff import htPFFilter
    process.HTSelection = htPFFilter.clone(
        HTSource = cms.InputTag(htInputCol),
        MinHT = cms.double(HTMin)
        )

    # MHT selection
    mhtMin = 0.
    mhtInputCol = 'mhtPF'
    if useCHSJets:
        mhtInputCol = 'mhtPFchs'

    from SandBox.Skims.RA2MHT_cff import mhtPFFilter
    process.MHTSelection = mhtPFFilter.clone(
        MHTSource = cms.InputTag(mhtInputCol),
        MinMHT = cms.double(MHTMin)
        )


    ## --- Additional Filters (not tagging mode) ------------------------------
    from RecoMET.METFilters.jetIDFailureFilter_cfi import jetIDFailure
    process.PBNRFilter = jetIDFailure.clone(
        JetSource = cms.InputTag('MHTJets'),
        MinJetPt      = cms.double(30.0),
        taggingMode   = cms.bool(False)
        )
	
	
    from RecoMET.METFilters.multiEventFilter_cfi import multiEventFilter
    process.HCALLaserEvtFilterList2012 = multiEventFilter.clone(
        file        =
        cms.FileInPath('RA2Classic/LostLeptonBkg/data/HCALLaserEventList_20Nov2012-v2_HT-HTMHT.txt'),
        taggingMode = cms.bool(False)
        )
	
	
	
	

    process.AdditionalFiltersInTagMode = cms.Sequence(
        process.PBNRFilter
        )

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
 #   FilterNames.append(cms.InputTag("RA2CaloVsPFMHTFilter"))
    FilterNames.append(cms.InputTag("RA2HONoiseFilter"))






    ## --- Setup WeightProducer -------------------------------------------
    from RA2Classic.WeightProducer.getWeightProducer_cff import getWeightProducer
    process.WeightProducer = getWeightProducer(process.source.fileNames[0])
        

    ## --- Setup of TreeMaker ----------------------------------------------
    FilterNames = cms.VInputTag()  # All filters in AdditionalFiltersInTagMode
    for f in process.AdditionalFiltersInTagMode.moduleNames():
        FilterNames.append(cms.InputTag(f))

##     FilterNames.append(cms.InputTag("HBHENoiseFilterRA2","HBHENoiseFilterResult","PAT"))
##     FilterNames.append(cms.InputTag("beamHaloFilter"))
##     FilterNames.append(cms.InputTag("eeNoiseFilter"))
##     FilterNames.append(cms.InputTag("trackingFailureFilter"))
##     FilterNames.append(cms.InputTag("inconsistentMuons"))
##     FilterNames.append(cms.InputTag("greedyMuons"))
##     FilterNames.append(cms.InputTag("ra2EcalTPFilter"))
##     FilterNames.append(cms.InputTag("ra2EcalBEFilter"))
##     FilterNames.append(cms.InputTag("hcalLaserEventFilter"))
##     FilterNames.append(cms.InputTag("eeBadScFilter"))



    from RA2Classic.TreeMaker.treemaker_cfi import TreeMaker
    process.RA2TreeMaker = TreeMaker.clone(
        TreeName          = cms.string("RA2PreSelection"),
        VertexCollection  = cms.InputTag('goodVertices'),
        HT                = cms.InputTag(htInputCol),
        HTJets            = cms.InputTag('HTJets'),
        MHT               = cms.InputTag(mhtInputCol),
        MHTJets           = cms.InputTag('MHTJets'),
        VarsDouble        = cms.VInputTag(cms.InputTag('WeightProducer:weight'),cms.InputTag('LostLeptonBkgProducer:LostLeptonWeight'),cms.InputTag('LostLeptonBkgProducer:Met'),cms.InputTag('LostLeptonBkgProducer:nMu'),cms.InputTag('LostLeptonBkgProducer:MuPt'),cms.InputTag('LostLeptonBkgProducer:MuEta'),cms.InputTag('LostLeptonBkgProducer:MuPhi' ),cms.InputTag('LostLeptonBkgProducer:deltaRMuJet'),cms.InputTag('LostLeptonBkgProducer:deltaRMuMHT' ),cms.InputTag('LostLeptonBkgProducer:deltaPtMuJet') ),
        VarsDoubleNamesInTree = cms.vstring('Weight','LostLeptonWeight','Met','numberOfMuons','MuPT','MuEta','MuPhi','DeltaRMuJet','DeltaRMuMHT','DeltaPtMuJet'), 
        Filters           = FilterNames
        )
	
	
	
	# filter used to slecte the RA2 baseline important for efficiency caluclaiton
    from RA2Classic.Utils.RA2Selection_cfi import RA2Selection
    process.RA2Selector = RA2Selection.clone(
    	nJets		= cms.uint32 (3),
	HTMin		= cms.double(500),
	MHTMin		= cms.double(100),
	
	)

   ## ---- Load lost lepton moduels
   # special calo jets for delta R in efficiencies
    process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

   
#   from RA2Classic.LostLeptonBkg.llPrediction_cff import llPrediction
 #  process.lostLeptonPrediction = llPrediction()
    from RA2Classic.LostLeptonBkg.MCEffCalculator_cfi import MCEffCalculator
    process.LostLeptonBkgMCEffCalculator = MCEffCalculator.clone(
        MuonIDTag = cms.InputTag("patMuonsPFID"),
	MuonIDISOTag = cms.InputTag("patMuonsPFIDIso"),
        ElecIDTag = cms.InputTag("ra2ElectronsID"), 
	ElecIDISOTag = cms.InputTag("ra2ElectronsIDIso"),
#        ElecIDTag = cms.InputTag("patElectronsIDIso"), 
#	ElecIDISOTag = cms.InputTag("patElectronsIDIso"),
	HTTag	   = cms.InputTag(htInputCol),
 	MHTTag	   = cms.InputTag(mhtInputCol),
	CaloJetTag	= cms.InputTag('ak5CaloJetsL2L3'),
#	CaloJetTag	= cms.InputTag('cleanPatJetsAK5Calo'),
    )


    from RA2Classic.LostLeptonBkg.promtisomu_cfi import promtIsoMu
    process.promtLeptons = promtIsoMu.clone(
	MuonIDISOTag = cms.InputTag("patMuonsPFIDIso"),
	CaloJetTag = cms.InputTag("ak5CaloJetsL2L3"),
#	CaloJetTag	= cms.InputTag('cleanPatJetsAK5Calo'),
    )

 #  process.lostLeptonPrediction = llPrediction()
    from RA2Classic.LostLeptonBkg.bkglostlepton_cfi import bkglostlepton
    process.LostLeptonBkgProducer = bkglostlepton.clone(
    	HTJets		= cms.InputTag('HTJets'),
	MetTag		= cms.InputTag('patMETsPF'),
	CaloJetTag	= cms.InputTag('ak5CaloJetsL2L3'),
#	CaloJetTag	= cms.InputTag('cleanPatJetsAK5Calo'),
#	MuonTag		= cms.InputTag('promtLeptons:PromtMuon'),
	MuonTag		= cms.InputTag('patMuonsPFIDIso'),
	ElecTag         = cms.InputTag("ra2ElectronsIDIso"),
#	ElecTag         = cms.InputTag("patElectronsIDIso"),
	MTWCut		= cms.bool(True),
	EfficiencyFileName = cms.string('MCEff.root'),
	HTTag	   = cms.InputTag(htInputCol),
 	MHTTag	   = cms.InputTag(mhtInputCol),
	MetJetTagUp = cms.InputTag('jesUp:METs'),
	MetJetTagDown = cms.InputTag('jesDown:METs'),
	IsoPlots = cms.bool(True),
	TAPUncertaintiesHTNJET = cms.bool(True),
	statErrorEffmaps = cms.bool(True),
    )



 #electrons selectors for ID electrons
    from SandBox.Skims.RA2Leptons_cff import patElectronsID
    from SandBox.Skims.RA2Leptons_cff import patElectronsIDIso
    process.ra2ElectronsID = patElectronsID.clone()
    process.ra2ElectronsIDIso = patElectronsIDIso.clone()
 
 
 # JES variation for MET 
    from RA2Classic.Utils.jesUncertaintyVariation_cfi import jesUncertaintyVariation
    process.jesUp = jesUncertaintyVariation.clone(
        Jets       = cms.InputTag('MHTJets'), # The input jet collection
        JetTypeId  = cms.string('AK5PFchs'),  # Type of the input jets (to obtain the uncertainty from the data base). 
        Variation  = cms.string('Up')         # Either 'Up' or 'Dn' to produce jets with JES +/- 1 sigma, respectively
    )
    process.jesDown = jesUncertaintyVariation.clone(
        Jets       = cms.InputTag('MHTJets'), # The input jet collection
        JetTypeId  = cms.string('AK5PFchs'),  # Type of the input jets (to obtain the uncertainty from the data base). 
        Variation  = cms.string('Down')         # Either 'Up' or 'Dn' to produce jets with JES +/- 1 sigma, respectively
    )

    from RA2Classic.LostLeptonBkg.ra2Filters_cfi import ra2FilterSelection
    process.RA2Filters = ra2FilterSelection.clone(
    Filters           = FilterNames
    )




    ## --- Final paths ----------------------------------------------------

    process.dump = cms.EDAnalyzer("EventContentAnalyzer")
    
    process.WriteTree = cms.Path(
        process.CleaningSelection *
	process.HCALLaserEvtFilterList2012 *
	process.ProduceRA2Jets *
        process.NumJetSelection *
        process.HTSelection *
        process.MHTSelection *
        process.AdditionalFiltersInTagMode *
        process.WeightProducer *
	process.ra2ElectronsID *
	process.ra2ElectronsIDIso *
#	process.jesUp *
#	process.jesDown *
#	process.promtLeptons *
	process.RA2Selector *
	process.RA2Filters *
	process.ak5CaloJetsL2L3 *
 #       process.dump *
	process.LostLeptonBkgMCEffCalculator *
	process.LostLeptonBkgProducer
#	process.RA2TreeMaker 

        )
