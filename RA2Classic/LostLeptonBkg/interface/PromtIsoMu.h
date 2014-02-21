// -*- C++ -*-
//
// Package:    PromtIsoMu
// Class:      PromtIsoMu
// 
/**\class PromtIsoMu PromtIsoMu.cc RA2Classic/PromtIsoMu/src/PromtIsoMu.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Arne-Rasmus Draeger,,,uni-hamburg
//         Created:  Fri Nov  2 12:04:49 CET 2012
// $Id: PromtIsoMu.h,v 1.2 2012/11/19 11:39:22 adraeger Exp $
//
//


// system include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"
#include "FWCore/Utilities/interface/EDMException.h"


#include <DataFormats/PatCandidates/interface/Jet.h>
#include <DataFormats/PatCandidates/interface/MET.h>
#include <DataFormats/PatCandidates/interface/Muon.h>
#include <DataFormats/PatCandidates/interface/Electron.h>
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Common/interface/View.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "TH2.h"
#include <utility>
#include <vector>
#include "TString.h"
#include "TTree.h"

#include "DataFormats/Candidate/interface/CandMatchMap.h"




//
// class declaration
//

class PromtIsoMu : public edm::EDProducer {
   public:
      explicit PromtIsoMu(const edm::ParameterSet&);
      ~PromtIsoMu();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

	// special functions
	void MuonFound(reco::GenParticleCollection::const_iterator cand, unsigned int i);
	void ElecFound(reco::GenParticleCollection::const_iterator cand, unsigned int i);
	void TauFound(const reco::Candidate* tau);
	void resetVariables();

	// input Tags etc.
	edm::InputTag muonIDISOTag_, elecIDISOTag_;

  	edm::InputTag genTag_;

	// technical variables
	bool isPromt_;
	Float_t muonGenPt_, muonGenEta_, muonGenPhi_, elecGenPt_, elecGenEta_, elecGenPhi_;
	unsigned int nGenMu_, nGenElec_;
	Float_t muonIsoPt_, muonIsoEta_, muonIsoPhi_;
	Float_t elecIsoPt_, elecIsoEta_, elecIsoPhi_;
      // ----------member data ---------------------------
};