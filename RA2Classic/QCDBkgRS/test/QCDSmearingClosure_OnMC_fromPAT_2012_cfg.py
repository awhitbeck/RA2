import FWCore.ParameterSet.Config as cms

process = cms.Process("RA2QCDSmearingClosure")

###############################################################################
#-- Message Logger ------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.destinations = cms.untracked.vstring('output.log')
process.MessageLogger.suppressWarning = cms.untracked.vstring('TriggerSelectionLow','TriggerSelectionHigh','TriggerSelectionSignal')
###############################################################################

## Options and Output Report
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
       '/store/user/kheine/QCD_Pt-15to3000_TuneZ2_Flat_8TeV_pythia6/RA2PreSelectionOnMC_Summer12-PU_S7_START52_V9-v1_v7/956a76b9479f1eb39208c1bee6fa7dc2/RA2SkimsOnMC_253_2_ZQN.root',
       '/store/user/kheine/QCD_Pt-15to3000_TuneZ2_Flat_8TeV_pythia6/RA2PreSelectionOnMC_Summer12-PU_S7_START52_V9-v1_v7/956a76b9479f1eb39208c1bee6fa7dc2/RA2SkimsOnMC_309_2_dS7.root',
       '/store/user/kheine/QCD_Pt-15to3000_TuneZ2_Flat_8TeV_pythia6/RA2PreSelectionOnMC_Summer12-PU_S7_START52_V9-v1_v7/956a76b9479f1eb39208c1bee6fa7dc2/RA2SkimsOnMC_134_2_SGe.root',
       '/store/user/kheine/QCD_Pt-15to3000_TuneZ2_Flat_8TeV_pythia6/RA2PreSelectionOnMC_Summer12-PU_S7_START52_V9-v1_v7/956a76b9479f1eb39208c1bee6fa7dc2/RA2SkimsOnMC_116_2_3Ag.root',
       '/store/user/kheine/QCD_Pt-15to3000_TuneZ2_Flat_8TeV_pythia6/RA2PreSelectionOnMC_Summer12-PU_S7_START52_V9-v1_v7/956a76b9479f1eb39208c1bee6fa7dc2/RA2SkimsOnMC_38_2_oB6.root',
       '/store/user/kheine/QCD_Pt-15to3000_TuneZ2_Flat_8TeV_pythia6/RA2PreSelectionOnMC_Summer12-PU_S7_START52_V9-v1_v7/956a76b9479f1eb39208c1bee6fa7dc2/RA2SkimsOnMC_105_1_lKH.root',
       '/store/user/kheine/QCD_Pt-15to3000_TuneZ2_Flat_8TeV_pythia6/RA2PreSelectionOnMC_Summer12-PU_S7_START52_V9-v1_v7/956a76b9479f1eb39208c1bee6fa7dc2/RA2SkimsOnMC_214_1_GQO.root',
       '/store/user/kheine/QCD_Pt-15to3000_TuneZ2_Flat_8TeV_pythia6/RA2PreSelectionOnMC_Summer12-PU_S7_START52_V9-v1_v7/956a76b9479f1eb39208c1bee6fa7dc2/RA2SkimsOnMC_388_1_KBb.root',       
 	)
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 1000 ) )
###############################################################################
## Global tags and geometry
# default configuration with frontier conditions
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START52_V11C::All')
###############################################################################

###############################################################################
process.TFileService = cms.Service("TFileService",fileName = cms.string("QCDSmearingClosure_OnMC.root") )
###############################################################################

###############################################################################
process.load("RA2Classic.QCDBkgRS.qcdbkgrs_cfi")
###############################################################################

