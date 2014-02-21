// -*- C++ -*-
//
// Package:    PATJetCollectionSubsetProducer
// Class:      PATJetCollectionSubsetProducer
// 
/**\class PATJetCollectionSubsetProducer PATJetCollectionSubsetProducer.cc RA2Classic/Utils/src/PATJetCollectionSubsetProducer.cc

Description: Select a subset of pat::Jets according to a pt and eta cut

*/
//
// Original Author:  Matthias Schroeder,,,
//         Created:  Thu Aug  2 15:11:11 CEST 2012
// $Id: PATJetCollectionSubsetProducer.cc,v 1.2 2012/12/12 19:09:29 mschrode Exp $
//
//


// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

//
// class declaration
//

class PATJetCollectionSubsetProducer : public edm::EDProducer {
public:
  explicit PATJetCollectionSubsetProducer(const edm::ParameterSet&);
  ~PATJetCollectionSubsetProducer() {};

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
  const double ptMin_;
  const double etaMax_;
};

//
// constructors and destructor
//
PATJetCollectionSubsetProducer::PATJetCollectionSubsetProducer(const edm::ParameterSet& iConfig) :
  inputJetsTag_(iConfig.getParameter<edm::InputTag>("Jets")),
  ptMin_(iConfig.getParameter<double>("PtMin")),
  etaMax_(iConfig.getParameter<double>("EtaMax"))
{
  produces< std::vector<pat::Jet> >();  
}

// ------------ method called to produce the data  ------------
void
PATJetCollectionSubsetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    // Get the jet collection from the event
  edm::Handle< edm::View<reco::Jet> > inputJetsHandle;
  iEvent.getByLabel(edm::InputTag(inputJetsTag_),inputJetsHandle);
  edm::View<reco::Jet> inputJets = *inputJetsHandle;
  
  // This will store the selected jets
  std::auto_ptr< std::vector<pat::Jet> > selectedJets(new std::vector<pat::Jet>);

  // Loop over original collection and select jets
  for(edm::View<reco::Jet>::const_iterator j = inputJets.begin();
      j != inputJets.end(); ++j) {
    if( j->pt() > ptMin_ && std::abs(j->eta()) < etaMax_ ) {
      selectedJets->push_back(*j);
    }
  }

  // Store collection of selected jets in the event
  iEvent.put(selectedJets);
}

// ------------ method called once each job just before starting event loop  ------------
void 
PATJetCollectionSubsetProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PATJetCollectionSubsetProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
PATJetCollectionSubsetProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
PATJetCollectionSubsetProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
PATJetCollectionSubsetProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
PATJetCollectionSubsetProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PATJetCollectionSubsetProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PATJetCollectionSubsetProducer);
