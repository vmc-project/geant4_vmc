//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4SpecialStackingAction.cxx
/// \brief Implementation of the TG4SpecialStackingAction class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SpecialStackingAction.h"
#include "TG4Globals.h"

#include <G4Track.hh>
#include <G4TrackStack.hh>
#include <G4StackedTrack.hh>
#include <G4StackManager.hh>

#include <TPDGCode.h>

//_____________________________________________________________________________
TG4SpecialStackingAction::TG4SpecialStackingAction()
  : G4UserStackingAction(),
    TG4Verbose("stackingAction",1),
    fMessenger(this),
    fStage(0),
    fSkipNeutrino(false),
    fWaitPrimary(true)
{
/// Default constructor

  G4cout << "### TG4SpecialStackingAction activated" << G4endl;
}

//_____________________________________________________________________________
TG4SpecialStackingAction::~TG4SpecialStackingAction()
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
G4ClassificationOfNewTrack
TG4SpecialStackingAction::ClassifyNewTrack(const G4Track* track)
{
/// Classify the new track.

  if (fWaitPrimary && fStage == 0) {
    // move all primaries to PrimaryStack
    return fPostpone;
  }

  if ( fSkipNeutrino ) {
    G4int pdgCode = track->GetDefinition()->GetPDGEncoding();
    if  ( pdgCode ==  kNuE || pdgCode ==  kNuEBar ||
          pdgCode ==  kNuMu || pdgCode == kNuMuBar ||
	  pdgCode ==  kNuTau || pdgCode == kNuTauBar ) {

      return fKill;
    }
  }

  return fUrgent;
}

//_____________________________________________________________________________
void TG4SpecialStackingAction::NewStage()
{
/// Called by G4 kernel at the new stage of stacking.

  fStage++;

  if (VerboseLevel() > 1) {
    G4cout << "TG4SpecialStackingAction::NewStage " << fStage
           << " has been started." << G4endl;
  }

  if (fWaitPrimary &&
      stackManager->GetNUrgentTrack() == 0 &&
      stackManager->GetNPostponedTrack() != 0 ) {

      stackManager->TransferOneStackedTrack(fPostpone, fUrgent);
  }
}

//_____________________________________________________________________________
void TG4SpecialStackingAction::PrepareNewEvent()
{
///  Since transition to G4SmartTrackStack in Geant4 9.6.x
///  secondaries are not ordered even when the special stacking is activated.

  fStage = 0;
}


