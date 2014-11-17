//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01RunConfiguration.cxx
/// \brief Implementation of the A01RunConfiguration class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include "A01RunConfiguration.h"
#include "A01DetectorConstruction.hh"

//_____________________________________________________________________________
A01RunConfiguration::A01RunConfiguration(const TString& physicsList,
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
A01RunConfiguration::~A01RunConfiguration()
{
/// Destructor
}

//
// protected methods
//

//_____________________________________________________________________________
G4VUserDetectorConstruction*  A01RunConfiguration::CreateDetectorConstruction()
{
/// The Geant4 VMC detector construction is overridden with the detector
/// construction class from the Geant4 novice example N03 library.

  return new A01DetectorConstruction();
}   
