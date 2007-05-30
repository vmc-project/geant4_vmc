// $Id: TG4TrackingAction.cxx,v 1.12 2007/05/22 12:26:42 brun Exp $
// Category: event
//
// Class TG4TrackingAction
// -----------------------
// See the class description in the header file.
//
// Author: I.Hrivnacova

#include "TG4TrackingAction.h"
#include "TG4TrackInformation.h"
#include "TG4TrackManager.h"
#include "TG4StepManager.h"
#include "TG4PhysicsManager.h"
#include "TG4ParticlesManager.h"
#include "TG4StackPopper.h"
#include "TG4SensitiveDetector.h"
#include "TG4SDServices.h"
#include "TG4Globals.h"

#include <TVirtualMC.h>
#include <TVirtualMCApplication.h>
#include <TVirtualMC.h>
#include <TMCProcess.h>

#include <G4TrackVector.hh>
#include <G4TrackingManager.hh>
#include <G4UImanager.hh>

// static data members
TG4TrackingAction* TG4TrackingAction::fgInstance = 0;

//_____________________________________________________________________________
TG4TrackingAction::TG4TrackingAction()     
  : G4UserTrackingAction(),
    TG4Verbose("trackingAction",2),    
    fMessenger(this),
    fTrackManager(0),
    fPrimaryTrackID(0),
    fCurrentTrackID(0),
    fSaveSecondaries(true),
    fNewVerboseLevel(0),
    fNewVerboseTrackID(-1)
{
//
  if (fgInstance) { 
    TG4Globals::Exception(
      "TG4TrackingAction", "TG4TrackingAction", 
      "Cannot create two instances of singleton.");
  }

  fTrackManager = new TG4TrackManager();

  fgInstance = this;
}

//_____________________________________________________________________________
TG4TrackingAction::~TG4TrackingAction() {
//
  fgInstance = 0;
  
  delete fTrackManager;
}

//
// private methods
//

//_____________________________________________________________________________
void TG4TrackingAction::UserProcessHits(const G4Track* track)
{
/// Let sensitive detector process the vertex step
/// (this ensures compatibility with G3 that
/// makes first step of zero length)

  TG4StepManager* stepManager = TG4StepManager::Instance();
  G4VPhysicalVolume* pv = stepManager->GetCurrentPhysicalVolume();
  
  if (!pv) {
    // was exception
    TG4Globals::Warning(
      "TG4TrackingAction", "UserProcessHits", "Cannot locate track vertex."); 
    return;
  }  
  
#ifdef MCDEBUG
  TG4SensitiveDetector* tsd
    = TG4SDServices::Instance()
         ->GetSensitiveDetector(
              pv->GetLogicalVolume()->GetSensitiveDetector());

  if (tsd) tsd->UserProcessHits((G4Track*)track, 0);
#else
  TG4SensitiveDetector* tsd
    = (TG4SensitiveDetector*) pv->GetLogicalVolume()->GetSensitiveDetector();

  if (tsd) tsd->UserProcessHits((G4Track*)track, 0);
#endif  
}

//_____________________________________________________________________________
void TG4TrackingAction::Verbose() const
{
/// Print primary track ID with a frequence defined
/// by verbose level.

  if (VerboseLevel() == 3) { 
    G4cout << "$$$ Primary track " << fPrimaryTrackID << G4endl;
  } 
  else if (VerboseLevel() == 2 &&  fPrimaryTrackID % 10 == 0 ) {
    G4cout << "$$$ Primary track " << fPrimaryTrackID  << G4endl;
  } 
  else if (VerboseLevel() == 1 &&  fPrimaryTrackID % 100 == 0 ) {
    G4cout << "$$$ Primary track " << fPrimaryTrackID  << G4endl;
  } 
}

//
// public methods
//

//_____________________________________________________________________________
void TG4TrackingAction::PrepareNewEvent()
{
/// Called by G4 kernel at the beginning of event.

  // set g4 stepping manager pointer
  TG4StepManager* stepManager = TG4StepManager::Instance();
  stepManager->SetSteppingManager(fpTrackingManager->GetSteppingManager());
  
  fTrackManager->SetG4TrackingManager(fpTrackingManager);

  if (fSaveSecondaries)
    fTrackManager->SetNofTracks(0);
  else  
    fTrackManager->SetNofTracks(gMC->GetStack()->GetNtrack());
    
  fCurrentTrackID = 0;
}

