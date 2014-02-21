#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <TH2F.h>
#include <TH3F.h>
#include <TFile.h>
#include "TTree.h"
#include <TChain.h>


//
// class declaration
//

class Limit_ll : public edm::EDProducer {
   public:
      explicit Limit_ll(const edm::ParameterSet&);
      ~Limit_ll();

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
	std::string nTupleName_;
	TString Path_;

	std::string EfficiencyFileName_;
	double MTWMax_;
	Float_t eventWeight_;
	bool MTWCut_;
	double DiLepCorrection_, DiLepCorrectionUp_, DiLepCorrectionDown_;
	Float_t muonAccEff2_, elecAccEff2_;
	Float_t muonBinByBinEff_, muonBinByBinWeight_, totalMuonsBinByBinWeight_, elecBinByBinEff_, elecWeightBinByBin_, resultWeightBinByBin_, resultWeightBinByBinMTW_, totalMuonsBinByBin_;
	// binbybin sepearte result values
	Float_t muonBinByBinIsoWeight_, muonBinByBinRecoWeight_, muonBinByBinAccWeight_;
 	Float_t muonBinByBinIsoEff_, muonBinByBinRecoEff_, muonBinByBinAccEff_;
	Float_t muonBinByBinIsoEffError_, muonBinByBinRecoEffError_, muonBinByBinAccEffError_;

	Float_t elecBinByBinIsoWeight_, elecBinByBinRecoWeight_, elecBinByBinAccWeight_;
 	Float_t elecBinByBinIsoEff_, elecBinByBinRecoEff_, elecBinByBinAccEff_;
	Float_t elecBinByBinIsoEffError_, elecBinByBinRecoEffError_, elecBinByBinAccEffError_;

	Float_t totalBinByBinMuons_;
	Float_t muonBinByBinTotalWeight_, elecBinByBinTotalWeight_;
	Float_t resultBBBW_;
	Float_t resultBBBWMTW_, resultBBBWMTWUp_, resultBBBWMTWDown_;
	Float_t MTWUncertaintyUp_, MTWUncertaintyDown_;

	// uncertainties and corrections for binbybin
	Float_t resultBBBWMTWDiLep_, resultBBBWMTWDiLepUp_, resultBBBWMTWDiLepDown_;

	TH1F *MTWNJet_;
	TH2F *MuonAccEff3_, *ElecAccEff3_;
	TH2F *MuonIsoBinByBinEff35_, *MuonIsoBinByBinEff67_, *MuonIsoBinByBinEff8Inf_;
	TH2F *ElecIsoBinByBinEff35_, *ElecIsoBinByBinEff67_, *ElecIsoBinByBinEff8Inf_;
	TH2F *MuonRecoBinByBinEff35_, *MuonRecoBinByBinEff67_, *MuonRecoBinByBinEff8Inf_;
	TH2F *ElecRecoBinByBinEff35_, *ElecRecoBinByBinEff67_, *ElecRecoBinByBinEff8Inf_;
};