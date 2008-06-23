// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file Ex03RunConfiguration3.cxx
/// \brief Implementation of the Ex03RunConfiguration3 class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex03RunConfiguration3.h"
#include "Ex03RegionConstruction.h"

//_____________________________________________________________________________
Ex03RunConfiguration3::Ex03RunConfiguration3(const TString& userGeometry,
                                             const TString& physicsList,
                                             const TString& specialProcess)
  : TG4RunConfiguration(userGeometry, physicsList, specialProcess) 
{
/// Standard constructor
/// \param userGeometry    Selection of geometry defintion
/// \param physicsList     Selection of physics
/// \param specialProcess  Selection of the special processes
///
/// The option for geometry selection has to be set here to 
/// "geomVMCtoGeant4, "geomRootToGeant4 or "geomGeant4",
/// as user regions can be defined only with Geant4 native
/// geomeytry navigation.
/// \see More on the available option in class TG4RunConfiguration:
/// http://ivana.home.cern.ch/ivana/g4vmc_html/classTG4RunConfiguration.html
}

//_____________________________________________________________________________
Ex03RunConfiguration3::~Ex03RunConfiguration3()
{
/// Destructor
}

//
// protected methods
//

//_____________________________________________________________________________
TG4VUserRegionConstruction*  Ex03RunConfiguration3::CreateUserRegionConstruction()
{
/// The Geant4 VMC detector construction is overridden with the detector
/// construction class from the Geant4 novice example N03 library.

  return new Ex03RegionConstruction();
}   
