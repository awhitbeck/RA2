#include <memory>

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/METReco/interface/MET.h"

#include "TMath.h"
#include "TVector2.h"


class CaloVsPFMetFilter : public edm::EDFilter {
public:
  
  explicit CaloVsPFMetFilter(const edm::ParameterSet & iConfig);
  ~CaloVsPFMetFilter() {}
  
private:
  const bool taggingMode_;
  const edm::InputTag caloMetInputTag_;
  const edm::InputTag pfMetInputTag_;
  const double minCaloOverPFMet_;
  const double sizeOfDeltaPhiWindowInNPi_;
  const double minOfDeltaPhiWindow_;
  const double maxOfDeltaPhiWindow_;
  
  virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
};



CaloVsPFMetFilter::CaloVsPFMetFilter(const edm::ParameterSet & iConfig) :
  taggingMode_ (iConfig.getParameter<bool>("TaggingMode")),
  caloMetInputTag_(iConfig.getParameter<edm::InputTag>("CaloMetInputTag")),
  pfMetInputTag_(iConfig.getParameter<edm::InputTag>("PFMetInputTag")),
  minCaloOverPFMet_(iConfig.getParameter<double>("MinCaloOverPFMet")),
  sizeOfDeltaPhiWindowInNPi_(iConfig.getParameter<double>("SizeOfDeltaPhiWindowInNPi")),
  minOfDeltaPhiWindow_(TMath::Pi()*(1.-0.5*sizeOfDeltaPhiWindowInNPi_)),
  maxOfDeltaPhiWindow_(TMath::Pi()*(1.+0.5*sizeOfDeltaPhiWindowInNPi_))
{
  produces<bool>();

  std::cout << "minOfDeltaPhiWindow_ = " << minOfDeltaPhiWindow_ << std::endl;
  std::cout << "maxOfDeltaPhiWindow_ = " << maxOfDeltaPhiWindow_ << std::endl;
}




bool CaloVsPFMetFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {
  // Filter decision
  bool pass = true;

  // Read Calo and PF MET
  edm::Handle< edm::View<reco::Candidate> > caloMet;
  edm::Handle< edm::View<reco::Candidate> > pfMet;
  iEvent.getByLabel(caloMetInputTag_,caloMet);
  iEvent.getByLabel(pfMetInputTag_,pfMet);

  if( caloMet.isValid() && pfMet.isValid() ) {
    if( pfMet->at(0).pt() > 0. ) {
      if( caloMet->at(0).pt() / pfMet->at(0).pt() < minCaloOverPFMet_ ) pass = false;
    }
    if( pass ) {
      double deltaPhi = TVector2::Phi_0_2pi(pfMet->at(0).phi()-caloMet->at(0).phi());
      if( deltaPhi > minOfDeltaPhiWindow_ && deltaPhi < maxOfDeltaPhiWindow_ ) pass = false;
    }
  }

  // Store filter decision in event
  iEvent.put( std::auto_ptr<bool>(new bool(pass)) );
  
  // Depending on mode, return filter decision or true
  return taggingMode_ || pass; 
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(CaloVsPFMetFilter);
