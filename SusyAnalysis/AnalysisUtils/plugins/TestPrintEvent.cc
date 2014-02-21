/* class TestPrintEvent
 *
 * \author Luca Lista, INFN
 */
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "SusyAnalysis/AnalysisUtils/interface/PrintEventGenInfo.h"

class TestPrintEvent : public edm::EDAnalyzer {
public:
  TestPrintEvent( const edm::ParameterSet & );
private:
  void analyze( const edm::Event &, const edm::EventSetup & );
  edm::ESHandle<ParticleDataTable> pdt_;
};

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include <iostream>
#include <sstream> 
#include <algorithm>
using namespace std;
using namespace edm;
using namespace reco;

TestPrintEvent::TestPrintEvent( const ParameterSet & cfg ) {
}

void TestPrintEvent::analyze( const Event & event, const EventSetup & es ) {  

  bool debug =true;

  es.getData( pdt_ );

  Handle<View<Candidate> > particles;
  event.getByLabel( "genParticles", particles );
  
  for( View<Candidate>::const_iterator p = particles->begin();
       p != particles->end(); ++ p ) {

    const ParticleData * pd = pdt_->particle( p->pdgId() );  
    
    if( anautils::select( * p ) && (std::abs(p->pdgId())==24 || std::abs(p->pdgId())==5) ) {

      std::cout << "W momentum vector " << pd->name()  << anautils::printP4( *p )  << std::endl;

      vector<const Candidate *> wdaus = anautils::decayWVec( *p, pdt_, debug);
      std::cout << "inside main wdaus.size:" << wdaus.size() << std::endl;
      for( size_t m = 0; m < wdaus.size(); ++ m ) {
	std::cout << "inside main " << (*wdaus[m]).pdgId() << " " << (*wdaus[m]).status() << " " << pdt_->particle((*wdaus[m]).pdgId())->name() << " pt " << (*wdaus[m]).pt() << std::endl;
      }
      int decayMode = anautils::decayMode( p->pdgId(), wdaus, pdt_, debug);
      std::cout << "decay mode " << decayMode << std::endl;
      std::cout << "-----------------------------------" << std::endl;
    }
  }

}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE( TestPrintEvent );

