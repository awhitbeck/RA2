#ifndef INCLUSIVE_SEARCH_BIN_H
#define INCLUSIVE_SEARCH_BIN_H

#include <string>
#include <set>

#include "SearchBin.h"

class InclusiveSearchBin {
public:
  InclusiveSearchBin(const std::string &var, double min, double max);

  void addSearchBin(const SearchBin* bin);

  void print() const;
  std::string toString() const;

  std::string var() const { return var_; }
  double min() const { return min_; }
  double max() const { return max_; }
  bool covers(double x) const { return x >= min_ && x <= max_; }

  double yield() const { return yield_; }
  double statDn() const { return statDn_; }
  double statUp() const { return statUp_; }
  double systDn() const { return systDn_; }
  double systUp() const { return systUp_; }


private:
  const std::string var_;
  const double min_;
  const double max_;

  std::set<unsigned int> bins_;
  double yield_;
  double statDn_;
  double statUp_;
  double systDn_;
  double systUp_;
};
#endif
