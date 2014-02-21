import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
	'file:reco_9_1_ZAm.root'
	)
)

process.source.fileNames = [
    'dcap://pnfs/cms/WAX/resilient/spadhi/CMS/spadhi/PhysicsProcesses_TopologyT2_38xFall10/PhysicsProcesses_TopologyT2_38xFall10/f959c379445f9d7\
540d41cf9ffa87a96//reco_9_1_ZAm.root'
]



process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('test2.root')
)


process.genParticlesClone = cms.EDFilter("CandViewShallowCloneProducer",
							 src = cms.InputTag("genParticles"),
    cut = cms.string("pt > 10 & abs( eta ) < 2.4")							 
							 )


process.rawmuons = cms.EDFilter("PdgIdAndStatusCandSelector",
						   src = cms.InputTag("genParticlesClone"),
						   pdgId = cms.vint32( 13 ),
						   status = cms.vint32( 1 )							 
)


process.rawelectrons = cms.EDFilter("PdgIdAndStatusCandSelector",
						   src = cms.InputTag("genParticlesClone"),
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



#process.load("SandBox.Skims.RA2DeltaR_cff")


#process.select = process.Sequence(process.muons*process.electrons)



process.demo  = cms.EDAnalyzer("SusyRA2ScanAnalysis",
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
		


process.jet0Pt= cms.EDAnalyzer("CandViewHistoAnalyzer",
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




process.p = cms.Path(
	process.genParticles*
	process.genParticlesClone *

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
process.jet0Pt*
process.missingPt*

					 process.demo)
