import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")

process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.TFileService = cms.Service(
     "TFileService",
     fileName = cms.string("runT2_ROOTSUFFIX.root")
)

process.maxEvents = cms.untracked.PSet(
#    input = cms.untracked.int32(100)
	    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
	'file:reco_9_1_ZAm.root'
	)
)

#  'dcap://pnfs/cms/WAX/resilient/spadhi/CMS/spadhi/PhysicsProcesses_TopologyT2_38xFall10/PhysicsProcesses_TopologyT2_38xFall10/f959c379445f9d7540d41cf9ffa87a96/ROOTFILE'
process.source.fileNames = [
  'dcap:/ROOTFILE'
	]


process.load("RecoJets.Configuration.GenJetParticles_cff")
process.genParticlesForJets.ignoreParticleIDs.extend(cms.vuint32(5000039))
process.load("RecoJets.JetProducers.ak5GenJets_cfi")


process.printGenParticles = cms.EDAnalyzer("ParticleListDrawer",
     src = cms.InputTag("genParticles"),
#	 src = cms.InputTag("ak5GenJets"),
     maxEventsToPrint = cms.untracked.int32(1)
)



process.MessageLogger = cms.Service("MessageLogger",
    cout2 = cms.untracked.PSet(
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        )
    ),
    destinations = cms.untracked.vstring('cout2')
)

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    moduleSeeds = cms.PSet(
        generator = cms.untracked.uint32(1234567),
        g4SimHits = cms.untracked.uint32(123456788),
        VtxSmeared = cms.untracked.uint32(123456789)
    ),
)



process.GEN = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('pythia8ex2.root'),
	outputCommands = cms.untracked.vstring(
	'keep *_*_*_*',
    'drop *_ak5GenJetsClone_*_*', 
    'drop *_twoJetsClone_*_*') 
)

process.genParticlesClone = cms.EDFilter("CandViewShallowCloneProducer",
										 src = cms.InputTag("genParticles"),
										 cut = cms.string("pdgId = 1000021 & status=3")
										 )


process.isrJet = cms.EDProducer("CandViewShallowCloneCombiner",
								decay       = cms.string("genParticlesClone genParticlesClone"), 
								checkCharge = cms.bool(False),
								cut         = cms.string("pt>-1")
								)

process.ak5Clone = cms.EDFilter("CandViewShallowCloneProducer",
										 src = cms.InputTag("ak5GenJets"),
										 cut = cms.string("pt>-1.")
										 )


process.allJets = cms.EDProducer("CompositeProducer",
										 dauSrc = cms.InputTag("ak5Clone"),
										 )

process.lastParticles = cms.EDProducer("ISRProducer")


process.ak5GenJetsClone = cms.EDFilter("LargestPtCandViewSelector",
										 src = cms.InputTag("ak5GenJets"),
									     maxNumber = cms.uint32(5),
										 cut = cms.string("pt>20.")									   
										 ) 

process.twoJetsClone = cms.EDFilter("LargestPtCandViewSelector",
										 src = cms.InputTag("ak5GenJets"),
									     maxNumber = cms.uint32(3)
										 ) 

process.jetPair = cms.EDProducer("DeltaPhiMinCandCombiner",
								decay       = cms.string("twoJetsClone twoJetsClone"),
								deltaPhiMin = cms.double(-6.0000),
                                cut = cms.string("pt>20.")
#								checkCharge = cms.bool(False),
#								cut         = cms.string("abs(daughter(0).phi-daughter(1).phi) > 0 ")
								)

process.printGenJets = cms.EDAnalyzer("ParticleListDrawer",
#     src = cms.InputTag("genParticles"),
#	 src = cms.InputTag("ak5GenJets"),
	 src = cms.InputTag("lastParticles"),									  
#	 src = cms.InputTag("ak5GenJetsClone"),
#	 src = cms.InputTag("isrJet"),
#	 src = cms.InputTag("jetPair"),
     maxEventsToPrint = cms.untracked.int32(2)
)

process.printTwoJets = cms.EDAnalyzer("ParticleListDrawer",
#     src = cms.InputTag("genParticles"),
#	 src = cms.InputTag("ak5GenJets"),
#	 src = cms.InputTag("ak5GenJetsClone"),
#	 src = cms.InputTag("isrJet"),
#	 src = cms.InputTag("twoJetsClone"),
	 src = cms.InputTag("allJets"),
     maxEventsToPrint = cms.untracked.int32(2)
)

