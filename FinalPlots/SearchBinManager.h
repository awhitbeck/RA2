#ifndef SEARCH_BIN_MANAGER_H
#define SEARCH_BIN_MANAGER_H

#include <map>
#include <string>
#include <vector>

#include "SearchBin.h"
#include "InclusiveSearchBin.h"


// Takes care of search-bin reading and combination
// This is where the magic is hidden...
class SearchBinManager {
public:
  SearchBinManager(const std::string& dir, const std::vector<std::string>& bkgs);
  ~SearchBinManager();

  std::vector<InclusiveSearchBin*> getInclusiveSearchBins(const std::string &var, const std::string& mode, const std::string& bkg) const;
  std::vector<SearchBin*> readSearchBins(const std::string &id) const;


private:
  std::string dir_;
  std::map< std::string, std::vector<SearchBin*> > searchBinsForBkgs_;
};
#endif
