#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "edm4hep/edm4hep.h"
#include "podio/CollectionBase.h"
#include "podio/UserDataCollection.h"

#include "putOnReadForAllProducts.h"

#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/WrapperBase.h"
#include "DataFormats/Provenance/interface/ProductDescription.h"
#include "DataFormats/Provenance/interface/ProductProvenance.h"
#include "FWCore/Utilities/interface/EDMException.h"

namespace c4h {

  namespace {

    template <typename CollectionType>
    void putOnReadForProductType(edm::EventPrincipal& eventPrincipal,
                                 edm::ProductDescription const& productDescription,
                                 const podio::CollectionBase* collectionBase) {
      std::optional<edm::ProductProvenance> productProvenance;

      const auto* collectionConstPtr = dynamic_cast<const CollectionType*>(collectionBase);
      if (!collectionConstPtr) {
        throw edm::Exception(edm::errors::LogicError, "putOnReadForProductType")
            << "Failed to get product with moduleLabel \"" << productDescription.moduleLabel()
            << "\" from podio::Frame\n";
      }

      // Not nice, but I have not yet thought of a way to avoid this const_cast.
      // Note that if you trace this pointer's origin it comes from an internal
      // pointer in podio that is the result of converting a non-const pointer
      // to a const pointer, so this is not undefined behavior, but it is fragile.
      auto* collection = const_cast<CollectionType*>(collectionConstPtr);

      auto wrappedCollection =
          std::make_unique<edm::Wrapper<CollectionType>>(edm::WrapperBase::Emplace(), std::move(*collection));
      eventPrincipal.putOnRead(productDescription, std::move(wrappedCollection), productProvenance);
    }
  }  // namespace

  void putOnReadForAllProducts(podio::Frame const& podioFrame,
                               edm::ProductRegistry const& productRegistry,
                               edm::EventPrincipal& eventPrincipal) {
    for (auto const& iter : productRegistry.productList()) {
      auto const& productDescription = iter.second;
      std::string const& moduleLabel = productDescription.moduleLabel();

      const podio::CollectionBase* collectionBase = podioFrame.get(moduleLabel);

      if (moduleLabel == "CalorimeterHitCollection") {
        putOnReadForProductType<edm4hep::CalorimeterHitCollection>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "CaloHitContributionCollection") {
        putOnReadForProductType<edm4hep::CaloHitContributionCollection>(
            eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "ClusterCollection") {
        putOnReadForProductType<edm4hep::ClusterCollection>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "EventHeader") {
        putOnReadForProductType<edm4hep::EventHeaderCollection>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "GeneratorEventParametersCollection") {
        putOnReadForProductType<edm4hep::GeneratorEventParametersCollection>(
            eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "MCParticleCollection") {
        putOnReadForProductType<edm4hep::MCParticleCollection>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "ParticleIDCollection") {
        putOnReadForProductType<edm4hep::ParticleIDCollection>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "RawCalorimeterHitCollection") {
        putOnReadForProductType<edm4hep::RawCalorimeterHitCollection>(
            eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "RawTimeSeriesCollection") {
        putOnReadForProductType<edm4hep::RawTimeSeriesCollection>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "RecDqdxCollection") {
        putOnReadForProductType<edm4hep::RecDqdxCollection>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "ReconstructedParticleCollection") {
        putOnReadForProductType<edm4hep::ReconstructedParticleCollection>(
            eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "SenseWireHitCollection") {
        putOnReadForProductType<edm4hep::SenseWireHitCollection>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "SimCalorimeterHitCollection") {
        putOnReadForProductType<edm4hep::SimCalorimeterHitCollection>(
            eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "SimTrackerHitCollection") {
        putOnReadForProductType<edm4hep::SimTrackerHitCollection>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "TimeSeriesCollection") {
        putOnReadForProductType<edm4hep::TimeSeriesCollection>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "TrackCollection") {
        putOnReadForProductType<edm4hep::TrackCollection>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "TrackerHitPlaneCollection") {
        putOnReadForProductType<edm4hep::TrackerHitPlaneCollection>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "TrackerHit3DCollection") {
        putOnReadForProductType<edm4hep::TrackerHit3DCollection>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "UserDataCollectionFloat") {
        putOnReadForProductType<podio::UserDataCollection<float>>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "UserDataCollectionInt") {
        putOnReadForProductType<podio::UserDataCollection<int32_t>>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "VertexCollection") {
        putOnReadForProductType<edm4hep::VertexCollection>(eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "CaloHitMCParticleLinkCollection") {
        putOnReadForProductType<podio::LinkCollection<edm4hep::CalorimeterHit, edm4hep::MCParticle>>(
            eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "CaloHitSimCaloHitLinkCollection") {
        putOnReadForProductType<podio::LinkCollection<edm4hep::CalorimeterHit, edm4hep::SimCalorimeterHit>>(
            eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "ClusterMCParticleLinkCollection") {
        putOnReadForProductType<podio::LinkCollection<edm4hep::Cluster, edm4hep::MCParticle>>(
            eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "RecoMCParticleLinkCollection") {
        putOnReadForProductType<podio::LinkCollection<edm4hep::ReconstructedParticle, edm4hep::MCParticle>>(
            eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "TrackMCParticleLinkCollection") {
        putOnReadForProductType<podio::LinkCollection<edm4hep::Track, edm4hep::MCParticle>>(
            eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "TrackerHitSimTrackerHitLinkCollection") {
        putOnReadForProductType<podio::LinkCollection<edm4hep::TrackerHit, edm4hep::SimTrackerHit>>(
            eventPrincipal, productDescription, collectionBase);
      } else if (moduleLabel == "VertexRecoParticleLinkCollection") {
        putOnReadForProductType<podio::LinkCollection<edm4hep::Vertex, edm4hep::ReconstructedParticle>>(
            eventPrincipal, productDescription, collectionBase);
      } else {
        throw edm::Exception(edm::errors::LogicError, "putOnReadForAllProducts")
            << "Unknown moduleLabel \"" << moduleLabel << "\"\n";
      }
    }
  }
}  // namespace c4h
