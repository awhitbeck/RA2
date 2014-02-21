# $Id: runMakeTreeFromSignalScan_T1tttt-1000-0400_cfg.py,v 1.1 2012/09/28 09:48:24 mschrode Exp $
#
# Trees for SMS T1tttt (pp --> gluino-gluino)
# Cross sections from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections8TeVstopsbottom

import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.patTemplate_cfg import *
from RA2Classic.TreeMaker.makeTreeFromSignalScan_cff import makeTreeFromSignalScan
makeTreeFromSignalScan(process,
                       globalTag="START52_V11C::All",
                       smsModel="T1tttt",
                       smsMotherMass=1000,
                       smsLSPMass=400,
                       xs=0.000435488,
                       numEvtsInSample=49994,
                       lumi=5.088
                       )

# outpath needs to be removed, otherwise crab won't
# create the jobs
if hasattr(process,"outpath"):
    delattr(process,"outpath")
