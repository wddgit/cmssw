import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring('file:testSeriesOfProcessesPROD100.root')
)

process.prod3 = cms.EDProducer("TestParameterSetIDProducer",
  value = cms.uint32(100)
)

process.ana4 = cms.EDAnalyzer("TestParameterSetIDAnalyzer",
  inputTag = cms.untracked.InputTag("prod1")
)

process.ana5 = cms.EDAnalyzer("TestParameterSetIDAnalyzer",
  inputTag = cms.untracked.InputTag("prod2")
)

process.ana6 = cms.EDAnalyzer("TestParameterSetIDAnalyzer",
  inputTag = cms.untracked.InputTag("prod3")
)

process.path1 = cms.Path(process.prod3 * process.ana4 * process.ana5 * process.ana6)
