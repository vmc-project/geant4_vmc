// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
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
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithAString.hh>

//______________________________________________________________________________
TG4OpticalPhysicsMessenger::TG4OpticalPhysicsMessenger(
                                      TG4OpticalPhysics* opticalPhysics)
  : G4UImessenger(),
    fOpticalPhysics(opticalPhysics),
    fSelectedProcess(kNoProcess),
    fSelectOpProcessCmd(0),
    fSetOpProcessActivationCmd(0),
    fSetOpProcessVerboseCmd(0),
    fSetCerenkovMaxPhotonsCmd(0),
    fSetCerenkovMaxBetaChangeCmd(0),
    fSetScintillationYieldFactorCmd(0),
    fSetOpticalSurfaceModelCmd(0),
    fSetTrackSecondariesFirstCmd(0)
{ 
/// Standard constructor

  fSelectOpProcessCmd 
    = new G4UIcmdWithAString("/mcPhysics/selectOpProcess", this);  
  fSelectOpProcessCmd
    ->SetGuidance("Select optical process for applying verbose/activation commands");
  fSelectOpProcessCmd->SetParameterName("OpProcess", false);
  G4String candidates;
  for ( G4int i=0; i<kNoProcess; i++ ) {
    candidates += TG4OpticalProcessName(i);
    candidates += G4String(" ");
  }  
  fSelectOpProcessCmd->SetCandidates(candidates);
  fSelectOpProcessCmd->AvailableForStates(G4State_PreInit, G4State_Idle, G4State_EventProc);  

  fSetOpProcessActivationCmd 
    = new G4UIcmdWithABool("/mcPhysics/setOpProcessActivation", this);  
  fSetOpProcessActivationCmd->SetGuidance("Activate/Inactivate selected optical process");
  fSetOpProcessActivationCmd->SetParameterName("OpProcessActivation", false);
  fSetOpProcessActivationCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  

  fSetOpProcessVerboseCmd 
    = new G4UIcmdWithAnInteger("/mcPhysics/setOpProcessVerbose", this);  
  fSetOpProcessVerboseCmd->SetGuidance("Set verbosity level for selected optical process");
  fSetOpProcessVerboseCmd->SetParameterName("OpProcessVerbose", false);
  fSetOpProcessVerboseCmd->SetRange("OpProcessVerbose>=0");
  fSetOpProcessVerboseCmd->AvailableForStates(G4State_PreInit, G4State_Idle, G4State_EventProc);  

  fSetCerenkovMaxPhotonsCmd 
    = new G4UIcmdWithAnInteger("/mcPhysics/setCerenkovMaxPhotons", this);  
  fSetCerenkovMaxPhotonsCmd->SetGuidance("Set maximum number of photons per step");
  fSetCerenkovMaxPhotonsCmd->SetParameterName("CerenkovMaxPhotons", false);
  fSetCerenkovMaxPhotonsCmd->SetRange("CerenkovMaxPhotons>=0");
  fSetCerenkovMaxPhotonsCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  

  fSetCerenkovMaxBetaChangeCmd 
    = new G4UIcmdWithADouble("/mcPhysics/setCerenkovMaxBetaChange", this);  
  fSetCerenkovMaxBetaChangeCmd
    ->SetGuidance("Set maximum change of beta of parent particle per step");
  fSetCerenkovMaxBetaChangeCmd->SetParameterName("CerenkovMaxBetaChange", false);
  fSetCerenkovMaxBetaChangeCmd->SetRange("CerenkovMaxBetaChange>=0");
  fSetCerenkovMaxBetaChangeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  

  fSetScintillationYieldFactorCmd 
    = new G4UIcmdWithADouble("/mcPhysics/setScintillationYieldFactor", this);  
  fSetScintillationYieldFactorCmd->SetGuidance("Set scintillation yield factor");
  fSetScintillationYieldFactorCmd->SetParameterName("ScintillationYieldFactor", false);
  fSetScintillationYieldFactorCmd->SetRange("ScintillationYieldFactor>=0");
  fSetScintillationYieldFactorCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  

  fSetOpticalSurfaceModelCmd 
    = new G4UIcmdWithAString("/mcPhysics/setOpticalSurfaceModel", this);  
  fSetOpticalSurfaceModelCmd
    ->SetGuidance("Set optical surface model (glisur or unified)");
  fSetOpticalSurfaceModelCmd->SetParameterName("OpticalSurfaceModel", false);
  fSetOpticalSurfaceModelCmd->SetCandidates("glisur unified");
  fSetOpticalSurfaceModelCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  

  fSetTrackSecondariesFirstCmd 
    = new G4UIcmdWithABool("/mcPhysics/setTrackSecondariesFirst", this);  
  fSetTrackSecondariesFirstCmd
    ->SetGuidance("Set option to track secondaries before finishing their parent track");
  fSetTrackSecondariesFirstCmd->SetParameterName("TrackSecondariesFirst", false);
  fSetTrackSecondariesFirstCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  
}

