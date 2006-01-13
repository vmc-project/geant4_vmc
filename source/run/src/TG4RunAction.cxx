// $Id: TG4RunAction.cxx,v 1.2 2004/11/10 11:39:28 brun Exp $
// Category: run
//
// Class TG4RunAction
// ------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include <G4Timer.hh>
   // in order to avoid the odd dependency for the
   // times system function this include must be the first

#include "TG4RunAction.h"
#include "TG4Globals.h"

#include <G4Run.hh>

//_____________________________________________________________________________
TG4RunAction::TG4RunAction()
  : TG4Verbose("runAction"),
    fRunID(-1) 
{
//
  fTimer = new G4Timer;
}

//_____________________________________________________________________________
TG4RunAction::TG4RunAction(const TG4RunAction& right) 
  : TG4Verbose("runAction") {
//
  TG4Globals::Exception("TG4RunAction is protected from copying.");
}

//_____________________________________________________________________________
TG4RunAction::~TG4RunAction() {
//
  delete fTimer;
}

//
// operators
//

//_____________________________________________________________________________
TG4RunAction& TG4RunAction::operator=(const TG4RunAction &right)
{
  // check assignement to self
  if (this == &right) return *this;
  
  TG4Globals::Exception("TG4RunAction is protected from assigning.");

  return *this;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4RunAction::BeginOfRunAction(const G4Run* run)
{
/// Called by G4 kernel at the beginning of run.

  fRunID++;
  
  if (VerboseLevel() > 0) {
    G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
  }
    
  fTimer->Start();
}

//_____________________________________________________________________________
void TG4RunAction::EndOfRunAction(const G4Run* run)
{
/// Called by G4 kernel at the end of run.

  fTimer->Stop();

  if (VerboseLevel() > 0) {
    G4cout << "Time of this run:   " << *fTimer << G4endl;
    G4cout << "Number of events processed: " << run->GetNumberOfEvent() << G4endl;
  }    
}    