###############################################################################
# Rebalancing and Smearing configuration
###############################################################################
#process.QCDfromSmearing.SmearingFile = '/afs/naf.desy.de/user/k/kriheine/Resolution/MCJetResolution_Summer12_QCD_Pt_15to3000_TuneZ2_Flat_8TeV_pythia6_withCHS_withPUReweighting_pixelcorr.root'
#process.QCDfromSmearing.SmearingFile = '/afs/naf.desy.de/user/k/kriheine/Resolution/MCJetResolution_Summer12_QCD_Pt_15to3000_TuneZ2_Flat_8TeV_pythia6_withoutCHS_withoutPUReweighting.root'
#process.QCDfromSmearing.SmearingFile = '/afs/naf.desy.de/user/k/kriheine/Resolution/MCJetResolution_Summer12_QCD_Pt_15to3000_TuneZ2_Flat_8TeV_pythia6_withCHS_withoutPUReweighting_fineLowPtBins.root'
#process.QCDfromSmearing.SmearingFile = '/afs/naf.desy.de/user/k/kriheine/Resolution/MCJetResolution_Summer12_QCD_Pt_15to3000_TuneZ2Star_Flat_8TeV_pythia6_NoPU_withCHS.root'
process.QCDfromSmearing.SmearingFile = '/afs/naf.desy.de/user/k/kriheine/Resolution/MCJetResolution_Summer12_QCD_Pt_15to3000_TuneZ2star_Flat_8TeV_pythia6_withCHS_withPUReweighting.root'

process.QCDfromSmearing.BProbabilityFile = '/afs/naf.desy.de/user/k/kriheine/Resolution/BJetProbabilityMC.root'
process.QCDfromSmearing.jetCollection = 'patJetsPF' 
#process.QCDfromSmearing.jetCollection = 'patJetsAK5PF'
process.QCDfromSmearing.uncertaintyName = '' 
#process.QCDfromSmearing.InputHisto1_HF = 'h_b_Jet1_ResponsePt'
#process.QCDfromSmearing.InputHisto2_HF = 'h_b_Jet2_ResponsePt'
#process.QCDfromSmearing.InputHisto3p_HF = 'h_b_Jet3p_ResponsePt'
#process.QCDfromSmearing.InputHisto1_NoHF = 'h_nob_Jet1_ResponsePt'
#process.QCDfromSmearing.InputHisto2_NoHF = 'h_nob_Jet2_ResponsePt'
#process.QCDfromSmearing.InputHisto3p_NoHF = 'h_nob_Jet3p_ResponsePt'
process.QCDfromSmearing.InputHisto1_HF = 'h_tot_JetAll_ResponsePt'
process.QCDfromSmearing.InputHisto2_HF = 'h_tot_JetAll_ResponsePt'
process.QCDfromSmearing.InputHisto3p_HF = 'h_tot_JetAll_ResponsePt'
process.QCDfromSmearing.InputHisto1_NoHF = 'h_tot_JetAll_ResponsePt'
process.QCDfromSmearing.InputHisto2_NoHF = 'h_tot_JetAll_ResponsePt'
process.QCDfromSmearing.InputHisto3p_NoHF = 'h_tot_JetAll_ResponsePt'
#process.QCDfromSmearing.InputHisto1_HF = 'h_b_JetAll_ResponsePt'
#process.QCDfromSmearing.InputHisto2_HF = 'h_b_JetAll_ResponsePt'
#process.QCDfromSmearing.InputHisto3p_HF = 'h_b_JetAll_ResponsePt'
#process.QCDfromSmearing.InputHisto1_NoHF = 'h_nob_JetAll_ResponsePt'
#process.QCDfromSmearing.InputHisto2_NoHF = 'h_nob_JetAll_ResponsePt'
#process.QCDfromSmearing.InputHisto3p_NoHF = 'h_nob_JetAll_ResponsePt'
process.QCDfromSmearing.RebalanceCorrectionFile = '/afs/naf.desy.de/user/k/kriheine/Resolution/RebalanceCorrection_withPUReweighting_pt10.root'
process.QCDfromSmearing.NRebin = 1
process.QCDfromSmearing.NJets = 2
process.QCDfromSmearing.SmearCollection = 'Reco'
process.QCDfromSmearing.PtBinEdges_scaling = cms.vdouble(0., 7000.)
process.QCDfromSmearing.EtaBinEdges_scaling = cms.vdouble(0.0, 5.0)
process.QCDfromSmearing.AdditionalSmearing = cms.vdouble(1.0)
process.QCDfromSmearing.LowerTailScaling = cms.vdouble(1.0)
process.QCDfromSmearing.UpperTailScaling = cms.vdouble(1.0)
process.QCDfromSmearing.SmearedJetPt = 0.
process.QCDfromSmearing.RebalanceJetPt = 10.
process.QCDfromSmearing.RebalanceMode = 'MHThigh'
process.QCDfromSmearing.weightName = 'weightProducer:weight'
process.QCDfromSmearing.ControlPlots = True
process.QCDfromSmearing.Ntries = 100
process.QCDfromSmearing.cleverPrescaleTreating = False
process.QCDfromSmearing.useRebalanceCorrectionFactors = True 
process.QCDfromSmearing.MHTcut_low = cms.double(200.)
process.QCDfromSmearing.MHTcut_medium = cms.double(350.)
process.QCDfromSmearing.MHTcut_high = cms.double(500.)
process.QCDfromSmearing.HTcut_low = cms.double(500.)
process.QCDfromSmearing.HTcut_medium = cms.double(800.)
process.QCDfromSmearing.HTcut_high = cms.double(1000.)
process.QCDfromSmearing.HTcut_veryhigh = cms.double(1200.)
process.QCDfromSmearing.HTcut_extremehigh = cms.double(1400.)
###############################################################################

