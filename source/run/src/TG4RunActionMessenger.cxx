//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4RunActionMessenger.cxx
/// \brief Implementation of the TG4RunActionMessenger class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RunActionMessenger.h"
#include "TG4Globals.h"
#include "TG4RunAction.h"

#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIdirectory.hh>

//_____________________________________________________________________________
TG4RunActionMessenger::TG4RunActionMessenger(TG4RunAction* runAction)
  : G4UImessenger(),
    fRunAction(runAction),
    fRunDirectory(0),
    fSaveRandomStatusCmd(0),
    fReadRandomStatusCmd(0),
    fRandomStatusFileCmd(0),
    fSetLooperThresholdWarningEnergyCmd(0),
    fSetLooperThresholdImportantEnergyCmd(0),
    fSetNumberOfLooperThresholdTrialsCmd(0)

{
  /// Standard constructor

  fRunDirectory = new G4UIdirectory("/mcRun/");
  fRunDirectory->SetGuidance("TG4RunAction control commands.");

  fSaveRandomStatusCmd = new G4UIcmdWithABool("/mcRun/saveRandom", this);
  fSaveRandomStatusCmd->SetGuidance(
    "Save random engine status in start of event");
  fSaveRandomStatusCmd->SetParameterName("SaveRandom", false);
  fSaveRandomStatusCmd->AvailableForStates(
    G4State_PreInit, G4State_Init, G4State_Idle);

  fReadRandomStatusCmd = new G4UIcmdWithABool("/mcRun/readRandom", this);
  fReadRandomStatusCmd->SetGuidance(
    "Read random engine status from a file in start of event");
  fReadRandomStatusCmd->SetParameterName("ReadRandom", false);
  fReadRandomStatusCmd->AvailableForStates(
    G4State_PreInit, G4State_Init, G4State_Idle);

  fRandomStatusFileCmd = new G4UIcmdWithAString("/mcRun/setRandomFile", this);
  fRandomStatusFileCmd->SetGuidance(
    "Set the name of the random engine status file to be read in");
  fRandomStatusFileCmd->SetParameterName("RandomFile", false);
  fRandomStatusFileCmd->AvailableForStates(
    G4State_PreInit, G4State_Init, G4State_Idle);

  fSetLooperThresholdWarningEnergyCmd = new G4UIcmdWithADoubleAndUnit(
    "/mcRun/setLooperThresholdWarningEnergy", this);
  G4String guidance =
    "Set energy threshold for warnings about killing looping tracks";
  fSetLooperThresholdWarningEnergyCmd->SetGuidance(guidance);
  fSetLooperThresholdWarningEnergyCmd->SetUnitCategory("Energy");
  fSetLooperThresholdWarningEnergyCmd->SetParameterName(
    "LooperThresholdWarningEnergy", false);
  fSetLooperThresholdWarningEnergyCmd->AvailableForStates(G4State_PreInit);

  fSetLooperThresholdImportantEnergyCmd = new G4UIcmdWithADoubleAndUnit(
    "/mcRun/setLooperThresholdImportantEnergy", this);
  guidance = "Set important energy threshold: ";
  fSetLooperThresholdImportantEnergyCmd->SetGuidance(guidance);
  guidance = "it enables tracks above its value to survive a chosen number of ‘tracking’ steps";
  fSetLooperThresholdImportantEnergyCmd->SetGuidance(guidance);
  fSetLooperThresholdImportantEnergyCmd->SetUnitCategory("Energy");
  fSetLooperThresholdImportantEnergyCmd->SetParameterName(
    "LooperThresholdImportantEnergy", false);
  fSetLooperThresholdImportantEnergyCmd->AvailableForStates(G4State_PreInit);

  fSetNumberOfLooperThresholdTrialsCmd = new G4UIcmdWithAnInteger(
    "/mcRun/setNumberOfLooperThresholdTrials", this);
  guidance = "Set number of trials to propagate a looping track";
  fSetNumberOfLooperThresholdTrialsCmd->SetGuidance(guidance);
  fSetNumberOfLooperThresholdTrialsCmd->SetParameterName(
    "NumberOfLooperThresholdTrials", false);
  fSetNumberOfLooperThresholdTrialsCmd->AvailableForStates(G4State_PreInit);
}

//_____________________________________________________________________________
TG4RunActionMessenger::~TG4RunActionMessenger()
{
  /// Destructor

  delete fRunDirectory;
  delete fSaveRandomStatusCmd;
  delete fReadRandomStatusCmd;
  delete fRandomStatusFileCmd;
  delete fSetLooperThresholdWarningEnergyCmd;
  delete fSetLooperThresholdImportantEnergyCmd;
  delete fSetNumberOfLooperThresholdTrialsCmd;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4RunActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  /// Apply command to the associated object.

  if (command == fSaveRandomStatusCmd) {
    fRunAction->SetSaveRandomStatus(
      fSaveRandomStatusCmd->GetNewBoolValue(newValue));
  }
  else if (command == fReadRandomStatusCmd) {
    fRunAction->SetReadRandomStatus(
      fReadRandomStatusCmd->GetNewBoolValue(newValue));
  }
  else if (command == fRandomStatusFileCmd) {
    fRunAction->SetRandomStatusFile(newValue);
  }
  else if (command == fSetLooperThresholdWarningEnergyCmd) {
    G4double value = G4UIcommand::ConvertToDouble(newValue);
    fRunAction->SetThresholdWarningEnergy(value);
  }
  else if (command == fSetLooperThresholdImportantEnergyCmd) {
    G4double value = G4UIcommand::ConvertToDouble(newValue);
    fRunAction->SetThresholdImportantEnergy(value);
  }
  else if (command == fSetNumberOfLooperThresholdTrialsCmd) {
    G4double value = G4UIcommand::ConvertToInt(newValue);
    fRunAction->SetNumberOfThresholdTrials(value);
  }
}
