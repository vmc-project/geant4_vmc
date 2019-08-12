//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4TrackingAction.cxx
/// \brief Implementation of the TG4TrackingAction class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4TrackingAction.h"
#include "TG4GeometryServices.h"
#include "TG4Globals.h"
#include "TG4ParticlesManager.h"
#include "TG4PhysicsManager.h"
#include "TG4SDServices.h"
#include "TG4SensitiveDetector.h"
#include "TG4SpecialControlsV2.h"
#include "TG4StackPopper.h"
#include "TG4StepManager.h"
#include "TG4TrackInformation.h"
#include "TG4TrackManager.h"

#include <TMCParticleStatus.h>
#include <TMCProcess.h>
#include <TVirtualMC.h>
#include <TVirtualMCApplication.h>

#include <G4TrackVector.hh>
#include <G4TrackingManager.hh>
#include <G4UImanager.hh>

// static data members
G4ThreadLocal TG4TrackingAction* TG4TrackingAction::fgInstance = 0;

//_____________________________________________________________________________
TG4TrackingAction::TG4TrackingAction()
  : G4UserTrackingAction(),
    TG4Verbose("trackingAction", 2),
    fMessenger(this),
    fSpecialControls(0),
    fTrackManager(0),
    fMCApplication(0),
    fMCStack(0),
    fStepManager(0),
    fStackPopper(0),
    fPrimaryTrackID(0),
    fCurrentTrackID(0),
    fTrackSaveControl(kDoNotSave),
    fOverwriteLastTrack(false),
    fNewVerboseLevel(0),
    fNewVerboseTrackID(-1),
    fDoFinishPrimary(true)
{
  /// Default constructor

  if (fgInstance) {
    TG4Globals::Exception("TG4TrackingAction", "TG4TrackingAction",
      "Cannot create two instances of singleton.");
  }

  fTrackManager = new TG4TrackManager();

  fgInstance = this;
}

//_____________________________________________________________________________
TG4TrackingAction::~TG4TrackingAction()
{
  /// Destructor

  fgInstance = 0;

  delete fTrackManager;
}

//
// private methods
//

//_____________________________________________________________________________
void TG4TrackingAction::UserProcessHits(const G4Track* /*track*/)
{
  /// Let sensitive detector process the vertex step
  /// (this ensures compatibility with G3 that
  /// makes first step of zero length)

  G4VPhysicalVolume* pv = fStepManager->GetCurrentPhysicalVolume();

  if (!pv) {
    // was exception
    TG4Globals::Warning(
      "TG4TrackingAction", "UserProcessHits", "Cannot locate track vertex.");
    return;
  }

#ifdef MCDEBUG
  TG4SensitiveDetector* tsd = TG4SDServices::Instance()->GetSensitiveDetector(
    pv->GetLogicalVolume()->GetSensitiveDetector());

  if (tsd) tsd->ProcessHitsOnTrackStart();
#else
  TG4SensitiveDetector* tsd =
    (TG4SensitiveDetector*)pv->GetLogicalVolume()->GetSensitiveDetector();

  if (tsd) tsd->ProcessHitsOnTrackStart();
#endif
}

//_____________________________________________________________________________
void TG4TrackingAction::Verbose() const
{
  /// Print primary track ID with a frequence defined
  /// by verbose level.

  if ((VerboseLevel() == 1 && fPrimaryTrackID % 100 == 0) ||
      (VerboseLevel() == 2 && fPrimaryTrackID % 10 == 0) ||
      VerboseLevel() == 3) {

    G4cout << "$$$X Primary track " << fPrimaryTrackID << G4endl;
  }
}

//
// public methods
//

//_____________________________________________________________________________
void TG4TrackingAction::LateInitialize()
{
  /// Cache thread-local pointers

  fMCApplication = TVirtualMCApplication::Instance();
  fStepManager = TG4StepManager::Instance();
  fStackPopper = TG4StackPopper::Instance();

  fTrackManager->LateInitialize();
}

//_____________________________________________________________________________
void TG4TrackingAction::PrepareNewEvent()
{
  /// Called by G4 kernel at the beginning of event.

  // set g4 stepping manager pointer and world volume
  fStepManager->SetSteppingManager(fpTrackingManager->GetSteppingManager());

  fTrackManager->SetG4TrackingManager(fpTrackingManager);

  if (fTrackManager->GetTrackSaveControl() != kDoNotSave)
    fTrackManager->SetNofTracks(0);
  else
    fTrackManager->SetNofTracks(fMCStack->GetNtrack());

  fCurrentTrackID = 0;
}

