// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file Ex03RegionConstruction.cxx
/// \brief Implementation of the Ex03RegionConstruction class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex03RegionConstruction.h"

#include "TG4GeometryServices.h"

#include "G4LogicalVolume.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4SystemOfUnits.hh"

//_____________________________________________________________________________
Ex03RegionConstruction::Ex03RegionConstruction() 
  : TG4VUserRegionConstruction()
{
/// Standard constructor
}

//_____________________________________________________________________________
Ex03RegionConstruction::~Ex03RegionConstruction()
{
/// Destructor
}

//
// protected methods
//

//_____________________________________________________________________________
void  Ex03RegionConstruction::Construct()
{
/// Definition of regions

  G4LogicalVolume* logicalVolume 
    = TG4GeometryServices::Instance()->FindLogicalVolume("LAYE");
  if ( ! logicalVolume )  {
    G4cerr << "Logical volume LAYE not found" << G4endl;
    return;
  }     


  G4Region* testRegion = new G4Region("Test_region");
  testRegion->AddRootLogicalVolume(logicalVolume);
  testRegion->SetProductionCuts(new G4ProductionCuts());

  std::vector<double> cuts; 
  cuts.push_back(1.0*cm);cuts.push_back(1.0*cm);cuts.push_back(1.0*cm);
  testRegion->GetProductionCuts()->SetProductionCuts(cuts);
}   
