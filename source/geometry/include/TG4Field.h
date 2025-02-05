#ifndef TG4_FIELD_H
#define TG4_FIELD_H

//-------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4Field.h
/// \brief Definition of the TG4Field class
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4MagneticField.hh>
#include <globals.hh>

class G4FieldParameters;
class G4LogicalVolume;

class TVirtualMagField;

/// \ingroup geometry
/// \brief The class for constructing magnetic, electromagnetic and gravity
/// fields which strength is defined via TVirtualMagField.
///
/// The field is constructed according to the setting in G4FieldParameters;
/// the Geant4 defaults can be redefined via UI commands.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4Field
{
 public:
  TG4Field(const G4FieldParameters& parameters, TVirtualMagField* magField,
    G4LogicalVolume* lv = nullptr);
  ~TG4Field();

  // access to field
  G4Field* GetG4Field() const;

 private:
  // data
  /// Geant4 field
  G4Field* fG4Field = nullptr;
  /// The associated TGeo magnetic field
  TVirtualMagField* fVirtualMagField = nullptr;
  /// The associated volume (if local field)
  G4LogicalVolume* fLogicalVolume = nullptr;
};

// inline functions

inline G4Field* TG4Field::GetG4Field() const
{
  /// Return the instantiated field
  return fG4Field;
}

#endif // TG4_FIELD_H
