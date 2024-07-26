import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.MessageLogger = cms.Service("MessageLogger")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20)
)

process.source = cms.Source("EmptySource",
    numberEventsInRun = cms.untracked.uint32(3)
)

# Tries to read a WhatsIt object
process.demo = cms.EDAnalyzer("WhatsItAnalyzer",
    expectedValues = cms.untracked.vint32(0)
)
#produces a WhatsIt object in the EventSetup
process.WhatsItESProducer = cms.ESProducer("WhatsItESProducer")

# Test intentionally throws an exception because this ESSource is missing
# and WhatsItProducer tries to get a Doodad object
#process.DoodadESSource = cms.ESSource("DoodadESSource")


process.bad = cms.ESSource("EmptyESSource",
    recordName = cms.string('GadgetRcd'),
    iovIsRunNotTime = cms.bool(True),
    firstValid = cms.vuint32(1)
)

process.p = cms.Path(process.demo)
