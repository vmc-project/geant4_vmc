// $Id: TG4PhysicsListMessenger.cxx,v 1.2 2002/12/18 09:35:32 brun Exp $
// Category: physics
//
// Author: I. Hrivnacova
//
// Class TG4PhysicsListMessenger
// --------------------------------
// See the class description in the header file.

#include "TG4PhysicsListMessenger.h"
#include "TG4ModularPhysicsList.h"
#include "TG4Globals.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithADouble.hh>

//______________________________________________________________________________
TG4PhysicsListMessenger::TG4PhysicsListMessenger(
                                   TG4ModularPhysicsList* physicsList)
  : fPhysicsList(physicsList)
{ 
//
  fSetEMCmd
     = new G4UIcmdWithABool("/mcPhysics/setEM", this);
  fSetEMCmd->SetGuidance("Set electromagnetic physics.");
  fSetEMCmd->SetParameterName("EMControl", false);
  fSetEMCmd->AvailableForStates(G4State_PreInit);

  fSetMuonCmd
     = new G4UIcmdWithABool("/mcPhysics/setMuon", this);
  fSetMuonCmd->SetGuidance("Set muon physics.");
  fSetMuonCmd->SetParameterName("EMControl", false);
  fSetMuonCmd->AvailableForStates(G4State_PreInit);

  fSetHadronCmd
     = new G4UIcmdWithABool("/mcPhysics/setHadron", this);
  fSetHadronCmd->SetGuidance("Set hadron physics.");
  fSetHadronCmd->SetParameterName("HadronControl", false);
  fSetHadronCmd->AvailableForStates(G4State_PreInit);

  fSetOpticalCmd
     = new G4UIcmdWithABool("/mcPhysics/setOptical", this);
  fSetOpticalCmd->SetGuidance("Set Cerenkov and optical physics.");
  fSetOpticalCmd->SetParameterName("OpticalControl", false);
  fSetOpticalCmd->AvailableForStates(G4State_PreInit);

  fSetSpecialCutsCmd
     = new G4UIcmdWithABool("/mcPhysics/setSpecialCuts", this);
  fSetSpecialCutsCmd->SetGuidance("Set special cuts process.");
  fSetSpecialCutsCmd
    ->SetGuidance("!! Support for this option is under development.");
  fSetSpecialCutsCmd->SetParameterName("SpecialCutsControl", false);
  fSetSpecialCutsCmd->AvailableForStates(G4State_PreInit);

  fSetSpecialControlsCmd
     = new G4UIcmdWithABool("/mcPhysics/setSpecialControls", this);
  fSetSpecialControlsCmd->SetGuidance("Set special controls process.");
  fSetSpecialControlsCmd
    ->SetGuidance("!! Support for this option is under development.");
  fSetSpecialControlsCmd->SetParameterName("SpecialFlagsControl", false);
  fSetSpecialControlsCmd->AvailableForStates(G4State_PreInit);

  fRangeCutCmd
     = new G4UIcmdWithADouble("/mcPhysics/rangeCut", this);
  fRangeCutCmd->SetGuidance("Sets the global cut in range (in mm)");
  fRangeCutCmd->AvailableForStates(G4State_PreInit);
}

//______________________________________________________________________________
TG4PhysicsListMessenger::TG4PhysicsListMessenger(){
//
} 

//______________________________________________________________________________
TG4PhysicsListMessenger::TG4PhysicsListMessenger(
                                   const TG4PhysicsListMessenger& right) {
// 
  TG4Globals::Exception("TG4PhysicsListMessenger is protected from copying.");
}

//______________________________________________________________________________
TG4PhysicsListMessenger::~TG4PhysicsListMessenger() {
//

  delete fSetEMCmd;
  delete fSetMuonCmd;
  delete fSetHadronCmd;
  delete fSetOpticalCmd;
  delete fSetSpecialCutsCmd;
  delete fSetSpecialControlsCmd;
  delete fRangeCutCmd;
}

// operators

//______________________________________________________________________________
TG4PhysicsListMessenger& 
TG4PhysicsListMessenger::operator=(const TG4PhysicsListMessenger& right)
{
  // check assignement to self
  if (this == &right) return *this;

  TG4Globals::Exception("TG4PhysicsListMessenger is protected from assigning.");
    
  return *this;  
}    
          
// public methods

//______________________________________________________________________________
void TG4PhysicsListMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{ 
// Applies command to the associated object.
// ---

  if (command == fSetEMCmd) {
    fPhysicsList
      ->SetEMPhysics(fSetEMCmd->GetNewBoolValue(newValue)); 
  }    
  if (command == fSetMuonCmd) {
    fPhysicsList
      ->SetMuonPhysics(fSetMuonCmd->GetNewBoolValue(newValue)); 
  }    
  else if (command == fSetHadronCmd) {
    fPhysicsList
      ->SetHadronPhysics(fSetHadronCmd->GetNewBoolValue(newValue)); 
  }    
  else if (command == fSetOpticalCmd) {
    fPhysicsList
      ->SetOpticalPhysics(fSetOpticalCmd->GetNewBoolValue(newValue)); 
  }    
  else if (command == fSetSpecialCutsCmd) {
    fPhysicsList
      ->SetSpecialCutsPhysics(fSetSpecialCutsCmd->GetNewBoolValue(newValue)); 
  }    
  else if (command == fSetSpecialControlsCmd) {
    fPhysicsList
      ->SetSpecialControlsPhysics(
          fSetSpecialControlsCmd->GetNewBoolValue(newValue)); 
  }    
  else if (command == fRangeCutCmd) {
    fPhysicsList
        ->SetRangeCut(fRangeCutCmd->GetNewDoubleValue(newValue));
  }  
}
