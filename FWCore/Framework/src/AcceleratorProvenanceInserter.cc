
#include "FWCore/Framework/src/AcceleratorProvenanceInserter.h"
#include "DataFormats/Common/interface/AcceleratorProvenance.h"
#include "FWCore/Framework/interface/ProcessBlock.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "FWCore/Utilities/interface/Transition.h"

namespace edm {

  AcceleratorProvenanceInserter::AcceleratorProvenanceInserter(ParameterSet const&)
      : putToken_(produces<AcceleratorProvenance, edm::Transition::BeginProcessBlock>()) {}

  void AcceleratorProvenanceInserter::beginProcessBlockProduce(ProcessBlock& processBlock) {
    processBlock.emplace(putToken_);
  }

  void AcceleratorProvenanceInserter::produce(StreamID, Event&, EventSetup const&) const {}

}  // namespace edm
