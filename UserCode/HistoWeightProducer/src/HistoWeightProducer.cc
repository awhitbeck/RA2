// -*- C++ -*-
//
// Package:    HistoWeightProducer
// Class:      HistoWeightProducer
// 
/**\class HistoWeightProducer HistoWeightProducer.cc UserCode/HistoWeightProducer/src/HistoWeightProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Stephen Mrenna
//         Created:  Sat Feb  5 07:55:58 CST 2011
// $Id: HistoWeightProducer.cc,v 1.1 2011/02/17 17:48:32 mrenna Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"

//
// class declaration
//

class HistoWeightProducer : public edm::EDProducer {
   public:
      explicit HistoWeightProducer(const edm::ParameterSet&);
      ~HistoWeightProducer();

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
      edm::InputTag src_;
      std::string m_inputFile;
      std::string m_outputFile;
      std::string m_treeName;
//      std::string reweightVariable_;

      TFile* m_inputTFile;
      TFile* m_outputTFile;
      TTree* m_inputTree;
    
      TH1* hrew_;
      TH1* href_;

      StringObjectFunction<reco::Candidate> fReweight_;

};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
HistoWeightProducer::HistoWeightProducer(const edm::ParameterSet& iConfig) :
   src_(iConfig.getParameter<edm::InputTag>("src")),
   m_inputFile(iConfig.getUntrackedParameter< std::string > ("inputFile")),
   m_outputFile(iConfig.getUntrackedParameter< std::string > ("outputFile")),
   m_treeName(iConfig.getUntrackedParameter< std::string > ("treeName")),
   fReweight_(iConfig.getUntrackedParameter< std::string > ("reweightVariable"))

{
   //register your products

//   produces<double>("label");
   produces<double>();

/* Examples
   produces<ExampleData2>();

   //if do put with a label
*/
   //now do what ever other initialization is needed
  
}


HistoWeightProducer::~HistoWeightProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
HistoWeightProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   Handle<std::vector<reco::CompositeCandidate> > pIn;
   iEvent.getByLabel(src_,pIn);

   std::vector<reco::CompositeCandidate>::const_iterator pit=pIn->begin();

//   Double_t htvalue = (*pit).pt();
//   Double_t htvalue = (*pit).energy();
   Double_t htvalue = fReweight_(*pit);

   Int_t iBin = href_->FindBin(htvalue);

   Double_t weight = href_->GetBinContent(iBin);

//   std::cout << " weight = " << weight << " ht = " << htvalue << std::endl;

   std::auto_ptr<double> outWeight(new double(weight));

   iEvent.put(outWeight);


/* This is an event example
   //Read 'ExampleData' from the Event
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);

   //Use the ExampleData to create an ExampleData2 which 
   // is put into the Event
   std::auto_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
   iEvent.put(pOut);
*/

/* this is an EventSetup example
   //Read SetupData from the SetupRecord in the EventSetup
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
*/
 
}

// ------------ method called once each job just before starting event loop  ------------
void 
HistoWeightProducer::beginJob()
{
   m_inputTFile = new TFile(m_inputFile.c_str());
   m_outputTFile = new TFile(m_outputFile.c_str());

   m_inputTFile->GetObject(m_treeName.c_str(),href_);
   m_outputTFile->GetObject(m_treeName.c_str(),hrew_);
   href_ -> Divide( hrew_ );
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HistoWeightProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HistoWeightProducer);
