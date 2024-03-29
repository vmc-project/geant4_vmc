// @(#)root/g4root:$Id$
// Author: Andrei Gheata   07/08/06

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

/// \file TG4RootNavigator.cxx
/// \brief Implementation of the TG4RootNavigator class
///
/// \author A. Gheata; CERN

#include "TGeoManager.h"

#include "TG4RootDetectorConstruction.h"
#include "TG4RootNavigator.h"

#include "G4SystemOfUnits.hh"

// ClassImp(TG4RootNavigator)
// To get the current track in case to restore a geometry status
#include "G4Track.hh"
#include "G4TrackingManager.hh"

/// constant for conversion cm <-> mm
static const double gCm = 1. / cm;
static const double gZeroStepThr = 1.e-3; // >1.e-4 limit in G4PropagatorInField
static const int gAbandonZeroSteps = 40;  // <50 limit in G4PropagatorInField

//______________________________________________________________________________
TG4RootNavigator::TG4RootNavigator()
  : G4Navigator(),
    fGeometry(0),
    fNavigator(0),
    fDetConstruction(0),
    fStepEntering(kFALSE),
    fStepExiting(kFALSE),
    fNextPoint(),
    fSafetyOrig(),
    fLastSafety(0),
    fNzeroSteps(0),
    fG4TrackingManager(nullptr),
    fRestoreGeoStateFunction(nullptr)
{
  /// Dummy ctor.
}

//______________________________________________________________________________
TG4RootNavigator::TG4RootNavigator(TG4RootDetectorConstruction* dc)
  : G4Navigator(),
    fGeometry(0),
    fNavigator(0),
    fDetConstruction(0),
    fStepEntering(kFALSE),
    fStepExiting(kFALSE),
    fNextPoint(),
    fSafetyOrig(),
    fLastSafety(0),
    fNzeroSteps(0),
    fG4TrackingManager(nullptr),
    fRestoreGeoStateFunction(nullptr)
{
  /// Default ctor.
  fSafetyOrig.set(kInfinity, kInfinity, kInfinity);
  SetDetectorConstruction(dc);
  SetWorldVolume(dc->GetTopPV());
}

//______________________________________________________________________________
TG4RootNavigator::~TG4RootNavigator()
{
  /// Destructor.
}

//______________________________________________________________________________
void TG4RootNavigator::SetDetectorConstruction(TG4RootDetectorConstruction* dc)
{
  /// Setter for detector construction. Root geometry manager pointer is taken
  /// from it and must be valid.
  if (dc) fGeometry = dc->GetGeometryManager();
  if (!fGeometry || !fGeometry->IsClosed()) {
    G4Exception("TG4RootNavigator::SetDetectorConstruction", "G4Root_F001",
      FatalException,
      "Cannot create TG4RootNavigator without closed ROOT geometry !");
  }
  fNavigator = fGeometry->GetCurrentNavigator();
  if (!fNavigator) fNavigator = fGeometry->AddNavigator();
  // G4cout << "Navigator created: " << fNavigator << G4endl;
  fDetConstruction = dc;
}

