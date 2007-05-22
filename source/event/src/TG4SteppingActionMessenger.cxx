// $Id: TG4SteppingActionMessenger.cxx,v 1.4 2006/12/12 16:21:15 brun Exp $
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
#include <G4UIcmdWithABool.hh>

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

  fSaveSecondariesCmd = new G4UIcmdWithABool("/mcTracking/saveSecondariesInStep", this);
  fSaveSecondariesCmd->SetGuidance("Option to save secondaries in the stack during stepping.");
  fSaveSecondariesCmd->SetGuidance("By default this option is false.");
  fSaveSecondariesCmd->SetParameterName("SaveSecondariesInStep", false);
  fSaveSecondariesCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
}

//_____________________________________________________________________________
TG4SteppingActionMessenger::~TG4SteppingActionMessenger() {
//
  delete fLoopVerboseCmd;
  delete fMaxNofStepsCmd;
  delete fSaveSecondariesCmd;
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
  else if(command == fSaveSecondariesCmd) { 
    fSteppingAction
      ->SetSaveSecondaries(fSaveSecondariesCmd->GetNewBoolValue(newValue)); 
  }   
}
