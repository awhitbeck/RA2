// -*- C++ -*-
//
// Package:    MCResolutions
// Class:      MCResolutions
//
/**\class MCResolutions MCResolutions.cc JetResolutionFromMC/MCResolutions/src/MCResolutions.cc

 Description: [one line class summary]

 Implementation:
 [Notes on implementation]
 */
//
// Original Author:  Christian Sander,,,
//         Created:  Wed Oct  6 18:22:21 CEST 2010
// $Id: MCResolutions.cc,v 1.2 2012/09/10 08:07:49 csander Exp $
//
//

#include "RA2Classic/MCResolutions/interface/MCResolutions.h"

//
// constructors and destructor
//
MCResolutions::MCResolutions(const edm::ParameterSet& iConfig) {
   //now do what ever initialization is needed
   _jetTag = iConfig.getParameter<edm::InputTag> ("jetTag");
   _muonTag = iConfig.getParameter<edm::InputTag> ("muonTag");
   _genJetTag = iConfig.getParameter<edm::InputTag> ("genJetTag");
   _weightName = iConfig.getParameter<edm::InputTag> ("weightName");
   _EBRecHits = iConfig.getParameter<edm::InputTag> ("EBRecHits");
   _EERecHits = iConfig.getParameter<edm::InputTag> ("EERecHits");
   _jetMultPtCut = iConfig.getParameter<double> ("jetMultPtCut");
   _jetMultEtaCut = iConfig.getParameter<double> ("jetMultEtaCut");
   _deltaPhiDiJet = iConfig.getParameter<double> ("deltaPhiDiJet");
   _absCut3rdJet = iConfig.getParameter<double> ("absCut3rdJet");
   _relCut3rdJet = iConfig.getParameter<double> ("relCut3rdJet");
   _deltaRMatch = iConfig.getParameter<double> ("deltaRMatch");
   _deltaRMatchVeto = iConfig.getParameter<double> ("deltaRMatchVeto");
   _absPtVeto = iConfig.getParameter<double> ("absPtVeto");
   _relPtVeto = iConfig.getParameter<double> ("relPtVeto");
   _deltaRDeadECal = iConfig.getParameter<double> ("deltaRDeadECal");
   _GenJetPtCut = iConfig.getParameter<double> ("GenJetPtCut");
   _maskedEcalChannelStatusThreshold = iConfig.getParameter<int> ("maskedEcalChannelStatusThreshold");
   _fileName = iConfig.getParameter<std::string> ("fileName");

   hfile = new TFile(_fileName.c_str(), "RECREATE", "Jet response in pT and eta bins");
   //hfile->mkdir(_dirName.c_str(), _dirName.c_str());

   PtBinEdges.push_back(0);
   PtBinEdges.push_back(20);
   PtBinEdges.push_back(30);
   PtBinEdges.push_back(50);
   PtBinEdges.push_back(80);
   PtBinEdges.push_back(120);
   PtBinEdges.push_back(170);
   PtBinEdges.push_back(230);
   PtBinEdges.push_back(300);
   PtBinEdges.push_back(380);
   PtBinEdges.push_back(470);
   PtBinEdges.push_back(570);
   PtBinEdges.push_back(680);
   PtBinEdges.push_back(800);
   PtBinEdges.push_back(1000);
   PtBinEdges.push_back(1300);
   PtBinEdges.push_back(1700);
   PtBinEdges.push_back(2200);
   PtBinEdges.push_back(2800);
   PtBinEdges.push_back(3500);

   EtaBinEdges.push_back(0.0);
   EtaBinEdges.push_back(0.3);
   EtaBinEdges.push_back(0.5);
   EtaBinEdges.push_back(0.8);
   EtaBinEdges.push_back(1.1);
   EtaBinEdges.push_back(1.4);
   EtaBinEdges.push_back(1.7);
   EtaBinEdges.push_back(2.0);
   EtaBinEdges.push_back(2.3);
   EtaBinEdges.push_back(2.8);
   EtaBinEdges.push_back(3.2);
   EtaBinEdges.push_back(4.1);
   EtaBinEdges.push_back(5.0);

   //   EtaBinEdges.push_back(0.0);
   //   EtaBinEdges.push_back(0.5);
   //   EtaBinEdges.push_back(1.1);
   //   EtaBinEdges.push_back(1.7);
   //   EtaBinEdges.push_back(2.3);
   //   EtaBinEdges.push_back(3.2);
   //   EtaBinEdges.push_back(5.0);

   //// Array of histograms for jet resolutions (all jet multiplicities)
   ResizeHistoVector(h_tot_DiJet_JetResPt_Pt);
   ResizeHistoVector(h_b_DiJet_JetResPt_Pt);
   ResizeHistoVector(h_nob_DiJet_JetResPt_Pt);
   ResizeHistoVector(h_dead_DiJet_JetResPt_Pt);
   ResizeHistoVector(h_deadb_DiJet_JetResPt_Pt);

   //// Array of histograms for jet resolutions (all jet multiplicities)
   ResizeHistoVector(h_tot_JetAll_JetResPt_Pt);
   ResizeHistoVector(h_b_JetAll_JetResPt_Pt);
   ResizeHistoVector(h_nob_JetAll_JetResPt_Pt);
   ResizeHistoVector(h_dead_JetAll_JetResPt_Pt);
   ResizeHistoVector(h_deadb_JetAll_JetResPt_Pt);

   //// Array of histograms for jet resolutions (first jet)
   ResizeHistoVector(h_tot_Jet1_JetResPt_Pt);
   ResizeHistoVector(h_b_Jet1_JetResPt_Pt);
   ResizeHistoVector(h_nob_Jet1_JetResPt_Pt);
   ResizeHistoVector(h_dead_Jet1_JetResPt_Pt);
   ResizeHistoVector(h_deadb_Jet1_JetResPt_Pt);

   //// Array of histograms for jet resolutions (second jet)
   ResizeHistoVector(h_tot_Jet2_JetResPt_Pt);
   ResizeHistoVector(h_b_Jet2_JetResPt_Pt);
   ResizeHistoVector(h_nob_Jet2_JetResPt_Pt);
   ResizeHistoVector(h_dead_Jet2_JetResPt_Pt);
   ResizeHistoVector(h_deadb_Jet2_JetResPt_Pt);

   //// Array of histograms for jet resolutions (third jet)
   ResizeHistoVector(h_tot_Jet3_JetResPt_Pt);
   ResizeHistoVector(h_b_Jet3_JetResPt_Pt);
   ResizeHistoVector(h_nob_Jet3_JetResPt_Pt);
   ResizeHistoVector(h_dead_Jet3_JetResPt_Pt);
   ResizeHistoVector(h_deadb_Jet3_JetResPt_Pt);

   //// Array of histograms for jet resolutions (fourth jet)
   ResizeHistoVector(h_tot_Jet4_JetResPt_Pt);
   ResizeHistoVector(h_b_Jet4_JetResPt_Pt);
   ResizeHistoVector(h_nob_Jet4_JetResPt_Pt);
   ResizeHistoVector(h_dead_Jet4_JetResPt_Pt);
   ResizeHistoVector(h_deadb_Jet4_JetResPt_Pt);

   //// Array of histograms for jet resolutions (fifth+ jet)
   ResizeHistoVector(h_tot_Jet5p_JetResPt_Pt);
   ResizeHistoVector(h_b_Jet5p_JetResPt_Pt);
   ResizeHistoVector(h_nob_Jet5p_JetResPt_Pt);
   ResizeHistoVector(h_dead_Jet5p_JetResPt_Pt);
   ResizeHistoVector(h_deadb_Jet5p_JetResPt_Pt);

}

