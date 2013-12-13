// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
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
#include "TGeant4.h"
#include "TG4Globals.h"
#include "TG4RegionsManager.h"

#include <G4Run.hh>
#include <Randomize.hh>
#include <G4UImanager.hh>
#include "G4AutoLock.hh"

#include <TObjArray.h>

// mutex in a file scope

namespace {
  //Mutex to lock master application when merging data
  G4Mutex mergeMutex = G4MUTEX_INITIALIZER;
}  

const G4String TG4RunAction::fgkDefaultRandomStatusFile = "currentRun.rndm";

//_____________________________________________________________________________
TG4RunAction::TG4RunAction()
  : G4UserRunAction(),
    TG4Verbose("runAction"),
    fMessenger(this),
    fCrossSectionManager(), 
    fTimer(0),
    fRunID(-1),
    fSaveRandomStatus(false),
    fReadRandomStatus(false),
    fRandomStatusFile(fgkDefaultRandomStatusFile)
{
/// Default constructor

  G4cout << "TG4RunAction::TG4RunAction " << this << G4endl;

  fTimer = new G4Timer;
}

//_____________________________________________________________________________
TG4RunAction::~TG4RunAction() 
{
/// Destructor

  G4cout << "TG4RunAction::~TG4RunAction " << this << G4endl;

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
    
  if ( TG4RegionsManager::Instance() ) {
    if ( TG4RegionsManager::Instance()->IsCheck() ) {
      TG4RegionsManager::Instance()->CheckRegions();
    }  
    if ( TG4RegionsManager::Instance()->IsPrint() ) {
      TG4RegionsManager::Instance()->PrintRegions();
    }  
  }  

  // activate random number status
  if ( fSaveRandomStatus) {
    G4UImanager::GetUIpointer()->ApplyCommand("/random/setSavingFlag true");
    if ( VerboseLevel() > 0)
      G4cout << "Activated saving random status " << G4endl;  
      CLHEP::HepRandom::showEngineStatus();
      G4cout << G4endl;  
  }    
    
  if ( fReadRandomStatus) {
    // restore event random number status from a file
    CLHEP::HepRandom::showEngineStatus();
    G4String command("/random/resetEngineFrom ");
    command += fRandomStatusFile;
    G4UImanager::GetUIpointer()->ApplyCommand(command.data());
    if ( VerboseLevel() > 0) {
      G4cout << "Resetting random engine from " << fRandomStatusFile << G4endl;  
      CLHEP::HepRandom::showEngineStatus();
      G4cout << G4endl;  
    }         
  }  

  fTimer->Start();
}

//_____________________________________________________________________________
void TG4RunAction::EndOfRunAction(const G4Run* run)
{
/// Called by G4 kernel at the end of run.


  // Merge user application data
  G4AutoLock lm(&mergeMutex);
  TGeant4::MasterApplicationInstance()->Merge(TVirtualMCApplication::Instance());
  lm.unlock();

  if ( fCrossSectionManager.IsMakeHistograms() ) {
    fCrossSectionManager.MakeHistograms();
  }  

  fTimer->Stop();

  if (VerboseLevel() > 0) {
    G4cout << "Time of this run:   " << *fTimer << G4endl;
    G4cout << "Number of events processed: " << run->GetNumberOfEvent() << G4endl;
  }    
}    
