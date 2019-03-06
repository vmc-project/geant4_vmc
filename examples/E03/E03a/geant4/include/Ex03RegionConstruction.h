#ifndef EX03_REGION_CONSTRUCTION_H
#define EX03_REGION_CONSTRUCTION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex03RegionConstruction.h
/// \brief Definition of the Ex03RegionConstruction class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4VUserRegionConstruction.h"

/// \ingroup E03
/// \brief Special class for definition of regions
///
/// \author I. Hrivnacova; IPN, Orsay

class Ex03RegionConstruction : public TG4VUserRegionConstruction
{
  public:
    Ex03RegionConstruction();
    virtual ~Ex03RegionConstruction();

    // methods
    virtual void Construct();
};

#endif //EX03_REGION_CONSTRUCTION_H

