//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4SensitiveDetector.cxx
/// \brief Implementation of the TG4SensitiveDetector class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SensitiveDetector.h"
#include "TG4StepManager.h"

#include <TVirtualMCApplication.h>
#include <TVirtualMCSensitiveDetector.h>

G4ThreadLocal G4int TG4SensitiveDetector::fgSDCounter = 0;

//_____________________________________________________________________________
TG4SensitiveDetector::TG4SensitiveDetector(G4String sdName, G4int mediumID)
  : G4VSensitiveDetector(sdName),
    fStepManager(TG4StepManager::Instance()),
    fMCApplication(TVirtualMCApplication::Instance()),
    fUserSD(0),
    fID(++fgSDCounter),
    fMediumID(mediumID)
{
  /// Standard constructor with the specified \em name
}

//_____________________________________________________________________________
TG4SensitiveDetector::TG4SensitiveDetector(
  TVirtualMCSensitiveDetector* userSD, G4int mediumID, G4bool exclusiveSD)
  : G4VSensitiveDetector(userSD->GetName()),
    fStepManager(TG4StepManager::Instance()),
    fMCApplication(0),
    fUserSD(userSD),
    fID(++fgSDCounter),
    fMediumID(mediumID)
{
  /// Standard constructor with the specified \em name

  if (!exclusiveSD) {
    fMCApplication = TVirtualMCApplication::Instance();
  }
}

//_____________________________________________________________________________
TG4SensitiveDetector::~TG4SensitiveDetector()
{
  /// Destructor
}

//
// private methods
//

//_____________________________________________________________________________
void TG4SensitiveDetector::UserProcessHits()
{
  /// Call user SD and/or VMC application stepping function.

  if (fUserSD) {
    fUserSD->ProcessHits();
  }

  if (fMCApplication) {
    fMCApplication->Stepping();
  }
}

//
// public methods
//

//_____________________________________________________________________________
G4bool TG4SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  /// Call user defined sensitive detector.

  // let user sensitive detector process normal step
  fStepManager->SetStep(step, kNormalStep);
  UserProcessHits();

  return true;
}

//_____________________________________________________________________________
G4bool TG4SensitiveDetector::ProcessHitsOnBoundary(G4Step* step)
{
  /// Call user defined sensitive detector
  /// when crossing a geometrical boundary.

  // let user sensitive detector process boundary step
  fStepManager->SetStep(step, kBoundary);
  UserProcessHits();

  return true;
}

//_____________________________________________________________________________
void TG4SensitiveDetector::ProcessHitsOnTrackStart()
{
  /// Call VMC application stepping function.

  UserProcessHits();
}
