// -*- C++ -*-
//
// Package:    SUSYParams
// Class:      AdditionalJetInfo
// 
/**\class AdditionalJetInfo  RA2Classic/SUSYParams/src/JetArea.cc

 Description: store additional jet-related variables
  - jet area for L1 corrections
  - neutral hadronic and em energy fractions for PBNR filter RecoMET/METFilters/plugins/JetIDFailureFilter.cc
 Requires in addition the jet 4-vectors
*/
//
// $Id: AdditionalJetInfo.cc,v 1.2 2013/01/24 19:33:40 seema Exp $
//
//


// system include files
#include <memory>
#include <iostream>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
//
// class declaration
//

class AdditionalJetInfo : public edm::EDProducer {
public:
  explicit AdditionalJetInfo(const edm::ParameterSet&);
  ~AdditionalJetInfo();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
private:
  const edm::InputTag jetCollTag_;
  std::string   btagname_;

  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void beginRun(edm::Run&, edm::EventSetup const&);
  virtual void endRun(edm::Run&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
};


AdditionalJetInfo::AdditionalJetInfo(const edm::ParameterSet& iConfig)
  : jetCollTag_(iConfig.getParameter<edm::InputTag>("JetSource")) {
  btagname_         = iConfig.getParameter<std::string>  ("bTagName");
  produces< std::vector<double> >("Area");
  produces< std::vector<double> >("NeutHadF");
  produces< std::vector<double> >("NeutEmF");
  produces< std::vector<double> >("BTagCSV");
}


AdditionalJetInfo::~AdditionalJetInfo()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
AdditionalJetInfo::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle< edm::View<pat::Jet> > jets;
  iEvent.getByLabel(jetCollTag_,jets);
  if( jets.isValid() ) {
    std::auto_ptr< std::vector<double> > areas(new std::vector<double>(jets->size(),0.));
    std::auto_ptr< std::vector<double> > nhfs(new std::vector<double>(jets->size(),0.));
    std::auto_ptr< std::vector<double> > nefs(new std::vector<double>(jets->size(),0.));
    std::auto_ptr< std::vector<double> > btag(new std::vector<double>(jets->size(),0.));
    
    unsigned int jidx = 0;
    for(edm::View<pat::Jet>::const_iterator j = jets->begin();
	j != jets->end(); ++j, ++jidx) {
      if(j->isPFJet()) {
	areas->at(jidx) = j->jetArea();
	// neutral hadron fraction already calculated on uncorrected jet energy
	nhfs->at(jidx) = j->neutralHadronEnergyFraction();
	// neutral emf, uncorrected jet energy
	nefs->at(jidx) = j->photonEnergyFraction()/j->jecFactor(0);
	// btag discriminator value
	btag->at(jidx) = j->bDiscriminator(btagname_.c_str());
      }
    }
    iEvent.put(areas,"Area");
    iEvent.put(nhfs,"NeutHadF");
    iEvent.put(nefs,"NeutEmF");
    iEvent.put(btag,"BTagCSV");
  }
}

// ------------ method called once each job just before starting event loop  ------------
void 
AdditionalJetInfo::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
AdditionalJetInfo::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
AdditionalJetInfo::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
AdditionalJetInfo::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
AdditionalJetInfo::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
AdditionalJetInfo::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
AdditionalJetInfo::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(AdditionalJetInfo);