process.sumPt= cms.EDAnalyzer("CandViewHistoAnalyzer",
							src = cms.InputTag("allJets"),
	  # weights = cms.untracked.InputTag("myProducerLabel"),							  
 histograms = cms.VPSet(
	    cms.PSet(
	    min = cms.untracked.double(0.0),
	    max = cms.untracked.double(1250.0),
	    nbins = cms.untracked.int32(100),
	    name = cms.untracked.string("jet_sumPt"),
	    description = cms.untracked.string("sumPt of extra jets"),
	    plotquantity = cms.untracked.string("energy")
	    )		,
		)
)


process.deltaPhi= cms.EDAnalyzer("CandViewHistoAnalyzer",
							src = cms.InputTag("jetPair"),
	  # weights = cms.untracked.InputTag("myProducerLabel"),								 
 histograms = cms.VPSet(
	    cms.PSet(
            itemsToPlot = cms.untracked.int32(3),
	    min = cms.untracked.double(0.0),
	    max = cms.untracked.double(3.15),
	    nbins = cms.untracked.int32(100),
	    name = cms.untracked.string("delta_Phi12"),
	    description = cms.untracked.string("delta Phi"),
	    plotquantity = cms.untracked.string("abs(deltaPhi(daughter(0).phi,daughter(1).phi))")
	    )		,
	    cms.PSet(
            itemsToPlot = cms.untracked.int32(3),
	    min = cms.untracked.double(0.55),
	    max = cms.untracked.double(6.0),
	    nbins = cms.untracked.int32(100),
	    name = cms.untracked.string("delta_R12"),
	    description = cms.untracked.string("delta R"),
	    plotquantity = cms.untracked.string("deltaR(daughter(0).eta,daughter(0).phi,daughter(1).eta,daughter(1).phi)")
	    )		
		)
)

process.lastPt= cms.EDAnalyzer("CandViewHistoAnalyzer",
							src = cms.InputTag("lastParticles"),
	  # weights = cms.untracked.InputTag("myProducerLabel"),							   
 histograms = cms.VPSet(
	    cms.PSet(
	    min = cms.untracked.double(0.0),
	    max = cms.untracked.double(1000.0),
	    nbins = cms.untracked.int32(100),	
	    name = cms.untracked.string("last_pT"),
	    description = cms.untracked.string("pT [GeV/c]"),
	    plotquantity = cms.untracked.string("pt")
	    )		
		)
)


process.gravPt= cms.EDAnalyzer("CandViewHistoAnalyzer",
							src = cms.InputTag("genParticlesClone"),
	  # weights = cms.untracked.InputTag("myProducerLabel"),							   
 histograms = cms.VPSet(
	    cms.PSet(
	    min = cms.untracked.double(0.0),
	    max = cms.untracked.double(1000.0),
	    nbins = cms.untracked.int32(100),
	    name = cms.untracked.string("graviton_pT"),
	    description = cms.untracked.string("pT [GeV/c]"),
	    plotquantity = cms.untracked.string("pt")
	    )		
		)
)

process.jet0Pt= cms.EDAnalyzer("CandViewHistoAnalyzer",
							src = cms.InputTag("ak5GenJetsClone"),
	  # weights = cms.untracked.InputTag("myProducerLabel"),							   
 histograms = cms.VPSet(
	    cms.PSet(
            itemsToPlot = cms.untracked.int32(5),
	    min = cms.untracked.double(20.0),
	    max = cms.untracked.double(500.0),
	    nbins = cms.untracked.int32(100),
	    name = cms.untracked.string("jet_pT"),
	    description = cms.untracked.string("pT [GeV/c]"),
	    plotquantity = cms.untracked.string("pt")
	    ),
		cms.PSet(
            itemsToPlot = cms.untracked.int32(5),
		min = cms.untracked.double(0.0),
	    max = cms.untracked.double(100.0),
	    nbins = cms.untracked.int32(50),
	    name = cms.untracked.string("jet_mass"),
	    description = cms.untracked.string("mass [GeV/c^2]"),
	    plotquantity = cms.untracked.string("mass")
	    ),
	    cms.PSet(
            itemsToPlot = cms.untracked.int32(5),
	    min = cms.untracked.double(-5.0),
	    max = cms.untracked.double(5.0),
	    nbins = cms.untracked.int32(100),
	    name = cms.untracked.string("jet_rap"),
	    description = cms.untracked.string("rapidity"),
	    plotquantity = cms.untracked.string("rapidity")
	    )						
		)
)


