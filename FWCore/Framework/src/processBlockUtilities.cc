#include "FWCore/Framework/interface/processBlockUtilities.h"

#include "FWCore/Framework/interface/Event.h"

namespace edm {

  unsigned int eventProcessBlockIndex(Event const& event, std::string const& processName) {
    return event.processBlockIndex(processName);
  }

  std::vector<unsigned int> const& eventProcessBlockIndexes(Event const& event) { return event.processBlockIndexes(); }

  std::vector<std::string> const& eventProcessesWithProcessBlockProducts(Event const& event) {
    return event.processesWithProcessBlockProducts();
  }

}  // namespace edm
