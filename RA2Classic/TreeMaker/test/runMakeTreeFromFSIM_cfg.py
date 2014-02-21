# $Id: $
#

import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.patTemplate_cfg import *
from RA2Classic.TreeMaker.makeTreeFromFSIM_cff import makeTreeFromFSIM
makeTreeFromFSIM(process,
                 globalTag="START53_V7G::All",
                 reportEveryEvt=100,
                 testFileName="/store/user/lveldere//MSSMsab/UkdYC7IXC4DQfQ/UkdYC7IXC4DQfQ_0_output.root",
                 numProcessedEvt=100
                 )

# outpath needs to be removed, otherwise crab won't
# create the jobs
if hasattr(process,"outpath"):
    delattr(process,"outpath")
