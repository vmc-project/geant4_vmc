//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ComposedPhysicsMessenger.cxx
/// \brief Implementation of the TG4ComposedPhysicsMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ComposedPhysicsList.h"
#include "TG4ComposedPhysicsMessenger.h"
#include "TG4GeometryServices.h"
#include "TG4PhysicsManager.h"
#include "TG4G3PhysicsManager.h"
#include "TG4ProcessControlMap.h"
#include "TG4ProcessMCMap.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAString.hh>

//______________________________________________________________________________
TG4ComposedPhysicsMessenger::TG4ComposedPhysicsMessenger(
                                   TG4ComposedPhysicsList* physicsList)
  : G4UImessenger(),
    fPhysicsList(physicsList),
    fRangeGammaCutCmd(0),   
    fRangeElectronCutCmd(0),
    fRangePositronCutCmd(0),
    fRangeAllCutCmd(0)     
{ 
/// Standard constructor

  fDirectory = new G4UIdirectory("/mcPhysics/");
  fDirectory->SetGuidance("TGeant4 physics control commands.");

  fRangeGammaCutCmd 
    = new G4UIcmdWithADoubleAndUnit("/mcPhysics/rangeCutForGamma", this);  
  fRangeGammaCutCmd->SetGuidance("Set range cut for gamma");
  fRangeGammaCutCmd->SetParameterName("GammaCut",false);
  fRangeGammaCutCmd->SetUnitCategory("Length");
  fRangeGammaCutCmd->SetRange("GammaCut>0.0");
  fRangeGammaCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fRangeElectronCutCmd 
    = new G4UIcmdWithADoubleAndUnit("/mcPhysics/rangeCutForElectron", this);  
  fRangeElectronCutCmd->SetGuidance("Set range cut for electron.");
  fRangeElectronCutCmd->SetParameterName("ElectronCut", false);
  fRangeElectronCutCmd->SetUnitCategory("Length");
  fRangeElectronCutCmd->SetRange("ElectronCut>0.0");
  fRangeElectronCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  
  fRangePositronCutCmd 
    = new G4UIcmdWithADoubleAndUnit("/mcPhysics/rangeCutForPositron", this);  
  fRangePositronCutCmd->SetGuidance("Set range cut for positron.");
  fRangePositronCutCmd->SetParameterName("PositronCut", false);
  fRangePositronCutCmd->SetUnitCategory("Length");
  fRangePositronCutCmd->SetRange("PositronCut>0.0");
  fRangePositronCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  

  fRangeAllCutCmd 
    = new G4UIcmdWithADoubleAndUnit("/mcPhysics/rangeCuts", this);  
  fRangeAllCutCmd->SetGuidance("Set range cut for all.");
  fRangeAllCutCmd->SetParameterName("AllCut", false);
  fRangeAllCutCmd->SetUnitCategory("Length");
  fRangeAllCutCmd->SetRange("AllCut>0.0");
  fRangeAllCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fPrintAllProcessesCmd 
    = new G4UIcmdWithoutParameter("/mcPhysics/printAllProcess", this);  
  fPrintAllProcessesCmd->SetGuidance("Print names for all instantiated processes.");
  fPrintAllProcessesCmd->AvailableForStates(G4State_Idle);  

  fDumpAllProcessesCmd 
    = new G4UIcmdWithoutParameter("/mcPhysics/dumpAllProcess", this);  
  fDumpAllProcessesCmd->SetGuidance("Dump all instantiated processes.");
  fDumpAllProcessesCmd->AvailableForStates(G4State_Idle);  

  fPrintProcessMCMapCmd
     = new G4UIcmdWithoutParameter("/mcPhysics/printProcessMCMap", this);
  fPrintProcessMCMapCmd
    ->SetGuidance("Print mapping of G4 processes to VMC process codes.");
  fPrintProcessMCMapCmd->AvailableForStates(G4State_Idle);

  fPrintProcessControlMapCmd
     = new G4UIcmdWithoutParameter("/mcPhysics/printProcessControlMap", this);
  fPrintProcessControlMapCmd
    ->SetGuidance("Print mapping of G4 processes to VMC (G3-like) controls.");
  fPrintProcessControlMapCmd->AvailableForStates(G4State_Idle);

  fPrintVolumeLimitsCmd
     = new G4UIcmdWithAString("/mcPhysics/printVolumeLimits", this);
  fPrintVolumeLimitsCmd
    ->SetGuidance("Print the limits set for the specified volume.");
  fPrintVolumeLimitsCmd->SetParameterName("PrintVolumeLimits", false);
  fPrintVolumeLimitsCmd->AvailableForStates(G4State_Idle);

  fPrintGlobalCutsCmd
     = new G4UIcmdWithoutParameter("/mcPhysics/printGlobalCuts", this);
  fPrintGlobalCutsCmd
    ->SetGuidance("Print global VMC (G3-like) cuts.");
  fPrintGlobalCutsCmd->AvailableForStates(G4State_Idle);

  fPrintGlobalControlsCmd
     = new G4UIcmdWithoutParameter("/mcPhysics/printGlobalControls", this);
  fPrintGlobalControlsCmd
    ->SetGuidance("Print global VMC (G3-like) process controls.");
  fPrintGlobalControlsCmd->AvailableForStates(G4State_Idle);
}