//______________________________________________________________________________
G4double TG4RootNavigator::ComputeStep(const G4ThreeVector& pGlobalPoint,
  const G4ThreeVector& pDirection, const G4double pCurrentProposedStepLength,
  G4double& pNewSafety)
{
  /// Calculate the distance to the next boundary intersected
  /// along the specified NORMALISED vector direction and
  /// from the specified point in the global coordinate
  /// system. LocateGlobalPointAndSetup or LocateGlobalPointWithinVolume
  /// must have been called with the same global point prior to this call.
  /// The isotropic distance to the nearest boundary is also
  /// calculated (usually an underestimate). The current
  /// proposed Step length is used to avoid intersection
  /// calculations: if it can be determined that the nearest
  /// boundary is >pCurrentProposedStepLength away, kInfinity
  /// is returned together with the computed isotropic safety
  /// distance. Geometry must be closed.

  // The following 2 lines are not needed if G4 calls first LocateGlobalPoint...
  //   fGeometry->ResetState();
  static Long64_t istep = 0;
  istep++;

#ifdef G4ROOT_DEBUG
  G4cout.precision(8);
  G4cout << "*** ComputeStep #" << istep << ": ***"
         << fHistory.GetTopVolume()->GetName()
         << " entered: " << fEnteredDaughter << "  exited: " << fExitedMother
         << G4endl;
#endif
  Double_t tol = 0.;
  Bool_t compute_safety = kTRUE;
  Double_t pstep = pCurrentProposedStepLength;
  if (pstep > TGeoShape::Big()) {
    pstep = TGeoShape::Big();
    compute_safety = kFALSE;
    pNewSafety = 0.0;
  }

  Bool_t frombdr = fEnteredDaughter | fExitedMother;
#ifdef G4ROOT_DEBUG
  Bool_t oldpoint = kFALSE;
#endif
  if (frombdr) {
    Double_t npt[3];
    tol = TGeoShape::Tolerance();
    npt[0] = pGlobalPoint.x() * gCm + tol * pDirection.x();
    npt[1] = pGlobalPoint.y() * gCm + tol * pDirection.y();
    npt[2] = pGlobalPoint.z() * gCm + tol * pDirection.z();
    fNavigator->SetCurrentPoint(npt[0], npt[1], npt[2]);
    compute_safety = kFALSE;
    pNewSafety = 0.0;
  }
  else {
    fNavigator->SetCurrentPoint(
      pGlobalPoint.x() * gCm, pGlobalPoint.y() * gCm, pGlobalPoint.z() * gCm);
    Double_t d2 = pGlobalPoint.diff2(fSafetyOrig);
    if (d2 < 1.e-10) {
#ifdef G4ROOT_DEBUG
      oldpoint = kTRUE;
#endif
      compute_safety = kFALSE;
      pNewSafety = fLastSafety;
    }
    fSafetyOrig = pGlobalPoint;
  }
  fNavigator->SetCurrentDirection(
    pDirection.x(), pDirection.y(), pDirection.z());
  fNavigator->FindNextBoundary(-(pstep * gCm - tol), "", !compute_safety);

  if (compute_safety) {
    pNewSafety = (fNavigator->GetSafeDistance() - tol) * cm;
    if (pNewSafety < 0.) pNewSafety = 0.;
    fLastSafety = pNewSafety;
  }
  G4double step = (fNavigator->GetStep() + tol) * cm;
  //   if (step >= pCurrentProposedStepLength) step = kInfinity;
  if (step < 1.e3 * tol * cm) {
    step = 0.;
    fNzeroSteps++;
    // Geant4 will abandon the track if the number of zero steps>50 just
    // because it expects a non-zero distance inside the mother to the next
    // daughter The way out is to generate an extra very small fake step in the
    // mother, before this threshold is reached
    if (fNzeroSteps > gAbandonZeroSteps) step = gZeroStepThr;
  }
  else {
    fNzeroSteps = 0;
  }
  fStepEntering = fNavigator->IsStepEntering();
  fStepExiting = fNavigator->IsStepExiting();
  if (fStepEntering || fStepExiting) {
    fNextPoint = pGlobalPoint + step * pDirection;
  }
  else {
    step = kInfinity;
  }

#ifdef G4ROOT_DEBUG
  G4cout.precision(12);
  G4cout << "ComputeStep: point=" << pGlobalPoint << " dir=" << pDirection
         << G4endl;
  G4cout << "             pstep=" << pCurrentProposedStepLength
         << " snext=" << step << G4endl;
  G4cout << "             safe =" << pNewSafety << "  frombdr=" << frombdr
         << "  oldpoint=" << oldpoint << " entering=" << fStepEntering
         << " exiting=" << fStepExiting << G4endl;
  if (fStepEntering || fStepExiting && fNavigator->GetNextNode()) {
    G4cout << "             current: "
           << fNavigator->GetCurrentNode()->GetName()
           << "  next: " << fNavigator->GetNextNode()->GetName() << G4endl;
  }
#endif
  return step;
}

