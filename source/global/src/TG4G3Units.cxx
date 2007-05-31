// $Id: TG4G3Units.cxx,v 1.3 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: global
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
TG4G3Units::~TG4G3Units() {
//
}

