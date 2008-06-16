// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4ExtDecayerMessenger.cxx
/// \brief Implementation of the TG4ExtDecayerMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ExtDecayerMessenger.h"
#include "TG4ExtDecayerPhysics.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>

//______________________________________________________________________________
TG4ExtDecayerMessenger::TG4ExtDecayerMessenger(
                            TG4ExtDecayerPhysics* extDecayerPhysics)
  : G4UImessenger(),
    fExtDecayerPhysics(extDecayerPhysics),
    fSetSelectionCmd(0)
{ 
/// Standard constructor

  fSetSelectionCmd 
    = new G4UIcmdWithAString("/mcPhysics/setExtDecayerSelection", this);  
  G4String guidance 
    = "Select particles for which the external decayer will ";
  guidance = guidance + "have priority over using Geant4 decay table";   
  fSetSelectionCmd->SetGuidance(guidance);
  fSetSelectionCmd->SetParameterName("ExtDecayerSelection", false);
  fSetSelectionCmd->AvailableForStates(G4State_PreInit);  
}

//______________________________________________________________________________
TG4ExtDecayerMessenger::~TG4ExtDecayerMessenger() 
{
/// Destructor

  delete fSetSelectionCmd;
}

//
// public methods
//

//______________________________________________________________________________
void TG4ExtDecayerMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{ 
/// Apply command to the associated object.
  
  if ( command == fSetSelectionCmd ) {
    G4cout << "TG4ExtDecayerMessenger::SetNewValue " << newValue << G4endl;
    fExtDecayerPhysics->SetSelection(newValue);
  }  
}
