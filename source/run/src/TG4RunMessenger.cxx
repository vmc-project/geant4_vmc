//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4RunMessenger.cxx
/// \brief Implementation of the TG4RunMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RunMessenger.h"
#include "TG4RunManager.h"
#include "TG4Globals.h"
#include "TG4UICmdWithAComplexString.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithABool.hh>

//_____________________________________________________________________________
TG4RunMessenger::TG4RunMessenger(TG4RunManager* runManager)
  : G4UImessenger(),
    fRunManager(runManager),
    fDirectory(0),
    fRootCmd(0),            
    fRootMacroCmd(0),  
    fRootCommandCmd(0),
    fUseRootRandomCmd(0),
    fG3DefaultsCmd(0)
{ 
/// Standard constructor

  fDirectory = new G4UIdirectory("/mcControl/");
  fDirectory->SetGuidance("TGeant4 control commands.");

  fRootCmd = new G4UIcmdWithoutParameter("/mcControl/root", this);
  fRootCmd->SetGuidance("Switch to Root interactive shell.");
  fRootCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle, 
                               G4State_GeomClosed, G4State_EventProc);

  fRootMacroCmd = new G4UIcmdWithAString("/mcControl/rootMacro", this);
  fRootMacroCmd->SetGuidance("Process Root macro with given name (from file name.C)");
  fRootMacroCmd->SetParameterName("macroName", true);
  fRootMacroCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle, 
                                    G4State_GeomClosed, G4State_EventProc);

  fRootCommandCmd = new TG4UICmdWithAComplexString("/mcControl/rootCmd", this);
  fRootCommandCmd->SetGuidance("Process Root command");
  fRootCommandCmd->SetParameterName("command", false);
  fRootCommandCmd->SetDefaultValue(" ");
  fRootCommandCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle, 
                                      G4State_GeomClosed, G4State_EventProc);

  fUseRootRandomCmd = new G4UIcmdWithABool("/mcControl/useRootRandom", this);
  fUseRootRandomCmd
    ->SetGuidance("(In)Activate passing the random number seed from Root to Geant4");
  fUseRootRandomCmd->SetParameterName("UseRootRandom", true);
  fUseRootRandomCmd->AvailableForStates(G4State_PreInit);

  fG3DefaultsCmd = new G4UIcmdWithoutParameter("/mcControl/g3Defaults", this);
  fG3DefaultsCmd->SetGuidance("Set G3 default parameters (cut values,");
  fG3DefaultsCmd->SetGuidance("tracking media max step values, ...)");
  fG3DefaultsCmd->AvailableForStates(G4State_PreInit);
}

//_____________________________________________________________________________
TG4RunMessenger::~TG4RunMessenger() 
{
/// Destructor

  delete fDirectory;
  delete fRootCmd;
  delete fRootMacroCmd;
  delete fRootCommandCmd;
  delete fUseRootRandomCmd;
  delete fG3DefaultsCmd;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4RunMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
/// Apply command to the associated object.

  if (command == fRootCmd) {
    fRunManager->StartRootUI(); 
  }
  else if (command == fRootMacroCmd) {  
    fRunManager->ProcessRootMacro(newValue); 
  }
  else if (command == fRootCommandCmd) {
    fRunManager->ProcessRootCommand(newValue); 
  }
  else if (command == fUseRootRandomCmd) {  
    fRunManager->UseRootRandom(fUseRootRandomCmd->GetNewBoolValue(newValue)); 
  }
  else if (command == fG3DefaultsCmd) {
    fRunManager->UseG3Defaults(); 
  }
}
