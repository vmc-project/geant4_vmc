#ifndef TG4_SD_CONSTRUCTION_H
#define TG4_SD_CONSTRUCTION_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4SDConstruction.h
/// \brief Definition of the TG4SDConstruction class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4Verbose.h"

class G4LogicalVolume;

/// \ingroup digits_hits
/// \class TG4SDConstruction
/// \brief Sensitive detector construction
/// 
/// Class for construction of user sensitive detectors.
/// Constructed sensitive detectors inherit from 
/// TG4VSensitiveDetector (see TG4VSensitiveDetector.h description);
/// all cloned logical volumes (which a single G3 volume correspond to)
/// share the same sensitive detector instance.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4SDConstruction : public TG4Verbose

{
  public:
    TG4SDConstruction();
    virtual ~TG4SDConstruction();

    // methods
    virtual void Construct();

  private:
    // methods
    void CreateSD(G4LogicalVolume* lv) const;
};

#endif //TG4_SD_CONSTRUCTION_H

