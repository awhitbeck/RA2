# $Id: runMakeTreeFromPAT_QCDFlat_Pt15to3000_cfg.py,v 1.1 2012/09/28 09:47:47 mschrode Exp $

import FWCore.ParameterSet.Config as cms
process = cms.Process("RA2EventSelection")

from RA2Classic.TreeMaker.makeTreeFromPAT_cff import makeTreeFromPAT
makeTreeFromPAT(process,
                outFileName="RA2Selection",
                useCHSJets=True,
                NJetsMin=2,
                HTMin=500.,
                MHTMin=200.,
                reportEveryEvt=5000,
                testFileName=["/QCD_Pt-15to3000_TuneZ2_Flat_8TeV_pythia6/kheine-RA2PreSelectionOnMC_Summer12-PU_S7_START52_V9-v1_v7-956a76b9479f1eb39208c1bee6fa7dc2/USER@cms_dbs_ph_analysis_02"],
                numProcessedEvt=100)
