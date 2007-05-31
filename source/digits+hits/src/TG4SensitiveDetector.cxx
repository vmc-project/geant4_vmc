// $Id: TG4SensitiveDetector.cxx,v 1.3 2006/12/12 16:21:15 brun Exp $ //

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: digits+hits
// Class TG4SensitiveDetector
// ---------------------------
// See the class description in the header file.

#include "TG4SensitiveDetector.h"
#include "TG4StepManager.h"

#include <TVirtualMCApplication.h>

G4int TG4SensitiveDetector::fgSDCounter = 0;

//_____________________________________________________________________________
TG4SensitiveDetector::TG4SensitiveDetector(G4String sdName)
  : G4VSensitiveDetector(sdName),
    fID(++fgSDCounter),
    fStepManager(TG4StepManager::Instance())
{
//
}

//_____________________________________________________________________________
TG4SensitiveDetector::TG4SensitiveDetector(G4String sdName, G4int id)
  : G4VSensitiveDetector(sdName),
    fID(id),
    fStepManager(TG4StepManager::Instance())

{
//
  ++fgSDCounter;
}

//_____________________________________________________________________________
TG4SensitiveDetector::~TG4SensitiveDetector() {
//
}

//
// public methods
//

//_____________________________________________________________________________
void TG4SensitiveDetector::UserProcessHits(const G4Track* /*track*/, 
                                           const G4Step* /*step*/)
{
/// Call VMC application stepping function.

  TVirtualMCApplication::Instance()->Stepping();
}

//_____________________________________________________________________________
G4bool TG4SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
/// Call user defined sensitive detector.

  // let user sensitive detector process normal step
  fStepManager->SetStep(step, kNormalStep);
  UserProcessHits(step->GetTrack(), step);

  return true;
}

//_____________________________________________________________________________
G4bool TG4SensitiveDetector::ProcessHitsOnBoundary(G4Step* step)
{
/// Call user defined sensitive detector 
/// when crossing a geometrical boundary.

  // let user sensitive detector process boundary step
  fStepManager->SetStep(step, kBoundary);
  UserProcessHits(step->GetTrack(), step);

  return true;
}

