#include <cmath>
#include <exception>
#include <iostream>
#include <map>
#include <unistd.h>
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
#include "SignalDistribution.h"
#include "Style.h"


// Get the current working directory
std::string pwd() {
  const size_t size = 500;
  char buffer[size];
  if( !getcwd(buffer,size) ) {
    std::cerr << "\n\nERROR finding current working directory" << std::endl;
    throw std::exception();
  }
  std::string dir(buffer);

  return dir;
}


void makeFinalPlots(const std::string& mode, bool isPaperPlot) {
  Style::init();
  const std::string dir(pwd());

  // Define plotted variables
  std::vector<std::string> vars;
  vars.push_back("HT");
  vars.push_back("MHT");
  if( mode == "Baseline" || mode == "AlternativeScheme" ) vars.push_back("NJets");

  // Define plotted backgrounds
  std::vector<std::string> bkgs;
  bkgs.push_back("QCD");
  bkgs.push_back("LostLepton");
  bkgs.push_back("HadTau");
  bkgs.push_back("ZInv");

  // Define plotted signal expectations
  std::vector<std::string> signals;
  signals.push_back("T1tttt:mGL1025_mLSP25");
  signals.push_back("T1tttt:mGL1100_mLSP25");

  // The out-name prefix
  const std::string outName = isPaperPlot ? "SUS-13-012_Result_"+mode : "RA2_Result_"+mode;

  // The histogram title
  std::vector<std::string> title;
  title.push_back("L = 19.5 fb^{-1},  #sqrt{s} = 8 TeV");
  if( isPaperPlot ) {
    title.back() = "CMS,  "+title.back();
  }
  if(         mode == "NJets3-5") {
    title.push_back("3 #leq N_{Jets}#leq 5");
  } else  if( mode == "NJets6-7") {
    title.push_back("6 #leq N_{Jets}#leq 7");
  } else  if( mode == "NJets8-Inf") {
    title.push_back("N_{Jets}#geq 8");
  } else {
    title.push_back("N_{Jets}#geq 3");
  }
  title.back() += ",  H_{T}> 500 GeV,  #slash{H}_{T}> 200 GeV";

  // Takes care of reading the histograms
  const HistogramReader hReader(dir+"/data");

  // Takes care of search-bin reading and combination
  const SearchBinManager binManager(dir+"/data",bkgs);

  // Loop over variables
  for(std::vector<std::string>::const_iterator varIt = vars.begin();
      varIt != vars.end(); ++varIt) {
    const std::string var = *varIt;

    std::cout << "\n\nCreating the " << var << " plot" << std::endl;

    // Create the plot
    const TH1* hData = hReader.getHistogram("Data",mode,var);
    const bool rebin = ( var == "HT" ); // Want to go from 50 to 100 GeV bins after scaling
    const bool lastBinIsOverflow = true;
    FinalPlot* plot = new FinalPlot(var,hData,rebin,lastBinIsOverflow,outName);
    plot->setTitle(title);
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

    // Add signal simulation
    for(std::vector<std::string>::const_iterator sigIt = signals.begin();
	sigIt != signals.end(); ++sigIt) {
      std::cout << "Adding the " << *sigIt << " signal expectations to the " << var << " plot" << std::endl;
      const TH1* hSignal = hReader.getHistogram(*sigIt,mode,var);
      SignalDistribution* sig = new SignalDistribution(*sigIt,var,hSignal);
      sig->setLineColor(Style::color(*sigIt));
      sig->setLegendLabel(Style::legendLabel(*sigIt));
      plot->addSignal(sig);
      delete hSignal;
    }


    // Draw the plot and clean up
    plot->draw();
    delete plot;
    
  } // End of loop over the histogram variables
}


int main() {
  const bool isPaperPlot = false; // Paper plot (label 'CMS')?
  
  makeFinalPlots("Baseline",isPaperPlot);	// the default NJets inclusive plots
  // makeFinalPlots("NJets3-5",isPaperPlot);
  // makeFinalPlots("NJets6-7",isPaperPlot);
  // makeFinalPlots("NJets8-Inf",isPaperPlot);
  //  makeFinalPlots("AlternativeScheme",isPaperPlot); // same as Baseline but with the alternative MHT combination scheme

  return 0;
}
