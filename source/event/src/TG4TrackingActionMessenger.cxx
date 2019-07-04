//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4TrackingActionMessenger.cxx
/// \brief Implementation of the TG4TrackingActionMessenger class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4TrackingActionMessenger.h"
#include "TG4Globals.h"
#include "TG4TrackManager.h"
#include "TG4TrackingAction.h"

#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIdirectory.hh>

//_____________________________________________________________________________
TG4TrackingActionMessenger::TG4TrackingActionMessenger(
  TG4TrackingAction* trackingAction)
  : G4UImessenger(),
    fTrackingAction(trackingAction),
    fTrackingDirectory(0),
    fNewVerboseCmd(0),
    fNewVerboseTrackCmd(0),
    fSaveSecondariesCmd(0),
    fSaveDynamicChargeCmd(0)
{
  /// Standard constructor

  fTrackingDirectory = new G4UIdirectory("/mcTracking/");
  fTrackingDirectory->SetGuidance("TG4TrackingAction control commands.");

  fNewVerboseCmd = new G4UIcmdWithAnInteger("/mcTracking/newVerbose", this);
  fNewVerboseCmd->SetGuidance("Set new verbose level (/tracking/verbose)");
  fNewVerboseCmd->SetGuidance("when a track with specified track ID ");
  fNewVerboseCmd->SetGuidance(
    "(/mcTracking/newVerboseTrack)\n starts tracking");
  fNewVerboseCmd->SetParameterName("NewVerboseLevel", false);
  fNewVerboseCmd->SetRange("NewVerboseLevel >= 0 && NewVerboseLevel <= 5");
  fNewVerboseCmd->AvailableForStates(
    G4State_PreInit, G4State_Init, G4State_Idle);

  fNewVerboseTrackCmd =
    new G4UIcmdWithAnInteger("/mcTracking/newVerboseTrack", this);
  fNewVerboseTrackCmd->SetGuidance(
    "Set the track ID for which the new verbose level");
  fNewVerboseTrackCmd->SetGuidance("(/mcTracking/newVerbose) will be applied.");
  fNewVerboseTrackCmd->SetParameterName("NewVerboseLevelTrackID", false);
  fNewVerboseTrackCmd->SetRange("NewVerboseLevelTrackID >= 0");
  fNewVerboseTrackCmd->AvailableForStates(
    G4State_PreInit, G4State_Init, G4State_Idle);

  fSaveSecondariesCmd =
    new G4UIcmdWithAString("/mcTracking/saveSecondaries", this);
  fSaveSecondariesCmd->SetGuidance(
    "Option for saving secondaries in the stack in tracking:");
  fSaveSecondariesCmd->SetGuidance("(Default is saving in pre-track.)");
  fSaveSecondariesCmd->SetParameterName("SaveSecondaries", false);
  fSaveSecondariesCmd->SetCandidates("DoNotSave SaveInPreTrack SaveInStep");
  fSaveSecondariesCmd->AvailableForStates(
    G4State_PreInit, G4State_Init, G4State_Idle);

  fSaveDynamicChargeCmd =
    new G4UIcmdWithABool("/mcTracking/saveDynamicCharge", this);
  fSaveDynamicChargeCmd->SetGuidance(
    "Option for saving dynamic charge of secondary particles (as TParticle "
    "status).");
  fSaveDynamicChargeCmd->SetGuidance(
    "(The dynamic charge is not saved by default.)");
  fSaveDynamicChargeCmd->SetParameterName("SaveDynamicCharge", false);
  fSaveDynamicChargeCmd->AvailableForStates(
    G4State_PreInit, G4State_Init, G4State_Idle);
}

//_____________________________________________________________________________
TG4TrackingActionMessenger::~TG4TrackingActionMessenger()
{
  /// Destructor

  delete fTrackingDirectory;
  delete fNewVerboseCmd;
  delete fNewVerboseTrackCmd;
  delete fSaveSecondariesCmd;
  delete fSaveDynamicChargeCmd;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4TrackingActionMessenger::SetNewValue(
  G4UIcommand* command, G4String newValue)
{
  /// Apply command to the associated object.

  if (command == fNewVerboseCmd) {
    fTrackingAction->SetNewVerboseLevel(
      fNewVerboseCmd->GetNewIntValue(newValue));
  }
  else if (command == fNewVerboseTrackCmd) {
    fTrackingAction->SetNewVerboseTrackID(
      fNewVerboseTrackCmd->GetNewIntValue(newValue));
  }
  else if (command == fSaveSecondariesCmd) {
    if (newValue == "DoNotSave")
      TG4TrackManager::Instance()->SetTrackSaveControl(kDoNotSave);
    else if (newValue == "SaveInPreTrack")
      TG4TrackManager::Instance()->SetTrackSaveControl(kSaveInPreTrack);
    else if (newValue == "SaveInStep")
      TG4TrackManager::Instance()->SetTrackSaveControl(kSaveInStep);
  }
  else if (command == fSaveDynamicChargeCmd) {
    TG4TrackManager::Instance()->SetSaveDynamicCharge(
      fSaveDynamicChargeCmd->GetNewBoolValue(newValue));
  }
}
