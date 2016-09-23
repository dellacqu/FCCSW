#ifndef DETSENSITIVE_GFLASHCALORIMETERSD_H
#define DETSENSITIVE_GFLASHCALORIMETERSD_H

// DD4hep
#include "DDG4/Geant4Hits.h"
#include "DDSegmentation/Segmentation.h"

// Geant
#include "G4VSensitiveDetector.hh"
#include "G4VGFlashSensitiveDetector.hh"
#include "G4THitsCollection.hh"

/** GFlashCalorimeterSD DetectorDescription/DetSensitive/src/GFlashCalorimeterSD.h GFlashCalorimeterSD.h
 *
 *  Sensitive detector for calorimeters that use GFlash parametrisation.
 *  If no parametrisation is invoked, hits are processed as in det::SimpleCalorimeterSD.
 *  No timing information is saved (for full sim: energy deposits aggregated in the cells,
 *  for gflash: hits are created instantly)
 *
 *  @author    Anna Zaborowska
 */

namespace det {
class GFlashCalorimeterSD : public G4VSensitiveDetector, public G4VGFlashSensitiveDetector {
public:
  /** Constructor.
   *  @param aDetectorName Name of the detector
   *  @param aReadoutName Name of the readout (used to name the collection)
   *  @param aSeg Segmentation of the detector (used to retrieve the cell ID)
   */
  GFlashCalorimeterSD(const std::string& aDetectorName,
                      const std::string& aReadoutName,
                      const DD4hep::Geometry::Segmentation& aSeg);
  /// Destructor
  ~GFlashCalorimeterSD();
  /** Initialization.
   *  Creates the hit collection with the name passed in the constructor.
   *  The hit collection is registered in Geant.
   *  @param aHitsCollections Geant hits collection.
   */
  void Initialize(G4HCofThisEvent* aHCE);
  /** Process hit once the particle hit the sensitive volume (anf full sim is performed)
   *  Full simulation is be invoked if the gflash model is not triggered (e.g. because of confinement)
   *  Checks if the energy deposit is larger than 0, calculates the position and cellID,
   *  saves that into the hit collection.
   *  If there is already entry in the same cell, the energy is accumulated.
   *  Otherwise new hit is created.
   *  @param aStep Step in which particle deposited the energy.
   */
  virtual bool ProcessHits(G4Step* aStep, G4TouchableHistory*) final;
  /** Process hit once the particle hit the sensitive volume and gflash parametrisation is triggered.
   *  Checks if the energy deposit is larger than 0, calculates the position and cellID,
   *  saves that into the hit collection.
   *  If there is already entry in the same cell, the energy is accumulated.
   *  Otherwise new hit is created.
   *  @param aSpot Spot in which particle triggered the GFlash model.
   */
  virtual bool ProcessHits(G4GFlashSpot* aSpot, G4TouchableHistory*) final;
  uint64_t cellID(const G4GFlashSpot& aSpot);
private:
  /// Collection of calorimeter hits that get registered in G4Event and deleted in ~G4Event
  G4THitsCollection<DD4hep::Simulation::Geant4CalorimeterHit>* calorimeterCollection;
  /// Segmentation of the detector used to retrieve the cell Ids
  DD4hep::Geometry::Segmentation m_seg;
};
}

#endif /* DETSENSITIVE_GFLASHCALORIMETERSD_H */
