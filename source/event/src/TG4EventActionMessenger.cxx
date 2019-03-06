//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4EventActionMessenger.cxx
/// \brief Implementation of the TG4EventActionMessenger class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4EventActionMessenger.h"
#include "TG4EventAction.h"
#include "TG4Globals.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithABool.hh>

//_____________________________________________________________________________
TG4EventActionMessenger::TG4EventActionMessenger(TG4EventAction* eventAction)
  : G4UImessenger(),
    fEventAction(eventAction),
    fEventDirectory(0),
    fPrintMemoryCmd(0),
    fSaveRandomStatusCmd(0)
{
/// Standard constructor

  fEventDirectory = new G4UIdirectory("/mcEvent/");
  fEventDirectory->SetGuidance("TG4EventAction control commands.");

  fPrintMemoryCmd = new G4UIcmdWithABool("/mcEvent/printMemory", this);
  fPrintMemoryCmd->SetGuidance("Print memory usage at the end of event");
  fPrintMemoryCmd->SetParameterName("PrintMemory", false);
  fPrintMemoryCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fSaveRandomStatusCmd = new G4UIcmdWithABool("/mcEvent/saveRandom", this);
  fSaveRandomStatusCmd->SetGuidance("Save random engine status for each event");
  fSaveRandomStatusCmd->SetParameterName("SaveRandom", false);
  fSaveRandomStatusCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
}

//_____________________________________________________________________________
TG4EventActionMessenger::~TG4EventActionMessenger()
{
/// Destructor

  delete fEventDirectory;
  delete fPrintMemoryCmd;
  delete fSaveRandomStatusCmd;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4EventActionMessenger::SetNewValue(G4UIcommand* command,
       G4String newValue)
{
/// Apply command to the associated object.

  if ( command == fPrintMemoryCmd )
  {
    fEventAction->SetPrintMemory(fPrintMemoryCmd->GetNewBoolValue(newValue));
  }
  else if ( command == fSaveRandomStatusCmd )
  {
    fEventAction->SetSaveRandomStatus(fSaveRandomStatusCmd->GetNewBoolValue(newValue));
  }
}
