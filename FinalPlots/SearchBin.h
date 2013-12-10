#ifndef SEARCH_BIN_H
#define SEARCH_BIN_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class SearchBin {
public:
  static std::vector<SearchBin*> createFromTxtFile(const std::string &input);
  static std::vector<SearchBin*> createFromTxtFile2(const std::string &input);

  SearchBin(unsigned int idx, double yield, double statDn, double statUp, double systDn, double systUp)
    : idx_(idx), yield_(yield), statDn_(statDn), statUp_(statUp), systDn_(systDn), systUp_(systUp) {};

  void print() const;

  unsigned int idx() const { return idx_; }
  double yield() const { return yield_; }
  double statDn() const { return statDn_; }
  double statUp() const { return statUp_; }
  double systDn() const { return systDn_; }
  double systUp() const { return systUp_; }


private:
  const unsigned int idx_;
  const double yield_;
  const double statDn_;
  const double statUp_;
  const double systDn_;
  const double systUp_;
};

#endif
