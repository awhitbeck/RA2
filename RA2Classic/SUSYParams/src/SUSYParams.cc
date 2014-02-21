// -*- C++ -*-
//
// Package:    SUSYParams
// Class:      SUSYParams
// 
/**\class SUSYParams SUSYParams.cc RA2Classic/SUSYParams/src/SUSYParams.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Gheorghe Lungu
//         Created:  Thu Oct  4 10:44:53 CDT 2012
// $Id: SUSYParams.cc,v 1.3 2013/01/24 20:30:14 seema Exp $
//
//


// system include files
#include <memory>
#include <iostream>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

//
// class declaration
//

class SUSYParams : public edm::EDProducer {
   public:
      explicit SUSYParams(const edm::ParameterSet&);
      ~SUSYParams();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
  void GetMSUGRA(const LHEEventProduct& lhep, const GenEventInfoProduct& genProd);
  void GetSMSs(const LHEEventProduct& lhep, const GenEventInfoProduct& genProd);
  int GetProcID(int procID);

      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      // ----------member data ---------------------------
  edm::Handle<GenEventInfoProduct> genProd_h;
  edm::Handle<LHEEventProduct> lheevp;
  std::string _model;
  bool _debug;
  double mzero, mhalf, azero, tanbeta, sgnMu, mMom, mDau, evtProcID;
};


SUSYParams::SUSYParams(const edm::ParameterSet& iConfig)
{
   
  _model =  iConfig.getParameter<std::string>("Model" );
  _debug = iConfig.getParameter< bool > ("Debug");  

  produces<double> ("m0");
  produces<double> ("m12");
  produces<double> ("A0");
  produces<double> ("tanb");
  produces<double> ("sgnMu");
  produces<double> ("mMom");
  produces<double> ("mDau");
  produces<double> ("evtProcID");

  
}


SUSYParams::~SUSYParams()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
SUSYParams::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   iEvent.getByLabel("generator",genProd_h);
   iEvent.getByLabel("source", lheevp);

   if (_model.find("msugra") != std::string::npos) {
     //std::cout<<"msugra"<<std::endl;
     GetMSUGRA(*lheevp,*genProd_h);//---get the mSUGRA parameters
     std::auto_ptr<double> mzero_p(new double(mzero));
     std::auto_ptr<double> mhalf_p(new double(mhalf));
     std::auto_ptr<double> azero_p(new double(azero));
     std::auto_ptr<double> tanbeta_p(new double(tanbeta));
     std::auto_ptr<double> sgnMu_p(new double(sgnMu));

     iEvent.put(mzero_p,"m0");
     iEvent.put(mhalf_p,"m12");
     iEvent.put(azero_p,"A0");
     iEvent.put(tanbeta_p,"tanbeta");
     iEvent.put(sgnMu_p,"sgnMu");
   }
   if (_model.find("T") != std::string::npos) {
     //std::cout<<"SMS"<<std::endl;
     GetSMSs(*lheevp,*genProd_h);//---get the SMS parameters
     std::auto_ptr<double> mMom_p(new double(mMom));
     std::auto_ptr<double> mDau_p(new double(mDau));

     //std::cout<<"(main) "<<mMom<<" "<<*mMom_p<<" "<<mDau<<" "<<*mDau_p<<std::endl;
     //std::cout<<mMom<<" "<<mDau<<std::endl;

     iEvent.put(mMom_p,"m0");
     iEvent.put(mDau_p,"m12");
   }
   std::auto_ptr<double> evtPID_p(new double(evtProcID));
   iEvent.put(evtPID_p,"evtProcID");
   //std::cout<<*evtPID_p<<" "<<evtProcID<<std::endl;
   //std::cout<<evtProcID<<std::endl;


 
}

// ------------ method called once each job just before starting event loop  ------------
void 
SUSYParams::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SUSYParams::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
SUSYParams::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
SUSYParams::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
SUSYParams::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
SUSYParams::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SUSYParams::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
void SUSYParams::GetMSUGRA(const LHEEventProduct& lhep, const GenEventInfoProduct& genProd) {
  typedef std::vector<std::string>::const_iterator comments_const_iterator;
  comments_const_iterator c_begin = lhep.comments_begin();
  comments_const_iterator c_end = lhep.comments_end();
  
  double m0, m12, tanb, a0, mu=1.0;
  double signMu;
  for( comments_const_iterator cit=c_begin; cit!=c_end; ++cit) {
    size_t found = (*cit).find("model");
    if( found != std::string::npos)   {    
      //         std::cout << *cit << std::endl;  
      size_t foundLength = (*cit).size();
      found = (*cit).find("=");
      std::string smaller = (*cit).substr(found+1,foundLength);
      found = smaller.find("_");
      smaller = smaller.substr(found+1,smaller.size());
      //
      std::istringstream iss(smaller);
      iss >> m0;
      iss.clear();
      //
      found = smaller.find("_");
      smaller = smaller.substr(found+1,smaller.size());
      iss.str(smaller);
      iss >> m12;
      iss.clear();
      //
      found = smaller.find("_");
      smaller = smaller.substr(found+1,smaller.size());
      iss.str(smaller);
      iss >> tanb;
      iss.clear();
      //
      found = smaller.find("_");
      smaller = smaller.substr(found+1,smaller.size());
      iss.str(smaller);
      iss >> a0;
      iss.clear();
      found = smaller.find("_");
      smaller = smaller.substr(found+1,smaller.size());
      iss.str(smaller);
      iss >> signMu;
      iss.clear();
      mu *= signMu;
    }
  }
  // char buffer[100];
//   if (_debug) {
//     sprintf(buffer,"mSugra model with parameters m0=%6.2f m12=%6.2f tanb=%6.2f A0=%6.2f mu=%6.2f\n",mzero,mhalf,tanb,azero,mu);
//     std::cout << buffer ;
//   }
  mzero = (float)m0;
  mhalf = (float)m12;
  azero = (float)a0;
  tanbeta = (float)tanb;
  sgnMu = (float)mu;

  //---get procID
  //const HepMC::GenEvent * myGenEvent = hmc.GetEvent();
  //cout<<myGenEvent->signal_process_id()<<endl;
  //

  evtProcID = 0;//---catches all events with procID not listed below
  evtProcID = (double)GetProcID(genProd.signalProcessID());
  //cout<<"(getsusy) "<<genProd.signalProcessID()<<" "<<evtProcID<<endl;
  //---event cross-section
  //cout <<"XS="<<genProd.weight()<<" procID="<<evtProcID<<endl;

}
void SUSYParams::GetSMSs(const LHEEventProduct& lhep, const GenEventInfoProduct& genProd) {
  typedef std::vector<std::string>::const_iterator comments_const_iterator;
  comments_const_iterator c_begin = lhep.comments_begin();
  comments_const_iterator c_end = lhep.comments_end();

  //double Mmom(0), Mdau(0);
  double mGL  = -1.0;
  double mSQ  = -1.0;
  double mLSP = -1.0; 
  double xCHI = -1.0;
  
  for( comments_const_iterator cit=c_begin; cit!=c_end; ++cit) {
    size_t found = (*cit).find("model");
    
    if( found != std::string::npos)   {
      if(_debug) std::cout << *cit << std::endl;
      size_t foundLength = (*cit).size();
      //found = (*cit).find("T5zz");
      found = (*cit).find(_model);
      
      std::string smaller = (*cit).substr(found+1,foundLength);
      found = smaller.find("_");
      smaller = smaller.substr(found+1,smaller.size());
      std::istringstream iss(smaller);
      
      if(_model=="T5ZZ" || _model=="T2bW") {
	iss >> xCHI;
	iss.clear();
	found = smaller.find("_");
	smaller = smaller.substr(found+1,smaller.size());
	iss.str(smaller);
	
	iss >> mGL;
	iss.clear();
	
	found = smaller.find("_");
	smaller = smaller.substr(found+1,smaller.size());
	iss.str(smaller);
	iss >> mLSP;
	iss.clear();

      } else if (_model=="T2" || _model=="T2tt" || _model=="T1" || _model=="T1tttt") {
	iss >> mGL;
	iss.clear();
	
	found = smaller.find("_");
	smaller = smaller.substr(found+1,smaller.size());
	iss.str(smaller);
	iss >> mLSP;
	iss.clear();
      }

    }
  }

  //char buffer[100];
  //int n = 
  //sprintf(buffer,"SMS model with parameters mMom=%6.2f mDau=%6.2f\n",Mmom,Mdau);
  //std::cout << buffer <<std::endl;;
  
  mMom = (float)mGL;
  mDau = (float)mLSP;
  
  //---get procID
  //const HepMC::GenEvent * myGenEvent = hmc.GetEvent();
  //cout<<myGenEvent->signal_process_id()<<endl;
  //

  evtProcID = 0;//---catches all events with procID not listed below
  evtProcID = (double)GetProcID(genProd.signalProcessID());

  if (_debug) std::cout<<"(getSMS) : mMom "<<mGL<<" "<<mMom<<" mDau "<<mLSP<<" "<<mDau<<" evtProcID "<<evtProcID<<std::endl;
 
  //cout<<"(getsms) "<<genProd.signalProcessID()<<" "<<evtProcID<<endl;
  //---event cross-section
  //cout <<"XS="<<genProd.weight()<<" procID="<<evtProcID<<endl;

}
int SUSYParams::GetProcID(int procID){
  
  int susy_proc_labelID = 0;

  if (procID<=214 && procID>=201) susy_proc_labelID = 4;//---ll
  if (procID<=236 && procID>=216) susy_proc_labelID = 3;//---nn
  if (procID<=242 && procID>=237) susy_proc_labelID = 1;//---ng
  if (procID<=244 && procID>=243) susy_proc_labelID = 9;//---gg
  if (procID<=256 && procID>=246) susy_proc_labelID = 2;//---ns
  if (procID<=259 && procID>=258) susy_proc_labelID = 10;//---sg
  if (procID<=265 && procID>=261) susy_proc_labelID = 7;//---tb
  if (procID<=273 && procID>=271) susy_proc_labelID = 6;//---ss
  if (procID<=280 && procID>=274) susy_proc_labelID = 5;//---sb
  if (procID<=283 && procID>=281) susy_proc_labelID = 6;//---ss
  if (procID<=286 && procID>=284) susy_proc_labelID = 5;//---sb
  if (procID<=290 && procID>=287) susy_proc_labelID = 8;//---bb
  if (procID<=293 && procID>=291) susy_proc_labelID = 6;//---ss
  if (procID<=295 && procID>=294) susy_proc_labelID = 11;//---bg
  if (procID==296) susy_proc_labelID = 8;//---bb

  //---so far 12 possible labels 
  return susy_proc_labelID;
}
//define this as a plug-in
DEFINE_FWK_MODULE(SUSYParams);