//______________________________________________________________________________
G4VPhysicalVolume* TG4RootNavigator::ResetHierarchyAndLocate(
  const G4ThreeVector& point, const G4ThreeVector& direction,
  const G4TouchableHistory& h)
{
/// Resets the geometrical hierarchy and search for the volumes deepest
/// in the hierarchy containing the point in the global coordinate space.
/// The direction is used to check if a volume is entered.
/// The search begin is the geometrical hierarchy at the location of the
/// last located point, or the endpoint of the previous Step if
/// SetGeometricallyLimitedStep() has been called immediately before.
///
/// Important Note: In order to call this the geometry MUST be closed.
///
/// In case of TGeo-based geometry all volumes look as normal positioned, so
/// there is no need to reset the hierarchy. The state of TGeo needs however
/// to be synchronized.
#ifdef G4ROOT_DEBUG
  G4cout.precision(12);
  G4cout << "ResetHierarchyAndLocate: POINT: " << point << " DIR: " << direction
         << G4endl;
#endif
  ResetState();
  fEnteredDaughter = kFALSE;
  fExitedMother = kFALSE;
  fStepEntering = kFALSE;
  fStepExiting = kFALSE;
  fHistory = *h.GetHistory();
  SynchronizeGeoManager();
  fNavigator->InitTrack(point.x() * gCm, point.y() * gCm, point.z() * gCm,
    direction.x(), direction.y(), direction.z());
  G4VPhysicalVolume* pVol = SynchronizeHistory();
  return pVol;
}

//______________________________________________________________________________
TGeoNode* TG4RootNavigator::SynchronizeGeoManager()
{
  /// Synchronize the current state of TGeoManager with the current navigation
  /// history. Do the minimum possible work in case
  /// states are already (or almost) in sync. Returns current logical node.
  Int_t geolevel = fNavigator->GetLevel();
  Int_t depth = fHistory.GetDepth();
  Int_t nodeIndex, level;
  G4VPhysicalVolume* pvol;
  TGeoNode *pnode, *newnode = 0;
  for (level = 1; level <= depth; level++) {
    pvol = fHistory.GetVolume(level);
    newnode = fDetConstruction->GetNode(pvol);
    if (level <= geolevel) {
      // TGeo has also something at this level - check if it matches what is
      // in fHistory
      pnode = fNavigator->GetMother(geolevel - level);
      // If the node at this level matches the one in the history, do nothing
      if (pnode == newnode) continue;
      // From this level down we need to update TGeo path.
      while (geolevel >= level) {
        fNavigator->CdUp();
        geolevel--;
      }
      // Now TGeo is at level-1 and needs to update level
      // this should be the index of the node to be used in CdDown(index)
      nodeIndex = fNavigator->GetCurrentVolume()->GetIndex(newnode);
      if (nodeIndex < 0) {
        G4cerr << "SynchronizeGeoManager did not work (1)!!!" << G4endl;
        return NULL;
      }
      //         nodeIndex = fHistory.GetReplicaNo(level);
      fNavigator->CdDown(nodeIndex);
      geolevel++; // Should be equal to i now
    }
    else {
      // This level has to be synchronized
      nodeIndex = fNavigator->GetCurrentVolume()->GetIndex(newnode);
      if (nodeIndex < 0) {
        G4cerr << "SynchronizeGeoManager did not work (2)!!!" << G4endl;
        return NULL;
      }
      //         nodeIndex = fHistory.GetReplicaNo(level);
      fNavigator->CdDown(nodeIndex);
      geolevel++; // Should be equal to i now
    }
  }
  return fNavigator->GetCurrentNode();
}

