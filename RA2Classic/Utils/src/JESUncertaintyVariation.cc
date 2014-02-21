// -*- C++ -*-
//
// Package:    JESUncertaintyVariation
// Class:      JESUncertaintyVariation
// 
/**\class JESUncertaintyVariation JESUncertaintyVariation.cc RA2Classic/Utils/src/JESUncertaintyVariation.cc

Description: Vary jet pt by its uncertainty and produce new jet collection.

*/
//
// Original Author:  Matthias Schroeder,,,
//         Created:  Mon Sep 17 16:52:55 CEST 2012
// $Id: JESUncertaintyVariation.cc,v 1.2 2012/12/12 19:08:22 mschrode Exp $
//
//


// system include files
#include <memory>
#include <string>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Selector.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/Utils/interface/PtComparator.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

//
// class declaration
//

class JESUncertaintyVariation : public edm::EDProducer {
public:
  explicit JESUncertaintyVariation(const edm::ParameterSet&);
  ~JESUncertaintyVariation() {};

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
      
  virtual void beginRun(edm::Run&, edm::EventSetup const&);
  virtual void endRun(edm::Run&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);


  // ----------member data ---------------------------
  const edm::InputTag inputJetsTag_;
  const std::string jetTypeId_;
  const edm::InputTag inputMetsTag_;
  const std::string variation_;

  JetCorrectionUncertainty* jecUnc_;
};

//
// constructors and destructor
//
JESUncertaintyVariation::JESUncertaintyVariation(const edm::ParameterSet& iConfig) :
  inputJetsTag_(iConfig.getParameter<edm::InputTag>("Jets")),
  jetTypeId_(iConfig.getParameter<std::string>("JetTypeId")),
  inputMetsTag_(iConfig.getParameter<edm::InputTag>("METs")),
  variation_( (iConfig.getParameter<std::string>("Variation")=="DN" ||
	       iConfig.getParameter<std::string>("Variation")=="Dn" ||
	       iConfig.getParameter<std::string>("Variation")=="dn")  ?
	      "Dn" : "Up" )
{
  jecUnc_ = 0;			// Initialized in first event loop because needs information from the event

  produces< std::vector<pat::Jet> >("Jets");
  produces< std::vector<pat::MET> >("METs");
}


// ------------ method called to produce the data  ------------
void
JESUncertaintyVariation::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // Initialize uncertainty as described at
  // https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#JetCorUncertainties
  if( jecUnc_ == 0 ) {
    edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
    iSetup.get< JetCorrectionsRecord >().get(jetTypeId_,JetCorParColl); 
    JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
    jecUnc_ = new JetCorrectionUncertainty(JetCorPar);
  }
  
  // Get the jet collection from the event
  edm::Handle< edm::View<pat::Jet> > inputJets;
  iEvent.getByLabel(inputJetsTag_,inputJets);

  // Get the vector of JEC-corrected (type 1) METs
  edm::Handle< edm::View<reco::MET> > inputMets;
  iEvent.getByLabel(inputMetsTag_,inputMets);

  // Collection of varied jets
  std::auto_ptr< std::vector<pat::Jet> > varJets(new std::vector<pat::Jet>); 

  //Vectorial difference between normal jets and modified jets, for MET corr.
  reco::MET::LorentzVector change(0,0,0,0);

  // Loop over jets and compute varied jets and track difference
  for(edm::View<pat::Jet>::const_iterator itInJet = inputJets->begin();
       itInJet != inputJets->end(); ++itInJet) {

    // Retrieve uncertainty of the current jet's JEC
    // IMPORTANT: the uncertainty is a function of the CORRECTED pt
    jecUnc_->setJetPt(itInJet->pt());
    jecUnc_->setJetEta(itInJet->eta());
    double correction = jecUnc_->getUncertainty(true);
    if( variation_ == "Dn" ) correction *= -1.0;
    
    // Create varied jet
    pat::Jet varJet( *itInJet );
    varJet.setP4( itInJet->p4() * (1.0 + correction) );
    varJets->push_back(varJet);

    //keep track of the change, in order to correct JEC-corrected MET, too.
    change += ( varJet.p4() - itInJet->p4() );

    //    std::cout << "(" << itInJet->pt() << "," << itInJet->eta() << ") --> (" << varJet.pt() << "," << varJet.eta() << ")" << std::endl;
  }

  // Sort varied jets in pt
  GreaterByPt<pat::Jet> pTComparator;
  std::sort(varJets->begin(),varJets->end(),pTComparator);

  // Put varied jets into event
  iEvent.put(varJets,"Jets");

  //recalculate MET taking intoaccount the new jet-collection for type 1:
  pat::MET inMet( *(inputMets->begin()));
  change = reco::MET::LorentzVector(change.x()+inMet.p4().x(),
				     change.y()+inMet.p4().y(),
				     0,
				     sqrt( (change.x()+inMet.p4().x())*(change.x()+inMet.p4().x()) + (change.y()+inMet.p4().y())*(change.y()+inMet.p4().y()) ) );
  pat::MET varMET( reco::MET( change.pt(), 
			      inMet.mEtCorr(),
			      change,
			      inMet.vertex() ) );
  
  // put mod. MET into event
  std::auto_ptr< std::vector<pat::MET> > varMETs(new std::vector<pat::MET>);
  varMETs->push_back( varMET );
  iEvent.put(varMETs,"METs");
}

// ------------ method called once each job just before starting event loop  ------------
void 
JESUncertaintyVariation::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
JESUncertaintyVariation::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
JESUncertaintyVariation::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
JESUncertaintyVariation::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
JESUncertaintyVariation::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
JESUncertaintyVariation::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
JESUncertaintyVariation::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(JESUncertaintyVariation);
