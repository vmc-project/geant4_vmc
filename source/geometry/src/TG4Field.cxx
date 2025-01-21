//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4Field.cxx
/// \brief Implementation of the TG4Field class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4Field.h"
#include "TG4CachedMagneticField.h"
#include "TG4MagneticField.h"
#include "TG4Globals.h"

#include <TVirtualMagField.h>

#include <G4FieldBuilder.hh>

//_____________________________________________________________________________
TG4Field::TG4Field(const G4FieldParameters& parameters,
  TVirtualMagField* magField, G4LogicalVolume* lv)
  : fVirtualMagField(magField),
    fLogicalVolume(lv)
{
  /// Default constructor

  // Consistency check
  if (magField == nullptr) {
    TG4Globals::Exception(
      "TG4Field", "TG4Field:", "No TVirtualMagField is defined.");
  }

  // Update(parameters);
  // Create field
  auto g4Field = CreateG4Field(parameters, fVirtualMagField);

  // Set field to G4 field builder
  auto fieldBuilder = G4FieldBuilder::Instance();
  if (lv == nullptr) {
    fieldBuilder->SetGlobalField(g4Field);
  }
  else {
    fieldBuilder->SetLocalField(g4Field, lv);
  }
}

//_____________________________________________________________________________
TG4Field::~TG4Field()
{
  /// Destructor
}

//
// private methods
//

//_____________________________________________________________________________
G4Field* TG4Field::CreateG4Field(
  const G4FieldParameters& parameters, TVirtualMagField* magField)
{
  /// Create magnetic, [electromagnetic or gravity - not yet provided]
  /// field according to the provided field type

  if (parameters.GetFieldType() == kMagnetic) {
    if (parameters.GetConstDistance() > 0.) {
      fG4Field =
        new TG4CachedMagneticField(magField, parameters.GetConstDistance());
    }
    else {
      fG4Field = new TG4MagneticField(magField);
    }
  }
  // else if ( parameters.GetFieldType() == kElectroMagnetic ) {
  //   fG4Field = new TG4ElectroMagneticField(magField);
  // }

  // A dd em, gravity field

  return fG4Field;
}