//______________________________________________________________________________
G4VPhysicalVolume* TG4RootNavigator::SynchronizeHistory()
{
  /// Synchronize the current navigation history according the state of
  /// TGeoManager Do the minimum possible work in case states are already (or
  /// almost) in sync. Returns current physical volume
  Int_t depth = fHistory.GetDepth();
  Int_t geolevel = fNavigator->GetLevel();
  G4VPhysicalVolume *pvol, *pnewvol = 0;
  TGeoNode* pnode;
  Int_t level;
  for (level = 0; level <= geolevel; level++) {
    pnode = fNavigator->GetMother(geolevel - level);
    pnewvol = fDetConstruction->GetG4VPhysicalVolume(pnode);
    if (level <= depth) {
      pvol = fHistory.GetVolume(level);
      // If the phys. volume at this level matches the one in the history, do
      // nothing
      if (pvol == pnewvol) continue;
      // From this level down we need to update G4 history.
      if (level) {
        fHistory.BackLevel(depth - level + 1);
        // Now fHistory is at the level i-1 and needs to update level i
        fHistory.NewLevel(pnewvol, kNormal, pnewvol->GetCopyNo());
      }
      else {
        // We need to refresh top level
        fHistory.BackLevel(depth);
        fHistory.SetFirstEntry(pnewvol);
      }
      depth = level;
    }
    else {
      // This level has to be added to the current history.
      fHistory.NewLevel(pnewvol, kNormal, pnewvol->GetCopyNo());
      depth++; // depth=level
    }
  }
  if (depth > level - 1) fHistory.BackLevel(depth - level + 1);
  if (fNavigator->IsOutside()) pnewvol = NULL;
  return pnewvol;
}

//______________________________________________________________________________
G4VPhysicalVolume* TG4RootNavigator::LocateGlobalPointAndSetup(
  const G4ThreeVector& globalPoint, const G4ThreeVector* pGlobalDirection,
  const G4bool /*relativeSearch*/, const G4bool ignoreDirection)
{
  /// Locate the point in the hierarchy return 0 if outside
  /// The direction is required
  ///    - if on an edge shared by more than two surfaces
  ///      (to resolve likely looping in tracking)
  ///    - at initial location of a particle
  ///      (to resolve potential ambiguity at boundary)
  ///
  /// Flags on exit: (comments to be completed)
  /// fEntering         - True if entering `daughter' volume (or replica)
  ///                     whether daughter of last mother directly
  ///                     or daughter of that volume's ancestor.

  static Long64_t ilocate = 0;
  ilocate++;

  // Flag if geometry state was recovered.
  Bool_t isGeoStateRestored = kFALSE;
  // Try recstore geometry for those G4Tracks which are treated as primaries
  // since they might have been transferred to GEANT4 from another engine
  if (fG4TrackingManager && fRestoreGeoStateFunction &&
      fG4TrackingManager->GetTrack()->GetParentID() == 0) {
    Int_t currG4TrackId = fG4TrackingManager->GetTrack()->GetTrackID();
    isGeoStateRestored = fRestoreGeoStateFunction(currG4TrackId);
  }

#ifdef G4ROOT_DEBUG
  G4cout.precision(12);
  G4cout << "LocateGlobalPointAndSetup #" << ilocate
         << ": point: " << globalPoint << G4endl;
#endif
  fNavigator->SetCurrentPoint(
    globalPoint.x() * gCm, globalPoint.y() * gCm, globalPoint.z() * gCm);
  fEnteredDaughter = fExitedMother = kFALSE;
  Bool_t onBoundary = kFALSE;
  if (fStepEntering || fStepExiting) {
    Double_t d2 = globalPoint.diff2(fNextPoint);
    if (d2 < 1.e-16) {
      //         fNextPoint = globalPoint;
      onBoundary = kTRUE;
    }
#ifdef G4ROOT_DEBUG
    if (onBoundary)
      G4cout << "   ON BOUNDARY "
             << "entering/exiting = " << fStepEntering << "/" << fStepExiting
             << G4endl;
    else
      G4cout << "   IN VOLUME   "
             << "entering/exiting = " << fStepEntering << "/" << fStepExiting
             << G4endl;
#endif
  }
  if ((!ignoreDirection || onBoundary) && pGlobalDirection) {
    fNavigator->SetCurrentDirection(
      pGlobalDirection->x(), pGlobalDirection->y(), pGlobalDirection->z());
  }

#ifdef G4ROOT_DEBUG
  printf("   level %i: %s\n", fNavigator->GetLevel(), fNavigator->GetPath());
  if (fNavigator->IsOutside()) G4cout << "   outside" << G4endl;
#endif
  if (onBoundary) {
    fEnteredDaughter = fStepEntering;
    fExitedMother = fStepExiting;
    TGeoNode* skip = fNavigator->GetCurrentNode();
    if (fNavigator->IsOutside()) skip = NULL;
    if (fStepExiting && !fNavigator->GetLevel()) {
      fNavigator->SetOutside();
      return NULL;
    }
    fNavigator->CdNext();
    fNavigator->CrossBoundaryAndLocate(fStepEntering, skip);
  }
  else if (!isGeoStateRestored) {
    //      if (!relativeSearch) fNavigator->CdTop();
    fNavigator->FindNode();
  }
  G4VPhysicalVolume* target = SynchronizeHistory();
#ifdef G4ROOT_DEBUG
  if (target)
    G4cout << "   POINT INSIDE: " << target->GetName()
           << " entered=" << fEnteredDaughter << " exited=" << fExitedMother
           << G4endl;
#endif
  return target;
}

