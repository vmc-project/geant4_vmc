// $Id: TG4EventAction.cxx,v 1.1.1.1 2002/06/16 15:57:34 hristov Exp $
// Category: event
//
// Author: I. Hrivnacova
//
// Class TG4EventAction
// ---------------------
// See the class description in the header file.

#include <G4Timer.hh>
   // in order to avoid the odd dependency for the
   // times system function this include must be the first

#include "TG4EventAction.h"
#include "TG4TrackingAction.h"
#include "TG4Globals.h"

#include <G4Event.hh>
#include <G4TrajectoryContainer.hh>
#include <G4Trajectory.hh>
#include <G4VVisManager.hh>
#include <G4UImanager.hh>

#include <TVirtualMC.h>
#include <TVirtualMCStack.h>
#include <TVirtualMCApplication.h>

//_____________________________________________________________________________
TG4EventAction::TG4EventAction()
  : TG4Verbose("eventAction"),
    fMessenger(this),
    fDrawFlag("CHARGED")
{
//
  fTimer = new G4Timer();
}

//_____________________________________________________________________________
TG4EventAction::TG4EventAction(const TG4EventAction& right)
  : TG4Verbose(""),
    fMessenger(this) {
//
  TG4Globals::Exception("TG4EventAction is protected from copying.");
}

//_____________________________________________________________________________
TG4EventAction::~TG4EventAction() {
//
  delete fTimer;
}

// operators

//_____________________________________________________________________________
TG4EventAction& TG4EventAction::operator=(const TG4EventAction &right)
{
  // check assignement to self
  if (this == &right) return *this;
  
  TG4Globals::Exception("TG4EventAction is protected from assigning.");

  return *this;
}

// private methods

//_____________________________________________________________________________
void TG4EventAction::DisplayEvent(const G4Event* event) const
{
// Draws trajectories.
// ---

  if (G4VVisManager::GetConcreteInstance()) {

    // trajectories processing
    G4TrajectoryContainer* trajectoryContainer 
      = event->GetTrajectoryContainer();

    G4int nofTrajectories = 0;
    if (trajectoryContainer)
      nofTrajectories = trajectoryContainer->entries(); 
  
    if (VerboseLevel() > 0 && nofTrajectories > 0) {
      G4cout << "    " << nofTrajectories; 
      G4cout << " trajectories stored." << G4endl;
    }  

    for (G4int i=0; i<nofTrajectories; i++) { 
      G4VTrajectory* vtrajectory = (*(event->GetTrajectoryContainer()))[i];
      G4Trajectory* trajectory = dynamic_cast<G4Trajectory*>(vtrajectory);
      if (!trajectory) {
        TG4Globals::Exception(
	  "TG4EventAction::DisplayEvent: Unknown trajectory type.");
      }
      if ( (fDrawFlag == "ALL") ||
          ((fDrawFlag == "CHARGED") && (trajectory->GetCharge() != 0.))){
	 trajectory->DrawTrajectory(50); 
	    // the argument number defines the size of the step points
	    // use 2000 to make step points well visible
      }	
    }      
  }
}

// public methods

//_____________________________________________________________________________
void TG4EventAction::BeginOfEventAction(const G4Event* event)
{
// Called by G4 kernel at the beginning of event.
// ---

  // reset the tracks counters
  if (TG4TrackingAction::Instance()) 
    TG4TrackingAction::Instance()->PrepareNewEvent();   

  if (VerboseLevel() > 0) {
    G4cout << ">>> Event " << event->GetEventID() << G4endl;
  }  

  fTimer->Start();
}

//_____________________________________________________________________________
void TG4EventAction::EndOfEventAction(const G4Event* event)
{
// Called by G4 kernel at the end of event.
// ---

  // finish the last primary track of the current event
  TG4TrackingAction* trackingAction = TG4TrackingAction::Instance();
  if (trackingAction) trackingAction->FinishPrimaryTrack();   

  if (VerboseLevel() > 0) {
    G4cout << G4endl;
    G4cout << ">>> End of Event " << event->GetEventID() << G4endl;
  }

  if (VerboseLevel() > 1) {
    G4int nofPrimaryTracks = gMC->GetStack()->GetNprimary();
    G4int nofSavedTracks = gMC->GetStack()->GetNtrack();
   
    G4cout  << "    " << nofPrimaryTracks << 
               " primary tracks processed." << G4endl;
    G4cout  << "    " << nofSavedTracks << 
               " tracks saved." << G4endl;
    if (trackingAction) {
       G4int nofAllTracks = trackingAction->GetNofTracks();
       G4cout  << "    " << nofAllTracks << 
                  " all tracks processed." << G4endl;
    }	  
  }	       

  // display event
  DisplayEvent(event);

  // aliroot finish event
  TVirtualMCApplication::Instance()->FinishEvent();    

  if (VerboseLevel() > 1) {
    // print time
    fTimer->Stop();
    G4cout << "Time of this event: " << *fTimer << G4endl;
  }  
 }
