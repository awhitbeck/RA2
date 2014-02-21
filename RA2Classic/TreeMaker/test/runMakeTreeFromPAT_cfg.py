# $Id: runMakeTreeFromPAT_cfg.py,v 1.12 2013/02/28 09:14:06 mschrode Exp $
#
# Expects a file name as argument e.g.
# cmsRun runMakeTreeFromPAT_cfg.py dataset=/store/user/mschrode/HT/RA2PreSelection_Run2012A-13Jul2012-v1_V4/21a074f94cdbe7cfbeeb19be46b40a6a/RA2Skim_9_1_h6A.root, is_data=True, global_tag=FT_53_V6C_AN3

#/store/user/adraeger/TT_CT10_TuneZ2star_8TeV-powheg-tauola/RA2PreSelection_Summer12_DR53X-PU_S10_START53_V7A-v2_V3//ea5cb269027308a5056cc4b44ea7e9f3/RA2Skim_1028_1_1VP.root

# Read parameters
from RA2Classic.Utils.CommandLineParams import CommandLineParams
parameters = CommandLineParams()

dataset      = parameters.value("dataset","")
njets_min    = parameters.value("njets_min",2)
ht_min       = parameters.value("ht_min",0)
mht_min      = parameters.value("mht_min",0)
globaltag    = parameters.value("global_tag","none")+"::All"
is_data      = parameters.value("is_data",True)


print "***** SETUP ************************************"
print "      dataset : "+dataset
print "      is_data : "+str(is_data)
print "    njets_min : "+str(njets_min)
print "       ht_min : "+str(ht_min)
print "      mht_min : "+str(mht_min)
print "    globaltag : "+globaltag
print "************************************************"

# The process needs to be defined AFTER reading sys.argv,
# otherwise edmConfigHash fails
import FWCore.ParameterSet.Config as cms
process = cms.Process("RA2EventSelection")

#from RA2Classic.TreeMaker.makeTreeFromPAT_cff import makeTreeFromPAT
from RA2Classic.TreeMaker.makeInclusiveTreeFromPAT_cff import makeTreeFromPAT
makeTreeFromPAT(process,
                outFileName="RA2Selection",
                NJetsMin=njets_min,
                HTMin=ht_min,
                MHTMin=mht_min,
                globalTag=globaltag,
                isData=is_data,
                hltPath=['HLT_PFHT350_PFMET100_v*','HLT_PFNoPUHT350_PFMET100_v*'],
                reportEveryEvt=100,
                testFileName=dataset,
                numProcessedEvt=100)
