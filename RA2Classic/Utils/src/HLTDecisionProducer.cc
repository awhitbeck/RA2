
// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"


class HLTDecisionProducer : public edm::EDProducer {
public:
  explicit HLTDecisionProducer(const edm::ParameterSet&);
  ~HLTDecisionProducer() {};

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
  const edm::InputTag hltResultsTag_;
  const std::string hltName_;
};

//
// constructors and destructor
//
HLTDecisionProducer::HLTDecisionProducer(const edm::ParameterSet& iConfig) :
  hltResultsTag_(iConfig.getParameter<edm::InputTag>("HLTResultsTag")),
  hltName_(iConfig.getParameter<std::string>("HLTName")+"_v")
{
  produces<bool>();  
}

// ------------ method called to produce the data  ------------
void
HLTDecisionProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  bool passesTrig = false;

  edm::Handle<edm::TriggerResults> triggerResults;
  if( iEvent.getByLabel(hltResultsTag_,triggerResults) ) {
    const edm::TriggerNames& trigNames = iEvent.triggerNames(*triggerResults);
    for(unsigned int itrig = 0; itrig != triggerResults->size(); ++itrig) {
      std::string trigName = trigNames.triggerName(itrig);
      if( trigName.find(hltName_) != std::string::npos ) {
	passesTrig = triggerResults->accept(itrig);
// 	std::cout << "\nFound trigger '" << trigName << "': " << std::flush;
// 	if( passesTrig ) std::cout << "passed" << std::endl;
// 	else std::cout << "rejected" << std::endl;
      }
    }
  }

  std::auto_ptr<bool> pOut(new bool(passesTrig));
  iEvent.put(pOut);
}

// ------------ method called once each job just before starting event loop  ------------
void 
HLTDecisionProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HLTDecisionProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
HLTDecisionProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
HLTDecisionProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
HLTDecisionProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
HLTDecisionProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HLTDecisionProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HLTDecisionProducer);
