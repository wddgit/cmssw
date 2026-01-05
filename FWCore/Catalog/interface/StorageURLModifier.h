#ifndef FWCore_Catalog_StorageURLModifier_h
#define FWCore_Catalog_StorageURLModifier_h

/**\class edm::StorageURLModifier

Description: This is a virtual base class for a services
providing the ability to modify PFN URLs.

*/
//
// Original Author: W. David Dagenhart
//         Created: 29 Dec 2025

#include <string>

namespace edm {
  // These enum values are used in classes derived from StorageURLModifier
  // as indexes into vectors. Do not modify them without making corresponding
  // changes in those derived classes.
  enum class SciTagCategory : unsigned char { Primary, Embedded, PreMixedPileup, Undefined };

  class StorageURLModifier {
  public:
    virtual ~StorageURLModifier() = default;

    virtual void appendScitagToURL(SciTagCategory sciTagCategory, std::string& url) const = 0;

    // The next 4 functions are intended to be used for
    // testing purposes only.
    virtual unsigned int analysisSciTag(SciTagCategory category) const = 0;
    virtual unsigned int productionSciTag(SciTagCategory category) const = 0;
    virtual bool enable() const = 0;
    virtual bool productionCase() const = 0;
  };

}  // namespace edm

#endif
