#ifndef HISTOGRAM_READER_H
#define HISTOGRAM_READER_H

#include <string>
#include "TH1.h"
#include "TString.h"


class HistogramReader {
public:
  HistogramReader(const std::string& dir)
    : dir_(dir.c_str()) {};

  TH1* getHistogram(const std::string &id, const std::string& mode, const std::string &var) const;


private:
  TString dir_;

  TH1* getAdjustedHistogram(TH1* h, const std::string& mode, const std::string &var) const;
  TH1* splitBinning(const TH1* h) const;
  TString getFileName(const std::string& id, const std::string& mode) const;
  TString getHistName(const std::string& id, const std::string& mode, const std::string& var) const;
  TString findMassId(const std::string& id) const;
  void normSignal(TFile& file, const TString& histName, TH1* h) const;
  void splitAtLast(const TString& str, const char c, TString& str1, TString& str2) const;
  bool isSignal(const std::string& id) const;
};


TH1* HistogramReader::getHistogram(const std::string &id, const std::string& mode, const std::string &var) const {

  const TString fileName( getFileName(id,mode) );
  TFile file(fileName,"READ");
  TH1* h = NULL;
  const TString histName( getHistName(id,mode,var) );
  file.GetObject(histName,h);
  if( h == NULL ) {
    std::cerr << "\n\nERROR when getting '" << var << "' histograms from file" << std::endl;
    std::cerr << "  Histogram '" << histName << "' not found" << std::endl;
    throw std::exception();
  }
  h->SetDirectory(0);
  h->SetName(("h"+var+"_"+id).c_str());

  // scale had-tau
  if( id == "HadTau" ) h->Scale(h->GetBinWidth(1));

  // scale signal
  if( isSignal(id) ) normSignal(file,histName,h);

  file.Close();

  if( isSignal(id) && var == "HT" ) {		// Signal HT plots are in 100 GeV binning only
    TH1* hNew = splitBinning(h);
    const TString nameOrig( h->GetName() );
    delete h;
    h = hNew;
    h->SetName( nameOrig );
  }

  // pick out interesting x-range and fill overflow bin
  TH1* hAdjusted = getAdjustedHistogram(h,mode,var);
  const TString nameOrig( h->GetName() );
  delete h;
  hAdjusted->SetName(nameOrig);

  return hAdjusted;
}


// Artificially histogram to to half the bin size (intended for signal HT distribution)
TH1* HistogramReader::splitBinning(const TH1* h) const {
  const double nBinsOrig = h->GetNbinsX();
  const double minOrig = h->GetXaxis()->GetBinLowEdge(1);
  const double maxOrig = h->GetXaxis()->GetBinUpEdge(h->GetNbinsX());
  const TString nameOrig( h->GetName() );
  TH1* hNew = new TH1D(nameOrig+":split","",2*nBinsOrig,minOrig,maxOrig);
  for(int bin = 1; bin <= h->GetNbinsX(); ++bin) {
    int newBin = 2*bin-1;
    hNew->SetBinContent(newBin,h->GetBinContent(bin));
    //std::cout << bin << "(" << h->GetXaxis()->GetBinLowEdge(bin) << "-" << h->GetXaxis()->GetBinUpEdge(bin) << ") -->" << newBin << "(" << hNew->GetXaxis()->GetBinLowEdge(newBin) << "-" << hNew->GetXaxis()->GetBinUpEdge(newBin) << ") : " << h->GetBinContent(bin) << std::endl;
  }
  
  return hNew;
}


