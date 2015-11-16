#ifndef EX05_RUN_CONFIGURATION1_H
#define EX05_RUN_CONFIGURATION1_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex05RunConfiguration1.h 
/// \brief Definition of the Ex05RunConfiguration1 class 
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RunConfiguration.h"

/// \ingroup E05
/// \brief User Geant4 VMC run configuration
///
/// This class overrides the default Geant4 VMC dummy 
/// fast simulation construction with the user class.
///
/// \author I. Hrivnacova; IPN, Orsay

class Ex05RunConfiguration : public TG4RunConfiguration
{
  public:
    Ex05RunConfiguration(const TString& geometry,
    	                 const TString& physicsList = "FTFP_BERT",
                         const TString& specialProcess = "stepLimiter");
    virtual ~Ex05RunConfiguration();

    // methods
    virtual TG4VUserFastSimulation*  CreateUserFastSimulation();
};

#endif //EX05_RUN_CONFIGURATION1_H

