//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4GflashSensitiveDetector.cxx
/// \brief Implementation of the TG4GflashSensitiveDetector class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4GflashSensitiveDetector.h"
#include "TG4StepManager.h"

#include <TVirtualMCApplication.h>

//_____________________________________________________________________________
TG4GflashSensitiveDetector::TG4GflashSensitiveDetector(G4String sdName, G4int mediumId)
  : TG4SensitiveDetector(sdName, mediumId)
{
/// Standard constructor with the specified \em name
}

//_____________________________________________________________________________
TG4GflashSensitiveDetector::~TG4GflashSensitiveDetector()
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
G4bool TG4GflashSensitiveDetector::ProcessHits(G4GFlashSpot* gflashSpot, G4TouchableHistory*)
{
/// Call user defined sensitive detector

  // let user sensitive detector process Gflash step
  GetStepManager()->SetStep(gflashSpot, kGflashSpot);
  TVirtualMCApplication::Instance()->Stepping();

  return true;
}
