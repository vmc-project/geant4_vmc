#ifndef TG4_GFLASH_SENSITIVE_DETECTOR_H
#define TG4_GFLASH_SENSITIVE_DETECTOR_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4GflashSensitiveDetector.h
/// \brief Definition of the TG4GflashSensitiveDetector class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SensitiveDetector.h"
#include "G4VGFlashSensitiveDetector.hh"

#include <globals.hh>

class G4GFlashSpot;

/// \ingroup digits_hits
/// \brief Sensitive detector with Gflash
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4GflashSensitiveDetector : public TG4SensitiveDetector,
                                   public G4VGFlashSensitiveDetector
{
  public:
    TG4GflashSensitiveDetector(G4String sdName, G4int mediumID);
    virtual ~TG4GflashSensitiveDetector();

    // methods
    using  TG4SensitiveDetector::ProcessHits;
    virtual G4bool ProcessHits(G4GFlashSpot* gflashSpot, G4TouchableHistory*);

  private:
    /// Not implemented
    TG4GflashSensitiveDetector();
    /// Not implemented
    TG4GflashSensitiveDetector(const TG4GflashSensitiveDetector& right);
    /// Not implemented
    TG4GflashSensitiveDetector& operator=(const TG4GflashSensitiveDetector &right);
};

#endif //TG4_GFLASH_SENSITIVE_DETECTOR_H
