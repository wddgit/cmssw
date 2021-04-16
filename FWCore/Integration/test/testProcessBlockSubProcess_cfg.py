import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.Tracer = cms.Service("Tracer")

process.options = cms.untracked.PSet(
    numberOfStreams = cms.untracked.uint32(1),
    numberOfThreads = cms.untracked.uint32(1),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:testProcessBlockMergeOfMergedFiles.root'
    )
)

process.intProducerBeginProcessBlockT = cms.EDProducer("IntProducerBeginProcessBlock", ivalue = cms.int32(4000))

process.intProducerEndProcessBlockT = cms.EDProducer("IntProducerEndProcessBlock", ivalue = cms.int32(40000))

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('testProcessBlockSubProcessTest.root'),
    outputCommands = cms.untracked.vstring(
        "keep *",
        "drop *_*_beginProcessBlock_*",
        "drop *_*_endProcessBlock_*"
    )
)

process.testOneOutput = cms.OutputModule("TestOneOutput",
    verbose = cms.untracked.bool(False),
    expectedProcessesWithProcessBlockProducts = cms.untracked.vstring('PROD1', 'MERGE', 'MERGEOFMERGED', 'TEST'),
    expectedWriteProcessBlockTransitions = cms.untracked.int32(8),
    outputCommands = cms.untracked.vstring(
        "keep *",
        "drop *_*_beginProcessBlock_*",
        "drop *_*_endProcessBlock_*"
    )
)

process.eventIntProducer = cms.EDProducer("IntProducer", ivalue = cms.int32(1))

process.transientIntProducerEndProcessBlock = cms.EDProducer("TransientIntProducerEndProcessBlock",
    ivalue = cms.int32(90)
)

process.nonEventIntProducer = cms.EDProducer("NonEventIntProducer",
    ivalue = cms.int32(1)
)

process.p = cms.Path(
    process.eventIntProducer *
    process.transientIntProducerEndProcessBlock *
    process.nonEventIntProducer *
    process.intProducerBeginProcessBlockT *
    process.intProducerEndProcessBlockT
)

process.e = cms.EndPath(
    process.out *
    process.testOneOutput
)

readProcess = cms.Process("READ")
process.addSubProcess(cms.SubProcess(readProcess,
    outputCommands = cms.untracked.vstring(
        "keep *"
    )
))

readProcess.intProducerBeginProcessBlockR = cms.EDProducer("IntProducerBeginProcessBlock", ivalue = cms.int32(5000))

readProcess.intProducerEndProcessBlockR = cms.EDProducer("IntProducerEndProcessBlock", ivalue = cms.int32(50000))

readProcess.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('testProcessBlockSubProcessRead.root'),
    outputCommands = cms.untracked.vstring(
        "keep *",
        "drop *_*_beginProcessBlock_*",
        "drop *_*_endProcessBlock_*"
    )
)

readProcess.testOneOutput = cms.OutputModule("TestOneOutput",
    verbose = cms.untracked.bool(False),
    expectedProcessesWithProcessBlockProducts = cms.untracked.vstring('PROD1', 'MERGE', 'MERGEOFMERGED', 'TEST',  'READ'),
    expectedWriteProcessBlockTransitions = cms.untracked.int32(8),
    outputCommands = cms.untracked.vstring(
        "keep *",
        "drop *_*_beginProcessBlock_*",
        "drop *_*_endProcessBlock_*"
    )
)

readProcess.p = cms.Path(
    readProcess.intProducerBeginProcessBlockR *
    readProcess.intProducerEndProcessBlockR
)

readProcess.e = cms.EndPath(
    readProcess.out *
    readProcess.testOneOutput
)

readAgainProcess = cms.Process("READAGAIN")
readProcess.addSubProcess(cms.SubProcess(readAgainProcess,
    outputCommands = cms.untracked.vstring(
        "keep *"
    )
))

readAgainProcess.readProcessBlocksOneAnalyzer1 = cms.EDAnalyzer("edmtest::one::InputProcessBlockIntAnalyzer",
                                            transitions = cms.int32(33),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 11, 22, 3300, 4400),
                                            expectedSum = cms.int32(8221)
)

readAgainProcess.readProcessBlocksOneAnalyzer2 = cms.EDAnalyzer("edmtest::one::InputProcessBlockIntAnalyzer",
                                            transitions = cms.int32(27),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 44, 44, 444, 444),
                                            expectedSum = cms.int32(488)
)

readAgainProcess.readProcessBlocksOneAnalyzer3 = cms.EDAnalyzer("edmtest::one::InputProcessBlockIntAnalyzer",
                                            transitions = cms.int32(24),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlockMM", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlockMM", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 644, 644, 644, 644),
                                            expectedSum = cms.int32(488)
)

readAgainProcess.readProcessBlocksOneAnalyzer4 = cms.EDAnalyzer("edmtest::one::InputProcessBlockIntAnalyzer",
                                            transitions = cms.int32(24),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlockT", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlockT", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            # The expectedByRun test cannot work because the data is from an earlier SubProcess
                                            expectedByRun = cms.vint32(),
                                            expectedFillerSum = cms.untracked.int32(132000),
                                            expectedSum = cms.int32(488)
)

readAgainProcess.readProcessBlocksOneAnalyzer5 = cms.EDAnalyzer("edmtest::one::InputProcessBlockIntAnalyzer",
                                            transitions = cms.int32(24),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlockR", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlockR", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            # The expectedByRun test cannot work because the data is from an earlier SubProcess
                                            expectedByRun = cms.vint32(),
                                            expectedFillerSum = cms.untracked.int32(165000),
                                            expectedSum = cms.int32(488),
                                            consumesBeginProcessBlockNotFound = cms.InputTag("intProducerBeginProcessBlockT"),
                                            consumesEndProcessBlockNotFound = cms.InputTag("intProducerEndProcessBlockT")
)

readAgainProcess.intProducerBeginProcessBlockRA = cms.EDProducer("IntProducerBeginProcessBlock", ivalue = cms.int32(100000))

readAgainProcess.intProducerEndProcessBlockRA = cms.EDProducer("IntProducerEndProcessBlock", ivalue = cms.int32(1000000))

readAgainProcess.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('testProcessBlockSubProcessReadAgain.root'),
    outputCommands = cms.untracked.vstring(
        "keep *",
        "drop *_*_beginProcessBlock_*",
        "drop *_*_endProcessBlock_*"
    )
)

readAgainProcess.testOneOutput = cms.OutputModule("TestOneOutput",
    verbose = cms.untracked.bool(False),
    expectedProcessesWithProcessBlockProducts = cms.untracked.vstring('PROD1', 'MERGE', 'MERGEOFMERGED', 'TEST',  'READ', 'READAGAIN'),
    expectedWriteProcessBlockTransitions = cms.untracked.int32(8),
    outputCommands = cms.untracked.vstring(
        "keep *",
        "drop *_*_beginProcessBlock_*",
        "drop *_*_endProcessBlock_*"
    )
)

readAgainProcess.p = cms.Path(
    readAgainProcess.intProducerBeginProcessBlockRA *
    readAgainProcess.intProducerEndProcessBlockRA *
    readAgainProcess.readProcessBlocksOneAnalyzer1 *
    readAgainProcess.readProcessBlocksOneAnalyzer2 *
    readAgainProcess.readProcessBlocksOneAnalyzer3 *
    readAgainProcess.readProcessBlocksOneAnalyzer4 *
    readAgainProcess.readProcessBlocksOneAnalyzer5
)

readAgainProcess.e = cms.EndPath(
    readAgainProcess.out *
    readAgainProcess.testOneOutput
)
