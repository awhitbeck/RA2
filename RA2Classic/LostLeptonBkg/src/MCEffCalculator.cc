// -*- C++ -*-
//
// Package:    MCEffCalculator
// Class:      MCEffCalculator
// 
/**\class MCEffCalculator MCEffCalculator.cc RA2Classic/MCEffCalculator/src/MCEffCalculator.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Arne-Rasmus Draeger,,,uni-hamburg
//         Created:  Thu Sep 27 10:50:02 CEST 2012
// $Id: MCEffCalculator.cc,v 1.22 2013/04/18 11:48:40 adraeger Exp $
//
//


// system include files
#include "RA2Classic/LostLeptonBkg/interface/MCEffCalculator.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include <DataFormats/Math/interface/deltaR.h>
//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MCEffCalculator::MCEffCalculator(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  treeName_ = iConfig.getParameter<std::string>("TreeName");
  evtWeightTag_ = iConfig.getParameter<edm::InputTag> ("EventWeightTag");
  // muon elec tags
  muonIDTag_ = iConfig.getParameter<edm::InputTag> ("MuonIDTag");
  muonIDISOTag_= iConfig.getParameter<edm::InputTag> ("MuonIDISOTag");
  muonPromtMatchedTag_= iConfig.getParameter<edm::InputTag> ("MuonPromtMatchedTag");
  elecIDTag_ = iConfig.getParameter<edm::InputTag> ("ElecIDTag");
  elecIDISOTag_ = iConfig.getParameter<edm::InputTag> ("ElecIDISOTag");
  caloJetTag_ =iConfig.getParameter<edm::InputTag> ("CaloJetTag");
  metTag_ = iConfig.getParameter<edm::InputTag> ("METTag");
  genTag_ = iConfig.getParameter<edm::InputTag> ("GenTag"); 
  minMuonPt_ = iConfig.getParameter<double> ("minMuonPt");
  maxMuonEta_ = iConfig.getParameter<double> ("maxMuonEta");
  minElecPt_ = iConfig.getParameter<double> ("minElecPt");
  maxElecEta_ =iConfig.getParameter<double> ("maxElecEta");
  minJetPt_ = iConfig.getParameter<double> ("minJetPt") ;

// addiontial plot related variables
   HtJetsTag_ = iConfig.getParameter<edm::InputTag>("HTJetsTag");
   MhtJetsTag_ = iConfig.getParameter<edm::InputTag>("MhtJetsTag");
   HTTag_ = iConfig.getParameter<edm::InputTag>("HTTag");
   MHTTag_ = iConfig.getParameter<edm::InputTag>("MHTTag");
   NVTag_ = iConfig.getParameter<edm::InputTag>("VertexCollection");

}


MCEffCalculator::~MCEffCalculator()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MCEffCalculator::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   // resets all the values
   ResetValues();
   using namespace edm;
   edm::Handle < reco::GenParticleCollection > genParticles;
   iEvent.getByLabel(genTag_, genParticles);
   // get a collection of reco muons
   edm::Handle <edm::View <reco::Candidate> > MuID;
   iEvent.getByLabel(muonIDTag_,MuID);
   edm::Handle <edm::View <reco::Candidate> > MuIDISO;
   iEvent.getByLabel(muonIDISOTag_,MuIDISO);

   edm::Handle <edm::View <reco::Candidate> > MuPromtMatched;
   iEvent.getByLabel(muonPromtMatchedTag_,MuPromtMatched);

   // get a collection of reco elec.
   edm::Handle <edm::View <reco::Candidate> > ElecID;
   iEvent.getByLabel(elecIDTag_,ElecID);
   edm::Handle <edm::View <reco::Candidate> > ElecIDISO;
   iEvent.getByLabel(elecIDISOTag_,ElecIDISO);
   // get Event weight
   edm::Handle<double> eventWeight;
   iEvent.getByLabel(evtWeightTag_,eventWeight);
   eventWeight_ = *eventWeight;

   edm::Handle< edm::View<reco::Candidate> > met;
   iEvent.getByLabel(metTag_,met);
   mt_= met->at(0).pt();



// addiontial input for plotting
  edm::Handle< edm::View<reco::Candidate> > htJets;
  iEvent.getByLabel(HtJetsTag_,htJets);

  nJets_ = htJets->size();

  edm::Handle< edm::View<reco::Candidate> > mhtJets;
  iEvent.getByLabel(MhtJetsTag_,mhtJets);
   

  // HT & MHT
  edm::Handle<double> ht;
  iEvent.getByLabel(HTTag_,ht);

    ht_ = *ht;

  edm::Handle< edm::View<reco::Candidate> > mht;
  iEvent.getByLabel(MHTTag_,mht);

    mht_ = mht->at(0).pt();

  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(NVTag_,vertices);
  if( vertices.isValid() ) 
  {
    	nV_ = vertices->size();
  }

  

  if (MuPromtMatched.isValid() )
  {
	muPromtMatched_ = MuPromtMatched->size();
  }



   // select muon id 13 on gen level
   // iterates over all gen particles
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





genDeltaR_=DRToClosestJet(iEvent, caloJetTag_,genEta_,genPhi_).first;
genPTJet_=DRToClosestJet(iEvent, caloJetTag_,genEta_,genPhi_).second;
genPTRelJet_=genPt_/genPTJet_;


// one muon has been found the three different efficiencies will be determined now. first acc than reco and final iso the results will be stored in th2f histogramms.
   if(nGenElec_==0 && nGenMu_==1 )
//   if (nGenMu_==1 && nGenElec_==0)
   {
	deltaGenR_ = DRToClosestJet(iEvent,caloJetTag_, muonGenEta_, muonGenPhi_).first;
	closestJetToMuonGenPt_ = DRToClosestJet(iEvent,caloJetTag_, muonGenEta_,muonGenPhi_).second;

				RecoMuonDeltaR_=DRToClosestJet(iEvent, caloJetTag_,muonGenEta_,muonGenPhi_).first;
				RecoMuonPTJet_=DRToClosestJet(iEvent, caloJetTag_,muonGenEta_,muonGenPhi_).second;
				RecoMuonPTRelJet_=muonRecoPt_/RecoMuonPTJet_;

	// decided if the muon passed or failed the acc criteria 
	if( std::abs(muonGenEta_)> maxMuonEta_ || muonGenPt_ < minMuonPt_ )
	{	
		MuonAccFailedTH2F_->Fill(mht_,nJets_,eventWeight_);

		MuonAccFailedTH1F_->Fill(muonGenPt_,eventWeight_);
		MuonAccFailed2TH1F_->Fill(mht_,eventWeight_);
		nAccMu_=-1;
		isExpectation_+=1;


		
	}
	// else the muon is within the acceptance
	else 
	{
		nAccMu_=+1;
		// This histogramm is used for the actual acceptance caluclation
		MuonAccPassedTH2F_->Fill(mht_,nJets_,eventWeight_);
		MuonAccPassedTH1F_->Fill(muonGenPt_,eventWeight_);
		MuonAccPassed2TH1F_->Fill(mht_,eventWeight_);
		muonPTAccPassed_=muonGenPt_;
		///////////////////// ID checking
		//check if ID criteria are matched
		muonNumberRecoID_=MuID->size();
		//true if no Mu has been reconstructed
		edm::View <reco::Candidate>::const_iterator itRecoMu = LeptonMatch(*MuID, muonGenEta_, muonGenPhi_,muonGenPt_);
		//true if reco muon matched

		if (itRecoMu != MuID->end() )
			{
			muonRecoPt_=itRecoMu->pt();
			muonRecoEta_ = itRecoMu->eta();
			muonRecoPhi_ = itRecoMu->phi();
			RecoGenMuDeltaR_=deltaR(muonGenEta_,muonGenPhi_,muonRecoEta_,muonRecoPhi_);
		// calculates the mtw for the reconstructed muon
		recoMtw1_= MTWCalculator(iEvent, metTag_,muonRecoPt_,muonRecoPhi_);
		promtRecoTLVMu_.SetPtEtaPhiE(muonRecoPt_,muonRecoEta_,muonRecoPhi_,0);
		promtRecoTLVMET_.SetPtEtaPhiE(met->at(0).pt(),met->at(0).eta(),met->at(0).phi(),0);
		promtRecoTLVWDecay_=promtRecoTLVMu_ + promtRecoTLVMET_;
		
		recoMuDeltaPTClosestJet1_= muonRecoPt_/DRToClosestJet(iEvent, caloJetTag_,muonRecoEta_,muonRecoPhi_).second;	
		recoMuDeltaRClosestJet1_ = DRToClosestJet(iEvent, caloJetTag_,muonRecoEta_,muonRecoPhi_).first;
				nRecoMu_=1;
				RecoMuonDeltaR_=DRToClosestJet(iEvent, caloJetTag_,muonRecoEta_,muonRecoPhi_).first;
				RecoMuonPTJet_=DRToClosestJet(iEvent, caloJetTag_,muonRecoEta_,muonRecoPhi_).second;
				RecoMuonPTRelJet_=muonRecoPt_/RecoMuonPTJet_;
				muonIDPassedTH2F_->Fill(RecoMuonDeltaR_,muonRecoPt_/RecoMuonPTJet_,eventWeight_);
				muonIDPassed2TH2F_->Fill(RecoMuonDeltaR_,muonRecoPt_,eventWeight_);
				// muon Iso check true if there are isolated muons
				for( edm::View <reco::Candidate>::const_iterator MuIDIsoCand = MuIDISO->begin(); MuIDIsoCand!=MuIDISO->end();MuIDIsoCand++)
				{
					muonIsoPt_ = MuIDIsoCand->pt();
					muonIsoEta_ = MuIDIsoCand->eta();
					muonIsoPhi_ = MuIDIsoCand->phi();
					// true if Iso is fulfilled
					if (deltaR(muonIsoEta_,muonIsoPhi_,muonRecoEta_,muonRecoPhi_) < 0.1)
					{	
						isExpectation_=-1;
						nIsoMu_=+1;
						IsoMuonDeltaR_=RecoMuonDeltaR_;
						IsoMuonPTJet_=RecoMuonPTJet_;
						IsoMuonPTRelJet_=muonRecoPt_/RecoMuonPTJet_;
						muonIsoPassedTH2F_->Fill(RecoMuonDeltaR_,muonRecoPt_/RecoMuonPTJet_,eventWeight_);
						muonIsoPassed2TH2F_->Fill(RecoMuonDeltaR_,muonRecoPt_,eventWeight_);
						mtw_= MTWCalculator(iEvent, metTag_,muonIsoPt_,muonIsoPhi_);
						MTWTH2F_->Fill(mtw_,mht_);
//						for (unsigned int i=0; i<nJets_;i++)
//						{
//							if( abs(htJets->at(i).pt() - muonIsoPt_)/muonIsoPt_ < 0.1 && deltaR(htJets->at(i).eta(),htJets->at(i).phi(), muonIsoEta_,muonIsoPhi_) < 0.2 ) nJets_-=1;
//						}



					}
					if(nIsoMu_>0) break;
				}

				if (nIsoMu_==0)
				{
					isExpectation_=1;
					nIsoMu_=-1;
					muonIsoFailedTH2F_->Fill(RecoMuonDeltaR_,muonRecoPt_/RecoMuonPTJet_,eventWeight_);
					muonIsoFailed2TH2F_->Fill(RecoMuonDeltaR_,muonRecoPt_,eventWeight_);
				}
			}
		
		// no gen muon could be matched to a reco muon
		if (nRecoMu_==0)
		{
			nRecoMu_=-1;
			muonIDFailedTH2F_->Fill(deltaGenR_,muonGenPt_/closestJetToMuonGenPt_,eventWeight_);
			muonIDFailed2TH2F_->Fill(deltaGenR_,muonGenPt_,eventWeight_);
			isExpectation_ =1;


		}
		///////////////////// ID checking DONE

	}
   }








// one elec has been found the three different efficiencies will be determined now. first acc than reco and final iso the results will be stored in th2f histogramms.
   if (nGenElec_==1 && nGenMu_==0)
   {

	deltaGenR_ = DRToClosestJet(iEvent,caloJetTag_, elecGenEta_, elecGenPhi_).first;
	closestJetToElecGenPt_ = DRToClosestJet(iEvent,caloJetTag_, elecGenEta_,elecGenPhi_).second;

				RecoElecDeltaR_=DRToClosestJet(iEvent, caloJetTag_,elecGenEta_,elecGenPhi_).first;
				RecoElecPTJet_=DRToClosestJet(iEvent, caloJetTag_,elecGenEta_,elecGenPhi_).second;
				RecoElecPTRelJet_=elecRecoPt_/RecoElecPTJet_;

	// decided if the elec passed or failed the acc criteria 
	if( std::abs(elecGenEta_)> maxElecEta_ || elecGenPt_ < minElecPt_)
	{	
		ElecAccFailedTH2F_->Fill(mht_,nJets_,eventWeight_);

		ElecAccFailedTH1F_->Fill(elecGenPt_,eventWeight_);
		ElecAccFailed2TH1F_->Fill(mht_,eventWeight_);
		nAccElec_=-1;
		isExpectation_+=1;


		
	}
	// else the elec is within the acceptance
	else 
	{
		nAccElec_=+1;
		// This histogramm is used for the actual acceptance caluclation
		ElecAccPassedTH2F_->Fill(mht_,nJets_,eventWeight_);
		ElecAccPassedTH1F_->Fill(elecGenPt_,eventWeight_);
		ElecAccPassed2TH1F_->Fill(mht_,eventWeight_);
		elecPTAccPassed_=elecGenPt_;
		///////////////////// ID checking
		//check if ID criteria are matched
		elecNumberRecoID_=ElecID->size();
		//true if no elec has been reconstructed

		edm::View <reco::Candidate>::const_iterator itRecoElec = LeptonMatch(*ElecID, elecGenEta_, elecGenPhi_,elecGenPt_);
		//true if reco muon matched
		if (itRecoElec != ElecID->end() )
			{
			elecRecoPt_= itRecoElec->pt();
			elecRecoEta_ = itRecoElec->eta();
			elecRecoPhi_ = itRecoElec->phi();
			RecoGenElecDeltaR_=deltaR(elecGenEta_,elecGenPhi_,elecRecoEta_,elecRecoPhi_);	
				nRecoElec_=+1;
				RecoElecDeltaR_=DRToClosestJet(iEvent, caloJetTag_,elecRecoEta_,elecRecoPhi_).first;
				RecoElecPTJet_=DRToClosestJet(iEvent, caloJetTag_,elecRecoEta_,elecRecoPhi_).second;
				RecoElecPTRelJet_=elecRecoPt_/RecoElecPTJet_;
				elecIDPassedTH2F_->Fill(RecoElecDeltaR_,elecRecoPt_/RecoElecPTJet_,eventWeight_);
				elecIDPassed2TH2F_->Fill(RecoElecDeltaR_,elecRecoPt_,eventWeight_);
				// elec Iso check true if there are isolated elec
				if (ElecIDISO->size() > 0 )
				{
					for( edm::View <reco::Candidate>::const_iterator ElecIDIsoCand = ElecIDISO->begin(); ElecIDIsoCand!=ElecIDISO->end();ElecIDIsoCand++)
					{
						elecIsoPt_ = ElecIDIsoCand->pt();
						elecIsoEta_ = ElecIDIsoCand->eta();
						elecIsoPhi_ = ElecIDIsoCand->phi();
						// true if Iso is fulfilled
						if (deltaR(elecIsoEta_,elecIsoPhi_,elecRecoEta_,elecRecoPhi_) < 0.1)
						{
							isExpectation_=-1;
							nIsoElec_=+1;
							IsoElecDeltaR_=RecoElecDeltaR_;
							IsoElecPTJet_=RecoElecPTJet_;
							IsoElecPTRelJet_=elecRecoPt_/RecoElecPTJet_;
							elecIsoPassedTH2F_->Fill(RecoElecDeltaR_,elecRecoPt_/RecoElecPTJet_,eventWeight_);
							elecIsoPassed2TH2F_->Fill(RecoElecDeltaR_,elecRecoPt_,eventWeight_);
//							for (unsigned int i=0; i<nJets_;i++)
//							{
//								if( abs(htJets->at(i).pt() - elecIsoPt_)/elecIsoPt_ < 0.1 && deltaR(htJets->at(i).eta(),htJets->at(i).phi(), elecIsoEta_,elecIsoPhi_) < 0.3 ) nJets_-=1;
//							}


						}
					if(nIsoElec_>0) break;
					}
				}
				if (nIsoElec_==0)
				{
					isExpectation_+=1;
					nIsoElec_=-1;
					elecIsoFailedTH2F_->Fill(RecoElecDeltaR_,elecRecoPt_/RecoElecPTJet_,eventWeight_);
					elecIsoFailed2TH2F_->Fill(RecoElecDeltaR_,elecRecoPt_,eventWeight_);
				}
			}
		// no gen elec could be matched to a reco elec
		if (nRecoElec_==0)
		{
			nRecoElec_=-1;
			elecIDFailedTH2F_->Fill(deltaGenR_,elecGenPt_/closestJetToElecGenPt_,eventWeight_);
			elecIDFailed2TH2F_->Fill(deltaGenR_,elecGenPt_,eventWeight_);
			isExpectation_ =1;
		}
		///////////////////// ID checking DONE

	}

   }

   if(nRecoMu_==-1 || nIsoMu_==-1|| nAccMu_ ==-1)
	{
		totalEffTH3FFailedMu_->Fill(ht_,mht_,nJets_,eventWeight_);
	}
   if (nIsoMu_==1)
	{
		totalEffTH3FPassedMu_->Fill(ht_,mht_,nJets_,eventWeight_);
	}


   if(nRecoElec_==-1 || nIsoElec_==-1|| nAccElec_ ==-1)
	{
		totalEffTH3FFailedElec_->Fill(ht_,mht_,nJets_,eventWeight_);
	}
   if (nIsoElec_==1)
	{
		totalEffTH3FPassedElec_->Fill(ht_,mht_,nJets_,eventWeight_);
	}


// seperate bin by bin eff

   if(nAccMu_ ==-1)
	{
		accEffTH3FFailedMu_->Fill(ht_,mht_,nJets_,eventWeight_);
	}
   if (nAccMu_==1)
	{
		accEffTH3FPassedMu_->Fill(ht_,mht_,nJets_,eventWeight_);
	}


   if(nAccElec_ ==-1)
	{
		accEffTH3FFailedElec_->Fill(ht_,mht_,nJets_,eventWeight_);
	}
   if (nAccElec_ ==1)
	{
		accEffTH3FPassedElec_->Fill(ht_,mht_,nJets_,eventWeight_);
	}

   if(nRecoMu_ ==-1)
	{
		recoEffTH3FFailedMu_->Fill(ht_,mht_,nJets_,eventWeight_);
		// seperate for NJets
		if (nJets_ > 2.5 && 5.5 > nJets_)recoEffTH2FailedMuNJet35_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 5.5 && 7.5 > nJets_)recoEffTH2FailedMuNJet67_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 7.5)recoEffTH2FailedMuNJet8Inf_->Fill(ht_,mht_,eventWeight_);
	}
   if (nRecoMu_==1)
	{
		recoEffTH3FPassedMu_->Fill(ht_,mht_,nJets_,eventWeight_);
		if (nJets_ > 2.5 && 5.5 > nJets_)recoEffTH2PassedMuNJet35_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 5.5 && 7.5 > nJets_)recoEffTH2PassedMuNJet67_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 7.5)recoEffTH2PassedMuNJet8Inf_->Fill(ht_,mht_,eventWeight_);
	}


   if(nRecoElec_ ==-1)
	{
		recoEffTH3FFailedElec_->Fill(ht_,mht_,nJets_,eventWeight_);
		// seperate for NJets
		if (nJets_ > 2.5 && 5.5 > nJets_)recoEffTH2FailedElecNJet35_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 5.5 && 7.5 > nJets_)recoEffTH2FailedElecNJet67_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 7.5)recoEffTH2FailedElecNJet8Inf_->Fill(ht_,mht_,eventWeight_);
	}
   if (nRecoElec_ ==1)
	{
		recoEffTH3FPassedElec_->Fill(ht_,mht_,nJets_,eventWeight_);
		// seperate for NJets
		if (nJets_ > 2.5 && 5.5 > nJets_)recoEffTH2PassedElecNJet35_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 5.5 && 7.5 > nJets_)recoEffTH2PassedElecNJet67_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 7.5)recoEffTH2PassedElecNJet8Inf_->Fill(ht_,mht_,eventWeight_);
	}

   if(nIsoMu_ ==-1)
	{
		isoEffTH3FFailedMu_->Fill(ht_,mht_,nJets_,eventWeight_);
		
		// seperate for NJets
		if (nJets_ > 2.5 && 5.5 > nJets_)isoEffTH2FailedMuNJet35_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 5.5 && 7.5 > nJets_)isoEffTH2FailedMuNJet67_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 7.5)isoEffTH2FailedMuNJet8Inf_->Fill(ht_,mht_,eventWeight_);

	}
   if (nIsoMu_==1)
	{
		isoEffTH3FPassedMu_->Fill(ht_,mht_,nJets_,eventWeight_);
		// seperate for NJets
		if (nJets_ > 2.5 && 5.5 > nJets_)isoEffTH2PassedMuNJet35_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 5.5 && 7.5 > nJets_)isoEffTH2PassedMuNJet67_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 7.5)isoEffTH2PassedMuNJet8Inf_->Fill(ht_,mht_,eventWeight_);
	}


   if(nIsoElec_ ==-1)
	{
		isoEffTH3FFailedElec_->Fill(ht_,mht_,nJets_,eventWeight_);
		// seperate for NJets
		if (nJets_ > 2.5 && 5.5 > nJets_)isoEffTH2FailedElecNJet35_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 5.5 && 7.5 > nJets_)isoEffTH2FailedElecNJet67_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 7.5)isoEffTH2FailedElecNJet8Inf_->Fill(ht_,mht_,eventWeight_);
	}
   if (nIsoElec_ ==1)
	{
		isoEffTH3FPassedElec_->Fill(ht_,mht_,nJets_,eventWeight_);
		// seperate for NJets
		if (nJets_ > 2.5 && 5.5 > nJets_)isoEffTH2PassedElecNJet35_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 5.5 && 7.5 > nJets_)isoEffTH2PassedElecNJet67_->Fill(ht_,mht_,eventWeight_);
		if (nJets_ > 7.5)isoEffTH2PassedElecNJet8Inf_->Fill(ht_,mht_,eventWeight_);
	}


   leptons_ = nGenMu_ + nGenElec_;
// Resets all values to default if more than 1 gen muon or gen elec is found in the event
   if (nGenMu_==2 || (nGenMu_+nGenElec_)>1)
   {

   }
   if (nGenElec_==2 || (nGenMu_+nGenElec_) > 1)
   {

   }

    nMuInIsoMuCollection_= MuIDISO->size();
	// this GEN pt is not GEN anymore it is reco pt
    if(nGenMu_==0 && nMuInIsoMuCollection_>0)
	{
	for( edm::View <reco::Candidate>::const_iterator MuIDIsoCand = MuIDISO->begin(); MuIDIsoCand!=MuIDISO->end();MuIDIsoCand++)
	{
		muonGenPt_ = MuIDIsoCand->pt();
		muonGenEta_ = MuIDIsoCand->eta();
		muonGenPhi_ = MuIDIsoCand->phi();
	}
	}



// studies for top properties

   for(reco::GenParticleCollection::const_iterator cand = genParticles->begin(); cand!=genParticles->end(); ++cand) 
   {
	// demands only w bosons from the hard interaction
    	if(abs(cand->pdgId())!=6 || cand->status()!=3) continue;
	// only not hadronically decaying w will survive
		nTop_+=1;
		TopPT_= cand->pt();
	for (unsigned int i=0; i < cand->numberOfDaughters(); i++)
	{
		// find w boson which decays to a muon
		if (abs(cand->daughter(i)->pdgId() ) == 24)
		{
			// iterate over all daughters of the w
			for (unsigned int ii=0; ii < cand->daughter(i)->numberOfDaughters(); ii++)
			{
				if (abs(cand->daughter(i)->daughter(ii)->pdgId() ) == 13)
				{
					//muon found
					MuTopPT_= cand->daughter(i)->daughter(ii)->pt();
					MuTopEta_= cand->daughter(i)->daughter(ii)->eta();
					MuTopPhi_= cand->daughter(i)->daughter(ii)->phi();
					nTopMu_+=1;
				}
				if (abs(cand->daughter(i)->daughter(ii)->pdgId() ) == 14)
				{
					// mu neutrino found
					MuNuTopPT_= cand->daughter(i)->daughter(ii)->pt();
					MuNuTopEta_= cand->daughter(i)->daughter(ii)->eta();
					MuNuTopPhi_= cand->daughter(i)->daughter(ii)->phi();
					nTopMuNu_+=1;
				}
			}
		if (nTopMu_>0)
		{
			nTopW_+=1;
			WTopPT_=cand->daughter(i)->pt();
			WTopEta_=cand->daughter(i)->eta();
			WTopPhi_=cand->daughter(i)->phi();
		}
		}
		// w checks done
	}
	//find the corresponding b quark only execute if one muon has been found
	for (unsigned int i=0; i < cand->numberOfDaughters() && nTopMu_==1; i++)
	{
		// true if particle is a b quark
		if (abs(cand->daughter(i)->pdgId() ) == 5) 
		{
			nTopB_+=1;
			if(nTopB_==1)
			{
				BTopPT_= cand->daughter(i)->pt();
				BTopEta_= cand->daughter(i)->eta();
				BTopPhi_= cand->daughter(i)->phi();
			}
		}
	}
   }


// if a top decayed to a b and a mu and munu calculate delta r and trans top mass

if (nTopMu_==1 )
{
	BMuTopDeltaR_= deltaR(BTopEta_,BTopPhi_,MuTopEta_,MuTopPhi_);

	// generator b selection with reco promt mu and met 
	double deltaPhi =reco::deltaPhi(promtRecoTLVWDecay_.Phi(), BTopPhi_);
	promtMuMetCombinedPT_=promtRecoTLVWDecay_.Pt();
	mttRecoMuMet_ = sqrt(2*BTopPT_*promtRecoTLVWDecay_.Pt()*(1-cos(deltaPhi)) );
	deltaPhi = reco::deltaPhi(WTopPhi_, BTopPhi_);
	mttGen_= sqrt(2*BTopPT_*WTopPT_*(1-cos(deltaPhi)) );
	//mttGen_= sqrt(2*lepPT*metPT*(1-cos(deltaPhi)) )

}
// x check how good the w pt eta and phi is calculated from the muon and metTag_
if(nTopMu_==1 && promtRecoTLVWDecay_.Pt() && nTopW_==1)
   {		
	wTopMassDifRecoGenPt_ = promtRecoTLVWDecay_.Pt()-WTopPT_;
	wTopMassDifRecoGenEta_ = promtRecoTLVWDecay_.Eta()-WTopEta_;
	wTopMassDifRecoGenPhi_ = promtRecoTLVWDecay_.Phi()-WTopPhi_;	
   }



// calculate mtw for all reco muons
int indexZ=1;
for( edm::View <reco::Candidate>::const_iterator MuIDCand = MuID->begin(); MuIDCand!=MuID->end();MuIDCand++)
{
	if ( std::abs(MTWCalculator(iEvent, metTag_,MuIDCand->pt(),MuIDCand->phi()) - recoMtw1_)<0.00001 ) continue;
	if(indexZ==1)
	{
		recoMtw2_= MTWCalculator(iEvent, metTag_,MuIDCand->pt(),MuIDCand->phi());
		recoMuDeltaPTClosestJet2_ = MuIDCand->pt()/DRToClosestJet(iEvent, caloJetTag_,MuIDCand->eta(),MuIDCand->phi()).second;
		recoMuDeltaRClosestJet2_ = DRToClosestJet(iEvent, caloJetTag_,MuIDCand->eta(),MuIDCand->phi()).first;
	}
	if(indexZ==2)	
	{
		recoMtw3_= MTWCalculator(iEvent, metTag_,MuIDCand->pt(),MuIDCand->phi());
		recoMuDeltaPTClosestJet3_ = MuIDCand->pt()/DRToClosestJet(iEvent, caloJetTag_,MuIDCand->eta(),MuIDCand->phi()).second;
		recoMuDeltaRClosestJet3_ = DRToClosestJet(iEvent, caloJetTag_,MuIDCand->eta(),MuIDCand->phi()).first;
	}
	if(indexZ==3)
	{
		recoMtw4_= MTWCalculator(iEvent, metTag_,MuIDCand->pt(),MuIDCand->phi());
		recoMuDeltaPTClosestJet4_ = MuIDCand->pt()/DRToClosestJet(iEvent, caloJetTag_,MuIDCand->eta(),MuIDCand->phi()).second;
		recoMuDeltaRClosestJet4_ = DRToClosestJet(iEvent, caloJetTag_,MuIDCand->eta(),MuIDCand->phi()).first;
	}
	if(indexZ==4)
	{
		recoMtw5_= MTWCalculator(iEvent, metTag_,MuIDCand->pt(),MuIDCand->phi());
		recoMuDeltaPTClosestJet5_ = MuIDCand->pt()/DRToClosestJet(iEvent, caloJetTag_,MuIDCand->eta(),MuIDCand->phi()).second;
		recoMuDeltaRClosestJet5_ = DRToClosestJet(iEvent, caloJetTag_,MuIDCand->eta(),MuIDCand->phi()).first;
	}
	indexZ++;
}


  tree_->Fill();

}



// ------------ method called once each job just before starting event loop  ------------
void 
MCEffCalculator::beginJob()
{
	double deltaRbinsIdMu []={0, 0.5,1, 3};
	double deltaRbinsIsoMu []={0, 0.3, 0.5, 0.65, 0.85, 1.0, 1.4, 1.8, 2.2, 2.6, 3};
	int DeltaRbinsIdMu = 3;
	int DeltaRbinsIsoMu = 10;
	double deltaRbinsIdElec []={0, 0.5, 0.7, 1, 3};
	double deltaRbinsIsoElec []={0, 0.3, 0.5, 0.65, 0.85, 1.0, 1.4, 1.8, 2.2, 2.6, 3};
	int DeltaRbinsIdElec = 4;
	int DeltaRbinsIsoElec = 10;
//	double deltaRbins []={0, 0.1, 0.2, 0.3, 0.4, 0.5,0.6,0.7,0.8,0.9,1,1.5,2,2.5,3};
	std::cout<<"deltaRbins created"<<std::endl;
	std::cout<<"MinMuonPTforAcc"<<minMuonPt_<<std::endl;
	std::cout<<"MinElecPTforAcc"<<minElecPt_<<std::endl;
	double ptbins []={0, 0.5, 1, 1.5, 2, 2.5, 5};
	int Ptbins = 6;

	double ptbinsMu []={10,40,200};
	int PtbinsMu = 2;
	double ptbinsElec []={10,20,100,200};
	int PtbinsElec = 3;
//	double ptbins [] = {0, 0.1, 0.2, 0.3, 0.4, 0.5,0.6,0.7,0.8,0.9,1,1.5,2,2.5,3};
	// binning for the mtw MHT distribution
	double mhtbins []={200,600,1600};
	double nJetBins []={3,4,6,7,14};
	int NJetBins = 4;
	int MhtBins = 2;

	double binNJet [] ={3,4,5,6,14};
	int BinNJets = 4;
	double binMHT  [] = {200,450,2500};
	int BinMHT = 2;
	double binHT [] = {500,1000,2500};
	int BinHT = 2;
  	edm::Service<TFileService> fs;
  	tree_ = fs->make<TTree>(treeName_,treeName_);
	std::cout<<"MCEffCalculator::creating TH3..."<<std::endl;
	totalEffTH3FPassedMu_=fs->make<TH3F>("BinByBinEffMuPassed","BinByBinEffMuPassed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);
	totalEffTH3FFailedMu_=fs->make<TH3F>("BinByBinEffMuFailed","BinByBinEffMuFailed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);
	totalEffTH3FPassedElec_=fs->make<TH3F>("BinByBinEffElecPassed","BinByBinEffElecPassed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);
	totalEffTH3FFailedElec_=fs->make<TH3F>("BinByBinEffElecFailed","BinByBinEffElecFailed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);


	accEffTH3FPassedMu_=fs->make<TH3F>("AccBinByBinEffMuPassed","AccBinByBinEffMuPassed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);
	accEffTH3FFailedMu_=fs->make<TH3F>("AccBinByBinEffMuFailed","AccBinByBinEffMuFailed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);
	accEffTH3FPassedElec_=fs->make<TH3F>("AccBinByBinEffElecPassed","AccBinByBinEffElecPassed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);
	accEffTH3FFailedElec_=fs->make<TH3F>("AccBinByBinEffElecFailed","AccBinByBinEffElecFailed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);

	recoEffTH3FPassedMu_=fs->make<TH3F>("RecoBinByBinEffMuPassed","RecoBinByBinEffMuPassed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);
	recoEffTH3FFailedMu_=fs->make<TH3F>("RecoBinByBinEffMuFailed","RecoBinByBinEffMuFailed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);
	recoEffTH3FPassedElec_=fs->make<TH3F>("RecoBinByBinEffElecPassed","RecoBinByBinEffElecPassed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);
	recoEffTH3FFailedElec_=fs->make<TH3F>("RecoBinByBinEffElecFailed","RecoBinByBinEffElecFailed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);

	isoEffTH3FPassedMu_=fs->make<TH3F>("IsoBinByBinEffMuPassed","IsoBinByBinEffMuPassed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);
	isoEffTH3FFailedMu_=fs->make<TH3F>("IsoBinByBinEffMuFailed","IsoBinByBinEffMuFailed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);
	isoEffTH3FPassedElec_=fs->make<TH3F>("IsoBinByBinEffElecPassed","IsoBinByBinEffElecPassed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);
	isoEffTH3FFailedElec_=fs->make<TH3F>("IsoBinByBinEffElecFailed","IsoBinByBinEffElecFailed",BinHT,binHT,BinMHT,binMHT,BinNJets,binNJet);
	std::cout<<"MCEffCalculator::TH3 created."<<std::endl;


	// iso electron and muon iso
	double binHTNJet35 [] = {500,800,1000,1250,2500};
	int BinHTNJet35 = 4;
	double binMHTNJet35  [] = {200,300,450,600,2500};
	int BinMHTNJet35 = 4;


	double binHTNJet67 [] = {500,800,1000,1250,1500,2500};
	int BinHTNJet67 = 5;
	double binMHTNJet67  [] = {200,300,450,2500};
	int BinMHTNJet67 = 3;


	double binHTNJet8InfMu [] = {500,800,1000,1250,1500,2500};
	int BinHTNJet8InfMu = 5;
	double binMHTNJet8InfMu  [] = {200,2500};
	int BinMHTNJet8InfMu = 1;

	double binHTNJet8InfElec [] = {500,1000,1250,1500,2500};
	int BinHTNJet8InfElec = 4;
	double binMHTNJet8InfElec  [] = {200,2500};
	int BinMHTNJet8InfElec = 1;


	std::cout<<"MCEffCalculator::TH2 creation started."<<std::endl;

 	isoEffTH2PassedMuNJet35_=fs->make<TH2F>("isoEffTH2PassedMuNJet35", "isoEffTH2PassedMuNJet35",BinHTNJet35,binHTNJet35,BinMHTNJet35,binMHTNJet35);
  	isoEffTH2PassedMuNJet35_->Sumw2();
 	isoEffTH2FailedMuNJet35_=fs->make<TH2F>("isoEffTH2FailedMuNJet35", "isoEffTH2FailedMuNJet35",BinHTNJet35,binHTNJet35,BinMHTNJet35,binMHTNJet35);
	isoEffTH2FailedMuNJet35_->Sumw2();

 	isoEffTH2PassedMuNJet67_=fs->make<TH2F>("isoEffTH2PassedMuNJet67", "isoEffTH2PassedMuNJet67_",BinHTNJet67,binHTNJet67,BinMHTNJet67,binMHTNJet67);
	isoEffTH2PassedMuNJet67_->Sumw2();
 	isoEffTH2FailedMuNJet67_=fs->make<TH2F>("isoEffTH2FailedMuNJet67", "isoEffTH2FailedMuNJet67_",BinHTNJet67,binHTNJet67,BinMHTNJet67,binMHTNJet67);
	isoEffTH2FailedMuNJet67_->Sumw2();

 	isoEffTH2PassedMuNJet8Inf_=fs->make<TH2F>("isoEffTH2PassedMuNJet8Inf", "isoEffTH2PassedMuNJet8Inf",BinHTNJet8InfMu,binHTNJet8InfMu,BinMHTNJet8InfMu,binMHTNJet8InfMu);
	isoEffTH2PassedMuNJet8Inf_->Sumw2();
 	isoEffTH2FailedMuNJet8Inf_=fs->make<TH2F>("isoEffTH2FailedMuNJet8Inf", "isoEffTH2FailedMuNJet8Inf",BinHTNJet8InfMu,binHTNJet8InfMu,BinMHTNJet8InfMu,binMHTNJet8InfMu);
	isoEffTH2FailedMuNJet8Inf_->Sumw2();


 	isoEffTH2PassedElecNJet35_=fs->make<TH2F>("isoEffTH2PassedElecNJet35", "isoEffTH2PassedElecNJet35",BinHTNJet35,binHTNJet35,BinMHTNJet35,binMHTNJet35);
	isoEffTH2PassedElecNJet35_->Sumw2();
 	isoEffTH2FailedElecNJet35_=fs->make<TH2F>("isoEffTH2FailedElecNJet35", "isoEffTH2FailedElecNJet35",BinHTNJet35,binHTNJet35,BinMHTNJet35,binMHTNJet35);
	isoEffTH2FailedElecNJet35_->Sumw2();

 	isoEffTH2PassedElecNJet67_=fs->make<TH2F>("isoEffTH2PassedElecNJet67", "isoEffTH2PassedElecNJet67",BinHTNJet67,binHTNJet67,BinMHTNJet67,binMHTNJet67);
	isoEffTH2PassedElecNJet67_->Sumw2();
 	isoEffTH2FailedElecNJet67_=fs->make<TH2F>("isoEffTH2FailedElecNJet67", "isoEffTH2FailedElecNJet67",BinHTNJet67,binHTNJet67,BinMHTNJet67,binMHTNJet67);
	isoEffTH2FailedElecNJet67_->Sumw2();

 	isoEffTH2PassedElecNJet8Inf_=fs->make<TH2F>("isoEffTH2PassedElecNJet8Inf", "isoEffTH2PassedElecNJet8Inf",BinHTNJet8InfElec,binHTNJet8InfElec,BinMHTNJet8InfElec,binMHTNJet8InfElec);
	isoEffTH2PassedElecNJet8Inf_->Sumw2();
 	isoEffTH2FailedElecNJet8Inf_=fs->make<TH2F>("isoEffTH2FailedElecNJet8Inf", "isoEffTH2FailedElecNJet8Inf",BinHTNJet8InfElec,binHTNJet8InfElec,BinMHTNJet8InfElec,binMHTNJet8InfElec);
	isoEffTH2FailedElecNJet8Inf_->Sumw2();


 	recoEffTH2PassedMuNJet35_=fs->make<TH2F>("recoEffTH2PassedMuNJet35", "recoEffTH2PassedMuNJet35",BinHTNJet35,binHTNJet35,BinMHTNJet35,binMHTNJet35);
  	recoEffTH2PassedMuNJet35_->Sumw2();
 	recoEffTH2FailedMuNJet35_=fs->make<TH2F>("recoEffTH2FailedMuNJet35", "recoEffTH2FailedMuNJet35",BinHTNJet35,binHTNJet35,BinMHTNJet35,binMHTNJet35);
	recoEffTH2FailedMuNJet35_->Sumw2();

 	recoEffTH2PassedMuNJet67_=fs->make<TH2F>("recoEffTH2PassedMuNJet67", "recoEffTH2PassedMuNJet67_",BinHTNJet67,binHTNJet67,BinMHTNJet67,binMHTNJet67);
	recoEffTH2PassedMuNJet67_->Sumw2();
 	recoEffTH2FailedMuNJet67_=fs->make<TH2F>("recoEffTH2FailedMuNJet67", "recoEffTH2FailedMuNJet67_",BinHTNJet67,binHTNJet67,BinMHTNJet67,binMHTNJet67);
	recoEffTH2FailedMuNJet67_->Sumw2();

 	recoEffTH2PassedMuNJet8Inf_=fs->make<TH2F>("recoEffTH2PassedMuNJet8Inf", "recoEffTH2PassedMuNJet8Inf",BinHTNJet8InfMu,binHTNJet8InfMu,BinMHTNJet8InfMu,binMHTNJet8InfMu);
	recoEffTH2PassedMuNJet8Inf_->Sumw2();
 	recoEffTH2FailedMuNJet8Inf_=fs->make<TH2F>("recoEffTH2FailedMuNJet8Inf", "recoEffTH2FailedMuNJet8Inf",BinHTNJet8InfMu,binHTNJet8InfMu,BinMHTNJet8InfMu,binMHTNJet8InfMu);
	recoEffTH2FailedMuNJet8Inf_->Sumw2();


 	recoEffTH2PassedElecNJet35_=fs->make<TH2F>("recoEffTH2PassedElecNJet35", "recoEffTH2PassedElecNJet35",BinHTNJet35,binHTNJet35,BinMHTNJet35,binMHTNJet35);
	recoEffTH2PassedElecNJet35_->Sumw2();
 	recoEffTH2FailedElecNJet35_=fs->make<TH2F>("recoEffTH2FailedElecNJet35", "recoEffTH2FailedElecNJet35",BinHTNJet35,binHTNJet35,BinMHTNJet35,binMHTNJet35);
	recoEffTH2FailedElecNJet35_->Sumw2();

 	recoEffTH2PassedElecNJet67_=fs->make<TH2F>("recoEffTH2PassedElecNJet67", "recoEffTH2PassedElecNJet67",BinHTNJet67,binHTNJet67,BinMHTNJet67,binMHTNJet67);
	recoEffTH2PassedElecNJet67_->Sumw2();
 	recoEffTH2FailedElecNJet67_=fs->make<TH2F>("recoEffTH2FailedElecNJet67", "recoEffTH2FailedElecNJet67",BinHTNJet67,binHTNJet67,BinMHTNJet67,binMHTNJet67);
	recoEffTH2FailedElecNJet67_->Sumw2();

 	recoEffTH2PassedElecNJet8Inf_=fs->make<TH2F>("recoEffTH2PassedElecNJet8Inf", "recoEffTH2PassedElecNJet8Inf",BinHTNJet8InfElec,binHTNJet8InfElec,BinMHTNJet8InfElec,binMHTNJet8InfElec);
	recoEffTH2PassedElecNJet8Inf_->Sumw2();
 	recoEffTH2FailedElecNJet8Inf_=fs->make<TH2F>("recoEffTH2FailedElecNJet8Inf", "recoEffTH2FailedElecNJet8Inf",BinHTNJet8InfElec,binHTNJet8InfElec,BinMHTNJet8InfElec,binMHTNJet8InfElec);
	recoEffTH2FailedElecNJet8Inf_->Sumw2();


	std::cout<<"MCEffCalculator::TH2 creation done."<<std::endl;

	// book all the result plots
	MuonAccFailedTH2F_ =fs->make<TH2F>("MuonAccFailed3", "MuonAccFailed3",MhtBins,mhtbins,NJetBins,nJetBins);
	MuonAccFailedTH2F_->Sumw2();
	MuonAccPassedTH2F_ =fs->make<TH2F>("MuonAccPassed3", "MuonAccPassed3",MhtBins,mhtbins,NJetBins,nJetBins);
	MuonAccPassedTH2F_->Sumw2();
	ElecAccFailedTH2F_ =fs->make<TH2F>("ElecAccFailed3", "ElecAccFailed3",MhtBins,mhtbins,NJetBins,nJetBins);
	ElecAccFailedTH2F_->Sumw2();
	ElecAccPassedTH2F_ =fs->make<TH2F>("ElecAccPassed3", "ElecAccPassed3",MhtBins,mhtbins,NJetBins,nJetBins);
	ElecAccPassedTH2F_->Sumw2();

	muonIDFailedTH2F_ = fs->make<TH2F>("MuonRecoFailed", "MuonRecoFailed",DeltaRbinsIdMu,deltaRbinsIdMu,Ptbins,ptbins);
	muonIDFailedTH2F_->Sumw2();
	muonIDPassedTH2F_ = fs->make<TH2F>("MuonRecoPassed", "MuonRecoPassed",DeltaRbinsIdMu,deltaRbinsIdMu,Ptbins,ptbins);
	muonIDPassedTH2F_->Sumw2();
	muonIsoFailedTH2F_ = fs->make<TH2F>("muonIsoFailed","muonIsoFailed",DeltaRbinsIsoMu,deltaRbinsIsoMu,Ptbins,ptbins);
	muonIsoFailedTH2F_->Sumw2();
	muonIsoPassedTH2F_ = fs->make<TH2F>("muonIsoPassed","muonIsoPassed",DeltaRbinsIsoMu,deltaRbinsIsoMu,Ptbins,ptbins);
	muonIsoPassedTH2F_->Sumw2();

	muonIDFailed2TH2F_ = fs->make<TH2F>("MuonRecoFailed2", "MuonRecoFailed2",DeltaRbinsIdMu,deltaRbinsIdMu,PtbinsMu,ptbinsMu);
	muonIDFailed2TH2F_->Sumw2();
	muonIDPassed2TH2F_ = fs->make<TH2F>("MuonRecoPassed2", "MuonRecoPassed2",DeltaRbinsIdMu,deltaRbinsIdMu,PtbinsMu,ptbinsMu);
	muonIDPassed2TH2F_->Sumw2();
	muonIsoFailed2TH2F_ = fs->make<TH2F>("muonIsoFailed2","muonIsoFailed2",DeltaRbinsIsoMu,deltaRbinsIsoMu,PtbinsMu,ptbinsMu);
	muonIsoFailed2TH2F_->Sumw2();
	muonIsoPassed2TH2F_ = fs->make<TH2F>("muonIsoPassed2","muonIsoPassed2",DeltaRbinsIsoMu,deltaRbinsIsoMu,PtbinsMu,ptbinsMu);
	muonIsoPassed2TH2F_->Sumw2();

	MuonAccPassedTH1F_ = fs->make<TH1F>("muonAccPassed","muonAccPassed",40,0,200);
	MuonAccPassedTH1F_->Sumw2();
	MuonAccPassed2TH1F_ = fs->make<TH1F>("muonAccPassed2","muonAccPassed2",MhtBins,mhtbins);
	MuonAccPassed2TH1F_->Sumw2();
	MuonAccFailedTH1F_ = fs->make<TH1F>("muonAccFailed","muonAccFailed",40,0,200);
	MuonAccFailedTH1F_->Sumw2();
	MuonAccFailed2TH1F_ = fs->make<TH1F>("muonAccFailed2","muonAccFailed2",MhtBins,mhtbins);
	MuonAccFailed2TH1F_->Sumw2();
	MuonMTPassedTH2F_ = fs->make<TH2F>("muonMT","muonMT",1,0,1,1,0,1);
	MuonMTPassedTH2F_->Sumw2();


	elecIDFailedTH2F_ = fs->make<TH2F>("elecIdFailed","elecIdFailed",DeltaRbinsIdElec,deltaRbinsIdElec,Ptbins,ptbins);
	elecIDFailedTH2F_->Sumw2();
	elecIDPassedTH2F_ = fs->make<TH2F>("elecIdPassed","elecIdPassed",DeltaRbinsIdElec,deltaRbinsIdElec,Ptbins,ptbins);
	elecIDPassedTH2F_->Sumw2();
	elecIsoFailedTH2F_ = fs->make<TH2F>("elecIsoFailed","elecIsoFailed",DeltaRbinsIsoElec,deltaRbinsIsoElec,Ptbins,ptbins);
	elecIsoFailedTH2F_->Sumw2();
	elecIsoPassedTH2F_ = fs->make<TH2F>("elecIsoPassed","elecIsoPassed",DeltaRbinsIsoElec,deltaRbinsIsoElec,Ptbins,ptbins);
	elecIsoPassedTH2F_->Sumw2();

	elecIDFailed2TH2F_ = fs->make<TH2F>("elecIdFailed2","elecIdFailed2",DeltaRbinsIdElec,deltaRbinsIdElec,PtbinsElec,ptbinsElec);
	elecIDFailed2TH2F_->Sumw2();
	elecIDPassed2TH2F_ = fs->make<TH2F>("elecIdPassed2","elecIdPassed2",DeltaRbinsIdElec,deltaRbinsIdElec,PtbinsElec,ptbinsElec);
	elecIDPassed2TH2F_->Sumw2();
	elecIsoFailed2TH2F_ = fs->make<TH2F>("elecIsoFailed2","elecIsoFailed2",DeltaRbinsIsoElec,deltaRbinsIsoElec,PtbinsElec,ptbinsElec);
	elecIsoFailed2TH2F_->Sumw2();
	elecIsoPassed2TH2F_ = fs->make<TH2F>("elecIsoPassed2","elecIsoPassed2",DeltaRbinsIsoElec,deltaRbinsIsoElec,PtbinsElec,ptbinsElec);
	elecIsoPassed2TH2F_->Sumw2();

	ElecAccPassedTH1F_ = fs->make<TH1F>("elecAccPassed","elecAccPassed",40,0,200);
	ElecAccPassedTH1F_->Sumw2();
	ElecAccPassed2TH1F_ = fs->make<TH1F>("elecAccPassed2","elecAccPassed2",MhtBins,mhtbins);
	ElecAccPassed2TH1F_->Sumw2();
	ElecAccFailedTH1F_ = fs->make<TH1F>("elecAccFailed","elecAccFailed",40,0,200);
	ElecAccFailedTH1F_->Sumw2();
	ElecAccFailed2TH1F_ = fs->make<TH1F>("elecAccFailed2","elecAccFailed2",MhtBins,mhtbins);
	ElecAccFailed2TH1F_->Sumw2();

	// MTW 
	MTWTH2F_ =fs->make<TH2F>("MTW","MTW",400,0,400,MhtBins,mhtbins);
	MTWTH2F_->Sumw2(); 
	
	// special HT1
	lepDeltaR_ = fs->make<TH1F>("lepDeltaR","lepDeltaR",40,0,4);


//create branches in the tree
    
    tree_->Branch("nLeptons",&leptons_,"nLeptons/I");
    tree_->Branch("nGenMu",&nGenMu_,"nGenMu/I");
    tree_->Branch("nGenElec",&nGenElec_,"nGenElec/I");
    tree_->Branch("genPt",&genPt_,"genPt/F");
    tree_->Branch("genPhi",&genPhi_,"genPhi/F");
    tree_->Branch("genEta",&genEta_,"genEta/F");
    tree_->Branch("genDeltaR",&genDeltaR_,"genDeltaR/F");
    tree_->Branch("genPTJet",&genPTJet_,"genPTJet/F");
    tree_->Branch("genPTRelJet",&genPTRelJet_,"genPTRelJet/F");

    tree_->Branch("MuonPTAccPassed",&muonPTAccPassed_,"MuonPTAccPassed/F");
    tree_->Branch("MuonIDPassed",&muonIDPassed_,"MuonIDPassed/F");
    tree_->Branch("MuonIDFailed",&muonIDFailed_,"MuonIDFailed/F");
    tree_->Branch("MuonGenPt",&muonGenPt_,"MuonGenPt/F");
    tree_->Branch("MuonGenPhi",&muonGenEta_,"MuonGenPhi/F");
    tree_->Branch("MuonGenEta",&muonGenPhi_,"MuonGenEta/F");
    tree_->Branch("MuonGenNumber",&nGenMu_,"MuonGenNumber/s");
    tree_->Branch("MuonNumberRecoID",&muonNumberRecoID_,"MuonNumberRecoID/I");


    tree_->Branch("ElecPTAccPassed",&elecPTAccPassed_,"ElecPTAccPassed/F");
    tree_->Branch("ElecGenPt",&elecGenPt_,"ElecGenPt/F");
    tree_->Branch("ElecGenPhi",&elecGenEta_,"ElecGenPhi/F");
    tree_->Branch("ElecGenEta",&elecGenPhi_,"ElecGenEta/F");
    tree_->Branch("ElecGenNumber",&nGenElec_,"ElecGenNumber/s");
    tree_->Branch("ElecNumberRecoID",&elecNumberRecoID_,"ElecNumberRecoID/I");


    tree_->Branch("GenDeltaRLepClosestJet",&deltaGenR_,"GenDeltaRLepClosestJet/F");
    tree_->Branch("ClosestJetElecGenPt",&closestJetToElecGenPt_,"ClosestJetElecGenPt/F");
    tree_->Branch("ClosestJetMuonGenPt",&closestJetToMuonGenPt_,"ClosestJetMuonGenPt/F");
    tree_->Branch("MTW",&mtw_,"MTW/F");

    // discriminator for mc expectaiton selector +1 passed -1 failed
    tree_->Branch("nAccMu",&nAccMu_,"nAccMu/I");
    tree_->Branch("nRecoMu",&nRecoMu_,"nRecoMu/I");
    tree_->Branch("nIsoMu",&nIsoMu_,"nIsoMu/I");

    tree_->Branch("nAccElec",&nAccElec_,"nAccElec/I");
    tree_->Branch("nRecoElec",&nRecoElec_,"nRecoElec/I");
    tree_->Branch("nIsoElec",&nIsoElec_,"nIsoElec/I");

    //Testing variables
    tree_->Branch("RecoLevelMuonDeltaR",&RecoMuonDeltaR_,"RecoLevelMuonDeltaR/F");
    tree_->Branch("RecoLevelMuonPTJet",&RecoMuonPTJet_,"RecoLevelMuonPTJet/F");
    tree_->Branch("RecoLevelMuonRelPTJet",&RecoMuonPTRelJet_,"RecoLevelMuonRelPTJet/F");
    tree_->Branch("RecoVsGenLevelMuonDeltaR",&RecoGenMuDeltaR_,"RecoVsGenLevelMuonDeltaR/F");
    tree_->Branch("muonRecoPt",&muonRecoPt_,"muonRecoPt/F");
    tree_->Branch("muonRecoEta",&muonRecoEta_,"muonRecoEta/F");
    tree_->Branch("muonRecoPhi",&muonRecoPhi_,"muonRecoPhi/F");



    tree_->Branch("IsoLevelMuonDeltaR",&IsoMuonDeltaR_,"IsoLevelMuonDeltaR/F");
    tree_->Branch("IsoLevelMuonPTJet",&IsoMuonPTJet_,"IsoLevelMuonPTJet/F");
    tree_->Branch("IsoLevelMuonRelPTJet",&IsoMuonPTRelJet_,"IsoLevelMuonRelPTJet/F");
    tree_->Branch("muonIsoPt",&muonIsoPt_,"muonIsoPt/F");
    tree_->Branch("muonIsoEta",&muonIsoEta_,"muonIsoEta/F");
    tree_->Branch("muonIsoPhi",&muonIsoPhi_,"muonIsoPhi/F");

    tree_->Branch("RecoLevelElecDeltaR",&RecoElecDeltaR_,"RecoLevelElecDeltaR/F");
    tree_->Branch("RecoLevelElecPTJet",&RecoElecPTJet_,"RecoLevelElecPTJet/F");
    tree_->Branch("RecoLevelElecRelPTJet",&RecoElecPTRelJet_,"RecoLevelElecRelPTJet/F");
    tree_->Branch("RecoVsGenLevelElecDeltaR",&RecoGenMuDeltaR_,"RecoVsGenLevelElecDeltaR/F");
    tree_->Branch("ElecRecoPt",&elecRecoPt_,"ElecRecoPt/F");
    tree_->Branch("ElecRecoEta",&elecRecoEta_,"ElecRecoEta/F");
    tree_->Branch("ElecRecoPhi",&elecRecoPhi_,"ElecRecoPhi/F");



    tree_->Branch("IsoLevelElecDeltaR",&IsoElecDeltaR_,"IsoLevelElecDeltaR/F");
    tree_->Branch("IsoLevelElecPTJet",&IsoElecPTJet_,"IsoLevelElecPTJet/F");
    tree_->Branch("IsoLevelElecRelPTJet",&IsoElecPTRelJet_,"IsoLevelElecRelPTJet/F");
    tree_->Branch("ElecIsoPt",&elecIsoPt_,"ElecIsoPt/F");
    tree_->Branch("ElecIsoEta",&elecIsoEta_,"ElecIsoEta/F");
    tree_->Branch("ElecIsoPhi",&elecIsoPhi_,"ElecIsoPhi/F");




    // test if the cs is the same as the selected one from the matching
    tree_->Branch("nMuInIsoMuCollection",&nMuInIsoMuCollection_,"nMuInIsoMuCollection/I");
    tree_->Branch("muPromtMatched",&muPromtMatched_,"muPromtMatched/I");


    // plotting values
    tree_->Branch("HT",&ht_,"HT/F");
    tree_->Branch("MHT",&mht_,"MHT/F");
    tree_->Branch("NJets",&nJets_,"NJets/s");
    tree_->Branch("Weight",&eventWeight_,"Weight/F");
    tree_->Branch("MT",&mt_,"MT/F");
    tree_->Branch("NVtx",&nV_,"NVtx/s");
    tree_->Branch("IsExpectation",&isExpectation_,"IsExpectation/I");

	// testing
	tree_->Branch("ntau",&ntau_,"ntau/I");
	tree_->Branch("ntauMu",&ntauMu_,"ntauMu/I");
	tree_->Branch("ntauElec",&ntauElec_,"ntauElec/I");

	// gen values for top b tagging
	tree_->Branch("nTop",&nTop_,"nTop/I");
	tree_->Branch("nTopB",&nTopB_,"nTopB/I");
	tree_->Branch("nTopW",&nTopW_,"nTopW/I");
	tree_->Branch("nTopMuNu",&nTopMuNu_,"nTopMuNu/I");
	tree_->Branch("nTopMu",&nTopMu_,"nTopMu/I");

	tree_->Branch("TopPT",&TopPT_,"TopPT/F");
	tree_->Branch("BTopPT",&BTopPT_,"BTopPT/F");
	tree_->Branch("WTopPT",&WTopPT_,"WTopPT/F");
	tree_->Branch("WTopEta",&WTopEta_,"WTopEta/F");
	tree_->Branch("WTopPhi",&WTopPhi_,"WTopPhi/F");
	tree_->Branch("MuNuTopPT",&MuNuTopPT_,"MuNuTopPT/F");
	tree_->Branch("MuTopPT",&MuTopPT_,"MuTopPT/F");
	tree_->Branch("ElecTopPT",&ElecTopPT_,"ElecTopPT/F");

	tree_->Branch("BMuTopDeltaR",&BMuTopDeltaR_,"BMuTopDeltaR/F");
	tree_->Branch("promtMuMetCombinedPT",&promtMuMetCombinedPT_,"promtMuMetCombinedPT/F");
	tree_->Branch("wTopMassDifRecoGenPt",&wTopMassDifRecoGenPt_,"wTopMassDifRecoGenPt/F");
	tree_->Branch("promtMuMetCombinedEta",&promtMuMetCombinedEta_,"promtMuMetCombinedEta/F");
	tree_->Branch("wTopMassDifRecoGenEta",&wTopMassDifRecoGenEta_,"wTopMassDifRecoGenEta/F");
	tree_->Branch("promtMuMetCombinedPhi",&promtMuMetCombinedPhi_,"promtMuMetCombinedPhi/F");
	tree_->Branch("wTopMassDifRecoGenPhi",&wTopMassDifRecoGenPhi_,"wTopMassDifRecoGenPhi/F");
	tree_->Branch("mttRecoMuMet",&mttRecoMuMet_,"mttRecoMuMet/F");
	tree_->Branch("mttGen",&mttGen_,"mttGen/F");
	tree_->Branch("mtt",&mtt_,"mtt/F");

	// mtw for all reco muons
	tree_->Branch("recoMtw1",&recoMtw1_,"recoMtw1/F");
	tree_->Branch("recoMuDeltaPTClosestJet1",&recoMuDeltaPTClosestJet1_,"recoMuDeltaPTClosestJet1/F");
	tree_->Branch("recoMuDeltaRClosestJet1",&recoMuDeltaRClosestJet1_,"recoMuDeltaRClosestJet1/F");
	tree_->Branch("recoMtw2",&recoMtw2_,"recoMtw2/F");
	tree_->Branch("recoMuDeltaPTClosestJet2",&recoMuDeltaPTClosestJet2_,"recoMuDeltaPTClosestJet2/F");
	tree_->Branch("recoMuDeltaRClosestJet2",&recoMuDeltaRClosestJet2_,"recoMuDeltaRClosestJet2/F");
	tree_->Branch("recoMtw3",&recoMtw3_,"recoMtw3/F");
	tree_->Branch("recoMuDeltaPTClosestJet3",&recoMuDeltaPTClosestJet3_,"recoMuDeltaPTClosestJet3/F");
	tree_->Branch("recoMuDeltaRClosestJet3",&recoMuDeltaRClosestJet3_,"recoMuDeltaRClosestJet3/F");
	tree_->Branch("recoMtw4",&recoMtw4_,"recoMtw4/F");
	tree_->Branch("recoMuDeltaPTClosestJet4",&recoMuDeltaPTClosestJet4_,"recoMuDeltaPTClosestJet4/F");
	tree_->Branch("recoMuDeltaRClosestJet4",&recoMuDeltaRClosestJet4_,"recoMuDeltaRClosestJet4/F");
	tree_->Branch("recoMtw5",&recoMtw5_,"recoMtw5/F");
	tree_->Branch("recoMuDeltaPTClosestJet5",&recoMuDeltaPTClosestJet5_,"recoMuDeltaPTClosestJet5/F");
	tree_->Branch("recoMuDeltaRClosestJet5",&recoMuDeltaRClosestJet5_,"recoMuDeltaRClosestJet5/F");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
MCEffCalculator::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
MCEffCalculator::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
MCEffCalculator::endRun(edm::Run const&, edm::EventSetup const&)
{

}

// ------------ method called when starting to processes a luminosity block  ------------
void 
MCEffCalculator::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
MCEffCalculator::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MCEffCalculator::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void
MCEffCalculator::ResetValues()
{


	muonPTAccPassed_	=-100;
	elecPTAccPassed_	=-100;
	muonNumberRecoID_	=-100;
	elecNumberRecoID_	=-100;
	muonIDPassed_		=-100;
	muonIDFailed_		=-100;
	elecIDPassed_		=-100;
	elecIDFailed_		=-100;
	muonRecoPt_		=-100;
	muonRecoEta_		=-100;
	muonRecoPhi_		=-100;
	elecRecoPt_		=-100;
	elecRecoEta_		=-100;
	elecRecoPhi_		=-100;
	muonGenPt_		=-100;
	muonGenEta_		=-100;
	muonGenPhi_		=-100;
	elecGenPt_		=-100;
	elecGenEta_		=-100;
	elecGenPhi_		=-100;
	deltaGenR_		=-100;
	closestJetToMuonGenPt_	=-100;
	closestJetToElecGenPt_	=-100;
	mtw_			=-100;
	mt_			=-100;
	mtwElec_		=-100;
	RecoMuonDeltaR_		=-100;
	RecoMuonPTJet_		=-100;
	RecoMuonPTRelJet_	=-100;
	IsoMuonDeltaR_		=-100;
	IsoMuonPTJet_		=-100;
	IsoMuonPTRelJet_	=-100;
	RecoGenMuDeltaR_	=-100;
	muonIsoPt_		=-100;
	muonIsoEta_		=-100;
	muonIsoPhi_		=-100;
	RecoElecDeltaR_		=-100;
	RecoElecPTJet_		=-100;
	RecoElecPTRelJet_	=-100;
	IsoElecDeltaR_		=-100;
	IsoElecPTJet_		=-100;
	IsoElecPTRelJet_	=-100;
	RecoGenElecDeltaR_	=-100;
	elecIsoPt_		=-100;
	elecIsoEta_		=-100;
	elecIsoPhi_		=-100;
	genPt_			=-100;
	genEta_			=-100;
	genPhi_			=-100;
	
	nGenMu_ =0;
	nGenElec_ =0;
	eventWeight_=0;
	nAccMu_=0;
	nAccElec_=0;
	nRecoMu_=0;
	nRecoElec_=0;
	nIsoMu_=0;
	isExpectation_ = 0;
	nIsoElec_=0;

	// TESTING VARIABLES
	nMuInIsoMuCollection_=0;
 	// additonal variables for plotting
	ht_=-100;
	nJets_=0;
	mht_=-100;
	nV_ =0;

	// test
	ntau_=0;
	ntauMu_=0;
	ntauElec_=0;

	// gen values for top b tagging
	nTop_=0;
	nTopW_=0;
	nTopB_=0;
	nTopMuNu_=0;
	nTopMu_=0;
	nTopElec_=0;

	TopPT_=-100;
	BTopPT_=-100;
	WTopPT_=-100;
	MuNuTopPT_=-100;
	MuTopPT_=-100;

	TopEta_=-100;
	BTopEta_=-100;
	WTopEta_=-100;
	MuNuTopEta_=-100;
	MuTopEta_=-100;

	TopPhi_=-100;
	BTopPhi_=-100;
	WTopPhi_=-100;
	MuNuTopPhi_=-100;
	MuTopPhi_=-100;



	ElecTopPT_=-100;

	BMuTopDeltaR_=-100;
	mtt_=-100;
	mttGen_=-100;
	mttRecoMuMet_=-100;
	promtMuMetCombinedPT_=-100;
	promtMuMetCombinedEta_=-100;
	promtMuMetCombinedPhi_=-100;
	wTopMassDifRecoGenPt_=-10000;
	wTopMassDifRecoGenPt_=-10000;
	wTopMassDifRecoGenPt_=-10000;

nTopB_=0;
nTopMu_=0;
nTopMuNu_=0;
nTopW_=0;
TopPT_=-100;
nTop_=0;
recoMtw1_=-100;
recoMtw2_=-100;
recoMtw3_=-100;
recoMtw4_=-100;
recoMtw5_=-100;
recoMuDeltaPTClosestJet1_=-1;
recoMuDeltaPTClosestJet2_=-1;
recoMuDeltaPTClosestJet3_=-1;
recoMuDeltaPTClosestJet4_=-1;
recoMuDeltaPTClosestJet5_=-1;

}


void
MCEffCalculator::MuonFound(reco::GenParticleCollection::const_iterator cand, unsigned int i)
{
	nGenMu_+=1;
	muonGenPt_  = cand->daughter(i)->pt();
	muonGenEta_ = cand->daughter(i)->eta();
	muonGenPhi_ = cand->daughter(i)->phi();
	// true if the muon is out of the detector acceptance
			genPt_ = muonGenPt_;
			genEta_= muonGenEta_;
			genPhi_=muonGenPhi_;

}

void
MCEffCalculator::ElecFound(reco::GenParticleCollection::const_iterator cand, unsigned int i)
{
	nGenElec_+=1;
		elecGenPt_  = cand->daughter(i)->pt();
		elecGenEta_ = cand->daughter(i)->eta();
		elecGenPhi_ = cand->daughter(i)->phi();
			genPt_ = elecGenPt_;
			genEta_= elecGenEta_;
			genPhi_= elecGenPhi_;

		// true if the elec is out of the detector acceptance


}

void
MCEffCalculator::TauFound(const reco::Candidate* tau)
{

	ntau_+=1;
	for (unsigned int ii=0; ii < tau->numberOfDaughters(); ii++)
	{
		// true if the selected tau is not the finally decaying tau but has an intermediate tau or w in the decay
		if (abs(tau->daughter(ii)->pdgId()) == 15 || abs(tau->daughter(ii)->pdgId()) == 24) TauFound(tau->daughter(ii));
		if (abs(tau->daughter(ii)->pdgId() ) == 13 )
		{
			//muon found
			ntauMu_+=1;
			nGenMu_+=1;
			muonGenPt_  = tau->daughter(ii)->pt();
			muonGenEta_ = tau->daughter(ii)->eta();
			muonGenPhi_ = tau->daughter(ii)->phi();
			genPt_ = muonGenPt_;
			genEta_= muonGenEta_;
			genPhi_=muonGenPhi_;
		}
		if (abs(tau->daughter(ii)->pdgId() ) == 11 )
		{
			//elec found
			ntauElec_+=1;
			nGenElec_+=1;
			elecGenPt_  = tau->daughter(ii)->pt();
			elecGenEta_ = tau->daughter(ii)->eta();
			elecGenPhi_ = tau->daughter(ii)->phi();
			genPt_ = elecGenPt_;
			genEta_= elecGenEta_;
			genPhi_= elecGenPhi_;

		}


	}
/*	for (unsigned int itau=0; itau < cand->daughter(i)->numberOfDaughters(); itau++)
	{	
	std::cout<<"MCEffCalculator::TauFound!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1"<<std::endl;
	std::cout<<"MCEffCalculator::Tau"<<cand->daughter(i)->pdgId()<<std::endl;
	std::cout<<"MCEffCalculator::Tau"<<cand->daughter(i)->daughter(itau)->daughter(0)->pdgId()<<std::endl;
	std::cout<<"MCEffCalculator::Tau"<<cand->daughter(i)->daughter(itau)->daughter(1)->pdgId()<<std::endl;
		if (abs(cand->daughter(i)->daughter(itau)->pdgId() ) ==13) 
		{
		std::cout<<"MCEffCalculator::MuonFoundfrom Tau!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
			ntauMu_+=1;
			nGenMu_+=1;
			muonGenPt_  = cand->daughter(i)->daughter(itau)->pt();
			muonGenEta_ = cand->daughter(i)->daughter(itau)->eta();
			muonGenPhi_ = cand->daughter(i)->daughter(itau)->phi();
		// true if the muon is out of the detector acceptance

		}
		if (abs(cand->daughter(i)->daughter(itau)->pdgId() ) ==11)  
		{
			nGenElec_+=1;
			elecGenPt_  = cand->daughter(i)->daughter(itau)->pt();
			elecGenEta_ = cand->daughter(i)->daughter(itau)->eta();
			elecGenPhi_ = cand->daughter(i)->daughter(itau)->phi();
			// true if the elec is out of the detector acceptance

		}
	}
*/
}

