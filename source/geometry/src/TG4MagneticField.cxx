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
#include "TG4Globals.h"

#include <TVirtualMCApplication.h>
#include <TVirtualMagField.h>

#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>
#include <G4ChordFinder.hh>
#include <G4Mag_EqRhs.hh>
#include <G4Mag_UsualEqRhs.hh>
#include <G4Mag_SpinEqRhs.hh>
#include <G4EqMagElectricField.hh>
#include <G4EqEMFieldWithSpin.hh>
#include <G4EqEMFieldWithEDM.hh>
#include <G4ConstRK4.hh>
#include <G4ClassicalRK4.hh>
#include <G4SimpleRunge.hh>
#include <G4ExplicitEuler.hh>
#include <G4SimpleHeum.hh>
#include <G4ImplicitEuler.hh>
#include <G4NystromRK4.hh>
#include <G4RKG3_Stepper.hh>
#include <G4MagHelicalStepper.hh>
#include <G4HelixHeum.hh>
#include <G4HelixSimpleRunge.hh>
#include <G4HelixExplicitEuler.hh>
#include <G4HelixMixedStepper.hh>
#include <G4ExactHelixStepper.hh>
#include <G4HelixImplicitEuler.hh>
#include <G4MagErrorStepper.hh>
#include <G4CashKarpRKF45.hh>

//_____________________________________________________________________________
TG4MagneticField::TG4MagneticField(TVirtualMagField* magField)
  : G4MagneticField(),
    fVirtualMagField(magField)
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
void TG4MagneticField::GetFieldValue(const G4double point[3], G4double* bfield) const
{
/// Return the bfield values in the given point.

  // Set units
  const G4double g3point[3] = { point[0] / TG4G3Units::Length(),
                                point[1] / TG4G3Units::Length(),
                                point[2] / TG4G3Units::Length() };

  // Call user field
  fVirtualMagField->Field(g3point, bfield);

  // Set units
  for (G4int i=0; i<3; i++) bfield[i] = bfield[i] * TG4G3Units::Field();
}
