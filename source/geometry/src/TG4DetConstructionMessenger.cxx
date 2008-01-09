// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
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
#include "TG4MagneticFieldType.h"
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
    fFieldTypeCmd(0),
    fSeparatorCmd(0),
    fUniformFieldValueCmd(0),
    fPrintMaterialsCmd(0),
    fPrintVolumesCmd(0)
    
{
/// Standard constructor

  fDirectory = new G4UIdirectory("/mcDet/");
  fDirectory->SetGuidance("Detector construction control commands.");

  fFieldTypeCmd = new G4UIcmdWithAString("/mcDet/fieldType", this);
  G4String guidance =   "Select type of magnetic field:\n";
  guidance = guidance + "  MCApplication:  field defined by VMC application (default)\n";
  guidance = guidance + "  Uniform:        uniform magnetic field\n";
  guidance = guidance + "  None:           no magnetic field";
  fFieldTypeCmd->SetGuidance(guidance);
  fFieldTypeCmd->SetParameterName("FieldType", true);
  fFieldTypeCmd->SetCandidates("MCApplication Uniform None");   
  fFieldTypeCmd->SetDefaultValue("MCApplication");
  fFieldTypeCmd->AvailableForStates(G4State_PreInit);

  fSeparatorCmd = new G4UIcmdWithAString("/mcDet/volNameSeparator", this);
  guidance =   "Override the default value of the volume name separator in g3tog4\n";
  fSeparatorCmd->SetGuidance(guidance);
  fSeparatorCmd->SetParameterName("VolNameSeparator", true);
  fSeparatorCmd->AvailableForStates(G4State_PreInit);

  fUniformFieldValueCmd 
    = new G4UIcmdWithADoubleAndUnit("/mcDet/uniformFieldValue", this);
  fUniformFieldValueCmd
    ->SetGuidance("Define uniform magnetic field in Z direction.");
  fUniformFieldValueCmd
    ->SetGuidance("(Uniform magnetic field type has to be selected first.)");
  fUniformFieldValueCmd->SetParameterName("UniformFieldValue", false, false);
  fUniformFieldValueCmd->SetDefaultUnit("tesla");
  fUniformFieldValueCmd->SetUnitCategory("Magnetic flux density");
  fUniformFieldValueCmd->AvailableForStates(G4State_Idle);  
  
  fPrintMaterialsCmd 
    = new G4UIcmdWithoutParameter("/mcDet/printMaterials", this);
  fPrintMaterialsCmd->SetGuidance("Prints all materials.");
  fPrintMaterialsCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);   

  fPrintMediaCmd 
    = new G4UIcmdWithoutParameter("/mcDet/printMedia", this);
  fPrintMediaCmd->SetGuidance("Prints all media.");
  fPrintMediaCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);   

  fPrintVolumesCmd 
    = new G4UIcmdWithoutParameter("/mcDet/printVolumes", this);
  fPrintMaterialsCmd->SetGuidance("Prints all volumes.");
  fPrintMaterialsCmd->AvailableForStates(G4State_Idle);   
}

//_____________________________________________________________________________
TG4DetConstructionMessenger::~TG4DetConstructionMessenger() 
{
/// Destructor

  delete fDirectory;
  delete fFieldTypeCmd;
  delete fSeparatorCmd;
  delete fUniformFieldValueCmd;
  delete fPrintMaterialsCmd;
  delete fPrintMediaCmd;
  delete fPrintVolumesCmd;
}

//
// public methods
//
  
//_____________________________________________________________________________
void TG4DetConstructionMessenger::SetNewValue(G4UIcommand* command, 
                                               G4String newValues)
{
/// Apply command to the associated object.

  if( command == fFieldTypeCmd ) { 
    if (newValues == "MCApplication") 
      fGeometryManager->SetFieldType(kMCApplicationField); 
    if (newValues == "Uniform") 
      fGeometryManager->SetFieldType(kUniformField); 
    if (newValues == "None") 
      fGeometryManager->SetFieldType(kNoField); 
  }
  else if( command == fSeparatorCmd ) { 
    char separator = newValues(0);
    TG4GeometryServices::Instance()->SetG3toG4Separator(separator);
  }
  if (command == fUniformFieldValueCmd) {  
    fGeometryManager
      ->SetUniformFieldValue(fUniformFieldValueCmd->GetNewDoubleValue(newValues)); 
  }
  else if (command == fPrintMaterialsCmd) {
    TG4GeometryServices::Instance()->PrintMaterials();
  }    
  else if (command == fPrintMediaCmd) {
    TG4GeometryServices::Instance()->PrintMedia();
  }    
  else if (command == fPrintVolumesCmd) {
    TG4GeometryServices::Instance()->PrintLogicalVolumeStore();
  }    
}
