#ifndef BACKGROUND_DISTRIBUTION_H
#define BACKGROUND_DISTRIBUTION_H

#include <string>
#include <vector>

#include "TH1.h"

#include "InclusiveSearchBin.h"


class BackgroundDistribution {
public:
  BackgroundDistribution(const std::string &name, const std::string &var, const TH1* hOrig, const std::vector<InclusiveSearchBin*> &inclSearchBins);
  ~BackgroundDistribution();

  void setLegendLabel(const std::string &label) { label_ = label; }
  void setFillColor(int color) { color_ = color; }

  std::string name() const { return name_; }
  std::string legendLabel() const { return label_; }
  int fillColor() const { return color_; }
  std::string var() const { return var_; }
  const TH1* originalDistribution() const { return hOrig_; }
  //  int nBins() const { return static_cast<int>(yields_.size()); }
  // 'bin' from 1 to 'nBins()', i.e. like for TH1
  double yield(int bin) const { return yields_.at(bin-1); }
  double uncertDn(int bin) const { return uncertDn_.at(bin-1); }
  double uncertUp(int bin) const { return uncertUp_.at(bin-1); }


private:
  const std::string name_;
  const std::string var_;
  std::string label_;
  int color_;

  TH1* hOrig_;
  std::vector<double> yields_;
  std::vector<double> uncertDn_;
  std::vector<double> uncertUp_;
  
  void scale(const std::vector<InclusiveSearchBin*> &inclSearchBins);
  void scaleInRange(int firstHistBin, int lastHistBin, const InclusiveSearchBin* inclSearchBin);
  std::string toString() const;
};
#endif
