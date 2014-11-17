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
#include "TG4RunAction.h"
#include "TG4Globals.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithABool.hh>

//_____________________________________________________________________________
TG4RunActionMessenger::TG4RunActionMessenger(TG4RunAction* runAction)
  : G4UImessenger(),
    fRunAction(runAction),
    fRunDirectory(0),
    fSaveRandomStatusCmd(0),
    fReadRandomStatusCmd(0),
    fRandomStatusFileCmd(0)
{ 
/// Standard constructor

  fRunDirectory = new G4UIdirectory("/mcRun/");
  fRunDirectory->SetGuidance("TG4RunAction control commands.");

  fSaveRandomStatusCmd = new G4UIcmdWithABool("/mcRun/saveRandom", this);
  fSaveRandomStatusCmd->SetGuidance("Save random engine status in start of event");
  fSaveRandomStatusCmd->SetParameterName("SaveRandom", false);
  fSaveRandomStatusCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fReadRandomStatusCmd = new G4UIcmdWithABool("/mcRun/readRandom", this);
  fReadRandomStatusCmd->SetGuidance("Read random engine status from a file in start of event");
  fReadRandomStatusCmd->SetParameterName("ReadRandom", false);
  fReadRandomStatusCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fRandomStatusFileCmd = new G4UIcmdWithAString("/mcRun/setRandomFile", this);
  fRandomStatusFileCmd->SetGuidance("Set the name of the random engine status file to be read in");
  fRandomStatusFileCmd->SetParameterName("RandomFile", false);
  fRandomStatusFileCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

}

//_____________________________________________________________________________
TG4RunActionMessenger::~TG4RunActionMessenger() 
{
/// Destructor

  delete fRunDirectory;
  delete fSaveRandomStatusCmd;
  delete fReadRandomStatusCmd;
  delete fRandomStatusFileCmd;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4RunActionMessenger::SetNewValue(G4UIcommand* command, 
       G4String newValue)
{ 
/// Apply command to the associated object.

  if(command == fSaveRandomStatusCmd)
  { 
    fRunAction->SetSaveRandomStatus(fSaveRandomStatusCmd->GetNewBoolValue(newValue)); 
  }   
  else if(command == fReadRandomStatusCmd)
  { 
    fRunAction->SetReadRandomStatus(fReadRandomStatusCmd->GetNewBoolValue(newValue)); 
  }   
  else if(command == fRandomStatusFileCmd)
  { 
    fRunAction->SetRandomStatusFile(newValue); 
  }   
}
