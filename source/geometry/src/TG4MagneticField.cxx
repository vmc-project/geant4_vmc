// $Id: TG4MagneticField.cxx,v 1.3 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: geometry
// Class TG4ParticlesManager
// -------------------------
// See the class description in the header file.

#include "TG4MagneticField.h"
#include "TG4G3Units.h"

#include <TVirtualMCApplication.h>

#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>

//_____________________________________________________________________________
TG4MagneticField::TG4MagneticField()
  : G4MagneticField() 
{
/// Standard constructor

  G4FieldManager* fieldManager
     = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  fieldManager->SetDetectorField(this);
  fieldManager->CreateChordFinder(this);
}

//_____________________________________________________________________________
TG4MagneticField::~TG4MagneticField() 
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void TG4MagneticField::GetFieldValue(const G4double point[3], G4double* bfield) const
{
/// Return the bfield values in the given point.

  // Set units
  const G4double g3point[3] = { point[0] / TG4G3Units::Length(),
                                 point[1] / TG4G3Units::Length(),
                                 point[2] / TG4G3Units::Length() };

  TVirtualMCApplication::Instance()->Field(g3point, bfield);
  
  // Set units
  for (G4int i=0; i<3; i++) bfield[i] = bfield[i] * TG4G3Units::Field();
}

