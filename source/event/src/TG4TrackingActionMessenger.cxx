// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4TrackingActionMessenger.cxx
/// \brief Implementation of the TG4TrackingActionMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay
 
#include "TG4TrackingActionMessenger.h"
#include "TG4TrackingAction.h"
#include "TG4Globals.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithABool.hh>

//_____________________________________________________________________________
TG4TrackingActionMessenger::TG4TrackingActionMessenger(
                               TG4TrackingAction* trackingAction)
  : G4UImessenger(),
    fTrackingAction(trackingAction),
    fTrackingDirectory(0),
    fNewVerboseCmd(0),
    fNewVerboseTrackCmd(0),
    fSaveSecondariesCmd(0)
{
/// Standard constructor

  fTrackingDirectory = new G4UIdirectory("/mcTracking/");
  fTrackingDirectory->SetGuidance("TG4TrackingAction control commands.");

  fNewVerboseCmd = new G4UIcmdWithAnInteger("/mcTracking/newVerbose", this);
  fNewVerboseCmd->SetGuidance("Set new verbose level (/tracking/verbose)");
  fNewVerboseCmd->SetGuidance("when a track with specified track ID ");
  fNewVerboseCmd->SetGuidance("(/TG4Tracking/newVerboseTrack)\n starts tracking");
  fNewVerboseCmd->SetParameterName("NewVerboseLevel", false);
  fNewVerboseCmd->SetRange("NewVerboseLevel >= 0 && NewVerboseLevel <= 5");
  fNewVerboseCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fNewVerboseTrackCmd = new G4UIcmdWithAnInteger("/mcTracking/newVerboseTrack", this);
  fNewVerboseTrackCmd->SetGuidance("Set the track ID for which the new verbose level");
  fNewVerboseTrackCmd->SetGuidance("(/TG4Tracking/newVerbose) will be applied.");
  fNewVerboseTrackCmd->SetParameterName("NewVerboseLevelTrackID", false);
  fNewVerboseTrackCmd->SetRange("NewVerboseLevelTrackID >= 0");
  fNewVerboseTrackCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fSaveSecondariesCmd = new G4UIcmdWithABool("/mcTracking/saveSecondaries", this);
  fSaveSecondariesCmd->SetGuidance("Option to save secondaries in the stack in tracking.");
  fSaveSecondariesCmd->SetGuidance("By default this option is true.");
  fSaveSecondariesCmd->SetParameterName("SaveSecondaries", false);
  fSaveSecondariesCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
}

//_____________________________________________________________________________
TG4TrackingActionMessenger::~TG4TrackingActionMessenger() 
{
/// Destructor

  delete fTrackingDirectory;
  delete fNewVerboseCmd;
  delete fNewVerboseTrackCmd;
  delete fSaveSecondariesCmd;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4TrackingActionMessenger::SetNewValue(G4UIcommand* command, 
       G4String newValue)
{ 
/// Apply command to the associated object.

  if(command == fNewVerboseCmd) { 
    fTrackingAction
      ->SetNewVerboseLevel(fNewVerboseCmd->GetNewIntValue(newValue)); 
  }   
  else if(command == fNewVerboseTrackCmd) { 
    fTrackingAction
      ->SetNewVerboseTrackID(fNewVerboseTrackCmd->GetNewIntValue(newValue)); 
  }   
  else if(command == fSaveSecondariesCmd) { 
    fTrackingAction
      ->SetSaveSecondaries(fSaveSecondariesCmd->GetNewBoolValue(newValue)); 
  }   
}
