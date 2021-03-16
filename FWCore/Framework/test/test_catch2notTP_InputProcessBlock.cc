#include "catch.hpp"

#include "FWCore/Framework/interface/moduleAbilities.h"
#include "FWCore/Framework/interface/stream/EDAnalyzer.h"

namespace edmtest {

  class TestAnalyzerWithInputProcessBlockAbility : public edm::stream::EDAnalyzer<edm::GlobalCache<int>, edm::InputProcessBlockCache<int, unsigned int, int>> {};

  class TestAnalyzerWithoutInputProcessBlockAbility : public edm::stream::EDAnalyzer<edm::GlobalCache<int>> {};

}  // namespace edmtest

TEST_CASE("test InputProcessBlock", "[InputProcessBlock]") {

  SECTION("test HasAbility") {
    REQUIRE(edmtest::TestAnalyzerWithInputProcessBlockAbility::HasAbility::kInputProcessBlockCache);
    REQUIRE(!edmtest::TestAnalyzerWithoutInputProcessBlockAbility::HasAbility::kInputProcessBlockCache);
  }

}
