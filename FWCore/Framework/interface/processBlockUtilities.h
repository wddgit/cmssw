#ifndef FWCore_Framework_processBlockUtilities_h
#define FWCore_Framework_processBlockUtilities_h

/**

\author W. David Dagenhart, created 13 January, 2021

*/

#include <string>
#include <vector>

namespace edm {

  class Event;

  unsigned int eventProcessBlockIndex(Event const& event, std::string const& processName);
  std::vector<unsigned int> const& eventProcessBlockIndexes(Event const& event);
  std::vector<std::string> const& eventProcessesWithProcessBlockProducts(Event const& event);

}  // namespace edm
#endif
