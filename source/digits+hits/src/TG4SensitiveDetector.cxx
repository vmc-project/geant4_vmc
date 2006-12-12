// $Id: TG4SensitiveDetector.cxx,v 1.2 2004/11/10 11:39:27 brun Exp $ //
// Category: digits+hits
//
// Class TG4SensitiveDetector
// ---------------------------
// See the class description in the header file.
//
// Author: I.Hrivnacova

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