MCResolutions::~MCResolutions() {

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ method called to for each event  ------------
void MCResolutions::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
   using namespace std;

   // Event setup
   envSet(iSetup);

   getChannelStatusMaps();

   //Weight
   edm::Handle<double> event_weight;
   bool findWeight = iEvent.getByLabel(_weightName, event_weight);
   weight = (event_weight.isValid() ? (*event_weight) : 1.0);
   if (!findWeight) {
      cout << "Weight not found!" << endl;
   }

   //GenJets
   edm::Handle<edm::View<reco::GenJet> > Jets_gen;
   iEvent.getByLabel(_genJetTag, Jets_gen);

   //RecoJets
   edm::Handle<edm::View<pat::Jet> > Jets_rec;
   iEvent.getByLabel(_jetTag, Jets_rec);

   edm::Handle<reco::GenParticleCollection> genParticles;
   iEvent.getByLabel("genParticles", genParticles);

   //// Do DiJet selection
   bool isDiJet = false;
   const reco::GenJet* gj1 = 0;
   const reco::GenJet* gj2 = 0;
   const reco::GenJet* gj3 = 0;
   int k = 0;
   for (edm::View<reco::GenJet>::const_iterator it = Jets_gen->begin(); it != Jets_gen->end(); ++it) {
      ++k;
      if (k == 1)
         gj1 = &(*it);
      if (k == 2)
         gj2 = &(*it);
      if (k == 3) {
         gj3 = &(*it);
         break;
      }
   }
   if (k > 1) {
      if (deltaPhi(gj1->phi(), gj2->phi()) > _deltaPhiDiJet) {
         if (k == 2)
            isDiJet = true;
         if (k == 3) {
            //if (gj3->pt()<(_relCut3rdJet*(gj1->pt()+gj2->pt())/2)){
            if (gj3->pt() < (_relCut3rdJet * gj1->pt())) {
               isDiJet = true;
            }
         }
      }
   }

   //// Calculate jet multiplicity
   double NGenJet = 0;
   for (edm::View<reco::GenJet>::const_iterator it = Jets_gen-> begin(); it != Jets_gen-> end(); ++it) {
      if (it->pt() > _jetMultPtCut && abs(it->eta()) < _jetMultEtaCut)
         ++NGenJet;
   }

   int JetNo = 0;
   for (edm::View<reco::GenJet>::const_iterator it = Jets_gen->begin(); it != Jets_gen->end(); ++it) {
      ++JetNo;

      if (it->pt() < _GenJetPtCut)
         continue;

      //// First look if there is no significant GenJet near the tested GenJet
      double dRgenjet = 999.;
      double PtdRmin = 0;
      for (edm::View<reco::GenJet>::const_iterator kt = Jets_gen-> begin(); kt != Jets_gen->end(); ++kt) {
         if (&(*it) != &(*kt))
            continue;
         double dR = deltaR(*it, *kt);
         if (dR < dRgenjet) {
            dRgenjet = dR;
            PtdRmin = kt->pt();
         }
      }
      if (dRgenjet < _deltaRMatchVeto && PtdRmin / it->pt() < _relPtVeto)
         continue;
      const pat::Jet* matchedJet = 0;
      const pat::Jet* nearestJet = 0;
      double dRmatched = 999.;
      double dRnearest = 999.;
      for (edm::View<pat::Jet>::const_iterator jt = Jets_rec-> begin(); jt != Jets_rec->end(); ++jt) {
         double dR = deltaR(*it, *jt);
         if (dR < dRmatched) {
            nearestJet = matchedJet;
            dRnearest = dRmatched;
            matchedJet = &(*jt);
            dRmatched = dR;
         } else if (dR < dRnearest) {
            nearestJet = &(*jt);
            dRnearest = dR;
         }
      }

      //// look if there is no further significant CaloJet near the genJet
      if (dRmatched < _deltaRMatch && (nearestJet == 0 || dRnearest > _deltaRMatchVeto || (nearestJet->pt()
            < _absPtVeto && nearestJet->pt() / matchedJet->pt() < _relPtVeto))) {
         double res = matchedJet->pt() / it->pt();
         h_tot_JetAll_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
         if (isDiJet && JetNo < 3) {
            h_tot_DiJet_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
         }
         if (JetNo == 1) {
            h_tot_Jet1_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
         }
         if (JetNo == 2) {
            h_tot_Jet2_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
         }
         if (JetNo == 3) {
            h_tot_Jet3_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
         }
         if (JetNo == 4) {
            h_tot_Jet4_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
         }
         if (JetNo > 4) {
            h_tot_Jet5p_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
         }

         //// Use algorithmic matching for heavy flavour ID
         bool bTag = false;
         if (fabs(matchedJet->partonFlavour()) == 4 || fabs(matchedJet->partonFlavour()) == 5) {
            bTag = true;
         }

         if (bTag) {
            h_b_JetAll_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
            if (isDiJet && JetNo < 3) {
               h_b_DiJet_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
            }
            if (JetNo == 1) {
               h_b_Jet1_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
            }
            if (JetNo == 2) {
               h_b_Jet2_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
            }
            if (JetNo == 3) {
               h_b_Jet3_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
            }
            if (JetNo == 4) {
               h_b_Jet4_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
            }
            if (JetNo > 4) {
               h_b_Jet5p_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
            }
         } else {
            h_nob_JetAll_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
            if (isDiJet && JetNo < 3) {
               h_nob_DiJet_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
            }
            if (JetNo == 1) {
               h_nob_Jet1_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
            }
            if (JetNo == 2) {
               h_nob_Jet2_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
            }
            if (JetNo == 3) {
               h_nob_Jet3_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
            }
            if (JetNo == 4) {
               h_nob_Jet4_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
            }
            if (JetNo > 4) {
               h_nob_Jet5p_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
            }
         }

         //// look if matched jet points into direction of masked ECAL cluster
         for (std::map<DetId, std::vector<double> >::iterator kt = EcalAllDeadChannelsValMap.begin(); kt
         != EcalAllDeadChannelsValMap.end(); ++kt) {
            math::PtEtaPhiMLorentzVectorD Evec(100., kt->second.at(0), kt->second.at(1), 0.);
            double dR_dead = deltaR(*matchedJet, Evec);
            if (dR_dead < _deltaRDeadECal) {
               h_dead_JetAll_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
               if (isDiJet && JetNo < 3) {
                  h_dead_DiJet_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
               }
               if (JetNo == 1) {
                  h_dead_Jet1_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
               }
               if (JetNo == 2) {
                  h_dead_Jet2_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
               }
               if (JetNo == 3) {
                  h_dead_Jet3_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
               }
               if (JetNo == 4) {
                  h_dead_Jet4_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
               }
               if (JetNo > 4) {
                  h_dead_Jet5p_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
               }
               if (bTag) {
                  h_deadb_JetAll_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
                  if (isDiJet && JetNo < 3) {
                     h_deadb_DiJet_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
                  }
                  if (JetNo == 1) {
                     h_deadb_Jet1_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
                  }
                  if (JetNo == 2) {
                     h_deadb_Jet2_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
                  }
                  if (JetNo == 3) {
                     h_deadb_Jet3_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
                  }
                  if (JetNo == 4) {
                     h_deadb_Jet4_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
                  }
                  if (JetNo > 4) {
                     h_deadb_Jet5p_JetResPt_Pt.at(EtaBin(it->eta())).at(PtBin(it->pt()))->Fill(res, weight);
                  }
               }
               break;
            }
         }
      }
   }

}

