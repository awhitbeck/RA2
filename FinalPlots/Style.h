#ifndef STYLE_H
#define STYLE_H

#include <string>
#include "TStyle.h"

// Style settings and labels
class Style {
public:
  static double markerSize() { return 0.95; }
  
  static std::string legendLabel(const std::string &id) {
    std::string label = id;
    if(      id == "Data"       ) label = "Data";
    else if( id == "QCD"        ) label = "QCD";
    else if( id == "LostLepton" ) label = "W/t#bar{t}(e/#mu+#nu)+jets";
    else if( id == "HadTau"     ) label = "W/t#bar{t}(#tau_{had}+#nu)+jets";
    else if( id == "ZInv"       ) label = "Z(#nu#bar{#nu})+jets";
    else if( id.find("T1tttt") != std::string::npos ) label = "T1tttt";
    else if( id.find("T1qqqq") != std::string::npos ) label = "T1qqqq";
    else if( id.find("T2qq"  ) != std::string::npos ) label = "T2qq";
    else if( id.find("T5VV"  ) != std::string::npos ) label = "T5VV";
  
    return label;
  }

  static int color(const std::string &id) {
    int col = kBlack;
    if(      id == "Data"       ) col = kBlack;
    else if( id == "QCD"        ) col = kRed+3;
    else if( id == "LostLepton" ) col = kRed+1;
    else if( id == "HadTau"     ) col = kYellow;
    else if( id == "ZInv"       ) col = kGreen+1;
    else if( id.find("T1tttt") != std::string::npos ) col = kBlue-2;
    else if( id.find("T1qqqq") != std::string::npos ) col = kBlue;
    else if( id.find("T2qq"  ) != std::string::npos ) col = kBlack;
    else if( id.find("T5VV"  ) != std::string::npos ) col = kMagenta+2;
  
    return col;
  }

  static void init() {
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
    gStyle->SetPadTopMargin(0.13);
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
    gStyle->SetTitleSize(0.045,"XYZ");
    gStyle->SetTitleXOffset(1.2);
    gStyle->SetTitleYOffset(2.2);
  
    //  For the legend
    gStyle->SetLegendBorderSize(0);
  }

};
#endif
