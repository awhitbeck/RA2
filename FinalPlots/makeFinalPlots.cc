#include <cmath>
#include <exception>
#include <iostream>
#include <map>
#include <vector>

#include "TColor.h"
#include "TFile.h"
#include "TH1.h"

#include "BackgroundDistribution.h"
#include "InclusiveSearchBin.h"
#include "FinalPlot.h"
#include "SearchBin.h"
#include "Style.h"

std::string PWD;


TH1* getAdjustedHistogram(TH1* h, const std::string &var) {
  TH1* hNew = NULL;
  if(      var == "HT"    )  hNew = new TH1D(h->GetName(),"",45,500,2750);
  else if( var == "MHT"   )  hNew = new TH1D(h->GetName(),"",19,200,1150);
  else if( var == "NJets" )  hNew = new TH1D(h->GetName(),"",8,2.5,10.5);
  const int lastBin = hNew->GetNbinsX();
  for(int bin = 1; bin <= lastBin; ++bin) {
    const int origBin = h->FindBin(hNew->GetBinCenter(bin));
    if( var != "NJets" ) {
      const double origMin = h->GetXaxis()->GetBinLowEdge(origBin);
      const double origMax = h->GetXaxis()->GetBinUpEdge(origBin);
      const double min = hNew->GetXaxis()->GetBinLowEdge(bin);
      const double max = hNew->GetXaxis()->GetBinUpEdge(bin);
      if( origMin != min || origMax != max ) {
	std::cerr << "\n\nERROR when adjusting histogram '" << h->GetName() << "'" << std::endl;
	std::cerr << "  Wrong edges in bin " << bin << std::endl;
	std::cerr << "    orig min = " << origMin << " != " << min << " = min" << std::endl;
	std::cerr << "    orig max = " << origMax << " != " << max << " = max" << std::endl;
	throw std::exception();
      }
    }
    hNew->SetBinContent(bin,h->GetBinContent(origBin));
    hNew->SetBinError(bin,h->GetBinError(origBin));
  }
  // Content of the last new bin
  double overflowContent = hNew->GetBinContent(lastBin);
  double overflowError2 = hNew->GetBinError(lastBin)*hNew->GetBinError(lastBin);
  // Add content of further original bins
  for(int origBin = h->FindBin(hNew->GetBinCenter(lastBin))+1;
      origBin <= h->GetNbinsX(); ++origBin) {
    overflowContent += h->GetBinContent(origBin);
    overflowError2 += h->GetBinError(origBin)*h->GetBinError(origBin);
  }      
  hNew->SetBinContent(lastBin,overflowContent);
  hNew->SetBinError(lastBin,sqrt(overflowError2));

  return hNew;
}


