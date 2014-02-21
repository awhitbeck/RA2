const TString baselineSel = "HT > 500 && MHT > 200 && NJets > 2 && DeltaPhi1 > 0.5 && DeltaPhi2 > 0.5 && DeltaPhi3 > 0.3";
const TString cleaningSel = "Filter_HBHENoiseFilterRA2>0 && Filter_beamHaloFilter>0 && Filter_trackingFailureFilter>0 && Filter_inconsistentMuons>0 && Filter_greedyMuons>0 && Filter_ra2EcalTPFilter>0 && Filter_ra2EcalBEFilter>0 && Filter_hcalLaserEventFilter>0 && Filter_ecalLaserCorrFilter>0 && Filter_eeBadScFilter>0 && Filter_HCALLaserEvtFilterList2012>0 && Filter_manystripclus53X<1 && Filter_toomanystripclus53X<1 && Filter_logErrorTooManyClusters<1 && Filter_PBNRFilter>0";


void plotNtuples(const TString &fileName) {
  //util::StyleSettings::setStylePresentationNoTitle();
  //  gStyle->SetOptStat(0);

  TChain* chain = new TChain("RA2TreeMaker/RA2PreSelection");
  chain->Add(fileName);

  chain->Draw("MHT",baselineSel+"&&"+cleaningSel);

  //chain->SetScanField(0);
  //chain->Scan("RunNum:LumiBlockNum:EvtNum:MHT:HT",baselineSel+"&&"+cleaningSel,"colsize=12");
}


