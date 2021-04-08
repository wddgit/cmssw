
#include "FWCore/Framework/interface/one/OutputModule.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ConstProductRegistry.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

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

    std::shared_ptr<int> globalBeginRun(RunForOutput const&) const override;
    void globalEndRun(RunForOutput const&) override;

    std::shared_ptr<int> globalBeginLuminosityBlock(LuminosityBlockForOutput const&) const override;
    void globalEndLuminosityBlock(LuminosityBlockForOutput const&) override;
    void endJob() override;

    bool verbose_;
    std::vector<std::string> expectedProcessesWithProcessBlockProducts_;
    int expectedWriteProcessBlockTransitions_;
    int countWriteProcessBlockTransitions_ = 0;
  };

  TestOneOutput::TestOneOutput(ParameterSet const& pset)
      : one::OutputModuleBase(pset),
        one::OutputModule<WatchInputFiles, RunCache<int>, LuminosityBlockCache<int>>(pset),
        verbose_(pset.getUntrackedParameter<bool>("verbose")),
        expectedProcessesWithProcessBlockProducts_(pset.getUntrackedParameter<std::vector<std::string>>("expectedProcessesWithProcessBlockProducts")),
        expectedWriteProcessBlockTransitions_(pset.getUntrackedParameter<int>("expectedWriteProcessBlockTransitions")) {}

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

  void TestOneOutput::writeRun(RunForOutput const&) { LogAbsolute("TestOneOutput") << "one writeRun"; }

  void TestOneOutput::writeProcessBlock(ProcessBlockForOutput const&) {
    LogAbsolute("TestOneOutput") << "one writeProcessBlock";
    ++countWriteProcessBlockTransitions_;
    if (!expectedProcessesWithProcessBlockProducts_.empty()) {
      for (auto const& x : outputProcessBlockHelper().processesWithProcessBlockProducts()) {
        LogAbsolute("TestOneOutput") << "one writeProcessBlock " << x;
      }
      if (expectedProcessesWithProcessBlockProducts_ != outputProcessBlockHelper().processesWithProcessBlockProducts()) {
        throw cms::Exception("TestFailure") << "TestOneOutput::writeProcessBlock unexpected process name list";
      }
    }
  }

  void TestOneOutput::respondToOpenInputFile(FileBlock const&) {
    if (verbose_) {
      LogAbsolute("TestOneOutput") << "one respondToOpenInputFile";
    }
  }

  void TestOneOutput::respondToCloseInputFile(FileBlock const&) {
    if (verbose_) {
      LogAbsolute("TestOneOutput") << "one respondToCloseInputFile";
    }
  }

  std::shared_ptr<int> TestOneOutput::globalBeginRun(RunForOutput const&) const {
    LogAbsolute("TestOneOutput") << "one globalBeginRun";
    if (verbose_) {
      BranchIDLists const* theBranchIDLists = branchIDLists();
      for (auto const& branchIDList : *theBranchIDLists) {
        LogAbsolute("TestOneOutput") << "A branchID list";
        for (auto const& branchID : branchIDList) {
          LogAbsolute("TestOneOutput") << "  one branchID " << branchID;
        }
      }
      edm::Service<edm::ConstProductRegistry> reg;
      for (auto const& it : reg->productList()) {
        LogAbsolute("TestOneOutput") << it.second;
      }
    }
    return std::make_shared<int>(0);
  }

  void TestOneOutput::globalEndRun(RunForOutput const&) {
    LogAbsolute("TestOneOutput") << "one globalEndRun";
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
      if (expectedWriteProcessBlockTransitions_ != countWriteProcessBlockTransitions_) {
        throw cms::Exception("TestFailure") << "TestOneOutput::writeProcessBlock unexpected number of writeProcessBlock transitions";
      }
    }
  }

  void TestOneOutput::fillDescriptions(ConfigurationDescriptions& descriptions) {
    ParameterSetDescription desc;
    OutputModule::fillDescription(desc);
    desc.addUntracked<bool>("verbose", true);
    desc.addUntracked<std::vector<std::string>>("expectedProcessesWithProcessBlockProducts", std::vector<std::string>());
    desc.addUntracked<int>("expectedWriteProcessBlockTransitions", -1);
    descriptions.addDefault(desc);
  }
}  // namespace edm

using edm::TestOneOutput;
DEFINE_FWK_MODULE(TestOneOutput);