TH1* getHistogram(const std::string &id, const std::string &var) {
  if( !( var == "HT" || var == "MHT" || var == "NJets" ) ) {
    std::cerr << "\n\nERROR when getting '" << var << "' histograms from file" << std::endl;
    std::cerr << "  Unknown variable '" << var << "'" << std::endl;
    throw std::exception();
  }

  std::string fileName(PWD+"data/");
  std::string histName("");

  if(      id == "Data" ) {
    fileName += "SUS-13-012_DataHistogramsBaseline.root";
    if(      var == "HT"    ) histName = "hHT";
    else if( var == "MHT"   ) histName = "hMHT";
    else if( var == "NJets" ) histName = "hNJets";

  } else if( id == "QCD"  ) {
    fileName += "QCDPrediction_histos.root";
    if(      var == "HT"    ) histName = "HT_baseline_pred_px";
    else if( var == "MHT"   ) histName = "MHT_baseline_pred_px";
    else if( var == "NJets" ) histName = "NJets_baseline_pred_px";

  } else if( id == "LostLepton"  ) {
    fileName += "lostleptonTH1d.root";
    if(      var == "HT"    ) histName = "LostLepton/HT";
    else if( var == "MHT"   ) histName = "LostLepton/MHT";
    else if( var == "NJets" ) histName = "LostLepton/NJet";

  } else if( id == "HadTau"  ) {
    fileName += "HadTauPrediction_HTMHTNjet_v2.root";
    if(      var == "HT"    ) histName = "HT_hadtau_baseline_pred";
    else if( var == "MHT"   ) histName = "MHT_hadtau_baseline_pred";
    else if( var == "NJets" ) histName = "NJets_hadtau_baseline_pred";

  } else if( id == "ZInv"  ) {
    fileName += "zinvisible_LO_predicted_distributions.root";
    if(      var == "HT"    ) histName = "htPlot";
    else if( var == "MHT"   ) histName = "mhtPlot";
    else if( var == "NJets" ) histName = "nJetsPlot";

  } else {
    std::cerr << "\n\nERROR when getting '" << var << "' histograms from file" << std::endl;
    std::cerr << "  Unknown id '" << id << "'" << std::endl;
    throw std::exception();
  }

  TFile file(fileName.c_str(),"READ");
  TH1* h = NULL;
  file.GetObject(histName.c_str(),h);
  if( h == NULL ) {
    std::cerr << "\n\nERROR when getting '" << var << "' histograms from file" << std::endl;
    std::cerr << "  Histogram '" << histName << "' not found" << std::endl;
    throw std::exception();
  }
  h->SetDirectory(0);
  h->SetName(("h"+var+"_"+id).c_str());
  file.Close();
  // scale had-tau
  if( id == "HadTau" ) h->Scale(h->GetBinWidth(1));
  TH1* hAdjusted = getAdjustedHistogram(h,var);
  delete h;

  return hAdjusted;
}


std::vector<SearchBin*> getSearchBins(const std::string &id) {
  std::string fileName(PWD+"data/");
  if(      id == "QCD"        ) fileName += "qcd_searchBinPredictions.txt";
  else if( id == "LostLepton" ) fileName += "lostLepton_searchBinPredictions.txt";
  else if( id == "HadTau"     ) fileName += "tau_searchBinPredictions.txt";
  else if( id == "ZInv"       ) fileName += "zinvis_searchBinPredictions.txt";
  else {
    std::cerr << "\n\nERROR when reading '" << id << "' search-bin yields" << std::endl;
    std::cerr << "  Unknown id '" << id << "'" << std::endl;
    throw std::exception();
  }
  
  return SearchBin::createFromTxtFile(fileName);
}


