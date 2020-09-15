#include "FWCore/Common/interface/ProcessBlockHelper.h"

#include "DataFormats/Provenance/interface/EventToProcessBlockIndexes.h"
#include "DataFormats/Provenance/interface/StoredProcessBlockHelper.h"

#include <cassert>
#include <string>
#include <utility>

namespace edm {

  ProcessBlockHelperBase const* ProcessBlockHelper::topProcessBlockHelper() const { return this; }

  std::vector<std::string> const& ProcessBlockHelper::topProcessesWithProcessBlockProducts() const {
    return processesWithProcessBlockProducts();
  }

  unsigned int ProcessBlockHelper::nProcessesInFirstFile() const { return nProcessesInFirstFile_; }

  std::vector<std::vector<unsigned int>> const& ProcessBlockHelper::processBlockCacheIndices() const {
    return processBlockCacheIndices_;
  }

  std::vector<std::vector<unsigned int>> const& ProcessBlockHelper::nEntries() const { return nEntries_; }

  std::vector<unsigned int> const& ProcessBlockHelper::cacheIndexVectorsPerFile() const {
    return cacheIndexVectorsPerFile_;
  }

  std::vector<unsigned int> const& ProcessBlockHelper::cacheEntriesPerFile() const { return cacheEntriesPerFile_; }

  unsigned int ProcessBlockHelper::processBlockIndex(
      std::string const& processName, EventToProcessBlockIndexes const& eventToProcessBlockIndexes) const {
    for (unsigned int iProcess = 0; iProcess < nProcessesInFirstFile_; ++iProcess) {
      if (processName == processesWithProcessBlockProducts()[iProcess]) {
        return processBlockCacheIndices_[eventToProcessBlockIndexes.index()][iProcess];
      }
    }
    return invalidCacheIndex();
  }

  std::vector<unsigned int> const& ProcessBlockHelper::processBlockIndexes(
      EventToProcessBlockIndexes const& eventToProcessBlockIndexes) const {
    return processBlockCacheIndices_[eventToProcessBlockIndexes.index()];
  }

