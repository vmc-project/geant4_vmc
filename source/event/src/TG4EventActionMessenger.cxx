// $Id: TG4EventActionMessenger.cxx,v 1.1.1.1 2002/06/16 15:57:34 hristov Exp $
// Category: event
//
// Author: I. Hrivnacova
//
// Class TG4EventActionMessenger
// -----------------------------
// See the class description in the header file.

#include "TG4EventActionMessenger.h"
#include "TG4EventAction.h"
#include "TG4Globals.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>

//_____________________________________________________________________________
TG4EventActionMessenger::TG4EventActionMessenger(TG4EventAction* eventAction)
  :fEventAction(eventAction)
{ 
//
  fEventDirectory = new G4UIdirectory("/mcEvent/");
  fEventDirectory->SetGuidance("TG4EventAction control commands.");

  fDrawTracksCmd = new G4UIcmdWithAString("/mcEvent/drawTracks", this);
  fDrawTracksCmd->SetGuidance("Draw the tracks in the event");
  fDrawTracksCmd->SetGuidance("  Choice : NONE, CHARGED(default), ALL");
  fDrawTracksCmd->SetParameterName("Choice", true);
  fDrawTracksCmd->SetDefaultValue("CHARGED");
  fDrawTracksCmd->SetCandidates("NONE CHARGED ALL");
  fDrawTracksCmd->AvailableForStates(Idle);
}

//_____________________________________________________________________________
TG4EventActionMessenger::TG4EventActionMessenger(){
//
}

//_____________________________________________________________________________
TG4EventActionMessenger::TG4EventActionMessenger(
                                 const TG4EventActionMessenger& right) {
//				 
  TG4Globals::Exception("TG4EventActionMessenger is protected from copying.");
}

//_____________________________________________________________________________
TG4EventActionMessenger::~TG4EventActionMessenger() {
//
  delete fEventDirectory;
  delete fDrawTracksCmd;
}

// operators

//_____________________________________________________________________________
TG4EventActionMessenger& 
TG4EventActionMessenger::operator=(const TG4EventActionMessenger &right)
{
  // check assignement to self
  if (this == &right) return *this;
  
  TG4Globals::Exception("TG4EventActionMessenger is protected from assigning.");

  return *this;
}

// public methods

//_____________________________________________________________________________
void TG4EventActionMessenger::SetNewValue(G4UIcommand* command, 
       G4String newValue)
{ 
// Applies command to the associated object.
// ---

  if(command == fDrawTracksCmd)
  { 
    fEventAction->SetDrawFlag(newValue); 
  }   
}
