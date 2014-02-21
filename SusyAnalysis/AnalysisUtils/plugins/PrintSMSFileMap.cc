// -*- C++ -*-
//
// Package:    PrintSMSFileMap
// Class:      PrintSMSFileMap
// 
/**\class PrintSMSFileMap PrintSMSFileMap.cc UserCode/PrintSMSFileMap/src/PrintSMSFileMap.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Seema Sharma
//         Created:  Thu Sep 20 12:22:59 CDT 2012
// Open/End file notification adapted from an example from Hongxuan
// 


// system include files
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// LHE Event
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
// to notify opening or closing a file
#include "FWCore/Framework/interface/FileBlock.h"

using namespace std;


// class declaration
class PrintSMSFileMap : public edm::EDAnalyzer {
public:
  explicit PrintSMSFileMap(const edm::ParameterSet&);
  ~PrintSMSFileMap();
  
  typedef std::vector<std::string>::const_iterator
  comments_const_iterator;
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void respondToOpenInputFile(edm::FileBlock const&);
  virtual void respondToCloseInputFile(edm::FileBlock const&);
  
  
  std::string   susyScanTopology_;
  double        susyScanMotherMass_;
  double        susyScanLSPMass_;
  double        susyScanFracLSP_;
  bool          debug_;
  bool          printINFO_;
  std::string   inputFileNameStr, inputFileNamePre;
  bool          initFile;
  double        prexpar, preypar;
  int           nEvtPerPairPars;

  typedef std::multimap< pair<int,int>, pair<std::string, unsigned int> > smsMap;
  typedef std::multimap< pair<int,int>, pair<std::string, unsigned int> >::iterator smsMapItr;
  //typedef std::multimap< pair<int,int>, pair<std::string, unsigned int> >::iterator mapItr2;
  typedef pair    < pair<int,int>, pair<std::string, unsigned int> > smsPair;
  smsMap    fileMap;
  smsMapItr mapItr;

};


PrintSMSFileMap::PrintSMSFileMap(const edm::ParameterSet& iConfig) {

   //now do what ever initialization is needed
  susyScanTopology_   = iConfig.getParameter<std::string> ("SusyScanTopology");
  susyScanMotherMass_ = iConfig.getParameter<double>      ("SusyScanMotherMass");
  susyScanLSPMass_    = iConfig.getParameter<double>      ("SusyScanLSPMass");
  susyScanFracLSP_    = iConfig.getParameter<double>      ("SusyScanFracLSP");
  debug_              = iConfig.getParameter<bool>        ("Debug");
  printINFO_          = iConfig.getParameter<bool>        ("PrintINFO");

  prexpar = -1; 
  preypar = -1; 
  nEvtPerPairPars =0;
  if( debug_ ) std::cout<<"Initial values  prexpar : "<<prexpar<<"  preypar : "<<preypar<<std::endl;
  initFile = false;  
}


PrintSMSFileMap::~PrintSMSFileMap() {

}

void PrintSMSFileMap::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;

  Handle<LHEEventProduct> product;
  iEvent.getByLabel("source", product);
  
  comments_const_iterator c_begin = product->comments_begin();
  comments_const_iterator c_end = product->comments_end();
   
  double mGL  = -10.0;
  double mSQ  = -10.0;
  double mLSP = -10.0; 
  double xCHI = -10.0;

  // susy scan specific
  for( LHEEventProduct::comments_const_iterator cit=c_begin; cit!=c_end; ++cit) {

    if(debug_) std::cout << *cit << std::endl;
    size_t found = (*cit).find("model");
    
    //model T1_0.0_1025.0_525.0
    //model T2_550.0_425.0
    //model TGQ_0.8_900.0_850.0

    if( found != std::string::npos)   {
      if(debug_) std::cout << *cit << std::endl;
      size_t foundLength = (*cit).size();
      found = (*cit).find(susyScanTopology_);

      std::string smaller = (*cit).substr(found+1,foundLength);
      found = smaller.find("_");
      smaller = smaller.substr(found+1,smaller.size());
      std::istringstream iss(smaller);

      if(susyScanTopology_=="T5ZZ" || susyScanTopology_=="T2bW") {
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

      } else if (susyScanTopology_=="T2" || susyScanTopology_=="T2tt" || susyScanTopology_=="T1" || susyScanTopology_=="T1tttt") {
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

  bool foundModel = false;
  if( mGL==susyScanMotherMass_  && mLSP==susyScanLSPMass_ ) {
    if(debug_) {
      std::cout << std::setw(9) << iEvent.id().run()<< ":" << iEvent.luminosityBlock()<<":"<<iEvent.id().event() 		
		<< std::setw(9) <<  susyScanTopology_ << " mother " << mGL << " LSP "<< mLSP << std::endl;
    }
    foundModel = true;
  }

  /*
  mapItr = fileMap.find( pair<int,int>(mGL,mLSP) );
  if( mapItr != fileMap.end() ) {
    (mapItr->second).second += 1;
  } else {
    size_t ii = inputFileNameStr.find("/store");
    std::string fileName = inputFileNameStr;
    if (ii != string::npos) fileName = inputFileNameStr.substr(ii);

    smsPair temp(pair<int,int>(mGL,mLSP), pair<std::string, unsigned int>(fileName,1));
    fileMap.insert( temp );
  }
  */ 
  
  size_t ii = inputFileNameStr.find("/store");
  std::string fileNamePre = inputFileNameStr;
  if (ii != string::npos) fileNamePre = inputFileNameStr.substr(ii);

  bool fileExist = false;
  for(smsMapItr mapItr2 = fileMap.begin(); mapItr2 != fileMap.end(); ++mapItr2){
    if( (mapItr2->second).first == fileNamePre) fileExist = true;
  }
  bool pointExist = false;
  for(smsMapItr mapItr2 = fileMap.begin(); mapItr2 != fileMap.end(); ++mapItr2){
    if( (mapItr2->first).first==mGL && (mapItr2->first).second==mLSP ) pointExist = true;
  }

  mapItr = fileMap.find( pair<int,int>(mGL,mLSP) );
  //if( mapItr != fileMap.end() && fileExist) {
  //(mapItr->second).second += 1;
  if( fileExist && pointExist ) {
    for(smsMapItr mapItr2 = fileMap.begin(); mapItr2 != fileMap.end(); ++mapItr2){
      if( (mapItr2->first).first==mGL && (mapItr2->first).second==mLSP && (mapItr2->second).first == fileNamePre )
	(mapItr2->second).second += 1;
    }
  } else {
    size_t ii = inputFileNameStr.find("/store");
    std::string fileName = inputFileNameStr;
    if (ii != string::npos) fileName = inputFileNameStr.substr(ii);
    smsPair temp(pair<int,int>(mGL,mLSP), pair<std::string, unsigned int>(fileName,1));
    fileMap.insert( temp );
  }

  if(debug_ ) {
    if( prexpar != mGL || preypar != mLSP ){
      if( printINFO_ ){
	if( !(prexpar == -1 && preypar == -1) ){
	  std::cout << "INFO ... RA2SignalScan   end " << "mother " <<  prexpar << " " << "daughter " << preypar << " " << "nEvtPerPairPars " << nEvtPerPairPars << std::endl;
	  std::cout << "INFO ... RA2SignalScan   bgn " << "mother " <<  prexpar << " " << "daughter " << preypar << " " << "nEvtPerPairPars " << 1 << std::endl;
	} else {
	  std::cout << "INFO ... RA2SignalScan   bgn " << "mother " <<  prexpar << " " << "daughter " << preypar << " " << "nEvtPerPairPars " << 1 << " " << inputFileNameStr<<  std::endl;
	}
      }    
      prexpar = mGL; preypar = mLSP; nEvtPerPairPars =1;
    } else {
      nEvtPerPairPars ++; 
    }
  }

  //char buffer[100];
  //int n =sprintf(buffer,"mSugra model with parameters m0=%6.2f m12=%6.2f tanb=%6.2f A0=%6.2f mu=%6.2f\n",m0,m12,tanb,A0,mu);
  //std::cout << buffer ;

  //return foundModel;
}

