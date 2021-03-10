import FWCore.ParameterSet.Config as cms

process = cms.Process("READ")

process.options = cms.untracked.PSet(
    numberOfStreams = cms.untracked.uint32(1),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:testProcessBlockMerge.root'
    )
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('testProcessBlockRead.root')
)

process.intProducerBeginProcessBlockR = cms.EDProducer("IntProducerBeginProcessBlock", ivalue = cms.int32(4))

process.intProducerEndProcessBlockR = cms.EDProducer("IntProducerEndProcessBlock", ivalue = cms.int32(40))

process.readProcessBlocks = cms.EDAnalyzer("edmtest::stream::InputProcessBlockIntAnalyzer")

process.readProcessBlocksG = cms.EDAnalyzer("edmtest::stream::InputProcessBlockIntAnalyzerG",
                                            transitions = cms.int32(0),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", "")
)

process.p = cms.Path(process.intProducerBeginProcessBlockR *
                     process.intProducerEndProcessBlockR *
                     process.readProcessBlocks *
                     process.readProcessBlocksG
)

process.e = cms.EndPath(process.out)
