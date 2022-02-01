#ifndef DataFormats_TestObjects_TestProductWithParameterSetID_h
#define DataFormats_TestObjects_TestProductWithParameterSetID_h

/** \class edmtest::TestProductWithParameterSetID
 *
 *  Original Author: W. David Dagenhart 31 January 2022
 */

#include "DataFormats/Provenance/interface/ParameterSetID.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/Registry.h"

namespace edmtest {

  class TestProductWithParameterSetID {
  public:
    TestProductWithParameterSetID() {}

    TestProductWithParameterSetID(edm::ParameterSet& pset) {
      pset.registerIt();
      parameterSetID_ = pset.id();
    }

    edm::ParameterSetID const& parameterSetID() const { return parameterSetID_; }

    edm::ParameterSet const* parameterSet() const {
      return edm::pset::Registry::instance()->getMapped(parameterSetID_);
    }

  private:
    edm::ParameterSetID parameterSetID_;
  };

}   // namespace edmtest
#endif
