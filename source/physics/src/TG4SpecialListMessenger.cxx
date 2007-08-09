// $Id: TG4SpecialListMessenger.cxx,v 1.8 2007/05/31 10:24:33 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics
// Class TG4SpecialListMessenger
// --------------------------------
// See the class description in the header file.

#include "TG4SpecialListMessenger.h"
#include "TG4SpecialPhysicsList.h"
#include "TG4Globals.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithAString.hh>

//______________________________________________________________________________
TG4SpecialListMessenger::TG4SpecialListMessenger(
                                   TG4SpecialPhysicsList* physicsList)
  : G4UImessenger(),
    fPhysicsList(physicsList),
    fSetStackPopperSelectionCmd(0)
{ 
//
  fSetStackPopperSelectionCmd 
    = new G4UIcmdWithAString("/mcPhysics/setStackPopperSelection",this);  
  fSetStackPopperSelectionCmd->SetGuidance("Selects particles for stack popper process");
  fSetStackPopperSelectionCmd->SetParameterName("StackPopperSelection",false);
  fSetStackPopperSelectionCmd->AvailableForStates(G4State_PreInit);  
}

//______________________________________________________________________________
TG4SpecialListMessenger::~TG4SpecialListMessenger() {
//
  delete fSetStackPopperSelectionCmd;
}

//
// public methods
//

//______________________________________________________________________________
void TG4SpecialListMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{ 
/// Apply command to the associated object.
  
  if (command == fSetStackPopperSelectionCmd) {
    G4cout << "TG4SpecialListMessenger::SetNewValue " << newValue << G4endl;
    fPhysicsList->SetStackPopperSelection(newValue);
  }  
}
