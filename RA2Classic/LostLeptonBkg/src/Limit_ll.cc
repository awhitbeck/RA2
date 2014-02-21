// -*- C++ -*-
//
// Package:    Limit_ll
// Class:      Limit_ll
// 
/**\class Limit_ll Limit_ll.cc RA2Classic/Limit_ll/src/Limit_ll.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Arne-Rasmus Draeger,68/111,4719,
//         Created:  Fri Mar  1 14:44:12 CET 2013
// $Id: Limit_ll.cc,v 1.3 2013/04/18 11:48:22 adraeger Exp $
//
//


// system include files
#include "RA2Classic/LostLeptonBkg/interface/Limit_ll.h"
#include "DataFormats/Math/interface/deltaPhi.h"
//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
Limit_ll::Limit_ll(const edm::ParameterSet& iConfig)
{

   EfficiencyFileName_ = iConfig.getParameter <std::string> ("EfficiencyFileName");
   nTupleName_ = iConfig.getParameter <std::string> ("NTupleName");
   Path_ = iConfig.getParameter <std::string> ("Path");
   MTWMax_ = iConfig.getParameter <double> ("MTWMax");
   MTWCut_ = iConfig.getParameter <bool> ("MTWCut");

	// lost lepton input

   DiLepCorrection_ = iConfig.getParameter <double> ("DiLepCorrection");
   DiLepCorrectionUp_ = iConfig.getParameter <double> ("DiLepCorrectionUp");
   DiLepCorrectionDown_ = iConfig.getParameter <double> ("DiLepCorrectionDown");


   //register your products
/* Examples
   produces<ExampleData2>();

   //if do put with a label
   produces<ExampleData2>("label");
 
   //if you want to put into the Run
   produces<ExampleData2,InRun>();
*/
   //now do what ever other initialization is needed
  
}


Limit_ll::~Limit_ll()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
Limit_ll::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
//	std::cout<<"Producer started"<<std::endl;
//	std::cout<<"Path_"<<Path_<<std::endl;
//	std::cout<<"nTupleName_"<<nTupleName_<<std::endl;

    	//open the input tree from the input file
   TChain* c = new TChain(Path_);
  TString fileName("");
  fileName += nTupleName_;
	std::cout<<"fileName"<<fileName<<std::endl;
   c->Add(fileName);
//	std::cout<<"chainLoaded"<<std::endl;

   	// load the variables
	Float_t ht_ =0;
	c->SetBranchAddress("HT",&ht_);
	Float_t mht_ =0;
	c->SetBranchAddress("MHT",&mht_);
	UShort_t nJets_ =0;
	c->SetBranchAddress("NJets",&nJets_);
	UShort_t NVtx =0;
	c->SetBranchAddress("NVtx",&NVtx);
	eventWeight_ =0;
	c->SetBranchAddress("Weight",&eventWeight_);
	UShort_t nMu_ = 0;
	c->SetBranchAddress("PATMuonsPFIDIsoNum",&nMu_);
	Float_t MuPt_[50];
	c->SetBranchAddress("PATMuonsPFIDIsoPt",&MuPt_);

	Float_t MuPhi_[50];
	c->SetBranchAddress("PATMuonsPFIDIsoPhi",&MuPhi_);

	UShort_t nElec_ = 0;
	c->SetBranchAddress("PATElectronsIDIsoNum",&nElec_);

	Float_t MetPt_[50];
	c->SetBranchAddress("PATMETsPFPt",&MetPt_);

	Float_t MetPhi_[50];
	c->SetBranchAddress("PATMETsPFPhi",&MetPhi_);


