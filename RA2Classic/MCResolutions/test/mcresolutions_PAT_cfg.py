#
#  SUSY-PAT configuration file adapted for RA2 workflow
#
#  PAT configuration for the SUSY group - 53X series
#  More information here:
#  https://twiki.cern.ch/twiki/bin/view/CMS/SusyPatLayer1DefV12
#
#  Run as
#  'cmsRun mcresolutions_PAT_cfg.py <command-line key>=<value>, <command-line key>=value>,...'


import FWCore.ParameterSet.Config as cms

#-- Global Parameters: Set Via Command-Line Arguments -------------------------
from RA2Classic.Utils.CommandLineParams import CommandLineParams
parameters = CommandLineParams()
#parameters.printParameters()

runOnMC          = parameters.value("is_mc",True)
dataSetName      = parameters.value("data_set","")
globalTag        = parameters.value("global_tag","")+"::All"
lumi             = parameters.value("lumi",1000.0)
doPUReweighting  = parameters.value("pu_reweighting",False)
mcResoJetTag     = parameters.value("jet_tag","patJetsPF")
mcResoFileName   = parameters.value("out_name","MCResoDefault.root")

print "*** JOB SETUP ****************************************************"
print "  is_mc          : "+str(runOnMC)
print "  data_set       : "+dataSetName
print "  global_tag     : "+globalTag
print "  lumi           : "+str(lumi)
print "  pu_reweighting : "+str(doPUReweighting)
print "  jet_tag        : "+mcResoJetTag
print "  out_name       : "+mcResoFileName
print "******************************************************************"




#-- Import the Default Process ------------------------------------------------
from PhysicsTools.PatAlgos.patTemplate_cfg import *


#-- Global Tag ----------------------------------------------------------------
process.GlobalTag.globaltag = globalTag


#-- Message Logger ------------------------------------------------------------
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(
    limit = cms.untracked.int32(10),
    reportEvery = cms.untracked.int32(1)
    )
process.MessageLogger.cerr.FwkReport.reportEvery = 5000


#-- Input Source --------------------------------------------------------------
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('/store/mc/Summer12/QCD_Pt-15to3000_TuneZ2_Flat_8TeV_pythia6/AODSIM/PU_S7_START52_V9-v1/0000/2C5FB809-F697-E111-87BB-0025901D4AF0.root')
                            )
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
process.maxEvents.input = 100


#-- SUSY PAT Specifics --------------------------------------------------------
from PhysicsTools.Configuration.SUSY_pattuple_cff import addDefaultSUSYPAT, getSUSY_pattuple_outputCommands

theJetColls = ['AK5PF']
jetMetCorr = ['L1FastJet', 'L2Relative', 'L3Absolute']

addDefaultSUSYPAT(process,
                  mcInfo=runOnMC,
                  HLTMenu='HLT',
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
process.pfIsolatedElectronsPF.isolationCut = -1.0
process.pfIsolatedMuonsPF.isolationCut = -1.0

# make loose clones of the original electron collection
process.pfRelaxedElectronsPF = process.pfIsolatedElectronsPF.clone()
process.pfRelaxedElectronsPF.isolationCut = 9999.0
process.pfElectronsFromVertexPF.dzCut = 9999.0
process.pfElectronsFromVertexPF.d0Cut = 9999.0
process.pfSelectedElectronsPF.cut = ""
process.patElectronsPF.pfElectronSource  = "pfRelaxedElectronsPF"
process.pfElectronSequencePF.replace(process.pfIsolatedElectronsPF,
                                     process.pfIsolatedElectronsPF +
                                     process.pfRelaxedElectronsPF)

# make loose clones of the original muon collection
process.pfRelaxedMuonsPF = process.pfIsolatedMuonsPF.clone()
process.pfRelaxedMuonsPF.isolationCut = 9999.0
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

# overwrite default output content
from SandBox.Skims.RA2Content_cff import getRA2PATOutput
process.out.outputCommands = getRA2PATOutput(process)
process.out.dropMetaData = cms.untracked.string('DROPPED')


#-- RA2 filtering -------------------------------------------------------------
process.load('SandBox.Skims.RA2Objects_cff')
process.load('SandBox.Skims.RA2Leptons_cff')
process.load('SandBox.Skims.RA2Selection_cff')
process.load('SandBox.Skims.RA2Cleaning_cff')
process.load("SandBox.Skims.provInfoMuons_cfi")
process.load("SandBox.Skims.provInfoElectrons_cfi")

# adjust the ra2NoiseCleaning sequence
process.trackingFailureFilter.taggingMode = False
process.beamHaloFilter.taggingMode        = False
process.inconsistentMuons.taggingMode     = False
process.greedyMuons.taggingMode           = False
process.ra2EcalTPFilter.taggingMode       = False
process.ra2EcalBEFilter.taggingMode       = False
process.hcalLaserEventFilter.taggingMode  = False
process.eeBadScFilter.taggingMode         = False
process.ecalLaserCorrFilter.taggingMode   = False

process.ra2NoiseCleaning.remove(process.eeNoiseFilter)
process.ra2NoiseCleaning.remove(process.HBHENoiseFilter)

from RecoMET.METFilters.jetIDFailureFilter_cfi import jetIDFailure
process.PBNRFilter = jetIDFailure.clone(
    JetSource = cms.InputTag(mcResoJetTag), # These are the jets for the resolution determination
    MinJetPt      = cms.double(0.0),
    taggingMode   = cms.bool(False)
    )
from RecoMET.METFilters.multiEventFilter_cfi import multiEventFilter
process.HCALLaserEvtFilterList2012 = multiEventFilter.clone(
    file        = cms.FileInPath('EventFilter/HcalRawToDigi/data/AllBadHCALLaser.txt'),
    taggingMode = cms.bool(False)
    )
process.ra2NoiseCleaning += process.PBNRFilter
process.ra2NoiseCleaning += process.HCALLaserEvtFilterList2012

process.LeptonVeto = cms.Sequence(
    process.ra2PFMuonVeto *
    process.ra2ElectronVeto
    )

# The final cleaning sequence
process.RA2CleaningSequence = cms.Sequence(
    process.susyPatDefaultSequence *
    process.ra2StdCleaning *
    process.ra2Objects * process.provInfoMuons * process.provInfoElectrons *
    process.ra2PostCleaning *
    process.LeptonVeto
    )



###############################################################################
from RA2Classic.MCResolutions.mcresolutions_cfi import MCResolutions
process.MCReso = MCResolutions.clone(
    jetTag = mcResoJetTag,
    fileName = mcResoFileName
    )
###############################################################################


###############################################################################
# Jet parton matching
#process.load("PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff")
#process.patJetPartonAssociation.jets = cms.InputTag("patJetsPF")
process.patJetPartonAssociation.coneSizeToAssociate = 0.5
###############################################################################


###############################################################################
# Weight producer
###############################################################################
from RA2Classic.WeightProducer.getWeightProducer_cff import getWeightProducer
process.WeightProducer = getWeightProducer(dataSetName)
process.WeightProducer.Lumi = lumi
if not doPUReweighting:
    process.WeightProducer.FileNamePUDataDistribution = 'NONE'
###############################################################################


###############################################################################
#-- process.outpath is set in PhysicsTools.PatAlgos.patTemplate_cfg
#-- No EDM-output file
del process.outpath
#del process.ppf
###############################################################################


process.p = cms.Path(
    process.RA2CleaningSequence *
    process.WeightProducer *
    process.MCReso
    )
