#include "RA2Classic/LostLeptonBkg/interface/PromtIsoMu.h"

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
PromtIsoMu::PromtIsoMu(const edm::ParameterSet& iConfig)
{
   muonIDISOTag_= iConfig.getParameter<edm::InputTag> ("MuonIDISOTag");
   elecIDISOTag_ = iConfig.getParameter<edm::InputTag> ("ElecIDISOTag");
   genTag_ = iConfig.getParameter<edm::InputTag> ("GenTag");


   //if do put with a label
   produces <std::vector <pat::Muon> >("PromtMuon");
   produces <std::vector <pat::Electron> >("PromtElec");
   //if you want to put into the Run


   //now do what ever other initialization is needed
  
}


PromtIsoMu::~PromtIsoMu()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
PromtIsoMu::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   resetVariables();
   using namespace edm;



   edm::Handle < reco::GenParticleCollection > genParticles;
   iEvent.getByLabel(genTag_, genParticles);

   edm::Handle <edm::View <pat::Muon> > MuIDISO;
   iEvent.getByLabel(muonIDISOTag_,MuIDISO);
   edm::Handle <edm::View <pat::Electron> > ElecIDISO;
   iEvent.getByLabel(elecIDISOTag_,ElecIDISO);
   std::auto_ptr< std::vector < pat::Muon> > selectedMuon(new std::vector< pat::Muon >);
   std::auto_ptr< std::vector < pat::Electron> >selectedElec(new std::vector< pat::Electron >);
      for(reco::GenParticleCollection::const_iterator cand = genParticles->begin(); cand!=genParticles->end(); ++cand) 
   {
	// demands only w bosons from the hard interaction
    	if(abs(cand->pdgId())!=24 || cand->status()!=3) continue;
	// only not hadronically decaying w will survive

	for (unsigned int i=0; i < cand->numberOfDaughters(); i++)
	{
		// muon
		if (abs(cand->daughter(i)->pdgId() ) == 13) 
		{
			MuonFound(cand,i);
			if(cand->daughter(i)->status()==2)std::cout<<"muon has status 2"<<std::endl;
		}
		//electrons
		if (abs(cand->daughter(i)->pdgId() ) == 11) 
		{
			ElecFound(cand,i);
			if(cand->daughter(i)->status()==2)std::cout<<"elec has status 2"<<std::endl;
		}
		// tau decay
		if (abs(cand->daughter(i)->pdgId() ) == 15)
		{
			TauFound(cand->daughter(i));
			if(cand->daughter(i)->status()==2)std::cout<<"tau has status 2"<<std::endl;
		}
	
	}
	
   }

// for now not fully correct
if (nGenMu_==1 && nGenElec_ == 0)
{
	for( edm::View <pat::Muon>::const_iterator MuIDIsoCand = MuIDISO->begin(); MuIDIsoCand!=MuIDISO->end();MuIDIsoCand++)
	{
		muonIsoPt_ = MuIDIsoCand->pt();
		muonIsoEta_ = MuIDIsoCand->eta();
		muonIsoPhi_ = MuIDIsoCand->phi();
		// true if Iso is fulfilled
		if (deltaR(muonIsoEta_,muonIsoPhi_,muonGenEta_,muonGenPhi_) < 0.3)
		{	
			selectedMuon->push_back(*MuIDIsoCand);
		}
	}
}


if(nGenElec_>0)
{
	for( edm::View <pat::Electron>::const_iterator ElecIDIsoCand = ElecIDISO->begin(); ElecIDIsoCand!=ElecIDISO->end();ElecIDIsoCand++)
	{
		muonIsoPt_ = ElecIDIsoCand->pt();
		muonIsoEta_ = ElecIDIsoCand->eta();
		muonIsoPhi_ = ElecIDIsoCand->phi();
		// true if Iso is fulfilled
		if (deltaR(elecIsoEta_,elecIsoPhi_,elecGenEta_,elecGenPhi_) < 0.3)
		{	
			selectedElec->push_back(*ElecIDIsoCand);
		}
	}
}












   iEvent.put(selectedMuon, "PromtMuon");
   iEvent.put(selectedElec, "PromtElec");

}

// ------------ method called once each job just before starting event loop  ------------
void 
PromtIsoMu::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PromtIsoMu::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
PromtIsoMu::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
PromtIsoMu::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
PromtIsoMu::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
PromtIsoMu::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PromtIsoMu::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


void
PromtIsoMu::resetVariables()
{
	isPromt_=0;
	muonGenPt_=0;
	muonGenEta_=0;
	muonGenPhi_=0;
	elecGenPt_=0;
	elecGenEta_=0;
	elecGenPhi_=0;
	nGenMu_=0;
	nGenElec_=0;
	muonIsoPt_=0;
	muonIsoEta_=0;
	muonIsoPhi_=0;
	elecIsoEta_=0;
	elecIsoPhi_=0;
}




void
PromtIsoMu::MuonFound(reco::GenParticleCollection::const_iterator cand, unsigned int i)
{
	nGenMu_+=1;
	muonGenPt_  = cand->daughter(i)->pt();
	muonGenEta_ = cand->daughter(i)->eta();
	muonGenPhi_ = cand->daughter(i)->phi();
	// true if the muon is out of the detector acceptance

}

void
PromtIsoMu::ElecFound(reco::GenParticleCollection::const_iterator cand, unsigned int i)
{
	nGenElec_+=1;
		elecGenPt_  = cand->daughter(i)->pt();
		elecGenEta_ = cand->daughter(i)->eta();
		elecGenPhi_ = cand->daughter(i)->phi();

		// true if the elec is out of the detector acceptance


}

void
PromtIsoMu::TauFound(const reco::Candidate* tau)
{


	for (unsigned int ii=0; ii < tau->numberOfDaughters(); ii++)
	{
		// true if the selected tau is not the finally decaying tau but has an intermediate tau or w in the decay
		if (abs(tau->daughter(ii)->pdgId()) == 15 || abs(tau->daughter(ii)->pdgId()) == 24) TauFound(tau->daughter(ii));
		if (abs(tau->daughter(ii)->pdgId() ) == 13 )
		{
			//muon found

			nGenMu_+=1;
			muonGenPt_  = tau->daughter(ii)->pt();
			muonGenEta_ = tau->daughter(ii)->eta();
			muonGenPhi_ = tau->daughter(ii)->phi();

		}
		if (abs(tau->daughter(ii)->pdgId() ) == 11 )
		{
			//elec found

			nGenElec_+=1;
			elecGenPt_  = tau->daughter(ii)->pt();
			elecGenEta_ = tau->daughter(ii)->eta();
			elecGenPhi_ = tau->daughter(ii)->phi();

		}


	}

}




//define this as a plug-in
DEFINE_FWK_MODULE(PromtIsoMu);
