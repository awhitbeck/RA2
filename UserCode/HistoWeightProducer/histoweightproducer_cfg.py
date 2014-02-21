import FWCore.ParameterSet.Config as cms

process = cms.Process("OWNPARTICLES")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:pythia8ex2.root'
    )
)

process.myProducerLabel = cms.EDProducer('HistoWeightProducer',
							  src=cms.InputTag('isrJet'),
							  inputFile=cms.untracked.string('runx10.root'),
							  outputFile=cms.untracked.string('runx1.root'),
							  treeName=cms.untracked.string('isrPt/isr_pT')
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('myOutputFile.root')
)

  
process.p = cms.Path(process.myProducerLabel)

process.e = cms.EndPath(process.out)
