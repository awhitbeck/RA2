// -*- C++ -*-
//
// Package:    FilterLHEParticles
// Class:      FilterLHEParticles
// 
/**\class FilterLHEParticles FilterLHEParticles.cc SusyAnalysis/FilterLHEParticles/src/FilterLHEParticles.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Seema Sharma
//         Created:  Thu Nov  1 08:59:35 CDT 2012
// $Id: FilterLHEParticles.cc,v 1.3 2012/11/05 17:33:12 seema Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "GeneratorInterface/LHEInterface/interface/LHEEvent.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"


class FilterLHEParticles : public edm::EDFilter {
public:
  explicit FilterLHEParticles(const edm::ParameterSet&);
  ~FilterLHEParticles();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
private:
  virtual void beginJob() ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  virtual bool beginRun(edm::Run&, edm::EventSetup const&);
  virtual bool endRun(edm::Run&, edm::EventSetup const&);
  virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
  virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

  edm::InputTag lheSrc_;
  bool          debug_;
  int           filterPdgID_;
  int           status_;
  double        minPt_;
};

FilterLHEParticles::FilterLHEParticles(const edm::ParameterSet& iConfig) {

  lheSrc_      = iConfig.getParameter<edm::InputTag>("LHEProductSrc");
  debug_       = iConfig.getParameter<bool>("Debug");
  filterPdgID_ = iConfig.getParameter<int>("FilterPdgId");
  status_      = iConfig.getParameter<int>("FilterStatus");
  minPt_       = iConfig.getParameter<double>("MinimumPt");
}

FilterLHEParticles::~FilterLHEParticles() {

}

bool FilterLHEParticles::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;
  
  edm::Handle<LHEEventProduct> lheProduct;
  iEvent.getByLabel(lheSrc_, lheProduct);

  const lhef::HEPEUP hepeup = lheProduct->hepeup();
  const std::vector<lhef::HEPEUP::FiveVector> pup = hepeup.PUP;
  const std::vector<int> pdgId  = hepeup.IDUP;
  const std::vector<int> status = hepeup.ISTUP;

  bool passEvt = true;

  size_t iMax = hepeup.NUP;
  //first two entries are incoming partons
  for(size_t i = 2; i < iMax; ++i) {
    
    //if( status[i] != 1 ) continue;
    // status=1 for photon, status=2 for Z
    if( std::abs(pdgId[i]) == filterPdgID_ &&
	status[i] == status_ &&
	std::sqrt(pup[i][0]*pup[i][0] + pup[i][1]*pup[i][1]) < minPt_)
      passEvt = false;
    
    //if( idabs != 21 && (idabs<1 || idabs>6) ) continue;
    //double ptPart = sqrt( pow(hepeup_.PUP[i][0],2) + pow(hepeup_.PUP[i][1],2) );
    if(debug_){ 
      std::cout << "pdgId " << pdgId[i] << " status " << status[i] 
		<< " (px,py,pz,e,m): (" 
		<< pup[i][0] << ", " << pup[i][1]<< ", " << pup[i][2]
		<< ", " << pup[i][3] << ", " << pup[i][4]
		<< " pt " << std::sqrt(pup[i][0]*pup[i][0] + pup[i][1]*pup[i][1])
		<< " passEvt " << passEvt
		<< std::endl;
    }
  }

  return passEvt;
}

void FilterLHEParticles::beginJob() { }

void FilterLHEParticles::endJob() { }

bool FilterLHEParticles::beginRun(edm::Run&, edm::EventSetup const&) { 
  return true;
}

bool FilterLHEParticles::endRun(edm::Run&, edm::EventSetup const&) {
  return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
bool FilterLHEParticles::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&){
  return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
bool FilterLHEParticles::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&){
  return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void FilterLHEParticles::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(FilterLHEParticles);
