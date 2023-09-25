//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4MagneticField.cxx
/// \brief Implementation of the TG4MagneticField class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4MagneticField.h"
#include "TG4G3Units.h"

#include <TVirtualMagField.h>

//_____________________________________________________________________________
TG4MagneticField::TG4MagneticField(TVirtualMagField* magField)
  : G4MagneticField(), fVirtualMagField(magField)
{
  /// Default constructor
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
void TG4MagneticField::GetFieldValue(
  const G4double point[3], G4double* bfield) const
{
  /// Return the bfield values in the given point.

  // Set units
  const G4double g3point[3] = { point[0] * TG4G3Units::InverseLength(),
    point[1] * TG4G3Units::InverseLength(), point[2] * TG4G3Units::InverseLength() };

  // Call user field
  fVirtualMagField->Field(g3point, bfield);

  // Set units
  for (G4int i = 0; i < 3; i++) bfield[i] = bfield[i] * TG4G3Units::Field();
}
