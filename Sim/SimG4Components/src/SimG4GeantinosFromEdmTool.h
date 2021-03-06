#ifndef SIMG4COMPONENTS_G4GEANTINOSFROMEDMTOOL_H
#define SIMG4COMPONENTS_G4GEANTINOSFROMEDMTOOL_H

// from Gaudi
#include "FWCore/DataHandle.h"
#include "GaudiAlg/GaudiTool.h"

#include "SimG4Interface/ISimG4EventProviderTool.h"

#include "G4VUserPrimaryGeneratorAction.hh"

// Forward declarations
// datamodel
namespace fcc {
class MCParticleCollection;
}

class SimG4GeantinosFromEdmTool : public GaudiTool, virtual public ISimG4EventProviderTool {
public:
  /// Standard constructor
  SimG4GeantinosFromEdmTool(const std::string& type, const std::string& name, const IInterface* parent);

  virtual ~SimG4GeantinosFromEdmTool();

  StatusCode initialize() final;

  /// Translates the input (fcc::MCParticleCollection) into a G4Event
  /// @returns G4Event with primaries generated from MCParticleCollection (ownership is transferred to the caller)
  virtual G4Event* g4Event() final;

private:
  /// Handle for the EDM MC particles to be read
  DataHandle<fcc::MCParticleCollection> m_genParticles{"allGenParticles", Gaudi::DataHandle::Reader, this};
};

#endif
