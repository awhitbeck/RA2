#include <TH2F.h>
#include <TFile.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <iostream>
#include "TStyle.h"
#include <ostream>
#include <TTree.h>
#include <TH1.h>
#include <TH3.h>
#include <TCanvas.h>


void MCEff_merging()
{
  gStyle->SetPaintTextFormat("5.2f"); 

  TFile* out = new TFile("MCEff.root","RECREATE");

  out->mkdir("Muon");
  out->mkdir("Electron");


//  out->mkdir("ttbar/Muon");
//  out->mkdir("ttbar/Electron");


  TDirectory *dMu = (TDirectory*)out->Get("Muon");
  TDirectory *dElec = (TDirectory*)out->Get("Electron");
  dMu->mkdir("IsoFitts");
  TDirectory *MuFit = (TDirectory*)dMu->Get("IsoFitts");


 // Open the input files

  TFile* fInput = TFile::Open("LostLeptonEffFromMC.root");
  TDirectory *dInput = (TDirectory*)fInput->Get("LostLeptonBkgMCEffCalculator");

 // new Histogramms for output

  TH1F *muonAccSum = new TH1F();
  TH1F *muonAccPassed = new TH1F();
  TH1F *muonAccSum2 = new TH1F();
  TH1F *muonAccEff2 = new TH1F();
  TH2F *muonAccSum3 = new TH2F();
  TH2F *muonAccEff3 = new TH2F();

  TH2F *muonRecoSum = new TH2F();
  TH2F *muonRecoEff = new TH2F();
  TH2F *muonRecoSum2 = new TH2F();
  TH2F *muonRecoEff2 = new TH2F();

  TH2F *muonIsoSum = new TH2F();
  TH2F *muonIsoEff = new TH2F();
  TH2F *muonIsoSum2 = new TH2F();
  TH2F *muonIsoEff2 = new TH2F();
  
  TH2F *MTW = new TH2F();


  TH1F *elecAccSum = new TH1F();
  TH1F *elecAccPassed = new TH1F();
  TH1F *elecAccSum2 = new TH1F();
  TH1F *elecAccEff2 = new TH1F();
  TH2F *elecAccSum3 = new TH2F();
  TH2F *elecAccEff3 = new TH2F();

  TH2F *elecRecoSum = new TH2F();
  TH2F *elecRecoEff = new TH2F();
  TH2F *elecRecoSum2 = new TH2F();
  TH2F *elecRecoEff2 = new TH2F();

  TH2F *elecIsoSum = new TH2F();
  TH2F *elecIsoEff = new TH2F();
  TH2F *elecIsoSum2 = new TH2F();
  TH2F *elecIsoEff2 = new TH2F();


  TH3F *muonBinByBinSum = new TH3F();
  TH3F *muonBinByBinEff = new TH3F();
  TH3F *muonBinByBinSum2 = new TH3F();
  TH3F *muonBinByBinEff2 = new TH3F();

  TH3F *elecBinByBinSum = new TH3F();
  TH3F *elecBinByBinEff = new TH3F();
  TH3F *elecBinByBinSum2 = new TH3F();
  TH3F *elecBinByBinEff2 = new TH3F();


 ///////////////////////////// Muon ///////////////////////////////

  // store the Acc in the new files and do the ratio

  muonAccSum = (TH1F*)dInput->Get("muonAccFailed");
  muonAccSum->SetName("MuonAccSum");
  muonAccSum->Sumw2();
  muonAccSum->Add( (TH1F*)dInput->Get("muonAccPassed") );

  muonAccPassed = (TH1F*)dInput->Get("muonAccPassed");
  muonAccPassed->SetName("MuonAccPassed");
  muonAccPassed->Sumw2();

  TGraphAsymmErrors *muonAccRatio = new TGraphAsymmErrors(muonAccPassed,muonAccSum);
  muonAccRatio->SetName("MuonAccRatio");
  muonAccRatio->SetTitle("MuonAccRatio;MuonPT;Ratio;ZAxLabel");


 // second version of acceptance eff
  muonAccSum2 = (TH1F*)dInput->Get("muonAccFailed2");
  muonAccSum2->SetName("MuonAccSum2");
  muonAccSum2->Sumw2();
  muonAccSum2->Add( (TH1F*)dInput->Get("muonAccPassed2") );

  muonAccEff2 = (TH1F*)dInput->Get("muonAccPassed2");
  muonAccEff2->SetName("MuonAccEff2");
  muonAccEff2->SetTitle("#mu acc. eff.;#slash{H}_{T};eff.");
  muonAccEff2->Sumw2();
  muonAccEff2->Divide(muonAccSum2);

std::cout<<"jup"<<std::endl;
 // third version of acceptance eff
  muonAccSum3 = (TH2F*)dInput->Get("MuonAccFailed3");
  muonAccSum3->SetName("MuonAccSum3");
  muonAccSum3->Sumw2();
  muonAccSum3->Add( (TH2F*)dInput->Get("MuonAccPassed3") );

  muonAccEff3 = (TH2F*)dInput->Get("MuonAccPassed3");
  muonAccEff3->SetName("MuonAccEff3");
  muonAccEff3->SetTitle("#mu acc. eff.;#slash{H}_{T};nJets");
  muonAccEff3->Sumw2();
  muonAccEff3->Divide(muonAccSum3);

std::cout<<"stts"<<std::endl;

  // the reconstruciton

  muonRecoSum = (TH2F*)dInput->Get("MuonRecoFailed");
  muonRecoSum->SetName("TotalNumberMuonsAfterAcc");
  muonRecoSum->Sumw2();
  muonRecoSum->Add( ( TH2F*)dInput->Get("MuonRecoPassed") );

  muonRecoEff = (TH2F*)dInput->Get("MuonRecoPassed");
  muonRecoEff->SetName("MuonRecoEff");
  muonRecoEff->SetTitle("MuonRecoEff;DeltaR;RelPT;Eff.");
  muonRecoEff->Sumw2();
  muonRecoEff->Divide(muonRecoSum);


  muonRecoSum2 = (TH2F*)dInput->Get("MuonRecoFailed2");
  muonRecoSum2->SetName("TotalNumberMuonsAfterAcc2");
  muonRecoSum2->Sumw2();
  muonRecoSum2->Add( ( TH2F*)dInput->Get("MuonRecoPassed2") );

  muonRecoEff2 = (TH2F*)dInput->Get("MuonRecoPassed2");
  muonRecoEff2->SetName("MuonRecoEff2");
  muonRecoEff2->SetTitle("#mu reco. eff.; #Delta R;#mu p_{T};Eff.");
  muonRecoEff2->Sumw2();
  muonRecoEff2->Divide(muonRecoSum2);


  // muon isolation

	//fitting

  
  
  muonIsoSum = (TH2F*)dInput->Get("muonIsoFailed");
  muonIsoSum->SetName("TotalNumberMuonsAfterReco");
  muonIsoSum->Sumw2();
  muonIsoSum->Add( ( TH2F*)dInput->Get("muonIsoPassed") );

  muonIsoEff = (TH2F*)dInput->Get("muonIsoPassed");
  muonIsoEff->SetName("MuonIsoEff");
  muonIsoEff->SetTitle("MuonIsoEff;#delta R;RelPT;Eff.");
  muonIsoEff->Sumw2();
  muonIsoEff->Divide(muonIsoSum);


  TTree * tree = (TTree*) dInput->Get("LostLeptonMCEff");

  const Int_t LepPtBins = muonIsoSum->GetNbinsY();

  TH1 *h1;
int ii=0;
  for (int i=1; i<LepPtBins;i++)
  {
   double down = muonIsoSum->GetYaxis()->GetBinLowEdge(i);
  double up = muonIsoSum->GetYaxis()->GetBinLowEdge(i+1);

TString low="";
low+=down;
TString high="";
high+=up;


  dInput->cd();
TCanvas *tc1= new TCanvas(low,low,1000,1000);
tc1->cd();
TH1 *h1= new TH1F("h1","h1",20,0,1); 
  tree->Draw("genDeltaR>>h1","(NJets >1 && nGenMu>0.2 && nIsoMu>0.5 && genDeltaR <1 && RecoLevelMuonRelPTJet>"+ low +" && RecoLevelMuonRelPTJet<"+high+")*Weight");
TH1 *h2= new TH1F("h2","h2",20,0,1);
  tree->Draw("genDeltaR>>h2","(NJets >1 && nGenMu>0.2 && nIsoMu<-0.5 && genDeltaR <1 && RecoLevelMuonRelPTJet>"+ low +" && RecoLevelMuonRelPTJet<"+high+")*Weight");
h2->Add(h1);
TH1 *hRatio= new TH1F("hRatio","hRatio",20,0,1);
  tree->Draw("genDeltaR>>hRatio","(NJets >1 && nGenMu>0.2 && nIsoMu>0.5 && genDeltaR <1 && RecoLevelMuonRelPTJet>"+ low +" && RecoLevelMuonRelPTJet<"+high+")*Weight");
hRatio->Divide(h2);
//h1->Fit("gaus");
tc1->Update();
std::cout<<"down"<<down<<", up"<<up<<std::endl;
std::cout<<"low"<<low<<", high"<<high<<std::endl;
low+="-";
low+=high;
low+=".eps";

tc1->SaveAs(low,"eps");
high="";
low="";




  }
/*

  const Int_t DeltaRBins = muonIsoSum->GetNbinsX();
  TH1 *h12;
  for (int i=1; i<DeltaRBins;i++)
  {

// second part
  double down2 = muonIsoSum->GetXaxis()->GetBinLowEdge(i);
  double up2 = muonIsoSum->GetXaxis()->GetBinLowEdge(i+1);

TString low2="";
low2+=down2;
TString high2="";
high2+=up2;


  dInput->cd();
TCanvas *tc12= new TCanvas(low2,low2,1000,1000);
tc12->cd();
TH1 *h12= new TH1F("h12","h12",30,0,3); 
  tree->Draw("RecoLevelMuonRelPTJet>>h12","(nGenMu>0.2 && nIsoMu>0.5 && RecoLevelMuonRelPTJet <3&& genDeltaR>"+ low2 +" && genDeltaR<"+high2+")*Weight");
TH1 *h22= new TH1F("h22","h22",30,0,3);
  tree->Draw("RecoLevelMuonRelPTJet>>h22","(nGenMu>0.2 && nIsoMu<-0.5 && RecoLevelMuonRelPTJet <3&& genDeltaR>"+ low2 +" && genDeltaR<"+high2+")*Weight");
h22->Add(h12);
TH1 *hRatio2= new TH1F("hRatio2","hRatio2",30,0,3);
  tree->Draw("RecoLevelMuonRelPTJet>>hRatio","(nGenMu>0.2 && nIsoMu>0.5 && RecoLevelMuonRelPTJet <3&& genDeltaR>"+ low2 +" && genDeltaR<"+high2+")*Weight");
hRatio2->Divide(h22);
//h1->Fit("gaus");
tc12->Update();
std::cout<<"down2"<<down2<<", up2"<<up2<<std::endl;
std::cout<<"low2"<<low2<<", high2"<<high2<<std::endl;
low2+="-";
low2+=high2;
low2+=".eps";

tc12->SaveAs(low2,"eps");
high2="";
low2="";

  }
*/




  muonIsoSum2 = (TH2F*)dInput->Get("muonIsoFailed2");
  muonIsoSum2->SetName("TotalNumberMuonsAfterReco2");
  muonIsoSum2->Sumw2();
  muonIsoSum2->Add( ( TH2F*)dInput->Get("muonIsoPassed2") );

  muonIsoEff2 = (TH2F*)dInput->Get("muonIsoPassed2");
  muonIsoEff2->SetName("MuonIsoEff2");
  muonIsoEff2->SetTitle("#mu iso. eff.; #Delta R;#mu p_{T};Eff.");
  muonIsoEff2->Sumw2();
  muonIsoEff2->Divide(muonIsoSum2);

  // MTW cut
  MTW = (TH2F*)dInput->Get("MTW");
  MTW->SetName("MTW");
  MTW->SetTitle("MTW;MTW;MHT;Eff.");
  MTW->Sumw2();


   //bin by bin eff
  muonBinByBinSum = (TH3F*)dInput->Get("BinByBinEffMuFailed");
  muonBinByBinSum->SetName("BinByBinEffMuFailed");
  muonBinByBinSum->Sumw2();
  muonBinByBinSum->Add( ( TH3F*)dInput->Get("BinByBinEffMuPassed") );

  muonBinByBinEff = (TH3F*)dInput->Get("BinByBinEffMuPassed");
  muonBinByBinEff->SetName("MuonBinByBinEff");
  muonBinByBinEff->SetTitle("#mu total eff; H_{T};#slash{H}_{T};NJet");
  muonBinByBinEff->Sumw2();
  muonBinByBinEff->Divide(muonBinByBinSum);





  dMu->cd();
muonBinByBinEff->Write();
  muonAccSum->Write();
  muonAccEff3->Write();
  MTW->Write();

  muonAccRatio->Write();
  muonAccEff2->Write();
  muonRecoEff->Write();
  muonRecoEff2->Write();
  muonIsoEff->Write();
  muonIsoEff2->Write();



  muonAccPassed->Write();
  muonRecoSum->Write();
  muonIsoSum->Write();
  muonRecoSum2->Write();
  muonIsoSum2->Write();

/////////////////////////// electron ////////////////////////

  // store the Acc ratio and the sum in the new files

  elecAccSum = (TH1F*)dInput->Get("elecAccFailed");
  elecAccSum->SetName("ElecAccSum");
  elecAccSum->Sumw2();
  elecAccSum->Add( (TH1F*)dInput->Get("elecAccPassed") );
  elecAccPassed = (TH1F*)dInput->Get("elecAccPassed");
  elecAccPassed->Sumw2();
  elecAccPassed->SetName("ElecAccPassed");

  TGraphAsymmErrors *elecAccRatio = new TGraphAsymmErrors(elecAccPassed,elecAccSum);
  elecAccRatio->SetName("ElecAccRatio");
  elecAccRatio->SetTitle("ElecAccRatio;ElecPT;Ratio;ZAX");

// second acc version
  elecAccSum2 = (TH1F*)dInput->Get("elecAccFailed2");
  elecAccSum2->SetName("ElecAccSum2");
  elecAccSum2->Sumw2();
  elecAccSum2->Add( (TH1F*)dInput->Get("elecAccPassed2") );
  elecAccEff2 = (TH1F*)dInput->Get("elecAccPassed2");
  elecAccEff2->SetName("ElecAccEff2");
  elecAccEff2->SetTitle("e acc. eff.;#slash{H}_{T};eff.");
  elecAccEff2->Sumw2();
  elecAccEff2->Divide(elecAccSum2);

 // third version of acceptance eff
  elecAccSum3 = (TH2F*)dInput->Get("ElecAccFailed3");
  elecAccSum3->SetName("ElecAccSum3");
  elecAccSum3->Sumw2();
  elecAccSum3->Add( (TH2F*)dInput->Get("ElecAccPassed3") );

  elecAccEff3 = (TH2F*)dInput->Get("ElecAccPassed3");
  elecAccEff3->SetName("ElecAccEff3");
  elecAccEff3->SetTitle("e acc. eff.;#slash{H}_{T};nJets");
  elecAccEff3->Sumw2();
  elecAccEff3->Divide(elecAccSum3);


  // elec reco eff
  elecRecoSum = (TH2F*)dInput->Get("elecIdFailed");
  elecRecoSum->SetName("TotalNumberElecAfterAcc");
  elecRecoSum->Sumw2();
  elecRecoSum->Add( ( TH2F*)dInput->Get("elecIdPassed") );

  elecRecoEff = (TH2F*)dInput->Get("elecIdPassed");
  elecRecoEff->SetName("ElecRecoEff");
  elecRecoEff->SetTitle("e reco. eff.; #Delta R;RelPT;Eff.");
  elecRecoEff->Sumw2();
  elecRecoEff->Divide(elecRecoSum);

 // second reco version
  elecRecoSum2 = (TH2F*)dInput->Get("elecIdFailed2");
  elecRecoSum2->SetName("TotalNumberElecAfterAcc2");
  elecRecoSum2->Sumw2();
  elecRecoSum2->Add( ( TH2F*)dInput->Get("elecIdPassed2") );

  elecRecoEff2 = (TH2F*)dInput->Get("elecIdPassed2");
  elecRecoEff2->SetName("ElecRecoEff2");
  elecRecoEff2->SetTitle("e reco. eff.; #Delta R;e p_{T};Eff.");
  elecRecoEff2->Sumw2();
  elecRecoEff2->Divide(elecRecoSum2);



  // elec iso eff
  elecIsoSum = (TH2F*)dInput->Get("elecIsoFailed");
  elecIsoSum->SetName("TotalNumberelecsAfterReco");
  elecIsoSum->Sumw2();
  elecIsoSum->Add( ( TH2F*)dInput->Get("elecIsoPassed") );

  elecIsoEff = (TH2F*)dInput->Get("elecIsoPassed");
  elecIsoEff->SetName("ElecIsoEff");
  elecIsoEff->SetTitle("e iso. eff.; #Delta R;RelPT;Eff.");
  elecIsoEff->Sumw2();
  elecIsoEff->Divide(elecIsoSum);

  elecIsoSum2 = (TH2F*)dInput->Get("elecIsoFailed2");
  elecIsoSum2->SetName("TotalNumberelecsAfterReco2");
  elecIsoSum2->Sumw2();
  elecIsoSum2->Add( ( TH2F*)dInput->Get("elecIsoPassed2") );

  elecIsoEff2 = (TH2F*)dInput->Get("elecIsoPassed2");
  elecIsoEff2->SetName("ElecIsoEff2");
  elecIsoEff2->SetTitle("e iso. eff.; #Delta R;e p_{T};Eff.");
  elecIsoEff2->Sumw2();
  elecIsoEff2->Divide(elecIsoSum2);

   //bin by bin eff
  elecBinByBinSum = (TH3F*)dInput->Get("BinByBinEffElecFailed");
  elecBinByBinSum->SetName("BinByBinEffElecFailed");
  elecBinByBinSum->Sumw2();
  elecBinByBinSum->Add( ( TH3F*)dInput->Get("BinByBinEffElecPassed") );

  elecBinByBinEff = (TH3F*)dInput->Get("BinByBinEffElecPassed");
  elecBinByBinEff->SetName("ElecBinByBinEff");
  elecBinByBinEff->SetTitle("elec total eff; H_{T};#slash{H}_{T};NJet");
  elecBinByBinEff->Sumw2();
  elecBinByBinEff->Divide(elecBinByBinSum);



  // write to file
  dElec->cd();
elecBinByBinEff->Write();
  elecAccRatio->Write();
  elecAccEff2->Write();
  elecAccEff3->Write();
  elecRecoEff->Write();
  elecRecoEff2->Write();
  elecIsoEff->Write();
  elecIsoEff2->Write();

  elecAccSum->Write();  
  elecAccPassed->Write();
  elecRecoSum->Write();
  elecIsoSum->Write();
  // close all files and write to output
  out->Write();
  out->Close();
  fInput->Close();
}