//______________________________________________________________________________
TG4OpticalPhysicsMessenger::~TG4OpticalPhysicsMessenger() 
{
/// Destructor

  delete fSelectOpProcessCmd; 
  delete fSetOpProcessActivationCmd; 
  delete fSetOpProcessVerboseCmd; 
  delete fSetCerenkovMaxPhotonsCmd;
  delete fSetCerenkovMaxBetaChangeCmd;
  delete fSetScintillationYieldFactorCmd;
  delete fSetOpticalSurfaceModelCmd;
  delete fSetTrackSecondariesFirstCmd; 
}

//
// public methods
//

//______________________________________________________________________________
void TG4OpticalPhysicsMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{ 
/// Apply command to the associated object.

  if (command == fSelectOpProcessCmd) {
    if      ( newValue == "Cerenkov" )      fSelectedProcess = kCerenkov;
    else if ( newValue == "Scintillation" ) fSelectedProcess = kScintillation;
    else if ( newValue == "Absorption" )    fSelectedProcess = kAbsorption;
    else if ( newValue == "Rayleigh" )      fSelectedProcess = kRayleigh;
    else if ( newValue == "Boundary" )      fSelectedProcess = kBoundary;
  }  
  else if (command == fSetOpProcessActivationCmd) {
    fOpticalPhysics
      ->SetProcessActivation(
          fSelectedProcess,
          fSetOpProcessActivationCmd->GetNewBoolValue(newValue));
  }  
  else if (command == fSetOpProcessVerboseCmd) {
    fOpticalPhysics
      ->SetProcessVerbose(
          fSelectedProcess, 
          fSetOpProcessVerboseCmd->GetNewIntValue(newValue));
  }  
  else if (command == fSetCerenkovMaxPhotonsCmd) {
    fOpticalPhysics
      ->SetMaxNumPhotonsPerStep(
          fSetCerenkovMaxPhotonsCmd->GetNewIntValue(newValue));
  }  
  else if (command == fSetCerenkovMaxBetaChangeCmd) {
    fOpticalPhysics
      ->SetMaxBetaChangePerStep(
          fSetCerenkovMaxBetaChangeCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fSetScintillationYieldFactorCmd) {
    fOpticalPhysics
      ->SetScintillationYieldFactor(
          fSetScintillationYieldFactorCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fSetOpticalSurfaceModelCmd) {
    if ( newValue == "glisur" ) {
      fOpticalPhysics
        ->SetOpticalSurfaceModel(glisur);
    }    
    if ( newValue == "unified" ) {
      fOpticalPhysics
        ->SetOpticalSurfaceModel(unified);
    } 
  }  
  else if (command == fSetTrackSecondariesFirstCmd) {
    fOpticalPhysics
      ->SetTrackSecondariesFirst(
          fSetTrackSecondariesFirstCmd->GetNewBoolValue(newValue));
  }  
}
