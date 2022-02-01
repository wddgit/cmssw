
#include "DataFormats/TestObjects/interface/TestProductWithParameterSetID.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/EDPutToken.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include <memory>
#include <utility>

namespace edmtest {
  class TestParameterSetIDProducer : public edm::global::EDProducer<> {
  public:
    explicit TestParameterSetIDProducer(edm::ParameterSet const&);

    void produce(edm::StreamID, edm::Event&, edm::EventSetup const&) const override;

    static void fillDescriptions(edm::ConfigurationDescriptions&);

  private:
    edm::EDPutTokenT<TestProductWithParameterSetID> evToken_;
    unsigned int value_;
  };

  TestParameterSetIDProducer::TestParameterSetIDProducer(edm::ParameterSet const& iConfig)
    : value_(iConfig.getParameter<unsigned int>("value")) {
    evToken_ = produces<TestProductWithParameterSetID>();
  }

  void TestParameterSetIDProducer::produce(edm::StreamID, edm::Event& event, edm::EventSetup const&) const {
    edm::ParameterSet parameterSet;
    parameterSet.addParameter<unsigned int>("value", value_);
    auto product = std::make_unique<TestProductWithParameterSetID>(parameterSet);
    event.put(evToken_, std::move(product));
  }

  void TestParameterSetIDProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<unsigned int>("value", 11);
    descriptions.addDefault(desc);
  }

}  // namespace edmtest
using edmtest::TestParameterSetIDProducer;
DEFINE_FWK_MODULE(TestParameterSetIDProducer);