//______________________________________________________________________________
void TG4RootNavigator::LocateGlobalPointWithinVolume(
  const G4ThreeVector& pGlobalPoint)
{
/// Notify the Navigator that a track has moved to the new Global point
/// 'position', that is known to be within the current safety.
/// No check is performed to ensure that it is within  the volume.
/// This method can be called instead of LocateGlobalPointAndSetup ONLY if
/// the caller is certain that the new global point (position) is inside the
/// same volume as the previous position.  Usually this can be guaranteed
/// only if the point is within safety.
///   fLastLocatedPointLocal = ComputeLocalPoint(pGlobalPoint);
///   G4cout << "LocateGlobalPointWithinVolume: POINT: " << pGlobalPoint <<
///   G4endl;
#ifdef G4ROOT_DEBUG
  G4cout.precision(12);
  G4cout << "LocateGlobalPointWithinVolume " << pGlobalPoint << G4endl;
#endif
  fNavigator->SetCurrentPoint(
    pGlobalPoint.x() * gCm, pGlobalPoint.y() * gCm, pGlobalPoint.z() * gCm);
  fStepEntering = kFALSE;
  fStepExiting = kFALSE;
  fEnteredDaughter = kFALSE;
  fExitedMother = kFALSE;
}

//______________________________________________________________________________
G4double TG4RootNavigator::ComputeSafety(const G4ThreeVector& globalpoint,
  const G4double pProposedMaxLength, const G4bool /*keepState*/)
{
  /// Compute safety and keep state on demand.
  G4double saf = ComputeSafety(globalpoint, pProposedMaxLength);
  return saf;
}

