//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex05RunConfiguration1.cxx
/// \brief Implementation of the Ex05RunConfiguration1 class 
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex05RunConfiguration.h"
#include "Ex05FastSimulation.h"

//_____________________________________________________________________________
Ex05RunConfiguration::Ex05RunConfiguration(const TString& geometry,
    	                                   const TString& physicsList,
                                           const TString& specialProcess)
  : TG4RunConfiguration(geometry, physicsList, specialProcess) 
{
/// Standard constructor
/// \param geometry        Selection of geometry option
/// \param physicsList     Selection of physics
/// \param specialProcess  Selection of the special processes
}

//_____________________________________________________________________________
Ex05RunConfiguration::~Ex05RunConfiguration()
{
/// Destructor
}

//
// protected methods
//

//_____________________________________________________________________________
TG4VUserFastSimulation*  Ex05RunConfiguration::CreateUserFastSimulation()
{
/// Create Gflash fast simulation model

  G4cout << "Going to create Ex05FastSimulation" << G4endl;

  return new Ex05FastSimulation();
}   
