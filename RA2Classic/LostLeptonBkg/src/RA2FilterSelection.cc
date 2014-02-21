// -*- C++ -*-
//
// Package:    RA2FilterSelection
// Class:      RA2FilterSelection
// 
/**\class RA2FilterSelection RA2FilterSelection.cc RA2Classic/RA2FilterSelection/src/RA2FilterSelection.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Arne-Rasmus Draeger,,,uni-hamburg
//         Created:  Tue Apr  2 10:39:26 CEST 2013
// $Id: RA2FilterSelection.cc,v 1.3 2013/04/18 11:48:40 adraeger Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <string>
#include <algorithm>
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
// class declaration
//

class RA2FilterSelection : public edm::EDFilter {
   public:
      explicit RA2FilterSelection(const edm::ParameterSet&);
      ~RA2FilterSelection();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual bool beginRun(edm::Run&, edm::EventSetup const&);
      virtual bool endRun(edm::Run&, edm::EventSetup const&);
      virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      // ----------member data ---------------------------
  std::vector<edm::InputTag> filterDecisionTags_;
  std::vector<UChar_t> filterDecisions_;
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
RA2FilterSelection::RA2FilterSelection(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  filterDecisionTags_ = iConfig.getParameter< std::vector<edm::InputTag> >("Filters");


  filterDecisions_ = std::vector<UChar_t>(filterDecisionTags_.size(),0);

}


RA2FilterSelection::~RA2FilterSelection()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
RA2FilterSelection::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   bool result= true;

  for(unsigned int i = 0; i < filterDecisionTags_.size(); ++i) 
  {
    	edm::Handle<bool> dec;
    	iEvent.getByLabel(filterDecisionTags_.at(i),dec);
//	std::cout<<"RA2FilterSelection::dec.isValid():"<<dec.isValid()<<std::endl;
//	std::cout<<"RA2FilterSelection::FilterName:"<<filterDecisionTags_.at(i)<<std::endl;
    	if( dec.isValid() ) 
	{	
      		if( !*dec ) result=false;

    	}
  }
  edm::EventAuxiliary aux = iEvent.eventAuxiliary();
    UInt_t runNum_       = aux.run();
    UInt_t lumiBlockNum_ = aux.luminosityBlock();
    UInt_t evtNum_       = aux.event();
    if( runNum_==207920 && lumiBlockNum_==250 && evtNum_==367385962) result=false;
    if( runNum_==202469 && lumiBlockNum_==291 && evtNum_==406344043) result=false;
   return result;
}

// ------------ method called once each job just before starting event loop  ------------
void 
RA2FilterSelection::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
RA2FilterSelection::endJob() {
}

// ------------ method called when starting to processes a run  ------------
bool 
RA2FilterSelection::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  return true;
}

// ------------ method called when ending the processing of a run  ------------
bool 
RA2FilterSelection::endRun(edm::Run&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
bool 
RA2FilterSelection::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
bool 
RA2FilterSelection::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
RA2FilterSelection::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(RA2FilterSelection);
