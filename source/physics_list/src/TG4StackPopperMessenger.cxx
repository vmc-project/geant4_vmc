// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4StackPopperMessenger.cxx
/// \brief Implementation of the TG4StackPopperMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay

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
/// Standard constructor

  fSetSelectionCmd 
    = new G4UIcmdWithAString("/mcPhysics/setStackPopperSelection", this);  
  fSetSelectionCmd->SetGuidance("Selects particles for stack popper process");
  fSetSelectionCmd->SetParameterName("StackPopperSelection", false);
  fSetSelectionCmd->AvailableForStates(G4State_PreInit);  
}

//______________________________________________________________________________
TG4StackPopperMessenger::~TG4StackPopperMessenger() 
{
/// Destructor

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
