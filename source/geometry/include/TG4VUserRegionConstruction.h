#ifndef TG4_V_USER_REGION_CONSTRUCTION_H
#define TG4_V_USER_REGION_CONSTRUCTION_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4VUserRegionConstruction.h
/// \brief Definition of the TG4VUserRegionConstruction class
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4VUserDetectorConstruction.hh>

/// \ingroup geometry
/// \brief The abstract base class for user defined regions
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4VUserRegionConstruction
{
  public:
    TG4VUserRegionConstruction();
    virtual ~TG4VUserRegionConstruction();

    ///  Method to be overriden by user
    virtual void Construct() = 0;

  private:
    /// Not implemented
    TG4VUserRegionConstruction(const TG4VUserRegionConstruction& right);
    /// Not implemented
    TG4VUserRegionConstruction& operator=(const TG4VUserRegionConstruction& right);
};

#endif //TG4_V_USER_REGION_CONSTRUCTION_H

