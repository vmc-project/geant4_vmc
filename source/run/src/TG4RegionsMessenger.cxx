//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4RegionsMessenger.cxx
/// \brief Implementation of the TG4RegionsMessenger class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RegionsMessenger.h"
#include "TG4RegionsManager.h"

#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIdirectory.hh>

//_____________________________________________________________________________
TG4RegionsMessenger::TG4RegionsMessenger(TG4RegionsManager* runManager)
  : G4UImessenger(),
    fRegionsManager(runManager),
    fDirectory(0),
    fIsFromG4Table(false)
{
  /// Standard constructor

  fDirectory = new G4UIdirectory("/mcRegions/");
  fDirectory->SetGuidance("TGeant4 regions commands.");

  fDumpRegionCmd = new G4UIcmdWithAString("/mcRegions/dumpRegion", this);
  fDumpRegionCmd->SetGuidance("Dump characteristics of region ");
  fDumpRegionCmd->SetParameterName("LVname", false);
  fDumpRegionCmd->SetDefaultValue(" ");
  fDumpRegionCmd->AvailableForStates(G4State_Idle, G4State_EventProc);

  fSetRangePrecisionCmd =
    new G4UIcmdWithAnInteger("/mcRegions/setRangePrecision", this);
  fSetRangePrecisionCmd->SetGuidance(
    "Set the precision for calculating ranges");
  fSetRangePrecisionCmd->SetParameterName("RangePrecision", false);
  fSetRangePrecisionCmd->AvailableForStates(G4State_PreInit, G4State_Init);

  fSetEnergyToleranceCmd =
    new G4UIcmdWithADouble("/mcRegions/setEnergyTolerance", this);
  fSetEnergyToleranceCmd->SetGuidance(
    "Set the tolerance (relative) for comparing energy cut values");
  fSetEnergyToleranceCmd->SetParameterName("EnergyTolerance", false);
  fSetEnergyToleranceCmd->AvailableForStates(G4State_PreInit, G4State_Init);

  fApplyForGammaCmd = new G4UIcmdWithABool("/mcRegions/applyForGamma", this);
  fApplyForGammaCmd->SetGuidance("Switch on|off applying range cuts for gamma");
  fApplyForGammaCmd->SetParameterName("ApplyForGamma", false);
  fApplyForGammaCmd->AvailableForStates(G4State_PreInit, G4State_Init);

  fApplyForElectronCmd =
    new G4UIcmdWithABool("/mcRegions/applyForElectron", this);
  fApplyForElectronCmd->SetGuidance("Switch on|off applying range cuts for e-");
  fApplyForElectronCmd->SetParameterName("ApplyForElectron", false);
  fApplyForElectronCmd->AvailableForStates(G4State_PreInit, G4State_Init);

  fApplyForPositronCmd =
    new G4UIcmdWithABool("/mcRegions/applyForPositron", this);
  fApplyForPositronCmd->SetGuidance("Switch on|off applying range cuts for e+");
  fApplyForPositronCmd->SetParameterName("ApplyForPositron", false);
  fApplyForPositronCmd->AvailableForStates(G4State_PreInit, G4State_Init);

  fApplyForProtonCmd = new G4UIcmdWithABool("/mcRegions/applyForProton", this);
  fApplyForProtonCmd->SetGuidance(
    "Switch on|off applying range cuts for protons");
  fApplyForProtonCmd->SetParameterName("ApplyForProton", false);
  fApplyForProtonCmd->AvailableForStates(G4State_PreInit, G4State_Init);

  fSetCheckCmd = new G4UIcmdWithABool("/mcRegions/check", this);
  fSetCheckCmd->SetGuidance(
    "Switch on|off check if region properties are consistent");
  fSetCheckCmd->SetGuidance("with energy cuts defined in limits");
  fSetCheckCmd->SetParameterName("IsCheck", false);
  fSetCheckCmd->AvailableForStates(G4State_PreInit, G4State_Init);

  fSetPrintCmd = new G4UIcmdWithABool("/mcRegions/print", this);
  fSetPrintCmd->SetGuidance("Switch on|off printing of all regions properties");
  fSetPrintCmd->SetParameterName("IsPrint", false);
  fSetPrintCmd->AvailableForStates(G4State_PreInit, G4State_Init);

  fSetSaveCmd = new G4UIcmdWithABool("/mcRegions/save", this);
  fSetSaveCmd->SetGuidance("Switch on|off saving of all regions properties in a file");
  fSetSaveCmd->SetParameterName("IsSave", false);
  fSetSaveCmd->AvailableForStates(G4State_PreInit, G4State_Init);

  fSetLoadCmd = new G4UIcmdWithABool("/mcRegions/load", this);
  fSetLoadCmd->SetGuidance("Switch on|off loading of all regions cuts & ranges from a file");
  fSetLoadCmd->SetParameterName("IsLoad", false);
  fSetLoadCmd->AvailableForStates(G4State_PreInit, G4State_Init);

  fSetFromG4TableCmd = new G4UIcmdWithABool("/mcRegions/fromG4Table", this);
  fSetFromG4TableCmd->SetGuidance("Switch on|off printing or saving regions properties\n"
    "from production cuts table.\n"
    "Must be called before \"print\" or \"save\" command.");
  fSetFromG4TableCmd->SetParameterName("IsFromG4Table", false);
  fSetFromG4TableCmd->AvailableForStates(G4State_PreInit, G4State_Init);

  fSetFileNameCmd = new G4UIcmdWithAString("/mcRegions/setFileName", this);
  fSetFileNameCmd->SetGuidance("Set file name for the regions output");
  fSetFileNameCmd->SetParameterName("FileName", false);
  fSetFileNameCmd->AvailableForStates(G4State_PreInit, G4State_Init);

}

