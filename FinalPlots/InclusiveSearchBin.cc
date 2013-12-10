#include "InclusiveSearchBin.h"
#include "SearchBin.h"

#include <cmath>
#include <exception>
#include <iostream>
#include <string>
#include <set>

#include "TString.h"


InclusiveSearchBin::InclusiveSearchBin(const std::string &var, double min, double max)
  : var_(var), min_(min), max_(max) {
  if( !( var_ == "HT" || var_ == "MHT" || var_ == "NJets" ) ) {
    std::cerr << "\n\nERROR in InclusiveSearchBin(): unknown variable '" << var_ << "'" << std::endl;
    throw std::exception();
  }
  if( min > max ) {
    std::cerr << "\n\nERROR in InclusiveSearchBin(): silly range '" << min << "' to '" << max << "'" << std::endl;
    throw std::exception();
  }

  // Set default values
  yield_ = 0.;
  statDn_ = 0.;
  statUp_ = 0.;
  systDn_ = 0.;
  systUp_ = 0.;
}


void InclusiveSearchBin::addSearchBin(const SearchBin* bin) {
  if( bins_.find(bin->idx()) != bins_.end() ) {
    // This search bin has already been added
    std::cerr << "\n\nWARNING when adding SearchBin to InclusiveSearchBin '" << toString() << "'" << std::endl;
    std::cerr << "  SearchBin '" << bin->idx() << "' has already been added. Ignoring.\n" << std::endl;

  } else {
    // Add this search bin
    bins_.insert(bin->idx());

    // Yields are added linearly
    yield_ += bin->yield();

    // The statistical components of the uncertainties are considered
    // uncorrelated, i.e. are added in quadrature
    statDn_ = sqrt( statDn_*statDn_ + bin->statDn()*bin->statDn() );
    statUp_ = sqrt( statUp_*statUp_ + bin->statUp()*bin->statUp() );
    
    // The systematic components of the uncertainties are considered
    // correlated, i.e. are added linearly
    systDn_ += bin->systDn();
    systUp_ += bin->systUp();

  }
}


void InclusiveSearchBin::print() const {
  std::cout << "\nInclusive search bin '" << toString() << "'" << std::endl;
  std::cout << "  Comprises the search bins:" << std::flush;
  for(std::set<unsigned int>::const_iterator it = bins_.begin();
      it != bins_.end(); ++it) {
    std::cout << "  " << *it << std::flush;
  }
  std::cout << std::endl;
  std::cout << "  Yield: " << yield_ << ", -" << statDn_ << " (stat), +" << statUp_ << " (stat), -" << systDn_ << " (syst), +" << systUp_ << " (syst)" << std::endl;
  std::cout << std::endl;
}


std::string InclusiveSearchBin::toString() const {
  TString tstr("");
  tstr += min_;
  tstr += " < ";
  tstr += var_;
  tstr += " < ";
  tstr += max_;

  return tstr.Data();
}

