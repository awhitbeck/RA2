// -*- C++ -*-
//
// Package:    Trigger
// Class:      Trigger
// 
/**\class Trigger Trigger.cc PhysicsTools/Trigger/src/Trigger.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Kristin Heine,,,DESY
//         Created:  Wed Apr 18 15:00:58 CEST 2012
// $Id: Trigger.cc,v 1.5 2013/01/18 09:40:27 kheine Exp $
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

//
// class declaration
//

using namespace edm;
using namespace std;
using namespace reco;
using namespace trigger;

class Trigger : public edm::EDAnalyzer {
public:
   explicit Trigger(const edm::ParameterSet&);
   ~Trigger();
   bool goodElectron(const pat::Electron* ele);
   bool goodMuon(const pat::Muon* mu);
   bool goodJet_MHT(const pat::Jet* jet);
   bool goodJet_HT(const pat::Jet* jet);

private:
   virtual void beginJob() ;
   virtual void analyze(const edm::Event&, const edm::EventSetup&);
   virtual void endJob() ;

   /// The instance of the HLTConfigProvider as a data member
   HLTConfigProvider hltConfig_;

   // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

edm::InputTag jets_;
edm::InputTag met_;
edm::InputTag electrons_;
edm::InputTag muons_;
edm::InputTag photons_;
edm::InputTag vertex_;
edm::InputTag triggerTag_;

double elec_pt_cut_;
double elec_eta_cut_;

double muon_pt_cut_;
double muon_eta_cut_;

double jet_pt_cut_MHT_;
double jet_eta_cut_MHT_;
double jet_pt_cut_HT_;
double jet_eta_cut_HT_;
bool jet_useJetID_;

math::XYZPoint bs;

TH1F* h_met;

TH1F* h_HLT_Ele27_WP80_prescale;
TH1F* h_HLT_PFHT650_prescale;

TH1F* h_HTallprobes_PFHT650;
TH1F* h_HTpassedprobes_PFHT650;
TH1F* h_HTallprobes_total_PFHT650;
TH1F* h_HTpassedprobes_total_PFHT650;
TH1F* h_HTallprobes_intEff_PFHT650;
TH1F* h_HTpassedprobes_intEff_PFHT650;

TH1F* h_HTallprobes_PFNoPUHT650;
TH1F* h_HTpassedprobes_PFNoPUHT650;
TH1F* h_HTallprobes_total_PFNoPUHT650;
TH1F* h_HTpassedprobes_total_PFNoPUHT650;
TH1F* h_HTallprobes_intEff_PFNoPUHT650;
TH1F* h_HTpassedprobes_intEff_PFNoPUHT650;

TH1F* h_HTallprobes_PFHT;
TH1F* h_HTpassedprobes_PFHT;
TH1F* h_HTallprobes_total_PFHT;
TH1F* h_HTpassedprobes_total_PFHT;
TH1F* h_HTallprobes_intEff_PFHT;
TH1F* h_HTpassedprobes_intEff_PFHT;
TH1F* h_MHTallprobes_PFHT;
TH1F* h_MHTpassedprobes_PFHT;
TH1F* h_MHTallprobes_total_PFHT;
TH1F* h_MHTpassedprobes_total_PFHT;
TH1F* h_MHTallprobes_intEff_PFHT;
TH1F* h_MHTpassedprobes_intEff_PFHT;

TH1F* h_HTallprobes_PFNoPUHT;
TH1F* h_HTpassedprobes_PFNoPUHT;
TH1F* h_HTallprobes_total_PFNoPUHT;
TH1F* h_HTpassedprobes_total_PFNoPUHT;
TH1F* h_HTallprobes_intEff_PFNoPUHT;
TH1F* h_HTpassedprobes_intEff_PFNoPUHT;
TH1F* h_MHTallprobes_PFNoPUHT;
TH1F* h_MHTpassedprobes_PFNoPUHT;
TH1F* h_MHTallprobes_total_PFNoPUHT;
TH1F* h_MHTpassedprobes_total_PFNoPUHT;
TH1F* h_MHTallprobes_intEff_PFNoPUHT;
TH1F* h_MHTpassedprobes_intEff_PFNoPUHT;

TH2F* h_allprobes_PFHT;
TH2F* h_allprobes_PFNoPUHT;
TH2F* h_passedprobes_PFHT;
TH2F* h_passedprobes_PFNoPUHT;

TH2F* h_Corr_JetPt_MuonPT;
TH2F* h_Corr_JetPt_ElectronPT;
TH2F* h_Corr_OnlineHT_OfflineHT;
TH2F* h_Corr_OnlineMHT_OfflineMHT;
TH2F* h_Corr_OnlineMHT_OfflineMET;

//
// constructors and destructor
//
Trigger::Trigger(const edm::ParameterSet& pset)
{
   //now do what ever initialization is needed
   jets_ = pset.getParameter < edm::InputTag > ("jetCollection");
   met_ = pset.getParameter < edm::InputTag > ("metCollection");
   vertex_ = pset.getParameter < edm::InputTag > ("vertexCollection");
   triggerTag_ = pset.getParameter < edm::InputTag > ("TriggerTag");

   muon_pt_cut_ = pset.getParameter<double>("muon_pt_cut");
   muon_eta_cut_ = pset.getParameter<double>("muon_eta_cut");

   elec_pt_cut_ = pset.getParameter<double>("elec_pt_cut");
   elec_eta_cut_ = pset.getParameter<double>("elec_eta_cut");

   jet_pt_cut_MHT_ = pset.getParameter<double>("jet_pt_cut_MHT");
   jet_eta_cut_MHT_ = pset.getParameter<double>("jet_eta_cut_MHT");
   jet_pt_cut_HT_ = pset.getParameter<double>("jet_pt_cut_HT");
   jet_eta_cut_HT_ = pset.getParameter<double>("jet_eta_cut_HT");
   jet_useJetID_ = pset.getParameter<bool>("jet_useJetID");

}


Trigger::~Trigger()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
Trigger::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   //// Load all the PAT collections

   //Jets
   Handle < edm::View<pat::Jet> > Jets;
   bool isFound = iEvent.getByLabel(jets_, Jets);
   if (!isFound)
      return;
   edm::View < pat::Jet > Jets_rec = *Jets;

   //MET
   Handle < edm::View<pat::MET> > Met;
   isFound = iEvent.getByLabel(met_, Met);
   if (!isFound)
      return;
   edm::View < pat::MET > Met_rec = *Met;

   //Vertex
   edm::Handle < reco::VertexCollection > vertices;
   isFound = iEvent.getByLabel(vertex_, vertices);
   if (!isFound)
      return;

   for (reco::VertexCollection::const_iterator vertex = vertices->begin(); vertex != vertices->end(); ++vertex) {
      bs = vertex->position();
      break;
   }

   // trigger info
   edm::Handle < edm::TriggerResults > hltresults;
   iEvent.getByLabel(triggerTag_, hltresults);

   // ----------------------------------------------------------------------------- //
   // fill MET histo 
   // ----------------------------------------------------------------------------- //

   for (edm::View<pat::MET>::const_iterator it = Met_rec.begin(); it != Met_rec.end(); ++it) {
      h_met->Fill(it->pt());
      break;
   }

   // ----------------------------------------------------------------------------- //
   // count # of jets 
   // ----------------------------------------------------------------------------- //
   int NJets = 0;
   for (edm::View<pat::Jet>::const_iterator it = Jets_rec.begin(); it != Jets_rec.end(); ++it) {
      if( goodJet_HT(&(*it)) ) NJets++;
   }

   // ----------------------------------------------------------------------------- //
   // check trigger results
   // ----------------------------------------------------------------------------- //

   // define trigger bools
   bool triggered_tag = false;
   bool triggered_probe_HT650 = false;
   bool triggered_probe_NoPUHT650 = false;
   bool triggered_probe_PFHT = false;
   bool triggered_probe_PFNoPUHT = false;

   // tag for HT + MHT
   const edm::TriggerNames & triggerNames_ = iEvent.triggerNames(*hltresults);
   for (unsigned int itrig = 0; itrig != hltresults->size(); ++itrig) {
      std::string trigName = triggerNames_.triggerName(itrig);
      //  std::cout<<" trigger name = " << trigName << std::endl;
      bool thistrig = false;
      if (trigName.find("HLT_Ele27_WP80_v") != std::string::npos) {
         int currentPrescale = hltConfig_.prescaleValue(iEvent, iSetup, trigName);
         h_HLT_Ele27_WP80_prescale->Fill(currentPrescale);
         thistrig = hltresults->accept(itrig);
      }
      if (thistrig)
         triggered_tag = true;
   }

   // select appropriate run range for trigger under study
   if( iEvent.id().run() < 198022 ) { // <  198022 for PFHT, >= 198022 for PFNoPUHT
      // probe for HT + MHT HLT_PFHT350_PFMET100
      for (unsigned int itrig = 0; itrig != hltresults->size(); ++itrig) {
         std::string trigName = triggerNames_.triggerName(itrig);
         //  std::cout<<" trigger name = " << trigName << std::endl;
         bool thistrig = false;
         bool thistrig_HT = false;
         if (trigName.find("HLT_PFHT350_PFMET100_v") != std::string::npos)
            thistrig = hltresults->accept(itrig);
         if (thistrig) 
            triggered_probe_PFHT = true;
         if (trigName.find("HLT_PFHT650_v") != std::string::npos) {
            thistrig_HT = hltresults->accept(itrig);
            int currentPrescale = hltConfig_.prescaleValue(iEvent, iSetup, trigName);
            h_HLT_PFHT650_prescale->Fill(currentPrescale);
         }
         if (thistrig_HT) 
            triggered_probe_HT650 = true;
      }
   }
   else {
      // probe for HT + MHT HLT_PFNoPUHT350_PFMET100
      for (unsigned int itrig = 0; itrig != hltresults->size(); ++itrig) {
         std::string trigName = triggerNames_.triggerName(itrig);
         //  std::cout<<" trigger name = " << trigName << std::endl;
         bool thistrig = false;
         bool thistrig_HT = false;
         if (trigName.find("HLT_PFNoPUHT350_PFMET100_v") != std::string::npos)
            thistrig = hltresults->accept(itrig);
         if (thistrig)
            triggered_probe_PFNoPUHT = true;
         if (trigName.find("HLT_PFNoPUHT650_v") != std::string::npos) {
            thistrig_HT = hltresults->accept(itrig);
         }
         if (thistrig_HT) 
            triggered_probe_NoPUHT650 = true;
      }  
   }

   // ----------------------------------------------------------------------------- //
   // check if jets in event fulfill jet ID
   // ----------------------------------------------------------------------------- //
   bool goodJetEvent = true;
   for (edm::View<pat::Jet>::const_iterator it = Jets_rec.begin(); it != Jets_rec.end(); ++it) {
      if ( it->pt() > 30. ) {
         PFJetIDSelectionFunctor jetIDLoosePF(PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::LOOSE);
         pat::strbitset ret = jetIDLoosePF.getBitTemplate();
         ret.set(false);
         bool loose = jetIDLoosePF(*it, ret);
         if (!loose) {
            goodJetEvent = false;
            break;
         } 
      }
   }

   // ----------------------------------------------------------------------------- //
   //// Fill the histograms
   // ----------------------------------------------------------------------------- //
   // derive turn-ons for NJet bins
   if( NJets == 3 || NJets == 4 || NJets == 5  ) {
      
      double HT = 0.;
      math::PtEtaPhiMLorentzVector MHT(0., 0., 0., 0.);
      for (edm::View<pat::Jet>::const_iterator it = Jets_rec.begin(); it != Jets_rec.end(); ++it) {
         if (goodJet_MHT(&(*it))) {
            MHT -= it->p4();
         }  
         if (goodJet_HT(&(*it))) {
            HT += it->pt();
         }
      }
      
      if( iEvent.id().run() < 198022 ) { // <  198022 for PFHT, >= 198022 for PFNoPUHT
         // fill histos for MHT turn-on
         if (triggered_tag && HT > 500. /*&& goodJetEvent*/) {
            h_MHTallprobes_PFHT->Fill(MHT.pt());
            if( MHT.pt() > 200. ) {
               h_MHTallprobes_total_PFHT->Fill(0.);
            }
            if( triggered_probe_PFHT ) {
               h_MHTpassedprobes_PFHT->Fill(MHT.pt());
               if( MHT.pt() > 200. ) {
                  h_MHTpassedprobes_total_PFHT->Fill(0.);
               }
            }
         }
         
         // fill histos for HT turn-on
         if (triggered_tag && MHT.pt() > 200. /*&& goodJetEvent*/) {
            h_HTallprobes_PFHT->Fill(HT);
            if( HT > 500. ) {
               h_HTallprobes_total_PFHT->Fill(0.);
            }
            if( triggered_probe_PFHT ) {
               h_HTpassedprobes_PFHT->Fill(HT);
               if( HT > 500. ) {
                  h_HTpassedprobes_total_PFHT->Fill(0.);
               }
            }
         }

         // fill histos for 2D eff
         if (triggered_tag) {
            h_allprobes_PFHT->Fill(HT, MHT.pt());
            if( triggered_probe_PFHT ) {
               h_passedprobes_PFHT->Fill(HT, MHT.pt());
            }
         }

         // fill histos for HT trigger turn-on
         if (triggered_tag /*&& goodJetEvent*/) {
            h_HTallprobes_PFHT650->Fill(HT);
            if(triggered_probe_HT650) {
               h_HTpassedprobes_PFHT650->Fill(HT);
            }
         }
      }
      else {
         // fill histos for MHT turn-on
         if (triggered_tag && HT > 500. /*&& goodJetEvent*/) {
            h_MHTallprobes_PFNoPUHT->Fill(MHT.pt());
            if( MHT.pt() > 200. ) {
               h_MHTallprobes_total_PFNoPUHT->Fill(0.);
            }
            if( triggered_probe_PFNoPUHT ) {
               h_MHTpassedprobes_PFNoPUHT->Fill(MHT.pt());
               if( MHT.pt() > 200. ) {
                  h_MHTpassedprobes_total_PFNoPUHT->Fill(0.);
               }
            }
         }
         
         // fill histos for HT turn-on
         if (triggered_tag && MHT.pt() > 200. /*&& goodJetEvent*/) {
            h_HTallprobes_PFNoPUHT->Fill(HT);
            if( HT > 500. ) {
               h_HTallprobes_total_PFNoPUHT->Fill(0.);
            }
            if( triggered_probe_PFNoPUHT ) {
               h_HTpassedprobes_PFNoPUHT->Fill(HT);
               if( HT > 500. ) {
                  h_HTpassedprobes_total_PFNoPUHT->Fill(0.);
               }
            }
         }

         // fill histos for 2D eff
         if (triggered_tag) {
            h_allprobes_PFNoPUHT->Fill(HT, MHT.pt());
            if( triggered_probe_PFNoPUHT ) {
               h_passedprobes_PFNoPUHT->Fill(HT, MHT.pt());
            }
         }
         
         // fill histos for HT trigger turn-on
         if (triggered_tag /*&& goodJetEvent*/) {
            h_HTallprobes_PFNoPUHT650->Fill(HT);
            if(triggered_probe_NoPUHT650) {
               h_HTpassedprobes_PFNoPUHT650->Fill(HT);
            }
         }
      }
      
      // ----------------------------------------------------------------------------- //
      // check events which do NOT pass the signal trigger
      // ----------------------------------------------------------------------------- //
      int jetCounter = 0;
      // control plots
      if (triggered_tag && HT > 500. && MHT.pt() > 200. && !(triggered_probe_PFNoPUHT || triggered_probe_PFHT)) {
         cout << "Bad event EventNumber:" << iEvent.id().event() << endl;
         cout << "Bad event LumiNumber:" << iEvent.id().luminosityBlock() << endl;
         cout << "Bad event RunNumber:" << iEvent.id().run() << endl;
         cout << "Bad event HT:" << HT << endl;
         cout << "Bad event MHT:" << MHT.pt() << endl;
         
         for (edm::View<pat::Jet>::const_iterator it = Jets_rec.begin(); it != Jets_rec.end(); ++it) {
            jetCounter++;
            if (goodJet_MHT(&(*it))) {
               cout << "MHT jet:" << jetCounter << ": pt:" << it->pt() << endl;
               cout << "MHT jet:" << jetCounter << ": eta:" << it->eta() << endl;
               cout << "MHT jet:" << jetCounter << ": phi:" << it->phi() << endl;
               cout << "MHT jet:" << jetCounter << ": photonEnergyFraction:" << it->photonEnergyFraction() << endl;
               cout << "MHT jet:" << jetCounter << ": electronEnergyFraction:" << it->electronEnergyFraction() << endl;
               cout << "MHT jet:" << jetCounter << ": muonEnergyFraction:" << it->muonEnergyFraction() << endl;
               cout << "MHT jet:" << jetCounter << ": HFHadronEnergyFraction:" << it->HFHadronEnergyFraction() << endl;
               cout << "MHT jet:" << jetCounter << ": HFEMEnergyFraction:" << it->HFEMEnergyFraction() << endl;
               cout << "MHT jet:" << jetCounter << ": chargedHadronMultiplicity:" << it->chargedHadronMultiplicity() << endl;
               cout << "MHT jet:" << jetCounter << ": neutralHadronMultiplicity:" << it->neutralHadronMultiplicity() << endl;
               cout << "MHT jet:" << jetCounter << ": photonMultiplicity:" << it->photonMultiplicity() << endl;
               cout << "MHT jet:" << jetCounter << ": electronMultiplicity:" << it->electronMultiplicity() << endl;
               cout << "MHT jet:" << jetCounter << ": HFHadronMultiplicity:" << it->HFHadronMultiplicity() << endl;
               cout << "MHT jet:" << jetCounter << ": HFEMMultiplicity:" << it->HFEMMultiplicity() << endl;
            }
         
            if (goodJet_HT(&(*it))) {
               cout << "HT jet:" << jetCounter << ": pt:" << it->pt() << endl;
               cout << "HT jet:" << jetCounter << ": eta:" << it->eta() << endl;
               cout << "HT jet:" << jetCounter << ": phi:" << it->phi() << endl;
               cout << "HT jet:" << jetCounter << ": photonEnergyFraction:" << it->photonEnergyFraction() << endl;
               cout << "HT jet:" << jetCounter << ": electronEnergyFraction:" << it->electronEnergyFraction() << endl;
               cout << "HT jet:" << jetCounter << ": muonEnergyFraction:" << it->muonEnergyFraction() << endl;
               cout << "HT jet:" << jetCounter << ": HFHadronEnergyFraction:" << it->HFHadronEnergyFraction() << endl;
               cout << "HT jet:" << jetCounter << ": HFEMEnergyFraction:" << it->HFEMEnergyFraction() << endl;
               cout << "HT jet:" << jetCounter << ": chargedHadronMultiplicity:" << it->chargedHadronMultiplicity() << endl;
               cout << "HT jet:" << jetCounter << ": neutralHadronMultiplicity:" << it->neutralHadronMultiplicity() << endl;
               cout << "HT jet:" << jetCounter << ": photonMultiplicity:" << it->photonMultiplicity() << endl;
               cout << "HT jet:" << jetCounter << ": electronMultiplicity:" << it->electronMultiplicity() << endl;
               cout << "HT jet:" << jetCounter << ": HFHadronMultiplicity:" << it->HFHadronMultiplicity() << endl;
               cout << "HT jet:" << jetCounter << ": HFEMMultiplicity:" << it->HFEMMultiplicity() << endl;
            }
         }
      }

      // ----------------------------------------------------------------------------- //
      // get online HT and MHT --> does not work properly for HT
      // ----------------------------------------------------------------------------- //
      //--- get trigger event
      edm::InputTag trigEventTag("hltTriggerSummaryAOD::HLT"); 
      edm::Handle < trigger::TriggerEvent > trigEvent; 
      iEvent.getByLabel(trigEventTag,trigEvent);
      
      //--- specify trigger filter names ---//
      std::string filterName_ht("hltPFHT350"); 
      std::string filterName_mht("hltPFMHT150Filter"); 
      
      //--- get index of specified trigger filter ---//
      trigger::size_type filterIndex_ht = 
         trigEvent->filterIndex(edm::InputTag(filterName_ht,"",trigEventTag.process())); 
      trigger::size_type filterIndex_mht = 
         trigEvent->filterIndex(edm::InputTag(filterName_mht,"",trigEventTag.process())); 
      
      //--- define online HT and MHT ---//
      float hltHt(0);
      float hltMHt(0);
      
      //--- fill online HT ---// --> does not work properly for HT
      if(filterIndex_ht < trigEvent->sizeFilters()){ 
         const trigger::Keys& trigKeys = trigEvent->filterKeys(filterIndex_ht); 
         const trigger::TriggerObjectCollection & trigObjColl(trigEvent->getObjects());
         //now loop over the trigger objects passing filter
         for(trigger::Keys::const_iterator keyIt = trigKeys.begin(); keyIt != trigKeys.end(); ++keyIt){ 
            const trigger::TriggerObject& obj = trigObjColl[*keyIt];
            hltHt = obj.pt();
            //    cout<< "hltHT:" << hltHt << endl;
         }   
      }
      
      //--- fill online MHT ---// 
      if(filterIndex_mht < trigEvent->sizeFilters()){ 
         const trigger::Keys& trigKeys = trigEvent->filterKeys(filterIndex_mht); 
         const trigger::TriggerObjectCollection & trigObjColl(trigEvent->getObjects());
         //now loop over the trigger objects passing filter
         for(trigger::Keys::const_iterator keyIt = trigKeys.begin(); keyIt != trigKeys.end(); ++keyIt){ 
            const trigger::TriggerObject& obj = trigObjColl[*keyIt];
            hltMHt = obj.pt();
         }   
      }
      
      // fill correlation histos online/offline HT/MHT --> does not work properly for HT
      if(triggered_probe_PFHT || triggered_probe_PFNoPUHT) {
         h_Corr_OnlineHT_OfflineHT->Fill(hltHt, HT);
         h_Corr_OnlineMHT_OfflineMHT->Fill(hltMHt, MHT.pt());
         for (edm::View<pat::MET>::const_iterator it = Met_rec.begin(); it != Met_rec.end(); ++it) {
            h_Corr_OnlineMHT_OfflineMET->Fill(hltMHt, it->pt());
            break;
         }
      }
   }
}

