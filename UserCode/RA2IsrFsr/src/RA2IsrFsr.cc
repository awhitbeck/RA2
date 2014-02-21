// -*- C++ -*-
//
// Package:    RA2IsrFsr
// Class:      RA2IsrFsr
// 
/**\class RA2IsrFsr RA2IsrFsr.cc UserCode/RA2IsrFsr/src/RA2IsrFsr.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Stephen Mrenna
//         Created:  Thu Feb 17 11:35:42 CST 2011
// $Id: RA2IsrFsr.cc,v 1.1 2011/02/17 17:47:22 mrenna Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
// class declaration
//

class RA2IsrFsr : public edm::EDAnalyzer {
   public:
      explicit RA2IsrFsr(const edm::ParameterSet&);
      ~RA2IsrFsr();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
RA2IsrFsr::RA2IsrFsr(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

}


RA2IsrFsr::~RA2IsrFsr()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
RA2IsrFsr::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;



#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
RA2IsrFsr::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
RA2IsrFsr::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(RA2IsrFsr);
