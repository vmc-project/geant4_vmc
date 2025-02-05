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

  // Create field
  fG4Field = new TG4MagneticField(magField);
     // field will be deleted by Geant4 kernel

  // Set field to G4 field builder
  auto fieldBuilder = G4FieldBuilder::Instance();
  if (lv == nullptr) {
    fieldBuilder->SetGlobalField(fG4Field);
  }
  else {
    fieldBuilder->SetLocalField(fG4Field, lv);
  }
}

//_____________________________________________________________________________
TG4Field::~TG4Field()
{
  /// Destructor
}