// ------------ method called once each job just before starting event loop  ------------
void 
Trigger::beginJob()
{
   edm::Service < TFileService > fs;
   if (!fs) {
      throw edm::Exception(edm::errors::Configuration, "TFile Service is not registered in cfg file");
   }

   h_HLT_Ele27_WP80_prescale = fs->make < TH1F > ("HLT_Ele27_WP80_prescale", "prescale", 1000, 0., 1000.);
   h_HLT_Ele27_WP80_prescale->Sumw2();
   h_HLT_PFHT650_prescale = fs->make < TH1F > ("HLT_PFHT650_prescale", "prescale", 1000, 0., 1000.);
   h_HLT_PFHT650_prescale->Sumw2();
  
   h_met = fs->make < TH1F > ("met", "Met (Type I)", 100, 0., 1000.);
   h_met->Sumw2();

   // ----------------------------------------------------------------------------- //

   h_MHTallprobes_PFHT = fs->make <TH1F> ("MHTallprobes_PFHT", "MHT all probes", 100, 0., 500.);
   h_MHTallprobes_PFHT->Sumw2();
   h_MHTpassedprobes_PFHT = fs->make <TH1F> ("MHTpassedprobes_PFHT", "MHT passed probes", 100, 0., 500.);
   h_MHTpassedprobes_PFHT->Sumw2();
   h_MHTallprobes_total_PFHT = fs->make <TH1F> ("MHTallprobes_total_PFHT", "MHT all probes", 1, -0.5, 0.5);
   h_MHTallprobes_total_PFHT->Sumw2();
   h_MHTpassedprobes_total_PFHT = fs->make <TH1F> ("MHTpassedprobes_total_PFHT", "MHT passed probes", 
                                                   1, -0.5, 0.5);
   h_MHTpassedprobes_total_PFHT->Sumw2();
   h_MHTallprobes_intEff_PFHT = fs->make <TH1F> ("MHTallprobes_intEff_PFHT", "MHT all probes", 
                                                 100, 0., 500.);
   h_MHTallprobes_intEff_PFHT->Sumw2();
   h_MHTpassedprobes_intEff_PFHT=fs->make<TH1F>("MHTpassedprobes_intEff_PFHT","MHT passed probes",
                                                100, 0., 500.);
   h_MHTpassedprobes_intEff_PFHT->Sumw2();

   ////////////////////////////////////////////////////

   h_HTallprobes_PFHT = fs->make < TH1F > ("HTallprobes_PFHT", "HT all probes", 100, 0., 2000.);
   h_HTallprobes_PFHT->Sumw2();
   h_HTpassedprobes_PFHT = fs->make < TH1F > ("HTpassedprobes_PFHT", "HT passed probes", 100, 0., 2000.);
   h_HTpassedprobes_PFHT->Sumw2();
   h_HTallprobes_total_PFHT = fs->make < TH1F > ("HTallprobes_total_PFHT", "HT all probes", 1, -0.5, 0.5);
   h_HTallprobes_total_PFHT->Sumw2();
   h_HTpassedprobes_total_PFHT = fs->make<TH1F> ("HTpassedprobes_total_PFHT", "HT passed probes", 
                                                 1, -0.5, 0.5);
   h_HTpassedprobes_total_PFHT->Sumw2();
   h_HTallprobes_intEff_PFHT = fs->make < TH1F > ("HTallprobes_intEff_PFHT", "HT all probes",
                                                  100, 0., 2000.);
   h_HTallprobes_intEff_PFHT->Sumw2();
   h_HTpassedprobes_intEff_PFHT=fs->make<TH1F>("HTpassedprobes_intEff_PFHT", "HT passed probes",
                                               100, 0., 2000.);
   h_HTpassedprobes_intEff_PFHT->Sumw2();

   ////////////////////////////////////////////////////

   h_MHTallprobes_PFNoPUHT = fs->make <TH1F> ("MHTallprobes_PFNoPUHT", "MHT all probes", 100, 0., 500.);
   h_MHTallprobes_PFNoPUHT->Sumw2();
   h_MHTpassedprobes_PFNoPUHT = fs->make <TH1F> ("MHTpassedprobes_PFNoPUHT", "MHT passed probes",
                                                 100, 0., 500.);
   h_MHTpassedprobes_PFNoPUHT->Sumw2();
   h_MHTallprobes_total_PFNoPUHT = fs->make <TH1F> ("MHTallprobes_total_PFNoPUHT", "MHT all probes",
                                                    1, -0.5, 0.5);
   h_MHTallprobes_total_PFNoPUHT->Sumw2();
   h_MHTpassedprobes_total_PFNoPUHT = fs->make<TH1F>("MHTpassedprobes_total_PFNoPUHT", "MHT passed probes",
                                                     1, -0.5, 0.5);
   h_MHTpassedprobes_total_PFNoPUHT->Sumw2();
   h_MHTallprobes_intEff_PFNoPUHT = fs->make <TH1F> ("MHTallprobes_intEff_PFNoPUHT", "MHT all probes",
                                                     100, 0., 500.);
   h_MHTallprobes_intEff_PFNoPUHT->Sumw2();
   h_MHTpassedprobes_intEff_PFNoPUHT=fs->make<TH1F>("MHTpassedprobes_intEff_PFNoPUHT","MHT passed probes",
                                                    100, 0., 500.);
   h_MHTpassedprobes_intEff_PFNoPUHT->Sumw2();

   ////////////////////////////////////////////////////

   h_HTallprobes_PFNoPUHT = fs->make < TH1F > ("HTallprobes_PFNoPUHT", "HT all probes", 100, 0., 2000.);
   h_HTallprobes_PFNoPUHT->Sumw2();
   h_HTpassedprobes_PFNoPUHT = fs->make < TH1F > ("HTpassedprobes_PFNoPUHT", "HT passed probes",
                                                  100, 0., 2000.);
   h_HTpassedprobes_PFNoPUHT->Sumw2();
   h_HTallprobes_total_PFNoPUHT = fs->make < TH1F > ("HTallprobes_total_PFNoPUHT", "HT all probes",
                                                     1, -0.5, 0.5);
   h_HTallprobes_total_PFNoPUHT->Sumw2();
   h_HTpassedprobes_total_PFNoPUHT = fs->make<TH1F> ("HTpassedprobes_total_PFNoPUHT", "HT passed probes",
                                                     1, -0.5, 0.5);
   h_HTpassedprobes_total_PFNoPUHT->Sumw2();
   h_HTallprobes_intEff_PFNoPUHT = fs->make < TH1F > ("HTallprobes_intEff_PFNoPUHT", "HT all probes",
                                                      100, 0., 2000.);
   h_HTallprobes_intEff_PFNoPUHT->Sumw2();
   h_HTpassedprobes_intEff_PFNoPUHT=fs->make<TH1F>("HTpassedprobes_intEff_PFNoPUHT", "HT passed probes",
                                                   100, 0., 2000.);
   h_HTpassedprobes_intEff_PFNoPUHT->Sumw2();

   // ----------------------------------------------------------------------------- //

   h_HTallprobes_PFHT650 = fs->make < TH1F > ("HTallprobes_PFHT650", "HT all probes", 100, 0., 2000.);
   h_HTallprobes_PFHT650->Sumw2();
   h_HTpassedprobes_PFHT650 = fs->make < TH1F > ("HTpassedprobes_PFHT650", "HT passed probes", 100, 0., 2000.);
   h_HTpassedprobes_PFHT650->Sumw2();
   h_HTallprobes_total_PFHT650 = fs->make < TH1F > ("HTallprobes_total_PFHT650", "HT all probes", 1, -0.5, 0.5);
   h_HTallprobes_total_PFHT650->Sumw2();
   h_HTpassedprobes_total_PFHT650 = fs->make<TH1F> ("HTpassedprobes_total_PFHT650", "HT passed probes", 
                                                 1, -0.5, 0.5);
   h_HTpassedprobes_total_PFHT650->Sumw2();
   h_HTallprobes_intEff_PFHT650 = fs->make < TH1F > ("HTallprobes_intEff_PFHT650", "HT all probes",
                                                  100, 0., 2000.);
   h_HTallprobes_intEff_PFHT650->Sumw2();
   h_HTpassedprobes_intEff_PFHT650=fs->make<TH1F>("HTpassedprobes_intEff_PFHT650", "HT passed probes",
                                               100, 0., 2000.);
   h_HTpassedprobes_intEff_PFHT650->Sumw2();

   // ----------------------------------------------------------------------------- //

   h_allprobes_PFHT = fs->make <TH2F> ("allprobes_PFHT", "all probes", 100, 0., 2000., 100, 0., 500.);
   h_allprobes_PFHT->Sumw2();

   h_allprobes_PFNoPUHT = fs->make <TH2F>("allprobes_PFNoPUHT", "all probes", 100, 0., 2000., 100, 0., 500.);
   h_allprobes_PFNoPUHT->Sumw2();
   h_passedprobes_PFHT = fs->make<TH2F>("passedprobes_PFHT", "passed probes", 100, 0., 2000., 100, 0., 500.);
   h_passedprobes_PFHT->Sumw2();
   h_passedprobes_PFNoPUHT = fs->make<TH2F>("passedprobes_PFNoPUHT", "passed probes", 100, 0., 2000., 100, 0., 500.);
   h_passedprobes_PFNoPUHT->Sumw2();
   
   // ----------------------------------------------------------------------------- //

   h_HTallprobes_PFNoPUHT650 = fs->make < TH1F > ("HTallprobes_PFNoPUHT650", "HT all probes", 100, 0., 2000.);
   h_HTallprobes_PFNoPUHT650->Sumw2();
   h_HTpassedprobes_PFNoPUHT650 = fs->make < TH1F > ("HTpassedprobes_PFNoPUHT650", "HT passed probes", 100, 0., 2000.);
   h_HTpassedprobes_PFNoPUHT650->Sumw2();
   h_HTallprobes_total_PFNoPUHT650 = fs->make < TH1F > ("HTallprobes_total_PFNoPUHT650", "HT all probes", 1, -0.5, 0.5);
   h_HTallprobes_total_PFNoPUHT650->Sumw2();
   h_HTpassedprobes_total_PFNoPUHT650 = fs->make<TH1F> ("HTpassedprobes_total_PFNoPUHT650", "HT passed probes", 
                                                 1, -0.5, 0.5);
   h_HTpassedprobes_total_PFNoPUHT650->Sumw2();
   h_HTallprobes_intEff_PFNoPUHT650 = fs->make < TH1F > ("HTallprobes_intEff_PFNoPUHT650", "HT all probes",
                                                  100, 0., 2000.);
   h_HTallprobes_intEff_PFNoPUHT650->Sumw2();
   h_HTpassedprobes_intEff_PFNoPUHT650=fs->make<TH1F>("HTpassedprobes_intEff_PFNoPUHT650", "HT passed probes",
                                               100, 0., 2000.);
   h_HTpassedprobes_intEff_PFNoPUHT650->Sumw2();

   // ----------------------------------------------------------------------------- //

   h_Corr_OnlineHT_OfflineHT = fs->make < TH2F > ("corr_online_offlineHT","Correlation Online/Offline HT",
                                                  100, 0., 1000., 100, 0., 1000.);
   h_Corr_OnlineHT_OfflineHT->Sumw2();
   h_Corr_OnlineMHT_OfflineMHT= fs->make < TH2F > ("corr_online_offlineMHT","Correlation Online/Offline MHT",
                                                   100, 0., 1000., 100, 0., 1000.);
   h_Corr_OnlineMHT_OfflineMHT->Sumw2();
   h_Corr_OnlineMHT_OfflineMET= fs->make < TH2F > ("corr_online_offlineMET","Correlation Online/Offline MET",
                                                   100, 0., 1000., 100, 0., 1000.);
   h_Corr_OnlineMHT_OfflineMET->Sumw2();

   // ----------------------------------------------------------------------------- //
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Trigger::endJob() 
{
   // ----------------------------------------------------------------------------- //
   // calculate integrated efficiencies
   // ----------------------------------------------------------------------------- //
   double MHTallprobes_error_PFHT;
   double MHTpassedprobes_error_PFHT;
   for(int i = 1; i< h_MHTallprobes_PFHT->GetNbinsX(); i++) {
      if( h_MHTallprobes_PFHT->GetBinContent(i) != 0 ) {
         h_MHTallprobes_intEff_PFHT->SetBinContent(i, h_MHTallprobes_PFHT->IntegralAndError(i, h_MHTallprobes_PFHT->GetNbinsX(),MHTallprobes_error_PFHT));
         h_MHTallprobes_intEff_PFHT->SetBinError(i, MHTallprobes_error_PFHT);
         h_MHTpassedprobes_intEff_PFHT->SetBinContent(i, h_MHTpassedprobes_PFHT->IntegralAndError(i, h_MHTallprobes_PFHT->GetNbinsX(),MHTpassedprobes_error_PFHT));
         h_MHTpassedprobes_intEff_PFHT->SetBinError(i, MHTpassedprobes_error_PFHT);
      }
   }

   double HTallprobes_error_PFHT;
   double HTpassedprobes_error_PFHT;
   for(int i = 1; i< h_HTallprobes_PFHT->GetNbinsX(); i++) {
      if( h_HTallprobes_PFHT->GetBinContent(i) != 0 ) {
         h_HTallprobes_intEff_PFHT->SetBinContent(i, h_HTallprobes_PFHT->IntegralAndError(i, h_HTallprobes_PFHT->GetNbinsX(), HTallprobes_error_PFHT));
         h_HTallprobes_intEff_PFHT->SetBinError(i, HTallprobes_error_PFHT); 
         h_HTpassedprobes_intEff_PFHT->SetBinContent(i, h_HTpassedprobes_PFHT->IntegralAndError(i, h_HTallprobes_PFHT->GetNbinsX(),HTpassedprobes_error_PFHT));
         h_HTpassedprobes_intEff_PFHT->SetBinError(i, HTpassedprobes_error_PFHT); 
      }
   }

   double HTallprobes_error_PFHT650;
   double HTpassedprobes_error_PFHT650;
   for(int i = 1; i< h_HTallprobes_PFHT650->GetNbinsX(); i++) {
      if( h_HTallprobes_PFHT650->GetBinContent(i) != 0 ) {
         h_HTallprobes_intEff_PFHT650->SetBinContent(i, h_HTallprobes_PFHT650->IntegralAndError(i, h_HTallprobes_PFHT650->GetNbinsX(), HTallprobes_error_PFHT650));
         h_HTallprobes_intEff_PFHT650->SetBinError(i, HTallprobes_error_PFHT650); 
         h_HTpassedprobes_intEff_PFHT650->SetBinContent(i, h_HTpassedprobes_PFHT650->IntegralAndError(i, h_HTallprobes_PFHT650->GetNbinsX(),HTpassedprobes_error_PFHT650));
         h_HTpassedprobes_intEff_PFHT650->SetBinError(i, HTpassedprobes_error_PFHT650); 
      }
   }

   double MHTallprobes_error_PFNoPUHT;
   double MHTpassedprobes_error_PFNoPUHT;
   for(int i = 1; i< h_MHTallprobes_PFNoPUHT->GetNbinsX(); i++) {
      if( h_MHTallprobes_PFNoPUHT->GetBinContent(i) != 0 ) {
         h_MHTallprobes_intEff_PFNoPUHT->SetBinContent(i, h_MHTallprobes_PFNoPUHT->IntegralAndError(i, h_MHTallprobes_PFNoPUHT->GetNbinsX(),MHTallprobes_error_PFNoPUHT));
         h_MHTallprobes_intEff_PFNoPUHT->SetBinError(i, MHTallprobes_error_PFNoPUHT);
         h_MHTpassedprobes_intEff_PFNoPUHT->SetBinContent(i, h_MHTpassedprobes_PFNoPUHT->IntegralAndError(i, h_MHTallprobes_PFNoPUHT->GetNbinsX(),MHTpassedprobes_error_PFNoPUHT));
         h_MHTpassedprobes_intEff_PFNoPUHT->SetBinError(i, MHTpassedprobes_error_PFNoPUHT);
      }
   }

   double HTallprobes_error_PFNoPUHT;
   double HTpassedprobes_error_PFNoPUHT;
   for(int i = 1; i< h_HTallprobes_PFNoPUHT->GetNbinsX(); i++) {
      if( h_HTallprobes_PFNoPUHT->GetBinContent(i) != 0 ) {
         h_HTallprobes_intEff_PFNoPUHT->SetBinContent(i, h_HTallprobes_PFNoPUHT->IntegralAndError(i, h_HTallprobes_PFNoPUHT->GetNbinsX(), HTallprobes_error_PFNoPUHT));
         h_HTallprobes_intEff_PFNoPUHT->SetBinError(i, HTallprobes_error_PFNoPUHT); 
         h_HTpassedprobes_intEff_PFNoPUHT->SetBinContent(i, h_HTpassedprobes_PFNoPUHT->IntegralAndError(i, h_HTallprobes_PFNoPUHT->GetNbinsX(),HTpassedprobes_error_PFNoPUHT));
         h_HTpassedprobes_intEff_PFNoPUHT->SetBinError(i, HTpassedprobes_error_PFNoPUHT); 
      }
   }

   double HTallprobes_error_PFNoPUHT650;
   double HTpassedprobes_error_PFNoPUHT650;
   for(int i = 1; i< h_HTallprobes_PFNoPUHT650->GetNbinsX(); i++) {
      if( h_HTallprobes_PFNoPUHT650->GetBinContent(i) != 0 ) {
         h_HTallprobes_intEff_PFNoPUHT650->SetBinContent(i, h_HTallprobes_PFNoPUHT650->IntegralAndError(i, h_HTallprobes_PFNoPUHT650->GetNbinsX(), HTallprobes_error_PFNoPUHT650));
         h_HTallprobes_intEff_PFNoPUHT650->SetBinError(i, HTallprobes_error_PFNoPUHT650); 
         h_HTpassedprobes_intEff_PFNoPUHT650->SetBinContent(i, h_HTpassedprobes_PFNoPUHT650->IntegralAndError(i, h_HTallprobes_PFNoPUHT650->GetNbinsX(),HTpassedprobes_error_PFNoPUHT650));
         h_HTpassedprobes_intEff_PFNoPUHT650->SetBinError(i, HTpassedprobes_error_PFNoPUHT650); 
      }
   }
}

// ---------------------------------------------------------------------------------------

bool Trigger::goodJet_MHT(const pat::Jet* jet) {
   if (jet->pt() < jet_pt_cut_MHT_)
      return false;
   if (fabs(jet->eta()) > jet_eta_cut_MHT_)
      return false;

   return true;
}

bool Trigger::goodJet_HT(const pat::Jet* jet) {
   if (jet->pt() < jet_pt_cut_HT_)
      return false;
   if (fabs(jet->eta()) > jet_eta_cut_HT_)
      return false;

   return true;
}

//define this as a plug-in
DEFINE_FWK_MODULE(Trigger);
