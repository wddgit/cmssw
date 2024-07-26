#include "FWCore/Framework/interface/NoProductResolverException.h"

namespace edm {
  namespace eventsetup {

    NoProductResolverException::NoProductResolverException(const EventSetupRecordKey& key, const char* iClassName,
                                                           const char* dataName, bool failedModuleLabelMatch)
      : cms::Exception("NoProductResolverException") {
      append("Cannot EventSetup resolver for data of type \"");
      append(iClassName);
      append("\" with label \"");
      append(dataName);
      append("\" in record \"");
      append(key.name());
      append("\"");
      if (failedModuleLabelMatch) {
        append("which produces the data with an ESSource or ESProducer configured with the module label specified in the ESInput tag in the configuration. The preferred module to produce this data in the configuration exists but has a different module label. ");
        append("Please ensure that there is an ESSource or ESProducer configured with the requested module label that both produces this data and is selected as the preferred provider for this data if there is more than one such ESSource or ESProducer.");
      } else {
        append("Please add an ESSource or ESProducer to your job which can deliver this data.\n");
      }
    }
  }
}
