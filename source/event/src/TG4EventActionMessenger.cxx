// $Id: TG4EventActionMessenger.cxx,v 1.3 2004/11/10 11:39:27 brun Exp $
// Category: event
//
// Class TG4EventActionMessenger
// -----------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4EventActionMessenger.h"
#include "TG4EventAction.h"
#include "TG4Globals.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>

//_____________________________________________________________________________
TG4EventActionMessenger::TG4EventActionMessenger(TG4EventAction* eventAction)
  : G4UImessenger(),
    fEventAction(eventAction),
    fEventDirectory(0),
    fDrawTracksCmd(0)
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
  fDrawTracksCmd->AvailableForStates(G4State_Idle);
}

//_____________________________________________________________________________
TG4EventActionMessenger::~TG4EventActionMessenger() {
//
  delete fEventDirectory;
  delete fDrawTracksCmd;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4EventActionMessenger::SetNewValue(G4UIcommand* command, 
       G4String newValue)
{ 
/// Apply command to the associated object.

  if(command == fDrawTracksCmd)
  { 
    fEventAction->SetDrawFlag(newValue); 
  }   
}