//_____________________________________________________________________________
void TG4TrackingAction::PreUserTrackingAction(const G4Track* track)
{
/// Called by G4 kernel before starting tracking.

  // do not call this function more than once
  if ( track->GetTrackID() == fCurrentTrackID ) return;
  
  // keep this track number for the check above
  fCurrentTrackID = track->GetTrackID();

  // finish previous primary track first
  if (track->GetParentID() == 0) {  
    FinishPrimaryTrack();
  }  

  // reset stack popper (if activated
  if ( TG4StackPopper::Instance() )
    TG4StackPopper::Instance()->Reset();

  // set step manager status
  TG4StepManager* stepManager = TG4StepManager::Instance();
  stepManager->SetStep((G4Track*)track, kVertex);
  
  // set track information
  G4int trackId = fTrackManager->SetTrackInformation(track);
  gMC->GetStack()->SetCurrentTrack(trackId);

  if (track->GetParentID() == 0) {  
    fPrimaryTrackID = track->GetTrackID();
    
    // begin this primary track
    TVirtualMCApplication::Instance()->BeginPrimary();
  }
  else {
    // save secondary particles info 
    if (  fSaveSecondaries && ! fTrackManager->IsUserTrack(track) ) {
      fTrackManager->TrackToStack(track);

      // Notify a stack popper (if activated) about saving this secondary
      if ( TG4StackPopper::Instance() )
        TG4StackPopper::Instance()->Notify();
    }  
  }
    

  // verbose
  if ( track->GetTrackID() == fNewVerboseTrackID) {
    G4String command = "/tracking/verbose ";
    TG4Globals::AppendNumberToString(command, fNewVerboseLevel);
    G4UImanager::GetUIpointer()->ApplyCommand(command);
  }    

  // VMC application pre track action
  TVirtualMCApplication::Instance()->PreTrack();

  // call pre-tracking action of derived class
  PreTrackingAction(track);

  // Let sensitive detector process vertex step
  UserProcessHits(track);
}

//_____________________________________________________________________________
void TG4TrackingAction::PostUserTrackingAction(const G4Track* track)
{
/// Called by G4 kernel after finishing tracking.

  // set parent track particle index to the secondary tracks 
  fTrackManager->SetParentToTrackInformation(track);

  // VMC application post track action
  TVirtualMCApplication::Instance()->PostTrack();

  // call post-tracking action of derived class
  PostTrackingAction(track);
}

//_____________________________________________________________________________
void TG4TrackingAction::FinishPrimaryTrack()
{
/// Call VMC application to finish a primary track.                          \n
/// !! This method has to be also called from the EventAction::EndOfEventAction() 
/// for storing the last primary track of the current event.
// --- 

  if (fPrimaryTrackID>0) {

    // set special step manager status
    // not in both stepping, vertex stage
    TG4StepManager* stepManager = TG4StepManager::Instance();
    G4Track* noTrack = 0;
    stepManager->SetStep(noTrack, kVertex);
   
    // verbose
    Verbose();

    // VMC application finish primary track       
    TVirtualMCApplication::Instance()->FinishPrimary();
  }
  
  fPrimaryTrackID = 0;
}  

//_____________________________________________________________________________
void TG4TrackingAction::SetNewVerboseLevel(G4int level)
{ 
/// Set the new verbose level that will be set when the track with 
/// specified track ID (fNewVerboseTrackID) starts tracking.

  fNewVerboseLevel = level;  
}

//_____________________________________________________________________________
void TG4TrackingAction::SetNewVerboseTrackID(G4int trackID)
{ 
/// Set the trackID for which the new verbose level (fNewVerboseLevel)
/// will be applied.

  fNewVerboseTrackID = trackID; 
}

//_____________________________________________________________________________
void TG4TrackingAction::SetSaveSecondaries(G4bool saveSecondaries) 
{ 
/// Set control for saving secondaries in the VMC stack and pass it
/// to TG4 stack manager

  fSaveSecondaries = saveSecondaries; 
  fTrackManager->SetSaveSecondaries(saveSecondaries);
}