//______________________________________________________________________________
TG4ComposedPhysicsMessenger::~TG4ComposedPhysicsMessenger() 
{
/// Destructor

  delete fDirectory;
  delete fRangeGammaCutCmd;
  delete fRangeElectronCutCmd;
  delete fRangePositronCutCmd;
  delete fRangeAllCutCmd;
  delete fPrintAllProcessesCmd;
  delete fDumpAllProcessesCmd;
  delete fPrintProcessMCMapCmd;
  delete fPrintProcessControlMapCmd;
  delete fPrintVolumeLimitsCmd;
  delete fPrintGlobalCutsCmd;
  delete fPrintGlobalControlsCmd;
}

//
// public methods
//

//______________________________________________________________________________
void TG4ComposedPhysicsMessenger::SetNewValue(G4UIcommand* command,
                                                 G4String newValue)
{ 
/// Apply command to the associated object.
  
  if( command == fRangeGammaCutCmd ) { 
    G4double cut = fRangeGammaCutCmd->GetNewDoubleValue(newValue);
    TG4PhysicsManager::Instance()->SetCutForGamma(cut);
    fPhysicsList->SetCutForGamma(cut);
  }
  else if ( command == fRangeElectronCutCmd ) { 
    G4double cut = fRangeElectronCutCmd->GetNewDoubleValue(newValue);
    TG4PhysicsManager::Instance()->SetCutForElectron(cut);
    fPhysicsList->SetCutForElectron(cut);
  }
  else if ( command == fRangePositronCutCmd ) { 
    G4double cut = fRangePositronCutCmd->GetNewDoubleValue(newValue);
    TG4PhysicsManager::Instance()->SetCutForPositron(cut);
    fPhysicsList->SetCutForPositron(cut);
  }
  else if( command == fRangeAllCutCmd ) {
    G4double cut = fRangeAllCutCmd->GetNewDoubleValue(newValue);
    TG4PhysicsManager::Instance()->SetCutForGamma(cut);
    TG4PhysicsManager::Instance()->SetCutForElectron(cut);
    TG4PhysicsManager::Instance()->SetCutForPositron(cut);
    fPhysicsList->SetCutForGamma(cut);
    fPhysicsList->SetCutForElectron(cut);
    fPhysicsList->SetCutForPositron(cut);
  }
  else if (command == fPrintAllProcessesCmd) {
    fPhysicsList->PrintAllProcesses();
  }  
  else if (command == fDumpAllProcessesCmd) {
    fPhysicsList->DumpAllProcesses();
  }  
  else if (command == fPrintProcessMCMapCmd) {
    TG4ProcessMCMap::Instance()->PrintAll();
  }  
  else if (command == fPrintProcessControlMapCmd) {
    TG4ProcessControlMap::Instance()->PrintAll();
  }  
  else if (command == fPrintVolumeLimitsCmd) {
    TG4GeometryServices::Instance()->PrintVolumeLimits(newValue);
  }  
  else if (command == fPrintGlobalCutsCmd) {
    TG4G3PhysicsManager::Instance()->GetCutVector()->Print();
  }  
  else if (command == fPrintGlobalControlsCmd) {
    TG4G3PhysicsManager::Instance()->GetControlVector()->Print();
  }  
}
