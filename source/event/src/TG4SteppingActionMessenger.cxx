// $Id: TG4SteppingActionMessenger.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: event
//
// Author: I. Hrivnacova
//
// Class TG4SteppingActionMessenger
// --------------------------------
// See the class description in the header file.

#include "TG4SteppingActionMessenger.h"
#include "TG4SteppingAction.h"
#include "TG4Globals.h"

#include <G4UIcmdWithAnInteger.hh>

//_____________________________________________________________________________
TG4SteppingActionMessenger::TG4SteppingActionMessenger(
                               TG4SteppingAction* steppingAction)
  :fSteppingAction(steppingAction)
{
// 
  fLoopVerboseCmd = new G4UIcmdWithAnInteger("/mcTracking/loopVerbose", this);
  fLoopVerboseCmd
    ->SetGuidance("Set tracking verbose level for detected looping tracks.");
  fLoopVerboseCmd->SetParameterName("LoopVerboseLevel", true);
  fLoopVerboseCmd->SetDefaultValue(1);
  fLoopVerboseCmd->SetRange("LoopVerboseLevel >= 0 && LoopVerboseLevel <= 5");
  fLoopVerboseCmd->AvailableForStates(G4State_Idle);

  fMaxNofStepsCmd = new G4UIcmdWithAnInteger("/mcTracking/maxNofSteps", this);
  fMaxNofStepsCmd
    ->SetGuidance("Set maximum number of steps allowed.");
  fMaxNofStepsCmd->SetParameterName("MaxNofSteps", false);
  fMaxNofStepsCmd->SetRange("MaxNofSteps >= 0");
  fMaxNofStepsCmd->AvailableForStates(G4State_Idle);
}

//_____________________________________________________________________________
TG4SteppingActionMessenger::TG4SteppingActionMessenger() {
//
}

//_____________________________________________________________________________
TG4SteppingActionMessenger::TG4SteppingActionMessenger(
                                 const TG4SteppingActionMessenger& right) {
//				 
  TG4Globals::Exception(
    "TG4SteppingActionMessenger is protected from copying.");
}

//_____________________________________________________________________________
TG4SteppingActionMessenger::~TG4SteppingActionMessenger() {
//
  delete fLoopVerboseCmd;
  delete fMaxNofStepsCmd;
}

// operators

//_____________________________________________________________________________
TG4SteppingActionMessenger& 
TG4SteppingActionMessenger::operator=(const TG4SteppingActionMessenger &right)
{
  // check assignement to self
  if (this == &right) return *this;
  
  TG4Globals::Exception(
    "TG4SteppingActionMessenger is protected from assigning.");

  return *this;
}

// public methods

//_____________________________________________________________________________
void TG4SteppingActionMessenger::SetNewValue(G4UIcommand* command, 
       G4String newValue)
{ 
// Applies command to the associated object.
// ---

  if(command == fLoopVerboseCmd) { 
    fSteppingAction
      ->SetLoopVerboseLevel(fLoopVerboseCmd->GetNewIntValue(newValue)); 
  }   
  else if(command == fMaxNofStepsCmd) { 
    fSteppingAction
      ->SetMaxNofSteps(fMaxNofStepsCmd->GetNewIntValue(newValue)); 
  }   
}
