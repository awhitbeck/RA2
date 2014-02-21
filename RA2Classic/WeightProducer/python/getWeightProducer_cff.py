# $Id: getWeightProducer_cff.py,v 1.13 2013/01/25 10:04:27 mschrode Exp $
#
# Returns a WeightProducer module that knows at runtime
# which data sample is produced and thus, what weights
# are required. The function can be used as follows:
#
#    from RA2Classic.WeightProducer.getWeightProducer_cff import getWeightProducer
#    process.WeightProducer = getWeightProducer(process.source.fileNames[0])


import FWCore.ParameterSet.Config as cms

def getWeightProducer(fileName):

    mcVersion = "none"                  # For lumi and PU weights
    applyWeight = False


    ## --- Setup default WeightProducer ------------------------------------

    # Import weightProducer
    from RA2Classic.WeightProducer.weightProducer_cfi import weightProducer

    # Set default values to produce an event weight of 1
    weightProducer.weight = cms.double(1.0)
    weightProducer.Method = cms.string("")
    weightProducer.LumiScale = cms.double(1.0)
    weightProducer.FileNamePUDataDistribution = cms.string("NONE")
    weightProducer.PU = cms.int32(0)


    ## --- Adjust WeightProducer for Summer12_5_3_X MC samples --------------------

    # For ttbar
    if "TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola" in fileName and "Summer12_DR53X-PU_S10_START53_V7A-v1" in fileName:
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(234)
        weightProducer.NumberEvts = cms.double(6923750)

    if "TT_CT10_TuneZ2star_8TeV-powheg-tauola" in fileName and "Summer12_DR53X-PU_S10_START53_V7A-v2" in fileName:
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(234)
        weightProducer.NumberEvts = cms.double(21675970)

    if "TTJets_SemiLeptMGDecays_8TeV-madgraph" in fileName and "Summer12_DR53X-PU_S10_START53_V7A_ext-v1" in fileName:
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(102.50) # BR(lh) = 0.438048
        weightProducer.NumberEvts = cms.double(25424818)

    if "TTJets_FullLeptMGDecays_8TeV-madgraph" in fileName and "Summer12_DR53X-PU_S10_START53_V7A-v2" in fileName:
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(24.56) # BR(ll) = 0.104976
        weightProducer.NumberEvts = cms.double(12119013)

    if "TTJets_HadronicMGDecays_8TeV-madgraph" in fileName and "Summer12_DR53X-PU_S10_START53_V7A_ext-v1" in fileName:
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(106.93) # BR(hh) = 0.456976
        weightProducer.NumberEvts = cms.double(31223821)

        
        

    # For wpj
    if "WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball" in fileName and "Summer12_DR53X-PU_S10_START53_V7A-v2" in fileName:
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(36257.2)
        weightProducer.NumberEvts = cms.double(57709905)

    if "WJetsToLNu_HT-400ToInf_8TeV-madgraph" in fileName and "Summer12_DR53X-PU_S10_START53_V7A-v1" in fileName:
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(30.08)
        weightProducer.NumberEvts = cms.double(6619654) # !!!!! This is the sum of events in the WJetsToLNu_HT-400ToInf_8TeV-madgraph and WJetsToLNu_HT-400ToInf_8TeV-madgraph_v2 samples --> weight only valid if both samples are used in sum !!!!!

    # For ZJets
    if "ZJetsToNuNu_50_HT_100_TuneZ2Star_8TeV_madgraph" in fileName and "Summer12_DR53X-PU_S10_START53_V7A-v1" in fileName:
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(452.75)
        weightProducer.NumberEvts = cms.double(4040980)
    if "ZJetsToNuNu_100_HT_200_TuneZ2Star_8TeV_madgraph" in fileName and "Summer12_DR53X-PU_S10_START53_V7A-v1" in fileName:
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(190.39)
        weightProducer.NumberEvts = cms.double(4416646)         
    if "ZJetsToNuNu_200_HT_400_TuneZ2Star_8TeV_madgraph" in fileName and "Summer12_DR53X-PU_S10_START53_V7A-v1" in fileName:
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(49.28)
        weightProducer.NumberEvts = cms.double(5055885)         
    if "ZJetsToNuNu_400_HT_inf_TuneZ2Star_8TeV_madgraph" in fileName and "Summer12_DR53X-PU_S10_START53_V7A-v1" in fileName:
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(6.26)
        weightProducer.NumberEvts = cms.double(1006928)         

    # For QCD
    if "QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6" in fileName and "Summer12_DR53X-PU_S10_START53_V7A-v1" in fileName:
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("PtHat")
        weightProducer.XS         = cms.double(2.99815997e+10)
        weightProducer.NumberEvts = cms.double(9991674)
        weightProducer.Exponent   = cms.double(-4.5)
    if "QCD_HT-100To250_TuneZ2star_8TeV-madgraph-pythia" in fileName and "Summer12_DR53X-PU_S10_START53_V7A-v1" in fileName:    
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(1.036e+7)
        weightProducer.NumberEvts = cms.double(50129518)
    if "QCD_HT-250To500_TuneZ2star_8TeV-madgraph-pythia6" in fileName and "Summer12_DR53X-PU_S10_START53_V7A-v1" in fileName:    
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(276000.0)
        weightProducer.NumberEvts = cms.double(27062078)
    if "QCD_HT-500To1000_TuneZ2star_8TeV-madgraph-pythia6" in fileName and "Summer12_DR53X-PU_S10_START53_V7A-v1" in fileName:    
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(8426.0)
        weightProducer.NumberEvts = cms.double(30599292)
    if "QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6" in fileName and "Summer12_DR53X-PU_S10_START53_V7A-v1" in fileName:    
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(204.0)
        weightProducer.NumberEvts = cms.double(13843863)

    # For LM points
    if "SUSY_LM6_sftsht_8TeV-pythia6" in fileName:
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(0.502)
        weightProducer.NumberEvts = cms.double(1000080)         
    if "SUSY_LM9_sftsht_8TeV-pythia6" in fileName:
        mcVersion = "Summer12_5_3_X"
        weightProducer.Method     = cms.string("Constant")
        weightProducer.XS         = cms.double(9.287)
        weightProducer.NumberEvts = cms.double(1000080)         


    ## --- PU Reweighting and Lumi ------------------------------------------------
         
    if mcVersion == "Summer12_5_3_X":
        weightProducer.weight     = cms.double(-1.)
        weightProducer.Lumi       = cms.double(19466)
        weightProducer.PU         = cms.int32(3) # PU S10
##        weightProducer.FileNamePUDataDistribution = cms.string("RA2Classic/AdditionalInputFiles/DataPileupHistogram_RA2Summer12_5_2_X_190456-196531_8TeV_PromptReco_WOLowPU.root")
        # grid-control requires additional input files to be in the data directory
        weightProducer.FileNamePUDataDistribution = cms.string("RA2Classic/WeightProducer/data/DataPileupHistogram_RA2Summer12_190456-208686_ABCD.root")
        applyWeight = True

    if applyWeight:
        print "Setup WeightProducer for '"+fileName+"'"

    return weightProducer
