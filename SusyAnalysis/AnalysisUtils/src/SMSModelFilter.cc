// -*- C++ -*-
//
// Package:    SMSModelFilter
// Class:      SMSModelFilter
// 
/**\class SMSModelFilter SMSModelFilter.cc UserCode/SMSModelFilter/src/SMSModelFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/

// system include files
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// LHE Event
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"



class SMSModelFilter : public edm::EDFilter {
   public:
      explicit SMSModelFilter(const edm::ParameterSet&);
      ~SMSModelFilter();

      typedef std::vector<std::string>::const_iterator
      comments_const_iterator;

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      

  std::string   susyScanTopology_;
  double        susyScanMotherMass_;
  double        susyScanLSPMass_;
  double        susyScanFracLSP_;
  bool          debug_;

};

SMSModelFilter::SMSModelFilter(const edm::ParameterSet& iConfig) {
   //now do what ever initialization is needed
  susyScanTopology_   = iConfig.getParameter<std::string> ("SusyScanTopology");
  susyScanMotherMass_ = iConfig.getParameter<double>      ("SusyScanMotherMass");
  susyScanLSPMass_    = iConfig.getParameter<double>      ("SusyScanLSPMass");
  susyScanFracLSP_    = iConfig.getParameter<double>      ("SusyScanFracLSP");
  debug_              = iConfig.getParameter<bool>        ("Debug");
}


SMSModelFilter::~SMSModelFilter() {
}


bool SMSModelFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;

  Handle<LHEEventProduct> product;
  iEvent.getByLabel("source", product);
  
  comments_const_iterator c_begin = product->comments_begin();
  comments_const_iterator c_end = product->comments_end();
   
  double mGL  = -1.0;
  double mSQ  = -1.0;
  double mLSP = -1.0; 
  double xCHI = -1.0;

  // susy scan specific
  for( LHEEventProduct::comments_const_iterator cit=c_begin; cit!=c_end; ++cit) {
    size_t found = (*cit).find("model");
    
    //model T1_0.0_1025.0_525.0
    //model T2_550.0_425.0
    //model TGQ_0.8_900.0_850.0

    if( found != std::string::npos)   {
      if(debug_) std::cout << *cit << std::endl;
      size_t foundLength = (*cit).size();
      //found = (*cit).find("T5zz");
      found = (*cit).find(susyScanTopology_);

      std::string smaller = (*cit).substr(found+1,foundLength);
      found = smaller.find("_");
      smaller = smaller.substr(found+1,smaller.size());
      std::istringstream iss(smaller);

      if(susyScanTopology_=="T5ZZ" || susyScanTopology_=="T2bW") {
	iss >> xCHI;
	iss.clear();
	found = smaller.find("_");
	smaller = smaller.substr(found+1,smaller.size());
	iss.str(smaller);
	
	iss >> mGL;
	iss.clear();
	
	found = smaller.find("_");
	smaller = smaller.substr(found+1,smaller.size());
	iss.str(smaller);
	iss >> mLSP;
	iss.clear();

      } else if (susyScanTopology_=="T2" || susyScanTopology_=="T2tt" || susyScanTopology_=="T1" || susyScanTopology_=="T1tttt") {
	iss >> mGL;
	iss.clear();
	
	found = smaller.find("_");
	smaller = smaller.substr(found+1,smaller.size());
	iss.str(smaller);
	iss >> mLSP;
	iss.clear();
      }

    }
  }

  bool foundModel = false;
  if( mGL==susyScanMotherMass_  && mLSP==susyScanLSPMass_ ) {
    //std::cout << std::setw(9) << iEvent.id().run()<< ":" << iEvent.luminosityBlock()<<":"<<iEvent.id().event() << std::setw(9) <<  susyScanTopology_ << " mother " << mGL << " LSP "<< mLSP << std::endl;
    foundModel = true;
  }
  
   return foundModel;
}

void SMSModelFilter::beginJob() {
}

void SMSModelFilter::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(SMSModelFilter);
