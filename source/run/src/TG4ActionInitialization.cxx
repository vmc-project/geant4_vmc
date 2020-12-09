//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ActionInitialization.cxx
/// \brief Implementation of the TG4ActionInitialization class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ActionInitialization.h"
#include "TG4EventAction.h"
#include "TG4Globals.h"
#include "TG4RunAction.h"
#include "TG4RunConfiguration.h"
#include "TG4SpecialControlsV2.h"
#include "TG4SpecialStackingAction.h"
#include "TG4SteppingAction.h"
#include "TG4TrackManager.h"
#include "TG4TrackingAction.h"
#include "TGeant4.h"

#include <RVersion.h>
#include <TInterpreter.h>
#include <TROOT.h>
#include <TVirtualMCApplication.h>

#include <G4Types.hh>
#include <G4Threading.hh>

//_____________________________________________________________________________
TG4ActionInitialization::TG4ActionInitialization(
  TG4RunConfiguration* runConfiguration)
  : G4VUserActionInitialization(),
    fRunConfiguration(runConfiguration),
    fEventAction(0),
    fTrackingAction(0),
    fSteppingAction(0),
    fStackingAction(0)
{
  /// Standard constructor

  fEventAction = fRunConfiguration->CreateEventAction();
  fTrackingAction = fRunConfiguration->CreateTrackingAction();
  fSteppingAction = fRunConfiguration->CreateSteppingAction();
  fStackingAction = fRunConfiguration->CreateStackingAction();
}

//_____________________________________________________________________________
TG4ActionInitialization::~TG4ActionInitialization()
{
  /// Destructor

#ifdef G4MULTITHREADED
  if (G4Threading::G4GetThreadId() != -2 &&
      fRunConfiguration->IsMTApplication()) {
    // Delete actions not registered to G4RunManager
    // (created on master in MT)
    delete fEventAction;
    delete fTrackingAction;
    delete fSteppingAction;
    delete fStackingAction;
  }
#endif
}

//
// public methods
//

//_____________________________________________________________________________
void TG4ActionInitialization::BuildForMaster() const
{
  /// Build user actions defined on master thread

  // G4cout << "TG4ActionInitialization::BuildForMaster " << this << G4endl;

  G4UserRunAction* runAction = fRunConfiguration->CreateRunAction();
  if (runAction) SetUserAction(runAction);

  // G4cout << "TG4ActionInitialization::BuildForMaster end " << G4endl;
}

//_____________________________________________________________________________
void TG4ActionInitialization::Build() const
{
  /// Build user actions defined on worker threads

  // G4cout << "TG4ActionInitialization::Build "  << this << G4endl;

  G4UserEventAction* eventAction = 0;
  TG4TrackingAction* trackingAction = 0;
  TG4SteppingAction* steppingAction = 0;
  G4UserStackingAction* stackingAction = 0;

  // create MC and MCApplication worker instances
#ifdef G4MULTITHREADED
  if (G4Threading::IsWorkerThread()) {
    if (!TGeant4::MasterApplicationInstance()->CloneForWorker()) {
      // Give an exception if user application does not implement
      // CloneForWorker as is run in MT
      TG4Globals::Exception("TG4ActionInitialization", "Build",
        "Failed to clone user application." + TG4Globals::Endl() +
          "TVirtualMCApplication::CloneForWorker() must be overriden in user "
          "code" +
          TG4Globals::Endl() + "in order to run in multi-threading mode.");
    }
    TGeant4::MasterInstance()->CloneForWorker();
    TVirtualMCApplication::Instance()->InitForWorker(); // deprecated
#if (ROOT_VERSION_CODE >= ROOT_VERSION(6, 10, 0))
    TVirtualMCApplication::Instance()->InitOnWorker(); // new
#endif

    // Create actions on workers
    eventAction = fRunConfiguration->CreateEventAction();
    trackingAction = fRunConfiguration->CreateTrackingAction();
    steppingAction = fRunConfiguration->CreateSteppingAction();
    stackingAction = fRunConfiguration->CreateStackingAction();

    // Pass properties from objects created on master
    if (eventAction) {
      TG4EventAction* tg4EventAction =
        dynamic_cast<TG4EventAction*>(eventAction);
      if (tg4EventAction) {
        TG4EventAction* masterEventAction =
          static_cast<TG4EventAction*>(fEventAction);
        tg4EventAction->SetPrintMemory(masterEventAction->GetPrintMemory());
        tg4EventAction->SetSaveRandomStatus(
          masterEventAction->GetSaveRandomStatus());
        tg4EventAction->VerboseLevel(masterEventAction->VerboseLevel());
      }
    }

    if (trackingAction) {
      trackingAction->SetNewVerboseLevel(fTrackingAction->GetNewVerboseLevel());
      trackingAction->SetNewVerboseTrackID(
        fTrackingAction->GetNewVerboseTrackID());
      trackingAction->GetTrackManager()->SetTrackSaveControl(
        fTrackingAction->GetTrackManager()->GetTrackSaveControl());
      trackingAction->GetTrackManager()->SetSaveDynamicCharge(
        fTrackingAction->GetTrackManager()->GetSaveDynamicCharge());
      trackingAction->VerboseLevel(fTrackingAction->VerboseLevel());
      trackingAction->GetTrackManager()->VerboseLevel(
        fTrackingAction->GetTrackManager()->VerboseLevel());
    }

    if (steppingAction) {
      steppingAction->SetLoopVerboseLevel(
        fSteppingAction->GetLoopVerboseLevel());
      steppingAction->SetMaxNofSteps(fSteppingAction->GetMaxNofSteps());
    }

    if (stackingAction) {
      TG4SpecialStackingAction* tg4StackingAction =
        dynamic_cast<TG4SpecialStackingAction*>(stackingAction);
      if (tg4StackingAction) {
        TG4SpecialStackingAction* masterStackingAction =
          static_cast<TG4SpecialStackingAction*>(fStackingAction);
        tg4StackingAction->SetSkipNeutrino(
          masterStackingAction->GetSkipNeutrino());
        tg4StackingAction->VerboseLevel(masterStackingAction->VerboseLevel());
      }
    }
  }
#endif

  // In sequential build just get the actions created in constructor
  if (!eventAction) eventAction = fEventAction;
  if (!trackingAction) trackingAction = fTrackingAction;
  if (!steppingAction) steppingAction = fSteppingAction;
  if (!stackingAction) stackingAction = fStackingAction;

  // Create actions (without messengers) which were not yet created
  // and set them to G4RunManager

  SetUserAction(fRunConfiguration->CreatePrimaryGenerator());

  G4UserRunAction* runAction = fRunConfiguration->CreateRunAction();
  if (runAction) SetUserAction(runAction);

  // Set already created actions (with messengers) to G4RunManager
  if (eventAction) SetUserAction(eventAction);
  if (trackingAction) SetUserAction(trackingAction);
  if (steppingAction) SetUserAction(steppingAction);
  if (stackingAction) SetUserAction(stackingAction);
  if (runAction) SetUserAction(runAction);

  // Special controls action
  //
  if (fRunConfiguration->IsSpecialControls()) {
    G4cout << "### TG4SpecialControlsV2 constructed" << G4endl;
    TG4SpecialControlsV2* specialControls = new TG4SpecialControlsV2();
    // special controls are deleted in TG4SteppingAction

    trackingAction->SetSpecialControls(specialControls);
    steppingAction->SetSpecialControls(specialControls);
  }
  // G4cout << "TG4ActionInitialization::Build done " << this << G4endl;
}
