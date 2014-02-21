// system include files
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

// user include files
#include "SusyAnalysis/AnalysisUtils/interface/PrintEventGenInfo.h"

namespace anautils {

  //https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookGenParticleCandidate
  
  //select the particles with status 3
  bool select( const reco::Candidate & c ) {
    return c.status() == 3;
  }

  bool hasValidDaughters( const reco::Candidate & c ) {
    size_t ndau = c.numberOfDaughters();
    for( size_t i = 0; i < ndau; ++ i )
      if ( select( * c.daughter( i ) ) )
	return true;
    return false;
  }
  
  std::string printP4( const Candidate & c, bool printP4_, bool printPtEtaPhi_, bool printVertex_ ) {
    ostringstream cout;
    if ( printP4_ ) cout << " (" << c.px() << ", " << c.py() << ", " << c.pz() << "; " << c.energy() << ")"; 
    if ( printPtEtaPhi_ ) cout << " [" << c.pt() << ": " << c.eta() << ", " << c.phi() << "]";
    if ( printVertex_ ) cout << " {" << c.vx() << ", " << c.vy() << ", " << c.vz() << "}";
    return cout.str();
  }

  // need to update this one
  void printParticle( const reco::Candidate & c, edm::ESHandle<ParticleDataTable> & pdt_ ){
    
    std::cout << "******** " << std::endl;
    
    ostringstream out;
    char buf[256];
    /*
      snprintf(buf, 256, " idx  |    ID -       Name |Stat|  Mo1  Mo2  Da1  Da2 |nMo nDa|    pt       eta     phi   |     px         py         pz        m     |"); 
      out << buf;
      snprintf(buf, 256, "        vx       vy        vz     |");
      out << buf;
    out << endl;
    */

    int idx  = -1;
    int iMo1 = -1;
    int iMo2 = -1;
    int iDa1 = -1;
    int iDa2 = -1;

    // Particle Name
    int id = c.pdgId();
    const ParticleData * pd = pdt_->particle( id );  
    assert( pd != 0 );
    string particleName = ( pd->name() );

    // Particles Mothers and Daighters
    iMo1 = -1;
    iMo2 = -1;
    iDa1 = -1;
    iDa2 = -1;
    int nMo = c.numberOfMothers();
    int nDa = c.numberOfDaughters();
    
    /*
    found = find(cands.begin(), cands.end(), c.mother(0));
    if(found != cands.end()) iMo1 = found - cands.begin() ;
    
    found = find(cands.begin(), cands.end(), c.mother(nMo-1));
    if(found != cands.end()) iMo2 = found - cands.begin() ;
    
    found = find(cands.begin(), cands.end(), c.daughter(0));
    if(found != cands.end()) iDa1 = found - cands.begin() ;
    
    found = find(cands.begin(), cands.end(), c.daughter(nDa-1));
    if(found != cands.end()) iDa2 = found - cands.begin() ;
    */

    snprintf(buf, 256,
	     " %4d | %5d - %10s | %2d | %4d %4d %4d %4d | %2d %2d | %7.3f %10.3f %6.3f | %10.3f %10.3f %10.3f %8.3f |",
             idx,
             c.pdgId(),
             particleName.c_str(),
             c.status(),
             iMo1,iMo2,iDa1,iDa2,nMo,nDa,
             c.pt(),
             c.eta(),
             c.phi(),
             c.px(),
             c.py(),
             c.pz(),
             c.mass()
	     );
    out << buf;
    
    snprintf(buf, 256, " %10.3f %10.3f %10.3f |",
	     c.vertex().x(),
	     c.vertex().y(),
	     c.vertex().z());
    out << buf;
    out << endl;
    
    cout << out.str();
    
  }