//	std::cout<<"Variables loaded"<<std::endl;
//	std::cout<<"c->GetEntries()"<<c->GetEntries()<<std::endl;	
	for(int ii = 0; ii < c->GetEntries(); ++ii) 
	{
  		c->GetEntry(ii);
	float MetPt= MetPt_[0];
	float MuPhi=MuPhi_[0];
	float MuPt = MuPt_[0];
	float MetPhi= MetPhi_[0];

//		std::cout<<"HT"<<ht_<<std::endl;
//if(nMu_>1)		std::cout<<"nMu_"<<nMu_;
//if(nElec_>0)		std::cout<<"nElec_"<<nElec_<<std::endl;


//	if(nMu_==1 && nElec_==0 && mtw_ && ht_>500 && mht_>200)
if ( ht_>500 && mht_>200 && nJets_>2 && nMu_==1 && nElec_==0)

{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//std::cout<<"nMu_"<<nMu_;
//std::cout<<", MuPhi"<<MuPhi;
//std::cout<<", MetPhi"<<MetPhi;
//std::cout<<", MuPt"<<MuPt;
//std::cout<<", MetPt"<<MetPt;

    double deltaPhi =reco::deltaPhi(MuPhi, MetPhi);
//std::cout<<"sqrt(2*MuPt_*MetPt*(1-cos(deltaPhi)) )"<<sqrt(2*MuPt*MetPt*(1-cos(deltaPhi)) )<<std::endl;
 if(    sqrt(2*MuPt*MetPt*(1-cos(deltaPhi)) )<100)
{







//	std::cout<<"eventWeight_"<<eventWeight_<<std::endl;
//	std::cout<<"nMu_"<<nMu_<<std::endl;
//	std::cout<<"nElec_"<<nElec_<<std::endl;
//	std::cout<<"nJets_"<<nJets_<<std::endl;
//	std::cout<<"mht_"<<mht_<<std::endl;
//	std::cout<<"ht_"<<ht_<<std::endl;
 	double muAccXmax = MuonAccEff3_->GetXaxis()->GetXmax();
//	std::cout<<"muAccXmax"<<muAccXmax<<std::endl;
 	double muAccYmax = MuonAccEff3_->GetYaxis()->GetXmax();
//	std::cout<<"muAccYmax"<<muAccYmax<<std::endl;
	double mht=mht_;
	if ( muAccXmax<mht_) mht=muAccXmax-1;
	int muonAccXaxis3= MuonAccEff3_->GetXaxis()->FindBin(mht);
	int nJets=nJets_;
	if (muAccYmax<nJets_) nJets=muAccYmax-0.2;
	int muonAccYaxis3= MuonAccEff3_->GetYaxis()->FindBin(nJets+0.1);

	muonAccEff2_ = MuonAccEff3_->GetBinContent(muonAccXaxis3,muonAccYaxis3);


	int muonBinByBinIsoXaxis=0;
	int muonBinByBinIsoYaxis=0;
	int muonBinByBinRecoXaxis=0;
	int muonBinByBinRecoYaxis=0;
	double muonBinByBinRecoEffError = 0; 


	if (nJets_ > 2.5 && 5.5 > nJets_)
	{
	 	double muIsoXmax = MuonIsoBinByBinEff35_->GetXaxis()->GetXmax();
		double ht = ht_;
		if (muIsoXmax<ht_)ht= muIsoXmax-1;
 		double muIsoYmax = MuonIsoBinByBinEff35_->GetYaxis()->GetXmax();
		double mht = mht_;
		if (muIsoYmax<mht_)mht= muIsoYmax-1;		
		muonBinByBinIsoXaxis = MuonIsoBinByBinEff35_->GetXaxis()->FindBin(ht);
		muonBinByBinIsoYaxis = MuonIsoBinByBinEff35_->GetYaxis()->FindBin(mht);
		muonBinByBinIsoEff_ = MuonIsoBinByBinEff35_->GetBinContent(muonBinByBinIsoXaxis,muonBinByBinIsoYaxis);


	 	double muRecoXmax = MuonRecoBinByBinEff35_->GetXaxis()->GetXmax();
		double ht1 = ht_;
		if (muRecoXmax<ht_)ht1= muRecoXmax-1;
 		double muRecoYmax = MuonRecoBinByBinEff35_->GetYaxis()->GetXmax();
		double mht1 = mht_;
		if (muRecoYmax<mht_)mht1= muRecoYmax-1;		
		muonBinByBinRecoXaxis = MuonRecoBinByBinEff35_->GetXaxis()->FindBin(ht1);
		muonBinByBinRecoYaxis = MuonRecoBinByBinEff35_->GetYaxis()->FindBin(mht1);
		muonBinByBinRecoEff_ = MuonRecoBinByBinEff35_->GetBinContent(muonBinByBinRecoXaxis,muonBinByBinRecoYaxis);

	}
	if (nJets_ > 5.5 && 7.5 > nJets_)
	{
	 	double muIsoXmax = MuonIsoBinByBinEff67_->GetXaxis()->GetXmax();
		double ht = ht_;
		if (muIsoXmax<ht_)ht= muIsoXmax-1;
 		double muIsoYmax = MuonIsoBinByBinEff67_->GetYaxis()->GetXmax();
		double mht = mht_;
		if (muIsoYmax<mht_)mht= muIsoYmax-1;		
		muonBinByBinIsoXaxis = MuonIsoBinByBinEff67_->GetXaxis()->FindBin(ht);
		muonBinByBinIsoYaxis = MuonIsoBinByBinEff67_->GetYaxis()->FindBin(mht);
		muonBinByBinIsoEff_ = MuonIsoBinByBinEff67_->GetBinContent(muonBinByBinIsoXaxis,muonBinByBinIsoYaxis);

	 	double muRecoXmax = MuonRecoBinByBinEff67_->GetXaxis()->GetXmax();
		double ht1 = ht_;
		if (muRecoXmax<ht_)ht1= muRecoXmax-1;
 		double muRecoYmax = MuonRecoBinByBinEff67_->GetYaxis()->GetXmax();
		double mht1 = mht_;
		if (muRecoYmax<mht_)mht1= muRecoYmax-1;		
		muonBinByBinRecoXaxis = MuonRecoBinByBinEff67_->GetXaxis()->FindBin(ht1);
		muonBinByBinRecoYaxis = MuonRecoBinByBinEff67_->GetYaxis()->FindBin(mht1);
		muonBinByBinRecoEff_ = MuonRecoBinByBinEff67_->GetBinContent(muonBinByBinRecoXaxis,muonBinByBinRecoYaxis);

	}
	if (nJets_ > 7.5)
	{
	 	double muIsoXmax = MuonIsoBinByBinEff8Inf_->GetXaxis()->GetXmax();
		double ht = ht_;
		if (muIsoXmax<ht_)ht= muIsoXmax-1;
 		double muIsoYmax = MuonIsoBinByBinEff8Inf_->GetYaxis()->GetXmax();
		double mht = mht_;
		if (muIsoYmax<mht_)mht= muIsoYmax-1;		
		muonBinByBinIsoXaxis = MuonIsoBinByBinEff8Inf_->GetXaxis()->FindBin(ht);
		muonBinByBinIsoYaxis = MuonIsoBinByBinEff8Inf_->GetYaxis()->FindBin(mht);
		muonBinByBinIsoEff_ = MuonIsoBinByBinEff8Inf_->GetBinContent(muonBinByBinIsoXaxis,muonBinByBinIsoYaxis);

	 	double muRecoXmax = MuonRecoBinByBinEff8Inf_->GetXaxis()->GetXmax();
		double ht1 = ht_;
		if (muRecoXmax<ht_)ht1= muRecoXmax-1;
 		double muRecoYmax = MuonRecoBinByBinEff8Inf_->GetYaxis()->GetXmax();
		double mht1 = mht_;
		if (muRecoYmax<mht_)mht1= muRecoYmax-1;		
		muonBinByBinRecoXaxis = MuonRecoBinByBinEff8Inf_->GetXaxis()->FindBin(ht1);
		muonBinByBinRecoYaxis = MuonRecoBinByBinEff8Inf_->GetYaxis()->FindBin(mht1);
		muonBinByBinRecoEff_ = MuonRecoBinByBinEff8Inf_->GetBinContent(muonBinByBinRecoXaxis,muonBinByBinRecoYaxis);

	}
//std::cout<<"bb1"<<std::endl;
double error_=0;


//	muonBinByBinIsoEff_ = MuonIsoBinByBinEff_->GetBinContent(muonBinByBinIsoXaxis,muonBinByBinIsoYaxis,muonBinByBinIsoZaxis );
	if (muonBinByBinIsoEff_ <0.01 || muonBinByBinIsoEff_ > 1) std::cout<<"Error muonBinByBinIsoEff_ is"<<muonBinByBinIsoEff_<<std::endl;
	if (muonBinByBinIsoEff_<0.01) muonBinByBinIsoEff_=0.01;
	if (muonBinByBinIsoEff_>1 ) muonBinByBinIsoEff_=1;
	muonBinByBinIsoWeight_ = eventWeight_ * (1 - muonBinByBinIsoEff_)/muonBinByBinIsoEff_;	
	//std::cout<<"muonBinByBinIsoWeight_"<<muonBinByBinIsoWeight_<<std::endl;	
	//used these in AN up to now 14Mar2013
//	muonBinByBinRecoEff_ = muonRecoEff2_;
//	muonBinByBinRecoEffError = muonRecoEff2Error;
	if (muonBinByBinRecoEff_ <0.01 || muonBinByBinRecoEff_ > 1) std::cout<<"Error muonBinByBinRecoEff_ is"<<muonBinByBinRecoEff_<<std::endl;
	if (muonBinByBinRecoEff_<0.01) muonBinByBinRecoEff_=0.01;
	if (muonBinByBinRecoEff_>1 ) muonBinByBinRecoEff_=1;
	muonBinByBinRecoWeight_ = eventWeight_ * 1 / muonBinByBinIsoEff_ * (1-muonBinByBinRecoEff_)/muonBinByBinRecoEff_;
//	std::cout<<"muonBinByBinRecoEffError"<<muonBinByBinRecoEffError<<std::endl;
//	std::cout<<"muonBinByBinRecoEff_"<<muonBinByBinRecoEff_<<std::endl;

	//std::cout<<"LostLeptonBkg::muonBinByBinRecoWeight_:"<<muonBinByBinRecoWeight_<<std::endl;
//	if (statErrorEffmaps_) muonBinByBinAccEffError_= MuonAccBinByBinEff_->GetBinError(muonBinByBinAccXaxis,muonBinByBinAccYaxis,muonBinByBinAccZaxis );


	if (muonBinByBinAccEff_ <0.01 || muonBinByBinAccEff_ > 1) std::cout<<"Error muonBinByBinAccEff_ is"<<muonBinByBinAccEff_<<std::endl;
	if (muonBinByBinAccEff_<0.01) muonBinByBinAccEff_=0.01;
	if (muonBinByBinAccEff_>1 ) muonBinByBinAccEff_=1;
	muonBinByBinAccWeight_ = eventWeight_ * 1/muonBinByBinIsoEff_ * 1/muonBinByBinRecoEff_ *(1-muonBinByBinAccEff_)/muonBinByBinAccEff_;
	//std::cout<<"LostLeptonBkg::muonBinByBinAccWeight_:"<<muonBinByBinAccWeight_<<std::endl;
	// total binbybin muon weight
	muonBinByBinTotalWeight_ = muonBinByBinIsoWeight_ + muonBinByBinRecoWeight_ + muonBinByBinAccWeight_;
	//std::cout<<"LostLeptonBkg::muonBinByBinTotalWeight_:"<<muonBinByBinTotalWeight_<<std::endl;
	totalBinByBinMuons_ = eventWeight_ / (muonBinByBinAccEff_ * muonBinByBinRecoEff_ * muonBinByBinIsoEff_);

	// binbybin elec eff

//	int elecBinByBinIsoXaxis= ElecIsoBinByBinEff_->GetXaxis()->FindBin(ht_);
//	if (elecBinByBinIsoXaxis > ElecIsoBinByBinEff_->GetNbinsX()) elecBinByBinIsoXaxis-=1;
//	int elecBinByBinIsoYaxis= ElecIsoBinByBinEff_->GetYaxis()->FindBin(mht_);
//	if (elecBinByBinIsoYaxis > ElecIsoBinByBinEff_->GetNbinsY()) elecBinByBinIsoYaxis-=1;
//	int elecBinByBinIsoZaxis= ElecIsoBinByBinEff_->GetZaxis()->FindBin(nJets_+0.1);
//	if (elecBinByBinIsoZaxis > ElecIsoBinByBinEff_->GetNbinsZ()) elecBinByBinIsoZaxis-=1;

	int elecBinByBinIsoXaxis=0;
	int elecBinByBinIsoYaxis=0;

	int elecBinByBinRecoXaxis=0;
	int elecBinByBinRecoYaxis=0;
	if (nJets_ > 2.5 && 5.5 > nJets_)
	{
	 	double elecIsoXmax = ElecIsoBinByBinEff35_->GetXaxis()->GetXmax();
		double ht = ht_;
		if (elecIsoXmax<ht_)ht= elecIsoXmax-1;
 		double elecIsoYmax = ElecIsoBinByBinEff35_->GetYaxis()->GetXmax();
		double mht = mht_;
		if (elecIsoYmax<mht_)mht= elecIsoYmax-1;		
		elecBinByBinIsoXaxis = ElecIsoBinByBinEff35_->GetXaxis()->FindBin(ht);
		elecBinByBinIsoXaxis = ElecIsoBinByBinEff35_->GetYaxis()->FindBin(mht);
		elecBinByBinIsoEff_ = ElecIsoBinByBinEff35_->GetBinContent(elecBinByBinIsoXaxis,elecBinByBinIsoYaxis);

	 	double elecRecoXmax = ElecRecoBinByBinEff35_->GetXaxis()->GetXmax();
		double ht3 = ht_;
		if (elecRecoXmax<ht_)ht3= elecRecoXmax-1;
 		double elecRecoYmax = ElecRecoBinByBinEff35_->GetYaxis()->GetXmax();
		double mht3 = mht_;
		if (elecRecoXmax<mht_)mht3= elecRecoYmax-1;		
		elecBinByBinRecoXaxis = ElecRecoBinByBinEff35_->GetXaxis()->FindBin(ht3);
		elecBinByBinRecoYaxis = ElecRecoBinByBinEff35_->GetYaxis()->FindBin(mht3);

		elecBinByBinRecoEff_ = ElecRecoBinByBinEff35_->GetBinContent(elecBinByBinRecoXaxis,elecBinByBinRecoYaxis);
	}
	if (nJets_ > 5.5 && 7.5 > nJets_)
	{
	 	double elecIsoXmax = ElecIsoBinByBinEff67_->GetXaxis()->GetXmax();
		double ht = ht_;
		if (elecIsoXmax<ht_)ht= elecIsoXmax-1;
 		double elecIsoYmax = ElecIsoBinByBinEff67_->GetYaxis()->GetXmax();
		double mht = mht_;
		if (elecIsoYmax<mht_)mht= elecIsoYmax-1;		
		elecBinByBinIsoXaxis = ElecIsoBinByBinEff67_->GetXaxis()->FindBin(ht);
		elecBinByBinIsoXaxis = ElecIsoBinByBinEff67_->GetYaxis()->FindBin(mht);
		elecBinByBinIsoEff_ = ElecIsoBinByBinEff67_->GetBinContent(elecBinByBinIsoXaxis,elecBinByBinIsoYaxis);
	 	double elecRecoXmax = ElecRecoBinByBinEff67_->GetXaxis()->GetXmax();
		double ht3 = ht_;
		if (elecRecoXmax<ht_)ht3= elecRecoXmax-1;
 		double elecRecoYmax = ElecRecoBinByBinEff67_->GetYaxis()->GetXmax();
		double mht3 = mht_;
		if (elecRecoXmax<mht_)mht3= elecRecoYmax-1;		
		elecBinByBinRecoXaxis = ElecRecoBinByBinEff67_->GetXaxis()->FindBin(ht3);
		elecBinByBinRecoYaxis = ElecRecoBinByBinEff67_->GetYaxis()->FindBin(mht3);
		elecBinByBinRecoEff_ = ElecRecoBinByBinEff67_->GetBinContent(elecBinByBinRecoXaxis,elecBinByBinRecoYaxis);
	}
	if (nJets_ > 7.5)
	{
	 	double elecIsoXmax = ElecIsoBinByBinEff8Inf_->GetXaxis()->GetXmax();
		double ht = ht_;
		if (elecIsoXmax<ht_)ht= elecIsoXmax-1;
 		double elecIsoYmax = ElecIsoBinByBinEff8Inf_->GetYaxis()->GetXmax();
		double mht = mht_;
		if (elecIsoYmax<mht_)mht= elecIsoYmax-1;		
		elecBinByBinIsoXaxis = ElecIsoBinByBinEff8Inf_->GetXaxis()->FindBin(ht);
		elecBinByBinIsoXaxis = ElecIsoBinByBinEff8Inf_->GetYaxis()->FindBin(mht);

		elecBinByBinIsoEff_ = ElecIsoBinByBinEff8Inf_->GetBinContent(elecBinByBinIsoXaxis,elecBinByBinIsoYaxis);
	 	double elecRecoXmax = ElecRecoBinByBinEff8Inf_->GetXaxis()->GetXmax();
		double ht3 = ht_;
		if (elecRecoXmax<ht_)ht3= elecRecoXmax-1;
 		double elecRecoYmax = ElecRecoBinByBinEff8Inf_->GetYaxis()->GetXmax();
		double mht3 = mht_;
		if (elecRecoXmax<mht_)mht3= elecRecoYmax-1;		
		elecBinByBinRecoXaxis = ElecRecoBinByBinEff8Inf_->GetXaxis()->FindBin(ht3);
		elecBinByBinRecoYaxis = ElecRecoBinByBinEff8Inf_->GetYaxis()->FindBin(mht3);

		elecBinByBinRecoEff_ = ElecRecoBinByBinEff8Inf_->GetBinContent(elecBinByBinRecoXaxis,elecBinByBinRecoYaxis);

	}


	// calc eff and weights

//	if (statErrorEffmaps_) elecBinByBinAccEffError_= ElecAccBinByBinEff_->GetBinError(elecBinByBinAccXaxis,elecBinByBinAccYaxis,elecBinByBinAccZaxis );

 	double elecAccXmax = ElecAccEff3_->GetXaxis()->GetXmax();
//	std::cout<<"muAccXmax"<<muAccXmax<<std::endl;
 	double elecAccYmax = ElecAccEff3_->GetYaxis()->GetXmax();
//	std::cout<<"muAccYmax"<<muAccYmax<<std::endl;
	double mht2=mht_;
	if ( elecAccXmax<mht_) mht2=elecAccXmax-1;
	int elecAccXaxis3= ElecAccEff3_->GetXaxis()->FindBin(mht2);

	int nJets2=nJets_;
	if (elecAccYmax<nJets_) nJets2=elecAccYmax-0.2;
	int elecAccYaxis3= ElecAccEff3_->GetYaxis()->FindBin(nJets2+0.1);
	elecAccEff2_ = ElecAccEff3_->GetBinContent(elecAccXaxis3,elecAccYaxis3);
	// use these IN AN up to now 14Mar2013
	elecBinByBinAccEff_ = elecAccEff2_;
//	elecBinByBinAccEffError = elecAccEff2Error;
	if (elecBinByBinAccEff_ <0.01 || elecBinByBinAccEff_ > 1) error_+=103;
	if (elecBinByBinAccEff_<0.01) elecBinByBinAccEff_=0.01;
	if (elecBinByBinAccEff_>1 ) elecBinByBinAccEff_=1;
	elecBinByBinAccWeight_ = totalBinByBinMuons_ * (1 - elecBinByBinAccEff_);
	// total binbybin elec weight


//	elecBinByBinRecoEff_ = ElecRecoBinByBinEff_->GetBinContent(elecBinByBinRecoXaxis,elecBinByBinRecoYaxis,elecBinByBinRecoZaxis );
//	if (statErrorEffmaps_) elecBinByBinRecoEffError_= ElecRecoBinByBinEff_->GetBinError(elecBinByBinRecoXaxis,elecBinByBinRecoYaxis,elecBinByBinRecoZaxis );
//	elecBinByBinRecoEff_ = elecRecoEff2_;
//	double elecBinByBinRecoEffError = elecRecoEff2Error;
	if (elecBinByBinRecoEff_ <0.01 || elecBinByBinRecoEff_ > 1) error_+=103;
	if (elecBinByBinRecoEff_<0.01) elecBinByBinRecoEff_=0.01;
	if (elecBinByBinRecoEff_>1 ) elecBinByBinRecoEff_=1;
	elecBinByBinRecoWeight_ = totalBinByBinMuons_ * (elecBinByBinAccEff_) * (1 - elecBinByBinRecoEff_);

//	elecBinByBinIsoEff_ = ElecIsoBinByBinEff_->GetBinContent(elecBinByBinIsoXaxis,elecBinByBinIsoYaxis,elecBinByBinIsoZaxis );
	if (elecBinByBinIsoEff_ <0.01 || elecBinByBinIsoEff_ > 1) error_+=102;
	if (elecBinByBinIsoEff_<0.01) elecBinByBinIsoEff_=0.01;
	if (elecBinByBinIsoEff_>1 ) elecBinByBinIsoEff_=1;
	elecBinByBinIsoWeight_ = totalBinByBinMuons_ * (elecBinByBinAccEff_) * (elecBinByBinRecoEff_) * (1 - elecBinByBinIsoEff_) ;
	elecBinByBinTotalWeight_ = elecBinByBinIsoWeight_ + elecBinByBinRecoWeight_ + elecBinByBinAccWeight_;
	resultBBBW_ = elecBinByBinTotalWeight_ + muonBinByBinTotalWeight_;
	// mtw cut correction
	if(MTWCut_)
	{
		int MTWbin = MTWNJet_->GetXaxis()->FindBin(nJets_);
		if ( MTWbin > MTWNJet_->GetNbinsX() ) MTWbin -=1;
		resultBBBWMTW_ = resultBBBW_ / MTWNJet_->GetBinContent(MTWbin );
	}
	else resultBBBWMTW_ = resultBBBW_;
	// dilep correction and corresponding uncertainty
	if (abs(DiLepCorrection_))
	{
		resultBBBWMTWDiLep_= resultBBBWMTW_ * (1 + DiLepCorrection_/100.);
		//std::cout<<"LostLeptonBkg::resultBBBWMTWDiLep_"<<resultBBBWMTWDiLep_<<std::endl;
		resultBBBWMTWDiLepUp_ = resultBBBWMTWDiLep_ * ( 1 + (std::abs(DiLepCorrection_) * DiLepCorrectionUp_)/10000.);
		resultBBBWMTWDiLepDown_ = resultBBBWMTWDiLep_ * (1 - ((std::abs(DiLepCorrection_)*DiLepCorrectionDown_)/10000.));
	}
	else resultBBBWMTWDiLep_=resultBBBWMTW_;
	// mtw uncertainty
if (resultBBBWMTWDiLep_>20)

{
std::cout<<"LostLeptonResultWeight"<<resultBBBWMTWDiLep_<<", original weight:"<<eventWeight_<<std::endl;
//std::cout<<"ht_"<<ht_<<", mht"<<mht_<<", nJets_"<<nJets_<<"mtw_"<<sqrt(2*MuPt*MetPt*(1-cos(deltaPhi)) )<<std::endl;
}
 std::cout<<"LostLeptonResultWeight"<<resultBBBWMTWDiLep_<<std::flush;
 if( resultBBBWMTWDiLep_ > 10 ) std::cout << "    <<<<======" << std::flush;
 std::cout << std::endl;
}


}
//else std::cout<<"Not 1 muon and 0 electrons!"<<std::endl;





























	}
	delete c;




