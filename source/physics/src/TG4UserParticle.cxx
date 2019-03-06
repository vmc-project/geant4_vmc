//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4UserParticle.cxx
/// \brief Implementation of the TG4UserParticle class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4UserParticle.h"

//_____________________________________________________________________________
TG4UserParticle::TG4UserParticle(
       const G4String& name,
       TMCParticleType mcType,
       G4double        mass,
       G4double        width,
       G4double        charge,
       G4int           iSpin,
       G4int           iParity,
       G4int           iConjugation,
       G4int           iIsospin,
       G4int           iIsospin3,
       G4int           gParity,
       const G4String& pType,
       G4int           lepton,
       G4int           baryon,
       G4int           encoding,
       G4bool          stable,
       G4double        lifetime,
       G4DecayTable    *decaytable ,
       G4bool          shortlived,
       const G4String& subType,
       G4int           anti_encoding,
       G4double        magneticMoment,
       G4double        excitation      )
  : G4ParticleDefinition(
       name, mass, width, charge, iSpin, iParity,
       iConjugation, iIsospin, iIsospin3, gParity, pType,
       lepton, baryon, encoding, stable, lifetime, decaytable,
       shortlived, subType, anti_encoding, magneticMoment),
    fMCType(mcType),
    fExcitationEnergy(excitation)
{
/// Standard constructor
}

TG4UserParticle::~TG4UserParticle()
{
/// Destructor
}








