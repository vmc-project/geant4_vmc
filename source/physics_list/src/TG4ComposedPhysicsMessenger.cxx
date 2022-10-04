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

#include "TG4ComposedPhysicsMessenger.h"
#include "TG4ComposedPhysicsList.h"
#include "TG4G3PhysicsManager.h"
#include "TG4GeometryServices.h"
#include "TG4PhysicsManager.h"
#include "TG4ProcessMap.h"

#include <G4AnalysisUtilities.hh>
#include <G4HadronicProcessStore.hh>
#include <G4NeutronHPManager.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIdirectory.hh>
#include <G4UnitsTable.hh>

//______________________________________________________________________________
TG4ComposedPhysicsMessenger::TG4ComposedPhysicsMessenger(
  TG4ComposedPhysicsList* physicsList)
  : G4UImessenger(),
    fPhysicsList(physicsList),
    fRangeGammaCutCmd(0),
    fRangeElectronCutCmd(0),
    fRangePositronCutCmd(0),
    fRangeProtonCutCmd(0),
    fRangeAllCutCmd(0),
    fSetGammaToMuonsCrossSectionFactorCmd(0),
    fPrintProcessMapCmd(0),
    fPrintVolumeLimitsCmd(0),
    fPrintGlobalCutsCmd(0),
    fPrintGlobalControlsCmd(0),
    fG4NeutronHPVerboseCmd(0),
    fG4HadronicProcessStoreVerboseCmd(0)
{
  /// Standard constructor

  fDirectory = new G4UIdirectory("/mcPhysics/");
  fDirectory->SetGuidance("TGeant4 physics control commands.");

  fRangeGammaCutCmd =
    new G4UIcmdWithADoubleAndUnit("/mcPhysics/rangeCutForGamma", this);
  fRangeGammaCutCmd->SetGuidance("Set range cut for gamma");
  fRangeGammaCutCmd->SetParameterName("GammaCut", false);
  fRangeGammaCutCmd->SetUnitCategory("Length");
  fRangeGammaCutCmd->SetRange("GammaCut>0.0");
  fRangeGammaCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fRangeElectronCutCmd =
    new G4UIcmdWithADoubleAndUnit("/mcPhysics/rangeCutForElectron", this);
  fRangeElectronCutCmd->SetGuidance("Set range cut for electron.");
  fRangeElectronCutCmd->SetParameterName("ElectronCut", false);
  fRangeElectronCutCmd->SetUnitCategory("Length");
  fRangeElectronCutCmd->SetRange("ElectronCut>0.0");
  fRangeElectronCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fRangePositronCutCmd =
    new G4UIcmdWithADoubleAndUnit("/mcPhysics/rangeCutForPositron", this);
  fRangePositronCutCmd->SetGuidance("Set range cut for positron.");
  fRangePositronCutCmd->SetParameterName("PositronCut", false);
  fRangePositronCutCmd->SetUnitCategory("Length");
  fRangePositronCutCmd->SetRange("PositronCut>0.0");
  fRangePositronCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fRangeProtonCutCmd =
    new G4UIcmdWithADoubleAndUnit("/mcPhysics/rangeCutForProton", this);
  fRangeProtonCutCmd->SetGuidance("Set range cut for proton.");
  fRangeProtonCutCmd->SetParameterName("ProtonCut", false);
  fRangeProtonCutCmd->SetUnitCategory("Length");
  fRangeProtonCutCmd->SetRange("ProtonCut>0.0");
  fRangeProtonCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fRangeAllCutCmd = new G4UIcmdWithADoubleAndUnit("/mcPhysics/rangeCuts", this);
  fRangeAllCutCmd->SetGuidance("Set range cut for all.");
  fRangeAllCutCmd->SetParameterName("AllCut", false);
  fRangeAllCutCmd->SetUnitCategory("Length");
  fRangeAllCutCmd->SetRange("AllCut>0.0");
  fRangeAllCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  CreateProductionCutsTableEnergyRangeCmd();

  fSetGammaToMuonsCrossSectionFactorCmd = new G4UIcmdWithADouble(
    "/mcPhysics/setGammaToMuonsCrossSectionFactor", this);
  G4String guidance = "Set gamma to muons cross section factor";
  fSetGammaToMuonsCrossSectionFactorCmd->SetGuidance(guidance);
  fSetGammaToMuonsCrossSectionFactorCmd->SetParameterName(
    "GammaToMuonsCrossSectionFactor", false);
  fSetGammaToMuonsCrossSectionFactorCmd->AvailableForStates(G4State_PreInit);

  fSetEnableHyperNucleiCmd = new G4UIcmdWithABool(
    "/mcPhysics/setEnableHyperNuclei", this);
  fSetEnableHyperNucleiCmd->SetGuidance("Enable hyper-nuclei physics processes");
  fSetEnableHyperNucleiCmd->SetParameterName("EnableHyperNuclei", false);
  fSetEnableHyperNucleiCmd->AvailableForStates(G4State_PreInit);

  fPrintAllProcessesCmd =
    new G4UIcmdWithoutParameter("/mcPhysics/printAllProcess", this);
  fPrintAllProcessesCmd->SetGuidance(
    "Print names for all instantiated processes.");
  fPrintAllProcessesCmd->AvailableForStates(G4State_Idle);

  fDumpAllProcessesCmd =
    new G4UIcmdWithoutParameter("/mcPhysics/dumpAllProcess", this);
  fDumpAllProcessesCmd->SetGuidance("Dump all instantiated processes.");
  fDumpAllProcessesCmd->AvailableForStates(G4State_Idle);

  fPrintProcessMapCmd =
    new G4UIcmdWithoutParameter("/mcPhysics/printProcessMap", this);
  fPrintProcessMapCmd->SetGuidance(
    "Print mapping of G4 processes to VMC process codes and (G3-like) controls.");
  fPrintProcessMapCmd->AvailableForStates(G4State_Idle);

  fPrintVolumeLimitsCmd =
    new G4UIcmdWithAString("/mcPhysics/printVolumeLimits", this);
  fPrintVolumeLimitsCmd->SetGuidance(
    "Print the limits set for the specified volume.");
  fPrintVolumeLimitsCmd->SetParameterName("PrintVolumeLimits", false);
  fPrintVolumeLimitsCmd->AvailableForStates(G4State_Idle);

  fPrintGlobalCutsCmd =
    new G4UIcmdWithoutParameter("/mcPhysics/printGlobalCuts", this);
  fPrintGlobalCutsCmd->SetGuidance("Print global VMC (G3-like) cuts.");
  fPrintGlobalCutsCmd->AvailableForStates(G4State_Idle);

  fPrintGlobalControlsCmd =
    new G4UIcmdWithoutParameter("/mcPhysics/printGlobalControls", this);
  fPrintGlobalControlsCmd->SetGuidance(
    "Print global VMC (G3-like) process controls.");
  fPrintGlobalControlsCmd->AvailableForStates(G4State_Idle);

  fG4NeutronHPVerboseCmd =
    new G4UIcmdWithAnInteger("/mcPhysics/g4NeutronHPVerbose", this);
  fG4NeutronHPVerboseCmd->SetGuidance("Set G4NeutronHPManager verbose level");
  fG4NeutronHPVerboseCmd->SetParameterName("NeutronHPVerbose", false);
  fG4NeutronHPVerboseCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fG4HadronicProcessStoreVerboseCmd =
    new G4UIcmdWithAnInteger("/mcPhysics/g4HadronicProcessStoreVerbose", this);
  fG4HadronicProcessStoreVerboseCmd->SetGuidance(
    "Set G4NeutronHPManager verbose level");
  fG4HadronicProcessStoreVerboseCmd->SetParameterName(
    "HadronicProcessStoreVerbose", false);
  fG4HadronicProcessStoreVerboseCmd->AvailableForStates(
    G4State_PreInit, G4State_Idle);

  fUseLowLooperThresholdsCmd =
    new G4UIcmdWithoutParameter("/mcPhysics/useLowLooperThresholds", this);
  fUseLowLooperThresholdsCmd->SetGuidance(
    "Set lower thresholds for killing particles looping in magnetic field.");
  fUseLowLooperThresholdsCmd->AvailableForStates(G4State_PreInit);

  fUseHighLooperThresholdsCmd =
    new G4UIcmdWithoutParameter("/mcPhysics/useHighLooperThresholds", this);
  fUseHighLooperThresholdsCmd->SetGuidance(
    "Set higher thresholds for killing particles looping in magnetic field.");
  fUseHighLooperThresholdsCmd->AvailableForStates(G4State_PreInit);
}

