#ifndef A01_RUN_CONFIGURATION1_H
#define A01_RUN_CONFIGURATION1_H

// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file A01RunConfiguration.h 
/// \brief Definition of the A01RunConfiguration class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RunConfiguration.h"

/// \ingroup A01
/// \brief User Geant4 VMC run configuration
///
/// This class overrides setting of the default Geant4 VMC geometry
/// construction with Geant4 detector construction
///
/// \author I. Hrivnacova; IPN, Orsay

class A01RunConfiguration : public TG4RunConfiguration
{
  public:
    A01RunConfiguration(const TString& physicsList = "emStandard",
                        const TString& specialProcess = "stepLimiter");
    virtual ~A01RunConfiguration();

    // methods
    virtual G4VUserDetectorConstruction*  CreateDetectorConstruction();
};

#endif //A01_RUN_CONFIGURATION1_H

