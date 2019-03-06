//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ParticlesCheckerMessenger.cxx
/// \brief Implementation of the TG4ParticlesCheckerMessenger class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ParticlesCheckerMessenger.h"
#include "TG4ParticlesChecker.h"
#include "TG4Globals.h"
#include "TG4UICmdWithAComplexString.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADouble.hh>

//_____________________________________________________________________________
TG4ParticlesCheckerMessenger::TG4ParticlesCheckerMessenger(
                                 TG4ParticlesChecker* particlesChecker)
  : G4UImessenger(),
    fParticlesChecker(particlesChecker),
    fDirectory(0),
    fSelectedProperty(""),
    fCheckParticlesCmd(0),
    fCheckParticleCmd(0),
    fSelectPropertyCmd(0),
    fSetCheckingCmd(0),
    fSetPrecisionCmd(0)
{
/// Standard constructor

  fDirectory = new G4UIdirectory("/mcParticlesChecker/");
  fDirectory->SetGuidance("Particles checker commands.");

  fCheckParticlesCmd = new G4UIcmdWithoutParameter("/mcParticlesChecker/checkParticles", this);
  fCheckParticlesCmd->SetGuidance("Check properties for all instantiated particles");
  fCheckParticlesCmd->AvailableForStates(G4State_Idle);

  fCheckParticleCmd = new G4UIcmdWithAnInteger("/mcParticlesChecker/checkParticle", this);
  fCheckParticleCmd->SetGuidance("Check properties for the particle with given PDG encoding");
  fCheckParticleCmd->SetParameterName("PDGEncoding", false);
  fCheckParticleCmd->AvailableForStates(G4State_Idle);

  fSelectPropertyCmd = new G4UIcmdWithAString("/mcParticlesChecker/selectProperty", this);
  fSelectPropertyCmd->SetParameterName("propertyName", true);
  fSelectPropertyCmd->AvailableForStates(G4State_Idle);

  fSetCheckingCmd = new G4UIcmdWithABool("/mcParticlesChecker/setChecking", this);
  fSetCheckingCmd->SetGuidance("(In)Activate checking for a selected property");
  fSetCheckingCmd->SetParameterName("Checking", false);
  fSetCheckingCmd->AvailableForStates(G4State_Idle);

  fSetPrecisionCmd = new G4UIcmdWithADouble("/mcParticlesChecker/setPrecision", this);
  fSetPrecisionCmd->SetGuidance("Set number of bins in P");
  fSetPrecisionCmd->SetParameterName("nofBinsE", false);
  fSetPrecisionCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
}

//_____________________________________________________________________________
TG4ParticlesCheckerMessenger::~TG4ParticlesCheckerMessenger()
{
/// Destructor

  delete fDirectory;
  delete fCheckParticlesCmd;
  delete fCheckParticleCmd;
  delete fSelectPropertyCmd;
  delete fSetCheckingCmd;
  delete fSetPrecisionCmd;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4ParticlesCheckerMessenger::Init()
{
/// Initialize the object.
/// We have to to this separately from the constructor as we need to get properties
/// from the particle checker, which are set after pnly after construction of its
/// messenger.

  G4String candidates;
  const std::set<TG4ParticlesChecker::ParticleProperty>& availableProperties
    = fParticlesChecker->GetAvailableProperties();
  std::set<TG4ParticlesChecker::ParticleProperty>::const_iterator it;
  for ( it = availableProperties.begin(); it != availableProperties.end(); it++ ) {
    candidates = candidates + TG4ParticlesChecker::GetParticlePropertyName(*it);
    candidates = candidates + G4String(" ");
  }
  fSelectPropertyCmd->SetCandidates(candidates);

  G4String guidance = "Select particle property to be checked.\n";
  guidance = guidance + "Available: ";
  guidance = guidance + candidates;
  fSelectPropertyCmd->SetGuidance(guidance);
}

//_____________________________________________________________________________
void TG4ParticlesCheckerMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
/// Apply command to the associated object.

  if ( command == fCheckParticlesCmd ) {
    fParticlesChecker->CheckParticles();
  }
  else if ( command == fCheckParticleCmd ) {
    fParticlesChecker->CheckParticle(fCheckParticleCmd->GetNewIntValue(newValue));
  }
  else if (command == fSelectPropertyCmd) {
    fSelectedProperty = newValue;
  }
  else if (command == fSetCheckingCmd) {
    if ( fSelectedProperty == "" ) {
      TString text = "The particle property has to be selected first.\n";
      text = text + "The command will have no effect.";
      TG4Globals::Warning("TG4ParticlesCheckerMessenger", "SetNewValue",
                          "The particle property has not been yet seleceted.");
      return;
    }
    fParticlesChecker
      ->SetChecking(TG4ParticlesChecker::GetParticleProperty(fSelectedProperty),
                    fSetCheckingCmd->GetNewBoolValue(newValue));
  }
  else if (command == fSetPrecisionCmd) {
    fParticlesChecker->SetPrecision(fSetPrecisionCmd->GetNewDoubleValue(newValue));
  }
}
