// $Id: TG4G3Units.cxx,v 1.1.1.1 2002/06/16 15:57:35 hristov Exp $
// Category: global
//
// Author: I. Hrivnacova
//
// Class TG4G3Units
// ----------------
// See the class description in the header file.

#include "TG4G3Units.h"

// static const data members

const G4double TG4G3Units::fgkLength  = cm;
const G4double TG4G3Units::fgkAngle   = deg;
const G4double TG4G3Units::fgkTime    = s;
const G4double TG4G3Units::fgkCharge  = eplus;
const G4double TG4G3Units::fgkEnergy  = GeV;
const G4double TG4G3Units::fgkMass    = GeV;
const G4double TG4G3Units::fgkMassDensity  = g/cm3;
const G4double TG4G3Units::fgkAtomicWeight = g/mole;
const G4double TG4G3Units::fgkField   = kilogauss;

//_____________________________________________________________________________
TG4G3Units::TG4G3Units() {
//
}
  
//_____________________________________________________________________________
TG4G3Units::~TG4G3Units() {
//
}

