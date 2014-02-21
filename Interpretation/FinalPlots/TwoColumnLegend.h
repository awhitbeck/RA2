#ifndef TWO_COLUMN_LEGEND_H
#define TWO_COLUMN_LEGEND_H

#include <string>
#include "TH1.h"
#include "TLegend.h"
#include "TStyle.h"

class TwoColumnLegend {
public:
  TwoColumnLegend(const double rowHeight, const unsigned int nRowsLeft, const unsigned int nRowsRight, const double colWidthLeft, const double colWidthRight);
  ~TwoColumnLegend();

  void addEntry(const TH1* h, const std::string& entry, const std::string& style);
  void draw(const std::string& option);


private:
  unsigned int nRowsLeft_;
  unsigned int nEntries_;
  TLegend* legLeft_;
  TLegend* legRight_;
};
#endif