//______________________________________________________________________________
TG4ComposedPhysicsMessenger::~TG4ComposedPhysicsMessenger()
{
  /// Destructor

  delete fDirectory;
  delete fRangeGammaCutCmd;
  delete fRangeElectronCutCmd;
  delete fRangePositronCutCmd;
  delete fRangeProtonCutCmd;
  delete fRangeAllCutCmd;
  delete fProductionCutsTableEnergyRangeCmd;
  delete fSetGammaToMuonsCrossSectionFactorCmd;
  delete fSetEnableHyperNucleiCmd;
  delete fPrintAllProcessesCmd;
  delete fDumpAllProcessesCmd;
  delete fPrintProcessMapCmd;
  delete fPrintVolumeLimitsCmd;
  delete fPrintGlobalCutsCmd;
  delete fPrintGlobalControlsCmd;
  delete fG4NeutronHPVerboseCmd;
  delete fG4HadronicProcessStoreVerboseCmd;
  delete fUseLowLooperThresholdsCmd;
  delete fUseHighLooperThresholdsCmd;
}

//
// private methods
//

//______________________________________________________________________________
void TG4ComposedPhysicsMessenger::CreateProductionCutsTableEnergyRangeCmd()
{
  /// Create poductionCutsTableEnergyRange command

  G4UIparameter* minEnergy = new G4UIparameter("minEnergy", 'd', false);
  minEnergy->SetGuidance("Production cuts table minimum energy.");

  G4UIparameter* minEnergyUnit = new G4UIparameter("minEnergyUnit", 's', false);
  minEnergyUnit->SetGuidance("Production cuts table min energy unit.");

  G4UIparameter* maxEnergy = new G4UIparameter("maxEnergy", 'd', false);
  maxEnergy->SetGuidance("Production cuts table maximum energy.");

  G4UIparameter* maxEnergyUnit = new G4UIparameter("maxEnergyUnit", 's', false);
  maxEnergyUnit->SetGuidance("Production cuts table max energy unit.");

  fProductionCutsTableEnergyRangeCmd =
    new G4UIcommand("/mcPhysics/productionCutsTableEnergyRange", this);
  fProductionCutsTableEnergyRangeCmd->SetGuidance(
    "Set the production cuts table energy range.");
  fProductionCutsTableEnergyRangeCmd->SetParameter(minEnergy);
  fProductionCutsTableEnergyRangeCmd->SetParameter(minEnergyUnit);
  fProductionCutsTableEnergyRangeCmd->SetParameter(maxEnergy);
  fProductionCutsTableEnergyRangeCmd->SetParameter(maxEnergyUnit);
  fProductionCutsTableEnergyRangeCmd->AvailableForStates(G4State_PreInit);
}