std::pair <double,double> MCEffCalculator::DRToClosestJet(const edm::Event& iEvent, edm::InputTag caloJetTag, double lepEta, double lepPhi)
{
	edm::Handle <edm::View<reco::CaloJet> >jets;
	iEvent.getByLabel(caloJetTag,jets);
	double resultDR=9999;
	double resultJetPt=9999;
	unsigned int nJets = jets->size();
	// loop over all the jets in the event
	for (unsigned int i=0; i<nJets;i++)
	{
		double tempDeltaR = deltaR(jets->at(i).eta(),jets->at(i).phi(),lepEta,lepPhi);
		if(tempDeltaR<resultDR && jets->at(i).emEnergyFraction() <0.9 && jets->at(i).emEnergyFraction() > 0.05 && jets->at(i).pt()>29.999 ) // removed here && tempDeltaR>0.1 
		{
			resultDR = tempDeltaR;
			resultJetPt = jets->at(i).pt();
		}
	}


	return std::make_pair(resultDR,resultJetPt);
}



double
MCEffCalculator::MTWCalculator(const edm::Event& iEvent, edm::InputTag metTag, double lepPT, double lepPhi)
{
  edm::Handle< edm::View<reco::Candidate> > met;
  iEvent.getByLabel(metTag,met);
  double metPT = met->at(0).pt();
//  std::cout<<"MTWCalculator:metPT"<<metPT<<std::endl;
//  std::cout<<"MTWCalculator:met->at(0).phi()"<<met->at(0).phi()<<std::endl;
 // dR = deltaR(met->at(0).eta(),met->at(0).phi(),lepEta,lepPhi);
    double deltaPhi =reco::deltaPhi(lepPhi, met->at(0).phi());
//  std::cout<<"MTWCalculator:deltaPhi"<<deltaPhi<<std::endl;
//    std::cout<<"MTWCalculator:MTW"<<sqrt(2*lepPT*metPT*(1-cos(deltaPhi)) )<<std::endl;
    return sqrt(2*lepPT*metPT*(1-cos(deltaPhi)) );

}