// ------------ method called once each job just before starting event loop  ------------
void MCResolutions::beginJob() {

   for (unsigned int i_pt = 0; i_pt < PtBinEdges.size() - 1; ++i_pt) {
      for (unsigned int i_eta = 0; i_eta < EtaBinEdges.size() - 1; ++i_eta) {
         char hname[100];
         //// Book histograms (all jet multiplicities)
         sprintf(hname, "h_tot_JetAll_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_tot_JetAll_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_tot_JetAll_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_b_JetAll_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_b_JetAll_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_b_JetAll_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_nob_JetAll_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_nob_JetAll_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_nob_JetAll_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_dead_JetAll_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_dead_JetAll_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_dead_JetAll_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_deadb_JetAll_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_deadb_JetAll_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_deadb_JetAll_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         //// Book histograms (DiJets)
         sprintf(hname, "h_tot_DiJet_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_tot_DiJet_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_tot_DiJet_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_b_DiJet_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_b_DiJet_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_b_DiJet_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_nob_DiJet_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_nob_DiJet_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_nob_DiJet_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_dead_DiJet_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_dead_DiJet_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_dead_DiJet_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_deadb_DiJet_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_deadb_DiJet_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_deadb_DiJet_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         //// Book histograms (leading jet)
         sprintf(hname, "h_tot_Jet1_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_tot_Jet1_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_tot_Jet1_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_b_Jet1_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_b_Jet1_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_b_Jet1_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_nob_Jet1_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_nob_Jet1_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_nob_Jet1_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_dead_Jet1_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_dead_Jet1_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_dead_Jet1_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_deadb_Jet1_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_deadb_Jet1_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_deadb_Jet1_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         //// Book histograms (second jet)
         sprintf(hname, "h_tot_Jet2_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_tot_Jet2_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_tot_Jet2_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_b_Jet2_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_b_Jet2_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_b_Jet2_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_nob_Jet2_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_nob_Jet2_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_nob_Jet2_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_dead_Jet2_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_dead_Jet2_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_dead_Jet2_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_deadb_Jet2_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_deadb_Jet2_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_deadb_Jet2_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         //// Book histograms (third jet)
         sprintf(hname, "h_tot_Jet3_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_tot_Jet3_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_tot_Jet3_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_b_Jet3_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_b_Jet3_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_b_Jet3_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_nob_Jet3_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_nob_Jet3_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_nob_Jet3_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_dead_Jet3_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_dead_Jet3_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_dead_Jet3_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_deadb_Jet3_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_deadb_Jet3_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_deadb_Jet3_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         //// Book histograms (fourth jet)
         sprintf(hname, "h_tot_Jet4_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_tot_Jet4_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_tot_Jet4_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_b_Jet4_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_b_Jet4_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_b_Jet4_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_nob_Jet4_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_nob_Jet4_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_nob_Jet4_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_dead_Jet4_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_dead_Jet4_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_dead_Jet4_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_deadb_Jet4_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_deadb_Jet4_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_deadb_Jet4_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         //// Book histograms (>= fifth jet)
         sprintf(hname, "h_tot_Jet5p_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_tot_Jet5p_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_tot_Jet5p_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_b_Jet5p_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_b_Jet5p_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_b_Jet5p_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_nob_Jet5p_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_nob_Jet5p_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_nob_Jet5p_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_dead_Jet5p_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_dead_Jet5p_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_dead_Jet5p_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
         sprintf(hname, "h_deadb_Jet5p_ResponsePt_Pt%i_Eta%i", i_pt, i_eta);
         h_deadb_Jet5p_JetResPt_Pt.at(i_eta).at(i_pt) = new TH1F(hname, hname, 300, 0., 3.);
         h_deadb_Jet5p_JetResPt_Pt.at(i_eta).at(i_pt)->Sumw2();
      }
   }

   mapsReady = false;
}

// ------------ method called once each job just after ending the event loop  ------------
void MCResolutions::endJob() {

   hfile->cd();
   //hfile->cd(_dirName.c_str());
   // Save all objects in this file
   for (unsigned int i_pt = 0; i_pt < PtBinEdges.size() - 1; ++i_pt) {
      for (unsigned int i_eta = 0; i_eta < EtaBinEdges.size() - 1; ++i_eta) {
         // total
         hfile->WriteTObject(h_tot_JetAll_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_tot_DiJet_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_tot_Jet1_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_tot_Jet2_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_tot_Jet3_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_tot_Jet4_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_tot_Jet5p_JetResPt_Pt.at(i_eta).at(i_pt));
         // with btag
         hfile->WriteTObject(h_b_JetAll_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_b_DiJet_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_b_Jet1_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_b_Jet2_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_b_Jet3_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_b_Jet4_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_b_Jet5p_JetResPt_Pt.at(i_eta).at(i_pt));
         // without btag
         hfile->WriteTObject(h_nob_JetAll_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_nob_DiJet_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_nob_Jet1_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_nob_Jet2_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_nob_Jet3_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_nob_Jet4_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_nob_Jet5p_JetResPt_Pt.at(i_eta).at(i_pt));
         // in direction of dead ECAL cells
         hfile->WriteTObject(h_dead_JetAll_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_dead_DiJet_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_dead_Jet1_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_dead_Jet2_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_dead_Jet3_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_dead_Jet4_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_dead_Jet5p_JetResPt_Pt.at(i_eta).at(i_pt));
         // in direction of dead ECAL cells and with b tag
         hfile->WriteTObject(h_deadb_JetAll_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_deadb_DiJet_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_deadb_Jet1_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_deadb_Jet2_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_deadb_Jet3_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_deadb_Jet4_JetResPt_Pt.at(i_eta).at(i_pt));
         hfile->WriteTObject(h_deadb_Jet5p_JetResPt_Pt.at(i_eta).at(i_pt));
      }
   }

   hfile->cd();
   hfile->WriteObject(&PtBinEdges, "PtBinEdges");
   hfile->WriteObject(&EtaBinEdges, "EtaBinEdges");
   //hfile->ls();

   // Close the file.
   hfile->Close();

}

int MCResolutions::PtBin(const double& pt) {
   int i_pt = -1;
   for (std::vector<double>::const_iterator it = PtBinEdges.begin(); it != PtBinEdges.end(); ++it) {
      if ((*it) > pt)
         break;
      ++i_pt;
   }
   if (i_pt < 0)
      i_pt = 0;
   if (i_pt > (int) PtBinEdges.size() - 2)
      i_pt = (int) PtBinEdges.size() - 2;

   return i_pt;
}

int MCResolutions::EtaBin(const double& eta) {
   int i_eta = -1;
   for (std::vector<double>::const_iterator it = EtaBinEdges.begin(); it != EtaBinEdges.end(); ++it) {
      if ((*it) > fabs(eta))
         break;
      ++i_eta;
   }
   if (i_eta < 0)
      i_eta = 0;
   if (i_eta > (int) EtaBinEdges.size() - 2)
      i_eta = (int) EtaBinEdges.size() - 2;
   return i_eta;
}

void MCResolutions::envSet(const edm::EventSetup& iSetup) {

   ecalScale.setEventSetup(iSetup);

   iSetup.get<EcalChannelStatusRcd> ().get(ecalStatus);
   iSetup.get<CaloGeometryRecord> ().get(geometry);

   if (!ecalStatus.isValid())
      throw "Failed to get ECAL channel status!";
   if (!geometry.isValid())
      throw "Failed to get the geometry!";

}

int MCResolutions::getChannelStatusMaps() {

   if (mapsReady)
      return -1;

   EcalAllDeadChannelsValMap.clear();

   // Loop over EB ...
   for (int ieta = -85; ieta <= 85; ieta++) {
      for (int iphi = 0; iphi <= 360; iphi++) {
         if (!EBDetId::validDetId(ieta, iphi))
            continue;

         const EBDetId detid = EBDetId(ieta, iphi, EBDetId::ETAPHIMODE);
         EcalChannelStatus::const_iterator chit = ecalStatus->find(detid);
         // refer https://twiki.cern.ch/twiki/bin/viewauth/CMS/EcalChannelStatus
         int status = (chit != ecalStatus->end()) ? chit->getStatusCode() & 0x1F : -1;
         //std::cout << ieta << " " << iphi << " " << status << std:: endl;
         const CaloSubdetectorGeometry* subGeom = geometry->getSubdetectorGeometry(detid);
         const CaloCellGeometry* cellGeom = subGeom->getGeometry(detid);
         double eta = cellGeom->getPosition().eta();
         double phi = cellGeom->getPosition().phi();
         double theta = cellGeom->getPosition().theta();

         if (status >= _maskedEcalChannelStatusThreshold) {
            std::vector<double> valVec;
            valVec.push_back(eta);
            valVec.push_back(phi);
            valVec.push_back(theta);
            //std::cout << eta << " " << phi << std::endl;
            EcalAllDeadChannelsValMap.insert(std::make_pair(detid, valVec));
         }
      } // end loop iphi
   } // end loop ieta

   // Loop over EE detid
   for (int ix = 0; ix <= 100; ix++) {
      for (int iy = 0; iy <= 100; iy++) {
         for (int iz = -1; iz <= 1; iz++) {
            if (iz == 0)
               continue;
            if (!EEDetId::validDetId(ix, iy, iz))
               continue;

            const EEDetId detid = EEDetId(ix, iy, iz, EEDetId::XYMODE);
            EcalChannelStatus::const_iterator chit = ecalStatus->find(detid);
            int status = (chit != ecalStatus->end()) ? chit->getStatusCode() & 0x1F : -1;

            const CaloSubdetectorGeometry* subGeom = geometry->getSubdetectorGeometry(detid);
            const CaloCellGeometry* cellGeom = subGeom->getGeometry(detid);
            double eta = cellGeom->getPosition().eta();
            double phi = cellGeom->getPosition().phi();
            double theta = cellGeom->getPosition().theta();

            if (status >= _maskedEcalChannelStatusThreshold) {
               std::vector<double> valVec;
               valVec.push_back(eta);
               valVec.push_back(phi);
               valVec.push_back(theta);
               //std::cout << eta << " " << phi << std::endl;
               EcalAllDeadChannelsValMap.insert(std::make_pair(detid, valVec));
            }
         } // end loop iz
      } // end loop iy
   } // end loop ix

   mapsReady = true;
   return 1;
}

void MCResolutions::ResizeHistoVector(std::vector<std::vector<TH1F*> > &histoVector) {

   histoVector.resize(EtaBinEdges.size() - 1);
   for (std::vector<std::vector<TH1F*> >::iterator it = histoVector.begin(); it != histoVector.end(); ++it) {
      it->resize(PtBinEdges.size() - 1);
   }
}

//define this as a plug-in
DEFINE_FWK_MODULE( MCResolutions);
