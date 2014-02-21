# $Id: llEff_test_cfg.py,v 1.3 2013/02/15 08:51:52 adraeger Exp $

# cmsRun ../test/llEff_test_cfg.py dataset=/store/user/adraeger/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1_V2//9a64257873f37bce72f62fbdaab014a9/RA2Skim_100_1_DnI.root, global_tag=START53_V7F::All


# Read parameters
from RA2Classic.Utils.CommandLineParams import CommandLineParams
parameters = CommandLineParams()

dataSetName = parameters.value("dataset","")
global_tag = parameters.value("global_tag","")

#global_tag="START53_V7F::All"
print "***** SETUP ************************************"
print "  dataSetName : "+dataSetName
print "  global_tag : "+global_tag
print "************************************************"



import FWCore.ParameterSet.Config as cms
process = cms.Process("RA2LostLeptonPrediction")

from RA2Classic.LostLeptonBkg.makeEffFromMC_cff import makeTreeFromPAT
makeTreeFromPAT(process,
                outFileName="LostLeptonEffFromMC",
                useCHSJets=True,
		invertLeptonVeto=True,
                NJetsMin=2,
                HTMin=00.,
                MHTMin=00.,
                reportEveryEvt=5000,
                testFileName=dataSetName,
		Global_Tag=global_tag,
                numProcessedEvt=-1)