TH1* HistogramReader::getAdjustedHistogram(TH1* h, const std::string& mode, const std::string &var) const {
  const TString newName = TString( h->GetName() ) + ":adjusted";
  TH1* hNew = NULL;
  if(      var == "HT"    ) {
    if( mode == "NJets6-7" ) {
      hNew = new TH1D(newName,"",38,500,2400);
    } else if( mode == "NJets8-Inf" ) {
      hNew = new TH1D(newName,"",26,500,1800);
    } else {
      hNew = new TH1D(newName,"",45,500,2750);
    }
  } else if( var == "MHT"   ) {
    if( mode == "NJets6-7" ) {
      hNew = new TH1D(newName,"",11,200,750);
    } else if( mode == "NJets8-Inf" ) {
      hNew = new TH1D(newName,"",6,200,500);
    } else {
      hNew = new TH1D(newName,"",19,200,1150);
    }
  } else if( var == "NJets" ) {
    hNew = new TH1D(newName,"",8,2.5,10.5);
  }
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


TString HistogramReader::getFileName(const std::string& id, const std::string& mode) const {
  if( !( mode=="Baseline" || mode=="AlternativeScheme" || mode=="NJets3-5" || mode=="NJets6-7" || mode=="NJets8-Inf" ) )  {
    std::cerr << "\n\nERROR when getting histograms from file" << std::endl;
    std::cerr << "  Unknown mode '" << mode << "'" << std::endl;
    throw std::exception();
  }
  TString fileName(dir_+"/");
  if(      id == "Data" ) {
    fileName += "SUS-13-012_DataHistograms.root";
  } else if( id == "QCD"  ) {
    fileName += "QCDPrediction_histos.root";
  } else if( id == "LostLepton"  ) {
    if( mode == "Baseline" || mode == "AlternativeScheme" ) {
      fileName += "lostleptonTH1d.root";
    } else if( mode == "NJets3-5" ) {
      fileName += "lostleptonTH1dNJet3-5.root";
    } else if( mode == "NJets6-7" ) {
      fileName += "lostleptonTH1dNJet6-7.root";
    } else if( mode == "NJets8-Inf" ) {
      fileName += "lostleptonTH1dNJet8-Inf.root";
    }
  } else if( id == "HadTau"  ) {
    if( mode == "Baseline" || mode == "AlternativeScheme" ) {
      fileName += "HadTauPrediction_HTMHTNjet_v2.root";
    } else if( mode == "NJets3-5" ) {
      fileName += "HadTauPrediction_HTMHTNjet_3_5.root";
    } else if( mode == "NJets6-7" ) {
      fileName += "HadTauPrediction_HTMHTNjet_6_7.root";
    } else if( mode == "NJets8-Inf" ) {
      fileName += "HadTauPrediction_HTMHTNjet_8_up.root";
    }
  } else if( id == "ZInv"  ) {
    fileName += "zinvisible_LO_predicted_distributions_full_set.root";
  } else if( id.find("T1qqqq") != std::string::npos ) {
    fileName += "SignalHistograms_T1qqqqMG.root";
  } else if( id.find("T1tttt") != std::string::npos ) {
    fileName += "SignalHistograms_T1ttttMG.root";
  } else if( id.find("T2qq"  ) != std::string::npos ) {
    fileName += "SignalHistograms_T2qqMG.root";
  } else if( id.find("T5VV") != std::string::npos ) {
    fileName += "SignalHistograms_T5VVMG.root";
  } else {
    std::cerr << "\n\nERROR when getting histograms from file" << std::endl;
    std::cerr << "  Unknown id '" << id << "'" << std::endl;
    throw std::exception();
  }

  return fileName;
}


TString HistogramReader::getHistName(const std::string& id, const std::string& mode, const std::string& var) const {
  if( !( var == "HT" || var == "MHT" || var == "NJets" ) ) {
    std::cerr << "\n\nERROR when getting histograms from file" << std::endl;
    std::cerr << "  Unknown variable '" << var << "'" << std::endl;
    throw std::exception();
  }
  if( !( mode=="Baseline" || mode=="AlternativeScheme" || mode=="NJets3-5" || mode=="NJets6-7" || mode=="NJets8-Inf" ) )  {
    std::cerr << "\n\nERROR when getting histograms from file" << std::endl;
    std::cerr << "  Unknown mode '" << mode << "'" << std::endl;
    throw std::exception();
  }
  
  TString histName("");
  
  if(      id == "Data" ) {
    if( mode == "Baseline" || mode == "AlternativeScheme" ) {
      if(      var == "HT"    ) histName = "HT_NJetsInclusive";
      else if( var == "MHT"   ) histName = "MHT_NJetsInclusive";
      else if( var == "NJets" ) histName = "NJets_NJetsInclusive";
    } else if( mode == "NJets3-5" ) {
      if(      var == "HT"    ) histName = "HT_NJets3-5";
      else if( var == "MHT"   ) histName = "MHT_NJets3-5";
    } else if( mode == "NJets6-7" ) {
      if(      var == "HT"    ) histName = "HT_NJets6-7";
      else if( var == "MHT"   ) histName = "MHT_NJets6-7";
    } else if( mode == "NJets8-Inf" ) {
      if(      var == "HT"    ) histName = "HT_NJets8-Inf";
      else if( var == "MHT"   ) histName = "MHT_NJets8-Inf";
    }
  } else if( id == "QCD"  ) {
    if( mode == "Baseline" || mode == "AlternativeScheme" ) {
      if(      var == "HT"    ) histName = "HT_baseline_pred_px"; 
      else if( var == "MHT"   ) histName = "MHT_baseline_pred_px";
      else if( var == "NJets" ) histName = "NJets_baseline_pred_px";
    } else if( mode == "NJets3-5" ) {
      if(      var == "HT"    ) histName = "HT_NJets3To5_pred_px";
      else if( var == "MHT"   ) histName = "MHT_NJets3To5_pred_px";
    } else if( mode == "NJets6-7" ) {
      if(      var == "HT"    ) histName = "HT_NJets6To7_pred_px";
      else if( var == "MHT"   ) histName = "MHT_NJets6To7_pred_px";
    } else if( mode == "NJets8-Inf" ) {
      if(      var == "HT"    ) histName = "HT_NJetsGreater7_pred_px";
      else if( var == "MHT"   ) histName = "MHT_NJetsGreater7_pred_px";
    }
  } else if( id == "LostLepton"  ) {
    if(      var == "HT"    ) histName = "LostLepton/HT";
    else if( var == "MHT"   ) histName = "LostLepton/MHT";
    else if( var == "NJets" ) histName = "LostLepton/NJet";
  } else if( id == "HadTau"  ) {
    if(      var == "HT"    ) histName = "HT_hadtau_baseline_pred";
    else if( var == "MHT"   ) histName = "MHT_hadtau_baseline_pred";
    else if( var == "NJets" ) histName = "NJets_hadtau_baseline_pred";
  } else if( id == "ZInv"  ) {
    if( mode == "Baseline" || mode == "AlternativeScheme" ) {
      if(      var == "HT"    ) histName = "htPlot_inclusive";
      else if( var == "MHT"   ) histName = "mhtPlot_inclusive";
      else if( var == "NJets" ) histName = "nJetsPlot_inclusive";
    } else if( mode == "NJets3-5" ) {
      if(      var == "HT"    ) histName = "htPlot_3to5";
      else if( var == "MHT"   ) histName = "mhtPlot_3to5";
    } else if( mode == "NJets6-7" ) {
      if(      var == "HT"    ) histName = "htPlot_6to7";
      else if( var == "MHT"   ) histName = "mhtPlot_6to7";
    } else if( mode == "NJets8-Inf" ) {
      if(      var == "HT"    ) histName = "htPlot_8plus";
      else if( var == "MHT"   ) histName = "mhtPlot_8plus";
    }

  } else if( isSignal(id) ) {
    // parse for mass values
    histName = "SigHists/h_" + findMassId(id) + "_HT500MHT200_";
    if( mode == "Baseline" || mode == "AlternativeScheme" ) {
      if(      var == "HT"    ) histName += "HT";
      else if( var == "MHT"   ) histName += "MHT";
      else if( var == "NJets" ) histName += "NJets";
    } else if( mode == "NJets3-5" ) {
      if(      var == "HT"    ) histName += "HT_3Jets5";
      else if( var == "MHT"   ) histName += "MHT_3Jets5";
    } else if( mode == "NJets6-7" ) {
      if(      var == "HT"    ) histName += "HT_6Jets7";
      else if( var == "MHT"   ) histName += "MHT_6Jets7";
    } else if( mode == "NJets8-Inf" ) {
      if(      var == "HT"    ) histName += "HT_8JetsX";
      else if( var == "MHT"   ) histName += "MHT_8JetsX";
    }
  } else {
    std::cerr << "\n\nERROR when getting histograms from file" << std::endl;
    std::cerr << "  Unknown id '" << id << "'" << std::endl;
    throw std::exception();
  }

  return histName;
}


TString HistogramReader::findMassId(const std::string& id) const {
  const TString str(id);
  const size_t pos = str.First(":") + 1;
  const size_t len = str.Length()-pos;
  
  return str(pos,len);
}


void HistogramReader::normSignal(TFile& file, const TString& histName, TH1* h) const {
  TString name(histName);
  TString part1;
  TString part2;
  splitAtLast(name,'_',part1,part2);
  if( !( part2 == "HT" || part2 == "MHT" || part2 == "NJets" ) ) {
    name = part1;
    splitAtLast(name,'_',part1,part2);
  }
  name = part1 + "_Total";
  std::cout << "  norm signal : " << histName << " : " << name << std::endl;
  TH1* hNorm = NULL;
  file.GetObject(name,hNorm);
  if( hNorm == NULL ) {
    std::cerr << "\n\nERROR when normalising signal histogram '" << h->GetName() << "'" << std::endl;
    std::cerr << "  Histogram '" << name << "' not found" << std::endl;
    throw std::exception();
  }
  hNorm->SetDirectory(0);
  const double norm = hNorm->GetEntries();
  if( norm > 0. ) h->Scale(1./norm);
}


void HistogramReader::splitAtLast(const TString& str, const char c, TString& str1, TString& str2) const {
  size_t pos = str.Last(c);
  str1 = str(0,pos);
  str2 = str(pos+1,str.Length()-pos);
}


bool HistogramReader::isSignal(const std::string& id) const {
  bool result = false;
  if(      id.find("T1qqqq") != std::string::npos ) result = true;
  else if( id.find("T1tttt") != std::string::npos ) result = true;
  else if( id.find("T2qq"  ) != std::string::npos ) result = true;
  else if( id.find("T5VV"  ) != std::string::npos ) result = true;

  return result;
}
#endif
