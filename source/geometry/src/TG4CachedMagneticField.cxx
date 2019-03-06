//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4CachedMagneticField.cxx
/// \brief Implementation of the TG4CachedMagneticField class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4CachedMagneticField.h"
#include "TG4G3Units.h"
#include "TG4Globals.h"

#include <TVirtualMCApplication.h>
#include <TVirtualMC.h>

//_____________________________________________________________________________
TG4CachedMagneticField::TG4CachedMagneticField(TVirtualMagField* magField,
                                               G4double constDistance)
  : TG4MagneticField(magField),
    fLastLocation(),
    fLastValue(),
    fCallsCounter(0),
    fEvaluationsCounter(0),
    fConstDistanceSquare(constDistance * constDistance)
{
/// Default constructor
}

//_____________________________________________________________________________
TG4CachedMagneticField::~TG4CachedMagneticField()
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void TG4CachedMagneticField::GetFieldValue(const G4double point[3], G4double* bfield) const
{
/// Return the bfield values in the given point.

  G4ThreeVector newLocation(point[0], point[1], point[2]);
  G4double distSq = (newLocation - fLastLocation).mag2();
  ++fCallsCounter;

  // Use cached value if within the constant distance
  if ( distSq < fConstDistanceSquare ) {
     bfield[0] = fLastValue.x();
     bfield[1] = fLastValue.y();
     bfield[2] = fLastValue.z();
     return;
  }

  // New evaluation
  // Set units
  const G4double g3point[3] = { point[0] / TG4G3Units::Length(),
                                point[1] / TG4G3Units::Length(),
                                point[2] / TG4G3Units::Length() };

  // Call user field
  fVirtualMagField->Field(g3point, bfield);

  // Set units
  for (G4int i=0; i<3; i++) bfield[i] = bfield[i] * TG4G3Units::Field();

  // Update counter and cache new values
  ++fEvaluationsCounter;
  fLastLocation = newLocation;
  fLastValue = G4ThreeVector(bfield[0], bfield[1], bfield[2]);
}

// //_____________________________________________________________________________
// void TG4CachedMagneticField::Update(const TG4FieldParameters& parameters)
// {
// /// Update field with new field parameters

//   // Update parameters in base class
//   // TG4MagneticField::Update(parameters);

//   // Const distance square
//   fConstDistanceSquare
//     = parameters.GetConstDistance()*parameters.GetConstDistance();
// }

//_____________________________________________________________________________
void TG4CachedMagneticField::PrintStatistics() const
{
/// Print the caching statistics

  if ( fConstDistanceSquare ) {
    G4cout << " Cached field: " << G4endl
	   << "   Number of calls:        " << fCallsCounter << G4endl
	   << "   Number of evaluations : " << fEvaluationsCounter << G4endl;
  }
}

//_____________________________________________________________________________
void TG4CachedMagneticField::SetConstDistance(G4double value)
{
/// Set new const distance value

   fConstDistanceSquare = value * value;
}

//_____________________________________________________________________________
void TG4CachedMagneticField::ClearCounter()
{
/// Clear counters

  fCallsCounter = 0;
  fEvaluationsCounter = 0;
}
