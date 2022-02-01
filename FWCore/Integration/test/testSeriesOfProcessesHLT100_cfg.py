import FWCore.ParameterSet.Config as cms

process = cms.Process("HLT")

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(3)
)

process.prod1 = cms.EDProducer("TestParameterSetIDProducer",
  value = cms.uint32(98)
)

process.ana1 = cms.EDAnalyzer("TestParameterSetIDAnalyzer",
  inputTag = cms.untracked.InputTag("prod1")
)

process.out = cms.OutputModule("EventStreamFileWriter",
  fileName = cms.untracked.string('testSeriesOfProcessesHLT100.dat'),
  compression_level = cms.untracked.int32(1),
  use_compression = cms.untracked.bool(True),
  max_event_size = cms.untracked.int32(7000000)
)

process.p01 = cms.Path(process.prod1 * process.ana1)

process.e = cms.EndPath(process.out)