  // check leptonic decay chain of W and returns a vector of daughter particles
  vector<const Candidate *> decayWVec( const Candidate & c, edm::ESHandle<ParticleDataTable> & pdt, bool debug) {

    int id = c.pdgId();
    const ParticleData * pd = pdt->particle( id );  
    assert( pd != 0 );
    
    vector<const Candidate *> wdaus;
    //if( std::abs(c.pdgId())==24 ) wdaus.push_back( &c );
    
    if( std::abs(id)==5 || std::abs(id)==4) {
      if(debug) std::cout << "inside decayWVec " << pdt->particle(c.pdgId())->name() <<" " << c.status() << " b/c quark " << std::endl ;
      //to save b/bbar quark, uncomment next line
      //wdaus.push_back( & c );
      vector<const Candidate *> local;
      decayBquarkVec( c, local, pdt, debug);
      for(size_t ii=0; ii<local.size(); ii++) 
	//to save all daughters, uncomment the following
	//if(local[ii]->status() == 1) wdaus.push_back( local[ii] );
	//save only leptonic daughters for now
	if(local[ii]->status() == 1 && 
	   std::abs(local[ii]->pdgId())>10 && 
	   std::abs(local[ii]->pdgId())<17)
	  wdaus.push_back( local[ii] );
    } else {
      size_t ndau = c.numberOfDaughters();
      if( ndau > 0 ) {
	for( size_t i = 0; i < ndau; ++ i ) {
	  const Candidate * d = c.daughter( i );
	  if ( select( * d ) ) {
	    if( std::abs(d->pdgId())==11 || std::abs(d->pdgId())==12 ) {
	      if(debug) std::cout << "inside decayWVec " 
				  << pdt->particle(d->pdgId())->name() <<" " 
				  << d->status() << " wenu " << std::endl ;
	      wdaus.push_back( d );
	    }
	    else if ( std::abs(d->pdgId())==13 || std::abs(d->pdgId())==14 ) {
	      if(debug) std::cout << "inside decayWVec " 
				  << pdt->particle(d->pdgId())->name() <<" " 
				  << d->status() << " wmunu " << std::endl ;
	      wdaus.push_back( d );
	    } 
	    else if ( std::abs(d->pdgId())==15 || std::abs(d->pdgId())==16) {
	      if(debug) std::cout << "inside decayWVec " 
				  << pdt->particle(d->pdgId())->name() <<" " 
				  << d->status() << " wtaunu " << std::endl ;
	      wdaus.push_back( d );
	      vector<const Candidate *> local;
	      if( std::abs(d->pdgId())==15 ) decayWtaunuVec( * d, local, pdt, debug);
	      for(size_t ii=0; ii<local.size(); ii++) 
		if(local[ii]->status() == 1) wdaus.push_back( local[ii] );
	    }
	  }
	}  
      }
    }

    return wdaus;
  }
  
  // check leptonic decay chain of tau and returns a vector of daughter particles
  void decayWtaunuVec( const Candidate & c, vector<const Candidate *>& taudaus, edm::ESHandle<ParticleDataTable> & pdt, bool debug) {
    
    int id = c.pdgId();
    const ParticleData * pd = pdt->particle( id );  
    assert( pd != 0 );

    if(debug) std::cout << "inside anautil decayWtaunuVec moms " 
			<< pd->name() << " " << c.status() 
			<< " pt " << c.pt() << std::endl;
   
    size_t ndau = c.numberOfDaughters();
    for( size_t i = 0; i < ndau; ++ i ) {
      const Candidate * d = c.daughter( i );
      if(debug) std::cout << "inside anautil decayWtaunuVec daus " 
			  << pdt->particle(d->pdgId())->name()  << " " 
			  << d->status() << " pt " << d->pt()
			  << std::endl;
      
      // add stable daughters to vector to go out
      if( d->status() == 1 ) {
	taudaus.push_back (d);
      }
      decayWtaunuVec( *d, taudaus, pdt, debug) ;
    }
  }
  