  void ProcessBlockHelper::initializeFromPrimaryInput(StoredProcessBlockHelper const& storedProcessBlockHelper,
                                                      std::vector<unsigned int>&& nEntries) {
    // The storedProcessBlockHelper holds information read from the input file.

    // Note that at the time this function is called drop on input has already
    // occurred and all traces of the ProcessBlock products dropped on input
    // have been removed. It is as if they had never been kept in
    // the input files. We don't need to worry about drop on input in
    // this function.

    std::vector<std::string> const& storedProcesses = storedProcessBlockHelper.processesWithProcessBlockProducts();
    std::vector<unsigned int> const& storedCacheIndices = storedProcessBlockHelper.processBlockCacheIndices();

    if (!initializedFromInput_) {
      // Initialization for the first input file
      initializedFromInput_ = true;

      assert(processesWithProcessBlockProducts().empty());
      processesWithProcessBlockProducts() = storedProcesses;
      nProcessesInFirstFile_ = processesWithProcessBlockProducts().size();

      if (storedProcesses.empty()) {
        return;
      }
      // Fill the cache index container from the input file.
      // Iterate over the outer vector of the container to be filled
      // and for each element iterate over the inner vector.
      // Note the stored vector is flattened to optimize I/O,
      // there is only one vector, not vectors in a vector.
      unsigned int nOuterLoop = storedCacheIndices.size() / storedProcesses.size();
      assert(storedProcesses.size() * nOuterLoop == storedCacheIndices.size());
      assert(processBlockCacheIndices_.empty());
      processBlockCacheIndices_.resize(nOuterLoop);
      cacheIndexVectorsPerFile_.push_back(nOuterLoop);
      unsigned int storedIndex = 0;
      for (auto& cacheIndexVector : processBlockCacheIndices_) {
        cacheIndexVector.resize(storedProcesses.size(), 0);
        for (auto& cacheIndex : cacheIndexVector) {
          cacheIndex = storedCacheIndices[storedIndex];
          ++storedIndex;
        }
      }
      unsigned int entriesThisFile = 0;
      for (auto const& entries : nEntries) {
        entriesThisFile += entries;
      }
      nEntries_.push_back(std::move(nEntries));
      cacheEntriesPerFile_.push_back(entriesThisFile);
      cacheIndexOffset_ += entriesThisFile;

    } else {
      // Initialization for input files after the first input file
      if (nProcessesInFirstFile_ == 0) {
        return;
      }

      outerOffset_ = processBlockCacheIndices_.size();

      std::vector<std::string> const& processes = processesWithProcessBlockProducts();
      assert(nProcessesInFirstFile_ <= processes.size());

      // Calculate a translation from an index into the process names from the first file
      // to an index into the process names from the current file. Note the value will
      // be left invalid if the process name is not found in the current file.
      std::vector<unsigned int> firstFileToStored(nProcessesInFirstFile_, invalidProcessIndex());
      for (unsigned int j = 0; j < nProcessesInFirstFile_; ++j) {
        for (unsigned int k = 0; k < storedProcesses.size(); ++k) {
          if (processes[j] == storedProcesses[k]) {
            firstFileToStored[j] = k;
          }
        }
      }

      // Append the cache indices from the current input file to the
      // cache indices container from the previous files.
      // Start by iterating over the outer vector in the container
      // to be filled, but only the new elements that need to be filled.
      if (!storedProcesses.empty()) {
        unsigned int nCacheIndexVectors = storedCacheIndices.size() / storedProcesses.size();
        assert(storedProcesses.size() * nCacheIndexVectors == storedCacheIndices.size());
        processBlockCacheIndices_.resize(processBlockCacheIndices_.size() + nCacheIndexVectors);
        unsigned int storedIndex = 0;
        for (unsigned int k = 0; k < nCacheIndexVectors; ++k) {
          processBlockCacheIndices_[outerOffset_ + k].reserve(nProcessesInFirstFile_);
          for (unsigned int j = 0; j < nProcessesInFirstFile_; ++j) {
            unsigned int iStored = firstFileToStored[j];
            if (iStored == storedProcessBlockHelper.invalidProcessIndex()) {
              processBlockCacheIndices_[outerOffset_ + k].push_back(invalidCacheIndex());
            } else {
              unsigned int oldCacheIndex = storedCacheIndices[storedIndex];
              ++storedIndex;
              unsigned int newCacheIndex = invalidCacheIndex();
              if (oldCacheIndex != invalidCacheIndex()) {
                newCacheIndex = oldCacheIndex + cacheIndexOffset_;
              }
              processBlockCacheIndices_[outerOffset_ + k].push_back(newCacheIndex);
            }
          }
        }
        cacheIndexVectorsPerFile_.push_back(nCacheIndexVectors);
      } else {
        // Create a single inner vector with all invalid values and insert it
        // in the case where the current input file has no processes listed
        // in StoredProcessBlockHelper but the first file did (probably an unusual
        // case, but allowed).
        processBlockCacheIndices_.emplace_back(nProcessesInFirstFile_, invalidCacheIndex());
        cacheIndexVectorsPerFile_.push_back(1);
      }

      unsigned int entriesThisFile = 0;
      std::vector<unsigned int> newNEntries;
      newNEntries.reserve(nProcessesInFirstFile_);
      for (unsigned int j = 0; j < nProcessesInFirstFile_; ++j) {
        unsigned int iStored = firstFileToStored[j];
        if (iStored == storedProcessBlockHelper.invalidProcessIndex()) {
          newNEntries.push_back(0);
        } else {
          newNEntries.push_back(nEntries[iStored]);
          entriesThisFile += nEntries[iStored];
        }
      }
      nEntries_.push_back(std::move(newNEntries));
      cacheEntriesPerFile_.push_back(entriesThisFile);
      cacheIndexOffset_ += entriesThisFile;
    }
  }

  void ProcessBlockHelper::clearAfterOutputFilesClose() {
    processBlockCacheIndices_.clear();
    nEntries_.clear();
    cacheIndexVectorsPerFile_.clear();
    cacheEntriesPerFile_.clear();
    outerOffset_ = 0;
    cacheIndexOffset_ = 0;
  }

}  // namespace edm
