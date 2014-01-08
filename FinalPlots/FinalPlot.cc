#include <cmath>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TLegend.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TStyle.h"

#include "FinalPlot.h"
#include "BackgroundDistribution.h"
#include "SignalDistribution.h"
#include "Style.h"
#include "TwoColumnLegend.h"


FinalPlot::FinalPlot(const std::string& mode, const std::string &var, const TH1* hData, bool rebin, bool lastBinIsOverflow, const std::string& outNamePrefix)
  : var_(var), rebin_(rebin), lastBinIsOverflow_(lastBinIsOverflow), outName_(outNamePrefix+"_"+var) {

  hDataOrig_ = static_cast<TH1*>(hData->Clone((var_+"_DataOrig").c_str()));
  hDataDrawn_ = static_cast<TH1*>(hData->Clone((var_+"_Data").c_str()));
  if( rebin_ ) this->rebin(hDataDrawn_,"histogram");
  hDataDrawn_->UseCurrentStyle();
  hDataDrawn_->SetMarkerStyle(20);
  hDataDrawn_->SetMarkerSize(Style::markerSize());
  hDataDrawn_->SetLineWidth(2);
  hDataDrawn_->GetYaxis()->SetTitle("Events");
  if(      var_ == "HT"    ) {
    if(        mode == "NJets6-7" ) {
      hDataDrawn_->GetYaxis()->SetRangeUser(3E-1,2E4);
    } else if( mode == "NJets8-Inf" ) {
      hDataDrawn_->GetYaxis()->SetRangeUser(3E-1,6E3);
    } else {
      hDataDrawn_->GetYaxis()->SetRangeUser(3E-1,2E5);
    }
  } else if( var_ == "MHT"   ) {
    if(        mode == "NJets6-7" ) {
      hDataDrawn_->GetYaxis()->SetRangeUser(3E-1,3E4);
    } else if( mode == "NJets8-Inf" ) {
      hDataDrawn_->GetYaxis()->SetRangeUser(3E-1,6E2);
    } else {
      hDataDrawn_->GetYaxis()->SetRangeUser(3E-1,3E5);
    }
  } else if( var_ == "NJets" ) {
    hDataDrawn_->GetYaxis()->SetRangeUser(3E-1,5E7);
  }
  if( var_ == "NJets" ) hDataDrawn_->GetXaxis()->SetNdivisions(205);
  else                  hDataDrawn_->GetXaxis()->SetNdivisions(505);
  hDataDrawn_->GetXaxis()->SetLabelSize(0);
  hDataDrawn_->GetXaxis()->SetTitle("");
  hDataDrawn_->GetYaxis()->SetTickLength(gStyle->GetTickLength("Y")/0.8);

  hRatioFrame_ = static_cast<TH1*>( hDataDrawn_->Clone("hRatioFrame") );
  for(int xBin = 1; xBin <= hRatioFrame_->GetNbinsX(); ++xBin) {
    hRatioFrame_->SetBinContent(xBin,0.);
  }
  hRatioFrame_->SetFillColor(0);
  hRatioFrame_->SetLineColor(kBlack);
  hRatioFrame_->SetLineWidth(1);
  hRatioFrame_->GetXaxis()->SetLabelSize(gStyle->GetLabelSize("X"));
  if(      var_ == "HT"    ) hRatioFrame_->GetXaxis()->SetTitle("H_{T} [GeV]");
  else if( var_ == "MHT"   ) hRatioFrame_->GetXaxis()->SetTitle("#slash{H}_{T} [GeV]");
  else if( var_ == "NJets" ) hRatioFrame_->GetXaxis()->SetTitle("N_{jets}");
  hRatioFrame_->GetYaxis()->SetRangeUser(-0.99,0.99);
  hRatioFrame_->GetYaxis()->SetTitle("(Data-Pred)/Pred");
  hRatioFrame_->GetYaxis()->SetNdivisions(205);
  hRatioFrame_->GetYaxis()->SetTickLength(gStyle->GetTickLength("Y")/0.2);
  hRatioFrame_->GetYaxis()->CenterTitle();
  hRatioFrame_->GetYaxis()->SetTitleOffset(0.9*hRatioFrame_->GetYaxis()->GetTitleOffset());

  title_ = new TPaveText();
}


