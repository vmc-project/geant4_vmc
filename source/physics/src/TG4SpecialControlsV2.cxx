//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4SpecialControlsV2.cxx
/// \brief Implementation of the TG4SpecialControlsV2 class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SpecialControlsV2.h"
#include "TG4GeometryServices.h"
#include "TG4Limits.h"
#include "TG4G3PhysicsManager.h"
#include "TG4G3ParticleWSP.h"

#include <G4StepStatus.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessVector.hh>

//_____________________________________________________________________________
TG4SpecialControlsV2::TG4SpecialControlsV2()
  : TG4Verbose("specialControlsV2"),
    fIsApplicable(false),
    fkTrack(0),
    fSwitch(kUnswitch),
    fSwitchedProcesses(),
    fSwitchedControls(),
    fProcessActivations()
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4SpecialControlsV2::~TG4SpecialControlsV2()
{
/// Destructor
}

//
// private methods
//

//_____________________________________________________________________________
void TG4SpecialControlsV2::SetSwitch()
{
/// Define the action which should be performed at this step

  // get limits
#ifdef MCDEBUG
  TG4Limits* limits
     = TG4GeometryServices::Instance()
         ->GetLimits(fkTrack->GetNextVolume()->GetLogicalVolume()->GetUserLimits());
#else
  TG4Limits* limits
    = (TG4Limits*) fkTrack->GetNextVolume()->GetLogicalVolume()->GetUserLimits();
#endif

  if ( ! limits ) {
    TG4Globals::Warning(
      "TG4SpecialControlsV2", "SetSwitch",
      "No limits defined in " +
      TString(fkTrack->GetNextVolume()->GetLogicalVolume()->GetName()));
    return;
  }

  if ( fSwitch != kUnswitch ) {
    if  ( limits->IsControl() ) {
      // particle is exiting a logical volume with special controls
      // and entering another logical volume with special controls
      fSwitch = kReswitch;
      if (VerboseLevel() > 1)  G4cout << "kReswitch" << G4endl;
    }
    else {
      // particle is exiting a logical volume with special controls
      // and entering a logical volume without special controls
      fSwitch = kUnswitch;
      if (VerboseLevel() > 1)  G4cout << "kUnswitch" << G4endl;
    }
  }
  else if (limits->IsControl()) {
    // particle is entering a logical volume with special controls
    // that have not yet been set
    fSwitch = kSwitch;
    if (VerboseLevel() > 1) G4cout << "kSwitch" << G4endl;
  }
}

//_____________________________________________________________________________
void TG4SpecialControlsV2::Reset()
{
/// Reset the buffers to the initial state.

  fSwitch = kUnswitch;

  // clear buffers
  fSwitchedProcesses.clear();
  fSwitchedControls.clear();
  fProcessActivations.clear();
}

//
// public methods
//

//_____________________________________________________________________________
void TG4SpecialControlsV2::StartTrack(const G4Track* track)
{
/// Store the processes activations in fProcessActivations vector

  // check applicability
  G4ParticleDefinition* particle = track->GetDefinition();
  TG4G3ParticleWSP particleWSP
      = TG4G3PhysicsManager::Instance()->GetG3ParticleWSP(particle);

  if ( particleWSP == kNofParticlesWSP ) {
    fIsApplicable = false;
    fkTrack = 0;
    return;
  }

  // set applicability, current track
  fIsApplicable = true;
  fkTrack = track;

  // save origin process activation
  G4ProcessManager* processManager
    = track->GetDefinition()->GetProcessManager();
  G4ProcessVector* processVector = processManager->GetProcessList();

  for (G4int i=0; i<processVector->length(); i++) {
    fProcessActivations.push_back(processManager->GetProcessActivation(i));
  }

  // apply controls
  ApplyControls();
}


//_____________________________________________________________________________
void TG4SpecialControlsV2::ApplyControls()
{
/// Change processes activation of the current track
/// according to the current user limits.

#ifdef MCDEBUG
  if ( ! fkTrack ) {
    TG4Globals::Exception(
      "TG4SpecialControlsV2", "ApplyControls", "No track is set.");
  }
#endif

  SetSwitch();

  G4ProcessManager* processManager
    = fkTrack->GetDefinition()->GetProcessManager();
  G4ProcessVector* processVector = processManager->GetProcessList();

  if ( fSwitch == kUnswitch || fSwitch == kReswitch ) {

    // set processes activation back
    for ( G4int i=0; i<fSwitchedProcesses.length(); i++ ) {
      if ( VerboseLevel() > 1 ) {
        G4cout << "Reset process activation back in "
               << fkTrack->GetNextVolume()->GetName()
               << G4endl;
      }
      processManager
        ->SetProcessActivation(fSwitchedProcesses[i], fSwitchedControls[i]);
    }
    fSwitchedProcesses.clear();
    fSwitchedControls.clear();
  }

  if ( fSwitch == kSwitch || fSwitch == kReswitch ) {

    // set TG4Limits processes controls
    TG4Limits* limits
    = (TG4Limits*) fkTrack->GetNextVolume()->GetLogicalVolume()->GetUserLimits();

    for ( G4int i=0; i<processVector->length(); i++ ) {

      TG4G3ControlValue control = limits->GetControl((*processVector)[i]);
      G4bool activation = processManager->GetProcessActivation(i);

      if (   control != kUnsetControlValue &&
           ! TG4Globals::Compare(activation, control) ) {

        // store the current processes controls
        if (VerboseLevel() > 1) {
          G4cout << "Something goes to fSwitchedProcesses" << G4endl;
        }
        fSwitchedProcesses.insert((*processVector)[i]);
        fSwitchedControls.push_back(activation);

        // set new process activation
        if (control == kInActivate) {
          if (VerboseLevel() > 1) {
            G4cout << "Set process inactivation for "
                   << (*processVector)[i]->GetProcessName() << " in "
                       << fkTrack->GetNextVolume()->GetName()
                   << G4endl;
          }
          processManager->SetProcessActivation(i,false);
        }
        else {
          // ((control == kActivate) || (control == kActivate2))
          if (VerboseLevel() > 1) {
            G4cout << "Set process activation for "
                   << (*processVector)[i]->GetProcessName() << " in "
                   << fkTrack->GetNextVolume()->GetName()
                   << G4endl;
          }
          processManager->SetProcessActivation(i,true);
        }
      }
    }
  }
}

//_____________________________________________________________________________
void TG4SpecialControlsV2::RestoreProcessActivations()
{
/// Restore the processes activations from fProcessActivations vector
/// and reset values

  G4ProcessManager* processManager
    = fkTrack->GetDefinition()->GetProcessManager();
  G4ProcessVector* processVector = processManager->GetProcessList();

  for (G4int i=0; i<processVector->length(); i++) {
    if ( processManager->GetProcessActivation(i) != fProcessActivations[i] ) {
      processManager->SetProcessActivation(i,fProcessActivations[i]);
    }
  }

  Reset();
}

