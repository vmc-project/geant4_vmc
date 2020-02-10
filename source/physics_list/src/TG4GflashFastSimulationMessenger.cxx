//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4GflashFastSimulationMessenger.cxx
/// \brief Implementation of the TG4GflashFastSimulationMessenger class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4GflashFastSimulationMessenger.h"
#include "TG4GflashFastSimulation.h"

#include <G4UIcmdWithAString.hh>
#include <G4UIdirectory.hh>

//______________________________________________________________________________
TG4GflashFastSimulationMessenger::TG4GflashFastSimulationMessenger(
  TG4GflashFastSimulation* gflashFastSimulation)
  : G4UImessenger(),
    fGflashFastSimulation(gflashFastSimulation),
    fSetGflashMaterialCmd(0)
{
  /// Standard constructor

  fSetGflashMaterialCmd =
    new G4UIcmdWithAString("/mcPhysics/setGflashMaterial", this);
  fSetGflashMaterialCmd->SetGuidance(
    "Set  material for shower parameterisation");
  fSetGflashMaterialCmd->SetParameterName("ExtDecayerSelection", false);
  fSetGflashMaterialCmd->AvailableForStates(G4State_PreInit);
}

//______________________________________________________________________________
TG4GflashFastSimulationMessenger::~TG4GflashFastSimulationMessenger()
{
  /// Destructor

  delete fSetGflashMaterialCmd;
}

//
// public methods
//

//______________________________________________________________________________
void TG4GflashFastSimulationMessenger::SetNewValue(
  G4UIcommand* command, G4String newValue)
{
  /// Apply command to the associated object.

  if (command == fSetGflashMaterialCmd) {
    fGflashFastSimulation->SetMaterialName(newValue);
  }
}
