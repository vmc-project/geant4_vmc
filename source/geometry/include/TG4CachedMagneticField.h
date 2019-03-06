#ifndef TG4_CACHED_MAGNETIC_FIELD_H
#define TG4_CACHED_MAGNETIC_FIELD_H

//-------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4CachedMagneticField.h
/// \brief Definition of the TG4CachedMagneticField class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4FieldParameters.h"
#include "TG4MagneticField.h"

#include <globals.hh>
#include <G4ThreeVector.hh>

class TG4FieldParameters;

class G4EquationOfMotion;
class G4MagIntegratorStepper;
class G4LogicalVolume;

class TVirtualMagField;

/// \ingroup geometry
/// \brief The cached magnetic field defined by the TVirtualMCApplication field map.
///
/// Overrides TG4MagneticField::GetFieldValue();
/// it uses the value from a previous call in case the distance of the
/// new point from a previous one is smaller than the value of
/// TG4FieldParameters::fConstDistance.
///
/// According to G4CachedMagneticField class.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4CachedMagneticField : public TG4MagneticField
{
  public:
    TG4CachedMagneticField(TVirtualMagField* magField,
                           G4double constDistance);
    virtual ~TG4CachedMagneticField();

    virtual void GetFieldValue(const G4double point[3], G4double* bfield) const;

    // virtual void Update(const TG4FieldParameters& parameters);
    virtual void PrintStatistics() const;

    void ClearCounter();
    void SetConstDistance(G4double value);

  private:
    // data members
    /// The last evaluated location
    mutable G4ThreeVector  fLastLocation;
    /// The last evaluated value
    mutable G4ThreeVector  fLastValue;
    /// The counter of calls to GetFieldValue()
    mutable G4int fCallsCounter;
    /// The counter of field value evaluations in GetFieldValue()
    mutable G4int fEvaluationsCounter;
    /// The square of the distance within which the field is considered constant
    G4double fConstDistanceSquare;
};

#endif //TG4_CACHED_MAGNETIC_FIELD_H
