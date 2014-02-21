// -*- C++ -*-
//
// Package:    JetSmearing
// Class:      JetSmearing
// 
/**\class JetSmearing JetSmearing.cc RA2Classic/Utils/src/JetSmearing.cc

Description: Vary jet pt by its uncertainty and produce new jet collection.

*/
//
// Original Author:  Matthias Schroeder,,,
//         Created:  Thu Dec 13 17:23:26 CET 2012
// $Id: JetSmearing.cc,v 1.2 2013/01/22 18:24:22 mschrode Exp $
//
//


// system include files
#include <cassert>
#include <memory>
#include <string>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Selector.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/Utils/interface/PtComparator.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TRandom.h"
#include "TString.h"


//
// class declaration
//

class JetSmearing : public edm::EDProducer {
public:
  explicit JetSmearing(const edm::ParameterSet&);
  ~JetSmearing();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
      
  virtual void beginRun(edm::Run&, edm::EventSetup const&);
  virtual void endRun(edm::Run&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

  void smearP4(pat::Jet &jet) const;
  bool etaBin(unsigned int &etaBin, double eta) const {
    return bin(etaBin,std::abs(eta),binEdgesEta_);
  }
  bool ptBin(unsigned int &ptBin, double pt) const {
    return bin(ptBin,std::abs(pt),binEdgesPt_);
  }
  bool etaBinCoreScaleFactors(unsigned int &etaBin, double eta) const {
    return bin(etaBin,std::abs(eta),binEdgesEtaCoreScaleFactors_);
  }
  bool bin(unsigned int &bin, double x, const std::vector<double> &edges) const;
  bool findResponseHist(const TH1* &hResp, double eta, double pt) const;
  void setResponseHistograms(const TString &respHistsFileName, const TString &respHistsNamePrefix);
  void convoluteWithGauss(TH1* &h, double eta) const;
  double coreWidth(TH1* h) const;


  // ----------member data ---------------------------
  const edm::InputTag inputJetsTag_;
  const std::vector<double> binEdgesEta_;
  const std::vector<double> binEdgesPt_;
  const std::vector<double> coreScaleFactors_;
  const std::vector<double> binEdgesEtaCoreScaleFactors_;
  const bool applyCoreScaleFactors_;

  std::vector< std::vector<const TH1*> > responseHists_;
};



//
// constructors and destructor
//
JetSmearing::JetSmearing(const edm::ParameterSet& iConfig) :
  inputJetsTag_(iConfig.getParameter<edm::InputTag>("Jets")),
  binEdgesEta_(iConfig.getParameter< std::vector<double> >("BinEdgesEta")),
  binEdgesPt_(iConfig.getParameter< std::vector<double> >("BinEdgesPt")),
  coreScaleFactors_(iConfig.getParameter< std::vector<double> >("CoreScaleFactors")),
  binEdgesEtaCoreScaleFactors_(iConfig.getParameter< std::vector<double> >("BinEdgesEtaCoreScaleFactors")),
  applyCoreScaleFactors_( coreScaleFactors_.size() > 0 )
{
  // Sanity checks
  assert( binEdgesEta_.size() > 1 );
  assert( binEdgesPt_.size() > 1 );
  if( applyCoreScaleFactors_ ) assert( binEdgesEtaCoreScaleFactors_.size() == coreScaleFactors_.size()+1 );

  // Get response histograms from file
  TString respHistsFileName = iConfig.getParameter<std::string>("ResponseHistsFileName");
  TString respHistsNamePrefix = iConfig.getParameter<std::string>("ResponseHistsNamePrefix");
  setResponseHistograms(respHistsFileName,respHistsNamePrefix);

  // Different seed per initialization
  gRandom->SetSeed(0);

  // Print info
  std::cout << "***** JetSmearing Module *****************************" << std::endl;
  std::cout << "  Producing smeared jets from '" << inputJetsTag_.instance() << ":" << inputJetsTag_.label() << "'" << std::endl;
  std::cout << "  Using resolution templates from '" << respHistsFileName << "'" << std::endl;
  if( applyCoreScaleFactors_ ) {
    std::cout << "  Applying core scale-factors" << std::endl;
    for(unsigned int i = 0; i < coreScaleFactors_.size(); ++i) {
      std::cout << "    Eta " << binEdgesEtaCoreScaleFactors_.at(i) << " - " << binEdgesEtaCoreScaleFactors_.at(i+1) << ": " << coreScaleFactors_.at(i) << std::endl;
    }
  }
  std::cout << "******************************************************" << std::endl;


  // Declare the products
  produces< std::vector<pat::Jet> >();
}

JetSmearing::~JetSmearing() {
  for(std::vector< std::vector<const TH1*> >::iterator iEta = responseHists_.begin();
      iEta != responseHists_.end(); ++iEta) {
    for(std::vector<const TH1*>::iterator iPt = iEta->begin();
	iPt != iEta->end(); ++iPt) {
      delete *iPt;
    }
  }
}


// ------------ method called to produce the data  ------------
void
JetSmearing::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // Get the jet collection from the event
  edm::Handle< edm::View<reco::Jet> > inputJets;
  iEvent.getByLabel(inputJetsTag_,inputJets);

  // Collection of smeared jets
  std::auto_ptr< std::vector<pat::Jet> > smearedJets(new std::vector<pat::Jet>); 

  // Loop over jets and compute smeared jets
  for(edm::View<reco::Jet>::const_iterator itInJet = inputJets->begin();
       itInJet != inputJets->end(); ++itInJet) {
    pat::Jet smearedJet( *itInJet );
    smearP4(smearedJet);
    smearedJets->push_back(smearedJet);
  }

