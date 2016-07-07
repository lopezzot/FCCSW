#ifndef DETSENSITIVE_SIMPLETRACKERSD_H
#define DETSENSITIVE_SIMPLETRACKERSD_H

// DD4hep
#include "DDG4/Geant4Hits.h"

// Geant
#include "G4VSensitiveDetector.hh"
#include "G4THitsCollection.hh"

/** SimpleTrackerSD DetectorDescription/DetSensitive/src/SimpleTrackerSD.h SimpleTrackerSD.h
 *
 *  Simple sensitive detector for tracker.
 *  The position of the hit is set to the position from G4Step::GetPreStepPoint().
 *
 *  @author    Anna Zaborowska
 */
namespace det {
  class SimpleTrackerSD : public G4VSensitiveDetector
{
  public:
  /** Constructor.
   *  @param aDetectorName Name of the detector
   *  @param aReadoutName Name of the readout (used to name the collection)
   *  @param aSeg Segmentation of the detector (used to retrieve the cell ID)
   */
  SimpleTrackerSD(const std::string& aDetectorName,
    const std::string& aReadoutName,
    const DD4hep::Geometry::Segmentation& aSeg);
  /// Destructor
  virtual ~SimpleTrackerSD();
  /** Initialization.
   *  Creates the hit collection with the name passed in the constructor.
   *  The hit collection is registered in Geant.
   *  @param aHitsCollections Geant hits collection.
   */
  virtual void Initialize(G4HCofThisEvent* aHitsCollections) final;
  /** Process hit once the particle hit the sensitive volume.
   *  Checks if the energy deposit is larger than 0, calculates the position and cellID,
   *  saves that into the hit collection.
   *  New hit is created for each energy deposit (to save information about time)
   *  @param aStep Step in which particle deposited the energy.
   */
  virtual bool ProcessHits(G4Step* aStep, G4TouchableHistory*) final;

private:
  /// Collection of tracker hits
  G4THitsCollection<DD4hep::Simulation::Geant4Hit>* trackerCollection;
  /// Segmentation of the detector used to retrieve the cell Ids
  DD4hep::Geometry::Segmentation m_seg;
};
}

#endif /* DETSENSITIVE_SIMPLETRACKERSD_H */
