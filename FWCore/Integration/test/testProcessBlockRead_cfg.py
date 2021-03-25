import FWCore.ParameterSet.Config as cms

process = cms.Process("READ")

#process.Tracer = cms.Service("Tracer")
process.Tracer = cms.Service("Tracer")

process.options = cms.untracked.PSet(
    #numberOfStreams = cms.untracked.uint32(4),
    #numberOfThreads = cms.untracked.uint32(4),
    #numberOfConcurrentRuns = cms.untracked.uint32(1),
    #numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(4)
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
    fileName = cms.untracked.string('testProcessBlockRead.root')
)

process.intProducerBeginProcessBlockR = cms.EDProducer("IntProducerBeginProcessBlock", ivalue = cms.int32(5))

process.intProducerEndProcessBlockR = cms.EDProducer("IntProducerEndProcessBlock", ivalue = cms.int32(50))

process.readProcessBlocks = cms.EDAnalyzer("edmtest::stream::InputProcessBlockIntAnalyzer",
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            expectedByRun = cms.vint32(0, 11, 22),
                                            sleepTime = cms.uint32(10000)
)

process.readProcessBlocksG = cms.EDAnalyzer("edmtest::stream::InputProcessBlockIntAnalyzerG",
                                            transitions = cms.int32(15),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 11, 22),
                                            expectedSum = cms.int32(77),
                                            sleepTime = cms.uint32(10000)
)

process.readProcessBlocksNS = cms.EDAnalyzer("edmtest::stream::InputProcessBlockIntAnalyzerNS")
process.readProcessBlocksGNS = cms.EDAnalyzer("edmtest::stream::InputProcessBlockIntAnalyzerGNS")

process.readProcessBlocksStreamFilter = cms.EDFilter("edmtest::stream::InputProcessBlockIntFilter",
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            expectedByRun = cms.vint32(0, 11, 22),
                                            sleepTime = cms.uint32(10000)
)

process.readProcessBlocksGStreamFilter = cms.EDFilter("edmtest::stream::InputProcessBlockIntFilterG",
                                            transitions = cms.int32(15),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 11, 22),
                                            expectedSum = cms.int32(77),
                                            sleepTime = cms.uint32(10000)
)

process.readProcessBlocksStreamProducer = cms.EDProducer("edmtest::stream::InputProcessBlockIntProducer",
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            expectedByRun = cms.vint32(0, 11, 22),
                                            sleepTime = cms.uint32(10000)
)

process.readProcessBlocksGStreamProducer = cms.EDProducer("edmtest::stream::InputProcessBlockIntProducerG",
                                            transitions = cms.int32(15),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 11, 22),
                                            expectedSum = cms.int32(77),
                                            sleepTime = cms.uint32(10000)
)

process.readProcessBlocksOneAnalyzer = cms.EDAnalyzer("edmtest::one::InputProcessBlockIntAnalyzer",
                                            transitions = cms.int32(15),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 11, 22),
                                            expectedSum = cms.int32(77)
)

process.readProcessBlocksOneFilter = cms.EDFilter("edmtest::one::InputProcessBlockIntFilter",
                                            transitions = cms.int32(15),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 11, 22),
                                            expectedSum = cms.int32(77)
)

process.readProcessBlocksOneProducer = cms.EDProducer("edmtest::one::InputProcessBlockIntProducer",
                                            transitions = cms.int32(15),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 11, 22),
                                            expectedSum = cms.int32(77)
)

process.readProcessBlocksGlobalAnalyzer = cms.EDAnalyzer("edmtest::global::InputProcessBlockIntAnalyzer",
                                            transitions = cms.int32(15),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 11, 22),
                                            expectedSum = cms.int32(77)
)

process.readProcessBlocksGlobalFilter = cms.EDFilter("edmtest::global::InputProcessBlockIntFilter",
                                            transitions = cms.int32(15),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 11, 22),
                                            expectedSum = cms.int32(77)
)

process.readProcessBlocksGlobalProducer = cms.EDProducer("edmtest::global::InputProcessBlockIntProducer",
                                            transitions = cms.int32(15),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 11, 22),
                                            expectedSum = cms.int32(77)
)

process.readProcessBlocksLimitedAnalyzer = cms.EDAnalyzer("edmtest::limited::InputProcessBlockIntAnalyzer",
                                            transitions = cms.int32(15),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 11, 22),
                                            expectedSum = cms.int32(77),
                                            concurrencyLimit = cms.untracked.uint32(4)
)

process.readProcessBlocksLimitedFilter = cms.EDFilter("edmtest::limited::InputProcessBlockIntFilter",
                                            transitions = cms.int32(15),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 11, 22),
                                            expectedSum = cms.int32(77),
                                            concurrencyLimit = cms.untracked.uint32(4)
)

process.readProcessBlocksLimitedProducer = cms.EDProducer("edmtest::limited::InputProcessBlockIntProducer",
                                            transitions = cms.int32(15),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 11, 22),
                                            expectedSum = cms.int32(77),
                                            concurrencyLimit = cms.untracked.uint32(4)
)

process.readProcessBlocksGlobalAnalyzerNoRegistration = cms.EDAnalyzer("edmtest::global::InputProcessBlockIntAnalyzerNoRegistration",
                                            transitions = cms.int32(6),
)

process.readProcessBlocksDoesNotExist = cms.EDAnalyzer("edmtest::global::InputProcessBlockAnalyzerThreeTags",
                                            transitions = cms.int32(6),
                                            consumesBeginProcessBlock0 = cms.InputTag("doesNotExist", ""),
                                            consumesEndProcessBlock0 = cms.InputTag("doesNotExist", ""),
                                            consumesBeginProcessBlock1 = cms.InputTag("doesNotExist", ""),
                                            consumesEndProcessBlock1 = cms.InputTag("doesNotExist", ""),
                                            consumesBeginProcessBlock2 = cms.InputTag("doesNotExist", ""),
                                            consumesEndProcessBlock2 = cms.InputTag("doesNotExist", ""),
                                            expectedByRun0 = cms.vint32(),
                                            expectedByRun1 = cms.vint32(),
                                            expectedByRun2 = cms.vint32(0, 11, 22)
)

process.p = cms.Path(process.intProducerBeginProcessBlockR *
                     process.intProducerEndProcessBlockR *
                     process.readProcessBlocks *
                     process.readProcessBlocksG *
                     process.readProcessBlocksNS *
                     process.readProcessBlocksGNS *
                     process.readProcessBlocksStreamFilter *
                     process.readProcessBlocksGStreamFilter *
                     process.readProcessBlocksStreamProducer *
                     process.readProcessBlocksGStreamProducer *
                     process.readProcessBlocksOneAnalyzer *
                     process.readProcessBlocksOneFilter *
                     process.readProcessBlocksOneProducer *
                     process.readProcessBlocksGlobalAnalyzer *
                     process.readProcessBlocksGlobalFilter *
                     process.readProcessBlocksGlobalProducer *
                     process.readProcessBlocksLimitedAnalyzer *
                     process.readProcessBlocksLimitedFilter *
                     process.readProcessBlocksLimitedProducer *
                     process.readProcessBlocksDoesNotExist *
                     process.readProcessBlocksGlobalAnalyzerNoRegistration
)

process.e = cms.EndPath(process.out)