FinalPlot::~FinalPlot() {
  delete hDataOrig_;
  delete hDataDrawn_;
  delete hRatioFrame_;
  for(std::vector<BackgroundDistribution*>::iterator bkgIt = bkgs_.begin();
      bkgIt != bkgs_.end(); ++bkgIt) {
    delete *bkgIt;
  }
  for(std::vector<SignalDistribution*>::iterator it = signals_.begin();
      it != signals_.end(); ++it) {
    delete *it;
  }
  delete title_;
}


void FinalPlot::addBackground(BackgroundDistribution* bkg) {
  // Sanity checks
  if( bkg->var() != var_ ) {
    std::cerr << "\n\nERROR adding '" << bkg->name() << "' background to '" << var_ << "' FinalPlot" << std::endl;
    std::cerr << "  Background object is '" << bkg->var() << "' distribution" << std::endl;
    throw std::exception();
  }
  const TH1* hBkgOrig = bkg->originalDistribution();
  if( hBkgOrig->GetNbinsX() != hDataOrig_->GetNbinsX() ||
      hBkgOrig->GetXaxis()->GetBinLowEdge(1) != hDataOrig_->GetXaxis()->GetBinLowEdge(1) ||
      hBkgOrig->GetXaxis()->GetBinUpEdge(hBkgOrig->GetNbinsX()) != hDataOrig_->GetXaxis()->GetBinUpEdge(hDataOrig_->GetNbinsX()) ) {
    std::cerr << "\n\nERROR adding '" << bkg->name() << "' background to '" << var_ << "' FinalPlot" << std::endl;
    std::cerr << "  Binning is different than in data histogram." << std::endl;
    throw std::exception();
  }

  // Add background
  bkgs_.push_back(bkg);
}


void FinalPlot::addSignal(SignalDistribution* sig) {
  // Sanity checks
  if( sig->var() != var_ ) {
    std::cerr << "\n\nERROR adding '" << sig->name() << "' signal to '" << var_ << "' FinalPlot" << std::endl;
    std::cerr << "  Signal object is '" << sig->var() << "' distribution" << std::endl;
    throw std::exception();
  }
  const TH1* hSig = sig->distribution("tmp");
  if( hSig->GetNbinsX() != hDataOrig_->GetNbinsX() ||
      hSig->GetXaxis()->GetBinLowEdge(1) != hDataOrig_->GetXaxis()->GetBinLowEdge(1) ||
      hSig->GetXaxis()->GetBinUpEdge(hSig->GetNbinsX()) != hDataOrig_->GetXaxis()->GetBinUpEdge(hDataOrig_->GetNbinsX()) ) {
    std::cerr << "\n\nERROR adding '" << hSig->GetName() << "' signal to '" << var_ << "' FinalPlot" << std::endl;
    std::cerr << "  Binning is different than in data histogram." << std::endl;
    throw std::exception();
  }
  delete hSig;

  signals_.push_back(sig);
}


void FinalPlot::setTitle(const std::vector<std::string>& lines) {
  // Create histogram title
  delete title_;
  title_ = new TPaveText(gStyle->GetPadLeftMargin(),
			 1.-gStyle->GetPadTopMargin()+0.01,
			 1.-gStyle->GetPadRightMargin(),
			 1.,"NDC");
  title_->SetBorderSize(0);
  title_->SetFillColor(0);
  title_->SetTextFont(42);
  title_->SetTextAlign(12);
  title_->SetTextSize(0.039);
  title_->SetMargin(0.);
  for(std::vector<std::string>::const_iterator it = lines.begin();
      it != lines.end(); ++it) {
    title_->AddText(it->c_str());
  }
}


