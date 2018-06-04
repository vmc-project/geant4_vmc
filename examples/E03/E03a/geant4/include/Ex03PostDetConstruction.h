#ifndef EX03_POST_DET_CONSTRUCTION_H
#define EX03_POST_DET_CONSTRUCTION_H 1

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex03PostDetConstruction.h
/// \brief Definition of the Ex03PostDetConstruction class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4VUserPostDetConstruction.h"
#include "globals.hh"

/// Post detector construction class which integrates  
/// user defined magnetic field equation of motion and integrator

class Ex03PostDetConstruction : public TG4VUserPostDetConstruction
{
  public:
    Ex03PostDetConstruction();
    virtual ~Ex03PostDetConstruction();

  public:
    virtual void Construct();
};

#endif //EX03_POST_DET_CONSTRUCTION_H

