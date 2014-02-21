# $Id: jetSmearing_cfi.py,v 1.2 2013/01/22 18:15:08 mschrode Exp $

import FWCore.ParameterSet.Config as cms

# Produces a new jet collection, with the jets'
# four-momentum 
jetSmearing = cms.EDProducer(
    'JetSmearing',
    Jets                        = cms.InputTag('patJets'),   # The input jet collection
    BinEdgesEta                 = cms.vdouble(0.0, 0.3, 0.5, 0.8, 1.1, 1.4, 1.7, 2.0, 2.3, 2.8, 3.2, 4.1, 5.0),
    BinEdgesPt                  = cms.vdouble(0., 20., 30., 50., 80., 120., 170., 230., 300., 380., 470., 570., 680., 800., 1000., 1300., 1700., 2200., 2800., 3500.),
    ResponseHistsFileName       = cms.string('RA2Classic/MCResolutions/data/MCJetResolutions_Summer12_DR53X_QCD_Pt_15to3000_TuneZ2star_Flat_8TeV_pythia6_withCHS_withPUReweighting_190456-208686_ABCD.root'),
    ResponseHistsNamePrefix     = cms.string('h_tot_JetAll_ResponsePt'),
    CoreScaleFactors            = cms.vdouble(),
    BinEdgesEtaCoreScaleFactors = cms.vdouble()
    )
