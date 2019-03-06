//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4DetConstructionMessenger.cxx
/// \brief Implementation of the TG4DetConstructionMessenger class
///
/// \author I. Hrivnacova; IPN, Orsay


#include "TG4DetConstructionMessenger.h"
#include "TG4DetConstruction.h"
#include "TG4GeometryManager.h"
#include "TG4RadiatorDescription.h"
#include "TG4GeometryServices.h"
#include "TG4G3Units.h"
#include "TG4Globals.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4AnalysisUtilities.hh>

//_____________________________________________________________________________
TG4DetConstructionMessenger::TG4DetConstructionMessenger(
                                   TG4GeometryManager* geometryManager)
  : G4UImessenger(),
    fGeometryManager(geometryManager),
    fDirectory(0),
    fUpdateFieldCmd(0),
    fCreateFieldParametersCmd(0),
    fIsLocalFieldCmd(0),
    fIsZeroFieldCmd(0),
    fSeparatorCmd(0),
    fPrintMaterialsCmd(0),
    fPrintMaterialsPropertiesCmd(0),
    fPrintVolumesCmd(0),
    fIsUserMaxStepCmd(),
    fIsMaxStepInLowDensityMaterialsCmd(0),
    fSetLimitDensityCmd(0),
    fSetMaxStepInLowDensityMaterialsCmd(0),
    fSetNewRadiatorCmd(0),
    fSetRadiatorLayerCmd(0),
    fSetRadiatorStrawTubeCmd(0),
    fSetRadiatorCmd(0),
    fRadiatorDescription(0)
{
/// Standard constructor

  fDirectory = new G4UIdirectory("/mcDet/");
  fDirectory->SetGuidance("Detector construction control commands.");

  fUpdateFieldCmd
    = new G4UIcmdWithoutParameter("/mcDet/updateMagField", this);
  G4String guidance = "Update magnetic field.\n";
  guidance += "This command must be called if the field parameters were changed \n";
  guidance += "in the Idle state.";
  fUpdateFieldCmd->SetGuidance(guidance);
  fUpdateFieldCmd->AvailableForStates(G4State_Idle);

  fCreateFieldParametersCmd
    = new G4UIcmdWithAString("/mcDet/createMagFieldParameters", this);
  fCreateFieldParametersCmd
    ->SetGuidance("Create parameters (and their commands) for a local magnetic field \n");
  fCreateFieldParametersCmd
    ->SetGuidance("associated with the volume with the given name.");
  fCreateFieldParametersCmd->SetParameterName("FieldVolName", false);
  fCreateFieldParametersCmd->AvailableForStates(G4State_PreInit);

  fIsLocalFieldCmd
    = new G4UIcmdWithABool("/mcDet/setIsLocalMagField", this);
  fIsLocalFieldCmd
    ->SetGuidance("Get local magnetic fields from Root geometry.");
  fIsLocalFieldCmd->SetParameterName("IsLocalField", false);
  fIsLocalFieldCmd->AvailableForStates(G4State_PreInit);

  fIsZeroFieldCmd
    = new G4UIcmdWithABool("/mcDet/setIsZeroMagField", this);
  guidance
    = "(In)activate propagating 'ifield = 0' flag defined in tracking media.\n";
  guidance
    += "When activated: a zero local magnetic field is set to the volumes defined\n";
  guidance
    += " with tracking medium with 'ifield = 0'.";
  fIsZeroFieldCmd->SetGuidance(guidance);
  fIsZeroFieldCmd->SetParameterName("IsZeroField", false);
  fIsZeroFieldCmd->AvailableForStates(G4State_PreInit);

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

  fIsUserMaxStepCmd
    = new G4UIcmdWithABool("/mcDet/setIsUserMaxStep", this);
  fIsUserMaxStepCmd
    ->SetGuidance("Active user step limits defined in tracking media.");
  fIsUserMaxStepCmd->SetParameterName("IsUserMaxStep", false);
  fIsUserMaxStepCmd->AvailableForStates(G4State_PreInit);

  fIsMaxStepInLowDensityMaterialsCmd
    = new G4UIcmdWithABool("/mcDet/setIsMaxStepInLowDensityMaterials", this);
  fIsMaxStepInLowDensityMaterialsCmd
    ->SetGuidance("Active user step limits defined in tracking media.");
  fIsMaxStepInLowDensityMaterialsCmd
    ->SetParameterName("IsMaxStepInLowDensityMaterials", false);
  fIsMaxStepInLowDensityMaterialsCmd->AvailableForStates(G4State_PreInit);

  fSetLimitDensityCmd
    = new G4UIcmdWithADoubleAndUnit("/mcDet/setLimitDensity", this);
  fSetLimitDensityCmd
    ->SetGuidance("Set the material density limit for setting max allowed step");
  fSetLimitDensityCmd
    ->SetGuidance("Note that setting step limits has to be activated first via:\n");
  fSetLimitDensityCmd
    ->SetGuidance("/mcDet/setIsMaxStepInLowDensityMaterials true");
  fSetLimitDensityCmd->SetParameterName("LimitDensity", false);
  fSetLimitDensityCmd->SetDefaultUnit("g/cm3");
  fSetLimitDensityCmd->SetUnitCategory("Volumic Mass");
  fSetLimitDensityCmd->AvailableForStates(G4State_PreInit);

  fSetMaxStepInLowDensityMaterialsCmd
    = new G4UIcmdWithADoubleAndUnit("/mcDet/setMaxStepInLowDensityMaterials", this);
  fSetMaxStepInLowDensityMaterialsCmd
    ->SetGuidance("Set max allowed step value in materials with density below the density limit");
  fSetMaxStepInLowDensityMaterialsCmd
    ->SetGuidance("Note that setting step limits has to be activated first via:\n");
  fSetMaxStepInLowDensityMaterialsCmd
    ->SetGuidance("/mcDet/setIsMaxStepInLowDensityMaterials true");
  fSetMaxStepInLowDensityMaterialsCmd->SetParameterName("MaxStepInLowDensityMaterials", false);
  fSetMaxStepInLowDensityMaterialsCmd->SetDefaultUnit("mm");
  fSetMaxStepInLowDensityMaterialsCmd->SetUnitCategory("Length");
  fSetMaxStepInLowDensityMaterialsCmd->AvailableForStates(G4State_PreInit);

  CreateSetNewRadiatorCmd();
  CreateSetRadiatorLayerCmd();
  CreateSetRadiatorStrawTubeCmd();

  // This command is now deprecated, will be removed in the next version.
  // It is replaced with a simple setNewRadiator command.
  CreateSetRadiatorCmd();
}