  // Sort varied jets in pt
  GreaterByPt<pat::Jet> pTComparator;
  std::sort(smearedJets->begin(),smearedJets->end(),pTComparator);

  // Put varied jets into event
  iEvent.put(smearedJets);
}


void
JetSmearing::smearP4(pat::Jet &jet) const {
  const TH1* hResponse = 0;
  if( findResponseHist(hResponse,jet.eta(),jet.pt()) ) {
    jet.setP4((hResponse->GetRandom())*jet.p4());
  } else {
    std::cout << "  Leaving jet-pt unsmeared"  << std::endl;
  }
}


bool
JetSmearing::findResponseHist(const TH1* &hResp, double eta, double pt) const {
  bool foundHist = false;
  hResp = 0;

  unsigned int iEta = 0;
  unsigned int iPt  = 0;

  if( etaBin(iEta,eta) && ptBin(iPt,pt) ) {
    hResp = responseHists_.at(iEta).at(iPt);
    foundHist = true;
  } else {
    std::cerr << "WARNING in JetSmearing::findResponseHist():" << std::endl;
    std::cerr << "  Jet out-of eta-pt binning" << std::endl;
    std::cerr << "    eta = " << eta << " --> bin = " << iEta << std::endl;
    std::cerr << "    pt  = " << pt << " --> bin = " << iPt << std::endl;
  }

  return foundHist;
}


bool
JetSmearing::bin(unsigned int &bin, double x, const std::vector<double> &edges) const {
  bool xInBinning = false;
  if( x >= edges.front() && x <= edges.back() ) {
    xInBinning = true;
    for(unsigned int i = 1; i < edges.size(); ++i) {
      if( x <= edges.at(i) ) {
	bin = i-1;
	break;
      }
    }
  }

  return xInBinning;      
}


void
JetSmearing::setResponseHistograms(const TString &respHistsFileName, const TString &respHistsNamePrefix) {
  // Open file with response histograms
  edm::FileInPath respHistsFileInPath(respHistsFileName);
  TFile file(respHistsFileInPath.fullPath().c_str(),"READ");

  // Loop over eta and pt bins and retrieve response
  // histogram from file
  for(unsigned int iEta = 0; iEta < binEdgesEta_.size()-1; ++iEta) {
    std::vector<const TH1*> hists;
    for(unsigned int iPt = 0; iPt < binEdgesPt_.size()-1; ++iPt) {
      TH1* h = 0;
      TString name = respHistsNamePrefix+"_Pt";
      name += iPt;
      name += "_Eta";
      name += iEta;
      file.GetObject(name,h);
      if( h ) {
	h->SetDirectory(0);
	// Optionally, convolute with Gaussian
	if( applyCoreScaleFactors_ ) {
	  double eta = 0.5*(binEdgesEta_.at(iEta)+binEdgesEta_.at(iEta+1));
	  convoluteWithGauss(h,eta);
	}
	hists.push_back(h);
      } else {
	std::cerr << "ERROR in JetSmearing::setResponseHistograms():" << std::endl;
	std::cerr << "  Histogram '" << name << "' not found in file '" << file.GetName() << "'" << std::endl;
	exit(-1);
      }	
    }
    responseHists_.push_back(hists);
  }

  file.Close();
}


void
JetSmearing::convoluteWithGauss(TH1* &h, double eta) const {
  // Get scale factor
  double scaling = -1.;
  unsigned int etaBin = 0;
  if( etaBinCoreScaleFactors(etaBin,eta) ) {
    scaling = coreScaleFactors_.at(etaBin);
  }

  if( scaling > 1. ) {
    // Store original response histogram
    TH1* hOrig = static_cast<TH1*>(h->Clone("tmp"));

    // Obtain resolution of central part of response distribution
    double width = coreWidth(h);

    // Width of Gaussian to convolute response histogram with
    scaling = sqrt( scaling*scaling - 1. )*width;

    // Convolute content of h with Gaussian of widht 'scaling'
    h->Reset();
    for(int bin = 1; bin <= hOrig->GetNbinsX(); ++bin) {
      double entries = hOrig->GetBinContent(bin);
      if( entries ) {
	double mean = hOrig->GetBinCenter(bin);
	for(int i = 1; i <= h->GetNbinsX(); ++i) {
	  double min = h->GetXaxis()->GetBinLowEdge(i);
	  double max = h->GetXaxis()->GetBinUpEdge(i);
	  double weight = entries;
	  weight *= 0.5*(erf((max-mean)/sqrt(2.)/scaling) - erf((min-mean)/sqrt(2.)/scaling));
	  h->Fill(h->GetBinCenter(i),weight);
	}
      }
    }
    delete hOrig;
  }
}


double
JetSmearing::coreWidth(TH1* h) const {
  double mean = h->GetMean();
  double rms = h->GetRMS();
  double sig = 2.5*rms;
  if( (h->Fit("gaus","0QIR","",mean-sig,mean+sig)) == 0 ) { // Fit within +/- 1 RMS around mean
    TF1* gauss = h->GetFunction("gaus");
    mean = gauss->GetParameter(1);
    sig = 2.*gauss->GetParameter(2);
    if( (h->Fit("gaus","0QIR","",mean-sig,mean+sig)) == 0 ) { // Fit within +/- 2.sigma around mean
      gauss = h->GetFunction("gaus");
      sig = gauss->GetParameter(2);
    } else {
      sig = h->GetRMS();
    }
  } else {
    sig = h->GetRMS();
  }

  return sig;
}




// ------------ method called once each job just before starting event loop  ------------
void 
JetSmearing::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
JetSmearing::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
JetSmearing::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
JetSmearing::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
JetSmearing::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
JetSmearing::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
JetSmearing::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetSmearing);
