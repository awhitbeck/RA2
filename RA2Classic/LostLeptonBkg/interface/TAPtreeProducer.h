// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "TLorentzVector.h"

#include <TH2F.h>
#include <TH3F.h>
#include <TFile.h>
#include "TTree.h"

//
// class declaration
//

class TAPtreeProducer : public edm::EDProducer {
   public:
      explicit TAPtreeProducer(const edm::ParameterSet&);
      ~TAPtreeProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

	// Functions
	void ResetValues();

	// Tags
	edm::InputTag HtJetsTag_;
	edm::InputTag MetTag_;
	edm::InputTag IsoMuonTag_;
	edm::InputTag IsoElecTag_;

	edm::InputTag RecoMuonTag_;
	edm::InputTag RecoElecTag_;

	edm::InputTag PromtMuonTag_;
	edm::InputTag PromtElecTag_;
	edm::InputTag GsfElecTag_;

	edm::InputTag MhtTag_,HTTag_,MHTTag_, NVTag_;
	edm::InputTag CaloJetTag_;
	edm::InputTag evtWeightTag_;

	// other inputs
	double MTWMax_;
	bool MTWCut_;
 	double MinMuPT_, MinElecPT_;
	Float_t eventWeight_;
	unsigned int MuElecIdIso_;

	// input corrections uncertainties
	edm::InputTag MetJetTagUp_, MetJetTagDown_;


	// Variables
	float met_;
	float mht_, ht_;
	float mtw_; 
	int nMu_;
	Float_t MuPt_;
	Float_t MuEta_;
	Float_t MuPhi_;
	int nV_, nJets_;
	int nCaloJets_;

	int muPromtMatched_;


        TTree* tree_;
	// weights for x checks
	int error_;


	// tag and probe variables
	UShort_t probeIsoMu_, probeIsoElec_ , tagIsoMu_, tagIsoElec_;
	UShort_t nIsoMu_, nRecoMu_, nPromtMu_, nIsoElec_, nRecoElec_, nPromtElec_;

	// muon 
	Float_t RecoMuInvariantMass_, IsoMuInvariantMass_, RecoElecInvariantMass_,RecoGsfElecInvariantMass_, IsoElecInvariantMass_;
	Int_t MuIdPass_, MuIsoPass_, ElecIdPass_, ElecIsoPass_, ElecIdGsfPass_;
	bool MuIdPairFound_, MuIsoPairFound_, ElecIdPairFound_, ElecIsoPairFound_, ElecIdGsfPairFound_ ;

	Float_t MuIdProbePt_, MuIdProbeEta_, MuIdProbePhi_;
	Float_t MuIsoProbePt_, MuIsoProbeEta_, MuIsoProbePhi_;
	Float_t ElecIdProbePt_, ElecIdProbeEta_, ElecIdProbePhi_;
	Float_t ElecIdGsfProbePt_, ElecIdGsfProbeEta_, ElecIdGsfProbePhi_;
	Float_t ElecIsoProbePt_, ElecIsoProbeEta_, ElecIsoProbePhi_;


      // ----------member data ---------------------------
};