void PrintSMSFileMap::beginJob() {

}

void PrintSMSFileMap::endJob() {
  std::cout << "=======================end job =====================" << std::endl;
  for(mapItr=fileMap.begin(); mapItr!=fileMap.end(); mapItr++) {
    std::cout << std::setw(7) << "mother" << std::setw(5) << (mapItr->first).first << std::setw(5) << "LSP" << std::setw(5) << (mapItr->first).second
	      << std::setw(9) << "nEvents" << std::setw(8) << (mapItr->second).second << std::setw(5) << "file" << (mapItr->second).first 
	      << std::endl;
  }

}

void PrintSMSFileMap::respondToOpenInputFile(edm::FileBlock const& fb){
  if(debug_) std::cout << "Opening new file : " <<  fb.fileName() << std::endl;
  prexpar = -1; preypar = -1; nEvtPerPairPars =0;
  if(!initFile) inputFileNamePre = std::string("tempFileName");
  else          inputFileNamePre = inputFileNameStr;
  inputFileNameStr = std::string(fb.fileName());
}

void PrintSMSFileMap::respondToCloseInputFile(edm::FileBlock const& fb){
  if(debug_) {
    if( printINFO_ ){
      std::cout << "INFO ... RA2SignalScan   end " << "mother " <<  prexpar << " " << "daughter " << preypar << " " << "nEvtPerPairPars " << nEvtPerPairPars  << std::endl;
    }
  }
}



//define this as a plug-in
DEFINE_FWK_MODULE(PrintSMSFileMap);
