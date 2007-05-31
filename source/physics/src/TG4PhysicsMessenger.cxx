// $Id: TG4PhysicsMessenger.cxx,v 1.5 2006/12/12 16:21:16 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics
// Class TG4PhysicsMessenger
// -------------------------
// See the class description in the header file.

#include "TG4PhysicsMessenger.h"
#include "TG4GeometryServices.h"
#include "TG4G3PhysicsManager.h"
#include "TG4ProcessControlMap.h"
#include "TG4ProcessMCMap.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAString.hh>

//_____________________________________________________________________________
TG4PhysicsMessenger::TG4PhysicsMessenger(TG4PhysicsManager* physicsManager)
  : G4UImessenger(),
    fPhysicsManager(physicsManager),
    fDirectory(0),         
    fPrintProcessMCMapCmd(0),
    fPrintProcessControlMapCmd(0),
    fPrintVolumeLimitsCmd(0),
    fPrintGeneralCutsCmd(0),
    fPrintGeneralControlsCmd(0)
{ 
//
  fDirectory = new G4UIdirectory("/mcPhysics/");
  fDirectory->SetGuidance("TGeant4 physics control commands.");

  fPrintProcessMCMapCmd
     = new G4UIcmdWithoutParameter("/mcPhysics/printProcessMCMap", this);
  fPrintProcessMCMapCmd
    ->SetGuidance("Prints mapping of G4 processes to G3 controls.");
  fPrintProcessMCMapCmd->AvailableForStates(G4State_Idle);

  fPrintProcessControlMapCmd
     = new G4UIcmdWithoutParameter("/mcPhysics/printProcessControlMap", this);
  fPrintProcessControlMapCmd
    ->SetGuidance("Prints mapping of G4 processes to G3 controls.");
  fPrintProcessControlMapCmd->AvailableForStates(G4State_Idle);

  fPrintVolumeLimitsCmd
     = new G4UIcmdWithAString("/mcPhysics/printVolumeLimits", this);
  fPrintVolumeLimitsCmd
    ->SetGuidance("Prints the limits set to the specified volume.");
  fPrintVolumeLimitsCmd->SetParameterName("PrintVolumeLimits", false);
  fPrintVolumeLimitsCmd->AvailableForStates(G4State_Idle);

  fPrintGeneralCutsCmd
     = new G4UIcmdWithoutParameter("/mcPhysics/printGeneralCuts", this);
  fPrintGeneralCutsCmd
    ->SetGuidance("Prints the general G3 cuts.");
  fPrintGeneralCutsCmd->AvailableForStates(G4State_Idle);

  fPrintGeneralControlsCmd
     = new G4UIcmdWithoutParameter("/mcPhysics/printGeneralControls", this);
  fPrintGeneralControlsCmd
    ->SetGuidance("Prints the general G3 process controls.");
  fPrintGeneralControlsCmd->AvailableForStates(G4State_Idle);
}

//_____________________________________________________________________________
TG4PhysicsMessenger::~TG4PhysicsMessenger() {
//

  delete fDirectory;
  delete fPrintProcessMCMapCmd;
  delete fPrintProcessControlMapCmd;
  delete fPrintVolumeLimitsCmd;
  delete fPrintGeneralCutsCmd;
  delete fPrintGeneralControlsCmd;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4PhysicsMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
/// Apply command to the associated object.

  if (command == fPrintProcessMCMapCmd) {
    TG4ProcessMCMap::Instance()->PrintAll();
  }  
  else if (command == fPrintProcessControlMapCmd) {
    TG4ProcessControlMap::Instance()->PrintAll();
  }  
  else if (command == fPrintVolumeLimitsCmd) {
    TG4GeometryServices::Instance()->PrintVolumeLimits(newValue);
  }  
  else if (command == fPrintGeneralCutsCmd) {
    TG4G3PhysicsManager::Instance()->GetCutVector()->Print();
  }  
  else if (command == fPrintGeneralControlsCmd) {
    TG4G3PhysicsManager::Instance()->GetControlVector()->Print();
  }  
}
