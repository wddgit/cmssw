import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")

process.source = cms.Source("NewEventStreamFileReader",
  fileNames = cms.untracked.vstring('file:testSeriesOfProcessesHLT100.dat')
)

process.prod2 = cms.EDProducer("TestParameterSetIDProducer",
  value = cms.uint32(99)
)

process.ana2 = cms.EDAnalyzer("TestParameterSetIDAnalyzer",
  inputTag = cms.untracked.InputTag("prod1")
)

process.ana3 = cms.EDAnalyzer("TestParameterSetIDAnalyzer",
  inputTag = cms.untracked.InputTag("prod2")
)

process.out = cms.OutputModule("PoolOutputModule",
  fileName = cms.untracked.string('testSeriesOfProcessesPROD100.root')
)

process.p1 = cms.Path(process.prod2 * process.ana2 * process.ana3)

process.e = cms.EndPath(process.out)
