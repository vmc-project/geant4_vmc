#ifndef EX03_RUN_CONFIGURATION3_H
#define EX03_RUN_CONFIGURATION3_H

// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file Ex03RunConfiguration3.h 
/// \brief Definition of the Ex03RunConfiguration3 class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RunConfiguration.h"

/// \ingroup E03
/// \brief User Geant4 VMC run configuration
///
/// This class demonstrates how to add to Geant4 VMC geometry construction 
/// user defined regions
///
/// \author I. Hrivnacova; IPN, Orsay

class Ex03RunConfiguration3 : public TG4RunConfiguration
{
  public:
    Ex03RunConfiguration3(const TString& userGeometry,
                          const TString& physicsList = "FTFP_BERT",
                          const TString& specialProcess = "stepLimiter");
    virtual ~Ex03RunConfiguration3();

    // methods
    virtual TG4VUserRegionConstruction*  CreateUserRegionConstruction();
};

#endif //EX03_RUN_CONFIGURATION3_H

