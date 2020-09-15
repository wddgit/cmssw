#include "FWCore/Framework/interface/FileBlock.h"

#include <algorithm>

namespace edm {
  TTree* FileBlock::processBlockTree(std::string const& processName) const {
    auto it = std::find(processesWithProcessBlockTrees_.begin(), processesWithProcessBlockTrees_.end(), processName);
    if (it != processesWithProcessBlockTrees_.end()) {
      auto index = std::distance(processesWithProcessBlockTrees_.begin(), it);
      return processBlockTrees_[index];
    }
    return nullptr;
  }
}  // namespace edm
