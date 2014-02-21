#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"
#include "FWCore/Utilities/interface/EDMException.h"


#include <DataFormats/PatCandidates/interface/Jet.h>
#include <DataFormats/PatCandidates/interface/MET.h>
#include <DataFormats/PatCandidates/interface/Muon.h>
#include <DataFormats/PatCandidates/interface/Electron.h>
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Common/interface/View.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "TH2.h"
#include "TH3.h"
#include <utility>
#include <vector>
#include "TString.h"
#include "TTree.h"

#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "TLorentzVector.h"
#include <DataFormats/JetReco/interface/CaloJet.h>




//
// class declaration
//

class MCEffCalculator : public edm::EDAnalyzer {
   public:
      explicit MCEffCalculator(const edm::ParameterSet&);
      ~MCEffCalculator();
	// first deltaR second CloestJetPT
	static std::pair <double,double> DRToClosestJet(const edm::Event& iEvent, edm::InputTag caloJetTag_, double lepEta, double lepPhi);
	static double MTWCalculator(const edm::Event& iEvent, edm::InputTag MTTag, double lepPT, double lepPhi);

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
	edm::View <reco::Candidate>::const_iterator LeptonMatch(const edm::View <reco::Candidate>  &MuID, Float_t lepGenEta, Float_t lepGenPhi, Float_t lepGenPt);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

	//special functions
	void MuonFound(reco::GenParticleCollection::const_iterator cand, unsigned int i);
	void ElecFound(reco::GenParticleCollection::const_iterator cand, unsigned int i);
	void TauFound(const reco::Candidate* tau);
	void ResetValues();

	//Tags
  	edm::InputTag evtWeightTag_;
  	edm::InputTag muonIDTag_, muonIDISOTag_;
  	edm::InputTag elecIDTag_, elecIDISOTag_;

	edm::InputTag caloJetTag_;
  	edm::InputTag metTag_;
  	edm::InputTag genTag_;
	
	// additional input tags for plotting
	edm::InputTag HtJetsTag_, MhtJetsTag_, HTTag_,MHTTag_, NVTag_;
   	edm::InputTag muonPromtMatchedTag_;


      // Variables
	//Input
	double minMuonPt_, minElecPt_, minJetPt_, maxMuonEta_, maxElecEta_;
	float eventWeight_;
	unsigned int nGenMu_, nGenElec_;
	TH2F *muonIDPassedTH2F_, *muonIsoPassedTH2F_, *muonIsoPassedUpTH2F_, *muonIsoPassedDownTH2F_;
	TH2F *muonIDPassed2TH2F_, *muonIsoPassed2TH2F_, *muonIsoPassedUp2TH2F_, *muonIsoPassedDown2TH2F_;

	TH2F *elecIDPassedTH2F_, *elecIsoPassedTH2F_, *elecIsoPassedUpTH2F_, *elecIsoPassedDownTH2F_;
	TH2F *elecIDPassed2TH2F_, *elecIsoPassed2TH2F_, *elecIsoPassedUp2TH2F_, *elecIsoPassedDown2TH2F_;

 	TH2F *muonIDFailedTH2F_, *muonIsoFailedTH2F_, *muonIsoFailedUpTH2F_, *muonIsoFailedDownTH2F_;
 	TH2F *muonIDFailed2TH2F_, *muonIsoFailed2TH2F_, *muonIsoFailedUp2TH2F_, *muonIsoFailedDown2TH2F_;

	TH2F *elecIDFailedTH2F_, *elecIsoFailedTH2F_, *elecIsoFailedUpTH2F_, *elecIsoFailedDownTH2F_;
	TH2F *elecIDFailed2TH2F_, *elecIsoFailed2TH2F_, *elecIsoFailedUp2TH2F_, *elecIsoFailedDown2TH2F_;

	TH2F *MuonMTPassedTH2F_, *MuonMTFailedTH2F_;

	TH3F *totalEffTH3FPassedMu_, *totalEffTH3FFailedMu_, *totalEffTH3FPassedElec_, *totalEffTH3FFailedElec_;

	TH3F *accEffTH3FPassedMu_, *accEffTH3FFailedMu_, *accEffTH3FPassedElec_, *accEffTH3FFailedElec_;
	TH3F *recoEffTH3FPassedMu_, *recoEffTH3FFailedMu_, *recoEffTH3FPassedElec_, *recoEffTH3FFailedElec_;
	TH3F *isoEffTH3FPassedMu_, *isoEffTH3FFailedMu_, *isoEffTH3FPassedElec_, *isoEffTH3FFailedElec_;

	// binbybin iso, reco

	TH2F *isoEffTH2PassedMuNJet35_, *isoEffTH2FailedMuNJet35_, *isoEffTH2PassedMuNJet67_, *isoEffTH2FailedMuNJet67_, *isoEffTH2PassedMuNJet8Inf_, *isoEffTH2FailedMuNJet8Inf_;
	TH2F *isoEffTH2PassedElecNJet35_, *isoEffTH2FailedElecNJet35_, *isoEffTH2PassedElecNJet67_, *isoEffTH2FailedElecNJet67_, *isoEffTH2PassedElecNJet8Inf_, *isoEffTH2FailedElecNJet8Inf_;

