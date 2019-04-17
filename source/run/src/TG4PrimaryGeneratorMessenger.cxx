//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2019 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4PrimaryGeneratorMessenger.cxx
/// \brief Implementation of the TG4PrimaryGeneratorMessenger class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4PrimaryGeneratorMessenger.h"
#include "TG4PrimaryGeneratorAction.h"

#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIdirectory.hh>

//_____________________________________________________________________________
TG4PrimaryGeneratorMessenger::TG4PrimaryGeneratorMessenger(
  TG4PrimaryGeneratorAction* action)
  : G4UImessenger(),
    fPrimaryGeneratorAction(action),
    fDirectory(0),
    fSkipUnknownParticlesCmd(0)
{
  /// Standard constructor

  fDirectory = new G4UIdirectory("/mcPrimaryGenerator/");
  fDirectory->SetGuidance("TGeant4 primary generator action commands.");

  fSkipUnknownParticlesCmd =
    new G4UIcmdWithABool("/mcPrimaryGenerator/skipUnknownParticles", this);
  fSkipUnknownParticlesCmd->SetGuidance(
    "Switch on|off applying range cuts for gamma");
  fSkipUnknownParticlesCmd->SetParameterName("ApplyForGamma", false);
  fSkipUnknownParticlesCmd->AvailableForStates(G4State_PreInit, G4State_Init);
}

//_____________________________________________________________________________
TG4PrimaryGeneratorMessenger::~TG4PrimaryGeneratorMessenger()
{
  /// Destructor

  delete fDirectory;
  delete fSkipUnknownParticlesCmd;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4PrimaryGeneratorMessenger::SetNewValue(
  G4UIcommand* command, G4String newValue)
{
  /// Apply command to the associated object.

  if (command == fSkipUnknownParticlesCmd) {
    fPrimaryGeneratorAction->SetSkipUnknownParticles(
      fSkipUnknownParticlesCmd->GetNewBoolValue(newValue));
  }
}
