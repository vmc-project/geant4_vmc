#ifndef EX03_RUN_CONFIGURATION4_H
#define EX03_RUN_CONFIGURATION4_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex03RunConfiguration4.h 
/// \brief Definition of the Ex03RunConfiguration4 class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RunConfiguration.h"

/// \ingroup E03
/// \brief User Geant4 VMC run configuration
///
/// This class demonstrates inclusion of a user defined detector construction class
/// with a user defined magnetic field equation of motion and/or its integrator.
/// \author I. Hrivnacova; IPN, Orsay

class Ex03RunConfiguration4 : public TG4RunConfiguration
{
  public:
    Ex03RunConfiguration4(const TString& userGeometry,
                         const TString& physicsList = "emStandard",
                         const TString& specialProcess = "stepLimiter",
                         Bool_t specialStacking = false,
                         Bool_t mtApplication = true);
    virtual ~Ex03RunConfiguration4();

    // methods
    virtual TG4VUserPostDetConstruction*  CreateUserPostDetConstruction();
};

#endif //EX03_RUN_CONFIGURATION4_H

