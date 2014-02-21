#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/Association.h"

#include "DataFormats/Provenance/interface/ProductID.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/UserData.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include <DataFormats/PatCandidates/interface/Muon.h>
#include "DataFormats/Common/interface/RefToBaseVector.h"

namespace { 
  struct dictionary {

     typedef unsigned long key_type;
  
     edm::ValueMap<edm::ProductID> vmpd;
     edm::ValueMap<key_type> vmky;

     edm::Wrapper<edm::ValueMap<edm::ProductID> > pd;
     edm::Wrapper<edm::ValueMap<key_type> >    ky;

     std::vector<edm::ProductID> vvpd;
     std::vector<key_type> vvky;

     edm::Wrapper<std::vector<edm::ProductID> > wvvpd;
     edm::Wrapper<std::vector<key_type> > wvvky;

     pat::UserHolder<edm::ProductID> uhpd;
     edm::Wrapper<pat::UserHolder<edm::ProductID> > wuhpd;

//     edm::reftobase::BaseVectorHolder<pat::Muon> bvhm;
     edm::RefToBaseVector<pat::Muon> rtbm;
     edm::Wrapper<edm::RefToBaseVector<pat::Muon> > wrtbm;

     std::vector<std::vector<double> > vvd;
     edm::Wrapper<std::vector<std::vector<double> > > wvvd;

     std::vector<std::vector<int> > vvi;
     edm::Wrapper<std::vector<std::vector<int> > > wvvi;

     std::vector<std::vector<std::string> > vvs;
     edm::Wrapper<std::vector<std::vector<std::string> > > wvvs;

  };
}


