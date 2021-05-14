import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
#process.Tracer = cms.Service('Tracer')

import FWCore.Framework.test.cmsExceptionsFatalOption_cff

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:testProcessBlockMerge.root',
        'file:testProcessBlockMerge2.root'
    )
)

# 50 transitions = 14 events + 12 access input ProcessBlock transitions + 24 fill calls 
process.readProcessBlocksOneAnalyzer = cms.EDAnalyzer("edmtest::one::InputProcessBlockIntAnalyzer",
                                            transitions = cms.int32(50),
                                            consumesBeginProcessBlock = cms.InputTag("intProducerBeginProcessBlock", ""),
                                            consumesEndProcessBlock = cms.InputTag("intProducerEndProcessBlock", ""),
                                            consumesBeginProcessBlockM = cms.InputTag("intProducerBeginProcessBlockM", ""),
                                            consumesEndProcessBlockM = cms.InputTag("intProducerEndProcessBlockM", ""),
                                            expectedByRun = cms.vint32(0, 11, 22, 3300, 4400),
                                            expectedSum = cms.int32(16442)
)

process.testOneOutput = cms.OutputModule("TestOneOutput",
    verbose = cms.untracked.bool(False),
    expectedProcessesWithProcessBlockProducts = cms.untracked.vstring('PROD1', 'MERGE'),
    expectedProcessNamesAtWrite = cms.untracked.vstring('PROD1', 'PROD1', 'MERGE', 'PROD1', 'PROD1', 'MERGE', 'PROD1', 'PROD1', 'MERGE', 'PROD1', 'PROD1', 'MERGE'),
    expectedWriteProcessBlockTransitions = cms.untracked.int32(13),
    testTTreesInFileBlock = cms.untracked.bool(True),
    expectedCacheIndexSize = cms.untracked.vuint32(2, 2, 2, 4, 4, 4, 6, 6, 6, 8, 8, 8)
)

process.looper = cms.Looper("NavigateEventsLooper")

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('file:testLooperEventNavigation2.root'),
    fastCloning = cms.untracked.bool(False)
)

process.path1 = cms.Path(process.readProcessBlocksOneAnalyzer)
process.endpath1 = cms.EndPath(process.out * process.testOneOutput)
