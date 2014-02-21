// -*- C++ -*-
//
// Package:    CompositeProducer
// Class:      CompositeProducer
// 
/**\class CompositeProducer CompositeProducer.cc Analysis/CompositeProducer/src/CompositeProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  "Salvatore Rappoccio"
// Modified by    :  "Stephen Mrenna "
//         Created:  Mon Sep 28 12:53:57 CDT 2009
// $Id: CompositeProducer.cc,v 1.1 2011/02/17 17:47:50 mrenna Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "PhysicsTools/CandUtils/interface/AddFourMomenta.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/LorentzVector.h"


#include <TLorentzVector.h>


#include <vector>

//
// class declaration
//

class CompositeProducer : public edm::EDProducer {
   public:
      explicit CompositeProducer(const edm::ParameterSet&);
      ~CompositeProducer();

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------

  edm::InputTag    dauSrc_;
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
CompositeProducer::CompositeProducer(const edm::ParameterSet& iConfig) :
  dauSrc_ ( iConfig.getParameter<edm::InputTag>("dauSrc") )
{
  produces<std::vector<reco::CompositeCandidate> > ();
  
}


CompositeProducer::~CompositeProducer()
{
 
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
CompositeProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  std::auto_ptr<std::vector<reco::CompositeCandidate> > jpsiCands( new std::vector<reco::CompositeCandidate> );
  edm::Handle<reco::CandidateCollection> h_daus;


  iEvent.getByLabel( dauSrc_, h_daus );

  reco::Particle::LorentzVector temp(0,0,0,0);
	
  if ( h_daus.isValid() && h_daus->size() > 0 ) {

    reco::CompositeCandidate jpsi;

    double hTrans=0.0;
    
    for ( reco::CandidateCollection::const_iterator muonsBegin = h_daus->begin(),
	    muonsEnd = h_daus->end(), imuon = muonsBegin+1;
	  imuon != muonsEnd; ++imuon ) {


	    jpsi.addDaughter( *imuon );
            hTrans += imuon->pt();
            temp += reco::Particle::LorentzVector(imuon->px(),imuon->py(),0.0,imuon->pt());
    }

//    AddFourMomenta addp4;
//    addp4.set( jpsi );
    jpsi.setP4( temp );
    jpsiCands->push_back( jpsi );

//    jpsi.addUserFloat("hTrans", hTrans );
  }
  iEvent.put( jpsiCands );
}

// ------------ method called once each job just before starting event loop  ------------
void 
CompositeProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CompositeProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(CompositeProducer);