//_____________________________________________________________________________
TG4DetConstructionMessenger::~TG4DetConstructionMessenger()
{
/// Destructor

  delete fDirectory;
  delete fUpdateFieldCmd;
  delete fCreateFieldParametersCmd;
  delete fIsLocalFieldCmd;
  delete fIsZeroFieldCmd;
  delete fSeparatorCmd;
  delete fPrintMaterialsCmd;
  delete fPrintMaterialsPropertiesCmd;
  delete fPrintMediaCmd;
  delete fPrintVolumesCmd;
  delete fPrintCutsCmd;
  delete fPrintControlsCmd;
  delete fIsUserMaxStepCmd;
  delete fIsMaxStepInLowDensityMaterialsCmd;
  delete fSetLimitDensityCmd;
  delete fSetMaxStepInLowDensityMaterialsCmd;
  delete fSetNewRadiatorCmd;
  delete fSetRadiatorLayerCmd;
  delete fSetRadiatorStrawTubeCmd;
  delete fSetRadiatorCmd;
}

//
// private methods
//

//_____________________________________________________________________________
void TG4DetConstructionMessenger::CreateSetNewRadiatorCmd()
{
  G4UIparameter* volumeName = new G4UIparameter("volumeName", 's', false);
  volumeName->SetGuidance("Radiator envelope.");

  G4UIparameter* xtrModel = new G4UIparameter("xtrModel", 's', false);
  xtrModel->SetGuidance("XTR model.");
  //xtrModel->SetCandidates("gammaR gammaM strawR regR transpR regM");

  G4UIparameter* foilNumber = new G4UIparameter("foilNumber", 'i', false);
  foilNumber->SetGuidance("Number of foils");

  G4UIparameter* strawTubeMaterial = new G4UIparameter("strawTubeMaterial", 's', true);
  strawTubeMaterial->SetGuidance("Straw tube material name.");
  strawTubeMaterial->SetDefaultValue("");

  fSetNewRadiatorCmd = new G4UIcommand("/mcDet/setNewRadiator", this);
  fSetNewRadiatorCmd->SetGuidance("Define new radiator.");
  fSetNewRadiatorCmd->SetParameter(volumeName);
  fSetNewRadiatorCmd->SetParameter(xtrModel);
  fSetNewRadiatorCmd->SetParameter(foilNumber);
  fSetNewRadiatorCmd->AvailableForStates(G4State_PreInit);
}

