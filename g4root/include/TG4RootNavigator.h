// @(#)root/g4root:$Id$
// Author: Andrei Gheata   07/08/06

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

/// \file TG4RootNavigator.h
/// \brief Definition of the TG4RootNavigator class
///
/// \author A. Gheata; CERN

#ifndef ROOT_TG4RootNavigator
#define ROOT_TG4RootNavigator

#include <functional>

#include "G4Navigator.hh"

#include <Rtypes.h>

class TGeoManager;
class TGeoNavigator;
class TGeoNode;
class TG4RootDetectorConstruction;
class G4TrackingManager;

/// \brief GEANT4 navigator using directly a TGeo geometry.
///
/// All navigation methods requred by G4 tracking are implemented by
/// this class by invoking the corresponding functionality of ROOT
/// geometry modeler.
///
/// \author A. Gheata; CERN

class TG4RootNavigator : public G4Navigator
{

 protected:
  TGeoManager* fGeometry;                        ///< TGeo geometry manager
  TGeoNavigator* fNavigator;                     ///< TGeo navigator
  TG4RootDetectorConstruction* fDetConstruction; ///< G4Root detector
                                                 /// construction

  Bool_t fStepEntering;      ///< Next step is entering daughter
  Bool_t fStepExiting;       ///< Next step is exiting current volume
  G4ThreeVector fNextPoint;  ///< Crossing point with next boundary
  G4ThreeVector fSafetyOrig; ///< Last computed safety origin
  G4double fLastSafety;      ///< Last computed safety
  Int_t fNzeroSteps;         ///< Number of zero steps in ComputeStep
  G4TrackingManager* fG4TrackingManager; ///< Store pointer to G4TrackingManager
  std::function<Bool_t(Int_t)> fRestoreGeoStateFunction; ///< Function pointer
                                                         /// to restore geometry
 private:
  G4VPhysicalVolume* SynchronizeHistory();
  TGeoNode* SynchronizeGeoManager();

 public:
  TG4RootNavigator();
  TG4RootNavigator(TG4RootDetectorConstruction* dc);
  virtual ~TG4RootNavigator();

  void SetDetectorConstruction(TG4RootDetectorConstruction* dc);

  /// Return the navigation history
  G4NavigationHistory* GetHistory() { return &fHistory; }

  // Virtual methods for navigation
  virtual G4double ComputeStep(const G4ThreeVector& pGlobalPoint,
    const G4ThreeVector& pDirection, const G4double pCurrentProposedStepLength,
    G4double& pNewSafety);
  virtual G4VPhysicalVolume* ResetHierarchyAndLocate(const G4ThreeVector& point,
    const G4ThreeVector& direction, const G4TouchableHistory& h);
  virtual G4VPhysicalVolume* LocateGlobalPointAndSetup(
    const G4ThreeVector& point, const G4ThreeVector* direction = 0,
    const G4bool pRelativeSearch = true, const G4bool ignoreDirection = true);
  virtual void LocateGlobalPointWithinVolume(const G4ThreeVector& position);
  //   virtual void LocateGlobalPointAndUpdateTouchableHandle(
  //                 const G4ThreeVector&       position,
  //                 const G4ThreeVector&       direction,
  //                       G4TouchableHandle&   oldTouchableToUpdate,
  //                 const G4bool               RelativeSearch = true);
  virtual G4double ComputeSafety(
    const G4ThreeVector& globalpoint, const G4double pProposedMaxLength);
  virtual G4double ComputeSafety(const G4ThreeVector& globalpoint,
    const G4double pProposedMaxLength, const G4bool keepState);
  virtual G4TouchableHandle CreateTouchableHistoryHandle() const;
  virtual G4ThreeVector GetLocalExitNormal(G4bool* valid);
  virtual G4ThreeVector GetGlobalExitNormal(
    const G4ThreeVector& point, G4bool* valid);
  // Return Exit Surface Normal and validity too.
  // Can only be called if the Navigator's last Step has crossed a
  // volume geometrical boundary.
  // It returns the Normal to the surface pointing out of the volume that
  // was left behind and/or into the volume that was entered.
  // Convention:
  //   The *local* normal is in the coordinate system of the *final* volume.
  // Restriction:
  //   Normals are not available for replica volumes (returns valid= false)
  // These methods takes full care about how to calculate this normal,
  // but if the surfaces are not convex it will return valid=false.

  /// Set current G4TrackingManager
  void SetG4TrackingManager(G4TrackingManager* trackingManager);

  /// Store and call the given function to request the restoring of the geometry
  /// status. Returns true if the geometry was restored, otherwise the node has
  /// to be found manually.
  void SetGeometryRestoreFunction(
    std::function<Bool_t(Int_t)> restoreGeoStateFunction);

  //   ClassDef(TG4RootNavigator,0)  // Class defining a G4Navigator based on
  //   ROOT geometry
};
#endif
