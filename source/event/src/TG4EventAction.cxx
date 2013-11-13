// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4EventAction.cxx
/// \brief Implementation of the TG4EventAction class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4EventAction.h"
#include "TG4TrackingAction.h"
#include "TG4ParticlesManager.h"
#include "TG4TrackManager.h"
#include "TG4StateManager.h"
#include "TG4Globals.h"

#include <G4Event.hh>
#include <G4TrajectoryContainer.hh>
#include <G4Trajectory.hh>
#include <G4VVisManager.hh>
#include <G4UImanager.hh>
#include <Randomize.hh>

#include <TVirtualMC.h>
#include <TVirtualMCStack.h>
#include <TVirtualMCApplication.h>
#include <TSystem.h>

#include <math.h>

//_____________________________________________________________________________
TG4EventAction::TG4EventAction()
  : TG4Verbose("eventAction"),
    fMessenger(this),
    fTimer(),
    fSaveRandomStatus(false)
{
/// Default constructor
}

//_____________________________________________________________________________
TG4EventAction::~TG4EventAction() 
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void TG4EventAction::BeginOfEventAction(const G4Event* event)
{
/// Called by G4 kernel at the beginning of event.

  // reset the tracks counters
  if (TG4TrackingAction::Instance()) 
    TG4TrackingAction::Instance()->PrepareNewEvent();   
    
  // fill primary particles in VMC stack if stack is empty
  if ( gMC->GetStack()->GetNtrack() == 0 ) {
    if (VerboseLevel() > 0)
      G4cout << "Filling VMC stack with primaries" << G4endl;
    
    for (G4int iv=0; iv<event->GetNumberOfPrimaryVertex(); iv++) {
      G4PrimaryVertex* vertex = event->GetPrimaryVertex(iv);
      
      for (G4int ip=0; ip<vertex->GetNumberOfParticle(); ip++) {
        G4PrimaryParticle* particle = vertex->GetPrimary(ip);
        TG4TrackManager::Instance()->PrimaryToStack(vertex, particle);
      }        
    }
  } 
  
  // save the event random number status per event
  if ( fSaveRandomStatus) {
    G4UImanager::GetUIpointer()->ApplyCommand("/random/saveThisEvent");
    if (VerboseLevel() > 0)
      G4cout << "Saving random status: " << G4endl;  
      CLHEP::HepRandom::showEngineStatus();
      G4cout << G4endl;  
  }    

  if (VerboseLevel() > 0) {
    G4cout << ">>> Event " << event->GetEventID() << G4endl;
    fTimer.Start();
  }  
}

//_____________________________________________________________________________
void TG4EventAction::EndOfEventAction(const G4Event* event)
{
/// Called by G4 kernel at the end of event.

  // finish the last primary track of the current event
  TG4TrackingAction* trackingAction = TG4TrackingAction::Instance();
  if (trackingAction) trackingAction->FinishPrimaryTrack();   

  if (VerboseLevel() > 1) {
    G4cout << G4endl;
    G4cout << ">>> End of Event " << event->GetEventID() << G4endl;
  }

  if (VerboseLevel() > 2) {
    G4int nofPrimaryTracks 
      = gMC->GetStack()->GetNprimary();
    G4int nofSavedTracks 
      = gMC->GetStack()->GetNtrack();
   
    G4cout  << "    " << nofPrimaryTracks << 
               " primary tracks processed." << G4endl;
    G4cout  << "    " << nofSavedTracks << 
               " tracks saved." << G4endl;

    G4int nofAllTracks = TG4TrackManager::Instance()->GetNofTracks();
    G4cout  << "    " << nofAllTracks << 
                  " all tracks processed." << G4endl;
  }               

  // VMC application finish event
  TVirtualMCApplication::Instance()->FinishEvent();    
  TG4StateManager::Instance()->SetNewState(kNotInApplication);

  if (VerboseLevel() > 1) {
    // print time
    fTimer.Stop();
    fTimer.Print();
  }  

  if ( fPrintMemory ) {
    ProcInfo_t procInfo;
    gSystem->GetProcInfo(&procInfo);
    G4cout << "Current memory usage: resident " 
           << procInfo.fMemResident << ", virtual " << procInfo.fMemVirtual << G4endl;
  }         
}
