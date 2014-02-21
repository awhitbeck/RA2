const TString baselineSel = "HT > 500 && MHT > 200 && NJets > 2 && DeltaPhi1 > 0.5 && DeltaPhi2 > 0.5 && DeltaPhi3 > 0.3";
const TString qcdSeedSel = "HT > 350 && NJets > 1";
const TString dijetSel = "HT>350 && NJets==2 && (DeltaPhi1 < 0.5 || DeltaPhi2 < 0.5)";
const TString cleaningSel = "Filter_HBHENoiseFilterRA2>0 && Filter_beamHaloFilter>0 && Filter_trackingFailureFilter>0 && Filter_inconsistentMuons>0 && Filter_greedyMuons>0 && Filter_ra2EcalTPFilter>0 && Filter_ra2EcalBEFilter>0 && Filter_hcalLaserEventFilter>0 && Filter_ecalLaserCorrFilter>0 && Filter_eeBadScFilter>0 && Filter_PBNRFilter>0 && Filter_HCALLaserEvtFilterList2012>0 && Filter_manystripclus53X<1 && Filter_toomanystripclus53X<1 && Filter_logErrorTooManyClusters<1";
const TString trigSel = "(Filter_HLTPFHT350PFMET100>0 || Filter_HLTPFNoPUHT350PFMET100>0)";
const TString trigSelQCDSeed = "(Filter_HLTPFHT350>0 || Filter_HLTPFHT650>0 || Filter_HLTPFNoPUHT350>0 || Filter_HLTPFNoPUHT650>0)";
const TString ratioRej = "CaloMHT/PFMHT<0.5";
const TString deltaPhiRej = "((TVector2::Phi_0_2pi(CaloMHTPhi-PFMHTPhi)>0.5*TMath::Pi() && TVector2::Phi_0_2pi(CaloMHTPhi-PFMHTPhi)<1.5*TMath::Pi()))";


void studyCaloVsPFMHT(const TString &fileName) {
  util::StyleSettings::setStylePresentationNoTitle();
  gStyle->SetOptStat(0);
  //plotMHTRatio2D(fileName,cleaningSel+"&&"+baselineSel,"CaloMHT/PFMHT");
  //plotMHTRatio2D(fileName,trigSelQCDSeed+"&&"+cleaningSel+"&&"+qcdSeedSel,"CaloMHT/PFMHT");
  //  plotMHTRatio2D(fileName,trigSelQCDSeed+"&&"+cleaningSel+"&&"+dijetSel,"CaloMHT/PFMHT");
  //plotMHTRatio2D(fileName,trigSel+"&&"+cleaningSel+"&&"+baselineSel,"CaloMHT/PFMHT");
  //plotMHTRatio2D(fileName,trigSel+"&&"+cleaningSel+"&&"+baselineSel,"CaloMHT/PFMHT");

  //plotMHTDeltaPhi2D(fileName,cleaningSel+"&&"+baselineSel,"CaloMHTPhi-PFMHTPhi");
  //  plotMHTDeltaPhi2D(fileName,trigSelQCDSeed+"&&"+cleaningSel+"&&"+qcdSeedSel,"CaloMHTPhi-PFMHTPhi");
  //  plotMHTDeltaPhi2D(fileName,trigSelQCDSeed+"&&"+cleaningSel+"&&"+dijetSel,"CaloMHTPhi-PFMHTPhi");
  //plotMHTDeltaPhi2D(fileName,trigSel+"&&"+cleaningSel+"&&"+baselineSel,"CaloMHTPhi-PFMHTPhi");
  //plotMHTDeltaPhi2D(fileName,ratioRej+"&&"+triggerSel+"&&"+cleaningSel+"&&"+baselineSel,"CaloMHTPhi-PFMHTPhi");
  //plotMHTDeltaPhi2D(fileName,deltaPhiRej+"&& !("+ratioRej+")&&"+triggerSel+"&&"+cleaningSel+"&&"+baselineSel,"CaloMHTPhi-PFMHTPhi");
  //plotMHTDeltaPhi2D(fileName,deltaPhiRej+"&&"+triggerSel+"&&"+cleaningSel+"&&"+baselineSel,"CaloMHTPhi-PFMHTPhi");

  //  plotEffVsMHT(fileName,cleaningSel+"&&"+baselineSel);
  //  plotEffVsMHTSig(fileName,cleaningSel+"&&"+baselineSel);
  //plotEffVsMHT(fileName,trigSel+"&&"+cleaningSel+"&&"+baselineSel);
  //plotEffVsMHTSig(fileName,trigSel+"&&"+cleaningSel+"&&"+baselineSel);
//   plotEffVsMHT(fileName,trigSelQCDSeed+"&&"+cleaningSel+"&&"+qcdSeedSel);
//   plotEffVsMHTSig(fileName,trigSelQCDSeed+"&&"+cleaningSel+"&&"+qcdSeedSel);
  plotEffVsMHT(fileName,trigSelQCDSeed+"&&"+cleaningSel+"&&"+dijetSel);
  plotEffVsMHTSig(fileName,trigSelQCDSeed+"&&"+cleaningSel+"&&"+dijetSel);


  //plotTriggers(fileName,cleaningSel+"&&"+baselineSel,"CaloMHT/PFMHT");
}