void FinalPlot::draw() const {
  // Create legend
  const unsigned int nLegEntries = 1 + bkgs_.size() + signals_.size();
  TwoColumnLegend* leg = new TwoColumnLegend(0.045,3,nLegEntries-3,0.31,0.31);
  leg->addEntry(hDataDrawn_,"Data","P");

  // Create background stack and error band
  std::vector<TH1*> stack;
  TGraphAsymmErrors* errorBand = NULL;
  createBackgroundStackAndErrorBand(stack,errorBand,leg);

  // Create signal histograms
  std::vector<TH1*> sigs;
  createSignalHistograms(sigs,leg);

  // Create ratios
  TH1* hRatio = NULL;
  TGraphAsymmErrors* ratioErrorBand = NULL;
  createRatioPlotAndErrorBand(hDataDrawn_,stack.back(),errorBand,hRatio,ratioErrorBand);

  // Plot
  TCanvas* can = new TCanvas(("can_"+var_).c_str(),("can_"+var_).c_str(),700,700);
  can->SetBottomMargin(0.2 + 0.8*can->GetBottomMargin()-0.2*can->GetTopMargin());
  can->cd();
  hDataDrawn_->Draw("PE");
  for(std::vector<TH1*>::const_reverse_iterator rit = stack.rbegin();
      rit != stack.rend(); ++rit) {
    (*rit)->Draw("HISTsame");
  }
  errorBand->Draw("E2same");
  for(std::vector<TH1*>::const_iterator it = sigs.begin();
      it != sigs.end(); ++it) {
    (*it)->Draw("Hsame");
  }
  hDataDrawn_->Draw("PEsame");
  leg->draw("same");
  title_->Draw("same");
  gPad->RedrawAxis();
  if( logy() ) can->SetLogy();

  TPad *ratioPad = new TPad("BottomPad","",0,0,1,1);
  ratioPad->SetTopMargin(0.8 - 0.8*ratioPad->GetBottomMargin()+0.2*ratioPad->GetTopMargin());
  ratioPad->SetFillStyle(0);
  ratioPad->SetFrameFillColor(10);
  ratioPad->SetFrameBorderMode(0);
  ratioPad->Draw();
  ratioPad->cd();
  hRatioFrame_->Draw("HIST");
  ratioErrorBand->Draw("E2same");
  hRatio->Draw("PEsame");

  gPad->RedrawAxis();

  can->SaveAs((outName_+".pdf").c_str());
  can->SaveAs((outName_+".png").c_str());
  can->SaveAs((outName_+".C").c_str());

  // Clean up
  for(std::vector<TH1*>::iterator it = stack.begin();
      it != stack.end(); ++it) {
    delete *it;
  }
  for(std::vector<TH1*>::iterator it = sigs.begin();
      it != sigs.end(); ++it) {
    delete *it;
  }
  delete errorBand;
  delete hRatio;
  delete ratioErrorBand;
  delete leg;
  delete can;
}


