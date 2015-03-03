//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4FieldParametersMessenger.cxx
/// \brief Implementation of the TG4FieldParametersMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay


#include "TG4FieldParametersMessenger.h"
#include "TG4FieldParameters.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

//_____________________________________________________________________________
TG4FieldParametersMessenger::TG4FieldParametersMessenger(
                                   TG4FieldParameters* fieldParameters)
  : G4UImessenger(),
    fFieldParameters(fieldParameters),
    fDirectory(0),
    fEquationTypeCmd(0),
    fStepperTypeCmd(0),
    fSetStepMinimumCmd(0),
    fSetDeltaChordCmd(0),
    fSetDeltaOneStepCmd(0),
    fSetDeltaIntersectionCmd(0),
    fSetMinimumEpsilonStepCmd(0),
    fSetMaximumEpsilonStepCmd(0),
    fSetConstDistanceCmd(0),
    fPrintParametersCmd(0)
{
/// Standard constructor

  G4String directoryName = "/mcMagField/";
  if ( fieldParameters->GetVolumeName() != "" )  {
    directoryName.append(fieldParameters->GetVolumeName());
    directoryName.append("/");
  }
  fDirectory = new G4UIdirectory(directoryName);
  fDirectory->SetGuidance("Magnetic field control commands.");

  G4String commandName = directoryName;
  commandName.append("equationType");
  fEquationTypeCmd = new G4UIcmdWithAString(commandName, this);
  G4String guidance 
    = "Select type of the equation of motion of a particle in a field";
  fEquationTypeCmd->SetGuidance(guidance);
  fEquationTypeCmd->SetParameterName("EquationType", false);
  G4String candidates; 
  for ( G4int i=kMagUsualEqRhs; i<=kEqEMFieldWithEDM; i++ ) {
    EquationType et = (EquationType)i;
    candidates += TG4FieldParameters::EquationTypeName(et);
    candidates += " ";
  }  
  fEquationTypeCmd->SetCandidates(candidates);   
  fEquationTypeCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  commandName = directoryName;
  commandName.append("stepperType");
  fStepperTypeCmd = new G4UIcmdWithAString(commandName, this);
  guidance 
    = "Select type of the the integrator of particle's equation of motion in a field";
  fStepperTypeCmd->SetGuidance(guidance);
  fStepperTypeCmd->SetParameterName("StepperType", false);
  candidates = "";
  for ( G4int i=kCashKarpRKF45; i<=kRKG3Stepper; i++ ) {
    StepperType st = (StepperType)i;
    candidates += TG4FieldParameters::StepperTypeName(st);
    candidates += " ";
  }  
  fStepperTypeCmd->SetCandidates(candidates);   
  fStepperTypeCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  commandName = directoryName;
  commandName.append("setStepMinimum");
  fSetStepMinimumCmd = new G4UIcmdWithADoubleAndUnit(commandName, this);
  fSetStepMinimumCmd->SetGuidance("Set minimum step in G4ChordFinder");
  fSetStepMinimumCmd->SetParameterName("StepMinimum", false);
  fSetStepMinimumCmd->SetDefaultUnit("mm");
  fSetStepMinimumCmd->SetUnitCategory("Length");
  fSetStepMinimumCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  
  commandName = directoryName;
  commandName.append("setDeltaChord");
  fSetDeltaChordCmd = new G4UIcmdWithADoubleAndUnit(commandName, this);
  fSetDeltaChordCmd->SetGuidance("Set delta chord in G4ChordFinder");
  fSetDeltaChordCmd->SetParameterName("DeltaChord", false);
  fSetDeltaChordCmd->SetDefaultUnit("mm");
  fSetDeltaChordCmd->SetUnitCategory("Length");
  fSetDeltaChordCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  
  commandName = directoryName;
  commandName.append("setDeltaOneStep");
  fSetDeltaOneStepCmd = new G4UIcmdWithADoubleAndUnit(commandName, this);
  fSetDeltaOneStepCmd->SetGuidance("Set delta one step in global field manager");
  fSetDeltaOneStepCmd->SetParameterName("DeltaOneStep", false);
  fSetDeltaOneStepCmd->SetDefaultUnit("mm");
  fSetDeltaOneStepCmd->SetUnitCategory("Length");
  fSetDeltaOneStepCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  
  commandName = directoryName;
  commandName.append("setDeltaIntersection");
  fSetDeltaIntersectionCmd = new G4UIcmdWithADoubleAndUnit(commandName, this);
  fSetDeltaIntersectionCmd->SetGuidance("Set delta intersection in global field manager");
  fSetDeltaIntersectionCmd->SetParameterName("DeltaIntersection", false);
  fSetDeltaIntersectionCmd->SetDefaultUnit("mm");
  fSetDeltaIntersectionCmd->SetUnitCategory("Length");
  fSetDeltaIntersectionCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  
  commandName = directoryName;
  commandName.append("setMinimumEpsilonStep");
  fSetMinimumEpsilonStepCmd = new G4UIcmdWithADouble(commandName, this);
  fSetMinimumEpsilonStepCmd->SetGuidance("Set minimum epsilon step in global field manager");
  fSetMinimumEpsilonStepCmd->SetParameterName("MinimumEpsilonStep", false);
  fSetMinimumEpsilonStepCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  
  commandName = directoryName;
  commandName.append("setMaximumEpsilonStep");
  fSetMaximumEpsilonStepCmd = new G4UIcmdWithADouble(commandName, this);
  fSetMaximumEpsilonStepCmd->SetGuidance("Set maximum epsilon step in global field manager");
  fSetMaximumEpsilonStepCmd->SetParameterName("MaximumEpsilonStep", false);
  fSetMaximumEpsilonStepCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  
  commandName = directoryName;
  commandName.append("setConstDistance");
  fSetConstDistanceCmd = new G4UIcmdWithADoubleAndUnit(commandName, this);
  fSetConstDistanceCmd
    ->SetGuidance("Set the distance within which the field is considered constant.");
  fSetConstDistanceCmd
    ->SetGuidance("Non-zero value will trigger creating a cached magnetic field.");
  fSetConstDistanceCmd->SetParameterName("ConstDistance", false);
  fSetConstDistanceCmd->SetDefaultUnit("mm");
  fSetConstDistanceCmd->SetUnitCategory("Length");
  fSetConstDistanceCmd->SetRange("ConstDistance >= 0");
  fSetConstDistanceCmd->AvailableForStates(G4State_PreInit);

  commandName = directoryName;
  commandName.append("printParameters");
  fPrintParametersCmd = new G4UIcmdWithoutParameter(commandName, this);
  fPrintParametersCmd->SetGuidance("Prints all accuracy parameters.");
  fPrintParametersCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);   
}