void plotMHTRatio2D(const TString& fileName, const TString &sel, const TString &metRatio) {
  std::cout << "Processing plotMHTRatio2D for '" << metRatio << "'" << std::endl;
  
  std::cout << "  Opening file" << std::endl;
  TChain* chain = new TChain("RA2TreeMaker/RA2PreSelection");
  chain->Add(fileName);


  std::cout << "  Plotting ratio vs MHT" << std::endl;
  TCanvas* canH2 = new TCanvas("canH2"+metRatio,"canH2 "+metRatio,500,500);
  canH2->cd();
  canH2->SetBottomMargin(canH2->GetBottomMargin()+0.05);
  canH2->SetRightMargin(canH2->GetRightMargin()+0.1);
  TH2* h2 = new TH2D("h2","",30,0,1500,100,0,2.);
  h2->GetYaxis()->SetTitle(metRatio);
  if( metRatio.Contains("MHT") ) h2->GetYaxis()->SetTitle("#slash{H}_{T}(Calo) / #slash{H}_{T}(PF)");
  h2->GetXaxis()->SetTitle("#slash{H}_{T} [GeV]");
  chain->Draw(metRatio+":MHT>>h2",sel,"colz");
  gPad->SetLogz();
  h2->SetName("h2"+metRatio);

  TLine* cutOnMetRatio = new TLine(h2->GetXaxis()->GetBinLowEdge(1),0.5,h2->GetXaxis()->GetBinUpEdge(h2->GetNbinsX()),0.5);
  cutOnMetRatio->SetLineWidth(5);
  cutOnMetRatio->SetLineColor(kBlack);
  cutOnMetRatio->SetLineStyle(2);
  cutOnMetRatio->Draw("same");

  std::cout << "  Selected " << h2->GetEntries() << " events" << std::endl;

  std::cout << "  Plotting ratio vs MHT/sqrt(HT)" << std::endl;
  TCanvas* canH3 = new TCanvas("canH3"+metRatio,"canH3 "+metRatio,500,500);
  canH3->cd();
  canH3->SetBottomMargin(canH3->GetBottomMargin()+0.05);
  canH3->SetRightMargin(canH3->GetRightMargin()+0.1);
  TH2* h3 = new TH2D("h3","",20,0,40,100,0,2.);
  h3->GetYaxis()->SetTitle(metRatio);
  if( metRatio.Contains("MHT") ) h3->GetYaxis()->SetTitle("#slash{H}_{T}(Calo) / #slash{H}_{T}(PF)");
  h3->GetXaxis()->SetTitle("#slash{H}_{T}/#sqrt{H_{T}} [#sqrt{GeV}]");
  chain->Draw(metRatio+":MHT/TMath::Sqrt(HT)>>h3",sel,"colz");
  gPad->SetLogz();
  h3->SetName("h3"+metRatio);

  TLine* cutOnMetRatio3 = new TLine(h3->GetXaxis()->GetBinLowEdge(1),0.5,h3->GetXaxis()->GetBinUpEdge(h3->GetNbinsX()),0.5);
  cutOnMetRatio3->SetLineWidth(5);
  cutOnMetRatio3->SetLineColor(kBlack);
  cutOnMetRatio3->SetLineStyle(2);
  cutOnMetRatio3->Draw("same");

  cutOnMetRatio3->Draw("same");

  std::cout << "  Selected " << h3->GetEntries() << " events" << std::endl;


  TString outName = metRatio+"_vs_MHT";
  outName.ReplaceAll("/","-");
  canH2->SaveAs(outName+".eps","eps");
  canH3->SaveAs(outName+"Sig.eps","eps");
}



