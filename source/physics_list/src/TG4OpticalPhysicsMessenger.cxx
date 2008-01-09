// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4OpticalPhysicsMessenger.cxx
/// \brief Implementation of the TG4OpticalPhysicsMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4OpticalPhysicsMessenger.h"
#include "TG4OpticalPhysics.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAnInteger.hh>

//______________________________________________________________________________
TG4OpticalPhysicsMessenger::TG4OpticalPhysicsMessenger(
                                      TG4OpticalPhysics* opticalPhysics)
  : G4UImessenger(),
    fOpticalPhysics(opticalPhysics),
    fSetCerenkovMaxPhotonsCmd(0)
{ 
/// Standard constructor

  fSetCerenkovMaxPhotonsCmd 
    = new G4UIcmdWithAnInteger("/mcPhysics/setCerenkovMaxPhotons", this);  
  fSetCerenkovMaxPhotonsCmd->SetGuidance("Set maximum number of photons per step");
  fSetCerenkovMaxPhotonsCmd->SetParameterName("CerenkovMaxPhotons", false);
  fSetCerenkovMaxPhotonsCmd->SetRange("CerenkovMaxPhotons>=0");
  fSetCerenkovMaxPhotonsCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  
}

//______________________________________________________________________________
TG4OpticalPhysicsMessenger::~TG4OpticalPhysicsMessenger() 
{
/// Destructor

  delete fSetCerenkovMaxPhotonsCmd;
}

//
// public methods
//

//______________________________________________________________________________
void TG4OpticalPhysicsMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{ 
/// Apply command to the associated object.
  
  if (command == fSetCerenkovMaxPhotonsCmd) {
    fOpticalPhysics
      ->SetMaxNumPhotonsPerStep(
          fSetCerenkovMaxPhotonsCmd->GetNewIntValue(newValue));
  }  
}
