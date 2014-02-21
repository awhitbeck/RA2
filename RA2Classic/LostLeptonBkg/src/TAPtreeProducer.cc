// -*- C++ -*-
//
// Package:    TAPtreeProducer
// Class:      TAPtreeProducer
// 
/**\class TAPtreeProducer TAPtreeProducer.cc RA2Classic/TAPtreeProducer/src/TAPtreeProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Arne-Rasmus Draeger,,,uni-hamburg
//         Created:  Tue Sep 25 13:40:40 CEST 2012
// $Id: TAPtreeProducer.cc,v 1.3 2013/04/18 11:48:40 adraeger Exp $
//
//
#include <TString.h>
#include "RA2Classic/LostLeptonBkg/interface/TAPtreeProducer.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include <DataFormats/Math/interface/deltaR.h>
#include <DataFormats/PatCandidates/interface/Jet.h>
#include "RA2Classic/LostLeptonBkg/interface/MCEffCalculator.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include <DataFormats/EgammaReco/interface/SuperCluster.h>
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
TAPtreeProducer::TAPtreeProducer(const edm::ParameterSet& iConfig)
{
   HtJetsTag_ = iConfig.getParameter<edm::InputTag>("HTJets");
   MhtTag_ = iConfig.getParameter<edm::InputTag>("MhtTag");
   MetTag_ = iConfig.getParameter<edm::InputTag>("MetTag");

   MuElecIdIso_ = iConfig.getParameter <unsigned int> ("MuElecIdIso");

   IsoMuonTag_ = iConfig.getParameter<edm::InputTag>("IsoMuonTag");
   IsoElecTag_ = iConfig.getParameter<edm::InputTag>("IsoElecTag");

   RecoMuonTag_ = iConfig.getParameter<edm::InputTag>("RecoMuonTag");
   RecoElecTag_ = iConfig.getParameter<edm::InputTag>("RecoElecTag");

   PromtMuonTag_ = iConfig.getParameter<edm::InputTag>("PromtMuonTag");
   PromtElecTag_ = iConfig.getParameter<edm::InputTag>("PromtElecTag");
   GsfElecTag_ = iConfig.getParameter<edm::InputTag>("GsfElecTag");
 
   evtWeightTag_ = iConfig.getParameter<edm::InputTag> ("EventWeightTag");
   CaloJetTag_ = iConfig.getParameter<edm::InputTag>("CaloJetTag");

   // input cuts
   MTWMax_ = iConfig.getParameter <double> ("MTWMax");
   MTWCut_ = iConfig.getParameter <bool> ("MTWCut");
   MinMuPT_ = iConfig.getParameter <double> ("MinMuPT");
   MinElecPT_ = iConfig.getParameter <double> ("MinElecPT");
   HTTag_ = iConfig.getParameter<edm::InputTag>("HTTag");
   MHTTag_ = iConfig.getParameter<edm::InputTag>("MHTTag");
   NVTag_ = iConfig.getParameter<edm::InputTag>("VertexCollection");





}


TAPtreeProducer::~TAPtreeProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
TAPtreeProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
//std::cout<<"TAPtreeProducer::loopstarted"<<std::endl;
   //std::cout<<"TAPtreeProducer:EventLoopStarted"<<std::endl;
   using namespace edm;
   ResetValues();
   
  edm::Handle< edm::View<reco::Candidate> > met;
  iEvent.getByLabel(MetTag_,met);
  if( met.isValid() ) {
    met_ = met->at(0).pt();
  }

// up down for mt cut efficiency


  edm::Handle< edm::View<reco::Candidate> > htJets;
  iEvent.getByLabel(HtJetsTag_,htJets);
  if( htJets.isValid() ) 
  {
	nJets_ = htJets->size();
  }
  edm::Handle< edm::View<reco::Candidate> > mht;
  iEvent.getByLabel(MHTTag_,mht);
  mht_ = mht->at(0).pt();

  edm::Handle<double> eventWeight;
  iEvent.getByLabel(evtWeightTag_,eventWeight);
  eventWeight_ = *eventWeight;
  edm::Handle<double> ht;
  iEvent.getByLabel(HTTag_,ht);
  ht_ = *ht;
  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(NVTag_,vertices);
  if( vertices.isValid() ) 
  {
    	nV_ = vertices->size();
  }

 


  // Muon criteria
  edm::Handle <edm::View <reco::CaloJet> > caloJets;
  iEvent.getByLabel(CaloJetTag_, caloJets);

  edm::Handle <edm::View <reco::Candidate> > IsoMu;
  iEvent.getByLabel(IsoMuonTag_,IsoMu);
  edm::Handle <edm::View <reco::Candidate> > IsoElec;
  iEvent.getByLabel(IsoElecTag_,IsoElec);

  edm::Handle <edm::View <reco::Candidate> > RecoMu;
  iEvent.getByLabel(RecoMuonTag_,RecoMu);
  edm::Handle <edm::View <reco::Candidate> > RecoElec;
  iEvent.getByLabel(RecoElecTag_,RecoElec);
//std::cout<<"promtMuTag:"<<PromtMuonTag_<<std::endl;
  edm::Handle <edm::View <reco::Candidate> > PromtMu;
  iEvent.getByLabel(PromtMuonTag_,PromtMu);

  edm::Handle <reco::SuperClusterCollection >  PromtElec;
  iEvent.getByLabel(PromtElecTag_,PromtElec);
//std::cout<<"PromtElecTag_:"<<PromtElecTag_<<std::endl;

  edm::Handle <edm::View <reco::Candidate> >  GsfElec;
  iEvent.getByLabel(GsfElecTag_,GsfElec);
//std::cout<<"GsfElecTag_:"<<GsfElecTag_<<std::endl;


  if (IsoMu.isValid() )
  {
	nIsoMu_=IsoMu->size();
//	std::cout<<"nIsoMu_"<<nIsoMu_<<std::endl;
  }
else std::cout<<"IsoMu Not valid!"<<std::endl;

  if (RecoMu.isValid() )
  {
	nRecoMu_=RecoMu->size();
//	std::cout<<"nRecoMu_"<<nRecoMu_<<std::endl;
  }
else std::cout<<"RecoMu Not valid!"<<std::endl;

  if (PromtMu.isValid() )
  {
	nPromtMu_=PromtMu->size();
//	std::cout<<"nPromtMu_"<<nPromtMu_<<std::endl;
  }
else std::cout<<"PromtMu Not valid!"<<std::endl;

  if (IsoElec.isValid() )
  {
	nIsoElec_=IsoElec->size();
//	std::cout<<"nIsoElec_"<<nIsoElec_<<std::endl;
  }
else std::cout<<"IsoElec Not valid!"<<std::endl;
  if (RecoElec.isValid() )
  {
	nRecoElec_=RecoElec->size();
//	std::cout<<"nRecoElec_"<<nRecoElec_<<std::endl;
  }
else std::cout<<"RecoElec Not valid!"<<std::endl;
  if (PromtElec.isValid() )
  {
	nPromtElec_=PromtElec->size();
//	std::cout<<"nPromtElec_"<<nPromtElec_<<std::endl;
  }
else std::cout<<"PromtElec Not valid!"<<std::endl;

if (!GsfElec.isValid() )
{
std::cout<<"GsfElec not valid!!"<<std::endl;
}

  



  // set up the result weight


  if (caloJets.isValid() )
  {
	nCaloJets_=caloJets->size();
  }


  // start the prediciton from Muon CS

// loop over all reconstructed muons as tag muons for reconstruction eff
for (edm::View <reco::Candidate>::const_iterator MuIDCand = RecoMu->begin(); MuIDCand!=RecoMu->end();MuIDCand++ )
{	
	if(MuIdPairFound_) continue;
	double tagPt = MuIDCand->pt();
	double tagPhi = MuIDCand->phi();
	double tagEta = MuIDCand->eta();
	double tagE = MuIDCand->energy();
	double tagPx = MuIDCand->px();
	double tagPy = MuIDCand->py();
	double tagPz = MuIDCand->pz();
	TLorentzVector TagLorentz(tagPx, tagPy, tagPz, tagE);
	
	// search for probe muon from PromtMu collection for reconstruction
	for(edm::View <reco::Candidate>::const_iterator MuPromtCand = PromtMu->begin(); MuPromtCand!=PromtMu->end();MuPromtCand++ )
	{
		// if the tag is the probe muon skip the probe muon
		if ( deltaR(tagEta,tagPhi,MuPromtCand->eta(),MuPromtCand->phi())<0.2 &&abs(tagPt - MuPromtCand->pt() )/tagPt <0.5) 
		{
		continue;
//		std::cout<<"the tag reconstructed muon is the probe promtMu muon skippt"<<std::endl;
		}
//		std::cout<<"The tag reconstructed muon is not the probe continue"<<std::endl;
		double probeE = MuPromtCand->energy();
		double probePx = MuPromtCand->px();
		double probePy = MuPromtCand->py();
		double probePz = MuPromtCand->pz();
		TLorentzVector ProbeLorentz(probePx, probePy, probePz, probeE);
		TLorentzVector v1;
		v1 = TagLorentz + ProbeLorentz;
		RecoMuInvariantMass_ = v1.M();
		MuIdPairFound_=true;
		if ( v1.M()<120 && v1.M()>60 && !MuIdPass_) 
		{
		RecoMuInvariantMass_ = v1.M();
		MuIdProbePt_=MuPromtCand->pt();
		MuIdProbeEta_=MuPromtCand->eta();
		MuIdProbePhi_=MuPromtCand->phi();
		// loop over all Id muons to see if the probe muon can be matched to a Id muon
		for (edm::View <reco::Candidate>::const_iterator MuIDCand2 = RecoMu->begin(); MuIDCand2!=RecoMu->end();MuIDCand2++)
		{
			if (deltaR(MuIDCand2->eta(),MuIDCand2->phi(),MuPromtCand->eta(),MuPromtCand->phi())<0.2 && abs(MuIDCand2->pt()-MuPromtCand->pt())/MuIDCand2->pt()<0.5 ) MuIdPass_=true;
		}
		}
		
//		std::cout<<"Invariant mass"<<v1.M();
		
		
	}

}





// loop over all isolated muons as tag muons for isolated eff
for (edm::View <reco::Candidate>::const_iterator MuISOCand = IsoMu->begin(); MuISOCand!=IsoMu->end();MuISOCand++ )
{	
	if(MuIsoPairFound_) continue;
	double tagPt = MuISOCand->pt();
	double tagPhi = MuISOCand->phi();
	double tagEta = MuISOCand->eta();
	double tagE = MuISOCand->energy();
	double tagPx = MuISOCand->px();
	double tagPy = MuISOCand->py();
	double tagPz = MuISOCand->pz();
	TLorentzVector TagLorentz(tagPx, tagPy, tagPz, tagE);
	
	// search for probe muon from RecoMu collection for reconstruction
	for(edm::View <reco::Candidate>::const_iterator MuRecoCand = RecoMu->begin(); MuRecoCand!=RecoMu->end();MuRecoCand++ )
	{
		// if the tag is the probe muon skip the probe muon
		if ( deltaR(tagEta,tagPhi,MuRecoCand->eta(),MuRecoCand->phi())<0.2 && abs(tagPt - MuRecoCand->pt() )/tagPt <0.5 ) continue;
		double probeE = MuRecoCand->energy();
		double probePx = MuRecoCand->px();
		double probePy = MuRecoCand->py();
		double probePz = MuRecoCand->pz();
		TLorentzVector ProbeLorentz(probePx, probePy, probePz, probeE);
		TLorentzVector v1;

		v1 = TagLorentz + ProbeLorentz;
		MuIsoPairFound_=true;
		IsoMuInvariantMass_ = v1.M();
		if ( v1.M()<120 && v1.M()>60 && !MuIsoPass_) 
		{
		IsoMuInvariantMass_ = v1.M();
		MuIsoProbePt_=MuRecoCand->pt();
		MuIsoProbeEta_=MuRecoCand->eta();
		MuIsoProbePhi_=MuRecoCand->phi();
		// loop over all Id muons to see if the probe muon can be matched to a Id muon
		for (edm::View <reco::Candidate>::const_iterator MuISOCand2 = IsoMu->begin(); MuISOCand2!=IsoMu->end();MuISOCand2++)
		{
			if (deltaR(MuISOCand2->eta(),MuISOCand2->phi(),MuRecoCand->eta(),MuRecoCand->phi())<0.2   && abs(MuISOCand2->pt()-MuRecoCand->pt())/MuISOCand2->pt()<0.5 ) MuIsoPass_=true;
		}
		}
		
//		std::cout<<"Invariant mass"<<v1.M();
		
		
	}

}




// loop over all reconstructed elecs as tag elecs for reconstruction eff
for (edm::View <reco::Candidate>::const_iterator ElecIDCand = RecoElec->begin(); ElecIDCand!=RecoElec->end();ElecIDCand++ )
{	
	if(ElecIdPairFound_) continue;
	double tagPt = ElecIDCand->pt();
	double tagPhi = ElecIDCand->phi();
	double tagEta = ElecIDCand->eta();
	double tagE = ElecIDCand->energy();
	double tagPx = ElecIDCand->px();
	double tagPy = ElecIDCand->py();
	double tagPz = ElecIDCand->pz();
	TLorentzVector TagLorentz(tagPx, tagPy, tagPz, tagE);
	

	reco::SuperClusterCollection::const_iterator ElecPromtCand;
	// search for probe elec from PromtElec collection for reconstruction
	for(ElecPromtCand=PromtElec->begin() ;  ElecPromtCand!=PromtElec->end() ; ElecPromtCand++)
	{
		// if the tag is the probe elec skip the probe elec
		double probeE = ElecPromtCand->energy();
		double eta = ElecPromtCand->eta();
		double phi = ElecPromtCand->phi();
		double theta = atan(exp(-eta)) * 2;
		if(deltaR(eta,phi,tagEta,tagPhi)<0.3 && abs(tagE-probeE)/tagE <0.5) continue;

		double probePx = probeE * sin(theta) * cos(phi);
		double probePy = probeE * sin(theta) * sin(phi);
		double probePz = probeE * cos(theta);
		TLorentzVector ProbeLorentz(probePx, probePy, probePz, probeE);
		TLorentzVector v1;
		v1 = TagLorentz + ProbeLorentz;
		RecoElecInvariantMass_ = v1.M();
		ElecIdPairFound_=true;
		if ( v1.M()<120 && v1.M()>60 && !ElecIdPass_) 
		{
		RecoElecInvariantMass_ = v1.M();
		ElecIdProbePt_=sqrt(probePx*probePx + probePy * probePy);
		ElecIdProbeEta_=ElecPromtCand->eta();
		ElecIdProbePhi_=ElecPromtCand->phi();
		// loop over all Id elecs to see if the probe elec can be matched to a Id elec
		for (edm::View <reco::Candidate>::const_iterator ElecIDCand2 = RecoElec->begin(); ElecIDCand2!=RecoElec->end();ElecIDCand2++)
		{
			if (deltaR(ElecIDCand2->eta(),ElecIDCand2->phi(),ElecPromtCand->eta(),ElecPromtCand->phi())<0.3   && abs(ElecIDCand2->pt()-ElecIdProbePt_)/ElecIDCand2->pt()<0.5) ElecIdPass_=true;
		}
		}
		
//		std::cout<<"Invariant mass"<<v1.M();
		
		
	}

}



// loop over all reconstructed elecs as tag elecs for reconstruction eff probe::gsfElectrons
for (edm::View <reco::Candidate>::const_iterator ElecIDCand = RecoElec->begin(); ElecIDCand!=RecoElec->end();ElecIDCand++ )
{	// make sure only one pair is found and saved
	std::cout<<"ElecIDGSf loop started, elecIdPairFound:"<<ElecIdGsfPairFound_<<std::endl;
	if(ElecIdGsfPairFound_) continue;
	double tagPt = ElecIDCand->pt();
	double tagPhi = ElecIDCand->phi();
	double tagEta = ElecIDCand->eta();
	double tagE = ElecIDCand->energy();
	double tagPx = ElecIDCand->px();
	double tagPy = ElecIDCand->py();
	double tagPz = ElecIDCand->pz();
	TLorentzVector TagLorentz(tagPx, tagPy, tagPz, tagE);
	
	// search for probe elec from RecoElec collection for reconstruction
	for(edm::View <reco::Candidate>::const_iterator ElecGsfCand = GsfElec->begin(); ElecGsfCand!=GsfElec->end();ElecGsfCand++ )
	{
		// if the tag is the probe elec skip the probe elec
		if ( deltaR(tagEta,tagPhi,ElecGsfCand->eta(),ElecGsfCand->phi())<0.3 && abs(tagPt - ElecGsfCand->pt() )/tagPt <0.5 ) continue;
		double probeE = ElecGsfCand->energy();
		double probePx = ElecGsfCand->px();
		double probePy = ElecGsfCand->py();
		double probePz = ElecGsfCand->pz();
		TLorentzVector ProbeLorentz(probePx, probePy, probePz, probeE);
		TLorentzVector v1;
		v1 = TagLorentz + ProbeLorentz;
		RecoGsfElecInvariantMass_ = v1.M();
		ElecIdGsfPairFound_=true;
		if ( v1.M()<120 && v1.M()>60 && !ElecIdGsfPass_) 
		{
		RecoGsfElecInvariantMass_ = v1.M();
		ElecIdGsfProbePt_=ElecGsfCand->pt();
		ElecIdGsfProbeEta_=ElecGsfCand->eta();
		ElecIdGsfProbePhi_=ElecGsfCand->phi();
		// loop over all Id elecs to see if the probe elec can be matched to a Id elec
		for (edm::View <reco::Candidate>::const_iterator ElecIDCand2 = RecoElec->begin(); ElecIDCand2!=RecoElec->end();ElecIDCand2++)
		{
			if (deltaR(ElecIDCand2->eta(),ElecIDCand2->phi(),ElecGsfCand->eta(),ElecGsfCand->phi())<0.3 && abs(ElecIDCand2->pt()-ElecGsfCand->pt())/ElecIDCand2->pt()<0.5  ) ElecIdGsfPass_=true;
		}
		}
		
//		std::cout<<"Invariant mass"<<v1.M();
		
		
	}

}






// loop over all isolated elecs as tag elecs for reconstruction eff
for (edm::View <reco::Candidate>::const_iterator ElecISOCand = IsoElec->begin(); ElecISOCand!=IsoElec->end();ElecISOCand++ )
{	
	double tagPt = ElecISOCand->pt();
	double tagPhi = ElecISOCand->phi();
	double tagEta = ElecISOCand->eta();
	double tagE = ElecISOCand->energy();
	double tagPx = ElecISOCand->px();
	double tagPy = ElecISOCand->py();
	double tagPz = ElecISOCand->pz();
	TLorentzVector TagLorentz(tagPx, tagPy, tagPz, tagE);
	
	// search for probe elec from RecoElec collection for reconstruction
	for(edm::View <reco::Candidate>::const_iterator ElecRecoCand = RecoElec->begin(); ElecRecoCand!=RecoElec->end();ElecRecoCand++ )
	{
		// if the tag is the probe elec skip the probe elec
		if ( deltaR(tagEta,tagPhi,ElecRecoCand->eta(),ElecRecoCand->phi())<0.3 && abs(tagPt - ElecRecoCand->pt() )/tagPt <0.5 ) continue;
		double probeE = ElecRecoCand->energy();
		double probePx = ElecRecoCand->px();
		double probePy = ElecRecoCand->py();
		double probePz = ElecRecoCand->pz();
		TLorentzVector ProbeLorentz(probePx, probePy, probePz, probeE);
		TLorentzVector v1;
		v1 = TagLorentz + ProbeLorentz;
		IsoElecInvariantMass_ = v1.M();
		ElecIsoPairFound_=true;
		if ( v1.M()<120 && v1.M()>60 && !ElecIsoPass_) 
		{
		IsoElecInvariantMass_ = v1.M();
		ElecIsoProbePt_=ElecRecoCand->pt();
		ElecIsoProbeEta_=ElecRecoCand->eta();
		ElecIsoProbePhi_=ElecRecoCand->phi();
		// loop over all Id elecs to see if the probe elec can be matched to a Id elec
		for (edm::View <reco::Candidate>::const_iterator ElecISOCand2 = IsoElec->begin(); ElecISOCand2!=IsoElec->end();ElecISOCand2++)
		{
			if (deltaR(ElecISOCand2->eta(),ElecISOCand2->phi(),ElecRecoCand->eta(),ElecRecoCand->phi())<0.3 && abs(ElecISOCand2->pt()-ElecRecoCand->pt())/ElecISOCand2->pt()<0.5  ) ElecIsoPass_=true;
		}
		}
		
//		std::cout<<"Invariant mass"<<v1.M();
		
		
	}

}



 

if (false)
{

	nMu_ = IsoMu->size();
//	std::cout<<"TAPtreeProducer:MuTag="<<MuonTag_<<std::endl;
	if (nMu_!=1) std::cout<<"Number of Muons="<<nMu_<<std::endl;
	MuPt_ = IsoMu->at(0).pt();
	MuEta_ = IsoMu->at(0).eta();
	MuPhi_ = IsoMu->at(0).phi();

}

if ( (MuElecIdIso_==0 && MuIdPairFound_) || (MuElecIdIso_==1 && MuIsoPairFound_) || (MuElecIdIso_==2 && ElecIdPairFound_) || (MuElecIdIso_==3 && ElecIsoPairFound_) || (MuElecIdIso_==4 && ElecIdGsfPairFound_))
{
   tree_->Fill();
}
}

// ------------ method called once each job just before starting event loop  ------------
void 
TAPtreeProducer::beginJob()
{
std::cout<<"TAPtreeProducer::beginJob:started"<<std::endl;
    	edm::Service<TFileService> fs;
  	tree_ = fs->make<TTree>("TAPtreeProducer","TAPtreeProducer");
    // plotting values
    tree_->Branch("HT",&ht_,"HT/F");
    tree_->Branch("MHT",&mht_,"MHT/F");
    tree_->Branch("NJets",&nJets_,"NJets/i");
    tree_->Branch("Weight",&eventWeight_,"Weight/F");
    tree_->Branch("NVtx",&nV_,"NVtx/s");
    tree_->Branch("MT",&met_,"MT/F");
//    tree_->Branch("NVtx",&nV_,"NVtx/s");
   tree_->Branch("tagIsoMu",&tagIsoMu_,"tagIsoMu/F");
   tree_->Branch("tagIsoElec",&tagIsoElec_,"tagIsoElec/F");

   tree_->Branch("probeIsoMu",&probeIsoMu_,"probeIsoMu/F");
   tree_->Branch("probeIsoElec",&probeIsoElec_,"probeIsoElec/F");

   tree_->Branch("nIsoMu",&nIsoMu_,"nIsoMu/s");
   tree_->Branch("nRecoMu",&nRecoMu_,"nRecoMu/s");
   tree_->Branch("nPromtMu",&nPromtMu_,"nPromtMu/s");

   tree_->Branch("nIsoElec",&nIsoElec_,"nIsoElec/s");
   tree_->Branch("nRecoElec",&nRecoElec_,"nRecoElec/s");
   tree_->Branch("nPromtElec",&nPromtElec_,"nPromtElec/s");
if(MuElecIdIso_==1)
{
   tree_->Branch("IsoMuInvariantMass",&IsoMuInvariantMass_,"IsoMuInvariantMass/F");
   tree_->Branch("MuIsoPass",&MuIsoPass_,"MuIsoPass/I");
   tree_->Branch("MuIsoProbePt",&MuIsoProbePt_,"MuIsoProbePt/F");
   tree_->Branch("MuIsoProbeEta",&MuIsoProbeEta_,"MuIsoProbeEta/F");
   tree_->Branch("MuIsoProbePhi",&MuIsoProbePhi_,"MuIsoProbePhi/F");
}
if(MuElecIdIso_==0)
{
   tree_->Branch("RecoMuInvariantMass",&RecoMuInvariantMass_,"RecoMuInvariantMass/F");
   tree_->Branch("MuIdPass",&MuIdPass_,"MuIdPass/I");
   tree_->Branch("MuIdProbePt",&MuIdProbePt_,"MuIdProbePt/F");
   tree_->Branch("MuIdProbeEta",&MuIdProbeEta_,"MuIdProbeEta/F");
   tree_->Branch("MuIdProbePhi",&MuIdProbePhi_,"MuIdProbePhi/F");
}
if(MuElecIdIso_==3)
{
   tree_->Branch("IsoElecInvariantMass",&IsoElecInvariantMass_,"IsoElecInvariantMass/F");
   tree_->Branch("ElecIsoPass",&ElecIsoPass_,"ElecIsoPass/I");
   tree_->Branch("ElecIsoProbePt",&ElecIsoProbePt_,"ElecIsoProbePt/F");
   tree_->Branch("ElecIsoProbeEta",&ElecIsoProbeEta_,"ElecIsoProbeEta/F");
   tree_->Branch("ElecIsoProbePhi",&ElecIsoProbePhi_,"ElecIsoProbePhi/F");
}
if(MuElecIdIso_==2)
{
   tree_->Branch("RecoElecInvariantMass",&RecoElecInvariantMass_,"RecoElecInvariantMass/F");
   tree_->Branch("ElecIdPass",&ElecIdPass_,"ElecIdPass/I");
   tree_->Branch("ElecIdProbePt",&ElecIdProbePt_,"ElecIdProbePt/F");
   tree_->Branch("ElecIdProbeEta",&ElecIdProbeEta_,"ElecIdProbeEta/F");
   tree_->Branch("ElecIdProbePhi",&ElecIdProbePhi_,"ElecIdProbePhi/F");
}
if(MuElecIdIso_==4)
{
   tree_->Branch("RecoGsfElecInvariantMass",&RecoGsfElecInvariantMass_,"RecoGsfElecInvariantMass/F");
   tree_->Branch("ElecIdGsfPass",&ElecIdGsfPass_,"ElecIdGsfPass/I");
   tree_->Branch("ElecIdGsfProbePt",&ElecIdGsfProbePt_,"ElecIdGsfProbePt/F");
   tree_->Branch("ElecIdGsfProbeEta",&ElecIdGsfProbeEta_,"ElecIdGsfProbeEta/F");
   tree_->Branch("ElecIdGsfProbePhi",&ElecIdGsfProbePhi_,"ElecIdGsfProbePhi/F");
}
   tree_->Branch("MuIdPairFound",&MuIdPairFound_,"MuIdPairFound/b");
   tree_->Branch("MuIsoPairFound",&MuIsoPairFound_,"MuIsoPairFound/b");
   tree_->Branch("ElecIdPairFound",&ElecIdPairFound_,"ElecIdPairFound/b");
   tree_->Branch("ElecIdGsfPairFound",&ElecIdGsfPairFound_,"ElecIdGsfPairFound/b");
   tree_->Branch("ElecIsoPairFound",&ElecIsoPairFound_,"ElecIsoPairFound/b");

   
	// x check values;


// binByBin eff seperately








	// tag and prob unceratinties
//    tree_->Branch("muonRecoTAPUp",&muonRecoTAPUp_,"muonRecoTAPUp/F");
//    tree_->Branch("muonRecoTAPDown",&muonRecoTAPDown_,"muonRecoTAPDown/F");
//    tree_->Branch("muonIsoTAPUp",&muonIsoTAPUp_,"muonIsoTAPUp/F");
//    tree_->Branch("muonIsoTAPDown",&muonIsoTAPDown_,"muonIsoTAPDown/F");

//    tree_->Branch("elecRecoTAPUp",&elecRecoTAPUp_,"elecRecoTAPUp/F");
//    tree_->Branch("elecRecoTAPDown",&elecRecoTAPDown_,"elecRecoTAPDown/F");
//    tree_->Branch("elecIsoTAPUp",&elecIsoTAPUp_,"elecIsoTAPUp/F");
//    tree_->Branch("elecIsoTAPDown",&elecIsoTAPDown_,"elecIsoTAPDown/F");


std::cout<<"TAPtreeProducer::beginJob:finished"<<std::endl;
}
void 
TAPtreeProducer::ResetValues()
{
	met_ =-1;
	mht_ = -1;
	mtw_ =-1;
	nMu_ = -1;
	MuPt_ = -1;
	MuEta_ =-1;
	MuPhi_ = -1;
	nJets_ = -1;
	nCaloJets_ = -1;	

	eventWeight_=0;
	error_=0;

	tagIsoMu_=0;
	probeIsoMu_=0;

	nIsoMu_=0;
	nRecoMu_=0;
	nPromtMu_=0;

	nIsoElec_=0;
	nRecoElec_=0;
	nPromtElec_=0;

	IsoMuInvariantMass_=0;
	RecoMuInvariantMass_=0;
	MuIdPass_=false;
	MuIsoPass_=false;

	IsoElecInvariantMass_=0;
	RecoElecInvariantMass_=0;
	RecoGsfElecInvariantMass_=0;
	ElecIdPass_=false;
	ElecIdGsfPass_=false;
	ElecIsoPass_=false;

	ElecIsoProbePt_=0;
	ElecIsoProbePhi_=0;
	ElecIsoProbeEta_=0;
	ElecIdProbePt_=0;
	ElecIdProbePhi_=0;
	ElecIdProbeEta_=0;
	ElecIdGsfProbePt_=0;
	ElecIdGsfProbePhi_=0;
	ElecIdGsfProbeEta_=0;

	MuIsoProbePt_=0;
	MuIsoProbePhi_=0;
	MuIsoProbeEta_=0;
	MuIdProbePt_=0;
	MuIdProbePhi_=0;
	MuIdProbeEta_=0;

MuIdPairFound_=false;
MuIsoPairFound_=false;
ElecIdPairFound_=false;
ElecIsoPairFound_=false;
ElecIdGsfPairFound_=false;


}
// ------------ method called once each job just after ending the event loop  ------------
void 
TAPtreeProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
TAPtreeProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
TAPtreeProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
TAPtreeProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
TAPtreeProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TAPtreeProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TAPtreeProducer);