void plotMHTDeltaPhi2D(const TString& fileName, const TString &sel, const TString &metDeltaPhi) {
  std::cout << "Processing plotMHTDeltaPhi2D for '" << metDeltaPhi << "'" << std::endl;
  
  std::cout << "  Opening file" << std::endl;
  TChain* chain = new TChain("RA2TreeMaker/RA2PreSelection");
  chain->Add(fileName);

  std::cout << "  Plotting deltaPhi vs MHT" << std::endl;
  TCanvas* canH2 = new TCanvas("canH2"+metDeltaPhi,"canH2 "+metDeltaPhi,500,500);
  canH2->cd();
  canH2->SetBottomMargin(canH2->GetBottomMargin()+0.05);
  canH2->SetRightMargin(canH2->GetRightMargin()+0.1);
  TH2* h2 = new TH2D("h2","",30,0,1500,62,0,6.2);
  h2->GetYaxis()->SetTitle(metDeltaPhi);
  h2->GetXaxis()->SetTitle("#slash{H}_{T} [GeV]");
  if( metDeltaPhi.Contains("MHT") ) h2->GetYaxis()->SetTitle("#Delta#phi(#slash{H}_{T}(Calo),#slash{H}_{T}(PF))");
  chain->Draw("TVector2::Phi_0_2pi("+metDeltaPhi+"):MHT>>h2",sel,"colz");
  gPad->SetLogz();
  h2->SetName("h2"+metDeltaPhi);

  TLine* cutOnMetDeltaPhi1 = new TLine(h2->GetXaxis()->GetBinLowEdge(1),0.5*TMath::Pi(),h2->GetXaxis()->GetBinUpEdge(h2->GetNbinsX()),0.5*TMath::Pi());
  cutOnMetDeltaPhi1->SetLineWidth(5);
  cutOnMetDeltaPhi1->SetLineColor(kBlack);
  cutOnMetDeltaPhi1->SetLineStyle(2);
  cutOnMetDeltaPhi1->Draw("same");
  TLine* cutOnMetDeltaPhi2 = new TLine(h2->GetXaxis()->GetBinLowEdge(1),1.5*TMath::Pi(),h2->GetXaxis()->GetBinUpEdge(h2->GetNbinsX()),1.5*TMath::Pi());
  cutOnMetDeltaPhi2->SetLineWidth(5);
  cutOnMetDeltaPhi2->SetLineColor(kBlack);
  cutOnMetDeltaPhi2->SetLineStyle(2);
  cutOnMetDeltaPhi2->Draw("same");

  std::cout << "  Selected " << h2->GetEntries() << " events" << std::endl;


  std::cout << "  Plotting deltaPhi vs MHT/sqrt(HT)" << std::endl;
  TCanvas* canH3 = new TCanvas("canH3"+metDeltaPhi,"canH3 "+metDeltaPhi,500,500);
  canH3->cd();
  canH3->SetBottomMargin(canH3->GetBottomMargin()+0.05);
  canH3->SetRightMargin(canH3->GetRightMargin()+0.1);
  TH2* h3 = new TH2D("h3","",20,0,40,62,0,6.2);
  h3->GetYaxis()->SetTitle(metDeltaPhi);
  h3->GetXaxis()->SetTitle("#slash{H}_{T}/#sqrt{H_{T}} [#sqrt{GeV}]");
  if( metDeltaPhi.Contains("MHT") ) h3->GetYaxis()->SetTitle("#Delta#phi(#slash{H}_{T}(Calo),#slash{H}_{T}(PF))");
  chain->Draw("TVector2::Phi_0_2pi("+metDeltaPhi+"):MHT/sqrt(HT)>>h3",sel,"colz");
  gPad->SetLogz();
  h3->SetName("h2"+metDeltaPhi);

  TLine* cutOnMetDeltaPhi31 = new TLine(h3->GetXaxis()->GetBinLowEdge(1),0.5*TMath::Pi(),h3->GetXaxis()->GetBinUpEdge(h3->GetNbinsX()),0.5*TMath::Pi());
  cutOnMetDeltaPhi31->SetLineWidth(5);
  cutOnMetDeltaPhi31->SetLineColor(kBlack);
  cutOnMetDeltaPhi31->SetLineStyle(2);
  cutOnMetDeltaPhi31->Draw("same");
  TLine* cutOnMetDeltaPhi32 = new TLine(h3->GetXaxis()->GetBinLowEdge(1),1.5*TMath::Pi(),h3->GetXaxis()->GetBinUpEdge(h3->GetNbinsX()),1.5*TMath::Pi());
  cutOnMetDeltaPhi32->SetLineWidth(5);
  cutOnMetDeltaPhi32->SetLineColor(kBlack);
  cutOnMetDeltaPhi32->SetLineStyle(2);
  cutOnMetDeltaPhi32->Draw("same");

  std::cout << "  Selected " << h3->GetEntries() << " events" << std::endl;

  TString outName = metDeltaPhi+"_vs_MHT";
  canH2->SaveAs(outName+".eps","eps");
  canH3->SaveAs(outName+"Sig.eps","eps");
}


