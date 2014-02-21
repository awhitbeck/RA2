
import FWCore.ParameterSet.Config as cms
# all corrections must be in % entered relative to the mean value of prediction
# allunceratinties are relative to the underlaying correction factor or the total weight in % to be entered


ra2FilterSelection = cms.EDFilter(
    'RA2FilterSelection',
    Filters           = cms.VInputTag()
)