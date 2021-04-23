
#include "FWCore/Framework/interface/FileBlock.h"
#include "FWCore/Framework/interface/one/OutputModule.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ProcessBlockForOutput.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ConstProductRegistry.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "TTree.h"

#include <memory>
#include <string>
#include <vector>

namespace edm {

  class TestOneOutput : public one::OutputModule<WatchInputFiles, RunCache<int>, LuminosityBlockCache<int>> {
  public:
    explicit TestOneOutput(ParameterSet const& pset);
    ~TestOneOutput() override;
    static void fillDescriptions(ConfigurationDescriptions& descriptions);

  private:
    void write(EventForOutput const& e) override;
    void writeLuminosityBlock(LuminosityBlockForOutput const&) override;
    void writeRun(RunForOutput const&) override;
    void writeProcessBlock(ProcessBlockForOutput const&) override;

    void respondToOpenInputFile(FileBlock const&) override;
    void respondToCloseInputFile(FileBlock const&) override;
    void testFileBlock(FileBlock const&);

    std::shared_ptr<int> globalBeginRun(RunForOutput const&) const override;
    void globalEndRun(RunForOutput const&) override;

    std::shared_ptr<int> globalBeginLuminosityBlock(LuminosityBlockForOutput const&) const override;
    void globalEndLuminosityBlock(LuminosityBlockForOutput const&) override;
    void endJob() override;

    bool verbose_;
    std::vector<std::string> expectedProcessesWithProcessBlockProducts_;
    std::vector<std::string> expectedProcessNamesAtWrite_;
    int expectedWriteProcessBlockTransitions_;
    unsigned int countWriteProcessBlockTransitions_ = 0;
    bool requireNullTTreesInFileBlock_;
    bool testTTreesInFileBlock_;
    std::vector<unsigned int> expectedCacheIndexSize_;
  };

  TestOneOutput::TestOneOutput(ParameterSet const& pset)
      : one::OutputModuleBase(pset),
        one::OutputModule<WatchInputFiles, RunCache<int>, LuminosityBlockCache<int>>(pset),
        verbose_(pset.getUntrackedParameter<bool>("verbose")),
        expectedProcessesWithProcessBlockProducts_(pset.getUntrackedParameter<std::vector<std::string>>("expectedProcessesWithProcessBlockProducts")),
        expectedProcessNamesAtWrite_(pset.getUntrackedParameter<std::vector<std::string>>("expectedProcessNamesAtWrite")),
        expectedWriteProcessBlockTransitions_(pset.getUntrackedParameter<int>("expectedWriteProcessBlockTransitions")),
        requireNullTTreesInFileBlock_(pset.getUntrackedParameter<bool>("requireNullTTreesInFileBlock")),
        testTTreesInFileBlock_(pset.getUntrackedParameter<bool>("testTTreesInFileBlock")),
        expectedCacheIndexSize_(pset.getUntrackedParameter<std::vector<unsigned int>>("expectedCacheIndexSize")) {}

  TestOneOutput::~TestOneOutput() {}

  void TestOneOutput::write(EventForOutput const& e) {
    if (verbose_) {
      LogAbsolute("TestOneOutput") << "one write event";
    }
  }

  void TestOneOutput::writeLuminosityBlock(LuminosityBlockForOutput const&) {
    if (verbose_) {
      LogAbsolute("TestOneOutput") << "one writeLuminosityBlock";
    }
  }

  void TestOneOutput::writeRun(RunForOutput const&) {
    if (verbose_) {
      LogAbsolute("TestOneOutput") << "one writeRun";
    }
  }

  void TestOneOutput::writeProcessBlock(ProcessBlockForOutput const& pb) {
    if (verbose_) {
      LogAbsolute("TestOneOutput") << "one writeProcessBlock";
    }
    if (countWriteProcessBlockTransitions_ < expectedProcessNamesAtWrite_.size()) {
      if (expectedProcessNamesAtWrite_[countWriteProcessBlockTransitions_] != pb.processName()) {
        throw cms::Exception("TestFailure") << "TestOneOutput::writeProcessBlock unexpected process name";
      }
    }
    if (!expectedProcessesWithProcessBlockProducts_.empty()) {
      if (verbose_) {
        for (auto const& process : outputProcessBlockHelper().processesWithProcessBlockProducts()) {
          LogAbsolute("TestOneOutput") << "    " << process;
        }
      }
      if (expectedProcessesWithProcessBlockProducts_ != outputProcessBlockHelper().processesWithProcessBlockProducts()) {
        throw cms::Exception("TestFailure") << "TestOneOutput::writeProcessBlock unexpected process name list";
      }
    }
    if (countWriteProcessBlockTransitions_ < expectedCacheIndexSize_.size()) {
      if (expectedCacheIndexSize_[countWriteProcessBlockTransitions_] !=
          outputProcessBlockHelper().processBlockHelper()->processBlockCacheIndices().size()) {
        throw cms::Exception("TestFailure") << "TestOneOutput::writeProcessBlock unexpected cache index size";
      }
    }
    ++countWriteProcessBlockTransitions_;
  }