std::vector<InclusiveSearchBin*> getInclusiveSearchBins(const std::string &var, const std::vector<SearchBin*> &searchBins) {

  std::vector<InclusiveSearchBin*> inclBins;

  if( var == "HT" ) {
    // 500 < HT < 800
    inclBins.push_back(new InclusiveSearchBin(var,500.,800.));
    inclBins.back()->addSearchBin(searchBins.at(0));
    inclBins.back()->addSearchBin(searchBins.at(1));
    inclBins.back()->addSearchBin(searchBins.at(2));
    inclBins.back()->addSearchBin(searchBins.at(3));
    inclBins.back()->addSearchBin(searchBins.at(17));
    inclBins.back()->addSearchBin(searchBins.at(18));
    inclBins.back()->addSearchBin(searchBins.at(19));
    inclBins.back()->addSearchBin(searchBins.at(31));
    
    // 800 < HT < 1000
    inclBins.push_back(new InclusiveSearchBin(var,800.,1000.));
    inclBins.back()->addSearchBin(searchBins.at(4));
    inclBins.back()->addSearchBin(searchBins.at(5));
    inclBins.back()->addSearchBin(searchBins.at(6));
    inclBins.back()->addSearchBin(searchBins.at(7));
    inclBins.back()->addSearchBin(searchBins.at(20));
    inclBins.back()->addSearchBin(searchBins.at(21));
    inclBins.back()->addSearchBin(searchBins.at(22));
    inclBins.back()->addSearchBin(searchBins.at(32));
    
    // 1000 < HT < 1250
    inclBins.push_back(new InclusiveSearchBin(var,1000.,1250.));
    inclBins.back()->addSearchBin(searchBins.at(8));
    inclBins.back()->addSearchBin(searchBins.at(9));
    inclBins.back()->addSearchBin(searchBins.at(10));
    inclBins.back()->addSearchBin(searchBins.at(11));
    inclBins.back()->addSearchBin(searchBins.at(23));
    inclBins.back()->addSearchBin(searchBins.at(24));
    inclBins.back()->addSearchBin(searchBins.at(25));
    inclBins.back()->addSearchBin(searchBins.at(33));

    // 1250 < HT < 1500
    inclBins.push_back(new InclusiveSearchBin(var,1250.,1500.));
    inclBins.back()->addSearchBin(searchBins.at(12));
    inclBins.back()->addSearchBin(searchBins.at(13));
    inclBins.back()->addSearchBin(searchBins.at(14));
    inclBins.back()->addSearchBin(searchBins.at(26));
    inclBins.back()->addSearchBin(searchBins.at(27));
    inclBins.back()->addSearchBin(searchBins.at(28));
    inclBins.back()->addSearchBin(searchBins.at(34));

    // HT > 1500
    inclBins.push_back(new InclusiveSearchBin(var,1500.,99999.));
    inclBins.back()->addSearchBin(searchBins.at(15));
    inclBins.back()->addSearchBin(searchBins.at(16));
    inclBins.back()->addSearchBin(searchBins.at(29));
    inclBins.back()->addSearchBin(searchBins.at(30));
    inclBins.back()->addSearchBin(searchBins.at(35));

  } else   if( var == "MHT" ) {
    // 200 < MHT < 300
    inclBins.push_back(new InclusiveSearchBin(var,200.,300.));
    inclBins.back()->addSearchBin(searchBins.at(0));
    inclBins.back()->addSearchBin(searchBins.at(4));
    inclBins.back()->addSearchBin(searchBins.at(8));
    inclBins.back()->addSearchBin(searchBins.at(12));
    inclBins.back()->addSearchBin(searchBins.at(15));
    inclBins.back()->addSearchBin(searchBins.at(17));
    inclBins.back()->addSearchBin(searchBins.at(20));
    inclBins.back()->addSearchBin(searchBins.at(23));
    inclBins.back()->addSearchBin(searchBins.at(26));
    inclBins.back()->addSearchBin(searchBins.at(29));
    inclBins.back()->addSearchBin(searchBins.at(31));
    inclBins.back()->addSearchBin(searchBins.at(32));
    inclBins.back()->addSearchBin(searchBins.at(33));
    inclBins.back()->addSearchBin(searchBins.at(34));
    inclBins.back()->addSearchBin(searchBins.at(35));

    // 300 < MHT < 450
    inclBins.push_back(new InclusiveSearchBin(var,300.,450.));
    inclBins.back()->addSearchBin(searchBins.at(1));
    inclBins.back()->addSearchBin(searchBins.at(5));
    inclBins.back()->addSearchBin(searchBins.at(9));
    inclBins.back()->addSearchBin(searchBins.at(13));
    inclBins.back()->addSearchBin(searchBins.at(16));
    inclBins.back()->addSearchBin(searchBins.at(18));
    inclBins.back()->addSearchBin(searchBins.at(21));
    inclBins.back()->addSearchBin(searchBins.at(24));
    inclBins.back()->addSearchBin(searchBins.at(27));
    inclBins.back()->addSearchBin(searchBins.at(30));

    // inclBins.back()->addSearchBin(searchBins.at(31));
    // inclBins.back()->addSearchBin(searchBins.at(32));
    // inclBins.back()->addSearchBin(searchBins.at(33));
    // inclBins.back()->addSearchBin(searchBins.at(34));
    // inclBins.back()->addSearchBin(searchBins.at(35));


    // 450 < MHT < 600
    inclBins.push_back(new InclusiveSearchBin(var,450.,600.));
    inclBins.back()->addSearchBin(searchBins.at(2));
    inclBins.back()->addSearchBin(searchBins.at(6));
    inclBins.back()->addSearchBin(searchBins.at(10));
    inclBins.back()->addSearchBin(searchBins.at(14));
    inclBins.back()->addSearchBin(searchBins.at(19));
    inclBins.back()->addSearchBin(searchBins.at(22));
    inclBins.back()->addSearchBin(searchBins.at(25));
    inclBins.back()->addSearchBin(searchBins.at(28));

    // inclBins.back()->addSearchBin(searchBins.at(16));
    // inclBins.back()->addSearchBin(searchBins.at(30));

    // MHT > 600
    inclBins.push_back(new InclusiveSearchBin(var,600.,9999.));
    inclBins.back()->addSearchBin(searchBins.at(3));
    inclBins.back()->addSearchBin(searchBins.at(7));
    inclBins.back()->addSearchBin(searchBins.at(11));

    // inclBins.back()->addSearchBin(searchBins.at(14));
    // inclBins.back()->addSearchBin(searchBins.at(19));
    // inclBins.back()->addSearchBin(searchBins.at(22));
    // inclBins.back()->addSearchBin(searchBins.at(25));
    // inclBins.back()->addSearchBin(searchBins.at(28));

  } else  if( var == "NJets" ) {
    // 3 <= NJets <= 5
    inclBins.push_back(new InclusiveSearchBin(var,3,5));
    for(int i = 0; i <= 16; ++i) {
      inclBins.back()->addSearchBin(searchBins.at(i));
    }
    
    // 6 <= NJets <= 7
    inclBins.push_back(new InclusiveSearchBin(var,6,7));
    for(int i = 17; i <= 30; ++i) {
      inclBins.back()->addSearchBin(searchBins.at(i));
    }
    
    // NJets >= 8
    inclBins.push_back(new InclusiveSearchBin(var,8,99999));
    for(int i = 31; i <= 35; ++i) {
      inclBins.back()->addSearchBin(searchBins.at(i));
    }

  } else {
    std::cerr << "\n\nERROR when creating inclusive search bins" << std::endl;
    std::cerr << "  Unknown variable '" << var << "'" << std::endl;
    throw std::exception();
  }

  return inclBins;
}


