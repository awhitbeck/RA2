#ifndef SEARCH_BIN_MANAGER_H
#define SEARCH_BIN_MANAGER_H

#include <map>
#include <string>
#include <vector>

#include "SearchBin.h"
#include "InclusiveSearchBin.h"


// Takes care of search-bin reading and combination
class SearchBinManager {
public:
  SearchBinManager(const std::string& dir, const std::vector<std::string>& bkgs);
  ~SearchBinManager();

  std::vector<InclusiveSearchBin*> getInclusiveSearchBins(const std::string &var, const std::string& mode, const std::string& bkg) const;


private:
  std::string dir_;
  std::map< std::string, std::vector<SearchBin*> > searchBinsForBkgs_;

  std::vector<SearchBin*> readSearchBins(const std::string &id) const;
};


SearchBinManager::SearchBinManager(const std::string& dir, const std::vector<std::string>& bkgs)
  : dir_(dir) {
  
  for(std::vector<std::string>::const_iterator bkgIt = bkgs.begin();
      bkgIt != bkgs.end(); ++bkgIt) {
    const std::string bkg = *bkgIt;
    std::cout << "\n\nReading " << bkg << " background predictions in search bins" << std::endl;
    std::vector<SearchBin*> searchBins = readSearchBins(bkg);
    for(std::vector<SearchBin*>::const_iterator it = searchBins.begin();
	it != searchBins.end(); ++it) {
      (*it)->print();
    }
    searchBinsForBkgs_[bkg] = searchBins;
  }
}

SearchBinManager::~SearchBinManager() {
  for(std::map< std::string, std::vector<SearchBin*> >::iterator mit = searchBinsForBkgs_.begin();
      mit != searchBinsForBkgs_.end(); ++mit) {
    for(std::vector<SearchBin*>::iterator sit = mit->second.begin();
	sit != mit->second.end(); ++sit) {
      delete *sit;
    }
  }
}


std::vector<SearchBin*> SearchBinManager::readSearchBins(const std::string &id) const {
  std::string fileName(dir_+"/");
  if(      id == "QCD"        ) fileName += "qcd_searchBinPredictions.txt";
  else if( id == "LostLepton" ) fileName += "lostLepton_searchBinPredictions.txt";
  else if( id == "HadTau"     ) fileName += "tau_searchBinPredictions.txt";
  else if( id == "ZInv"       ) fileName += "zinvis_searchBinPredictions.txt";
  else {
    std::cerr << "\n\nERROR when reading '" << id << "' search-bin yields" << std::endl;
    std::cerr << "  Unknown id '" << id << "'" << std::endl;
    throw std::exception();
  }
  
  return SearchBin::createFromTxtFile(fileName);
}