  // check daughters of b decay
  void decayBquarkVec( const Candidate & c, vector<const Candidate *>& taudaus, edm::ESHandle<ParticleDataTable> & pdt, bool debug) {
    
    int id = c.pdgId();
    const ParticleData * pd = pdt->particle( id );  
    assert( pd != 0 );
    
    if(debug) std::cout << "inside anautil decayBquarkVec moms " 
			<< pd->name() << " " << c.status() 
			<< " pt " << c.pt() << " ndau " << c.numberOfDaughters()
			<< std::endl;
    size_t ndau = c.numberOfDaughters();
    for( size_t i = 0; i < ndau; ++ i ) {
      const Candidate * d = c.daughter( i );
      if(debug) std::cout << "inside anautil decayBquarkVec daus - all daus " 
			  << pdt->particle(d->pdgId())->name()  << " " 
			  << d->status() << " pt " << d->pt()
			  << " ndau " << d->numberOfDaughters()
			  << std::endl;
    }   

    for( size_t i = 0; i < ndau; ++ i ) {
      const Candidate * d = c.daughter( i );
      if(debug) std::cout << "inside anautil decayBquarkVec daus " 
			  << pdt->particle(d->pdgId())->name()  << " " 
			  << d->status() << " pt " << d->pt()
			  << " ndau " << d->numberOfDaughters()
			  << std::endl;
      
      // add stable daughters to vector to go out
      if( d->status() == 1 && d->numberOfDaughters()==0 ) {
	bool found = false;
	for(size_t idau=0; idau<taudaus.size(); idau++) {
	  if( taudaus[idau] == d ) found = true;
	}
	bool momIsPi0   = false;
	bool momIsGamma = false;
	if( d->numberOfMothers()>0 && d->mother(0)->pdgId()==111) momIsPi0   = true;
	if( d->numberOfMothers()>1 && d->mother(1)->pdgId()==111) momIsPi0   = true;
	if( d->numberOfMothers()>0 && d->mother(0)->pdgId()==22 ) momIsGamma = true;
	if( d->numberOfMothers()>1 && d->mother(1)->pdgId()==22 ) momIsGamma = true;
	if( !found && !momIsPi0 && !momIsGamma ) taudaus.push_back (d);
      }

      /*
	inside anautil decayBquarkVec moms g 2 pt 1.26631 ndau 2
	inside anautil decayBquarkVec daus - all daus g 2 pt 1.26631 ndau 2
	inside anautil decayBquarkVec daus - all daus string 2 pt 99.5473 ndau 73
	inside anautil decayBquarkVec daus g 2 pt 1.26631 ndau 2
	inside anautil decayBquarkVec moms g 2 pt 1.26631 ndau 2
	inside anautil decayBquarkVec daus - all daus g 2 pt 1.26631 ndau 2
	inside anautil decayBquarkVec daus - all daus string 2 pt 99.5473 ndau 73
	inside anautil decayBquarkVec daus g 2 pt 1.26631 ndau 2
	inside anautil decayBquarkVec moms g 2 pt 1.26631 ndau 2
      */
      // to avoid infinite loops like above
      bool sameP = false;
      if( d == (&c) && d->status()>1 && c.status()>1 ) sameP = true ;
      //std::cout << "sameP  " << sameP << std::endl;
      if( d->status()>1 && !sameP) decayBquarkVec( *d, taudaus, pdt, debug) ;
    }

  }

  int decayMode(int moPdgId, vector<const Candidate *> &daus, edm::ESHandle<ParticleDataTable> &pdt, bool debug) {
    if( std::abs(moPdgId)==24 ) {
      if     ( isWenu(daus, pdt, debug) )          return WENU;
      else if( isWmunu(daus, pdt, debug) )         return WMUNU;
      else if( isWtaunu(daus, pdt, debug) ){
	if     ( isWtaunu_munu(daus, pdt, debug) ) return WTAUNU_MUNU;
	else if( isWtaunu_enu (daus, pdt, debug) ) return WTAUNU_ENU;
	else                                       return WTAUNU_HAD;
      } 
      else                                         return HAD_DECAY;
    } else if( std::abs(moPdgId)==5 || std::abs(moPdgId)==4) {
      bool bquark_munu = isBquark_munu(daus, pdt, debug);
      bool bquark_enu  = isBquark_enu (daus, pdt, debug);
      bool bquark_munuenu = (bquark_munu && bquark_enu);
      if      ( bquark_munuenu ) return BQUARK_MUNUENU;
      else if ( bquark_munu )    return BQUARK_MUNU;
      else if ( bquark_enu  )    return BQUARK_ENU;
      else                       return HAD_DECAY;
    }
    else return -1;
  }

  bool isWenu(vector<const Candidate *> &daus, edm::ESHandle<ParticleDataTable> &pdt, bool debug) {
    
    bool iswenu = false;
    for(size_t idau=0; idau<daus.size(); idau++) {
      if(debug) std::cout << "inside isWenu "
			  << (*daus[idau]).pdgId() << " " 
			  << (*daus[idau]).status() << " " 
			  << pdt->particle((*daus[idau]).pdgId())->name() 
			  << " pt " << (*daus[idau]).pt() << std::endl;
      if( (*daus[idau]).status()==3 && 
	  (std::abs((*daus[idau]).pdgId())==11 || 
	   std::abs((*daus[idau]).pdgId())==12) )
	iswenu = true;
    }
    return iswenu;
  }

  bool isWmunu(vector<const Candidate *> &daus, edm::ESHandle<ParticleDataTable> &pdt, bool debug) {
    
    bool iswmunu = false;
    for(size_t idau=0; idau<daus.size(); idau++) {
      if(debug) std::cout << "inside isWmunu "
			  << (*daus[idau]).pdgId() << " " 
			  << (*daus[idau]).status() << " " 
			  << pdt->particle((*daus[idau]).pdgId())->name() 
			  << " pt " << (*daus[idau]).pt() << std::endl;
      if( (*daus[idau]).status()==3 && 
	  (std::abs((*daus[idau]).pdgId())==13 || 
	   std::abs((*daus[idau]).pdgId())==14) )
	iswmunu = true;
    }
    return iswmunu;
  }

