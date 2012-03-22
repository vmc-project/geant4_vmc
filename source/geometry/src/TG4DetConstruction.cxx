// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4DetConstruction.cxx
/// \brief Implementation of the TG4DetConstruction class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4DetConstruction.h"
#include "TG4GeometryManager.h"
#include "TG4GeometryServices.h"
#include "TG4Globals.h"

#include <G4VPhysicalVolume.hh>

//_____________________________________________________________________________
TG4DetConstruction::TG4DetConstruction()
  : G4VUserDetectorConstruction(),
    fWorld(0)
{
/// Default constructor
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

  fWorld = TG4GeometryServices::Instance()->GetWorld();  
  
  return fWorld;    
}

//_____________________________________________________________________________
G4VPhysicalVolume* TG4DetConstruction::ConstructSlave()
{ 
/// Construct geometry is delegated to TG4GeometryManager
// --

  // Build G4 geometry
  TG4GeometryManager::Instance()->ConstructSlaveGeometry();

  G4cout << "Setting world to TG4GeometryServices::Instance " 
         << TG4GeometryServices::Instance() << " " << fWorld << G4endl;
         
  TG4GeometryServices::Instance()->SetWorld(fWorld);  
  
  return fWorld;    
}

//_____________________________________________________________________________
void TG4DetConstruction::SlaveTG4DetConstruction()
{
/// Nothing to be done yet

  G4cout << "TG4DetConstruction::SlaveTG4DetConstruction" << G4endl;
}