std::vector<InclusiveSearchBin*> SearchBinManager::getInclusiveSearchBins(const std::string &var, const std::string& mode, const std::string& bkg) const {

  // Retrieve individual search bins for background
  std::map< std::string, std::vector<SearchBin*> >::const_iterator mit = searchBinsForBkgs_.find(bkg);
  if( mit == searchBinsForBkgs_.end() ) {
    std::cerr << "\n\nERROR when combining search bins" << std::endl;
    std::cerr << "  Unknown background '" << bkg << "'" << std::endl;
    throw std::exception();
  }
  const std::vector<SearchBin*> searchBins = mit->second;


  std::vector<InclusiveSearchBin*> inclBins;

  if( mode == "NJetsInclusive" ) {
    if( var == "HT" ) {
      // 500 < HT < 800
      inclBins.push_back(new InclusiveSearchBin(var,500.,800.));
      inclBins.back()->addSearchBin(searchBins.at(0));
      inclBins.back()->addSearchBin(searchBins.at(1));
      inclBins.back()->addSearchBin(searchBins.at(2));
      inclBins.back()->addSearchBin(searchBins.at(3));
      inclBins.back()->addSearchBin(searchBins.at(17));
      inclBins.back()->addSearchBin(searchBins.at(18));
      inclBins.back()->addSearchBin(searchBins.at(19));
      inclBins.back()->addSearchBin(searchBins.at(31));
    
      // 800 < HT < 1000
      inclBins.push_back(new InclusiveSearchBin(var,800.,1000.));
      inclBins.back()->addSearchBin(searchBins.at(4));
      inclBins.back()->addSearchBin(searchBins.at(5));
      inclBins.back()->addSearchBin(searchBins.at(6));
      inclBins.back()->addSearchBin(searchBins.at(7));
      inclBins.back()->addSearchBin(searchBins.at(20));
      inclBins.back()->addSearchBin(searchBins.at(21));
      inclBins.back()->addSearchBin(searchBins.at(22));
      inclBins.back()->addSearchBin(searchBins.at(32));
    
      // 1000 < HT < 1250
      inclBins.push_back(new InclusiveSearchBin(var,1000.,1250.));
      inclBins.back()->addSearchBin(searchBins.at(8));
      inclBins.back()->addSearchBin(searchBins.at(9));
      inclBins.back()->addSearchBin(searchBins.at(10));
      inclBins.back()->addSearchBin(searchBins.at(11));
      inclBins.back()->addSearchBin(searchBins.at(23));
      inclBins.back()->addSearchBin(searchBins.at(24));
      inclBins.back()->addSearchBin(searchBins.at(25));
      inclBins.back()->addSearchBin(searchBins.at(33));

      // 1250 < HT < 1500
      inclBins.push_back(new InclusiveSearchBin(var,1250.,1500.));
      inclBins.back()->addSearchBin(searchBins.at(12));
      inclBins.back()->addSearchBin(searchBins.at(13));
      inclBins.back()->addSearchBin(searchBins.at(14));
      inclBins.back()->addSearchBin(searchBins.at(26));
      inclBins.back()->addSearchBin(searchBins.at(27));
      inclBins.back()->addSearchBin(searchBins.at(28));
      inclBins.back()->addSearchBin(searchBins.at(34));

      // HT > 1500
      inclBins.push_back(new InclusiveSearchBin(var,1500.,99999.));
      inclBins.back()->addSearchBin(searchBins.at(15));
      inclBins.back()->addSearchBin(searchBins.at(16));
      inclBins.back()->addSearchBin(searchBins.at(29));
      inclBins.back()->addSearchBin(searchBins.at(30));
      inclBins.back()->addSearchBin(searchBins.at(35));

    } else   if( var == "MHT" ) {
      // 200 < MHT < 300
      inclBins.push_back(new InclusiveSearchBin(var,200.,300.));
      inclBins.back()->addSearchBin(searchBins.at(0));
      inclBins.back()->addSearchBin(searchBins.at(4));
      inclBins.back()->addSearchBin(searchBins.at(8));
      inclBins.back()->addSearchBin(searchBins.at(12));
      inclBins.back()->addSearchBin(searchBins.at(15));
      inclBins.back()->addSearchBin(searchBins.at(17));
      inclBins.back()->addSearchBin(searchBins.at(20));
      inclBins.back()->addSearchBin(searchBins.at(23));
      inclBins.back()->addSearchBin(searchBins.at(26));
      inclBins.back()->addSearchBin(searchBins.at(29));
      inclBins.back()->addSearchBin(searchBins.at(31));
      inclBins.back()->addSearchBin(searchBins.at(32));
      inclBins.back()->addSearchBin(searchBins.at(33));
      inclBins.back()->addSearchBin(searchBins.at(34));
      inclBins.back()->addSearchBin(searchBins.at(35));

      // 300 < MHT < 450
      inclBins.push_back(new InclusiveSearchBin(var,300.,450.));
      inclBins.back()->addSearchBin(searchBins.at(1));
      inclBins.back()->addSearchBin(searchBins.at(5));
      inclBins.back()->addSearchBin(searchBins.at(9));
      inclBins.back()->addSearchBin(searchBins.at(13));
      inclBins.back()->addSearchBin(searchBins.at(16));
      inclBins.back()->addSearchBin(searchBins.at(18));
      inclBins.back()->addSearchBin(searchBins.at(21));
      inclBins.back()->addSearchBin(searchBins.at(24));
      inclBins.back()->addSearchBin(searchBins.at(27));
      inclBins.back()->addSearchBin(searchBins.at(30));

      // inclBins.back()->addSearchBin(searchBins.at(31));
      // inclBins.back()->addSearchBin(searchBins.at(32));
      // inclBins.back()->addSearchBin(searchBins.at(33));
      // inclBins.back()->addSearchBin(searchBins.at(34));
      // inclBins.back()->addSearchBin(searchBins.at(35));


      // 450 < MHT < 600
      inclBins.push_back(new InclusiveSearchBin(var,450.,600.));
      inclBins.back()->addSearchBin(searchBins.at(2));
      inclBins.back()->addSearchBin(searchBins.at(6));
      inclBins.back()->addSearchBin(searchBins.at(10));
      inclBins.back()->addSearchBin(searchBins.at(14));
      inclBins.back()->addSearchBin(searchBins.at(19));
      inclBins.back()->addSearchBin(searchBins.at(22));
      inclBins.back()->addSearchBin(searchBins.at(25));
      inclBins.back()->addSearchBin(searchBins.at(28));

      // inclBins.back()->addSearchBin(searchBins.at(16));
      // inclBins.back()->addSearchBin(searchBins.at(30));

      // MHT > 600
      inclBins.push_back(new InclusiveSearchBin(var,600.,9999.));
      inclBins.back()->addSearchBin(searchBins.at(3));
      inclBins.back()->addSearchBin(searchBins.at(7));
      inclBins.back()->addSearchBin(searchBins.at(11));

      // inclBins.back()->addSearchBin(searchBins.at(14));
      // inclBins.back()->addSearchBin(searchBins.at(19));
      // inclBins.back()->addSearchBin(searchBins.at(22));
      // inclBins.back()->addSearchBin(searchBins.at(25));
      // inclBins.back()->addSearchBin(searchBins.at(28));

    } else  if( var == "NJets" ) {
      // 3 <= NJets <= 5
      inclBins.push_back(new InclusiveSearchBin(var,3,5));
      for(int i = 0; i <= 16; ++i) {
	inclBins.back()->addSearchBin(searchBins.at(i));
      }
    
      // 6 <= NJets <= 7
      inclBins.push_back(new InclusiveSearchBin(var,6,7));
      for(int i = 17; i <= 30; ++i) {
	inclBins.back()->addSearchBin(searchBins.at(i));
      }
    
      // NJets >= 8
      inclBins.push_back(new InclusiveSearchBin(var,8,99999));
      for(int i = 31; i <= 35; ++i) {
	inclBins.back()->addSearchBin(searchBins.at(i));
      }

    } else {
      std::cerr << "\n\nERROR when creating inclusive search bins" << std::endl;
      std::cerr << "  Unknown variable '" << var << "'" << std::endl;
      throw std::exception();
    }
  } else {
    std::cerr << "\n\nERROR when combining search bins" << std::endl;
    std::cerr << "  Unknown mode '" << mode << "'" << std::endl;
    throw std::exception();
  }

  return inclBins;
}

#endif
