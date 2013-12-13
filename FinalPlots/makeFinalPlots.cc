#include <cmath>
#include <exception>
#include <iostream>
#include <map>
#include <vector>

#include "TColor.h"
#include "TFile.h"
#include "TH1.h"

#include "BackgroundDistribution.h"
#include "FinalPlot.h"
#include "HistogramReader.h"
#include "InclusiveSearchBin.h"
#include "SearchBin.h"
#include "SearchBinManager.h"
#include "Style.h"


int main() {
  Style::init();
  const std::string pwd = "/home/matsch/Development/FinalPlots/";

  // Define combination mode
  const std::string mode("NJetsInclusive");

  // Define plotted variables
  std::vector<std::string> vars;
  vars.push_back("HT");
  vars.push_back("MHT");
  vars.push_back("NJets");

  // Define plotted backgrounds
  std::vector<std::string> bkgs;
  bkgs.push_back("QCD");
  bkgs.push_back("LostLepton");
  bkgs.push_back("HadTau");
  bkgs.push_back("ZInv");

  // Takes care of reading the histograms
  const HistogramReader hReader(pwd+"/data");

  // Takes care of search-bin reading and combination
  const SearchBinManager binManager(pwd+"/data",bkgs);

  // Loop over variables
  for(std::vector<std::string>::const_iterator varIt = vars.begin();
      varIt != vars.end(); ++varIt) {
    const std::string var = *varIt;

    std::cout << "\n\nCreating the " << var << " plot" << std::endl;

    // Create the plot
    const TH1* hData = hReader.getHistogram("Data",mode,var);
    const bool rebin = ( var == "HT" ); // Want to go from 50 to 100 GeV bins after scaling
    const bool lastBinIsOverflow = true;
    FinalPlot* plot = new FinalPlot(var,hData,rebin,lastBinIsOverflow);
    delete hData;
  
    // Add backgrounds to plot
    for(std::vector<std::string>::const_iterator bkgIt = bkgs.begin();
	bkgIt != bkgs.end(); ++bkgIt) {
      const std::string bkg = *bkgIt;
      std::cout << "Adding the " << bkg << " background to the " << var << " plot" << std::endl;
      
      // Get background histogram
      const TH1* hBkg = hReader.getHistogram(bkg,mode,var);
      
      // Combined search bins for this variable
      std::vector<InclusiveSearchBin*> inclSearchBins = binManager.getInclusiveSearchBins(var,mode,bkg);
      for(std::vector<InclusiveSearchBin*>::const_iterator it = inclSearchBins.begin();
	  it != inclSearchBins.end(); ++it) {
	(*it)->print();
      }
  
      // Add background distribution with approximated proper
      // yields and uncertainties to plot
      BackgroundDistribution* bkgDist = new BackgroundDistribution(bkg,var,hBkg,inclSearchBins);
      bkgDist->setLegendLabel(Style::legendLabel(bkg));
      bkgDist->setFillColor(Style::color(bkg));
      plot->addBackground(bkgDist);

      // Delete up helper objecs
      delete hBkg;
      for(std::vector<InclusiveSearchBin*>::iterator it = inclSearchBins.begin();
	  it != inclSearchBins.end(); ++it) {
	delete *it;
      }

    } // End of loop over backgrounds

    // Draw the plot and clean up
    plot->draw();
    delete plot;
    
  } // End of loop over the histogram variables
  
  return 0;
}
