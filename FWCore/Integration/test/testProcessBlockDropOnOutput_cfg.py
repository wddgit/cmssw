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
        'file:testProcessBlockMergeOfMergedFiles.root',
        'file:testProcessBlockMergeOfMergedFiles2.root'
    ),
)


process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('testProcessBlockDropOnOutput.root'),
    outputCommands = cms.untracked.vstring(
        'keep *',
        'drop *_intProducerBeginProcessBlockB_*_*',
        'drop *_intProducerEndProcessBlockB_*_*',
        'drop *_intProducerBeginProcessBlockM_*_*',
        'drop *_intProducerEndProcessBlockM_*_*'
    )
)

process.testOneOutput = cms.OutputModule("TestOneOutput",
    outputCommands = cms.untracked.vstring(
        'keep *',
        'drop *_intProducerBeginProcessBlockB_*_*',
        'drop *_intProducerEndProcessBlockB_*_*',
        'drop *_intProducerBeginProcessBlockM_*_*',
        'drop *_intProducerEndProcessBlockM_*_*'
    ),
    verbose = cms.untracked.bool(False),
    expectedProcessesWithProcessBlockProducts = cms.untracked.vstring('PROD1', 'MERGEOFMERGED'),
    expectedTopProcessesWithProcessBlockProducts = cms.untracked.vstring('PROD1', 'MERGE', 'MERGEOFMERGED'),
    expectedTranslateFromStoredIndex = cms.untracked.vuint32(0, 2),
    expectedNAddedProcesses = cms.untracked.uint32(0),
    expectedProductsFromInputKept = cms.untracked.bool(True)
)

process.e = cms.EndPath(
    process.out *
    process.testOneOutput
)
