import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.options = cms.untracked.PSet(
    numberOfStreams = cms.untracked.uint32(1),
    numberOfThreads = cms.untracked.uint32(1),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:testProcessBlockMerge.root'
    )
)

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
    expectedProcessesWithProcessBlockProducts = cms.untracked.vstring('PROD1', 'MERGE'),
    expectedWriteProcessBlockTransitions = cms.untracked.int32(4),
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
    process.nonEventIntProducer
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
    expectedProcessesWithProcessBlockProducts = cms.untracked.vstring('PROD1', 'MERGE'),
    expectedWriteProcessBlockTransitions = cms.untracked.int32(4),
    outputCommands = cms.untracked.vstring(
        "keep *",
        "drop *_*_beginProcessBlock_*",
        "drop *_*_endProcessBlock_*"
    )
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
    expectedProcessesWithProcessBlockProducts = cms.untracked.vstring('PROD1', 'MERGE', 'READAGAIN'),
    expectedWriteProcessBlockTransitions = cms.untracked.int32(4),
    outputCommands = cms.untracked.vstring(
        "keep *",
        "drop *_*_beginProcessBlock_*",
        "drop *_*_endProcessBlock_*"
    )
)

readAgainProcess.p = cms.Path(
    readAgainProcess.intProducerBeginProcessBlockRA *
    readAgainProcess.intProducerEndProcessBlockRA
)

readAgainProcess.e = cms.EndPath(
    readAgainProcess.out *
    readAgainProcess.testOneOutput
)
