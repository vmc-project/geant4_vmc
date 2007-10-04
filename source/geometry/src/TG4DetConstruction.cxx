// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: geometry
// Class TG4DetConstruction
// ---------------------------
// See the class description in the header file.

#include "TG4DetConstruction.h"
#include "TG4GeometryManager.h"
#include "TG4GeometryServices.h"
#include "TG4Globals.h"

#include <G4VPhysicalVolume.hh>

//_____________________________________________________________________________
TG4DetConstruction::TG4DetConstruction()
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4DetConstruction::~TG4DetConstruction() 
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
G4VPhysicalVolume* TG4DetConstruction::Construct()
{ 
/// Construct geometry is delegated to TG4GeometryManager
// --

  // Build G4 geometry
  TG4GeometryManager::Instance()->ConstructGeometry();

  return TG4GeometryServices::Instance()->GetWorld();      
}

