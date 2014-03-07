#ifndef EX03_RUN_CONFIGURATION1_H
#define EX03_RUN_CONFIGURATION1_H

// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file Ex03RunConfiguration1.h 
/// \brief Definition of the Ex03RunConfiguration1 class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RunConfiguration.h"

/// \ingroup E03
/// \brief User Geant4 VMC run configuration
///
/// This class overrides setting of the default Geant4 VMC geometry
/// construction with Geant4 detector construction
///
/// \author I. Hrivnacova; IPN, Orsay

class Ex03RunConfiguration1 : public TG4RunConfiguration
{
  public:
    Ex03RunConfiguration1(const TString& physicsList = "FTFP_BERT",
                          const TString& specialProcess = "stepLimiter");
    virtual ~Ex03RunConfiguration1();

    // methods
    virtual G4VUserDetectorConstruction*  CreateDetectorConstruction();
};

#endif //EX03_RUN_CONFIGURATION1_H

