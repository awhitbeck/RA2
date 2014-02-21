// -*- C++ -*-
//
// Package:    WeightProducer
// Class:      WeightProducer
//
/**\class WeightProducer WeightProducer.cc RA2/WeightProducer/src/WeightProducer.cc

 Description: <one line class summary>

 Implementation:
 <Notes on implementation>
 */
//
//         Created:  Tue Nov 10 17:58:04 CET 2009
// $Id: PrescaleWeightProducer.cc,v 1.1 2012/08/01 13:08:37 kheine Exp $
//
//


// system include files
#include <cmath>
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <map>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

using namespace std;
using namespace trigger;
using namespace edm;

//
// class decleration
//

class PrescaleWeightProducer: public edm::EDProducer {
   public:
      explicit PrescaleWeightProducer(const edm::ParameterSet&);
      ~PrescaleWeightProducer();

   private:
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void beginJob();
      virtual void endJob();
      virtual void beginRun(edm::Run&, edm::EventSetup const&);

      const double _startWeight;
      const double _LumiScale;
      const double _PrescaleCut;

      const bool _PFHTWeights;

      edm::InputTag _weightName;
      string _processName;
      edm::InputTag _hltTag;
      edm::InputTag _trigEvtObj;

      /// The instance of the HLTConfigProvider as a data member
      HLTConfigProvider hltConfig_;
};

PrescaleWeightProducer::PrescaleWeightProducer(const edm::ParameterSet& iConfig) :
   _startWeight(iConfig.getParameter<double> ("weight")), _LumiScale(iConfig.getParameter<double> ("LumiScale")), _PrescaleCut(iConfig.getParameter<double> ("PrescaleCut")), _PFHTWeights(iConfig.getParameter<bool> ("PFHTWeights")), 
         _weightName(iConfig.getParameter<edm::InputTag> ("weightName")), _processName(iConfig.getParameter<string> (
               "HLTProcess")), _hltTag(iConfig.getParameter<edm::InputTag> ("hltTag")), _trigEvtObj(
               iConfig.getParameter<edm::InputTag> ("trgEvtObj")) {

   if (_startWeight >= 0) {
      cout << "PrescaleWeightProducer: Using constant event weight of " << _startWeight << endl;
   } else {
      cout << "PrescaleWeightProducer: Using weight from event" << endl;
   }

   ///This is to consider the lumi-uncertainty, i.e. to scale the weights up- or down by 1sigma of the lumi-scale
   ///uncertainty. In general the scale is 1.0!
   if (_LumiScale != 1.) {
      cout << "PrescaleWeightProducer: Scaling event weights by factor " << _LumiScale << endl;
   }

   //register your products
   produces<double> ("weight");
}

PrescaleWeightProducer::~PrescaleWeightProducer() {
}

