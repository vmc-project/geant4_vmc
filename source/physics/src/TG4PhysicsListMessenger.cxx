// $Id: TG4PhysicsListMessenger.cxx,v 1.4 2005/05/17 13:43:57 brun Exp $
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

//______________________________________________________________________________
TG4PhysicsListMessenger::TG4PhysicsListMessenger(
                                   TG4ModularPhysicsList* physicsList)
  : fPhysicsList(physicsList)
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
}

//______________________________________________________________________________
TG4PhysicsListMessenger::TG4PhysicsListMessenger(){
//
} 

//______________________________________________________________________________
TG4PhysicsListMessenger::TG4PhysicsListMessenger(
                                   const TG4PhysicsListMessenger& right) {
// 
  TG4Globals::Exception("TG4PhysicsListMessenger is protected from copying.");
}

//______________________________________________________________________________
TG4PhysicsListMessenger::~TG4PhysicsListMessenger() {
//

  delete fRangeCutCmd;
  delete fSetCerenkovMaxPhotonsCmd;
}

//
// operators
//

//______________________________________________________________________________
TG4PhysicsListMessenger& 
TG4PhysicsListMessenger::operator=(const TG4PhysicsListMessenger& right)
{
  // check assignement to self
  if (this == &right) return *this;

  TG4Globals::Exception("TG4PhysicsListMessenger is protected from assigning.");
    
  return *this;  
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
}
