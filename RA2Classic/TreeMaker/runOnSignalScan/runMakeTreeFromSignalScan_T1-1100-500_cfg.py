# $Id: runMakeTreeFromSignalScan_T1-1100-500_cfg.py,v 1.1 2012/09/28 09:48:24 mschrode Exp $
#
# Trees for SMS T1 (pp --> gluino-gluino)
# Cross sections from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections8TeVgluglu

import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.patTemplate_cfg import *
from RA2Classic.TreeMaker.makeTreeFromSignalScan_cff import makeTreeFromSignalScan
makeTreeFromSignalScan(process,
                       globalTag="START52_V11C::All",
                       smsModel="T1",
                       smsMotherMass=1100,
                       smsLSPMass=500,
                       xs=0.0101744,
                       numEvtsInSample=10000,
                       lumi=5.088
                       )

# outpath needs to be removed, otherwise crab won't
# create the jobs
if hasattr(process,"outpath"):
    delattr(process,"outpath")
