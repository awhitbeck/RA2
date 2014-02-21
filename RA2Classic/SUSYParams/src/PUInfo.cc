// -*- C++ -*-
//
// Package:    SUSYParams
// Class:      PUInfo
// 
/**\class PUInfo  RA2Classic/SUSYParams/src/PUInfo.cc

 Description: store number of simulated pile-up interactions

*/
//
// $Id: PUInfo.cc,v 1.1 2012/11/27 21:48:31 mschrode Exp $
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

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

//
// class declaration
//

class PUInfo : public edm::EDProducer {
public:
  explicit PUInfo(const edm::ParameterSet&);
  ~PUInfo();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
private:
  const edm::InputTag pileupSummaryInfoTag_;

  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void beginRun(edm::Run&, edm::EventSetup const&);
  virtual void endRun(edm::Run&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
};


PUInfo::PUInfo(const edm::ParameterSet& iConfig)
  : pileupSummaryInfoTag_(iConfig.getParameter<edm::InputTag>("PileupSummaryInfoTag")) {
  produces<double>("Num");
}


PUInfo::~PUInfo()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
PUInfo::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  int nPU = 0;
  edm::Handle< std::vector<PileupSummaryInfo> > puInfo;
  iEvent.getByLabel(pileupSummaryInfoTag_,puInfo);
  if( puInfo.isValid() ) {
    std::vector<PileupSummaryInfo>::const_iterator puIt;
    for(puIt = puInfo->begin(); puIt != puInfo->end(); ++puIt) {
      if( puIt->getBunchCrossing() == 0 ) { // Select in-time bunch crossing
	nPU = puIt->getTrueNumInteractions();
	break;
      }
    }
  } else {
    std::cerr << "\n\n***** WARNING in PUInfo **********" << std::endl;
    std::cerr << "   No valid PileupSummaryInfo object." << std::endl;
    std::cerr << "   Using default value of 0 true pile-up interactions." << std::endl;
  }

  iEvent.put(std::auto_ptr<double>(new double(nPU)),"Num");
}

// ------------ method called once each job just before starting event loop  ------------
void 
PUInfo::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PUInfo::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
PUInfo::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
PUInfo::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
PUInfo::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
PUInfo::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PUInfo::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PUInfo);
