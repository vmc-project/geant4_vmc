// $Id: TG4SpecialStackingAction.cxx,v 1.11.6.2 2002/08/02 12:56:54 ivana Exp $
// Category: event
//
// Author: I. Hrivnacova
//
// Class TG4SpecialStackingAction
// -----------------------
// See the class description in the header file.

#include "TG4SpecialStackingAction.h"
#include "TG4TrackingAction.h"
#include "TG4Globals.h"

#include <G4Track.hh>
#include <G4TrackStack.hh>
#include <G4StackedTrack.hh>
#include <G4StackManager.hh>
#include <G4NeutrinoE.hh>
#include <G4NeutrinoMu.hh>
#include <G4NeutrinoTau.hh>
#include <G4AntiNeutrinoE.hh>
#include <G4AntiNeutrinoMu.hh>
#include <G4AntiNeutrinoTau.hh>

//_____________________________________________________________________________
TG4SpecialStackingAction::TG4SpecialStackingAction()
  : TG4Verbose("stackingAction",1),
    fStage(0), 
    fTrackingAction(0)
{
// 
}

//_____________________________________________________________________________
TG4SpecialStackingAction::TG4SpecialStackingAction(const TG4SpecialStackingAction& right) 
  : TG4Verbose("stackingAction") {
//
  TG4Globals::Exception("TG4SpecialStackingAction is protected from copying.");
}

//_____________________________________________________________________________
TG4SpecialStackingAction::~TG4SpecialStackingAction() {
// 
}

// operators

//_____________________________________________________________________________
TG4SpecialStackingAction& 
TG4SpecialStackingAction::operator=(const TG4SpecialStackingAction &right)
{
  // check assignement to self
  if (this == &right) return *this;
  
  TG4Globals::Exception("TG4SpecialStackingAction is protected from assigning.");

  return *this;
}

// public methods

//_____________________________________________________________________________
G4ClassificationOfNewTrack 
TG4SpecialStackingAction::ClassifyNewTrack(const G4Track* track)
{
// Classifies the new track.
// ---

  G4ClassificationOfNewTrack classification;
  if (fStage == 0) { 
    // move all primaries to PrimaryStack
    classification = fPostpone;

    // save primary particle info
    // (secondary particles are stored 
    //  by AlTrackingAction::PreUserTrackingAction() method)
    if (fTrackingAction->GetSavePrimaries())
      fTrackingAction->TrackToStack(track);
  }  
  else {
     // exclude neutrinos
    G4ParticleDefinition* particle = track->GetDefinition();
    if( particle == G4NeutrinoE::NeutrinoEDefinition() ||
        particle == G4NeutrinoMu::NeutrinoMuDefinition() ||
        particle == G4NeutrinoTau::NeutrinoTauDefinition() ||
        particle == G4AntiNeutrinoE::AntiNeutrinoEDefinition() ||
        particle == G4AntiNeutrinoMu::AntiNeutrinoMuDefinition() ||
        particle == G4AntiNeutrinoTau::AntiNeutrinoTauDefinition()) {

        return fKill;	 
     }	

     G4int parentID = track->GetParentID();
     if (parentID ==0) { 
        return fUrgent; 
     }
     else { 
        return fWaiting; 
     }
  }
  return classification;
}

//_____________________________________________________________________________
void TG4SpecialStackingAction::NewStage()
{
// Called by G4 kernel at the new stage of stacking.
// ---

  fStage++;
  
  if (VerboseLevel() > 1) {
    G4cout << "TG4SpecialStackingAction::NewStage " << fStage 
           << " has been started." << G4endl;
  }

  if (stackManager->GetNUrgentTrack() == 0 &&
      stackManager->GetNPostponedTrack() != 0 ) {
      
      stackManager->TransferOneStackedTrack(fPostpone, fUrgent);
  }
}
    
//_____________________________________________________________________________
void TG4SpecialStackingAction::PrepareNewEvent()
{
// Called by G4 kernel at the beginning of event.
// ---

  fStage = 0;
  fTrackingAction = TG4TrackingAction::Instance();
}


