#include "DataFormats/Provenance/interface/StoredProcessBlockHelper.h"

#include <algorithm>
#include <cassert>

namespace edm {

  StoredProcessBlockHelper::StoredProcessBlockHelper() {}

  StoredProcessBlockHelper::StoredProcessBlockHelper(std::vector<std::string> const& processesWithProcessBlockProducts)
      : processesWithProcessBlockProducts_(processesWithProcessBlockProducts) {}

  bool StoredProcessBlockHelper::dropProcessesAndReorder(std::set<std::string> const& processesToKeep,
                                                         std::vector<std::string> const& firstFileOrder,
                                                         unsigned int nProcessesInFirstFile,
                                                         std::vector<unsigned int> const& nEntries,
                                                         std::vector<unsigned int>& firstFileToStored,
                                                         bool firstInputFile) {
    // Modify the vector of process names and the container of cache indices
    // by dropping processes and/or reordering them. Processes are dropped
    // if all ProcessBlock products associated with a process were dropped
    // on input. Also reorder the processes for input files after the first
    // file to have the same order as in the first input file.

    // It is OK if there are processes missing that were in the first
    // input file. This function does not add them.

    // processToKeep contains the processes that have kept ProcessBlock
    // products (not dropped on input).

    // firstFileOrder provides the process order from the first input file.
    // If this is being called for the first input file, then firstFileOrder
    // IS the vector of processes in this object. In this case,
    // the order does not change, but processes may need to be dropped.
    // If there are extra process names that are not in firstFileOrder,
    // then they are silently dropped, but note that in another
    // function called later that merges ProductRegistries an
    // exception will be thrown in this case.

    // nEntries provides the number of entries in the ProcessBlock
    // TTree for each process.

    // firstFileToStored is filled with the association between the indexes
    // into the process names vector in the first input file and the
    // current StoredProcessBlockHelper

    std::vector<std::string>& processes = processesWithProcessBlockProducts_;
    assert(!processes.empty());

    bool isModified = false;

    // Return early if nothing needs to be modified.
    if (firstInputFile || (processes.size() == nProcessesInFirstFile &&
                           std::equal(processes.begin(), processes.end(), firstFileOrder.begin()))) {
      bool keepAllProcesses = true;
      for (auto const& process : processes) {
        if (processesToKeep.find(process) == processesToKeep.end()) {
          keepAllProcesses = false;
          break;
        }
      }
      if (keepAllProcesses) {
        isModified = false;
        return isModified;
      }
      isModified = true;
    }

    assert(nProcessesInFirstFile <= firstFileOrder.size());
    firstFileToStored.assign(nProcessesInFirstFile, invalidProcessIndex());
    unsigned int finalSize = 0;
    if (firstInputFile) {
      for (unsigned int index = 0; index < processes.size(); ++index) {
        if (processesToKeep.find(processes[index]) != processesToKeep.end()) {
          // The position and value are the same because for the first
          // file because the vectors of process names are the same vector.
          // Note that if not kept the translation value is left invalid.
          firstFileToStored[index] = index;
          ++finalSize;
        }
      }
    } else {
      for (unsigned int iFirst = 0; iFirst < nProcessesInFirstFile; ++iFirst) {
        for (unsigned int iStored = 0; iStored < processes.size(); ++iStored) {
          std::string const& process = processes[iStored];
          if (firstFileOrder[iFirst] == process) {
            if (processesToKeep.find(process) != processesToKeep.end()) {
              if (finalSize != iStored) {
                isModified = true;
              }
              firstFileToStored[iFirst] = iStored;
              ++finalSize;
            }
            break;
          }
        }
      }
      if (finalSize != processes.size()) {
        isModified = true;
      }
      if (!isModified) {
        return isModified;
      }
    }

    std::vector<unsigned int> oldOffsets;
    oldOffsets.reserve(processes.size());
    unsigned int offset = 0;
    for (auto const& entries : nEntries) {
      oldOffsets.push_back(offset);
      offset += entries;
    }
    assert(oldOffsets.size() == processes.size());

    std::vector<unsigned int> newOffsets;
    newOffsets.reserve(nProcessesInFirstFile);
    offset = 0;
    for (unsigned int iNew = 0; iNew < nProcessesInFirstFile; ++iNew) {
      newOffsets.push_back(offset);
      unsigned int iOld = firstFileToStored[iNew];
      if (iOld != invalidProcessIndex()) {
        offset += nEntries[iOld];
      }
    }

    std::vector<unsigned int> const& oldCacheIndices = processBlockCacheIndices_;
    std::vector<unsigned int> newCacheIndices;

    unsigned int nOuterLoop = oldCacheIndices.size() / processes.size();
    assert(nOuterLoop * processes.size() == oldCacheIndices.size());
    newCacheIndices.reserve(nOuterLoop * finalSize);
    unsigned int oldOuterOffset = 0;

    for (unsigned int k = 0; k < nOuterLoop; ++k) {
      for (unsigned int j = 0; j < nProcessesInFirstFile; ++j) {
        if (firstFileToStored[j] != invalidProcessIndex()) {
          unsigned int oldCacheIndex = oldCacheIndices[oldOuterOffset + firstFileToStored[j]];
          unsigned int newCacheIndex = invalidCacheIndex();
          if (oldCacheIndex != invalidCacheIndex()) {
            newCacheIndex = oldCacheIndex - oldOffsets[firstFileToStored[j]] + newOffsets[j];
          }
          newCacheIndices.push_back(newCacheIndex);
        }
      }
      oldOuterOffset += processes.size();
    }
    processBlockCacheIndices_.swap(newCacheIndices);

    std::vector<std::string> newProcessNames;
    newProcessNames.reserve(finalSize);
    for (unsigned int j = 0; j < nProcessesInFirstFile; ++j) {
      if (firstFileToStored[j] != invalidProcessIndex()) {
        newProcessNames.push_back(firstFileOrder[j]);
      }
    }
    processes.swap(newProcessNames);

    return isModified;
  }

}  // namespace edm