int main() {
  Style::init();
  PWD = "/home/matsch/Development/FinalPlots/";

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

  // Get background yields in search bins
  std::map< std::string, std::vector<SearchBin*> > searchBinsForBkgs;
  for(std::vector<std::string>::const_iterator bkgIt = bkgs.begin();
      bkgIt != bkgs.end(); ++bkgIt) {
    const std::string bkg = *bkgIt;
    std::cout << "\n\nReading " << bkg << " background predictions in search bins" << std::endl;
    std::vector<SearchBin*> searchBins = getSearchBins(bkg);
    for(std::vector<SearchBin*>::const_iterator it = searchBins.begin();
	it != searchBins.end(); ++it) {
      (*it)->print();
    }
    searchBinsForBkgs[bkg] = searchBins;
  }

  // Loop over variables
  for(std::vector<std::string>::const_iterator varIt = vars.begin();
      varIt != vars.end(); ++varIt) {
    const std::string var = *varIt;

    std::cout << "\n\nCreating the " << var << " plot" << std::endl;

    // Create the plot
    const TH1* hData = getHistogram("Data",var);
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
      const TH1* hBkg = getHistogram(bkg,var);
      
      // Retrieve individual search bins from map
      const std::vector<SearchBin*> searchBins = searchBinsForBkgs.find(bkg)->second;
      
      // Combine search bins for this variable
      std::vector<InclusiveSearchBin*> inclSearchBins = getInclusiveSearchBins(var,searchBins);
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
  
  // Clean up
  for(std::map< std::string, std::vector<SearchBin*> >::iterator mit = searchBinsForBkgs.begin();
      mit != searchBinsForBkgs.end(); ++mit) {
    for(std::vector<SearchBin*>::iterator sit = mit->second.begin();
	sit != mit->second.end(); ++sit) {
      delete *sit;
    }
  }
  
  return 0;
}