void plotEffVsMHT(const TString& fileName, const TString &sel) {
  std::cout << "Processing plotEffVsMHT" << std::endl;
  
  std::cout << "  Opening file" << std::endl;
  TChain* chain = new TChain("RA2TreeMaker/RA2PreSelection");
  chain->Add(fileName);

  TH1* h1 = new TH1D("h1","",17,0,1700);
  h1->GetYaxis()->SetTitle("Entries");
  h1->GetXaxis()->SetTitle("#slash{H}_{T} [GeV]");
  TH1* h2 = static_cast<TH1*>(h1->Clone("h2"));
  TH1* hEff = static_cast<TH1*>(h1->Clone("hEffMHT"));

  std::cout << "  Plotting eff vs MHT" << std::endl;
  TCanvas* can1 = new TCanvas("canBeforeMHT","before MHT",500,500);
  can1->cd();
  chain->Draw("MHT>>h1",sel,"HIST");
  gPad->SetLogy();
  h1->SetName("hBeforeMHT");

  TCanvas* can2 = new TCanvas("canRejectedMHT","after MHT",500,500);
  can2->cd();
  chain->Draw("MHT>>h2",sel+"&& ("+ratioRej+"||"+deltaPhiRej+")","HIST");
  gPad->SetLogy();
  h2->SetName("hRejectedMHT");

  hEff->GetYaxis()->SetRangeUser(0.0001,1.);
  hEff->GetYaxis()->SetTitle("#epsilon");

  // Last 2 bins for total eff
  for(int n = 0; n < h1->Integral(1,h1->GetNbinsX()-2); ++n) {
    h1->Fill(h1->GetBinCenter(h1->GetNbinsX()));
  }
  for(int n = 0; n < h2->Integral(1,h2->GetNbinsX()-2); ++n) {
    h2->Fill(h2->GetBinCenter(h2->GetNbinsX()));
  }
 
 
  TGraphAsymmErrors* gEff = new TGraphAsymmErrors(h2,h1,"cp");
  gEff->SetMarkerStyle(20);
  TCanvas* can3 = new TCanvas("canEffMHT","eff MHT",500,500);
  can3->cd();
  hEff->Draw();
  gEff->Draw("PE1same");
  gPad->SetLogy();


  std::cout << "    N(Before) = " << h1->GetBinContent(h1->GetNbinsX()) << std::endl;
  std::cout << "  N(Rejected) = " << h2->GetBinContent(h2->GetNbinsX()) << std::endl;
  std::cout << "   Efficiency = " << gEff->GetY()[gEff->GetN()-1] << std::endl;
  for(int bin = 1; bin < h1->GetNbinsX(); ++bin) {
    std::cout << "     MHT: " << h2->GetXaxis()->GetBinLowEdge(bin) << " - " << h2->GetXaxis()->GetBinUpEdge(bin) << " \t: " << h2->GetBinContent(bin) << std::endl;
  }


  can1->SaveAs("All_vs_MHT.eps","eps");
  can2->SaveAs("Rejected_vs_MHT.eps","eps");
  can3->SaveAs("Eff_vs_MHT.eps","eps");
}