//_____________________________________________________________________________
void TG4DetConstructionMessenger::CreateSetRadiatorLayerCmd()
{
  G4UIparameter* volumeName = new G4UIparameter("materialName", 's', false);
  volumeName->SetGuidance("Layer material name.");

  G4UIparameter* thickness = new G4UIparameter("thickness", 'd', false);
  thickness->SetGuidance("The layer thickness (cm)");

  G4UIparameter* fluctuation = new G4UIparameter("fluctuation", 'd', true);
  G4String guidance = "Parameter that refers to the layer Gamma-distributed thickness.\n";
  guidance += "The relative thickness fluctuation is ~ 1/sqrt(param)";
  fluctuation->SetGuidance(guidance);

  fSetRadiatorLayerCmd = new G4UIcommand("/mcDet/setRadiatorLayer", this);
  fSetRadiatorLayerCmd->SetGuidance("Define the radiator layer (foil/gass) properties.");
  fSetRadiatorLayerCmd->SetParameter(volumeName);
  fSetRadiatorLayerCmd->SetParameter(thickness);
  fSetRadiatorLayerCmd->SetParameter(fluctuation);
  fSetRadiatorLayerCmd->AvailableForStates(G4State_PreInit);
}

//_____________________________________________________________________________
void TG4DetConstructionMessenger::CreateSetRadiatorStrawTubeCmd()
{
  G4UIparameter* gasMaterialName = new G4UIparameter("gasMaterialName", 's', false);
  gasMaterialName->SetGuidance("Straw tube gas material name.");

  G4UIparameter* wallThickness = new G4UIparameter("wallThickness", 'd', false);
  wallThickness->SetGuidance("The mean straw tube wall thickness (cm)");

  G4UIparameter* gasThickness = new G4UIparameter("gasThickness", 'd', false);
  wallThickness->SetGuidance("The mean straw tube gass thickness (cm)");

  fSetRadiatorStrawTubeCmd = new G4UIcommand("/mcDet/setRadiatorStrawTube", this);
  fSetRadiatorStrawTubeCmd->SetGuidance("Define the radiator straw tube properties.");
  fSetRadiatorStrawTubeCmd->SetParameter(gasMaterialName);
  fSetRadiatorStrawTubeCmd->SetParameter(wallThickness);
  fSetRadiatorStrawTubeCmd->SetParameter(gasThickness);
  fSetRadiatorStrawTubeCmd->AvailableForStates(G4State_PreInit);
}

