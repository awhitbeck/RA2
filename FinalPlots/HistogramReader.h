#ifndef HISTOGRAM_READER_H
#define HISTOGRAM_READER_H

#include <string>
#include "TH1.h"


class HistogramReader {
public:
  HistogramReader(const std::string& dir)
    : dir_(dir) {};

  TH1* getHistogram(const std::string &id, const std::string& mode, const std::string &var) const;


private:
  std::string dir_;

  TH1* getAdjustedHistogram(TH1* h, const std::string &var) const;
  std::string getFileName(const std::string& id) const;
  std::string getHistName(const std::string& id, const std::string& mode, const std::string& var) const;
};


TH1* HistogramReader::getHistogram(const std::string &id, const std::string& mode, const std::string &var) const {

  const std::string fileName( getFileName(id) );
  TFile file(fileName.c_str(),"READ");
  TH1* h = NULL;
  const std::string histName( getHistName(id,mode,var) );
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


TH1* HistogramReader::getAdjustedHistogram(TH1* h, const std::string &var) const {
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


std::string HistogramReader::getFileName(const std::string& id) const {
  std::string fileName(dir_+"/");
  if(      id == "Data" ) {
    fileName += "SUS-13-012_DataHistogramsBaseline.root";
  } else if( id == "QCD"  ) {
    fileName += "QCDPrediction_histos.root";
  } else if( id == "LostLepton"  ) {
    fileName += "lostleptonTH1d.root";
  } else if( id == "HadTau"  ) {
    fileName += "HadTauPrediction_HTMHTNjet_v2.root";
  } else if( id == "ZInv"  ) {
    fileName += "zinvisible_LO_predicted_distributions.root";
  } else {
    std::cerr << "\n\nERROR when getting histograms from file" << std::endl;
    std::cerr << "  Unknown id '" << id << "'" << std::endl;
    throw std::exception();
  }

  return fileName;
}


std::string HistogramReader::getHistName(const std::string& id, const std::string& mode, const std::string& var) const {
  if( !( var == "HT" || var == "MHT" || var == "NJets" ) ) {
    std::cerr << "\n\nERROR when getting histograms from file" << std::endl;
    std::cerr << "  Unknown variable '" << var << "'" << std::endl;
    throw std::exception();
  }
  if( !( mode == "NJetsInclusive" ) )  {
    std::cerr << "\n\nERROR when getting histograms from file" << std::endl;
    std::cerr << "  Unknown mode '" << mode << "'" << std::endl;
    throw std::exception();
  }
  
  std::string histName("");
  
  if(      id == "Data" ) {
    if( mode == "NJetsInclusive" ) {
      if(      var == "HT"    ) histName = "hHT";
      else if( var == "MHT"   ) histName = "hMHT";
      else if( var == "NJets" ) histName = "hNJets";
    }
  } else if( id == "QCD"  ) {
    if( mode == "NJetsInclusive" ) {
      if(      var == "HT"    ) histName = "HT_baseline_pred_px";
      else if( var == "MHT"   ) histName = "MHT_baseline_pred_px";
      else if( var == "NJets" ) histName = "NJets_baseline_pred_px";
    }
  } else if( id == "LostLepton"  ) {
    if( mode == "NJetsInclusive" ) {
      if(      var == "HT"    ) histName = "LostLepton/HT";
      else if( var == "MHT"   ) histName = "LostLepton/MHT";
      else if( var == "NJets" ) histName = "LostLepton/NJet";
    }
  } else if( id == "HadTau"  ) {
    if( mode == "NJetsInclusive" ) {
      if(      var == "HT"    ) histName = "HT_hadtau_baseline_pred";
      else if( var == "MHT"   ) histName = "MHT_hadtau_baseline_pred";
      else if( var == "NJets" ) histName = "NJets_hadtau_baseline_pred";
    }
  } else if( id == "ZInv"  ) {
    if( mode == "NJetsInclusive" ) {
      if(      var == "HT"    ) histName = "htPlot";
      else if( var == "MHT"   ) histName = "mhtPlot";
      else if( var == "NJets" ) histName = "nJetsPlot";
    }
  } else {
    std::cerr << "\n\nERROR when getting histograms from file" << std::endl;
    std::cerr << "  Unknown id '" << id << "'" << std::endl;
    throw std::exception();
  }

  return histName;
}

#endif
