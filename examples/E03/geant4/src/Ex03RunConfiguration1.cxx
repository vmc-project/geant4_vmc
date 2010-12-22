// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file Ex03RunConfiguration1.cxx
/// \brief Implementation of the Ex03RunConfiguration1 class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex03RunConfiguration1.h"
#include "DetectorConstruction.hh"

//_____________________________________________________________________________
Ex03RunConfiguration1::Ex03RunConfiguration1(const TString& physicsList,
                                             const TString& specialProcess)
  : TG4RunConfiguration("geomGeant4", physicsList, specialProcess) 
{
/// Standard constructor
/// \param physicsList     Selection of physics
/// \param specialProcess  Selection of the special processes
///
/// The option for geometry selection has to be set here to "geomGeant4",
/// as geometry will be defined directly via Geant4.
/// \see More on the available option in class TG4RunConfiguration:
/// http://ivana.home.cern.ch/ivana/g4vmc_html/classTG4RunConfiguration.html
}

//_____________________________________________________________________________
Ex03RunConfiguration1::~Ex03RunConfiguration1()
{
/// Destructor
}

//
// protected methods
//

//_____________________________________________________________________________
G4VUserDetectorConstruction*  Ex03RunConfiguration1::CreateDetectorConstruction()
{
/// The Geant4 VMC detector construction is overridden with the detector
/// construction class from the Geant4 novice example N03 library.

  return new DetectorConstruction();
}   
