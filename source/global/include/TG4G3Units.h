#ifndef TG4_G3_UNITS_H
#define TG4_G3_UNITS_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4G3Units.h
/// \brief Definition of the TG4G3Units class
///
/// \author I. Hrivnacova; IPN, Orsay

#include <globals.hh>

/// \ingroup global
/// \brief G3 default units of physical quantities;
///
/// All physical quantities returned by VMC are expressed in these units.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4G3Units
{
 public:
  ~TG4G3Units();

  // static get methods
  static G4double Length();
  static G4double Angle();
  static G4double Time();
  static G4double Charge();
  static G4double Energy();
  static G4double Mass();
  static G4double MassDensity();
  static G4double AtomicWeight();
  static G4double Field();
  // inverse values
  static G4double InverseLength();
  static G4double InverseAngle();
  static G4double InverseTime();
  static G4double InverseCharge();
  static G4double InverseEnergy();
  static G4double InverseMass();
  static G4double InverseMassDensity();
  static G4double InverseAtomicWeight();
  static G4double InverseField();


 private:
  TG4G3Units();

  // static data members
  static const G4double fgkLength;       ///< G3 length unit
  static const G4double fgkAngle;        ///< G3 angle unit
  static const G4double fgkTime;         ///< G3 time unit
  static const G4double fgkCharge;       ///< G3 charge unit
  static const G4double fgkEnergy;       ///< G3 energy unit
  static const G4double fgkMass;         ///< G3 mass unit
  static const G4double fgkMassDensity;  ///< G3 mass density unit
  static const G4double fgkAtomicWeight; ///< G3 atomic weight unit
  static const G4double fgkField;        ///< G3 magnetic field unit
  // inverse values
  static const G4double fgkInverseLength;       ///< 1 over G3 length unit
  static const G4double fgkInverseAngle;        ///< 1 over G3 angle unit
  static const G4double fgkInverseTime;         ///< 1 over G3 time unit
  static const G4double fgkInverseCharge;       ///< 1 over G3 charge unit
  static const G4double fgkInverseEnergy;       ///< 1 over G3 energy unit
  static const G4double fgkInverseMass;         ///< 1 over G3 mass unit
  static const G4double fgkInverseMassDensity;  ///< 1 over G3 mass density unit
  static const G4double fgkInverseAtomicWeight; ///< 1 over G3 atomic weight unit
  static const G4double fgkInverseField;        ///< 1 over G3 magnetic field unit
};

// inline methods

inline G4double TG4G3Units::Length()
{
  /// Return G3 length unit
  return fgkLength;
}

inline G4double TG4G3Units::Angle()
{
  /// Return G3 angle unit
  return fgkAngle;
}

inline G4double TG4G3Units::Time()
{
  /// Return G3 time unit
  return fgkTime;
}

inline G4double TG4G3Units::Charge()
{
  /// Return G3 chanrge unit
  return fgkCharge;
}

inline G4double TG4G3Units::Energy()
{
  /// Return G3 energy unit
  return fgkEnergy;
}

inline G4double TG4G3Units::Mass()
{
  /// Return G3 mass unit
  return fgkMass;
}

inline G4double TG4G3Units::MassDensity()
{
  /// Return G3 mass density unit
  return fgkMassDensity;
}

inline G4double TG4G3Units::AtomicWeight()
{
  /// Return G3 atomic weight unit
  return fgkAtomicWeight;
}

inline G4double TG4G3Units::Field()
{
  /// Return G3 magnetic field unit
  return fgkField;
}

inline G4double TG4G3Units::InverseLength() {
  return fgkInverseLength;
}

inline G4double TG4G3Units::InverseAngle()
{
  /// Return G3 angle unit
  return fgkInverseAngle;
}

inline G4double TG4G3Units::InverseTime()
{
  /// Return G3 time unit
  return fgkInverseTime;
}

inline G4double TG4G3Units::InverseCharge()
{
  /// Return G3 chanrge unit
  return fgkInverseCharge;
}

inline G4double TG4G3Units::InverseEnergy()
{
  /// Return G3 energy unit
  return fgkInverseEnergy;
}

inline G4double TG4G3Units::InverseMass()
{
  /// Return G3 mass unit
  return fgkInverseMass;
}

inline G4double TG4G3Units::InverseMassDensity()
{
  /// Return G3 mass density unit
  return fgkInverseMassDensity;
}

inline G4double TG4G3Units::InverseAtomicWeight()
{
  /// Return G3 atomic weight unit
  return fgkInverseAtomicWeight;
}

inline G4double TG4G3Units::InverseField()
{
  /// Return G3 magnetic field unit
  return fgkInverseField;
}

#endif // TG4_G3_UNITS_H
