#ifndef TG4_MAGNETIC_FIELD_H
#define TG4_MAGNETIC_FIELD_H

//-------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4MagneticField.h
/// \brief Definition of the TG4MagneticField class
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4MagneticField.hh>
#include <globals.hh>


class TG4FieldParameters;

class G4EquationOfMotion;
class G4MagIntegratorStepper;
class G4LogicalVolume;

class TVirtualMagField;

/// \ingroup geometry
/// \brief The magnetic field defined via TVirtualMagField.
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

class TG4MagneticField : public G4MagneticField
{
  public:
    TG4MagneticField(TVirtualMagField* magField);
    virtual ~TG4MagneticField();

    virtual void GetFieldValue(const G4double point[3], G4double* bfield) const;

    virtual void PrintStatistics() const {}

  protected:
    // data
    /// The associated TGeo magnetic field
    TVirtualMagField*  fVirtualMagField;
};

#endif //TG4_MAGNETIC_FIELD_H

