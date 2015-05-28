//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex03RunConfiguration4.cxx
/// \brief Implementation of the Ex03RunConfiguration4 class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex03RunConfiguration4.h"
#include "Ex03G4DetectorConstruction.h"


//_____________________________________________________________________________
Ex03RunConfiguration4::Ex03RunConfiguration4(const TString& userGeometry,
                                             const TString& physicsList,
                                             const TString& specialProcess,
                                             Bool_t specialStacking,
                                             Bool_t mtApplication)
  : TG4RunConfiguration(userGeometry, physicsList, specialProcess,
  	                    specialStacking, mtApplication) 
{
/// Standard constructor
}

//_____________________________________________________________________________
Ex03RunConfiguration4::~Ex03RunConfiguration4()
{
/// Destructor
}

//
// protected methods
//

//_____________________________________________________________________________
G4VUserDetectorConstruction*  Ex03RunConfiguration4::CreateDetectorConstruction()
{
/// User defined detector construction

  return new Ex03G4DetectorConstruction();
}   
