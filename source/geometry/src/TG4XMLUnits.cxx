// $Id: TG4XMLUnits.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: global
//
// Author: I. Hrivnacova
//
// Class TG4XMLUnits
// ----------------
// See the class description in the header file.

#include "TG4XMLUnits.h"

// static const data members

const G4double TG4XMLUnits::fgkLength  = cm;
const G4double TG4XMLUnits::fgkAngle   = deg;
const G4double TG4XMLUnits::fgkTime    = s;
const G4double TG4XMLUnits::fgkCharge  = eplus;
const G4double TG4XMLUnits::fgkEnergy  = GeV;
const G4double TG4XMLUnits::fgkMass    = GeV;
const G4double TG4XMLUnits::fgkMassDensity  = g/cm3;
const G4double TG4XMLUnits::fgkAtomicWeight = g/mole;
const G4double TG4XMLUnits::fgkField   = kilogauss;

//_____________________________________________________________________________
TG4XMLUnits::TG4XMLUnits() {
//
}
  
//_____________________________________________________________________________
TG4XMLUnits::~TG4XMLUnits() {
//
}

