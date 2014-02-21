# $Id: llCS_WPJ_cfg.py,v 1.1 2012/10/26 12:42:17 adraeger Exp $







import FWCore.ParameterSet.Config as cms
process = cms.Process("RA2LostLeptonPrediction")

from RA2Classic.LostLeptonBkg.makeTreeFromPAT_cff import makeTreeFromPAT
makeTreeFromPAT(process,
                outFileName="lostLeptonCS",
                useCHSJets=True,
		invertLeptonVeto=True,
                NJetsMin=2,
                HTMin=500.,
                MHTMin=250.,
                reportEveryEvt=5000,
                testFileName=["/store/user/adraeger/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2_v2//9a64257873f37bce72f62fbdaab014a9/RA2Skim_434_1_9wn.root"],
                numProcessedEvt=100)
