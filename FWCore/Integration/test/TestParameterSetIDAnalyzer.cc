
#include "DataFormats/TestObjects/interface/TestProductWithParameterSetID.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/global/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include <iostream>

namespace edmtest {
  class TestParameterSetIDAnalyzer : public edm::global::EDAnalyzer<> {
  public:
    explicit TestParameterSetIDAnalyzer(edm::ParameterSet const&);

    void analyze(edm::StreamID, edm::Event const&, edm::EventSetup const&) const override final;

    static void fillDescriptions(edm::ConfigurationDescriptions&);

  private:
    edm::EDGetTokenT<TestProductWithParameterSetID> token_;
  };

  TestParameterSetIDAnalyzer::TestParameterSetIDAnalyzer(edm::ParameterSet const& pset) {
    token_ = consumes<TestProductWithParameterSetID>(pset.getUntrackedParameter<edm::InputTag>("inputTag"));
  }

  void TestParameterSetIDAnalyzer::analyze(edm::StreamID, edm::Event const& event, edm::EventSetup const&) const {
    edm::ParameterSet const* pset = event.get(token_).parameterSet();
    if (pset) {
      std::cout << pset->getParameter<unsigned int>("value")  << std::endl;
    } else {
      std::cout << "ParameterSet not found" << std::endl;
    }
  }

  void TestParameterSetIDAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.addUntracked<edm::InputTag>("inputTag");
    descriptions.addDefault(desc);
  }

}  // namespace edmtest
using edmtest::TestParameterSetIDAnalyzer;
DEFINE_FWK_MODULE(TestParameterSetIDAnalyzer);
