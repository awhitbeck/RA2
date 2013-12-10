#include "BackgroundDistribution.h"
#include "InclusiveSearchBin.h"

#include <cmath>
#include <iostream>
#include <exception>
#include <string>
#include <vector>

#include "TH1.h"


BackgroundDistribution::BackgroundDistribution(const std::string &name, const std::string &var, const TH1* hOrig, const std::vector<InclusiveSearchBin*> &inclSearchBins)
  : name_(name), var_(var), label_(name), color_(1) {
  
  for(std::vector<InclusiveSearchBin*>::const_iterator it = inclSearchBins.begin();
      it != inclSearchBins.end(); ++it) {
    if( var_ != (*it)->var() ) {
      std::cerr << "\n\nERROR when creating BackgroundDistribution:" << std::endl;
      std::cerr << "  InclusiveSearchBin '" << (*it)->var() << "' does not fit to variable '" << var_ << "'" << std::endl;
      throw std::exception();
    }
  }

  hOrig_ = static_cast<TH1*>(hOrig->Clone(("BkgDist_"+var+"_orig").c_str()));
  yields_ = std::vector<double>(hOrig_->GetNbinsX(),0.);
  uncertDn_ = std::vector<double>(hOrig_->GetNbinsX(),0.);
  uncertUp_ = std::vector<double>(hOrig_->GetNbinsX(),0.);
  scale(inclSearchBins);
}

BackgroundDistribution::~BackgroundDistribution() {
  delete hOrig_;
}


void BackgroundDistribution::scale(const std::vector<InclusiveSearchBin*> &inclSearchBins) {
  
  std::cout << "\nPerforming scaling for " << toString() << std::endl;

  std::vector<InclusiveSearchBin*>::const_iterator itCurrInclBin = inclSearchBins.end();
  int firstHistBinInConsideredRange = 0;
  int lastHistBinInConsideredRange = 0;
  for(int histBin = 1; histBin <= hOrig_->GetNbinsX(); ++histBin) {
    // Histogram bin center
    const double val = hOrig_->GetBinCenter(histBin);

    // Find inclusive bin that covers the histogram bin center
    std::vector<InclusiveSearchBin*>::const_iterator itInclBin = inclSearchBins.begin();
    for(; itInclBin != inclSearchBins.end(); ++itInclBin) {
      if( (*itInclBin)->covers(val) ) {
	break;
      }
    }

    if( itCurrInclBin == inclSearchBins.end() ) {
      // The last histogram bin has not been covered by
      // any inclusive search bin
      
      if( itInclBin != inclSearchBins.end() ) {
	// This histogram bin is covered by an inclusive
	// search bin, and it is a new coverage (previous
	// hist bin had no coverage)

	if( !( firstHistBinInConsideredRange == 0 && lastHistBinInConsideredRange == 0 ) ) {
	  std::cout << "  - >>>> no coverage of histogram bins " << firstHistBinInConsideredRange << " - " << lastHistBinInConsideredRange << std::endl;
	}

	itCurrInclBin = itInclBin; // Store the covering inclusive search bin
	firstHistBinInConsideredRange = histBin; // This hist bin starts a new hist-bin range to be covered
	lastHistBinInConsideredRange = histBin;

      } else {
	// This histogram bin is also not covered
	// by any inclusive search bin
	
	lastHistBinInConsideredRange = histBin;

      }

    } else {
      // The previous histogram bin was covered by an
      // inclusive search bin

      if( itInclBin == itCurrInclBin ) {
	// This histogram bin is covered by the same inclusive
	// search bin as the previous histogram bin

	lastHistBinInConsideredRange = histBin;

      } else {
	// This histogram bin is covered by a new inclusive
	// search bin. Thus,

	// do the scaling for the previous range of covered
	// histogram bins;
	std::cout << "  - histogram bins " << firstHistBinInConsideredRange << " - " << lastHistBinInConsideredRange << " (" << hOrig_->GetXaxis()->GetBinLowEdge(firstHistBinInConsideredRange) << " - " << hOrig_->GetXaxis()->GetBinUpEdge(lastHistBinInConsideredRange) << ")" << " covered by InclusiveSearchBin " << (*itCurrInclBin)->toString() << std::endl;
	scaleInRange(firstHistBinInConsideredRange,lastHistBinInConsideredRange,*itCurrInclBin);

	// then, reset the histogram bin range counting
	itCurrInclBin = itInclBin;
	firstHistBinInConsideredRange = histBin;
	lastHistBinInConsideredRange = histBin;
      }

    }

  } // End of loop over histogram bins

  if( itCurrInclBin == inclSearchBins.end() ) {
    std::cout << "  - >>>> no coverage of histogram bins " << firstHistBinInConsideredRange << " - " << lastHistBinInConsideredRange << std::endl;
  } else {
    std::cout << "  - histogram bins " << firstHistBinInConsideredRange << " - " << lastHistBinInConsideredRange << " (" << hOrig_->GetXaxis()->GetBinLowEdge(firstHistBinInConsideredRange) << " - " << hOrig_->GetXaxis()->GetBinUpEdge(lastHistBinInConsideredRange) << ")" << " covered by InclusiveSearchBin " << (*itCurrInclBin)->toString() << std::endl;
    scaleInRange(firstHistBinInConsideredRange,lastHistBinInConsideredRange,*itCurrInclBin);
  }
}