//______________________________________________________________________________
G4double TG4RootNavigator::ComputeSafety(
  const G4ThreeVector& globalpoint, const G4double /*pProposedMaxLength*/)
{
  /// Calculate the isotropic distance to the nearest boundary from the
  /// specified point in the global coordinate system.
  /// The globalpoint utilised must be within the current volume.
  /// The value returned is usually an underestimate.
  /// The proposed maximum length is used to avoid volume safety
  /// calculations.  The geometry must be closed.

  /// TO CHANGE TGeoManager::Safety To take into account pProposedMaxLength
  ///   fEnteredDaughter = kFALSE;
  ///   fExitedMother = kFALSE;
  ///   fStepEntering = kFALSE;
  ///   fStepExiting = kFALSE;
  Double_t d2 = globalpoint.diff2(fNextPoint);
  if (d2 < 1.e-10) {
#ifdef G4ROOT_DEBUG
    G4cout << "ComputeSafety: POINT on boundary: " << globalpoint
           << " SKIPPED... oldsafe= 0.0" << G4endl;
#endif
    return 0.0;
  }
  d2 = globalpoint.diff2(fSafetyOrig);
  if (d2 < 1.e-10) {
#ifdef G4ROOT_DEBUG
    G4cout << "ComputeSafety: POINT not changed: " << globalpoint
           << " SKIPPED... oldsafe=" << fLastSafety << G4endl;
#endif
    return fLastSafety;
  }
  fNavigator->ResetState();
  fNavigator->SetCurrentPoint(
    globalpoint.x() * gCm, globalpoint.y() * gCm, globalpoint.z() * gCm);
  G4double safety = fNavigator->Safety() * cm;
  fSafetyOrig = globalpoint;
  fLastSafety = safety;

#ifdef G4ROOT_DEBUG
  G4cout.precision(12);
  G4cout << "ComputeSafety: POINT: " << globalpoint << " safe = " << safety
         << G4endl;
#endif
  return safety;
}

//______________________________________________________________________________
G4TouchableHandle TG4RootNavigator::CreateTouchableHistoryHandle() const
{
  /// Returns a reference counted handle to a touchable history.
  return G4Navigator::CreateTouchableHistoryHandle();
}

//______________________________________________________________________________
G4ThreeVector TG4RootNavigator::GetLocalExitNormal(G4bool* valid)
{
  /// Returns Exit Surface Normal and validity too.
  /// It can only be called if the Navigator's last Step has crossed a
  /// volume geometrical boundary.
  /// It returns the Normal to the surface pointing out of the volume that
  /// was left behind and/or into the volume that was entered.
  /// (The normal is in the coordinate system of the final volume.)
  /// This function takes full care about how to calculate this normal,
  /// but if the surfaces are not convex it will return valid=false.
  Double_t *norm, lnorm[3];
  *valid = true;
  norm = fNavigator->FindNormalFast();
  G4ThreeVector normal(0., 0., 1.);
  if (!norm) {
    *valid = false;
    return normal;
  }
  fNavigator->MasterToLocalVect(norm, lnorm);
  normal.setX(lnorm[0]);
  normal.setY(lnorm[1]);
  normal.setZ(lnorm[2]);
#ifdef G4ROOT_DEBUG
  G4cout << "GetLocalExitNormal: " << normal << G4endl;
#endif
  return normal;
}

//______________________________________________________________________________
G4ThreeVector TG4RootNavigator::GetGlobalExitNormal(
  const G4ThreeVector& /*point*/, G4bool* valid)
{
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
  Double_t* norm;
  *valid = true;
  norm = fNavigator->FindNormalFast();
  G4ThreeVector normal(0., 0., 1.);
  if (!norm) {
    *valid = false;
    return normal;
  }
  normal.setX(norm[0]);
  normal.setY(norm[1]);
  normal.setZ(norm[2]);
#ifdef G4ROOT_DEBUG
  G4cout << "GetGlobalExitNormal: " << normal << G4endl;
#endif
  return normal;
}

//______________________________________________________________________________
void TG4RootNavigator::SetG4TrackingManager(G4TrackingManager* trackingManager)
{
  fG4TrackingManager = trackingManager;
}

//______________________________________________________________________________
void TG4RootNavigator::SetGeometryRestoreFunction(
  std::function<Bool_t(Int_t)> restoreGeoStateFunction)
{
  fRestoreGeoStateFunction = restoreGeoStateFunction;
}
