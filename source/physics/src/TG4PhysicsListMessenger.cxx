// $Id: TG4PhysicsListMessenger.cxx,v 1.6 2006/12/12 16:21:16 brun Exp $
// Category: physics
//
// Class TG4PhysicsListMessenger
// --------------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4PhysicsListMessenger.h"
#include "TG4ModularPhysicsList.h"
#include "TG4Globals.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithAString.hh>

//______________________________________________________________________________
TG4PhysicsListMessenger::TG4PhysicsListMessenger(
                                   TG4ModularPhysicsList* physicsList)
  : G4UImessenger(),
    fPhysicsList(physicsList),
    fRangeCutCmd(0),
    fProcessActivationCmd(0),
    fSetCerenkovMaxPhotonsCmd(0),
    fSetStackPopperSelectionCmd(0)
{ 
//
  fRangeCutCmd
     = new G4UIcmdWithADouble("/mcPhysics/rangeCut", this);
  fRangeCutCmd->SetGuidance("Sets the global cut in range (in mm)");
  fRangeCutCmd->AvailableForStates(G4State_PreInit);
  
  fSetCerenkovMaxPhotonsCmd 
    = new G4UIcmdWithAnInteger("/mcPhysics/setCerenkovMaxPhotons",this);  
  fSetCerenkovMaxPhotonsCmd->SetGuidance("Sets maximum number of photons per step");
  fSetCerenkovMaxPhotonsCmd->SetParameterName("CerenkovMaxPhotons",false);
  fSetCerenkovMaxPhotonsCmd->SetRange("CerenkovMaxPhotons>=0");
  fSetCerenkovMaxPhotonsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  

  fSetStackPopperSelectionCmd 
    = new G4UIcmdWithAString("/mcPhysics/setStackPopperSelection",this);  
  fSetStackPopperSelectionCmd->SetGuidance("Selects particles for stack popper process");
  fSetStackPopperSelectionCmd->SetParameterName("StackPopperSelection",false);
  fSetStackPopperSelectionCmd->AvailableForStates(G4State_PreInit);  
}

//______________________________________________________________________________
TG4PhysicsListMessenger::~TG4PhysicsListMessenger() {
//

  delete fRangeCutCmd;
  delete fSetCerenkovMaxPhotonsCmd;
  delete fSetStackPopperSelectionCmd;
}

//
// public methods
//

//______________________________________________________________________________
void TG4PhysicsListMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{ 
/// Apply command to the associated object.

  if (command == fRangeCutCmd) {
    fPhysicsList
        ->SetRangeCut(fRangeCutCmd->GetNewDoubleValue(newValue));
  }  
  else if (command == fSetCerenkovMaxPhotonsCmd) {
    fPhysicsList
        ->SetMaxNumPhotonsPerStep(
             fSetCerenkovMaxPhotonsCmd->GetNewIntValue(newValue));
  }  
  else if (command == fSetStackPopperSelectionCmd) {
    G4cout << "TG4PhysicsListMessenger::SetNewValue " << newValue << G4endl;
    fPhysicsList
        ->SetStackPopperSelection(newValue);
  }  
}
