#ifndef FWCore_Common_ProcessBlockHelper_h
#define FWCore_Common_ProcessBlockHelper_h

/** \class edm::ProcessBlockHelper

\author W. David Dagenhart, created 15 September, 2020

*/

#include "DataFormats/Provenance/interface/ProvenanceFwd.h"
#include "FWCore/Common/interface/ProcessBlockHelperBase.h"

#include <vector>

namespace edm {

  class ProcessBlockHelper : public ProcessBlockHelperBase {
  public:
    ProcessBlockHelperBase const* topProcessBlockHelper() const override;
    std::vector<std::string> const& topProcessesWithProcessBlockProducts() const override;
    unsigned int nProcessesInFirstFile() const override;
    std::vector<std::vector<unsigned int>> const& processBlockCacheIndices() const override;
    std::vector<std::vector<unsigned int>> const& nEntries() const override;
    std::vector<unsigned int> const& cacheIndexVectorsPerFile() const override;
    std::vector<unsigned int> const& cacheEntriesPerFile() const override;
    unsigned int processBlockIndex(std::string const& processName, EventToProcessBlockIndexes const&) const override;
    std::vector<unsigned int> const& processBlockIndexes(EventToProcessBlockIndexes const&) const override;

    unsigned int outerOffset() const { return outerOffset_; }

    bool initializedFromInput() const { return initializedFromInput_; }

    void initializeFromPrimaryInput(StoredProcessBlockHelper const& storedProcessBlockHelper,
                                    std::vector<unsigned int>&& nEntries);

    void clearAfterOutputFilesClose();

  private:
    // Events/Runs/Lumis hold an index into the outer vector
    // (an offset needs to added in the case of multiple input
    // files). The elements of the inner vector correspond to the
    // processes in processesWithProcessBlockProducts_ (exactly
    // 1 to 1 in the same order except it only includes those processes
    // from the input, if the current Process and/or SubProcesses are
    // added, then they are added to the container of cache indices when
    // the output module makes its modified copy). The values inside
    // the inner vector are the cache indices into the cache vectors
    // contained by user modules. This cache order is the same as the
    // processing order of ProcessBlocks in the current process.
    // This inner vector can contain invalid cache index values
    // if in a file merge step a file after the first has dropped
    // ProcessBlock products that were present in the first file.
    std::vector<std::vector<unsigned int>> processBlockCacheIndices_;

    // Number of entries per ProcessBlock TTree.
    // The outer vector has an element for each input file.
    // The inner vector elements correspond 1-to-1 with
    // processesWithProcessBlockProducts_ and in the same
    // order, except only the processes from the input files.
    // This can contain zeroes.
    std::vector<std::vector<unsigned int>> nEntries_;

    // The index into the next two vectors is the input file index
    // which is based on the order input files are read
    // These can contain zeroes.
    std::vector<unsigned int> cacheIndexVectorsPerFile_;
    std::vector<unsigned int> cacheEntriesPerFile_;

    unsigned int nProcessesInFirstFile_ = 0;

    bool initializedFromInput_ = false;

    // Index of the first outer vector element in the cache indices
    // container that is associated with the current input file. If
    // it points to the end, then there were no cache indices in the
    // current input file.
    unsigned int outerOffset_ = 0;

    // Offset for the cacheIndex, counts all entries in
    // ProcessBlock TTree's in all input files seen so far.
    unsigned int cacheIndexOffset_ = 0;
  };
}  // namespace edm
#endif
