// $Id: TG4SteppingActionMessenger.cxx,v 1.3 2004/11/10 11:39:27 brun Exp $
// Category: event
//
// Class TG4SteppingActionMessenger
// --------------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4SteppingActionMessenger.h"
#include "TG4SteppingAction.h"
#include "TG4Globals.h"

#include <G4UIcmdWithAnInteger.hh>

//_____________________________________________________________________________
TG4SteppingActionMessenger::TG4SteppingActionMessenger(
                               TG4SteppingAction* steppingAction)
  : G4UImessenger(),
    fSteppingAction(steppingAction),
    fLoopVerboseCmd(0),
    fMaxNofStepsCmd(0)
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
TG4SteppingActionMessenger::~TG4SteppingActionMessenger() {
//
  delete fLoopVerboseCmd;
  delete fMaxNofStepsCmd;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4SteppingActionMessenger::SetNewValue(G4UIcommand* command, 
       G4String newValue)
{ 
/// Apply command to the associated object.

  if(command == fLoopVerboseCmd) { 
    fSteppingAction
      ->SetLoopVerboseLevel(fLoopVerboseCmd->GetNewIntValue(newValue)); 
  }   
  else if(command == fMaxNofStepsCmd) { 
    fSteppingAction
      ->SetMaxNofSteps(fMaxNofStepsCmd->GetNewIntValue(newValue)); 
  }   
}