/* This is an event example
   //Read 'ExampleData' from the Event
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);

   //Use the ExampleData to create an ExampleData2 which 
   // is put into the Event
   std::auto_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
   iEvent.put(pOut);
*/

/* this is an EventSetup example
   //Read SetupData from the SetupRecord in the EventSetup
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
*/
 
}

// ------------ method called once each job just before starting event loop  ------------
void 
Limit_ll::beginJob()
{
std::cout<<"Eff loading...";
  std::string fileName("RA2Classic/LostLeptonBkg/data/");
  fileName += EfficiencyFileName_;
  edm::FileInPath fip1(fileName);


  TFile* effInput = TFile::Open(fip1.fullPath().c_str());

  TDirectory *dMuon = (TDirectory*)effInput->Get("Muon");
  
  TDirectory *dElec = (TDirectory*)effInput->Get("Electron");

//load eff maps
  MuonAccEff3_ = (TH2F*)dMuon->Get("MuonAccEff3");
  ElecAccEff3_ = (TH2F*)dElec->Get("ElecAccEff3");
// iso
  MuonIsoBinByBinEff35_ = (TH2F*)dMuon->Get("IsoHTMHTMuEffNJet_35");
  MuonIsoBinByBinEff67_ = (TH2F*)dMuon->Get("IsoHTMHTMuEffNJet_67");
  MuonIsoBinByBinEff8Inf_ = (TH2F*)dMuon->Get("IsoHTMHTMuEffNJet_8Inf");

  ElecIsoBinByBinEff35_ = (TH2F*)dElec->Get("IsoHTMHTElecEffNJet_35");
  ElecIsoBinByBinEff67_ = (TH2F*)dElec->Get("IsoHTMHTElecEffNJet_67");
  ElecIsoBinByBinEff8Inf_ = (TH2F*)dElec->Get("IsoHTMHTElecEffNJet_8Inf");
// reco
  MuonRecoBinByBinEff35_ = (TH2F*)dMuon->Get("RecoHTMHTMuEffNJet_35");
  MuonRecoBinByBinEff67_ = (TH2F*)dMuon->Get("RecoHTMHTMuEffNJet_67");
  MuonRecoBinByBinEff8Inf_ = (TH2F*)dMuon->Get("RecoHTMHTMuEffNJet_8Inf");

  ElecRecoBinByBinEff35_ = (TH2F*)dElec->Get("RecoHTMHTElecEffNJet_35");
  ElecRecoBinByBinEff67_ = (TH2F*)dElec->Get("RecoHTMHTElecEffNJet_67");
  ElecRecoBinByBinEff8Inf_ = (TH2F*)dElec->Get("RecoHTMHTElecEffNJet_8Inf");


MTWNJet_= (TH1F*)dMuon->Get("MTWCutNJet");


  std::string fileName1("RA2Classic/LostLeptonBkg/data/");
  fileName1 += nTupleName_;
  edm::FileInPath fip(fileName1);
  TFile* InputFile = TFile::Open(fip.fullPath().c_str());
std::cout<<"done"<<std::endl;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Limit_ll::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
Limit_ll::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
Limit_ll::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
Limit_ll::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
Limit_ll::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
Limit_ll::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(Limit_ll);