//
// public methods
//

//______________________________________________________________________________
void TG4ComposedPhysicsMessenger::SetNewValue(
  G4UIcommand* command, G4String newValue)
{
  /// Apply command to the associated object.

  if (command == fRangeGammaCutCmd) {
    G4double cut = fRangeGammaCutCmd->GetNewDoubleValue(newValue);
    TG4PhysicsManager::Instance()->SetCutForGamma(cut);
    fPhysicsList->SetCutForGamma(cut);
  }
  else if (command == fRangeElectronCutCmd) {
    G4double cut = fRangeElectronCutCmd->GetNewDoubleValue(newValue);
    TG4PhysicsManager::Instance()->SetCutForElectron(cut);
    fPhysicsList->SetCutForElectron(cut);
  }
  else if (command == fRangePositronCutCmd) {
    G4double cut = fRangePositronCutCmd->GetNewDoubleValue(newValue);
    TG4PhysicsManager::Instance()->SetCutForPositron(cut);
    fPhysicsList->SetCutForPositron(cut);
  }
  else if (command == fRangeProtonCutCmd) {
    G4double cut = fRangeProtonCutCmd->GetNewDoubleValue(newValue);
    TG4PhysicsManager::Instance()->SetCutForProton(cut);
    fPhysicsList->SetCutForProton(cut);
  }
  else if (command == fRangeAllCutCmd) {
    G4double cut = fRangeAllCutCmd->GetNewDoubleValue(newValue);
    TG4PhysicsManager::Instance()->SetCutForGamma(cut);
    TG4PhysicsManager::Instance()->SetCutForElectron(cut);
    TG4PhysicsManager::Instance()->SetCutForPositron(cut);
    TG4PhysicsManager::Instance()->SetCutForProton(cut);
    fPhysicsList->SetCutForGamma(cut);
    fPhysicsList->SetCutForElectron(cut);
    fPhysicsList->SetCutForPositron(cut);
    fPhysicsList->SetCutForProton(cut);
  }
  else if (command == fProductionCutsTableEnergyRangeCmd) {
    // tokenize parameters in a vector
    std::vector<G4String> parameters;
    G4Analysis::Tokenize(newValue, parameters);

    G4int counter = 0;
    G4double minEnergy = G4UIcommand::ConvertToDouble(parameters[counter++]);
    G4double minEUnit = G4UnitDefinition::GetValueOf(parameters[counter++]);
    G4double maxEnergy = G4UIcommand::ConvertToDouble(parameters[counter++]);
    G4double maxEUnit = G4UnitDefinition::GetValueOf(parameters[counter++]);
    fPhysicsList->SetProductionCutsTableEnergyRange(
      minEnergy * minEUnit, maxEnergy * maxEUnit);
  }
  else if (command == fSetGammaToMuonsCrossSectionFactorCmd) {
    G4double value = G4UIcommand::ConvertToDouble(newValue);
    fPhysicsList->SetGammaToMuonsCrossSectionFactor(value);
  }
  else if (command == fSetEnableHyperNucleiCmd) {
    fPhysicsList->SetEnableHyperNuclei(G4UIcommand::ConvertToBool(newValue));
  }
  else if (command == fPrintAllProcessesCmd) {
    fPhysicsList->PrintAllProcesses();
  }
  else if (command == fDumpAllProcessesCmd) {
    fPhysicsList->DumpAllProcesses();
  }
  else if (command == fPrintProcessMapCmd) {
    TG4ProcessMap::Instance()->PrintAll();
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
  else if (command == fG4NeutronHPVerboseCmd) {
    G4NeutronHPManager::GetInstance()->SetVerboseLevel(
      fG4NeutronHPVerboseCmd->GetNewIntValue(newValue));
  }
  else if (command == fG4HadronicProcessStoreVerboseCmd) {
    G4HadronicProcessStore::Instance()->SetVerbose(
      fG4HadronicProcessStoreVerboseCmd->GetNewIntValue(newValue));
  }
  else if (command == fUseLowLooperThresholdsCmd) {
    fPhysicsList->SetLooperThresholdsLevel(0);
  }
  else if (command == fUseHighLooperThresholdsCmd) {
    fPhysicsList->SetLooperThresholdsLevel(2);
  }
}