//_____________________________________________________________________________
void TG4DetConstructionMessenger::CreateSetRadiatorCmd()
{
  G4UIparameter* volumeName = new G4UIparameter("volumeName", 's', false);
  volumeName->SetGuidance("Radiator envelope.");

  G4UIparameter* xtrModel = new G4UIparameter("xtrModel", 's', false);
  xtrModel->SetGuidance("XTR model.");
  //xtrModel->SetCandidates("gammaR gammaM strawR regR transpR regM");

  G4UIparameter* foilMaterial = new G4UIparameter("foilMaterial", 's', false);
  foilMaterial->SetGuidance("Foil material name.");

  G4UIparameter* gasMaterial = new G4UIparameter("gasMaterial", 's', false);
  gasMaterial->SetGuidance("Gas material name.");

  G4UIparameter* foilThickness = new G4UIparameter("foilThickness", 'd', false);
  foilThickness->SetGuidance("Foil thickness (cm).");

  G4UIparameter* gasThickness = new G4UIparameter("gasThickness", 'd', false);
  gasThickness->SetGuidance("Gas thickness (cm).");

  G4UIparameter* foilNumber = new G4UIparameter("foilNumber", 'i', false);
  foilNumber->SetGuidance("Number of foils");

  G4UIparameter* strawTubeMaterial = new G4UIparameter("strawTubeMaterial", 's', true);
  strawTubeMaterial->SetGuidance("Straw tube material name.");
  strawTubeMaterial->SetDefaultValue("");

  fSetRadiatorCmd = new G4UIcommand("/mcDet/setRadiator", this);
  fSetRadiatorCmd->SetGuidance("Define radiator properties.");
  fSetRadiatorCmd->SetParameter(volumeName);
  fSetRadiatorCmd->SetParameter(xtrModel);
  fSetRadiatorCmd->SetParameter(foilMaterial);
  fSetRadiatorCmd->SetParameter(gasMaterial);
  fSetRadiatorCmd->SetParameter(foilThickness);
  fSetRadiatorCmd->SetParameter(gasThickness);
  fSetRadiatorCmd->SetParameter(foilNumber);
  fSetRadiatorCmd->SetParameter(strawTubeMaterial);
  fSetRadiatorCmd->AvailableForStates(G4State_PreInit);
}

//
// public methods
//