void BackgroundDistribution::scaleInRange(int firstHistBin, int lastHistBin, const InclusiveSearchBin* inclSearchBin) {
  const double histYieldSum = hOrig_->Integral(firstHistBin,lastHistBin);
  std::cout << "      scale total yield: " << histYieldSum << " --> " << inclSearchBin->yield() << std::endl;
  if( histYieldSum > 0. ) {
    // scale the yields
    for(int histBin = firstHistBin; histBin <= lastHistBin; ++histBin) {
      const double histYield = hOrig_->GetBinContent(histBin);
      const double scaledHistYieldBin = histYield * inclSearchBin->yield() / histYieldSum;
      yields_.at(histBin-1) = scaledHistYieldBin;
      std::cout << "      yield in histogram bin " << histBin << ": " << histYield << " --> " << scaledHistYieldBin << std::endl;
    }
    // compute scaling factor for uncertainties
    std::cout << "      approximating uncertainties" << std::endl;
    double denom = 0.;
    for(int histBin = firstHistBin; histBin <= lastHistBin; ++histBin) {
      denom += ( yields_.at(histBin-1) + yields_.at(histBin-1)*yields_.at(histBin-1) );
    }
    const double scaleDn = sqrt( ( inclSearchBin->statDn()*inclSearchBin->statDn() + 
				   inclSearchBin->systDn()*inclSearchBin->systDn()   ) /
				 denom                                                   );
    const double scaleUp = sqrt( ( inclSearchBin->statUp()*inclSearchBin->statUp() + 
				   inclSearchBin->systUp()*inclSearchBin->systUp()   ) /
				 denom                                                   );
    // store total uncertainty per histogram bin
    for(int histBin = firstHistBin; histBin <= lastHistBin; ++histBin) {
      const double statDn = scaleDn * sqrt( yields_.at(histBin-1) ); 
      const double systDn = scaleDn * yields_.at(histBin-1);
      uncertDn_.at(histBin-1) = sqrt( statDn*statDn + systDn*systDn );
      const double statUp = scaleUp * sqrt( yields_.at(histBin-1) ); 
      const double systUp = scaleUp * yields_.at(histBin-1);
      uncertUp_.at(histBin-1) = sqrt( statUp*statUp + systUp*systUp );
    }
    // print out
    for(int histBin = firstHistBin; histBin <= lastHistBin; ++histBin) {
      std::cout << "      uncertainty in histogram bin " << histBin << ": -" << uncertDn_.at(histBin-1) << " +" << uncertUp_.at(histBin-1) << std::endl;
    }
  }

}


std::string BackgroundDistribution::toString() const {
  return var_ + " distribution of " + name_;
}
