#include <string>
#include "TH1.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TwoColumnLegend.h"

TwoColumnLegend::TwoColumnLegend(const double rowHeight, const unsigned int nRowsLeft, const unsigned int nRowsRight, const double colWidthLeft, const double colWidthRight)
  : nRowsLeft_(nRowsLeft), nEntries_(0) {
  const double center = 1.-gStyle->GetPadRightMargin()-0.04-colWidthRight;
  const double top = gStyle->GetPadTopMargin()+0.025;
  legLeft_ = new TLegend(center-0.02-colWidthLeft,
			 1.-top-nRowsLeft*rowHeight,
			 center-0.02,
			 1.-top   );
  legLeft_->SetFillColor(0);
  legLeft_->SetBorderSize(0);
  legLeft_->SetTextFont(42);

  legRight_ = new TLegend(center,
			  1.-top-nRowsRight*rowHeight,
			  1.-gStyle->GetPadRightMargin()-0.04,
			  1.-top   );
  legRight_->SetFillColor(0);
  legRight_->SetBorderSize(0);
  legRight_->SetTextFont(42);
}


TwoColumnLegend::~TwoColumnLegend() {
  delete legLeft_;
  delete legRight_;
}


void TwoColumnLegend::addEntry(const TH1* h, const std::string& entry, const std::string& style) {
  ++nEntries_;
  TLegend* leg = legLeft_;
  if( nEntries_ > nRowsLeft_ ) leg = legRight_;
  leg->AddEntry(h,entry.c_str(),style.c_str());
}


void TwoColumnLegend::draw(const std::string& option) {
  legLeft_->Draw(option.c_str());
  legRight_->Draw(option.c_str());
}
