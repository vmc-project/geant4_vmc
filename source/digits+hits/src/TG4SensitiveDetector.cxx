// $Id: TG4SensitiveDetector.cxx,v 1.1.1.1 2002/06/16 15:57:34 hristov Exp $ //
// Category: digits+hits
//
// See the class description in the header file.

#include "TG4SensitiveDetector.h"
#include "TG4StepManager.h"

#include <TVirtualMCApplication.h>

G4int TG4SensitiveDetector::fgSDCounter = 0;

//_____________________________________________________________________________
TG4SensitiveDetector::TG4SensitiveDetector(G4String sdName)
  : G4VSensitiveDetector(sdName),
    fStepManager(TG4StepManager::Instance())
{
//
  fID = ++fgSDCounter;
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
TG4SensitiveDetector::TG4SensitiveDetector(
                                    const TG4SensitiveDetector& right)
  : G4VSensitiveDetector(right)
{  				    
//
  // copy stuff
  *this = right;

  ++fgSDCounter;;
}

//_____________________________________________________________________________
TG4SensitiveDetector::TG4SensitiveDetector()
  : G4VSensitiveDetector("") 
{
//
}

//_____________________________________________________________________________
TG4SensitiveDetector::~TG4SensitiveDetector() {
//
}

// operators

//_____________________________________________________________________________
TG4SensitiveDetector& TG4SensitiveDetector::operator=(
                                    const TG4SensitiveDetector &right)
{
  // check assignement to self
  if (this == &right) return *this;

  // base class assignement
  TG4SensitiveDetector::operator=(right);
  
  fID = right.fID;
  fStepManager = right.fStepManager;
  
  return *this;
}

// public methods

//_____________________________________________________________________________
void TG4SensitiveDetector::UserProcessHits(const G4Track* track, 
                                           const G4Step* step)
{
// Calls MC application stepping function.
// ---

  TVirtualMCApplication::Instance()->Stepping();
}

//_____________________________________________________________________________
G4bool TG4SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
// Calls user defined sensitive detector.
// ---

  // let user sensitive detector process normal step
  fStepManager->SetStep(step, kNormalStep);
  UserProcessHits(step->GetTrack(), step);

  return true;
}

//_____________________________________________________________________________
G4bool TG4SensitiveDetector::ProcessHitsOnBoundary(G4Step* step)
{
// Calls user defined sensitive detector 
// when crossing a geometrical boundary.
// ---

  // let user sensitive detector process boundary step
  fStepManager->SetStep(step, kBoundary);
  UserProcessHits(step->GetTrack(), step);

  return true;
}