void plotEffVsMHTSig(const TString& fileName, const TString &sel) {
  std::cout << "Processing plotEffVsMHTSig" << std::endl;
  
  std::cout << "  Opening file" << std::endl;
  TChain* chain = new TChain("RA2TreeMaker/RA2PreSelection");
  chain->Add(fileName);

  TH1* h1 = new TH1D("h1","",22,0,44);
  h1->GetYaxis()->SetTitle("Entries");
  h1->GetXaxis()->SetTitle("#slash{H}_{T}/#sqrt{H_{T}} [#sqrt{GeV}]");
  TH1* h2 = static_cast<TH1*>(h1->Clone("h2"));
  TH1* hEff = static_cast<TH1*>(h1->Clone("hEffMHTSig"));

  std::cout << "  Plotting eff vs MHT" << std::endl;
  TCanvas* can1 = new TCanvas("canBeforeMHTSig","before MHTSig",500,500);
  can1->cd();
  chain->Draw("MHT/TMath::Sqrt(HT)>>h1",sel,"HIST");
  gPad->SetLogy();
  h1->SetName("hBeforeMHTSig");

  TCanvas* can2 = new TCanvas("canRejectedMHTSig","rejected MHTSig",500,500);
  can2->cd();
  chain->Draw("MHT/TMath::Sqrt(HT)>>h2",sel+"&& ("+ratioRej+"||"+deltaPhiRej+")","HIST");
  gPad->SetLogy();
  h2->SetName("hRejectedMHTSig");

  hEff->GetYaxis()->SetRangeUser(0.0001,1.);
  hEff->GetYaxis()->SetTitle("#epsilon");

  // Last 2 bins for total eff
  for(int n = 0; n < h1->Integral(1,h1->GetNbinsX()-2); ++n) {
    h1->Fill(h1->GetBinCenter(h1->GetNbinsX()));
  }
  for(int n = 0; n < h2->Integral(1,h2->GetNbinsX()-2); ++n) {
    h2->Fill(h2->GetBinCenter(h2->GetNbinsX()));
  }
 
 
  TGraphAsymmErrors* gEff = new TGraphAsymmErrors(h2,h1,"cp");
  gEff->SetMarkerStyle(20);
  TCanvas* can3 = new TCanvas("canEffMHTSig","eff MHTSig",500,500);
  can3->cd();
  hEff->Draw();
  gEff->Draw("PE1same");
  gPad->SetLogy();


  std::cout << "    N(Before) = " << h1->GetBinContent(h1->GetNbinsX()) << std::endl;
  std::cout << "  N(Rejected) = " << h2->GetBinContent(h2->GetNbinsX()) << std::endl;
  std::cout << "   Efficiency = " << gEff->GetY()[gEff->GetN()-1] << std::endl;
  for(int bin = 1; bin < h1->GetNbinsX(); ++bin) {
    std::cout << "     MHT/sqrt(HT): " << h2->GetXaxis()->GetBinLowEdge(bin) << " - " << h2->GetXaxis()->GetBinUpEdge(bin) << " \t: " << h2->GetBinContent(bin) << std::endl;
  }


  can1->SaveAs("All_vs_MHTSig.eps","eps");
  can2->SaveAs("Rejected_vs_MHTSig.eps","eps");
  can3->SaveAs("Eff_vs_MHTSig.eps","eps");
}