edm::View <reco::Candidate>::const_iterator
MCEffCalculator::LeptonMatch(const edm::View <reco::Candidate>  &MuID, Float_t lepGenEta, Float_t lepGenPhi, Float_t lepGenPt)
{
	edm::View <reco::Candidate>::const_iterator result=MuID.end();
	double minRecoDeltaR = 1000;
	double RecoPhi=0;
	double RecoEta=0;
       	double RecoPt=0;
	for( edm::View <reco::Candidate>::const_iterator MuIDCand = MuID.begin(); MuIDCand!=MuID.end();MuIDCand++)
	{

		RecoPhi=MuIDCand->phi();
		RecoEta=MuIDCand->eta();
		RecoPt=MuIDCand->pt();

		if (deltaR(lepGenEta,lepGenPhi,RecoEta,RecoPhi) < 0.2 && abs(lepGenPt-RecoPt)/RecoPt < 0.2 && deltaR(lepGenEta,lepGenPhi,RecoEta,RecoPhi) < minRecoDeltaR  )
		{
			if (minRecoDeltaR < 0.21)
			{	
				// true if the new found muon is closer than 0.1 but the old one isnt
				if ( deltaR(lepGenEta,lepGenPhi,RecoEta,RecoPhi) <0.1 && minRecoDeltaR >0.1) result = MuIDCand;
				if (deltaR(lepGenEta,lepGenPhi,RecoEta,RecoPhi) <0.1 && minRecoDeltaR <0.1)result = MuID.end();
			}
			
			result = MuIDCand;
			minRecoDeltaR= deltaR(lepGenEta,lepGenPhi,RecoEta,RecoPhi);

		}
		
	}
	return result;

}

//define this as a plug-in
DEFINE_FWK_MODULE(MCEffCalculator);
