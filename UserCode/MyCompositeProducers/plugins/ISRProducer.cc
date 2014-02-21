// -*- C++ -*-
//
// Package:    ISRProducer
// Class:      ISRProducer
// 
/**\class ISRProducer CompositeProducer.cc Analysis/CompositeProducer/src/CompositeProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  "Salvatore Rappoccio"
// Modified by    :  "Stephen Mrenna "
//         Created:  Mon Sep 28 12:53:57 CDT 2009
// $Id: ISRProducer.cc,v 1.3 2011/03/08 04:40:34 mrenna Exp $
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

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include <TLorentzVector.h>


#include <vector>

//
// class declaration
//

class ISRProducer : public edm::EDProducer {
   public:
      explicit ISRProducer(const edm::ParameterSet&);
      ~ISRProducer();

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------

      void getAncestors(const reco::Candidate &c, std::vector<reco::Candidate const *> & moms );

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
ISRProducer::ISRProducer(const edm::ParameterSet& iConfig) 
{
  produces<std::vector<reco::CompositeCandidate> > ();
}


ISRProducer::~ISRProducer()
{
 
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
ISRProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  std::auto_ptr<std::vector<reco::CompositeCandidate> > jpsiCands( new std::vector<reco::CompositeCandidate> );
  edm::Handle<reco::CandidateView> h_daus;

  iEvent.getByLabel("genParticles", h_daus );

  if ( h_daus.isValid() && h_daus->size() > 8 ) {

     reco::CompositeCandidate jpsi;
     reco::Particle::LorentzVector temp(0,0,0,0);

    
     for ( reco::CandidateView::const_iterator muonsBegin = h_daus->begin()+6,
              muonsEnd = h_daus->begin()+8, imuon = muonsBegin;
           imuon != muonsEnd; ++imuon ) {

//        if( imuon->status() == 3) continue;

        bool match = true;

        int iMo1 = -1;
/*        found = find(cands.begin(), cands.end(), imuon->mother(0));
        if(found != cands.end()) iMo1 = found - cands.begin() ;
// include the beam remnants, because this is what I did originally
//        if( iMo1 == 2 || iMo1 == 3 || iMo1 == 0 || iMo1 == 1 ) match = true;
if( iMo1 == 2 || iMo1 == 3 ) match = true; */


        if( match ) {

//          std::cout << imuon->pdgId() << " " << " " << imuon->status() << " " << iMo1 << std::endl;

          jpsi.addDaughter( *imuon );

          temp += reco::Particle::LorentzVector(imuon->px(),imuon->py(),0,imuon->pt());

//         std::cout << imuon->px() << imuon->py() << imuon->pz() << imuon->energy() << std::endl;

        }
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
ISRProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ISRProducer::endJob() {
}

void ISRProducer::getAncestors(const reco::Candidate &c,
                               std::vector<reco::Candidate const *> & moms )
{

  if( c.numberOfMothers() == 1 ) {
    const reco::Candidate * dau = &c;
    const reco::Candidate * mom = c.mother();
    while ( dau->numberOfMothers() != 0) {
      moms.push_back( dau );
      dau = mom ;
      mom = dau->mother();
    } 
  } 
}


//define this as a plug-in
DEFINE_FWK_MODULE(ISRProducer);

