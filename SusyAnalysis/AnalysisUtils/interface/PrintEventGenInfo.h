// system include files
#include <memory>
#include <iostream>
#include <sstream> 
#include <algorithm>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "FWCore/Framework/interface/ESHandle.h"
using namespace std;
using namespace edm;
using namespace reco;

namespace anautils {
  
  //enum decaymode {WHAD=1, WENU, WMUNU, WTAUNU, WTAUNU_HAD, WTAUNU_MUNU, WTAUNU_ENU, BQUARK_HAD,BQUARK_MUNU, BQUARK_ENU};
  enum decaymode {HAD_DECAY=1, WENU, WMUNU, WTAUNU, WTAUNU_HAD, WTAUNU_MUNU, WTAUNU_ENU, BQUARK_MUNU, BQUARK_ENU, BQUARK_MUNUENU};

  /// select candidate
  bool select( const reco::Candidate & );

  /// has valid daughters in the chain
  bool hasValidDaughters( const reco::Candidate & );
  
  std::string printP4( const Candidate & c, bool printP4_=false, bool printPtEtaPhi_=true, bool printVertex_=false );
  //std::string printP4( const Candidate & c, bool printP4_, bool printPtEtaPhi_, bool printVertex_ );

  vector<const Candidate *> decayWVec( const reco::Candidate &, edm::ESHandle<ParticleDataTable> &, bool debug=false );
  void decayWtaunuVec( const reco::Candidate &, vector<const Candidate *> &, edm::ESHandle<ParticleDataTable> &, bool debug=false );
  void decayBquarkVec( const reco::Candidate &, vector<const Candidate *> &, edm::ESHandle<ParticleDataTable> &, bool debug=false );
  int  decayMode     ( int moPdgId, vector<const Candidate *> &, edm::ESHandle<ParticleDataTable> &, bool debug=false );
  bool isWenu        ( vector<const Candidate *> &, edm::ESHandle<ParticleDataTable> &, bool debug=false );
  bool isWmunu       ( vector<const Candidate *> &, edm::ESHandle<ParticleDataTable> &, bool debug=false );
  bool isWtaunu      ( vector<const Candidate *> &, edm::ESHandle<ParticleDataTable> &, bool debug=false );
  bool isWtaunu_munu ( vector<const Candidate *> &, edm::ESHandle<ParticleDataTable> &, bool debug=false );
  bool isWtaunu_enu  ( vector<const Candidate *> &, edm::ESHandle<ParticleDataTable> &, bool debug=false );
  bool isBquark_munu ( vector<const Candidate *> &, edm::ESHandle<ParticleDataTable> &, bool debug=false );
  bool isBquark_enu  ( vector<const Candidate *> &, edm::ESHandle<ParticleDataTable> &, bool debug=false );

  //print particle four vector
  void printParticle ( const reco::Candidate & c, edm::ESHandle<ParticleDataTable> & pdt_ );

  //print LHE comment, needed to identify model point in scans
  void printLHEComment(edm::Handle<LHEEventProduct>& lhe);
  
}
