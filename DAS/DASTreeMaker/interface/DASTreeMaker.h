//
// Original Author:  Matthias Schroeder,32 3-B20,+41227677557,
//         Created:  Sat Oct  5 17:17:51 CEST 2013
// $Id$
//
//


// system include files
#include <memory>
#include <string>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TTree.h"

//
// class declaration
//

class DASTreeMaker : public edm::EDAnalyzer {
public:
  explicit DASTreeMaker(const edm::ParameterSet&);
  ~DASTreeMaker();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();
  
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

  bool isHTJet(const reco::Candidate &jet) const { 
    return jet.pt() > 50. && std::abs(jet.eta()) < 2.5;
  }
  bool isMHTJet(const reco::Candidate &jet) const { 
    return jet.pt() > 30. && std::abs(jet.eta()) < 5.;
  }
  double mt(double pt, double phi, double met, double metPhi) const;
  void analyzeJets(const edm::View<reco::Candidate> &jets);
  unsigned int analyzeWDecay(const edm::View<reco::GenParticle> &genPs);
  int flagTauDecay(const reco::Candidate &cand, const reco::Candidate* &lepton) const;
  void setBranchVariablesToDefault();
  
  // ----------member data ---------------------------


  std::string treeName_;
  TTree* tree_;

  // External information
  const int sampleId_;

  // Event-provenance information 
  UInt_t runNum_;      
  UInt_t lumiBlockNum_;
  UInt_t evtNum_;

  // Global event information 
  std::vector<edm::InputTag> weightTags_;
  std::vector<std::string> weightNamesInTree_;
  std::vector<Float_t> weights_;

  edm::InputTag vertexCollectionTag_;
  UShort_t nVtx_;

  std::vector<edm::InputTag> metTags_;
  std::vector<std::string> metNamesInTree_;
  std::vector<Float_t> metMags_;
  std::vector<Float_t> metPhis_;
  Float_t mt_;

  Float_t ht_;
  Float_t mhtMag_;
  Float_t mhtPhi_;
  UShort_t nJets_;
  Float_t deltaPhi1_, deltaPhi2_, deltaPhi3_;


  // Jet and lepton collections
  const unsigned int maxCandColSize_;  // Maximum number of elements of a collection stored in the ntuple
  unsigned int isoMuCandColIdx_;
  std::vector<edm::InputTag> candColInputTags_;
  std::vector<std::string>   candColNamesInTree_;
  std::vector<UShort_t> candColN_;
  std::vector<Float_t*> candColPt_;
  std::vector<Float_t*> candColEta_;
  std::vector<Float_t*> candColPhi_;
  std::vector<Float_t*> candColE_;


  // W-decay information
  edm::InputTag genParticleColTag_;
  std::vector<std::string> genPartNamesInTree_;
  std::vector<Float_t> genPartPt_;
  std::vector<Float_t> genPartEta_;
  std::vector<Float_t> genPartPhi_;
  std::vector<Float_t> genPartE_;
  std::vector<Int_t> genPartPdgId_;
  UShort_t flgW_;
  UShort_t flgTau_;
};
