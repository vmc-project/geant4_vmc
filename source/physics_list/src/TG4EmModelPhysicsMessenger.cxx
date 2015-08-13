//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4EmModelPhysicsMessenger.cxx
/// \brief Implementation of the TG4EmModelPhysicsMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4EmModelPhysicsMessenger.h"
#include "TG4EmModelPhysics.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>

//______________________________________________________________________________
TG4EmModelPhysicsMessenger::TG4EmModelPhysicsMessenger(
                                      TG4EmModelPhysics* emModelPhysics)
  : G4UImessenger(),
    fEmModelPhysics(emModelPhysics),
    fSelectedEmModel(),
    fDirectory(0),
    fSetEmModelCmd(0),
    fSetParticlesCmd(0),
    fSetRegionsCmd(0)
{ 
/// Standard constructor

  fDirectory = new G4UIdirectory("/mcPhysics/emModel/");
  fDirectory->SetGuidance("EM model physics commands.");

  G4String availableModels;
  for ( G4int i=kPAIModel; i<kNoEmModel; i++ ) {
    availableModels += TG4EmModelPhysics::GetEmModelName(i);
    availableModels += "  ";
  }  

  fSetEmModelCmd
    = new G4UIcmdWithAString("/mcPhysics/emModel/setEmModel", this);
  fSetEmModelCmd->SetGuidance("Define an extra EM model");
  fSetEmModelCmd->SetParameterName("EmModel", false);
  fSetEmModelCmd->SetCandidates(availableModels);
  fSetEmModelCmd->AvailableForStates(G4State_PreInit);

  fSetParticlesCmd
    = new G4UIcmdWithAString("/mcPhysics/emModel/setParticles", this);
  fSetParticlesCmd
    ->SetGuidance("Set particles for the selected extra EM model");
  fSetParticlesCmd
    ->SetGuidance("(all = select all particles with ionosation process activated.)");
  fSetParticlesCmd->SetParameterName("Particles", false);
  fSetParticlesCmd->AvailableForStates(G4State_PreInit);

  fSetRegionsCmd
    = new G4UIcmdWithAString("/mcPhysics/emModel/setRegions", this);
  fSetRegionsCmd
    ->SetGuidance("Set tracking media names (regions) for the selected extra model");
  fSetRegionsCmd
    ->SetGuidance("("" = the model will be applied to the default world region.");
  fSetRegionsCmd->SetParameterName("Regions", false);
  fSetParticlesCmd->AvailableForStates(G4State_PreInit);
}

//______________________________________________________________________________
TG4EmModelPhysicsMessenger::~TG4EmModelPhysicsMessenger() 
{
/// Destructor

  delete fDirectory;
  delete fSetEmModelCmd;
  delete fSetParticlesCmd;
  delete fSetRegionsCmd;
}

//
// public methods
//

//______________________________________________________________________________
void TG4EmModelPhysicsMessenger::SetNewValue(G4UIcommand* command,
                                             G4String newValue)
{ 
/// Apply command to the associated object.
  
  if (command == fSetEmModelCmd) {
    fSelectedEmModel = newValue;
    fEmModelPhysics
      ->SetEmModel(fSelectedEmModel);
  }  
  else if (command == fSetParticlesCmd) {
    fEmModelPhysics
      ->SetEmModelParticles(fSelectedEmModel, newValue);
  }  
  else if (command == fSetRegionsCmd) {
    fEmModelPhysics
      ->SetEmModelRegions(fSelectedEmModel, newValue);
  }  
}