void FinalPlot::createBackgroundStackAndErrorBand(std::vector<TH1*> &stack, TGraphAsymmErrors* &band, TwoColumnLegend* leg) const {
  // Create background stack and store total error
  std::vector<double> errDn;
  std::vector<double> errUp;
  // Loop over backgrounds: stack histograms and
  // get quadratic sum of errors
  for(std::vector<BackgroundDistribution*>::const_iterator bkgIt = bkgs_.begin();
      bkgIt != bkgs_.end(); ++bkgIt) {
    const BackgroundDistribution* bkg = *bkgIt;

    // background distribution; will perform potential
    // rebinning of original histograms
    TH1* hBkg = NULL;
    TH1* hDn = NULL;
    TH1* hUp = NULL;
    createBackgroundHistogramWithErrors(bkg,hBkg,hDn,hUp);
    
    // add error from this background to total error in quadrature
    if( errDn.size() == 0 ) errDn = std::vector<double>(hDn->GetNbinsX(),0.);
    if( errUp.size() == 0 ) errUp = std::vector<double>(hUp->GetNbinsX(),0.);
    for(int bin = 1; bin <= hDn->GetNbinsX(); ++bin) {
      errDn.at(bin-1) += hDn->GetBinContent(bin)*hDn->GetBinContent(bin);
      errUp.at(bin-1) += hUp->GetBinContent(bin)*hUp->GetBinContent(bin);
    }
    delete hDn;
    delete hUp;

    // if not the first one, add previous to this one (stack)
    if( stack.size() > 0 ) {
      hBkg->Add(stack.back());
    }
    stack.push_back(hBkg);
  }
  for(unsigned int i = 0; i < errDn.size(); ++i) {
    errDn.at(i) = sqrt( errDn.at(i) );
    errUp.at(i) = sqrt( errUp.at(i) );
  }


  // Error band
  const TH1* hTot = stack.back();
  std::vector<double> x(hTot->GetNbinsX(),0.);
  std::vector<double> xe(hTot->GetNbinsX(),0.);
  std::vector<double> y(hTot->GetNbinsX(),0.);
  for(int bin = 1; bin <= hTot->GetNbinsX(); ++bin) {
    x.at(bin-1) = hTot->GetBinCenter(bin);
    xe.at(bin-1) = 0.5 * hTot->GetBinWidth(bin);
    y.at(bin-1) = hTot->GetBinContent(bin);
  }
  band = new TGraphAsymmErrors( x.size(),&(x.front()),&(y.front()),
			        &(xe.front()),&(xe.front()),
			        &(errDn.front()),&(errUp.front())   );
  band->SetFillStyle(3004);
  band->SetLineColor(kBlue);
  band->SetLineWidth(2);

  // Legend
  std::vector<TH1*>::const_reverse_iterator hIt = stack.rbegin();
  std::vector<BackgroundDistribution*>::const_reverse_iterator bkgIt = bkgs_.rbegin();
  for(; bkgIt != bkgs_.rend(); ++bkgIt, ++hIt) {
    leg->addEntry(*hIt,((*bkgIt)->legendLabel()).c_str(),"F");
  }
}


void FinalPlot::createRatioPlotAndErrorBand(const TH1* hData, const TH1* hBkg, const TGraphAsymmErrors* band, TH1* &hRatio, TGraphAsymmErrors* &ratioBand) const {
  hRatio = static_cast<TH1*>(hData->Clone("Ratio"));
  for(int bin = 1; bin <= hRatio->GetNbinsX(); ++bin) {
    const double bkg = hBkg->GetBinContent(bin);
    if( bkg > 0. ) {
      hRatio->SetBinContent(bin, hRatio->GetBinContent(bin) / bkg - 1. );
      hRatio->SetBinError(bin, hRatio->GetBinError(bin) / bkg );
    } else {
      hRatio->SetBinContent(bin,-1.);
      hRatio->SetBinError(bin,0.);
    }
  }

  ratioBand = static_cast<TGraphAsymmErrors*>(band->Clone());
  for(int i = 0; i < ratioBand->GetN(); ++i) {
    const double bkg = hBkg->GetBinContent(i+1);
    if( bkg > 0. ) {
      ratioBand->GetY()[i] = ratioBand->GetY()[i] / bkg - 1.;
      ratioBand->GetEYlow()[i] /= bkg;
      ratioBand->GetEYhigh()[i] /= bkg;      
    } else {
      ratioBand->GetY()[i] = -1.;
      ratioBand->GetEYlow()[i] = 0.;
      ratioBand->GetEYhigh()[i] = 0.;      
    }      
  }
}


void FinalPlot::createBackgroundHistogramWithErrors(const BackgroundDistribution* bkg, TH1* &hBkg, TH1* &hDn, TH1* &hUp) const {
  
  hBkg = static_cast<TH1*>(hDataOrig_->Clone((var_+"_"+bkg->name()).c_str()));
  hBkg->Reset();
  hDn = static_cast<TH1*>(hBkg->Clone((var_+"_"+bkg->name()+"_Dn").c_str()));
  hUp = static_cast<TH1*>(hBkg->Clone((var_+"_"+bkg->name()+"_Up").c_str()));
  for(int bin = 1; bin <= hBkg->GetNbinsX(); ++bin) {
    hBkg->SetBinContent(bin,bkg->yield(bin));
    hBkg->SetBinError(bin,0.);
    hDn->SetBinContent(bin,bkg->uncertDn(bin));
    hDn->SetBinError(bin,0.);
    hUp->SetBinContent(bin,bkg->uncertUp(bin));
    hUp->SetBinError(bin,0.);
  }
  if( rebin_ ) {
    rebin(hBkg,"histogram");
    rebin(hDn,"error");
    rebin(hUp,"error");
  }
  hBkg->UseCurrentStyle();
  hBkg->SetLineColor(bkg->fillColor());
  hBkg->SetFillColor(bkg->fillColor());
  hBkg->SetMarkerStyle(1);
}