void plotTriggers(const TString& fileName, const TString &sel, const TString &metRatio) {

  TString hlts[100];
  for(int i = 0; i < 100; ++i) {
    hlts[i] = "";
  }
  hlts[0] = "Filter_HLTCleanPFHT300Ele15CaloIdTCaloIsoVLTrkIdTTrkIsoVLPFMET45";
  hlts[1] = "Filter_HLTCleanPFHT300Ele15CaloIdTCaloIsoVLTrkIdTTrkIsoVLPFMET50";
  hlts[2] = "Filter_HLTCleanPFHT300Ele40CaloIdVTTrkIdT";
  hlts[3] = "Filter_HLTCleanPFHT300Ele60CaloIdVTTrkIdT";
  hlts[4] = "Filter_HLTCleanPFHT350Ele5CaloIdTCaloIsoVLTrkIdTTrkIsoVLPFMET45";
  hlts[5] = "Filter_HLTCleanPFHT350Ele5CaloIdTCaloIsoVLTrkIdTTrkIsoVLPFMET50";
  hlts[6] = "Filter_HLTFatDiPFJetMass750DR1p1Deta1p5";
  hlts[7] = "Filter_HLTHT200AlphaT0p57";
  hlts[8] = "Filter_HLTHT200";
  hlts[9] = "Filter_HLTHT250AlphaT0p55";
  hlts[10] = "Filter_HLTHT250AlphaT0p57";
  hlts[11] = "Filter_HLTHT250DoubleDisplacedPFJet60ChgFraction10";
  hlts[12] = "Filter_HLTHT250DoubleDisplacedPFJet60";
  hlts[13] = "Filter_HLTHT250SingleDisplacedPFJet60ChgFraction10";
  hlts[14] = "Filter_HLTHT250SingleDisplacedPFJet60";
  hlts[15] = "Filter_HLTHT250";
  hlts[16] = "Filter_HLTHT300AlphaT0p53";
  hlts[17] = "Filter_HLTHT300AlphaT0p54";
  hlts[18] = "Filter_HLTHT300DoubleDisplacedPFJet60ChgFraction10";
  hlts[19] = "Filter_HLTHT300DoubleDisplacedPFJet60";
  hlts[20] = "Filter_HLTHT300SingleDisplacedPFJet60";
  hlts[21] = "Filter_HLTHT300SingleDisplacedPFJet60";
  hlts[22] = "Filter_HLTHT300";
  hlts[23] = "Filter_HLTHT350AlphaT0p52";
  hlts[24] = "Filter_HLTHT350AlphaT0p53";
  hlts[25] = "Filter_HLTHT350";
  hlts[26] = "Filter_HLTHT400AlphaT0p51";
  hlts[27] = "Filter_HLTHT400AlphaT0p52";
  hlts[28] = "Filter_HLTHT400";
  hlts[29] = "Filter_HLTHT450AlphaT0p51";
  hlts[30] = "Filter_HLTHT450";
  hlts[31] = "Filter_HLTHT500";
  hlts[32] = "Filter_HLTHT550";
  hlts[33] = "Filter_HLTHT650";
  hlts[34] = "Filter_HLTHT750";
  hlts[35] = "Filter_HLTPFHT350PFMET100";
  hlts[36] = "Filter_HLTPFHT350";
  hlts[37] = "Filter_HLTPFHT400PFMET100";
  hlts[38] = "Filter_HLTPFHT650DiCentralPFJet80CenPFJet40";
  hlts[39] = "Filter_HLTPFHT650";
  hlts[40] = "Filter_HLTPFHT700";
  hlts[41] = "Filter_HLTPFHT750";
  hlts[42] = "Filter_HLTRsqMR40Rsq0p04";
  hlts[43] = "Filter_HLTRsqMR45Rsq0p09";
  hlts[44] = "Filter_HLTRsqMR55Rsq0p09MR150";
  hlts[45] = "Filter_HLTRsqMR60Rsq0p09MR150";
  hlts[46] = "Filter_HLTRsqMR65Rsq0p09MR150";
  hlts[47] = "Filter_HLTHT250AlphaT0p55";
  hlts[48] = "Filter_HLTHT250AlphaT0p57";
  hlts[49] = "Filter_HLTHT300AlphaT0p53";
  hlts[50] = "Filter_HLTHT300AlphaT0p54";
  hlts[51] = "Filter_HLTHT350AlphaT0p52";
  hlts[52] = "Filter_HLTHT350AlphaT0p53";
  hlts[53] = "Filter_HLTHT400AlphaT0p51";
  hlts[54] = "Filter_HLTHT400AlphaT0p52";
  hlts[55] = "Filter_HLTHT450AlphaT0p51";
  hlts[56] = "Filter_HLTPFNoPUHT350PFMET100";
  hlts[57] = "Filter_HLTPFNoPUHT400PFMET100";
  hlts[58] = "Filter_HLTRsqMR40Rsq0p04";
  hlts[59] = "Filter_HLTRsqMR55Rsq0p09MR150";
  hlts[60] = "Filter_HLTRsqMR60Rsq0p09MR150";
  hlts[61] = "Filter_HLTRsqMR65Rsq0p09MR150";
  hlts[62] = "Filter_DiPFJetAve320";
  hlts[63] = "Filter_DiPFJetAve400";
  hlts[64] = "Filter_FatDiPFJetMass750DR1p1Deta1p5";
  hlts[65] = "Filter_HT200";
  hlts[66] = "Filter_HT250";
  hlts[67] = "Filter_HT300DoubleDisplacedPFJet60ChgFraction10";
  hlts[68] = "Filter_HT300DoubleDisplacedPFJet60";
  hlts[69] = "Filter_HT300SingleDisplacedPFJet60";
  hlts[70] = "Filter_HT300SingleDisplacedPFJet60";
  hlts[71] = "Filter_HT300";
  hlts[72] = "Filter_HT350";
  hlts[73] = "Filter_HT400";
  hlts[74] = "Filter_HT450";
  hlts[75] = "Filter_HT500";
  hlts[76] = "Filter_HT550";
  hlts[77] = "Filter_HT650Track50dEdx3p6";
  hlts[78] = "Filter_HT650Track60dEdx3p7";
  hlts[79] = "Filter_HT650";
  hlts[80] = "Filter_HT750";
  hlts[81] = "Filter_Jet370NoJetID";
  hlts[82] = "Filter_MET80Track50dEdx3p6";
  hlts[83] = "Filter_MET80Track60dEdx3p7";
  hlts[84] = "Filter_MET80";
  hlts[85] = "Filter_PFJet320";
  hlts[86] = "Filter_PFJet400";
  hlts[87] = "Filter_PFNoPUHT350";
  hlts[88] = "Filter_PFNoPUHT650DiCentralPFNoPUJet80CenPFNoPUJet40";
  hlts[89] = "Filter_PFNoPUHT650";
  hlts[90] = "Filter_PFNoPUHT700";
  hlts[91] = "Filter_PFNoPUHT750";


  const int nHLT = 92;

  std::cout << "Processing plotTriggers for '" << metRatio << "'" << std::endl;
  
  std::cout << "  Opening file" << std::endl;
  TChain* chain = new TChain("RA2TreeMaker/RA2PreSelection");
  chain->Add(fileName);

  TH1* hEff = new TH1D("hEff","",nHLT,-0.5,nHLT-0.5);
  TH1* h1 = new TH1D("h1","",26,200,1500);
  TH1* h2 = static_cast<TH1*>(h1->Clone("h2"));
  std::cout << "  Plotting eff vs MHT" << std::endl;
  for(int i = 0; i < nHLT; ++i) {
    chain->Draw("MHT>>h1",sel+"&&"+hlts[i]+">0","goff");
    chain->Draw("MHT>>h2",sel+"&&"+hlts[i]+">0"+"&&"+metRatio+"<0.5","goff");

    //     std::cout << "    " << hlts[i] << std::endl;
    //     std::cout << "      " << h1->Integral() << std::endl;
    //     std::cout << "      " << h2->Integral() << std::endl;

    if( h1->Integral() ) {
      hEff->SetBinContent(1+i,h2->Integral()/h1->Integral());
    }
  }    


  TCanvas* can1 = new TCanvas("canTriggers"+metRatio,"Triggers "+metRatio,500,500);
  can1->cd();
  hEff->Draw();


  for(int i = 0; i < nHLT; ++i) {
    std::cout << "    " << i << "\t: " << hlts[i] << std::endl;
  }
}
