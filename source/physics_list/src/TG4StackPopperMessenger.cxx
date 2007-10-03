// $Id: TG4StackPopperMessenger.cxx,v 1.1 2007/08/09 13:43:34 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics_lists
// Class TG4StackPopperMessenger
// ------------------------------------
// See the class description in the header file.

#include "TG4StackPopperMessenger.h"
#include "TG4StackPopperPhysics.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>

//______________________________________________________________________________
TG4StackPopperMessenger::TG4StackPopperMessenger(
                            TG4StackPopperPhysics* stackPopperPhysics)
  : G4UImessenger(),
    fStackPopperPhysics(stackPopperPhysics),
    fSetSelectionCmd(0)
{ 
//
  fSetSelectionCmd 
    = new G4UIcmdWithAString("/mcPhysics/setStackPopperSelection", this);  
  fSetSelectionCmd->SetGuidance("Selects particles for stack popper process");
  fSetSelectionCmd->SetParameterName("StackPopperSelection", false);
  fSetSelectionCmd->AvailableForStates(G4State_PreInit);  
}

//______________________________________________________________________________
TG4StackPopperMessenger::~TG4StackPopperMessenger() {
//
  delete fSetSelectionCmd;
}

//
// public methods
//

//______________________________________________________________________________
void TG4StackPopperMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{ 
/// Apply command to the associated object.
  
  if ( command == fSetSelectionCmd ) {
    G4cout << "TG4StackPopperMessenger::SetNewValue " << newValue << G4endl;
    fStackPopperPhysics->SetSelection(newValue);
  }  
}
