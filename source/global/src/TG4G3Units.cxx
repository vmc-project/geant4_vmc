//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4G3Units.cxx
/// \brief Implementation of the TG4G3Units class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4G3Units.h"

#include <G4SystemOfUnits.hh>

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
TG4G3Units::~TG4G3Units()
{
/// Destructor
}