//_____________________________________________________________________________
TG4FieldParametersMessenger::~TG4FieldParametersMessenger() 
{
/// Destructor

  delete fDirectory;
  delete fEquationTypeCmd;
  delete fStepperTypeCmd;
  delete fSetStepMinimumCmd;
  delete fSetDeltaChordCmd;
  delete fSetDeltaOneStepCmd;
  delete fSetDeltaIntersectionCmd;
  delete fSetMinimumEpsilonStepCmd;
  delete fSetMaximumEpsilonStepCmd;
  delete fSetConstDistanceCmd;
}

//
// public methods
//
  
//_____________________________________________________________________________
void TG4FieldParametersMessenger::SetNewValue(G4UIcommand* command, 
                                               G4String newValues)
{
/// Apply command to the associated object.

  if( command == fEquationTypeCmd ) { 
    for ( G4int i=kMagUsualEqRhs; i<=kEqEMFieldWithEDM; i++ ) {
      EquationType et = (EquationType)i;
      if ( newValues == TG4FieldParameters::EquationTypeName(et) ) {
        fFieldParameters->SetEquationType(et);
        break;
      }  
    }     
  }
  else if( command == fStepperTypeCmd ) { 
    for ( G4int i=kCashKarpRKF45; i<=kRKG3Stepper; i++ ) {
      StepperType st = (StepperType)i;
      if ( newValues == TG4FieldParameters::StepperTypeName(st) ) {
        fFieldParameters->SetStepperType(st);
        break;
      } 
    }   
  }
  if (command == fSetStepMinimumCmd) {  
    fFieldParameters
      ->SetStepMinimum(fSetStepMinimumCmd->GetNewDoubleValue(newValues)); 
  }
  if (command == fSetDeltaChordCmd) {  
    fFieldParameters
      ->SetDeltaChord(fSetDeltaChordCmd->GetNewDoubleValue(newValues)); 
  }
  if (command == fSetDeltaOneStepCmd) {  
    fFieldParameters
      ->SetDeltaOneStep(fSetDeltaOneStepCmd->GetNewDoubleValue(newValues)); 
  }
  if (command == fSetDeltaIntersectionCmd) {  
    fFieldParameters
      ->SetDeltaIntersection(fSetDeltaIntersectionCmd->GetNewDoubleValue(newValues)); 
  }
  if (command == fSetMinimumEpsilonStepCmd) {  
    fFieldParameters
      ->SetMinimumEpsilonStep(fSetMinimumEpsilonStepCmd->GetNewDoubleValue(newValues)); 
  }
  if (command == fSetMaximumEpsilonStepCmd) {  
    fFieldParameters
      ->SetMaximumEpsilonStep(fSetMaximumEpsilonStepCmd->GetNewDoubleValue(newValues)); 
  }
  if (command == fSetConstDistanceCmd) {
    fFieldParameters
      ->SetConstDistance(fSetConstDistanceCmd->GetNewDoubleValue(newValues));
  }
  else if (command == fPrintParametersCmd) {
    fFieldParameters->PrintParameters();
  }    
}