//_____________________________________________________________________________
TG4RegionsMessenger::~TG4RegionsMessenger()
{
  /// Destructor

  delete fDirectory;
  delete fDumpRegionCmd;
  delete fSetRangePrecisionCmd;
  delete fSetEnergyToleranceCmd;
  delete fApplyForGammaCmd;
  delete fApplyForElectronCmd;
  delete fApplyForPositronCmd;
  delete fApplyForProtonCmd;
  delete fSetCheckCmd;
  delete fSetPrintCmd;
  delete fSetSaveCmd;
  delete fSetLoadCmd;
  delete fSetFromG4TableCmd;
  delete fSetFileNameCmd;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4RegionsMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  /// Apply command to the associated object.

  if (command == fDumpRegionCmd) {
    fRegionsManager->DumpRegion(newValue);
  }
  else if (command == fSetRangePrecisionCmd) {
    fRegionsManager->SetRangePrecision(
      fSetRangePrecisionCmd->GetNewIntValue(newValue));
  }
  else if (command == fSetEnergyToleranceCmd) {
    fRegionsManager->SetEnergyTolerance(
      fSetEnergyToleranceCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fApplyForGammaCmd) {
    fRegionsManager->SetApplyForGamma(
      fApplyForGammaCmd->GetNewBoolValue(newValue));
  }
  else if (command == fApplyForElectronCmd) {
    fRegionsManager->SetApplyForGamma(
      fApplyForElectronCmd->GetNewBoolValue(newValue));
  }
  else if (command == fApplyForPositronCmd) {
    fRegionsManager->SetApplyForPositron(
      fApplyForPositronCmd->GetNewBoolValue(newValue));
  }
  else if (command == fApplyForProtonCmd) {
    fRegionsManager->SetApplyForProton(
      fApplyForProtonCmd->GetNewBoolValue(newValue));
  }
  else if (command == fSetCheckCmd) {
    fRegionsManager->SetCheck(fSetCheckCmd->GetNewBoolValue(newValue));
  }
  else if (command == fSetPrintCmd) {
    fRegionsManager->SetPrint(fSetPrintCmd->GetNewBoolValue(newValue), fIsFromG4Table);
  }
  else if (command == fSetSaveCmd) {
    fRegionsManager->SetSave(fSetSaveCmd->GetNewBoolValue(newValue), fIsFromG4Table);
  }
  else if (command == fSetLoadCmd) {
    fRegionsManager->SetLoad(fSetLoadCmd->GetNewBoolValue(newValue));
  }
  else if (command == fSetFromG4TableCmd) {
    fIsFromG4Table = fSetFromG4TableCmd->GetNewBoolValue(newValue);
  }
  else if (command == fSetFileNameCmd) {
    fRegionsManager->SetFileName(newValue);
  }
}
