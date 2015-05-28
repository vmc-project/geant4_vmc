#ifndef EX03_G4_DETECTOR_CONSTRUCTION_H
#define EX03_G4_DETECTOR_CONSTRUCTION_H 1

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex03G4DetectorConstruction.hh
/// \brief Definition of the Ex03G4DetectorConstruction class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4DetConstruction.h"
#include "globals.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;

/// Detector construction class which specialize TG4DetectorConstruction
/// with a user defined magnetic field equation of motion and integrator

class Ex03G4DetectorConstruction : public TG4DetConstruction
{
  public:
    Ex03G4DetectorConstruction();
    virtual ~Ex03G4DetectorConstruction();

  public:
    virtual void ConstructSDandField();
};

#endif //EX03_G4_DETECTOR_CONSTRUCTION_H