process.isrPt= cms.EDAnalyzer("CandViewHistoAnalyzer",
							src = cms.InputTag("isrJet"),
	  # weights = cms.untracked.InputTag("myProducerLabel"),
 histograms = cms.VPSet(
	    cms.PSet(
	    min = cms.untracked.double(0.0),
	    max = cms.untracked.double(1000.0),
	    nbins = cms.untracked.int32(100),
	    name = cms.untracked.string("isr_pT"),
	    description = cms.untracked.string("pT [GeV/c]"),
	    plotquantity = cms.untracked.string("pt")
	    ),
	    cms.PSet(
	    min = cms.untracked.double(-5.0),
	    max = cms.untracked.double(5.0),
	    nbins = cms.untracked.int32(100),
	    name = cms.untracked.string("isr_rap"),
	    description = cms.untracked.string("rapidity"),
	    plotquantity = cms.untracked.string("rapidity")
	    )				
		)
)

process.myProducerLabel = cms.EDProducer('HistoWeightProducer',
#							  src=cms.InputTag('allJets'),
							  src=cms.InputTag('lastParticles'),
							  outputFile=cms.untracked.string('run10.root'),
							  inputFile=cms.untracked.string('runROOTSUFFIX.root'),
#							  outputFile=cms.untracked.string('run0.root'),
#							  treeName=cms.untracked.string('isrPt/isr_pT')
#							  treeName=cms.untracked.string('sumPt/jet_sumPt'),
 							  treeName=cms.untracked.string('lastPt/last_pT'),
                              reweightVariable=cms.untracked.string('pt')								 
)




process.p = cms.Path(
#	process.generator*process.genParticles
					 process.genParticlesForJets
	*process.lastParticles
					 *process.ak5GenJets
					 *process.ak5GenJetsClone*process.ak5Clone
#					 *process.isrClone
					 *process.genParticlesClone					 
					 *process.isrJet
                     *process.twoJetsClone*process.jetPair*process.allJets
#					 *process.myProducerLabel
					 *process.printGenParticles*process.printGenJets*process.printTwoJets
					 *process.gravPt
					 *process.jet0Pt
					 *process.isrPt
					 *process.lastPt					 
                                         *process.deltaPhi
                                         *process.sumPt
					 )

#process.outpath = cms.EndPath(process.GEN)

#process.schedule = cms.Schedule(process.p, process.outpath)


process.genParticlesClone1 = cms.EDFilter("CandViewShallowCloneProducer",
							 src = cms.InputTag("genParticles"),
    cut = cms.string("pt > 10 & abs( eta ) < 2.5 & status = 1 & ( abs(pdgId)=11 | abs(pdgId)=13 )")							 
							 )


process.rawmuons = cms.EDFilter("PdgIdAndStatusCandSelector",
						   src = cms.InputTag("genParticlesClone1"),
						   pdgId = cms.vint32( 13 ),
						   status = cms.vint32( 1 )							 
)


process.rawelectrons = cms.EDFilter("PdgIdAndStatusCandSelector",
						   src = cms.InputTag("genParticlesClone1"),
						   pdgId = cms.vint32( 11 ),
						   status = cms.vint32( 1 )							 
)

process.jets = cms.EDProducer("GenJetShallowCloneProducer",
							  src = cms.InputTag("ak5GenJets")
							  )

process.selectedMuons = cms.EDFilter("CandSelector",
    src = cms.InputTag("rawmuons"),
    cut = cms.string("pt > 10 & abs( eta ) < 2.4")
  )

process.selectedElectrons = cms.EDFilter("CandSelector",
    src = cms.InputTag("rawelectrons"),
    cut = cms.string("pt > 15 & abs( eta ) < 2.5")
  )

# should require e/mu to be isolated

process.selectedJets = cms.EDFilter("CandSelector",
    src = cms.InputTag("jets"),
    cut = cms.string("pt > 50 & abs( eta ) < 2.5")
  )


process.countFilter = cms.EDFilter("CandCountFilter",
								   src = cms.InputTag("selectedJets"),
								   minNumber = cms.uint32(3)
								   )


process.electronVeto = cms.EDFilter("CandCountFilter",
								   src = cms.InputTag("selectedElectrons"),
								   minNumber = cms.uint32(1)
								   )


process.muonVeto = cms.EDFilter("CandCountFilter",
								   src = cms.InputTag("selectedMuons"),
								   minNumber = cms.uint32(1)
								   )


process.HT = cms.EDProducer("HTProducer",
							JetCollection = cms.InputTag("ak5GenJets"),
							MinJetPt = cms.double(50.0),
							MaxJetEta = cms.double(2.5)
							)

process.MHT = cms.EDProducer("MHTProducer",
							JetCollection = cms.InputTag("ak5GenJets"),
							MinJetPt = cms.double(50.0),
							MaxJetEta = cms.double(2.5)
							)