//_____________________________________________________________________________
void TG4DetConstructionMessenger::SetNewValue(G4UIcommand* command,
                                              G4String newValues)
{
/// Apply command to the associated object.

  if (command == fUpdateFieldCmd) {
    TG4GeometryManager::Instance()->UpdateField();
  }
  else if( command == fCreateFieldParametersCmd ) {
    TG4GeometryManager::Instance()->CreateFieldParameters(newValues);
  }
  else if (command == fIsLocalFieldCmd) {
    TG4GeometryManager::Instance()
      ->SetIsLocalField(fIsLocalFieldCmd->GetNewBoolValue(newValues));
  }
  else if (command == fIsZeroFieldCmd) {
    TG4GeometryManager::Instance()
      ->SetIsZeroField(fIsZeroFieldCmd->GetNewBoolValue(newValues));
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
  else if (command == fIsUserMaxStepCmd) {
    TG4GeometryManager::Instance()
      ->SetIsUserMaxStep(fIsUserMaxStepCmd->GetNewBoolValue(newValues));
  }
  else if (command == fIsMaxStepInLowDensityMaterialsCmd) {
    TG4GeometryManager::Instance()
      ->SetIsMaxStepInLowDensityMaterials(
          fIsMaxStepInLowDensityMaterialsCmd->GetNewBoolValue(newValues));
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
  else if (command == fSetNewRadiatorCmd) {
    // tokenize parameters in a vector
    std::vector<G4String> parameters;
    G4Analysis::Tokenize(newValues, parameters);

    G4int counter = 0;
    G4String volumeName = parameters[counter++];
    G4String xtrModel = parameters[counter++];
    G4int foilNumber = G4UIcommand::ConvertToInt(parameters[counter++]);

    fRadiatorDescription
      = TG4GeometryManager::Instance()->CreateRadiator(volumeName);
    fRadiatorDescription->SetXtrModel(xtrModel);
    fRadiatorDescription->SetFoilNumber(foilNumber);
  }
  else if (command == fSetRadiatorLayerCmd) {

    if ( ! fRadiatorDescription ) {
      TG4Globals::Warning(
        "TG4DetConstructionMessenger", "SetNewValue",
        TString("Radiator was not defined.")
          + TG4Globals::Endl()
          + TString("/mcDetector/setNewRadiator must be called first."));
        return;
    }

    // tokenize parameters in a vector
    std::vector<G4String> parameters;
    G4Analysis::Tokenize(newValues, parameters);

    G4int counter = 0;
    G4String materialName = parameters[counter++];
    G4double thickness = G4UIcommand::ConvertToDouble(parameters[counter++]);
    G4double fluctuation = 0.;
    if ( G4int(parameters.size()) > counter) {
      fluctuation = G4UIcommand::ConvertToDouble(parameters[counter++]);
    }

    // apply units
    thickness *= TG4G3Units::Length();

    fRadiatorDescription->SetLayer(materialName, thickness, fluctuation);
  }
  else if (command == fSetRadiatorStrawTubeCmd) {

    if ( ! fRadiatorDescription ) {
      TG4Globals::Warning(
        "TG4DetConstructionMessenger", "SetNewValue",
        TString("Radiator was not defined.")
          + TG4Globals::Endl()
          + TString("/mcDetector/setNewRadiator must be called first."));
        return;
    }

    // tokenize parameters in a vector
    std::vector<G4String> parameters;
    G4Analysis::Tokenize(newValues, parameters);

    G4int counter = 0;
    G4String gasMaterialName = parameters[counter++];
    G4double wallThickness = G4UIcommand::ConvertToDouble(parameters[counter++]);
    G4double gasThickness = G4UIcommand::ConvertToDouble(parameters[counter++]);

    // apply units
    wallThickness *= TG4G3Units::Length();
    gasThickness *= TG4G3Units::Length();

    fRadiatorDescription->SetStrawTube(gasMaterialName, wallThickness, gasThickness);
  }
  else if (command == fSetRadiatorCmd) {
    // The following code is deprecated, will be removed in the next version

    // tokenize parameters in a vector
    std::vector<G4String> parameters;
    G4Analysis::Tokenize(newValues, parameters);

    G4int counter = 0;
    G4String volumeName = parameters[counter++];
    G4String xtrModel = parameters[counter++];
    G4String foilMaterial = parameters[counter++];
    G4String gasMaterial = parameters[counter++];
    G4double foilThickness = G4UIcommand::ConvertToDouble(parameters[counter++]);
    G4double gasThickness = G4UIcommand::ConvertToDouble(parameters[counter++]);
    G4int foilNumber = G4UIcommand::ConvertToInt(parameters[counter++]);
    G4String strawTubeMaterial;
    if ( G4int(parameters.size()) > counter) {
      strawTubeMaterial = parameters[counter++];
    }

    // apply units
    foilThickness *= TG4G3Units::Length();
    gasThickness *= TG4G3Units::Length();

    TG4RadiatorDescription* radiatorDescription
      = TG4GeometryManager::Instance()->CreateRadiator(volumeName);
    radiatorDescription->SetXtrModel(xtrModel);
    radiatorDescription->SetFoilNumber(foilNumber);
    radiatorDescription->SetLayer(foilMaterial, foilThickness, 100.);
    radiatorDescription->SetLayer(gasMaterial, gasThickness, 100.);
    if ( strawTubeMaterial.size() ) {
      radiatorDescription->SetStrawTube(strawTubeMaterial, 0.53*CLHEP::mm, 3.14159*CLHEP::mm);
    }
  }
}
