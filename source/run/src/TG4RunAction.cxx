//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4RunAction.cxx
/// \brief Implementation of the TG4RunAction class
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4Timer.hh>
// in order to avoid the odd dependency for the
// times system function this include must be the first

#include "TG4Globals.h"
#include "TG4VRegionsManager.h"
#include "TG4RunAction.h"
#include "TGeant4.h"

#include <G4AutoLock.hh>
#include <G4Electron.hh>
#include <G4Positron.hh>
#include <G4Run.hh>
#include <G4SystemOfUnits.hh>
#include <G4Threading.hh>
#include <G4Transportation.hh>
#include <G4Types.hh>
#include <G4UImanager.hh>
#include <Randomize.hh>

#include <TObjArray.h>

// mutex in a file scope

namespace
{

#ifdef G4MULTITHREADED
// Mutex to lock master application when merging data
G4Mutex mergeMutex = G4MUTEX_INITIALIZER;
#endif

G4Transportation* FindTransportation(
  const G4ParticleDefinition* particleDefinition)
{
  const auto* processManager = particleDefinition->GetProcessManager();
  return dynamic_cast<G4Transportation*>(
    processManager->GetProcess("Transportation"));
}

} // namespace

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
    fRandomStatusFile(fgkDefaultRandomStatusFile),
    fThresholdWarningEnergy(-1.0),
    fThresholdImportantEnergy(-1.0),
    fNumberOfThresholdTrials(0)
{
  /// Default constructor

  if (VerboseLevel() > 2)
    G4cout << "TG4RunAction::TG4RunAction " << this << G4endl;

  fTimer = new G4Timer;
}

//_____________________________________________________________________________
TG4RunAction::~TG4RunAction()
{
  /// Destructor

  if (VerboseLevel() > 2)
    G4cout << "TG4RunAction::~TG4RunAction " << this << G4endl;

  delete fTimer;
}

//
// private methods
//

//_____________________________________________________________________________
void TG4RunAction::ChangeLooperParameters(
  const G4ParticleDefinition* particleDefinition)
{
  // Nothing to be done if no parameters change
  if (fThresholdWarningEnergy < 0. && fThresholdImportantEnergy < 0. &&
      fNumberOfThresholdTrials == 0) {
    return;
  }

  auto* transportation = FindTransportation(particleDefinition);
  if (!transportation) {
    TG4Globals::Warning("TG4RunAction", "ChangeLooperParameters",
      "Cannot set parameters. Transportation process not found.");
    return;
  }

  if (fThresholdWarningEnergy >= 0.) {
    if (VerboseLevel() > 2) {
      G4cout << "ChangeLooperParameters: ThresholdWarningEnergy [keV] = "
             << fThresholdWarningEnergy / keV << G4endl;
    }
    transportation->SetThresholdWarningEnergy(fThresholdWarningEnergy);
  }

  if (fThresholdImportantEnergy >= 0.) {
    if (VerboseLevel() > 2) {
      G4cout << "ChangeLooperParameters: ThresholdImportantEnergy [keV] = "
             << fThresholdImportantEnergy / keV << G4endl;
    }
    transportation->SetThresholdImportantEnergy(fThresholdImportantEnergy);
  }

  if (fNumberOfThresholdTrials > 0) {
    if (VerboseLevel() > 2) {
      G4cout << "ChangeLooperParameters: NumberOfThresholdTrials = "
             << fNumberOfThresholdTrials << G4endl;
    }
    transportation->SetThresholdTrials(fNumberOfThresholdTrials);
  }
}

//_____________________________________________________________________________
void TG4RunAction::PrintLooperParameters() const
{
  /// Print looping thresholds parameters

  auto transportation = FindTransportation(G4Electron::Definition());
  if (transportation) {
    G4cout << "ThresholdWarningEnergy [MeV] = "
           << transportation->GetThresholdWarningEnergy() / MeV << G4endl;
    G4cout << "ThresholdImportantEnergy [MeV] = "
           << transportation->GetThresholdImportantEnergy() / MeV << G4endl;
    G4cout << "NumberOfThresholdTrials = "
           << transportation->GetThresholdTrials() << G4endl;
  }
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

  auto regionsManager = TG4VRegionsManager::Instance();
  if (regionsManager != nullptr) {
    if (regionsManager->IsCheck()) {
      regionsManager->CheckRegions();
    }
    if (regionsManager->IsPrint()) {
      regionsManager->PrintRegions(G4cout);
    }
    if (regionsManager->IsSave()) {
      regionsManager->SaveRegions();
    }
  }

  // activate random number status
  if (fSaveRandomStatus) {
    G4UImanager::GetUIpointer()->ApplyCommand("/random/setSavingFlag true");
    if (VerboseLevel() > 0)
      G4cout << "Activated saving random status " << G4endl;
    CLHEP::HepRandom::showEngineStatus();
    G4cout << G4endl;
  }

  if (fReadRandomStatus) {
    // restore event random number status from a file
    CLHEP::HepRandom::showEngineStatus();
    G4String command("/random/resetEngineFrom ");
    command += fRandomStatusFile;
    G4UImanager::GetUIpointer()->ApplyCommand(command.data());
    if (VerboseLevel() > 0) {
      G4cout << "Resetting random engine from " << fRandomStatusFile << G4endl;
      CLHEP::HepRandom::showEngineStatus();
      G4cout << G4endl;
    }
  }

  // set looper thresholds parameters
  // (only if defaults are overriden by user)
  ChangeLooperParameters(G4Electron::Electron());
  ChangeLooperParameters(G4Positron::Positron());

  // Print looping threshold parameters
  if (VerboseLevel() > 1) {
    PrintLooperParameters();
  }

  fTimer->Start();
}

//_____________________________________________________________________________
void TG4RunAction::EndOfRunAction(const G4Run* run)
{
  /// Called by G4 kernel at the end of run.

#ifdef G4MULTITHREADED
  if (! IsMaster()) {
    // Merge user application data collected on workers to master
    G4AutoLock lm(&mergeMutex);
    TGeant4::MasterApplicationInstance()->Merge(
      TVirtualMCApplication::Instance());
    lm.unlock();
  }
#endif

  if (fCrossSectionManager.IsMakeHistograms()) {
    fCrossSectionManager.MakeHistograms();
  }

  fTimer->Stop();

  if (VerboseLevel() > 0) {
    G4cout << "Time of this run:   " << *fTimer << G4endl;
    G4cout << "Number of events processed: " << run->GetNumberOfEvent()
           << G4endl;
  }
}
