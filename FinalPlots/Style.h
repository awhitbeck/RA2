#ifndef STYLE_H
#define STYLE_H

#include <string>
#include "TStyle.h"

// Style settings and labels
class Style {
public:
  static void init();
  static std::string legendLabel(const std::string &id);
  static int color(const std::string &id);
};


std::string Style::legendLabel(const std::string &id) {
  std::string label = id;
  if(      id == "Data"       ) label = "Data";
  else if( id == "QCD"        ) label = "QCD";
  else if( id == "LostLepton" ) label = "W/t#bar{t} #rightarrow e/#mu+jets";
  else if( id == "HadTau"     ) label = "W/t#bar{t} #rightarrow #tau_{h}+jets";
  else if( id == "ZInv"       ) label = "Z #rightarrow #nu#bar{#nu}+jets";
  
  return label;
}

int Style::color(const std::string &id) {
  int col = kBlack;
  if(      id == "Data"       ) col = kBlack;
  else if( id == "QCD"        ) col = kRed+3;
  else if( id == "LostLepton" ) col = kRed+1;
  else if( id == "HadTau"     ) col = kYellow;
  else if( id == "ZInv"       ) col = kGreen+1;
  
  return col;
}

void Style::init() {
  gStyle->SetErrorX(0);
  
  //  For the canvas
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(800); //Height of canvas
  gStyle->SetCanvasDefW(800); //Width of canvas
  gStyle->SetCanvasDefX(0);   //Position on screen
  gStyle->SetCanvasDefY(0);
  
  //  For the frame
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(kBlack);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(kBlack);
  gStyle->SetFrameLineStyle(0);
  gStyle->SetFrameLineWidth(1);
  
  //  For the Pad
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);
  
  //  Margins
  gStyle->SetPadTopMargin(0.08);
  gStyle->SetPadBottomMargin(0.19);
  gStyle->SetPadLeftMargin(0.20);
  gStyle->SetPadRightMargin(0.07);
  
  //  For the histo:
  gStyle->SetHistLineColor(kBlack);
  gStyle->SetHistLineStyle(0);
  gStyle->SetHistLineWidth(2);
  gStyle->SetMarkerSize(1.4);
  gStyle->SetEndErrorSize(4);
  
  //  For the statistics box:
  gStyle->SetOptStat(0);
  
  //  For the axis
  gStyle->SetAxisColor(1,"XYZ");
  gStyle->SetTickLength(0.03,"XYZ");
  gStyle->SetNdivisions(510,"XYZ");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetStripDecimals(kFALSE);
  
  //  For the axis labels and titles
  gStyle->SetTitleColor(1,"XYZ");
  gStyle->SetLabelColor(1,"XYZ");
  gStyle->SetLabelFont(42,"XYZ");
  gStyle->SetLabelOffset(0.007,"XYZ");
  gStyle->SetLabelSize(0.045,"XYZ");
  gStyle->SetTitleFont(42,"XYZ");
  gStyle->SetTitleSize(0.06,"XYZ");
  gStyle->SetTitleXOffset(1.2);
  gStyle->SetTitleYOffset(1.7);
  
  //  For the legend
  gStyle->SetLegendBorderSize(0);
}

#endif
