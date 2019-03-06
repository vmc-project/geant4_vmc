#ifndef TG4_POST_DET_CONSTRUCTION_H
#define TG4_POST_DET_CONSTRUCTION_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4PostDetConstruction.h
/// \brief Definition of the TG4PostDetConstruction class
///
/// \author I. Hrivnacova; IPN, Orsay

#ifdef USE_G4ROOT

#include <TG4RootDetectorConstruction.h>

class TG4PostDetConstruction;

/// \ingroup geometry
/// \brief The post detector construction used with G4Root navigation
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4PostDetConstruction : public TVirtualUserPostDetConstruction
{
  public:
    TG4PostDetConstruction();
    virtual ~TG4PostDetConstruction();

    // methods
    virtual void Initialize(TG4RootDetectorConstruction *dc);
    virtual void InitializeSDandField();

  private:
    /// Not implemented
    TG4PostDetConstruction(const TG4PostDetConstruction& right);
    /// Not implemented
    TG4PostDetConstruction& operator=(const TG4PostDetConstruction& right);
};

#endif //USE_G4ROOT

#endif //TG4_DET_CONSTRUCTION_H

