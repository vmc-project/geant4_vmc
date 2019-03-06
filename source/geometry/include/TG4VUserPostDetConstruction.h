#ifndef TG4_V_USER_POST_DET_CONSTRUCTION_H
#define TG4_V_USER_POST_DET_CONSTRUCTION_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4VUserPostDetConstruction.h
/// \brief Definition of the TG4VUserPostDetConstruction class
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4VUserDetectorConstruction.hh>

/// \ingroup geometry
/// \brief The abstract base class for user defined class to customize
/// geometry.
///
/// An implemented use case is a user customization of magnetic field
/// equation of motion and its stepper.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4VUserPostDetConstruction
{
  public:
    TG4VUserPostDetConstruction() {}
    virtual ~TG4VUserPostDetConstruction() {}

    ///  Method to be overriden by user
    virtual void Construct() = 0;

  private:
    /// Not implemented
    TG4VUserPostDetConstruction(const TG4VUserPostDetConstruction& right);
    /// Not implemented
    TG4VUserPostDetConstruction& operator=(const TG4VUserPostDetConstruction& right);
};

#endif //TG4_V_USER_POST_DET_CONSTRUCTION_H

