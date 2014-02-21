
// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"


class MTProducer : public edm::EDProducer {
public:
  explicit MTProducer(const edm::ParameterSet&);
  ~MTProducer() {};

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
  const edm::InputTag candidateCollTag_;
  const edm::InputTag metTag_;
};

//
// constructors and destructor
//
MTProducer::MTProducer(const edm::ParameterSet& iConfig) :
  candidateCollTag_ ( iConfig.getParameter<edm::InputTag>("CandidateCollectionTag") ),
  metTag_           ( iConfig.getParameter<edm::InputTag>("METTag")                 )
{
  produces< std::vector<double> >();  
}

// ------------ method called to produce the data  ------------
void
MTProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // The candidate collection, e.g. the leptons
  edm::Handle< edm::View<reco::Candidate> > candColl;
  iEvent.getByLabel(candidateCollTag_,candColl);

  // The MET
  edm::Handle< edm::View<reco::Candidate> > met;
  iEvent.getByLabel(metTag_,met);

  // MT per candidate
  std::auto_ptr< std::vector<double> > mtColl(new std::vector<double>(candColl->size(),9999));
  
  // Loop over candidates and compute MT
  unsigned int mtIdx = 0;
  for(edm::View<reco::Candidate>::const_iterator c = candColl->begin();
      c != candColl->end(); ++c, ++mtIdx) {

    double deltaPhi = reco::deltaPhi( c->phi(), met->at(0).phi() );
    mtColl->at(mtIdx) = sqrt( 2.*(c->pt())*(met->at(0).pt())*(1.-cos(deltaPhi)) );
  }

  // Store MT collection in event 
  iEvent.put(mtColl);
}

// ------------ method called once each job just before starting event loop  ------------
void 
MTProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MTProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
MTProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
MTProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
MTProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
MTProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MTProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MTProducer);
