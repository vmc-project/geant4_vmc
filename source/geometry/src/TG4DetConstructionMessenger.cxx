// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4DetConstructionMessenger.cxx
/// \brief Implementation of the TG4DetConstructionMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay


#include "TG4DetConstructionMessenger.h"
#include "TG4DetConstruction.h"
#include "TG4GeometryManager.h"
#include "TG4GeometryServices.h"
#include "TG4Globals.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

//_____________________________________________________________________________
TG4DetConstructionMessenger::TG4DetConstructionMessenger(
                                   TG4GeometryManager* geometryManager)
  : G4UImessenger(),
    fGeometryManager(geometryManager),
    fDirectory(0),
    fUpdateMagFieldCmd(0),
    fSeparatorCmd(0),
    fPrintMaterialsCmd(0),
    fPrintMaterialsPropertiesCmd(0),
    fPrintVolumesCmd(0),
    fSetLimitDensityCmd(0),
    fSetMaxStepInLowDensityMaterialsCmd(0)
    
{
/// Standard constructor

  fDirectory = new G4UIdirectory("/mcDet/");
  fDirectory->SetGuidance("Detector construction control commands.");

  fUpdateMagFieldCmd 
    = new G4UIcmdWithoutParameter("/mcDet/updateMagField", this);
  G4String guidance = "Update magnetic field.\n";
  guidance += "This command must be called if the field parameters were changed \n";
  guidance += "in the Idle state.";
  fUpdateMagFieldCmd->SetGuidance(guidance);
  fUpdateMagFieldCmd->AvailableForStates(G4State_Idle);   

  fSeparatorCmd = new G4UIcmdWithAString("/mcDet/volNameSeparator", this);
  guidance 
    = "Override the default value of the volume name separator in g3tog4\n";
  fSeparatorCmd->SetGuidance(guidance);
  fSeparatorCmd->SetParameterName("VolNameSeparator", true);
  fSeparatorCmd->AvailableForStates(G4State_PreInit);

  fPrintMaterialsCmd 
    = new G4UIcmdWithoutParameter("/mcDet/printMaterials", this);
  fPrintMaterialsCmd->SetGuidance("Prints all materials.");
  fPrintMaterialsCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);   

  fPrintMaterialsPropertiesCmd 
    = new G4UIcmdWithoutParameter("/mcDet/printMaterialsProperties", this);
  fPrintMaterialsPropertiesCmd->SetGuidance("Prints all material properties for all materials.");
  fPrintMaterialsPropertiesCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);   

  fPrintMediaCmd 
    = new G4UIcmdWithoutParameter("/mcDet/printMedia", this);
  fPrintMediaCmd->SetGuidance("Prints all media.");
  fPrintMediaCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);   

  fPrintVolumesCmd 
    = new G4UIcmdWithoutParameter("/mcDet/printVolumes", this);
  fPrintVolumesCmd->SetGuidance("Prints all volumes.");
  fPrintVolumesCmd->AvailableForStates(G4State_Idle);   

  fPrintCutsCmd 
    = new G4UIcmdWithAString("/mcDet/printCuts", this);
  fPrintCutsCmd
    ->SetGuidance("Prints the cut value for given CutName for all tracking media");
  fPrintCutsCmd->SetParameterName("CutName", false);
  fPrintCutsCmd->AvailableForStates(G4State_Idle);   

  fPrintControlsCmd 
    = new G4UIcmdWithAString("/mcDet/printControls", this);
  fPrintControlsCmd
    ->SetGuidance("Prints the control value for given ControlName for all tracking media");
  fPrintControlsCmd->SetParameterName("ControlName", false);
  fPrintControlsCmd->AvailableForStates(G4State_Idle);   

  fSetLimitDensityCmd 
    = new G4UIcmdWithADoubleAndUnit("/mcDet/setLimitDensity", this);
  fSetLimitDensityCmd->SetGuidance("Set the material density limit for setting max allowed step");
  fSetLimitDensityCmd->SetParameterName("LimitDensity", false);
  fSetLimitDensityCmd->SetDefaultUnit("g/cm3");
  fSetLimitDensityCmd->SetUnitCategory("Volumic Mass");
  fSetLimitDensityCmd->AvailableForStates(G4State_PreInit);
  
  fSetMaxStepInLowDensityMaterialsCmd 
    = new G4UIcmdWithADoubleAndUnit("/mcDet/setMaxStepInLowDensityMaterials", this);
  fSetMaxStepInLowDensityMaterialsCmd
    ->SetGuidance("Set max allowed step value in materials with density below the density limit");
  fSetMaxStepInLowDensityMaterialsCmd->SetParameterName("MaxStepInLowDensityMaterials", false);
  fSetMaxStepInLowDensityMaterialsCmd->SetDefaultUnit("mm");
  fSetMaxStepInLowDensityMaterialsCmd->SetUnitCategory("Length");
  fSetMaxStepInLowDensityMaterialsCmd->AvailableForStates(G4State_PreInit);
  
}

//_____________________________________________________________________________
TG4DetConstructionMessenger::~TG4DetConstructionMessenger() 
{
/// Destructor

  delete fDirectory;
  delete fUpdateMagFieldCmd;
  delete fSeparatorCmd;
  delete fPrintMaterialsCmd;
  delete fPrintMaterialsPropertiesCmd;
  delete fPrintMediaCmd;
  delete fPrintVolumesCmd;
  delete fPrintCutsCmd; 
  delete fPrintControlsCmd; 
  delete fSetLimitDensityCmd;
  delete fSetMaxStepInLowDensityMaterialsCmd;
}

//
// public methods
//
  
//_____________________________________________________________________________
void TG4DetConstructionMessenger::SetNewValue(G4UIcommand* command, 
                                               G4String newValues)
{
/// Apply command to the associated object.

  if (command == fUpdateMagFieldCmd) {
    TG4GeometryManager::Instance()->UpdateMagField();
  }    
  else if( command == fSeparatorCmd ) { 
    char separator = newValues(0);
    TG4GeometryServices::Instance()->SetG3toG4Separator(separator);
  }
  else if (command == fPrintMaterialsCmd) {
    TG4GeometryServices::Instance()->PrintMaterials();
  }    
  else if (command == fPrintMaterialsPropertiesCmd) {
    TG4GeometryServices::Instance()->PrintMaterialsProperties();
  }    
  else if (command == fPrintMediaCmd) {
    TG4GeometryServices::Instance()->PrintMedia();
  }    
  else if (command == fPrintVolumesCmd) {
    TG4GeometryServices::Instance()->PrintLogicalVolumeStore();
  }    
  else if (command == fPrintCutsCmd) {
    TG4GeometryServices::Instance()->PrintCuts(newValues);
  }    
  else if (command == fPrintControlsCmd) {
    TG4GeometryServices::Instance()->PrintControls(newValues);
  } 
  else if (command == fSetLimitDensityCmd) {
    TG4GeometryManager::Instance()
      ->SetLimitDensity(fSetLimitDensityCmd->GetNewDoubleValue(newValues));
  } 
  else if (command == fSetMaxStepInLowDensityMaterialsCmd) {
    TG4GeometryManager::Instance()
      ->SetMaxStepInLowDensityMaterials(
          fSetMaxStepInLowDensityMaterialsCmd->GetNewDoubleValue(newValues));
  } 
}