	TH2F *recoEffTH2PassedMuNJet35_, *recoEffTH2FailedMuNJet35_, *recoEffTH2PassedMuNJet67_, *recoEffTH2FailedMuNJet67_, *recoEffTH2PassedMuNJet8Inf_, *recoEffTH2FailedMuNJet8Inf_;
	TH2F *recoEffTH2PassedElecNJet35_, *recoEffTH2FailedElecNJet35_, *recoEffTH2PassedElecNJet67_, *recoEffTH2FailedElecNJet67_, *recoEffTH2PassedElecNJet8Inf_, *recoEffTH2FailedElecNJet8Inf_;

	TH1F *MuonAccPassedTH1F_, *MuonAccFailedTH1F_;
	TH1F *MuonAccPassed2TH1F_, *MuonAccFailed2TH1F_;
	TH1F *ElecAccPassedTH1F_, *ElecAccFailedTH1F_;
	TH1F *ElecAccPassed2TH1F_, *ElecAccFailed2TH1F_;
	TH2F *MuonAccPassedTH2F_, *MuonAccFailedTH2F_;
	TH2F *ElecAccPassedTH2F_, *ElecAccFailedTH2F_;

	TH2F *MTWTH2F_;
	TH1F *lepDeltaR_;


	//Tree and stuff to be filed in it
        TString treeName_;
        TTree* tree_;

	int muonNumberRecoID_, elecNumberRecoID_, leptons_;
	Float_t muonPTAccPassed_, elecPTAccPassed_;
	// changed to float_t!! from double 12 12 2012
	Float_t muonIDPassed_, muonIDFailed_, elecIDPassed_, elecIDFailed_, muonRecoPt_, muonRecoEta_, muonRecoPhi_, elecRecoPt_, elecRecoEta_, elecRecoPhi_;
	Float_t muonGenPt_, muonGenEta_, muonGenPhi_, elecGenPt_, elecGenEta_, elecGenPhi_;
	Float_t deltaGenR_, closestJetToMuonGenPt_,closestJetToElecGenPt_, mt_, mtw_, mtwElec_, genPt_, genPhi_, genEta_, genDeltaR_, genPTJet_, genPTRelJet_;
	// reco iso eff plots
	TH2F *elecIdEffTH2F_, *elecIsoEffTH2F_, *elecIsoEffUpTH2F_, *elecIsoEffDownTH2F_;
 	TH2F *muonIdEffTH2F_, *muonIsoEffTH2F_, *muonIsoEffUpTH2F_, *muonIsoEffDownTH2F_;	


	//Varialbes used for studies
	Float_t RecoMuonDeltaR_, RecoMuonPTJet_, RecoMuonPTRelJet_, IsoMuonDeltaR_, IsoMuonPTJet_, IsoMuonPTRelJet_, RecoGenMuDeltaR_;
	int nRecoMu_, nRecoElec_, nIsoMu_, nIsoElec_, nAccMu_, nAccElec_;
	//not plotted
	Float_t muonIsoPt_, muonIsoEta_, muonIsoPhi_;

	Float_t RecoElecDeltaR_, RecoElecPTJet_, RecoElecPTRelJet_, IsoElecDeltaR_, IsoElecPTJet_, IsoElecPTRelJet_, RecoGenElecDeltaR_;
	//not plotted
	Float_t elecIsoPt_, elecIsoEta_, elecIsoPhi_;

	// only for plotting purposes
	Float_t ht_, mht_;
	UShort_t nJets_, nV_;
	int isExpectation_;
	int nMuInIsoMuCollection_;
	int muPromtMatched_;

	// testing
	int ntau_, ntauMu_, ntauElec_;

	// gen values for top b tagging
	int nTop_, nTopB_, nTopW_, nTopMuNu_, nTopMu_, nTopElec_;
	Float_t TopPT_, BTopPT_, WTopPT_, MuNuTopPT_, MuTopPT_, ElecTopPT_;
	Float_t TopEta_, BTopEta_, WTopEta_, MuNuTopEta_, MuTopEta_, ElecTopEta_;
	Float_t TopPhi_, BTopPhi_, WTopPhi_, MuNuTopPhi_, MuTopPhi_, ElecTopPhi_;

	Float_t promtMuMetCombinedPT_, promtMuMetCombinedEta_, promtMuMetCombinedPhi_;
	Float_t wTopMassDifRecoGenPt_, wTopMassDifRecoGenEta_, wTopMassDifRecoGenPhi_;

	Float_t BMuTopDeltaR_, mtt_;
	Float_t recoMtw1_,recoMtw2_,recoMtw3_,recoMtw4_,recoMtw5_;
	Float_t recoMuDeltaPTClosestJet1_, recoMuDeltaPTClosestJet2_, recoMuDeltaPTClosestJet3_, recoMuDeltaPTClosestJet4_, recoMuDeltaPTClosestJet5_;
	Float_t recoMuDeltaRClosestJet1_, recoMuDeltaRClosestJet2_, recoMuDeltaRClosestJet3_, recoMuDeltaRClosestJet4_, recoMuDeltaRClosestJet5_;
	TLorentzVector promtRecoTLVMu_,promtRecoTLVMET_, promtRecoTLVWDecay_, mttRecoMuMet_, mttGen_;
};