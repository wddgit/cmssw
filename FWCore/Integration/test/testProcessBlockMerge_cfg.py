import FWCore.ParameterSet.Config as cms

process = cms.Process("MERGE")

process.options = cms.untracked.PSet(
    numberOfStreams = cms.untracked.uint32(1),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:testProcessBlock1.root',
        'file:testProcessBlock3.root'
    )
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('testProcessBlockMerge.root')
)

process.intProducerBeginProcessBlockM = cms.EDProducer("IntProducerBeginProcessBlock", ivalue = cms.int32(4))

process.intProducerEndProcessBlockM = cms.EDProducer("IntProducerEndProcessBlock", ivalue = cms.int32(40))

process.p = cms.Path(process.intProducerBeginProcessBlockM *
                     process.intProducerEndProcessBlockM)

process.e = cms.EndPath(process.out)
