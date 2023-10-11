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

#include "TG4FieldParameters.h"

#include <G4MagneticField.hh>
#include <globals.hh>

class TG4FieldParameters;

class G4ChordFinder;
class G4EquationOfMotion;
class G4MagIntegratorStepper;
class G4LogicalVolume;
class G4VIntegrationDriver;

class TVirtualMagField;

/// \ingroup geometry
/// \brief The class for constructing magnetic, electromagnetic and gravity
/// fields which strength is defined via TVirtualMagField.
///
/// The equation of motion motion of a particle in a field  and the
/// integration method is set according to the selection in
/// TG4FieldParameters, as well as other accuracy parameters.
/// The default values in TG4FieldParameters correspond to defaults
/// set in Geant4 (taken from Geant4 9.3 release.)
/// As Geant4 classes to not provide access methods for these defaults,
/// the defaults have to be checked with each new Geant4 release.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4Field
{
 public:
  TG4Field(const TG4FieldParameters& parameters, TVirtualMagField* magField,
    G4LogicalVolume* lv = 0);
  ~TG4Field();

  void Update(const TG4FieldParameters& parameters);

  // access to field setting
  G4Field* GetG4Field() const;
  G4EquationOfMotion* GetEquation() const;
  G4MagIntegratorStepper* GetStepper() const;
  G4VIntegrationDriver* GetIntegrationDriver() const;

 private:
  // methods
  G4Field* CreateG4Field(
    const TG4FieldParameters& parameters, TVirtualMagField* magField);
  G4EquationOfMotion* CreateEquation(EquationType equation);
  G4MagIntegratorStepper* CreateStepper(
    G4EquationOfMotion* equation, StepperType stepper);
  G4VIntegrationDriver* CreateFSALStepperAndDriver(
    G4EquationOfMotion* equation, StepperType stepper, G4double minStep);

  // data
  /// Geant4 field
  G4Field* fG4Field;
  /// The associated TGeo magnetic field
  TVirtualMagField* fVirtualMagField;
  /// The associated volume (if local field)
  G4LogicalVolume* fLogicalVolume;
  /// The equation of motion
  G4EquationOfMotion* fEquation;
  /// The magnetic integrator stepper
  G4MagIntegratorStepper* fStepper;
  /// The magnetic integrator driver
  G4VIntegrationDriver* fDriver;
  /// Chord finder
  G4ChordFinder* fChordFinder;
};

// inline functions

inline G4Field* TG4Field::GetG4Field() const
{
  /// Return the instantiated field
  return fG4Field;
}

inline G4EquationOfMotion* TG4Field::GetEquation() const
{
  /// Return the equation of motion
  return fEquation;
}

inline G4MagIntegratorStepper* TG4Field::GetStepper() const
{
  /// Return the magnetic integrator stepper
  return fStepper;
}

#endif // TG4_FIELD_H
