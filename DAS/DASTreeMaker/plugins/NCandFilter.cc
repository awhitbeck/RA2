#include <memory>

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Candidate/interface/Candidate.h"



class NCandFilter : public edm::EDFilter {
public:
  
  explicit NCandFilter(const edm::ParameterSet & iConfig);
  ~NCandFilter() {}
  
private:
  const edm::InputTag candsInputTag_;
  const double ptMin_;
  const double etaMax_;
  const int nCandsMin_;
  
  virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
};



NCandFilter::NCandFilter(const edm::ParameterSet & iConfig) :
  candsInputTag_(iConfig.getParameter<edm::InputTag>("CandidateCollection")),
  ptMin_(iConfig.getParameter<double>("PtMin")),
  etaMax_(iConfig.getParameter<double>("EtaMax")),
  nCandsMin_(iConfig.getParameter<int>("NumMin")) {
}


bool NCandFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {
  int num = 0;
  edm::Handle< edm::View<reco::Candidate> > cands;
  iEvent.getByLabel(candsInputTag_,cands);
  if( cands.isValid() ) {
    for(edm::View<reco::Candidate>::const_iterator cand = cands->begin();
	cand != cands->end(); ++cand) {
      if( cand->pt() > ptMin_ && std::abs(cand->eta()) < etaMax_ ) {
	++num;
      }
      if( num >= nCandsMin_ ) {
	break;
      }
    }
  }

  return num >= nCandsMin_;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NCandFilter);
