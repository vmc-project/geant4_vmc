// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4RegionsMessenger.cxx
/// \brief Implementation of the TG4RegionsMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RegionsMessenger.h"
#include "TG4RegionsManager.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithABool.hh>

//_____________________________________________________________________________
TG4RegionsMessenger::TG4RegionsMessenger(TG4RegionsManager* runManager)
  : G4UImessenger(),
    fRegionsManager(runManager),
    fDirectory(0),
    fDumpRegionCmd(0)
{ 
/// Standard constructor

  fDirectory = new G4UIdirectory("/mcRegions/");
  fDirectory->SetGuidance("TGeant4 regions commands.");

  fDumpRegionCmd = new G4UIcmdWithAString("/mcRegions/dumpRegion", this);
  fDumpRegionCmd->SetGuidance("Dump characteristics of region ");
  fDumpRegionCmd->SetParameterName("LVname", false);
  fDumpRegionCmd->SetDefaultValue(" ");
  fDumpRegionCmd->AvailableForStates(G4State_Idle, G4State_EventProc);

  fSetRangePrecisionCmd = new G4UIcmdWithAnInteger("/mcRegions/setRangePrecision", this);
  fSetRangePrecisionCmd->SetGuidance("Set the precision for calculating ranges");
  fSetRangePrecisionCmd->SetParameterName("RangePrecision", false);
  fSetRangePrecisionCmd->AvailableForStates(G4State_PreInit, G4State_Init);

  fSetCheckCmd = new G4UIcmdWithABool("/mcRegions/check", this);
  fSetCheckCmd->SetGuidance("Switch on|off check if region properties are consistent");
  fSetCheckCmd->SetGuidance("with energy cuts defined in limits");
  fSetCheckCmd->SetParameterName("IsCheck", false);
  fSetCheckCmd->AvailableForStates(G4State_PreInit, G4State_Init);
}

//_____________________________________________________________________________
TG4RegionsMessenger::~TG4RegionsMessenger() 
{
/// Destructor

  delete fDirectory;
  delete fDumpRegionCmd;
  delete fSetRangePrecisionCmd;
  delete fSetCheckCmd;
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
    fRegionsManager->SetRangePrecision(fSetRangePrecisionCmd->GetNewIntValue(newValue)); 
  }
  else if (command == fSetCheckCmd) {
    fRegionsManager->SetCheck(fSetCheckCmd->GetNewBoolValue(newValue)); 
  }
}