  bool isWtaunu(vector<const Candidate *> &daus, edm::ESHandle<ParticleDataTable> &pdt, bool debug) {
    
    bool iswtaunu = false;
    for(size_t idau=0; idau<daus.size(); idau++) {
      if(debug) std::cout << "inside isWtaunu "
			  << (*daus[idau]).pdgId() << " " 
			  << (*daus[idau]).status() << " " 
			  << pdt->particle((*daus[idau]).pdgId())->name() 
			  << " pt " << (*daus[idau]).pt() << std::endl;
      if( (*daus[idau]).status()==3 && 
	  (std::abs((*daus[idau]).pdgId())==15 || 
	   std::abs((*daus[idau]).pdgId())==16) )
	iswtaunu = true;
    }
    return iswtaunu;
  }

  bool isWtaunu_enu(vector<const Candidate *> &daus, edm::ESHandle<ParticleDataTable> &pdt, bool debug) {
    
    bool istaunu = isWtaunu(daus, pdt, debug);

    bool iswtaunu_enu = false;
    if( istaunu ) { 
      for(size_t idau=0; idau<daus.size(); idau++) {
	if(debug) std::cout << "inside isWtaunu_enu "
			    << (*daus[idau]).pdgId() << " " 
			    << (*daus[idau]).status() << " " 
			    << pdt->particle((*daus[idau]).pdgId())->name() 
			    << " pt " << (*daus[idau]).pt() << std::endl;
	if( (*daus[idau]).status()==1 && 
	    (std::abs((*daus[idau]).pdgId())==11 || 
	     std::abs((*daus[idau]).pdgId())==12) )
	  iswtaunu_enu = true;
      }
    }
    return iswtaunu_enu;
  }

  bool isWtaunu_munu(vector<const Candidate *> &daus, edm::ESHandle<ParticleDataTable> &pdt, bool debug) {
    
    bool istaunu = isWtaunu(daus, pdt, debug);
    bool iswtaunu_munu = false;

    if( istaunu ) { 
      for(size_t idau=0; idau<daus.size(); idau++) {
	if(debug) std::cout << "inside isWtaunu_munu "
			    << (*daus[idau]).pdgId() << " " 
			    << (*daus[idau]).status() << " " 
			    << pdt->particle((*daus[idau]).pdgId())->name() 
			    << " pt " << (*daus[idau]).pt() << std::endl;
	if( (*daus[idau]).status()==1 && 
	    (std::abs((*daus[idau]).pdgId())==13 || 
	     std::abs((*daus[idau]).pdgId())==14) )
	  iswtaunu_munu = true;
      }
    }
    return iswtaunu_munu;
  }

  bool isBquark_munu(vector<const Candidate *> &daus, edm::ESHandle<ParticleDataTable> &pdt, bool debug) {
    
    bool isbmunu = false;
    for(size_t idau=0; idau<daus.size(); idau++) {
      if(debug) std::cout << "inside isbmunu "
			  << (*daus[idau]).pdgId() << " " 
			  << (*daus[idau]).status() << " " 
			  << pdt->particle((*daus[idau]).pdgId())->name() 
			  << " pt " << (*daus[idau]).pt() << std::endl;
      if( (*daus[idau]).status()==1 && 
	  (std::abs((*daus[idau]).pdgId())==13 || 
	   std::abs((*daus[idau]).pdgId())==14) )
	isbmunu = true;
    }
    return isbmunu;
  }

  bool isBquark_enu(vector<const Candidate *> &daus, edm::ESHandle<ParticleDataTable> &pdt, bool debug) {
    
    bool isbenu = false;
    for(size_t idau=0; idau<daus.size(); idau++) {
      if(debug) std::cout << "inside isbenu "
			  << (*daus[idau]).pdgId() << " " 
			  << (*daus[idau]).status() << " " 
			  << pdt->particle((*daus[idau]).pdgId())->name() 
			  << " pt " << (*daus[idau]).pt() << std::endl;
      if( (*daus[idau]).status()==1 && 
	  (std::abs((*daus[idau]).pdgId())==11 || 
	   std::abs((*daus[idau]).pdgId())==12) )
	isbenu = true;
    }
    return isbenu;
  }

  void printLHEComment(edm::Handle<LHEEventProduct>& lhe) {
    
    typedef std::vector<std::string>::const_iterator comments_const_iterator;

    comments_const_iterator c_begin = lhe->comments_begin();
    comments_const_iterator c_end   = lhe->comments_end();

    // susy scan specific
    for( LHEEventProduct::comments_const_iterator cit=c_begin; cit!=c_end; ++cit) {
      std::cout << *cit << std::endl;
    }

  }
  
}
