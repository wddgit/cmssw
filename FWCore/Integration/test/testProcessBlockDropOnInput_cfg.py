import FWCore.ParameterSet.Config as cms

process = cms.Process("READ")

process.options = cms.untracked.PSet(
    numberOfStreams = cms.untracked.uint32(1),
    numberOfThreads = cms.untracked.uint32(1),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:testProcessBlockMergeOfMergedFiles.root'
    ),
    inputCommands=cms.untracked.vstring(
        'keep *',
        'drop *_intProducerBeginProcessBlockB_*_*',
        'drop *_intProducerEndProcessBlockB_*_*',
        'drop *_intProducerBeginProcessBlockM_*_*',
        'drop *_intProducerEndProcessBlockM_*_*'
    )
)

process.readProcessBlocksOneAnalyzer = cms.EDAnalyzer("edmtest::one::InputProcessBlockIntAnalyzer",
                                            transitions = cms.int32(29),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 11, 22, 3300, 4400),
                                            expectedSum = cms.int32(7733),
                                            consumesProcessBlockNotFound1 = cms.InputTag("intProducerBeginProcessBlockB"),
                                            consumesProcessBlockNotFound2 = cms.InputTag("intProducerEndProcessBlockB"),
                                            consumesProcessBlockNotFound3 = cms.InputTag("intProducerBeginProcessBlockM"),
                                            consumesProcessBlockNotFound4 = cms.InputTag("intProducerEndProcessBlockM")
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('testProcessBlockDropOnInput.root')
)

process.testOneOutput = cms.OutputModule("TestOneOutput",
    verbose = cms.untracked.bool(False),
    expectedProcessesWithProcessBlockProducts = cms.untracked.vstring('PROD1', 'MERGEOFMERGED'),
    expectedTopProcessesWithProcessBlockProducts = cms.untracked.vstring('PROD1', 'MERGEOFMERGED'),
    expectedTopCacheIndices = cms.untracked.vuint32(0, 4, 1, 4, 2, 4, 3, 4),
    expectedWriteProcessBlockTransitions = cms.untracked.int32(6)
)


process.p = cms.Path(process.readProcessBlocksOneAnalyzer)

process.e = cms.EndPath(
    process.out *
    process.testOneOutput
)
