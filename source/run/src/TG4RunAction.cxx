// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4RunAction.cxx
/// \brief Implementation of the TG4RunAction class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4Timer.hh>
   // in order to avoid the odd dependency for the
   // times system function this include must be the first

#include "TG4RunAction.h"
#include "TG4Globals.h"

#include <G4Run.hh>

//_____________________________________________________________________________
TG4RunAction::TG4RunAction()
  : G4UserRunAction(),
    TG4Verbose("runAction"),
    fTimer(0),
    fRunID(-1) 
{
/// Default constructor

  fTimer = new G4Timer;
}

//_____________________________________________________________________________
TG4RunAction::~TG4RunAction() 
{
/// Destructor

  delete fTimer;
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
