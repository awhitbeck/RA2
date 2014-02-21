// -*- C++ -*-
//
// Package:    JESChange
// Class:      JESChange
// 
/**\class JESChange JESChange.cc RA2Classic/Utils/src/JESChange.cc

Description: Vary jet pt by its uncertainty and produce new jet collection.

*/
//
// Original Author:  Matthias Schroeder,,,
//         Created:  Mon Sep 17 16:52:55 CEST 2012
// $Id: JESChange.cc,v 1.2 2012/12/20 19:44:36 seema Exp $
//
//


// system include files
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

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/Utils/interface/PtComparator.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"


//
// class declaration
//

class JESChange : public edm::EDProducer {
public:
  explicit JESChange(const edm::ParameterSet&);
  ~JESChange() {};

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
  const edm::InputTag inputJetsTag_;
  const edm::InputTag inputMetsTag_;
  const std::string jecLevel_;
  bool debug_;
};

//
// constructors and destructor
//
JESChange::JESChange(const edm::ParameterSet& iConfig) :
  inputJetsTag_(iConfig.getParameter<edm::InputTag>("Jets")),
  inputMetsTag_(iConfig.getParameter<edm::InputTag>("METs")),
  jecLevel_(iConfig.getParameter<std::string>("JECLevel")),
  debug_(iConfig.getParameter<bool>("Debug"))
{
  produces< std::vector<pat::Jet> >("");
  produces< std::vector<pat::MET> >("");
}


// ------------ method called to produce the data  ------------
void
JESChange::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // Get the jet collection from the event
  edm::Handle< edm::View<pat::Jet> > inputJets;
  iEvent.getByLabel(inputJetsTag_,inputJets);

  // Get the vector of JEC-corrected (type 1) METs
  edm::Handle< edm::View<reco::MET> > inputMets;
  iEvent.getByLabel(inputMetsTag_,inputMets);

  // Collection of varied jets
  std::auto_ptr< std::vector<pat::Jet> > varJets(new std::vector<pat::Jet>); 

  //Vectorial difference between normal jets and modified jets, for MET corr.
  reco::MET::LorentzVector change(0,0,0,0);

  // Loop over jet collection, retrieve JEC factor from the
  // jets 4-momenta, and store corrected jet. Details at
  // https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#CorrOnTheFly
  const JetCorrector* corrector = JetCorrector::getJetCorrector(jecLevel_,iSetup);
  for(edm::View<pat::Jet>::const_iterator itInJet = inputJets->begin();
      itInJet != inputJets->end(); ++itInJet) {

    //if( itInJet->pt()< 30.0 ) continue;
    // Label for uncorrected jet level
    std::string labelLevel0 = itInJet->availableJECLevels().at(0);
    
    if( debug_ ) {
      std::cout << "JEC levels " << std::flush;
      for(unsigned int ii=0; ii<itInJet->availableJECLevels().size(); ii++) 
	std::cout << itInJet->availableJECLevels().at(ii) << " ";
      std::cout << std::endl;
    }
    // Create varied jet; first with uncorrected pt
    pat::Jet varJet = itInJet->correctedJet(labelLevel0);
    if( debug_ ) 
      std::cout << " >>> rawpt " << varJet.pt() << std::flush;

    // Retrieve correction factor from database
    double corrFactor = corrector->correction(varJet,iEvent,iSetup);

    if( debug_ ) 
      std::cout << " x (" << corrFactor << " = " << itInJet->pt()/varJet.pt() << ") " << std::flush;

    // Scale uncorrected p4 with new JEC
    //    pat::Jet varJet( *itInJet );
    //    varJet.setP4( corrFactor * itInJet->correctedP4("uncorrected") );
    varJet.scaleEnergy(corrFactor);
    varJets->push_back(varJet);

    if( debug_ ) 
      std::cout << " -->newcorrpt " << varJet.pt() << " <--> oldcorrpt " << itInJet->pt() << ", " << std::abs(varJet.eta()) << std::endl;
    
    //keep track of the change, in order to correct JEC-corrected MET, too.
    change += ( varJet.p4() - itInJet->p4() );
  }

  // Sort varied jets in pt
  GreaterByPt<pat::Jet> pTComparator;
  std::sort(varJets->begin(),varJets->end(),pTComparator);

  // Put varied jets into event
  iEvent.put(varJets);

  //recalculate MET taking intoaccount the new jet-collection for type 1:
  pat::MET inMet( *(inputMets->begin()));
  if( debug_ ) std::cout << "inMet " << inMet.p4().pt() << " " << inMet.p4().phi() << std::flush;

  change = reco::MET::LorentzVector(change.x()+inMet.p4().x(),
				     change.y()+inMet.p4().y(),
				     0,
				     sqrt( (change.x()+inMet.p4().x())*(change.x()+inMet.p4().x()) + (change.y()+inMet.p4().y())*(change.y()+inMet.p4().y()) ) );
  pat::MET varMET( reco::MET( change.pt(), 
			      inMet.mEtCorr(),
			      change,
			      inMet.vertex() ) );
  
  if( debug_ ) std::cout << "  newMET " << varMET.pt() << " " << varMET.phi() << std::endl;
  // put mod. MET into event
  std::auto_ptr< std::vector<pat::MET> > varMETs(new std::vector<pat::MET>);
  varMETs->push_back( varMET );
  iEvent.put(varMETs);
}

// ------------ method called once each job just before starting event loop  ------------
void 
JESChange::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
JESChange::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
JESChange::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
JESChange::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
JESChange::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
JESChange::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
JESChange::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(JESChange);
