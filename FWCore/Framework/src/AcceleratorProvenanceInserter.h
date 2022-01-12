#ifndef FWCore_Framework_AcceleratorProvenanceInserter_h
#define FWCore_Framework_AcceleratorProvenanceInserter_h

/** \class edm::AcceleratorProvenanceInserter

\author W. David Dagenhart, created 12 Jan, 2022
*/

#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Framework/interface/moduleAbilities.h"
#include "FWCore/Utilities/interface/EDPutToken.h"

namespace edm {
  class AcceleratorProvenance;
  class Event;
  class EventSetup;
  class ParameterSet;
  class ProcessBlock;
  class StreamID;

  class AcceleratorProvenanceInserter : public global::EDProducer<BeginProcessBlockProducer> {
  public:
    explicit AcceleratorProvenanceInserter(ParameterSet const&);
    void beginProcessBlockProduce(ProcessBlock&) override;
    void produce(StreamID, Event&, EventSetup const&) const override;

  private:
    EDPutTokenT<AcceleratorProvenance> putToken_;
  };
}  // namespace edm
#endif
