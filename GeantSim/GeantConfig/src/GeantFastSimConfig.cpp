#include "GeantFastSimConfig.h"

// Gaudi
#include "GaudiKernel/DeclareFactoryEntries.h"

// Geant4
#include "FTFP_BERT.hh"
#include "G4VModularPhysicsList.hh"
#include "G4RegionStore.hh"
#include "G4VFastSimulationModel.hh"

// FCCSW
#include "GeantFast/FastSimPhysics.h"
#include "GeantFast/FastSimModelTracker.h"
#include "GeantGeneral/TrackingAction.h"
#include "GeantComponents/ISmearingTool.h"

DECLARE_COMPONENT(GeantFastSimConfig)

GeantFastSimConfig::GeantFastSimConfig(const std::string& type, const std::string& name, const IInterface* parent) :
GaudiTool(type, name, parent) {
   declareInterface<IGeantConfigTool>(this);
   declareProperty ("smearing", m_smearToolName = "SimpleSmear" ) ;
}

GeantFastSimConfig::~GeantFastSimConfig() {}

StatusCode GeantFastSimConfig::initialize() {
   if(GaudiTool::initialize().isFailure()) {
      return StatusCode::FAILURE;
   }
   m_smearTool = tool<ISmearingTool>(m_smearToolName);
   return StatusCode::SUCCESS;
}

G4VModularPhysicsList* GeantFastSimConfig::getPhysicsList() {
   // we pass the ownership to Geant4Simulation which will register it in G4RunManager and will be deleted in ~G4RunManager()
   G4VModularPhysicsList* physicsList = new FTFP_BERT;
   // Coupled transportation enables calculation of particle trajectory in envelopes with fast sim models attached
   G4PhysicsListHelper::GetPhysicsListHelper()->UseCoupledTransportation();
   // Attach Fast Simulation Process (will take over normal transportation if FastSimModel triggered
   physicsList->RegisterPhysics(new FastSimPhysics);
   return physicsList;
}

G4UserTrackingAction* GeantFastSimConfig::getTrackingAction() {
   // G4VUserTrackingAction deleted in ~G4TrackingManager()
   G4UserTrackingAction* trackingAction = new TrackingAction;
   return trackingAction;
}

StatusCode GeantFastSimConfig::getOtherSettings() {
   // Create the envelopes for the geometry, attach fast sim models for Tracker, EMCal, HCal
   G4LogicalVolume* world = (*G4TransportationManager::GetTransportationManager()->GetWorldsIterator())->GetLogicalVolume();
   for(int iter_region = 0; iter_region<world->GetNoDaughters(); ++iter_region) {
      // TO DO: proper integration with DD4hep to resolve where to attach a fast sim model
      if(world->GetDaughter(iter_region)->GetName().find("Tracker") != std::string::npos) {
          m_g4regions.emplace_back(new G4Region(world->GetDaughter(iter_region)->GetName()+"_fastsim"));
          info()<<world->GetDaughter(iter_region)->GetName()<<"\t"<<world->GetDaughter(iter_region)->IsRegion()<<endmsg;
          //m_g4regions[iter_region]->AddRootLogicalVolume(world->GetDaughter(iter_region)->GetLogicalVolume());
          //m_models.emplace_back(new FastSimModelTracker(m_g4regions.back()->GetName(),m_g4regions.back(),m_smearToolName));
   // info()<<"Attaching a Fast Simulation Model to the region "<<m_g4regions.back()->GetName()<<endmsg;
         }
          }
   return StatusCode::SUCCESS;
}