void FinalPlot::createSignalHistograms(std::vector<TH1*> &hists, TwoColumnLegend* leg) const {
  // Loop over signals
  for(std::vector<SignalDistribution*>::const_iterator sigIt = signals_.begin();
      sigIt != signals_.end(); ++sigIt) {
    const SignalDistribution* sig = *sigIt;

    TString name( sig->name() );
    name += "_Signal";
    name += signals_.size();
    TH1* h = sig->distribution( std::string(name.Data()) );
    for(int bin = 1; bin <= h->GetNbinsX(); ++bin) {
      h->SetBinError(bin,0.);
    }
    if( rebin_ ) this->rebin(h,"histogram");
    h->SetLineColor(sig->lineColor());

    hists.push_back( h );
    leg->addEntry(h,(sig->legendLabel()).c_str(),"L");
  }
}


// Merge two adjacent bins, starting with bin 1
// type either "histogram" or "error"; the latter will be added
// in quadrature
void FinalPlot::rebin(TH1* &h, const TString &type) const {
  const int nBinsInRange = lastBinIsOverflow_ ? h->GetNbinsX()-1 : h->GetNbinsX();
  const int nBinsRebinned = nBinsInRange%2 == 0 ? nBinsInRange : nBinsInRange-1;
  const int nBinsNew = lastBinIsOverflow_ ? nBinsRebinned/2 + 1 : nBinsRebinned/2;
  const double min = h->GetXaxis()->GetBinLowEdge(1);
  const double max = lastBinIsOverflow_ ? h->GetXaxis()->GetBinUpEdge(nBinsRebinned) + 2.*h->GetXaxis()->GetBinWidth(nBinsRebinned) : h->GetXaxis()->GetBinUpEdge(nBinsRebinned);
  const std::string name(h->GetName());
  TH1* hNew = new TH1D("rebin_tmp_new","",nBinsNew,min,max);
  int bin = 1;
  int binNew = 1;
  for(; bin <= nBinsRebinned; bin += 2, binNew += 1) {
    // Add bin content dependent on type
    const double val1 = h->GetBinContent(bin);
    const double val2 = h->GetBinContent(bin+1);
    const double val = ( type=="error" ) ? sqrt( val1*val1 + val2*val2 ) : val1 + val2;
    hNew->SetBinContent(binNew,val);
    // In case of type != error, add bin errors
    // in quadrature
    const double err1 = h->GetBinError(bin);
    const double err2 = h->GetBinError(bin+1);
    const double err = ( type=="error" ) ? 0. : sqrt( err1*err1 + err2*err2 );    
    hNew->SetBinError(binNew,err);
  }
  if( lastBinIsOverflow_ ) {
    double val = h->GetBinContent(h->GetNbinsX());
    double err = h->GetBinError(h->GetNbinsX());
    if( nBinsInRange % 2 != 0 ) {
      const double val1 = h->GetBinContent(h->GetNbinsX()-1);
      const double err1 = h->GetBinError(h->GetNbinsX()-1);
      if( type=="error" ) {
	val = sqrt( val*val + val1*val1 );
	err = 0.;
      } else {
	val += val1;
	err = sqrt( err*err + err1*err1 );
      } 
    }
    hNew->SetBinContent(nBinsNew,val);
    hNew->SetBinError(nBinsNew,err);
  }

  delete h;
  h = hNew;
  h->SetName(name.c_str());
}
