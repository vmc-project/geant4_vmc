// $Id: TG4PhysicsMessenger.cxx,v 1.2 2002/12/18 09:35:32 brun Exp $
// Category: physics
//
// Author: I. Hrivnacova
//
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
  : fPhysicsManager(physicsManager)
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
TG4PhysicsMessenger::TG4PhysicsMessenger(){
//
} 

//_____________________________________________________________________________
TG4PhysicsMessenger::TG4PhysicsMessenger(const TG4PhysicsMessenger& right) {
// 
  TG4Globals::Exception("TG4PhysicsMessenger is protected from copying.");
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

// operators

//_____________________________________________________________________________
TG4PhysicsMessenger& TG4PhysicsMessenger::operator=(const TG4PhysicsMessenger& right)
{
  // check assignement to self
  if (this == &right) return *this;

  TG4Globals::Exception("TG4PhysicsMessenger is protected from assigning.");
    
  return *this;  
}    
          
// public methods

//_____________________________________________________________________________
void TG4PhysicsMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
// Applies command to the associated object.
// ---

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