// ------------ method called to produce the data  ------------
void PrescaleWeightProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

   double resultWeight = 1.;

   //set start weight
   if (_startWeight >= 0) {
      resultWeight = _startWeight;
   } else {
      edm::Handle<double> event_weight;
      iEvent.getByLabel(_weightName, event_weight);
      resultWeight = (event_weight.isValid() ? (*event_weight) : 1.0);
   }
   ///This is to consider the lumi-uncertainty, i.e. to scale the weights up- or down by 1sigma of the lumi-scale
   ///uncertainty. In general the scale is 1.0!
   resultWeight *= _LumiScale;

   edm::Handle<edm::TriggerResults> triggerResults;
   if (!iEvent.getByLabel(_hltTag, triggerResults)) {
      cout << ">>> TRIGGER collection does not exist !!!" << endl;
      return;
   }
   const edm::TriggerNames & trigNames = iEvent.triggerNames(*triggerResults);

   map<char, int> vmap;
   pair<map<char, int>::iterator, bool> ret;

   int finalPrescale = 0;

   // ---- get prescale weights for 7 TeV data ---- //
   if( !_PFHTWeights ) {
      
      //// 1. Step: Count versions of HT triggers
      for (unsigned int i = 0; i < triggerResults->size(); i++) {
         const string trigName = trigNames.triggerName(i);
         size_t found = trigName.find("HLT_HT"); 
         if (found == string::npos)
            continue;
         found = trigName.find("0_v");
         if (found != 8)
            continue;
         //  cout << trigName << " " << hltConfig_.prescaleValue(iEvent, iSetup, trigName) << endl;
         ret = vmap.insert(pair<char, int> (trigName[11], 1)); 
         if (ret.second == false) {
            ret.first->second += 1;
         }   
      }
   
      //// 2. Step: find most frequent version of HT triggers
      int max = 0;
      char vmax = '0';
      for (map<char, int>::iterator it = vmap.begin(); it != vmap.end(); it++) {
         if ((*it).second > max) {
            max = (*it).second;
            vmax = (*it).first;
         }
      }

      //// 3. Step: save trigger names and trigger thresholds in map
      ////    Find lowest prescale of trigger which fired
      vector<double> triggerThres;
      double finalThreshold = 0;
      for (unsigned int i = 0; i < triggerResults->size(); i++) {
         const string trigName = trigNames.triggerName(i);
         size_t found = trigName.find("HLT_HT");       
         if (found == string::npos)
            continue;
         found = trigName.find("0_v");
         if (found != 8)
            continue;
         if (trigName[11] == vmax) {
            string strthr = trigName.substr(6, 3);
            double threshold = atof(strthr.c_str());
            //    cout << strthr << ": " << threshold << endl;
            triggerThres.push_back(threshold);
            if (triggerResults->accept(i)) {
               int currentPrescale = hltConfig_.prescaleValue(iEvent, iSetup, trigName);
               if (currentPrescale < finalPrescale || finalPrescale == 0) {
                  finalPrescale = currentPrescale;
                  finalThreshold = threshold;
               }
            }
         }  
      }  
   
      /*   if (finalThreshold > 0) {
           cout << "Event triggered with HLT_HT" << finalThreshold << "_v" << vmax << " with prescale: " << finalPrescale
           << endl;
           } else {
           cout << "Event not triggered with any HLT_HTXXX_v" << vmax << " trigger" << endl;
           }*/


      //// 4. Step: Make sure that a higher threshold trigger would not fire if unprescaled
      //// Get the online HT
      edm::Handle<trigger::TriggerEvent> trgEvtObj;
      if (!iEvent.getByLabel(_trigEvtObj, trgEvtObj)) {
         cout << ">>> TRIGGER event summary does not exist !!!" << endl;
         return;
      }

      const trigger::TriggerObjectCollection& toc(trgEvtObj->getObjects());
      size_t htflt(0);
      char tempStr[100];
      sprintf(tempStr, "hltHT%i::HLT", (int) finalThreshold); 
      edm::InputTag htFilter(tempStr);
      htflt = trgEvtObj->filterIndex(htFilter);

      // for (trigger::size_type i = 0; i != trgEvtObj->sizeFilters(); ++i)
      // cout << trgEvtObj->filterTag(i) << endl;

      float hltHt(0);//---online HLT
      if (htflt != trgEvtObj->sizeFilters()) {
         const trigger::Keys& htKey = trgEvtObj->filterKeys(htflt);
         for (trigger::Keys::const_iterator htkit = htKey.begin(); htkit != htKey.end(); htkit++) {
            hltHt = toc[*htkit].pt();
         }
      }
      // cout << "Online HT: " << hltHt << endl;
      double thresMax = 0;
      for (vector<double>::iterator it = triggerThres.begin(); it != triggerThres.end(); ++it) {
         //   cout << "threshold:" << *it << " " ;
         if (*it < hltHt && *it > thresMax)
            thresMax = *it;
      }
      if (thresMax > finalThreshold && finalPrescale > 1) {
         finalPrescale = 0;
         //  cout << "Event would have been triggered by prescaled trigger with threshold: " << thresMax << endl;
      }

      if (finalPrescale > _PrescaleCut && _PrescaleCut > 0.)
         finalPrescale = 0;
   }

   // ---- get prescale weights for 8 TeV data ---- //
   if( _PFHTWeights ) {

      //// 1. Step: Count versions of HT triggers
      for (unsigned int i = 0; i < triggerResults->size(); i++) {
         const string trigName = trigNames.triggerName(i);
         size_t found;
         if( iEvent.id().run() < 198022 ) {
            found = trigName.find("HLT_PFHT");
            if (found == string::npos)
               continue;
            found = trigName.find("0_v");
            if (found != 10)
               continue;
            //cout << trigName << " " << hltConfig_.prescaleValue(iEvent, iSetup, trigName) << endl;
            ret = vmap.insert(pair<char, int> (trigName[13], 1));
            if (ret.second == false) {
               ret.first->second += 1;
            }   
         }
   
         else {
            found = trigName.find("HLT_PFNoPUHT");
            if (found == string::npos)
               continue;
            found = trigName.find("0_v");
            if (found != 14)
               continue;
            //cout << trigName << " " << hltConfig_.prescaleValue(iEvent, iSetup, trigName) << endl;
            ret = vmap.insert(pair<char, int> (trigName[17], 1));
            if (ret.second == false) {
               ret.first->second += 1;
            }
         }
      }

      //// 2. Step: find most frequent version of HT triggers
      int max = 0;
      char vmax = '0';
      for (map<char, int>::iterator it = vmap.begin(); it != vmap.end(); it++) {
         if ((*it).second > max) {
            max = (*it).second;
            vmax = (*it).first;
         }
      }

      //// 3. Step: save trigger names and trigger thresholds in map
      ////    Find lowest prescale of trigger which fired
      vector<double> triggerThres;
      double finalThreshold = 0;
      for (unsigned int i = 0; i < triggerResults->size(); i++) {
         const string trigName = trigNames.triggerName(i);
         size_t found;
         if( iEvent.id().run() < 198022 ) {
            found = trigName.find("HLT_PFHT");
            if (found == string::npos)
               continue;
            found = trigName.find("0_v");
            if (found != 10)
               continue;
            if (trigName[13] == vmax) {
               string strthr = trigName.substr(8, 3);
               double threshold = atof(strthr.c_str());
               //  cout << strthr << ": " << threshold << endl;
               triggerThres.push_back(threshold);
               if (triggerResults->accept(i)) {
                  int currentPrescale = hltConfig_.prescaleValue(iEvent, iSetup, trigName);
                  if (currentPrescale < finalPrescale || finalPrescale == 0) {
                     finalPrescale = currentPrescale;
                     finalThreshold = threshold;
                  }
               }
            }  
         }  
         
         else {
            found = trigName.find("HLT_PFNoPUHT");
            if (found == string::npos)
               continue;
            found = trigName.find("0_v");
            if (found != 14)
               continue;
            if (trigName[17] == vmax) {
               string strthr = trigName.substr(12, 3);
               double threshold = atof(strthr.c_str());
               //   cout << strthr << ": " << threshold << endl;
               triggerThres.push_back(threshold);
               if (triggerResults->accept(i)) {
                  int currentPrescale = hltConfig_.prescaleValue(iEvent, iSetup, trigName);
                  if (currentPrescale < finalPrescale || finalPrescale == 0) {
                     finalPrescale = currentPrescale;
                     finalThreshold = threshold;
                  }
               }
            }
         }
      }
      /*   if (finalThreshold > 0) {
           cout << "Event triggered with HLT_HT" << finalThreshold << "_v" << vmax << " with prescale: " << finalPrescale
           << endl;
           } else {
           cout << "Event not triggered with any HLT_HTXXX_v" << vmax << " trigger" << endl;
           }*/
   }

   // calculate final weight
   resultWeight *= finalPrescale;
  
   // put weight into the Event
   auto_ptr<double> pOut(new double(resultWeight));
   iEvent.put(pOut, "weight");

}

// ------------ method called once each job just before starting event loop  ------------
void PrescaleWeightProducer::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void PrescaleWeightProducer::endJob() {
}

// ------------ method called at beginning for each run  ---------------------------------
void PrescaleWeightProducer::beginRun(edm::Run& iRun, edm::EventSetup const& iSetup) {

   bool changed(true);
   if (hltConfig_.init(iRun, iSetup, _processName, changed)) {
      if (changed) {
         //         hltConfig_.dump("Streams");
         //         hltConfig_.dump("Datasets");
         //         hltConfig_.dump("PrescaleTable");
         //         hltConfig_.dump("ProcessPSet");
      }
   } else {
      // if init returns FALSE, initialisation has NOT succeeded, which indicates a problem
      // with the file and/or code and needs to be investigated!
      cout << " HLT config extraction failure with process name " << _processName << endl;
      // In this case, all access methods will return empty values!
   }
}

//define this as a plug-in
DEFINE_FWK_MODULE( PrescaleWeightProducer);
