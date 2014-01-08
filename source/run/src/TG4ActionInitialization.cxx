// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4ActionInitialization.cxx
/// \brief Implementation of the TG4ActionInitialization class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ActionInitialization.h"
#include "TG4RunConfiguration.h"
#include "TG4RunAction.h"
#include "TG4EventAction.h"
#include "TG4TrackingAction.h"
#include "TG4SteppingAction.h"
#include "TG4SpecialStackingAction.h"
#include "TG4SpecialControlsV2.h"
#include "TGeant4.h"

#include <G4Threading.hh>

#include <TROOT.h>
#include <TInterpreter.h>
#include <TVirtualMCApplication.h>

//_____________________________________________________________________________
TG4ActionInitialization::TG4ActionInitialization(
                                  TG4RunConfiguration* runConfiguration)
  : G4VUserActionInitialization(),
    fRunConfiguration(runConfiguration),
    fSpecialControls(0)
{
/// Standard constructor

  if ( fRunConfiguration->IsSpecialControls() ) {
    // add test if both tracking action and stepping action
    fSpecialControls = new TG4SpecialControlsV2();
  }  
}

//_____________________________________________________________________________
TG4ActionInitialization::~TG4ActionInitialization()
{
/// Destructor

  delete fSpecialControls;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4ActionInitialization::BuildForMaster() const
{
/// Build user actions defined on master thread

  //G4cout << "TG4ActionInitialization::BuildForMaster " << this << G4endl;

  G4UserRunAction* runAction = fRunConfiguration->CreateRunAction();
  if ( runAction ) SetUserAction(runAction);  

  //G4cout << "TG4ActionInitialization::BuildForMaster end " << G4endl;
}

//_____________________________________________________________________________
void TG4ActionInitialization::Build() const
{
/// Build user actions defined on worker threads

 //G4cout << "TG4ActionInitialization::Build "  << this << G4endl;

  // create MC and MCApplication worker instances
  if ( G4Threading::IsWorkerThread() ) {
    TGeant4::MasterApplicationInstance()->CloneForWorker();
    TGeant4::MasterInstance()->CloneForWorker();
    TVirtualMCApplication::Instance()->InitForWorker();
  }  

  SetUserAction(fRunConfiguration->CreatePrimaryGenerator());

  G4UserRunAction* runAction = fRunConfiguration->CreateRunAction();
  if ( runAction ) SetUserAction(runAction);  

  G4UserEventAction* eventAction = fRunConfiguration->CreateEventAction();
  if ( eventAction ) SetUserAction(eventAction); 
    
  TG4TrackingAction* trackingAction = fRunConfiguration->CreateTrackingAction();
  if ( trackingAction) SetUserAction(trackingAction); 
    
  TG4SteppingAction* steppingAction = fRunConfiguration->CreateSteppingAction();
  if ( steppingAction) SetUserAction(steppingAction);

  G4UserStackingAction* stackingAction = fRunConfiguration->CreateStackingAction(); 
  if ( stackingAction) SetUserAction(stackingAction);

  // Special controls action
  //
/*
  if ( fSpecialControls ) {
    trackingAction->SetSpecialControls(fSpecialControls);
    steppingAction->SetSpecialControls(fSpecialControls);
  }
*/
  //G4cout << "TG4ActionInitialization::Build done " << this << G4endl;
}