###############################################################################
# Weight producer
###############################################################################
process.load("RA2Classic.WeightProducer.weightProducer_cfi")
process.weightProducer.weightName	= 'weight'

## QCD_Pt-15to3000_TuneZ2_Flat_8TeV_pythia6
#process.weightProducer.Method			= 'PtHat'
#process.weightProducer.Exponent		= -4.5
#process.weightProducer.XS				= 2.99913994e+10
#process.weightProducer.NumberEvts	= 9998154
#process.weightProducer.Lumi			= 1000
#process.weightProducer.weight			= -1.0

## QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6
process.weightProducer.Method          = 'PtHat'
process.weightProducer.Exponent        = -4.5
process.weightProducer.XS              = 2.99815997e+10
process.weightProducer.NumberEvts      = 9993618
process.weightProducer.Lumi            = 1000
process.weightProducer.weight          = -1.0

#process.weightProducer.Method		= 'BinnedPythia'
#process.weightProducer.Lumi			= 1000

#process.weightProducer.FileNamePUDataDistribution = 'NONE'
process.weightProducer.FileNamePUDataDistribution = 'RA2Classic/AdditionalInputFiles/data/DataPileupHistogram_RA2Summer12_190456-196531_8TeV_PromptReco_WOLowPU_pixelcorr.root'
process.weightProducer.PU = 2 # 0==Flat10, 1==Fall11, 2==Summer12                        
###############################################################################