  void TestOneOutput::respondToOpenInputFile(FileBlock const& fb) {
    if (verbose_) {
      LogAbsolute("TestOneOutput") << "one respondToOpenInputFile";
    }
    testFileBlock(fb);
  }

  void TestOneOutput::respondToCloseInputFile(FileBlock const& fb) {
    if (verbose_) {
      LogAbsolute("TestOneOutput") << "one respondToCloseInputFile";
    }
    testFileBlock(fb);
  }

  void TestOneOutput::testFileBlock(FileBlock const& fb) {
    if (requireNullTTreesInFileBlock_) {
      if (fb.tree() != nullptr ||
          fb.lumiTree() != nullptr ||
          fb.runTree() != nullptr ||
          !fb.processBlockTrees().empty() ||
          !fb.processesWithProcessBlockTrees().empty() ||
          fb.processBlockTree(std::string("DoesNotExist")) != nullptr) {
        throw cms::Exception("TestFailure") << "TestOneOutput::respondToOpenInputFile expected null TTree pointers in FileBlock";
      }
    } else if (testTTreesInFileBlock_) {
      if (std::string("Events") != fb.tree()->GetName() ||
          std::string("LuminosityBlocks") != fb.lumiTree()->GetName() ||
          std::string("Runs") != fb.runTree()->GetName() ||
          fb.processesWithProcessBlockTrees().size() != 2 ||
          fb.processesWithProcessBlockTrees()[0] != "PROD1" ||
          fb.processesWithProcessBlockTrees()[1] != "MERGE" ||
          fb.processBlockTrees().size() != 2 ||
          std::string("ProcessBlocksPROD1") != fb.processBlockTrees()[0]->GetName() ||
          std::string("ProcessBlocksMERGE") != fb.processBlockTrees()[1]->GetName() ||
          std::string("ProcessBlocksPROD1") != fb.processBlockTree("PROD1")->GetName() ||
          std::string("ProcessBlocksMERGE") != fb.processBlockTree("MERGE")->GetName() ||
          fb.processBlockTree("DOESNOTEXIST") != nullptr) {
        throw cms::Exception("TestFailure") << "TestOneOutput::testFileBlock failed. Testing tree pointers";
      }
    }
  }

  std::shared_ptr<int> TestOneOutput::globalBeginRun(RunForOutput const&) const {
    if (verbose_) {
      LogAbsolute("TestOneOutput") << "one globalBeginRun";
      edm::Service<edm::ConstProductRegistry> reg;
      for (auto const& it : reg->productList()) {
        LogAbsolute("TestOneOutput") << it.second;
      }
    }
    return std::make_shared<int>(0);
  }

  void TestOneOutput::globalEndRun(RunForOutput const&) {
    if (verbose_) {
      LogAbsolute("TestOneOutput") << "one globalEndRun";
    }
  }

  std::shared_ptr<int> TestOneOutput::globalBeginLuminosityBlock(LuminosityBlockForOutput const&) const {
    if (verbose_) {
      LogAbsolute("TestOneOutput") << "one globalBeginLuminosityBlock";
    }
    return std::make_shared<int>(0);
  }

  void TestOneOutput::globalEndLuminosityBlock(LuminosityBlockForOutput const&) {
    if (verbose_) {
      LogAbsolute("TestOneOutput") << "one globalEndLuminosityBlock";
    }
  }

  void TestOneOutput::endJob() {
    if (expectedWriteProcessBlockTransitions_ >= 0) {
      if (static_cast<unsigned int>(expectedWriteProcessBlockTransitions_) != countWriteProcessBlockTransitions_) {
        throw cms::Exception("TestFailure") << "TestOneOutput::writeProcessBlock unexpected number of writeProcessBlock transitions";
      }
    }
  }

  void TestOneOutput::fillDescriptions(ConfigurationDescriptions& descriptions) {
    ParameterSetDescription desc;
    OutputModule::fillDescription(desc);
    desc.addUntracked<bool>("verbose", true);
    desc.addUntracked<std::vector<std::string>>("expectedProcessesWithProcessBlockProducts", std::vector<std::string>());
    desc.addUntracked<std::vector<std::string>>("expectedProcessNamesAtWrite", std::vector<std::string>());
    desc.addUntracked<int>("expectedWriteProcessBlockTransitions", -1);
    desc.addUntracked<bool>("requireNullTTreesInFileBlock", false);
    desc.addUntracked<bool>("testTTreesInFileBlock", false);
    desc.addUntracked<std::vector<unsigned int>>("expectedCacheIndexSize", std::vector<unsigned int>());
    descriptions.addDefault(desc);
  }
}  // namespace edm

using edm::TestOneOutput;
DEFINE_FWK_MODULE(TestOneOutput);
