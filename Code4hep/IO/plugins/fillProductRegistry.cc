#include <cstddef>
#include <vector>

#include "edm4hep/edm4hep.h"
#include "podio/UserDataCollection.h"

#include "fillProductRegistry.h"

#include "DataFormats/Provenance/interface/ProductDescription.h"
#include "FWCore/Utilities/interface/BranchType.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Utilities/interface/TypeID.h"

namespace c4h {

  std::unique_ptr<edm::ProductRegistry> fillProductRegistry(podio::Frame const& podioFrame,
                                                            std::string const& processName) {
    auto productRegistry = std::make_unique<edm::ProductRegistry>();

    // Fill the process order first
    // The process order only contains the one process name for input products
    std::vector<std::string> processOrder = {processName};
    productRegistry->setProcessOrder(processOrder);

    edm::BranchType const branchType = edm::InEvent;
    std::string const productInstanceName = "";
    edm::TypeID typeID;
    bool const produced = false;

    for (std::string const& collection : podioFrame.getAvailableCollections()) {
      // This line of code just emphasizes to the reader that a podio collection
      // name is used as the module label in the EDM ProductDescription
      std::string const& moduleLabel = collection;

      if (collection == "CalorimeterHitCollection") {
        typeID = edm::TypeID(typeid(edm4hep::CalorimeterHitCollection));
      } else if (collection == "CaloHitContributionCollection") {
        typeID = edm::TypeID(typeid(edm4hep::CaloHitContributionCollection));
      } else if (collection == "ClusterCollection") {
        typeID = edm::TypeID(typeid(edm4hep::ClusterCollection));
      } else if (collection == "EventHeader") {
        typeID = edm::TypeID(typeid(edm4hep::EventHeaderCollection));
      } else if (collection == "GeneratorEventParametersCollection") {
        typeID = edm::TypeID(typeid(edm4hep::GeneratorEventParametersCollection));
      } else if (collection == "MCParticleCollection") {
        typeID = edm::TypeID(typeid(edm4hep::MCParticleCollection));
      } else if (collection == "ParticleIDCollection") {
        typeID = edm::TypeID(typeid(edm4hep::ParticleIDCollection));
      } else if (collection == "RawCalorimeterHitCollection") {
        typeID = edm::TypeID(typeid(edm4hep::RawCalorimeterHitCollection));
      } else if (collection == "RawTimeSeriesCollection") {
        typeID = edm::TypeID(typeid(edm4hep::RawTimeSeriesCollection));
      } else if (collection == "RecDqdxCollection") {
        typeID = edm::TypeID(typeid(edm4hep::RecDqdxCollection));
      } else if (collection == "ReconstructedParticleCollection") {
        typeID = edm::TypeID(typeid(edm4hep::ReconstructedParticleCollection));
      } else if (collection == "SenseWireHitCollection") {
        typeID = edm::TypeID(typeid(edm4hep::SenseWireHitCollection));
      } else if (collection == "SimCalorimeterHitCollection") {
        typeID = edm::TypeID(typeid(edm4hep::SimCalorimeterHitCollection));
      } else if (collection == "SimTrackerHitCollection") {
        typeID = edm::TypeID(typeid(edm4hep::SimTrackerHitCollection));
      } else if (collection == "TimeSeriesCollection") {
        typeID = edm::TypeID(typeid(edm4hep::TimeSeriesCollection));
      } else if (collection == "TrackCollection") {
        typeID = edm::TypeID(typeid(edm4hep::TrackCollection));
      } else if (collection == "TrackerHitPlaneCollection") {
        typeID = edm::TypeID(typeid(edm4hep::TrackerHitPlaneCollection));
      } else if (collection == "TrackerHit3DCollection") {
        typeID = edm::TypeID(typeid(edm4hep::TrackerHit3DCollection));
      } else if (collection == "UserDataCollectionFloat") {
        typeID = edm::TypeID(typeid(podio::UserDataCollection<float>));
      } else if (collection == "UserDataCollectionInt") {
        typeID = edm::TypeID(typeid(podio::UserDataCollection<int32_t>));
      } else if (collection == "VertexCollection") {
        typeID = edm::TypeID(typeid(edm4hep::VertexCollection));
      } else if (collection == "CaloHitMCParticleLinkCollection") {
        typeID = edm::TypeID(typeid(podio::LinkCollection<edm4hep::CalorimeterHit, edm4hep::MCParticle>));
      } else if (collection == "CaloHitSimCaloHitLinkCollection") {
        typeID = edm::TypeID(typeid(podio::LinkCollection<edm4hep::CalorimeterHit, edm4hep::SimCalorimeterHit>));
      } else if (collection == "ClusterMCParticleLinkCollection") {
        typeID = edm::TypeID(typeid(podio::LinkCollection<edm4hep::Cluster, edm4hep::MCParticle>));
      } else if (collection == "RecoMCParticleLinkCollection") {
        typeID = edm::TypeID(typeid(podio::LinkCollection<edm4hep::ReconstructedParticle, edm4hep::MCParticle>));
      } else if (collection == "TrackMCParticleLinkCollection") {
        typeID = edm::TypeID(typeid(podio::LinkCollection<edm4hep::Track, edm4hep::MCParticle>));
      } else if (collection == "TrackerHitSimTrackerHitLinkCollection") {
        typeID = edm::TypeID(typeid(podio::LinkCollection<edm4hep::TrackerHit, edm4hep::SimTrackerHit>));
      } else if (collection == "VertexRecoParticleLinkCollection") {
        typeID = edm::TypeID(typeid(podio::LinkCollection<edm4hep::Vertex, edm4hep::ReconstructedParticle>));
      } else {
        throw edm::Exception(edm::errors::LogicError, "fillProductRegistry")
            << "Unknown podio collection name " << collection << "\n";
      }
      edm::ProductDescription productDescription(
          branchType, moduleLabel, processName, productInstanceName, typeID, produced);

      productRegistry->copyProduct(productDescription);
    }
    return productRegistry;
  }
}  // namespace c4h
