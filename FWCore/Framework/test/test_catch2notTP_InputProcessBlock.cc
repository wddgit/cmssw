#include "catch.hpp"

#include "FWCore/Framework/interface/InputProcessBlockCacheImpl.h"
#include "FWCore/Framework/interface/moduleAbilities.h"
#include "FWCore/Framework/interface/stream/dummy_helpers.h"
#include "FWCore/Framework/interface/stream/EDAnalyzer.h"

#include <memory>
#include <type_traits>

namespace edmtest {

  class TestAnalyzerWithInputProcessBlockAbility : public edm::stream::EDAnalyzer<edm::GlobalCache<int>, edm::InputProcessBlockCache<int, unsigned int, int>> {};

  class TestAnalyzerWithoutInputProcessBlockAbility : public edm::stream::EDAnalyzer<edm::GlobalCache<int>> {};

}  // namespace edmtest

TEST_CASE("test InputProcessBlock", "[InputProcessBlock]") {

  SECTION("test HasAbility") {
    REQUIRE(edmtest::TestAnalyzerWithInputProcessBlockAbility::HasAbility::kInputProcessBlockCache);
    REQUIRE(!edmtest::TestAnalyzerWithoutInputProcessBlockAbility::HasAbility::kInputProcessBlockCache);
  }
  SECTION("test type aliases") {
    REQUIRE(std::is_same<edmtest::TestAnalyzerWithInputProcessBlockAbility::InputProcessBlockCache, edm::impl::InputProcessBlockCacheImpl<int, unsigned int, int>>());
    REQUIRE(std::is_same<edmtest::TestAnalyzerWithoutInputProcessBlockAbility::InputProcessBlockCache, void>());
    REQUIRE(std::is_same<edm::stream::impl::choose_unique_ptr<edmtest::TestAnalyzerWithInputProcessBlockAbility::InputProcessBlockCache>::type, std::unique_ptr<edm::impl::InputProcessBlockCacheImpl<int, unsigned int, int>>>());
    REQUIRE(std::is_same<edm::stream::impl::choose_unique_ptr<edmtest::TestAnalyzerWithoutInputProcessBlockAbility::InputProcessBlockCache>::type, edm::stream::impl::dummy_ptr>());
  }

}
