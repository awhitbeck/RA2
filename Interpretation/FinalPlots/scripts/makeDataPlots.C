{
  gROOT->Reset();

  TFile* fout = new TFile("SUS-13-012_DataHistograms.root","RECREATE");
  TFile* fin = new TFile("RA2Ntuple_Run2012-ABCv1ReReco-Cv2DPromptReco_V4.root","READ");
  gDirectory->cd("RA2TreeMaker");

  TH1* hHT    = new TH1D("hHT","",80,0,4000);
  TH1* hMHT   = new TH1D("hMHT","",40,0,2000);
  TH1* hNJets = new TH1D("hNJets","",12,0,12);

  TString sel = "HT>500 && MHT>200 && NJets>=3 && DeltaPhi1>0.5 && DeltaPhi2>0.5 && DeltaPhi3>0.3";
  RA2PreSelection->Draw("HT>>hHT",sel);
  RA2PreSelection->Draw("MHT>>hMHT",sel);
  RA2PreSelection->Draw("NJets>>hNJets",sel);
  fout->WriteTObject(hHT,"HT_NJetsInclusive");
  fout->WriteTObject(hMHT,"MHT_NJetsInclusive");
  fout->WriteTObject(hNJets,"NJets_NJetsInclusive");

  sel = "HT>500 && MHT>200 && NJets>=3 && NJets<=5 && DeltaPhi1>0.5 && DeltaPhi2>0.5 && DeltaPhi3>0.3";
  RA2PreSelection->Draw("HT>>hHT",sel);
  RA2PreSelection->Draw("MHT>>hMHT",sel);
  fout->WriteTObject(hHT,"HT_NJets3-5");
  fout->WriteTObject(hMHT,"MHT_NJets3-5");

  sel = "HT>500 && MHT>200 && NJets>=6 && NJets<=7 && DeltaPhi1>0.5 && DeltaPhi2>0.5 && DeltaPhi3>0.3";
  RA2PreSelection->Draw("HT>>hHT",sel);
  RA2PreSelection->Draw("MHT>>hMHT",sel);
  fout->WriteTObject(hHT,"HT_NJets6-7");
  fout->WriteTObject(hMHT,"MHT_NJets6-7");

  sel = "HT>500 && MHT>200 && NJets>=8 && DeltaPhi1>0.5 && DeltaPhi2>0.5 && DeltaPhi3>0.3";
  RA2PreSelection->Draw("HT>>hHT",sel);
  RA2PreSelection->Draw("MHT>>hMHT",sel);
  fout->WriteTObject(hHT,"HT_NJets8-Inf");
  fout->WriteTObject(hMHT,"MHT_NJets8-Inf");

  fin->Close();
  fout->Close();
}
