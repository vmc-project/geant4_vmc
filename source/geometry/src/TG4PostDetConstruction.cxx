//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4PostDetConstruction.cxx
/// \brief Implementation of the TG4PostDetConstruction class
///
/// \author I. Hrivnacova; IPN, Orsay

#ifdef USE_G4ROOT

#include "TG4PostDetConstruction.h"
#include "TG4GeometryManager.h"
#include "TG4Globals.h"

//_____________________________________________________________________________
TG4PostDetConstruction::TG4PostDetConstruction()
  : TVirtualUserPostDetConstruction()
{
/// Default constructor
}

//_____________________________________________________________________________
TG4PostDetConstruction::~TG4PostDetConstruction()
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void TG4PostDetConstruction::Initialize(TG4RootDetectorConstruction* /*dc*/)
{
/// Construct geometry using the VMC application.
// --

  G4cout << "TG4PostDetConstruction::Initialize" << G4endl;
  TG4GeometryManager::Instance()->ConstructGeometry();
}

//_____________________________________________________________________________
void TG4PostDetConstruction::InitializeSDandField()
{
/// Construct geometry using the VMC application.
// --

  G4cout << "TG4PostDetConstruction::InitializeSDandField" << G4endl;
  TG4GeometryManager::Instance()->ConstructSDandField();
}

#endif
