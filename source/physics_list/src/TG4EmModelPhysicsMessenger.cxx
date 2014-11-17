//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
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
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithAString.hh>

//______________________________________________________________________________
TG4EmModelPhysicsMessenger::TG4EmModelPhysicsMessenger(
                                      TG4EmModelPhysics* emModelPhysics)
  : G4UImessenger(),
    fEmModelPhysics(emModelPhysics),
    fSelectedMediumId(-1),
    fDirectory(0),
    fSelectMediumCmd(0),                                              
    fSetElossModelCmd(0),                                              
    fSetFluctModelCmd(0),                                              
    fSetParticlesCmd(0)                                             
{ 
/// Standard constructor

  fDirectory = new G4UIdirectory("/mcPhysics/emModel/");
  fDirectory->SetGuidance("EM model physics commands.");

  fSelectMediumCmd 
    = new G4UIcmdWithAnInteger("/mcPhysics/emModel/selectMedium", this);  
  fSelectMediumCmd->SetGuidance("Select tracking medium with an extra EM model");
  fSelectMediumCmd->SetParameterName("MediumId", false);
  fSelectMediumCmd->SetRange("MediumId>=0");
  fSelectMediumCmd->AvailableForStates(G4State_PreInit);  

  G4String availableModels;
  for ( G4int i=kPAIModel; i<kNoEmModel; i++ ) {
    availableModels += TG4EmModelPhysics::GetEmModelName(i);
    availableModels += "  ";
  }  

  fSetElossModelCmd 
    = new G4UIcmdWithAString("/mcPhysics/emModel/setElossModel", this);  
  fSetElossModelCmd
    ->SetGuidance("Set the given energy loss model to selected tracking medium");
  fSetElossModelCmd->SetParameterName("ElossModel", false);
  fSetElossModelCmd->SetCandidates(availableModels);
  fSetElossModelCmd->AvailableForStates(G4State_PreInit);  

  fSetFluctModelCmd 
    = new G4UIcmdWithAString("/mcPhysics/emModel/setFluctModel", this);  
  fSetFluctModelCmd
    ->SetGuidance("Set the given energy fluctuations model to selected tracking medium");
  fSetFluctModelCmd->SetParameterName("FluctModel", false);
  fSetFluctModelCmd->SetCandidates(availableModels);
  fSetFluctModelCmd->AvailableForStates(G4State_PreInit);  

  fSetParticlesCmd 
    = new G4UIcmdWithAString("/mcPhysics/emModel/setParticles", this);  
  fSetParticlesCmd
    ->SetGuidance("Set particles for the extra model to selected tracking medium");
  fSetParticlesCmd
    ->SetGuidance("(all = select all particles with ionosation process activated.)");
  fSetParticlesCmd->SetParameterName("Particles", false);
  fSetParticlesCmd->AvailableForStates(G4State_PreInit);  
}

//______________________________________________________________________________
TG4EmModelPhysicsMessenger::~TG4EmModelPhysicsMessenger() 
{
/// Destructor

  delete fDirectory;
  delete fSelectMediumCmd;
  delete fSetElossModelCmd;
  delete fSetFluctModelCmd;
  delete fSetParticlesCmd;
}

//
// public methods
//

//______________________________________________________________________________
void TG4EmModelPhysicsMessenger::SetNewValue(G4UIcommand* command,
                                             G4String newValue)
{ 
/// Apply command to the associated object.
  
  if (command == fSelectMediumCmd) {
    fSelectedMediumId = fSelectMediumCmd->GetNewIntValue(newValue);
  }  
  else if (command == fSetElossModelCmd) {
    fEmModelPhysics
      ->SetEmElossModel(fSelectedMediumId, newValue);
  }  
  else if (command == fSetFluctModelCmd) {
    fEmModelPhysics
      ->SetEmFluctModel(fSelectedMediumId, newValue);
  }  
  else if (command == fSetParticlesCmd) {
    fEmModelPhysics
      ->SetEmModelParticles(fSelectedMediumId, newValue);
  }  
}
