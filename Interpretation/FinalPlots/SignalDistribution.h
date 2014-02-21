#ifndef SIGNAL_DISTRIBUTION_H
#define SIGNAL_DISTRIBUTION_H

#include <string>
#include "TH1.h"


class SignalDistribution {
public:
  SignalDistribution(const std::string &name, const std::string &var, const TH1* h);
  ~SignalDistribution();

  void setLegendLabel(const std::string &label) { label_ = label; }
  void setLineColor(int color) { color_ = color; }
  void setLineStyle(int style) { style_ = style; }

  std::string name() const { return name_; }
  std::string legendLabel() const { return label_; }
  int lineColor() const { return color_; }
  int lineStyle() const { return style_; }
  std::string var() const { return var_; }
  TH1* distribution(const std::string& newName) const { return static_cast<TH1*>(hOrig_->Clone(newName.c_str())); }


private:
  const std::string name_;
  const std::string var_;
  std::string label_;
  int color_;
  int style_;

  TH1* hOrig_;
};
#endif
