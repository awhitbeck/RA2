# $Id: puWeightProducer_cfi.py,v 1.1 2012/08/08 09:32:47 mschrode Exp $
#
# Example: stores a double variable 'puWeightProducer:weight' in the event
# which can be used to reweight the sample according to the
# specified pile-up scenario.

import FWCore.ParameterSet.Config as cms

from RA2Classic.WeightProducer.weightProducer_cfi import weightProducer

puWeightProducer = weightProducer.clone(
	# Data PU distribution. If a file name is specified,
	# a multiplicative PU weight factor is applied.
	FileNamePUDataDistribution = cms.string("RA2Classic/AdditionalInputFiles/DataPileupHistogram_RA2Summer12_190456-196531_8TeV_PromptReco_WOLowPU.root"),   
	## use this for different PU distributions 0 Flat10 PU; 1 for Fall11; 2 for Summer12
	PU = cms.int32(2)
)
