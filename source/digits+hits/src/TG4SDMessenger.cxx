//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4SDMessenger.cxx
/// \brief Implementation of the TG4SDMessenger class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SDMessenger.h"
#include "TG4SDConstruction.h"
#include "TG4SDServices.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithoutParameter.hh>

//______________________________________________________________________________
TG4SDMessenger::TG4SDMessenger(TG4SDConstruction* sdConstruction)
  : G4UImessenger(),
    fSDConstruction(sdConstruction),
    fAddSDSelectionCmd(0),
    fSetSDSelectionFromTGeoCmd(0),
    fSetSVLabelCmd(0),
    fSetGflashCmd(0),
    fSetExclusiveSDScoringCmd(0),
    fPrintUserSDsCmd(0)
{
/// Standard constructor

  fAddSDSelectionCmd
    = new G4UIcmdWithAString("/mcDet/addSDSelection", this);
  G4String guidance
    = "Selects volumes which will be make sensitive \n";
  guidance += "(When any selection is applied MCApllication::Stepping() is called only ";
  guidance += "from the selected volumes.)";
  fAddSDSelectionCmd->SetGuidance(guidance);
  fAddSDSelectionCmd->SetParameterName("SDSelection", false);
  fAddSDSelectionCmd->AvailableForStates(G4State_PreInit);

  fSetSDSelectionFromTGeoCmd
    = new G4UIcmdWithABool("/mcDet/setSDSelectionFromTGeo", this);
  guidance
    = "Get sensitive volumes selection from TGeo geometry. \n";
  guidance += "(When any selection is applied MCApllication::Stepping() is called only ";
  guidance += "from the selected volumes.)";
  fSetSDSelectionFromTGeoCmd->SetGuidance(guidance);
  fSetSDSelectionFromTGeoCmd->SetParameterName("SDSelectionFromTGeo", false);
  fSetSDSelectionFromTGeoCmd->AvailableForStates(G4State_PreInit);

  fSetSVLabelCmd
    = new G4UIcmdWithAString("/mcDet/setSVLabel", this);
  guidance
    = "Set the string which is used in TGeoVolume::SetOption  \n";
  guidance += "(to label sensitive volumes (default is \"";
  guidance += TG4SDConstruction::GetDefaultSVLabel();
  guidance += "\".)";
  fSetSVLabelCmd->SetGuidance(guidance);
  fSetSVLabelCmd->SetParameterName("SVLabel", false);
  fSetSVLabelCmd->AvailableForStates(G4State_PreInit);

  fSetGflashCmd
    = new G4UIcmdWithABool("/mcDet/setGflash", this);
  guidance
    = "Activate creating sensitive detectors adapted for GFlash.";
  fSetGflashCmd->SetGuidance(guidance);
  fSetGflashCmd->SetParameterName("Gflash", false);
  fSetGflashCmd->AvailableForStates(G4State_PreInit);

  fSetExclusiveSDScoringCmd
    = new G4UIcmdWithABool("/mcDet/setExclusiveSDScoring", this);
  guidance
    = "Activate scoring by user sensitive detectors only.\n";
  guidance
    += "The MC Application::Stepping() will be not called.";
  fSetExclusiveSDScoringCmd->SetGuidance(guidance);
  fSetExclusiveSDScoringCmd->SetParameterName("ExclusiveSDScoring", false);
  fSetExclusiveSDScoringCmd->AvailableForStates(G4State_PreInit);

  fPrintUserSDsCmd
    = new G4UIcmdWithoutParameter("/mcDet/printUserSDs", this);
  fPrintUserSDsCmd->SetGuidance("Prints user sensitive detectors.");
  fPrintUserSDsCmd->AvailableForStates(G4State_Init, G4State_Idle);
}

//______________________________________________________________________________
TG4SDMessenger::~TG4SDMessenger()
{
/// Destructor

  delete fAddSDSelectionCmd;
  delete fSetSDSelectionFromTGeoCmd;
  delete fSetSVLabelCmd;
  delete fSetGflashCmd;
  delete fSetExclusiveSDScoringCmd;
  delete fPrintUserSDsCmd;
}

//
// public methods
//

//______________________________________________________________________________
void TG4SDMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{
/// Apply command to the associated object.

  if ( command == fAddSDSelectionCmd ) {
    fSDConstruction->AddSelection(newValue);
  }
  else if ( command == fSetSDSelectionFromTGeoCmd ) {
    fSDConstruction->SetSelectionFromTGeo(
                       fSetSDSelectionFromTGeoCmd->GetNewBoolValue(newValue));
  }
  else if ( command == fSetSVLabelCmd ) {
    fSDConstruction->SetSensitiveVolumeLabel(newValue);
  }
  else if ( command == fSetGflashCmd ) {
    fSDConstruction->SetIsGflash(
                       fSetGflashCmd->GetNewBoolValue(newValue));
  }
  else if ( command == fSetExclusiveSDScoringCmd ) {
    fSDConstruction->SetExclusiveSDScoring(
                       fSetExclusiveSDScoringCmd->GetNewBoolValue(newValue));
  }
  else if ( command == fPrintUserSDsCmd ) {
    TG4SDServices::Instance()->PrintUserSensitiveDetectors();
  }
}