###############################################################################
process.load("SandBox.Skims.filterBoolean_cfi")
process.RA2_HBHENoiseFilterRA2    = process.booleanFilter.clone()
process.RA2_HBHENoiseFilterRA2.ResultSource = cms.InputTag("HBHENoiseFilterRA2","HBHENoiseFilterResult","PAT")
process.RA2_beamHaloFilter        = process.booleanFilter.clone()
process.RA2_beamHaloFilter.ResultSource = cms.InputTag("beamHaloFilter")
process.RA2_eeNoiseFilter         = process.booleanFilter.clone()
process.RA2_eeNoiseFilter.ResultSource = cms.InputTag("eeNoiseFilter")
process.RA2_trackingFailureFilter = process.booleanFilter.clone()
process.RA2_trackingFailureFilter.ResultSource = cms.InputTag("trackingFailureFilter")
process.RA2_inconsistentMuons     = process.booleanFilter.clone()
process.RA2_inconsistentMuons.ResultSource = cms.InputTag("inconsistentMuons")
process.RA2_greedyMuons           = process.booleanFilter.clone()
process.RA2_greedyMuons.ResultSource = cms.InputTag("greedyMuons")
process.RA2_EcalTPFilter          = process.booleanFilter.clone()
process.RA2_EcalTPFilter.ResultSource = cms.InputTag("ra2EcalTPFilter")
process.RA2_EcalBEFilter          = process.booleanFilter.clone()
process.RA2_EcalBEFilter.ResultSource = cms.InputTag("ra2EcalBEFilter")
process.HcalLaserEventFilter      = process.booleanFilter.clone()
process.HcalLaserEventFilter.ResultSource = cms.InputTag("hcalLaserEventFilter")
process.EEBadScFilter             = process.booleanFilter.clone()
process.EEBadScFilter.ResultSource = cms.InputTag("eeBadScFilter")
###############################################################################

###############################################################################
process.load('SandBox.Skims.RA2HT_cff')
process.load('SandBox.Skims.RA2Leptons_cff')
###############################################################################

###############################################################################
## --- Setup of TreeMaker ----------------------------------------------
from RA2Classic.TreeMaker.treemaker_cfi import TreeMaker
process.RA2TreeMaker = TreeMaker.clone(
    TreeName         = cms.string("RA2PreSelection"),
    VarsDouble       = cms.VInputTag(cms.InputTag('weightProducer:weight')),
    VarsDoubleNamesInTree = cms.vstring('Weight'),
    VertexCollection = cms.InputTag('goodVertices'),
 #   HT               = cms.InputTag('htPF'),
    HT               = cms.InputTag('htPFchs'),
    HTJets           = cms.InputTag('HTJets'),
 #   MHT              = cms.InputTag('mhtPF'),
    MHT              = cms.InputTag('mhtPFchs'),
    MHTJets          = cms.InputTag('MHTJets')
    )
process.load('RA2Classic.Utils.produceRA2JetsPFCHS_cff')
#process.load('RA2Classic.Utils.produceRA2JetsAK5PF_cff')
###############################################################################

###############################################################################
process.dump   = cms.EDAnalyzer("EventContentAnalyzer")
###############################################################################

process.prediction = cms.Path(
#	process.dump *
#   process.htPFFilter *    # to be used for standard jets
   process.htPFchsFilter *  # to be used for chs jets
   process.RA2_HBHENoiseFilterRA2 *
   process.RA2_beamHaloFilter *
   process.RA2_eeNoiseFilter *
   process.RA2_trackingFailureFilter *
   process.RA2_inconsistentMuons *
   process.RA2_greedyMuons *
   process.RA2_EcalTPFilter *
   process.RA2_EcalBEFilter *
   process.HcalLaserEventFilter *
   process.EEBadScFilter *
	process.weightProducer *
   process.ra2PFMuonVeto *
   process.ra2ElectronVeto *
	process.QCDfromSmearing 
)

process.mc = cms.Path(
 #  process.htPFFilter *    # to be used for standard jets
   process.htPFchsFilter *     # to be used for chs jets 
   process.RA2_HBHENoiseFilterRA2 *
   process.RA2_beamHaloFilter *
   process.RA2_eeNoiseFilter *
   process.RA2_trackingFailureFilter *
   process.RA2_inconsistentMuons *
   process.RA2_greedyMuons *
   process.RA2_EcalTPFilter *
   process.RA2_EcalBEFilter *
   process.HcalLaserEventFilter *
   process.EEBadScFilter *
	process.weightProducer *
   process.ra2PFMuonVeto *
   process.ra2ElectronVeto * 
   process.produceRA2JetsPFCHS *
 #  process.produceRA2JetsAK5PF *
   process.RA2TreeMaker   
)
