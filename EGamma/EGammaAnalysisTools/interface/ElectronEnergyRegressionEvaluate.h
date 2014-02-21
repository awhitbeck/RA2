/*
   Class to apply electron energy regression. To be used in conjunction with the output of the macro trainElectronEnergyRegression.C
 */
#ifndef ELECTRONENERGYREGRESSIONEVALUATE_H
#define ELECTRONENERGYREGRESSIONEVALUATE_H

#include "TFile.h"
#include "TTree.h"

// For applying regression
#include "CondFormats/EgammaObjects/interface/GBRForest.h"

#endif

class ElectronEnergyRegressionEvaluate{
	public:
		ElectronEnergyRegressionEvaluate();
		~ElectronEnergyRegressionEvaluate();

		enum ElectronEnergyRegressionType {
		  kNoTrkVar,
		  kWithTrkVar,
		  kNoTrkVarTwoPtBins,
		  kWithTrkVarTwoPtBins
		};

		void initialize(std::string weightsFile,
				ElectronEnergyRegressionEvaluate::ElectronEnergyRegressionType type);

		bool isInitialized() const {return fIsInitialized;}

		double regressionValueNoTrkVar(			// Evaluates regression without tracker variables
				double SCRawEnergy,
				double scEta,
				double scPhi,
				double R9,
				double E5x5,
				double etawidth,
				double phiwidth,
				double NClusters,
				double HoE,
				double rho,
				double vertices,
				double EtaSeed,
				double PhiSeed,
				double ESeed,
				double E3x3Seed,
				double E5x5Seed,
				double see,
				double spp,
				double sep,
				double EMaxSeed,
				double E2ndSeed,
				double ETopSeed,
				double EBottomSeed,
				double ELeftSeed,
				double ERightSeed,
				double E2x5MaxSeed,
				double E2x5TopSeed,
				double E2x5BottomSeed,
				double E2x5LeftSeed,
				double E2x5RightSeed,
				double pt,
				double IEtaSeed,
				double IPhiSeed,
				double EtaCrySeed,
				double PhiCrySeed,
				double PreShowerOverRaw );

		double regressionValueWithTrkVar(				// Evaluates regression using tracker variables
				double SCRawEnergy,
				double scEta,
				double scPhi,
				double R9,
				double E5x5,
				double etawidth,
				double phiwidth,
				double NClusters,
				double HoE,
				double rho,
				double vertices,
				double EtaSeed,
				double PhiSeed,
				double ESeed,
				double E3x3Seed,
				double E5x5Seed,
				double see,
				double spp,
				double sep,
				double EMaxSeed,
				double E2ndSeed,
				double ETopSeed,
				double EBottomSeed,
				double ELeftSeed,
				double ERightSeed,
				double E2x5MaxSeed,
				double E2x5TopSeed,
				double E2x5BottomSeed,
				double E2x5LeftSeed,
				double E2x5RightSeed,
				double pt,
				double GsfTrackPIn,
				double fbrem,
				double Charge,
				double EoP,
				double IEtaSeed,
				double IPhiSeed,
				double EtaCrySeed,
				double PhiCrySeed,
				double PreShowerOverRaw );

	private:
		bool fIsInitialized;
		ElectronEnergyRegressionEvaluate::ElectronEnergyRegressionType fVersionType;
		GBRForest *forest_eb;		// Pointer to the GBRForest for barrel
		GBRForest *forest_ee;		// Pointer to the GBRForest for endcap
		GBRForest *forest_lowPt_eb;	// Pointer to the GBRForest for barrel, low Pt
		GBRForest *forest_lowPt_ee;	// Pointer to the GBRForest for endcap, low Pt
		GBRForest *forest_highPt_eb;	// Pointer to the GBRForest for barrel, high Pt
		GBRForest *forest_highPt_ee;	// Pointer to the GBRForest for endcap, high Pt


};