### HT > 300 GeV from HTProducer

process.htSelect = cms.EDFilter("HTFilter",
    HTSource = cms.InputTag("HT"),
    MinHT = cms.double(300.)
  )
### MHT > 150 GeV for MHTProducer
process.mhtSelect = cms.EDFilter("MHTFilter",
    MHTSource = cms.InputTag("MHT"),
    MinMHT = cms.double(150.)
  )



### deltaR --> deltaPhi -->  mht * jet0 > 0.5 *jet1 > 0.5 *jet2>0.3
process.dphiSelect = cms.EDFilter("JetMHTDPhiFilter",
								  MHTSource = cms.InputTag("MHT"),
								  JetSource = cms.InputTag("ak5GenJets")
								  )


process.extraPlots  = cms.EDAnalyzer("SusyRA2ScanAnalysis",
                                         MHTSource = cms.InputTag("MHT"),
                                         HTSource  = cms.InputTag("HT"),
                                         JetSource = cms.InputTag("ak5GenJets"),
                                         METSource = cms.InputTag("genMetTrue"),
                                         susyScanXSecSrc = cms.InputTag("susyScanCrossSection"),
                                         susyScanMCHSrc  = cms.InputTag("susyScanMCH"),
                                         susyScanMGSrc   = cms.InputTag("susyScanM0"),  # for T2
                                         susyScanMLSPSrc = cms.InputTag("susyScanM12"), # for T2
                                         #susyScanMGSrc   = cms.InputTag("susyScanMG"),  # for T6
                                         #susyScanMLSPSrc = cms.InputTag("susyScanMLSP"),# for T6
                                         susyScanRunSrc  = cms.InputTag("susyScanRun"),
                                         susyScanTopology = cms.string("susyScanTopology")
)

process.missingPt= cms.EDAnalyzer("CandViewHistoAnalyzer",
#							src = cms.InputTag("ak5GenJetsClone"),
							src = cms.InputTag("MHT"),							   
	  # weights = cms.untracked.InputTag("myProducerLabel"),							   
 histograms = cms.VPSet(
	    cms.PSet(
            itemsToPlot = cms.untracked.int32(1),
	    min = cms.untracked.double(0.0),
	    max = cms.untracked.double(1000.0),
	    nbins = cms.untracked.int32(100),
	    name = cms.untracked.string("missing_pT"),
	    description = cms.untracked.string("pT [GeV/c]"),
	    plotquantity = cms.untracked.string("pt")
	    )
		)
)
		


process.jetsPt= cms.EDAnalyzer("CandViewHistoAnalyzer",
#							src = cms.InputTag("ak5GenJetsClone"),
							src = cms.InputTag("ak5GenJets"),							   
	  # weights = cms.untracked.InputTag("myProducerLabel"),							   
 histograms = cms.VPSet(
	    cms.PSet(
            itemsToPlot = cms.untracked.int32(5),
	    min = cms.untracked.double(20.0),
	    max = cms.untracked.double(500.0),
	    nbins = cms.untracked.int32(100),
	    name = cms.untracked.string("jet_pT"),
	    description = cms.untracked.string("pT [GeV/c]"),
	    plotquantity = cms.untracked.string("pt")
	    ),
		cms.PSet(
            itemsToPlot = cms.untracked.int32(5),
		min = cms.untracked.double(0.0),
	    max = cms.untracked.double(100.0),
	    nbins = cms.untracked.int32(50),
	    name = cms.untracked.string("jet_mass"),
	    description = cms.untracked.string("mass [GeV/c^2]"),
	    plotquantity = cms.untracked.string("mass")
	    ),
	    cms.PSet(
            itemsToPlot = cms.untracked.int32(5),
	    min = cms.untracked.double(-5.0),
	    max = cms.untracked.double(5.0),
	    nbins = cms.untracked.int32(100),
	    name = cms.untracked.string("jet_rap"),
	    description = cms.untracked.string("rapidity"),
	    plotquantity = cms.untracked.string("rapidity")
	    )						
		)
)




process.pp = cms.Path(
 	process.genParticlesForJets*
	process.genParticlesClone1 *

process.rawmuons *

process.rawelectrons *

process.jets *

process.selectedMuons *

process.selectedElectrons *

process.selectedJets *

#process.met *

process.countFilter *

~process.electronVeto *

~process.muonVeto *

#process.jets0 *

process.HT *

process.MHT * 

process.htSelect *
process.mhtSelect *
process.dphiSelect *
process.jetsPt*
process.missingPt

*process.extraPlots
					 )



process.schedule = cms.Schedule(process.p,process.pp)

