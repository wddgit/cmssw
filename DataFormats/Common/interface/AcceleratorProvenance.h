#ifndef DataFormats_Common_AcceleratorProvenance_h
#define DataFormats_Common_AcceleratorProvenance_h

/** \class edm::AcceleratorProvenance

Records information related to hardware accelerator
options configured and hardware accelerator components
available when a process was run. GPUs and FPGAs are
two possible examples of hardware accelerators.

We expect these products to be written into ProcessBlocks.

\author W. David Dagenhart, created 11 Jan, 2022
*/

#include <string>
#include <utility>
#include <vector>

namespace edm {

  enum AcceleratorType { GPU = 0, FPGA = 1 };

  class AcceleratorProvenance {
  public:
    AcceleratorProvenance();

    std::vector<AcceleratorType> const& acceleratorTypes() const { return acceleratorTypes_; }
    std::vector<std::string> const& availableHardware() const { return availableHardware_; }
    std::vector<unsigned int> const& numberAvailable() const { return numberAvailable_; }

    void setAcceleratorTypes(std::vector<AcceleratorType> val) { acceleratorTypes_ = std::move(val); }
    void setAvailableHardware(std::vector<std::string> val) { availableHardware_ = std::move(val); }
    void setNumberAvailable(std::vector<unsigned int> val) { numberAvailable_ = std::move(val); }

  private:
    std::vector<AcceleratorType> acceleratorTypes_;
    std::vector<std::string> availableHardware_;
    std::vector<unsigned int> numberAvailable_;
  };

}  // namespace edm
#endif
