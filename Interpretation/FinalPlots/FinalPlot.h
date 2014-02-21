#ifndef FINAL_PLOTS_H
#define FINAL_PLOTS_H

#include <string>
#include <vector>

#include "TH1.h"
#include "TGraphAsymmErrors.h"
#include "TLegend.h"
#include "TPaveText.h"

#include "BackgroundDistribution.h"
#include "SignalDistribution.h"
#include "TwoColumnLegend.h"


class FinalPlot {
public:
  FinalPlot(const std::string& mode, const std::string &var, const TH1* hData, bool rebin, bool lastBinIsOverflow, const std::string& outNamePrefix);
  ~FinalPlot();

  void addBackground(BackgroundDistribution* bkg);
  void addSignal(SignalDistribution* sig);
  void setTitle(const std::vector<std::string>& lines);
  void setSubPlotLabel(const std::string& label);
  void draw() const;


private:
  const std::string var_;
  const bool rebin_;
  const bool lastBinIsOverflow_;
  const std::string outName_;

  TH1* hDataOrig_;		// Data histogram
  TH1* hDataDrawn_;	       	// Data histogram with style changes and possibly rebinned
  TH1* hRatioFrame_;
  std::vector<BackgroundDistribution*> bkgs_;
  std::vector<SignalDistribution*> signals_;
  TPaveText* title_;
  TPaveText* subPlotLabel_;

  void createBackgroundStackAndErrorBand(std::vector<TH1*> &stack, TGraphAsymmErrors* &band, TwoColumnLegend* leg) const;
  void createSignalHistograms(std::vector<TH1*> &hists, TwoColumnLegend* leg) const;
  void createRatioPlotAndErrorBand(const TH1* hData, const TH1* hBkg, const TGraphAsymmErrors* band, TH1* &hRatio, TGraphAsymmErrors* &ratioBand) const;
  void createBackgroundHistogramWithErrors(const BackgroundDistribution* bkg, TH1* &hBkg, TH1* &hDn, TH1* &hUp) const;
  void rebin(TH1* &h, const TString &type) const;
  bool logy() const { return true; }
};
#endif
