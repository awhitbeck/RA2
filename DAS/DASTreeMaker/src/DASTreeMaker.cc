// -*- C++ -*-
//
// Package:    DASTreeMaker
// Class:      DASTreeMaker
// 
/**\class DASTreeMaker DASTreeMaker.cc DAS/DASTreeMaker/src/DASTreeMaker.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Matthias Schroeder,32 3-B20,+41227677557,
//         Created:  Sat Oct  5 17:17:51 CEST 2013
// $Id$
//
//


// system include files
#include <algorithm>
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

#include "DAS/DASTreeMaker/interface/DASTreeMaker.h"

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

//
// constructors and destructor
//
DASTreeMaker::DASTreeMaker(const edm::ParameterSet& iConfig)
  : tree_(0),
    sampleId_(iConfig.getParameter<int>("SampleId")),
    maxCandColSize_(50) {
  
  ///// Set parameter values and set up variable containers

  // Output
  treeName_ = iConfig.getParameter<std::string>("TreeName");


  // Primary vertices
  vertexCollectionTag_ = iConfig.getParameter<edm::InputTag>("VertexCollection");


  // Event weights
  weightTags_.push_back( iConfig.getParameter<edm::InputTag>("WeightTag") );
  weightNamesInTree_.push_back("Weight");
  weightTags_.push_back( iConfig.getParameter<edm::InputTag>("PrescaleTag") );
  weightNamesInTree_.push_back("HLTPrescale");
  weights_ = std::vector<Float_t>(weightTags_.size(),1.);


  // MET variables
  metTags_.push_back( iConfig.getParameter<edm::InputTag>("MET") );
  metNamesInTree_.push_back("MET");
  metMags_ = std::vector<Float_t>(metTags_.size(),-9999.);
  metPhis_ = std::vector<Float_t>(metTags_.size(),-9999.);


  // Jet and lepton collections
  candColInputTags_.push_back( iConfig.getParameter<edm::InputTag>("Jets") );
  candColNamesInTree_.push_back("Jets");
  candColInputTags_.push_back( iConfig.getParameter<edm::InputTag>("Electrons") );
  candColNamesInTree_.push_back("Elecs");
  candColInputTags_.push_back( iConfig.getParameter<edm::InputTag>("IsoElectrons") );
  candColNamesInTree_.push_back("IsoElecs");
  candColInputTags_.push_back( iConfig.getParameter<edm::InputTag>("Muons") );
  candColNamesInTree_.push_back("Muons");
  candColInputTags_.push_back( iConfig.getParameter<edm::InputTag>("IsoMuons") );
  candColNamesInTree_.push_back("IsoMuons");
  isoMuCandColIdx_ = candColInputTags_.size() - 1;

  candColN_ = std::vector<UShort_t>(candColInputTags_.size(),0);
  for(unsigned int i = 0; i < candColInputTags_.size(); ++i) {
    candColPt_.push_back (new Float_t[maxCandColSize_]);
    candColEta_.push_back(new Float_t[maxCandColSize_]);
    candColPhi_.push_back(new Float_t[maxCandColSize_]);
    candColE_.push_back  (new Float_t[maxCandColSize_]);    
  }
  
  
  // W-decay information
  genParticleColTag_ = iConfig.getParameter<edm::InputTag>("GenParticles");
  genPartNamesInTree_.push_back("GenW");
  genPartNamesInTree_.push_back("GenLepton");
  genPartNamesInTree_.push_back("GenNeutrino");
  genPartNamesInTree_.push_back("GenLeptonFromTau");
  genPartPt_    = std::vector<Float_t>(genPartNamesInTree_.size(),-99999.);
  genPartEta_   = std::vector<Float_t>(genPartNamesInTree_.size(),-99999.);
  genPartPhi_   = std::vector<Float_t>(genPartNamesInTree_.size(),-99999.);
  genPartE_     = std::vector<Float_t>(genPartNamesInTree_.size(),-99999.);
  genPartPdgId_ = std::vector<Int_t>(genPartNamesInTree_.size(),-99999);
  flgW_ = 0;
  flgTau_ = 0;
  
  
  // Set default values for all branch variables
  setBranchVariablesToDefault();
}


DASTreeMaker::~DASTreeMaker() {
  for(unsigned int i = 0; i < candColInputTags_.size(); ++i) {
    delete [] candColPt_.at(i);
    delete [] candColEta_.at(i);
    delete [] candColPhi_.at(i);
    delete [] candColE_.at(i);
  }
}


//
// member functions
//

// ------------ method called for each event  ------------
void
DASTreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // Set default values for all branch variables
  setBranchVariablesToDefault();
  

  // Event-provenance information
  edm::EventAuxiliary aux = iEvent.eventAuxiliary();
  runNum_       = aux.run();
  lumiBlockNum_ = aux.luminosityBlock();
  evtNum_       = aux.event();


  // Event weights
  for(unsigned int i = 0; i < weightTags_.size(); ++i) {
    edm::Handle<double> weight;
    iEvent.getByLabel(weightTags_.at(i),weight);
    if( weight.isValid() ) {
      weights_.at(i) = *weight;
    }
  }
  

  // Number of vertices
  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(vertexCollectionTag_,vertices);
  if( vertices.isValid() ) {
    nVtx_ = vertices->size();
  }


  // Jet and lepton collections
  for(unsigned int i = 0; i < candColInputTags_.size(); ++i) {
    edm::Handle< edm::View<reco::Candidate> > cands;
    iEvent.getByLabel(candColInputTags_.at(i),cands);
    if( cands.isValid() ) {
      unsigned int nCands= std::min(cands->size(),maxCandColSize_);
      candColN_.at(i) = nCands;
      for(unsigned int j = 0; j < nCands; ++j) {
	candColPt_.at(i)[j] = cands->at(j).pt();
	candColEta_.at(i)[j] = cands->at(j).eta();
	candColPhi_.at(i)[j] = cands->at(j).phi();
	candColE_.at(i)[j] = cands->at(j).energy();
      }

      if( i == 0 ) {
	// in case of jets, store HT, MHT, NJets
	analyzeJets(*cands);
      }
    }
  }


  // METs
  for(unsigned int i = 0; i < metTags_.size(); ++i) {
    edm::Handle< edm::View<reco::Candidate> > met;
    iEvent.getByLabel(metTags_.at(i),met);
    if( met.isValid() ) {
      metMags_.at(i) = met->at(0).pt();
      metPhis_.at(i) = met->at(0).phi();
    }
  }


  // MT of leading isomuon and MET
  if( candColN_.at(isoMuCandColIdx_) > 0 ) {
    mt_ = mt(candColPt_.at(isoMuCandColIdx_)[0],candColPhi_.at(isoMuCandColIdx_)[0],metMags_.front(),metPhis_.front());
  }


  // Generator particle information
  //  if( sampleId_ == 24 ) {
    edm::Handle< edm::View<reco::GenParticle> > genParticles;
    iEvent.getByLabel(genParticleColTag_,genParticles);
    if( genParticles.isValid() ) {
      analyzeWDecay(*genParticles);
    }
    //  }

  
  // Fill variables into tree
  tree_->Fill();
}


void
DASTreeMaker::analyzeJets(const edm::View<reco::Candidate> &jets) {
  unsigned short int nJets = 0;
  double ht = 0.;
  reco::MET::LorentzVector mht(0.,0.,0.,0.);
  for(edm::View<reco::Candidate>::const_iterator jet = jets.begin();
      jet != jets.end(); ++jet) {
    if( isHTJet(*jet) ) {
      ++nJets;
      ht += jet->pt();
    }
    if( isMHTJet(*jet) ) {
      mht -= jet->p4();
    }
  }
  nJets_ = nJets;
  ht_ = ht;
  mhtMag_ = mht.pt();
  mhtPhi_ = mht.phi();

  unsigned int nMHTJet = 0;
  for(edm::View<reco::Candidate>::const_iterator jet = jets.begin();
      jet != jets.end(); ++jet) {
    if( isMHTJet(*jet) ) {
      if( nMHTJet == 0 ) {
	deltaPhi1_ = std::abs(reco::deltaPhi(jet->phi(),mht.phi()));
      } else if( nMHTJet == 1 ) {
	deltaPhi2_ = std::abs(reco::deltaPhi(jet->phi(),mht.phi()));
      } else if( nMHTJet == 2 ) {
	deltaPhi3_ = std::abs(reco::deltaPhi(jet->phi(),mht.phi()));
      }
      ++nMHTJet;
    }
  }
}


double
DASTreeMaker::mt(double pt, double phi, double met, double metPhi) const {
  const double deltaPhi = reco::deltaPhi(phi,metPhi);
  return sqrt( 2.*pt*met*(1.-cos(deltaPhi)) );
}


// Find a status-3 W, and stores its kinematic information, and flags
// its decay. In case of leptonic decays, kinematic information of the
// leptons are also stored. If the lepton is a tau, the hadronic-tau flag
// is set accordingly.
// Returns number of status-3 Ws. Only the first one is analyzed. 
unsigned int
DASTreeMaker::analyzeWDecay(const edm::View<reco::GenParticle> &genPs) {
  unsigned int nW = 0;
  unsigned int nDcyLep = 0;
  flgW_ = 0;
  flgTau_ = 0;
  
  if( &genPs ) {
    for(edm::View<reco::GenParticle>::const_iterator gp = genPs.begin();
	gp != genPs.end(); ++gp) {
      
      // Consider only status-3 particles
      if( gp->status() != 3 ) continue;
      
      const int pdgId = std::abs(gp->pdgId());
      
      if( pdgId == 24 ) {
	//---pick-up the W boson 4-vector
	
	if( nW < 1 ) {
	  genPartPdgId_.front() = gp->pdgId();
	  genPartPt_.front()   = gp->pt();
	  genPartEta_.front()  = gp->eta();
	  genPartPhi_.front()  = gp->phi();
	  genPartE_.front()    = gp->energy();
	}
	++nW;
	
      } else if( pdgId > 10 && pdgId < 17 ) {
	//----pick-up the leptonic W decays 4-vectors
	
	if( std::abs(gp->mother()->pdgId()) == 24 ) {
	  if( nDcyLep < 2 ) {
	    
	    const unsigned int gpIdx = ( pdgId == 11 || pdgId == 13 || pdgId == 15 ) ? 1 : 2;
	    genPartPdgId_.at(gpIdx) = gp->pdgId();
	    genPartPt_.at(gpIdx)  = gp->pt();
	    genPartEta_.at(gpIdx) = gp->eta();
	    genPartPhi_.at(gpIdx) = gp->phi();
	    genPartE_.at(gpIdx)   = gp->energy();
	    
	    // in case this is the lepton (not the neutrino)
	    // flag event as leptonic W-decay according to the lepton ID
	    if( gpIdx == 1 ) flgW_ = static_cast<short int>(pdgId);
	    
	    // in case the lepton is a tau, flag event
	    // according to the tau decay
	    if( pdgId == 15 ) {

	      const reco::Candidate* lepton = NULL;
	      flgTau_ = static_cast<UShort_t>( flagTauDecay(*gp,lepton) );
 	      if( flgTau_ == 11 || flgTau_ == 13 ) {
 		if( lepton != NULL ) {
		  genPartPdgId_.at(3) = lepton->pdgId();
		  genPartPt_.at(3)  = lepton->pt();
		  genPartEta_.at(3) = lepton->eta();
		  genPartPhi_.at(3) = lepton->phi();
		  genPartE_.at(3)   = lepton->energy();
 		}
 	      }

	    }

	  }
	  ++nDcyLep;
	}
      }
    }
  } 
  return nW;
}



// Returns flag according to tau decay:
//  1 for fully-hadronic decay of tau
//  11 for leptonic decay into electron
//  13 for leptonic decay into muon
int
DASTreeMaker::flagTauDecay(const reco::Candidate &cand, const reco::Candidate* &lepton) const {
  int flag = 1;// Assume hadronically decaying tau
  for(reco::Candidate::const_iterator itc = cand.begin();
      itc != cand.end(); ++itc) {
    int pdgIdDaughter = std::abs(itc->pdgId());

    if( pdgIdDaughter == 15 || pdgIdDaughter == 24) {
      // tau might not be the finally decaying tau but have
      // an intermediate tau or w in the decay

      flag = flagTauDecay(*itc,lepton);
      if( flag == 11 || flag == 13 ) break;      

    } else if( pdgIdDaughter == 11 ) {
      // Leptonic decay into electron
      flag = 11;
      lepton = &(*itc);
      break;
    } else if( pdgIdDaughter == 13 ) {
      // Leptonic decay into muon
      flag = 13;
      lepton = &(*itc);
      break;
    }
  }

  return flag;
}



void 
DASTreeMaker::setBranchVariablesToDefault() {
  runNum_ = 0;      
  lumiBlockNum_ = 0;
  evtNum_ = 0;      
  nVtx_ = 0;
  ht_ = 0.;
  mhtMag_ = 0.;
  mhtPhi_ = 0.;
  nJets_ = 0;
  deltaPhi1_ = -9999.;
  deltaPhi2_ = -9999.;
  deltaPhi3_ = -9999.;

  for(unsigned int i = 0; i < weights_.size(); ++i) {
    weights_.at(i) = 1.;
  }
  for(unsigned int i = 0; i < metMags_.size(); ++i) {
    metMags_.at(i) = -9999.;
    metPhis_.at(i) = -9999.;
  }
  mt_ = -9999.;
  for(unsigned int i = 0; i < candColInputTags_.size(); ++i) {
    candColN_.at(i) = 0;
    for(unsigned int j = 0; j < maxCandColSize_; ++j) {
      candColPt_.at(i)[j]  = -9999.;
      candColEta_.at(i)[j] = -9999.;
      candColPhi_.at(i)[j] = -9999.;
      candColE_.at(i)[j]   = -9999.;
    }
  }

  for(unsigned int i = 0; i < genPartNamesInTree_.size(); ++i) {
    genPartPt_.at(i)    = -9999.;
    genPartEta_.at(i)   = -9999.;
    genPartPhi_.at(i)   = -9999.;
    genPartE_.at(i)     = -9999.;
    genPartPdgId_.at(i) = -9999.;
  }
  flgW_ = 0;
  flgTau_ = 0;
}



// ------------ method called once each job just before starting event loop  ------------
void 
DASTreeMaker::beginJob() {

  edm::Service<TFileService> fs;
  if( !fs ) {
    throw edm::Exception(edm::errors::Configuration, "TFile Service is not registered in cfg file");
  }
  tree_ = fs->make<TTree>(treeName_.c_str(),treeName_.c_str());
  tree_->SetAutoSave(10000000000);
  tree_->SetAutoFlush(1000000);

  tree_->Branch("RunNum",&runNum_,"RunNum/i");
  tree_->Branch("LumiBlockNum",&lumiBlockNum_,"LumiBlockNum/i");
  tree_->Branch("EvtNum",&evtNum_,"EvtNum/i");
  tree_->Branch("NVtx",&nVtx_,"NVtx/s");
  tree_->Branch("HT",&ht_,"HT/F");
  tree_->Branch("MHT",&mhtMag_,"MHT/F");
  tree_->Branch("MHTPhi",&mhtPhi_,"MHTPhi/F");
  tree_->Branch("NJets",&nJets_,"NJets/s");
  tree_->Branch("DeltaPhi1",&deltaPhi1_,"DeltaPhi1/F");
  tree_->Branch("DeltaPhi2",&deltaPhi2_,"DeltaPhi2/F");
  tree_->Branch("DeltaPhi3",&deltaPhi3_,"DeltaPhi3/F");

  for(unsigned int i = 0; i < weights_.size(); ++i) {
    const std::string name = weightNamesInTree_.at(i);
    tree_->Branch(name.c_str(),&(weights_.at(i)),(name+"/F").c_str());
  }

  for(unsigned int i = 0; i < metMags_.size(); ++i) {
    const std::string name = metNamesInTree_.at(i);
    tree_->Branch(name.c_str(),&(metMags_.at(i)),(name+"/F").c_str());
    tree_->Branch((name+"Phi").c_str(),&(metPhis_.at(i)),(name+"Phi/F").c_str());
  }
  tree_->Branch("MT",&mt_,"MT/F");

  for(unsigned int i = 0; i < candColInputTags_.size(); ++i) {
    const std::string name = candColNamesInTree_.at(i);
    tree_->Branch((name+"Num").c_str(),&(candColN_.at(i)),(name+"Num/s").c_str());
    tree_->Branch((name+"Pt").c_str(), candColPt_.at(i), (name+"Pt["+name+"Num]/F").c_str());
    tree_->Branch((name+"Eta").c_str(),candColEta_.at(i),(name+"Eta["+name+"Num]/F").c_str());
    tree_->Branch((name+"Phi").c_str(),candColPhi_.at(i),(name+"Phi["+name+"Num]/F").c_str());
    tree_->Branch((name+"E").c_str(),  candColE_.at(i),  (name+"E["+name+"Num]/F").c_str());
  }

  if( sampleId_ == 24 ) {
    for(unsigned int i = 0; i < genPartNamesInTree_.size(); ++i) {
      const std::string name = genPartNamesInTree_.at(i);
      tree_->Branch((name+"Pt").c_str(),&(genPartPt_.at(i)),(name+"Pt/F").c_str());
      tree_->Branch((name+"Eta").c_str(),&(genPartEta_.at(i)),(name+"Eta/F").c_str());
      tree_->Branch((name+"Phi").c_str(),&(genPartPhi_.at(i)),(name+"Phi/F").c_str());
      tree_->Branch((name+"E").c_str(),&(genPartE_.at(i)),(name+"E/F").c_str());
      tree_->Branch((name+"PdgId").c_str(),&(genPartPdgId_.at(i)),(name+"PdgId/I").c_str());
    }
    tree_->Branch("FlgW",&flgW_,"FlgW/s");
    tree_->Branch("FlgTau",&flgTau_,"FlgTau/s");
  }
}


// ------------ method called once each job just after ending the event loop  ------------
void 
DASTreeMaker::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
DASTreeMaker::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
DASTreeMaker::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
DASTreeMaker::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
DASTreeMaker::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DASTreeMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DASTreeMaker);
