#ifndef FWCore_Catalog_test_TestScitagConfig_h
#define FWCore_Catalog_test_TestScitagConfig_h

#include <memory>
#include <string>
#include <vector>

#include "FWCore/Catalog/interface/StorageURLModifier.h"
#include "FWCore/ServiceRegistry/interface/ServiceRegistry.h"
#include "FWCore/ServiceRegistry/interface/ServiceToken.h"

namespace edmtest::catalog {

  class TestScitagConfig : public edm::StorageURLModifier {
  public:
    TestScitagConfig() : testSuffixes_{"?scitag.flow=196664", "?scitag.flow=196700", "?scitag.flow=196704"} {}

    void appendScitagToURL(edm::SciTagCategory sciTagCategory, std::string& url) const override {
      url += testSuffixes_.at(static_cast<unsigned char>(sciTagCategory));
    }

    // These functions are not used. They only exist so it will compile.
    // The pure virtual functions in the base class must be defined.
    unsigned int analysisSciTag(edm::SciTagCategory category) const override { return 0; }
    unsigned int productionSciTag(edm::SciTagCategory category) const override { return 0; }
    bool enable() const override { return false; }
    bool productionCase() const override { return false; }

  private:
    std::vector<std::string> testSuffixes_;
  };

  inline edm::ServiceToken makeTestScitagConfigToken(edm::ServiceToken const& token) {
    return edm::ServiceToken(edm::ServiceRegistry::createContaining(
        std::unique_ptr<edm::StorageURLModifier>(std::make_unique<TestScitagConfig>()), token, true));
  }
}  // namespace edmtest::catalog

#endif