//_____________________________________________________________________________
void TG4TrackingAction::PreUserTrackingAction(const G4Track* track)
{
  /// Called by G4 kernel before starting tracking.

  // do not call this function more than once
  if (track->GetTrackID() == fCurrentTrackID) return;

  // keep this track number for the check above
  fCurrentTrackID = track->GetTrackID();

  G4bool isFirstStep = (track->GetCurrentStepNumber() == 0);

  // finish previous primary track first
  if (track->GetParentID() == 0 && isFirstStep) {
    FinishPrimaryTrack();
  }

  // initialize special controls manager
  if (fSpecialControls) fSpecialControls->StartTrack(track);

  // reset stack popper (if activated
  if (fStackPopper) fStackPopper->Reset();

  // set track information
  TG4TrackInformation* trackInfo =
    fTrackManager->SetTrackInformation(track, fOverwriteLastTrack);

  fStepManager->SetInitialVMCTrackStatus(nullptr);

  TMCParticleStatus* particleStatus =
    const_cast<TMCParticleStatus*>(trackInfo->GetInitialTrackStatus());

  if (isFirstStep) {
    // set step manager status
    fStepManager->SetStep((G4Track*)track, kVertex);
    fStepManager->SetInitialVMCTrackStatus(particleStatus);

    if (track->GetParentID() == 0) {
      fPrimaryTrackID = track->GetTrackID();

      // begin this primary track if the VMC particle is actually a primary and
      // only if it has initial step number == 0
      if (!particleStatus ||
          (particleStatus->fStepNumber == 0 && particleStatus->fParentId < 0)) {
        fMCStack->SetCurrentTrack(trackInfo->GetTrackParticleID());
        fMCApplication->BeginPrimary();
      }

      fDoFinishPrimary = true;
      if (particleStatus && particleStatus->fParentId > -1) {
        // Flag that to be a
        fDoFinishPrimary = false;
      }

      // set saving flag
      fTrackSaveControl = kDoNotSave;
    }
    else {
      // set saving flag
      fTrackSaveControl = (fTrackManager->IsUserTrack(track))
                            ? kDoNotSave
                            : fTrackManager->GetTrackSaveControl();
    }

    // save track in stack
    if (fTrackSaveControl == kSaveInPreTrack) {
      fTrackManager->TrackToStack(track, fOverwriteLastTrack);

      // Notify a stack popper (if activated) about saving this secondary
      if (fStackPopper) fStackPopper->Notify();
    }
  }

  fMCStack->SetCurrentTrack(trackInfo->GetTrackParticleID());

  // verbose
  if (track->GetTrackID() == fNewVerboseTrackID) {
    fpTrackingManager->SetVerboseLevel(fNewVerboseLevel);
  }

  // VMC application pre track action
  if (isFirstStep) {
    if (!particleStatus || particleStatus->fStepNumber == 0) {
      fMCApplication->PreTrack();
    }

    // call pre-tracking action of derived class
    PreTrackingAction(track);

    if (track->GetTrackStatus() != fStopAndKill) {
      // Let sensitive detector process vertex step
      UserProcessHits(track);
    }
  }
}

//_____________________________________________________________________________
void TG4TrackingAction::PostUserTrackingAction(const G4Track* track)
{
  /// Called by G4 kernel after finishing tracking.

#ifdef STACK_WITH_KEEP_FLAG
  // Remember whether this track should be kept in the stack
  // or can be overwritten:
  // the track will not be overwritten if it was flagged in the stack
  // to be kept or if it has produced any secondary particles.
  fOverwriteLastTrack = (!fMCStack->GetKeepCurrentTrack()) &&
                        (!fpTrackingManager->GimmeSecondaries() ||
                          fpTrackingManager->GimmeSecondaries()->size() == 0);
  // Experimental code with flagging tracks in stack for overwrite;
  // not yet available in distribution
#else
  fOverwriteLastTrack = false;
#endif

  // restore processes activation
  if (fSpecialControls && fSpecialControls->IsApplicable())
    fSpecialControls->RestoreProcessActivations();

  // set back max step limit if it has been modified on fly by user
  if (fStepManager->GetLimitsModifiedOnFly()) fStepManager->SetMaxStepBack();

  // set parent track particle index to the secondary tracks
  fTrackManager->SetParentToTrackInformation(track);

  // restore particle lifetime if it was modified by user
  fTrackManager->SetBackPDGLifetime(track);

  // Do this only if the track was not interrupted but either stopped or for all
  // other reasons the transport has been finished.
  auto trackInfo = fTrackManager->GetTrackInformation(track);
  if (track->GetTrackStatus() != fSuspend && !trackInfo->IsInterrupt()) {

    // VMC application post track action
    fMCApplication->PostTrack();

    // call post-tracking action of derived class
    PostTrackingAction(track);
  }
  const TMCParticleStatus* particleStatus = trackInfo->GetInitialTrackStatus();
  // If this is an interrupted primary, when we pick up the next primary,
  // this one is not finished yet...
  if (particleStatus && particleStatus->fParentId < 0 &&
      trackInfo->IsInterrupt()) {
    fDoFinishPrimary = false;
  }
}

//_____________________________________________________________________________
void TG4TrackingAction::FinishPrimaryTrack()
{
  /// Call VMC application to finish a primary track. \n
  /// !! This method has to be also called from the
  /// EventAction::EndOfEventAction() for storing the last primary track of the
  /// current event.
  // ---

  if (fPrimaryTrackID > 0) {

    // set special step manager status
    // not in both stepping, vertex stage
    G4Track* noTrack = 0;
    fStepManager->SetStep(noTrack, kVertex);

    // verbose
    Verbose();

    // VMC application finish primary track
    if (fDoFinishPrimary) {
      fMCApplication->FinishPrimary();
      fDoFinishPrimary = false;
    }
  }

  fPrimaryTrackID = 0;
}

//_____________________________________________________________________________
void TG4TrackingAction::SetNewVerboseLevel(G4int level)
{
  /// Set the new verbose level that will be set when the track with
  /// specified track ID (fNewVerboseTrackID) starts tracking.

  fNewVerboseLevel = level;
}

//_____________________________________________________________________________
void TG4TrackingAction::SetNewVerboseTrackID(G4int trackID)
{
  /// Set the trackID for which the new verbose level (fNewVerboseLevel)
  /// will be applied.

  fNewVerboseTrackID = trackID;
}

//_____________________________________________________________________________
G4int TG4TrackingAction::GetNewVerboseLevel() const
{
  /// Return new /tracking/verbose level

  return fNewVerboseLevel;
}

//_____________________________________________________________________________
G4int TG4TrackingAction::GetNewVerboseTrackID() const
{
  /// Return track ID for which the new verbose level is applied

  return fNewVerboseTrackID;
}

//_____________________________________________________________________________
TG4TrackManager* TG4TrackingAction::GetTrackManager() const
{
  /// Return track manager

  return fTrackManager;
}
