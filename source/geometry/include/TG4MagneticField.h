//  $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup geometry
/// \class TG4MagneticField
/// \brief The magnetic field defined by the TVirtualMCApplication field map. 
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_MAGNETIC_FIELD_H
#define TG4_MAGNETIC_FIELD_H

#include <G4MagneticField.hh>
#include <globals.hh>

class TG4MagneticField : public G4MagneticField
{
  public:
    TG4MagneticField();
    virtual ~TG4MagneticField();

    void GetFieldValue(const G4double point[3], G4double* bfield) const;
};

#endif //TG4_MAGNETIC_FIELD_H

