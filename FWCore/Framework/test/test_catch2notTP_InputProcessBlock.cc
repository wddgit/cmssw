#include "catch.hpp"

#include "FWCore/Framework/interface/InputProcessBlockCacheImpl.h"
#include "FWCore/Framework/interface/moduleAbilities.h"
#include "FWCore/Framework/interface/stream/EDAnalyzer.h"

#include <type_traits>

namespace edmtest {

  class TestAnalyzerWithInputProcessBlockAbility : public edm::stream::EDAnalyzer<edm::GlobalCache<int>, edm::InputProcessBlockCache<int, unsigned int, int>> {};

  class TestAnalyzerWithoutInputProcessBlockAbility : public edm::stream::EDAnalyzer<edm::GlobalCache<int>> {};

}  // namespace edmtest

TEST_CASE("test InputProcessBlock", "[InputProcessBlock]") {

  SECTION("test HasAbility") {
    REQUIRE(edmtest::TestAnalyzerWithInputProcessBlockAbility::HasAbility::kInputProcessBlockCache);
    REQUIRE(!edmtest::TestAnalyzerWithoutInputProcessBlockAbility::HasAbility::kInputProcessBlockCache);
    REQUIRE(std::is_same<edmtest::TestAnalyzerWithInputProcessBlockAbility::InputProcessBlockCache, edm::impl::InputProcessBlockCacheImpl<int, unsigned int, int>>());
    REQUIRE(std::is_same<edmtest::TestAnalyzerWithoutInputProcessBlockAbility::InputProcessBlockCache, void>());
  }

}
