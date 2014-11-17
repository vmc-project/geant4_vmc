//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4EmModelConfiguration.cxx
/// \brief Implementation of the TG4EmModelConfiguration class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4EmModelConfiguration.h"

//_____________________________________________________________________________
TG4EmModelConfiguration::TG4EmModelConfiguration(
                            const G4String& elossModelName,
                            const G4String& fluctModelName,
                            const G4String& particles)
  : fElossModelName(elossModelName),
    fFluctModelName(fluctModelName),
    fParticles(particles)   
{
/// Standard constructor

  G4cout << "TG4EmModelConfiguration::TG4EmModelConfiguration" << G4endl;
}

//_____________________________________________________________________________
TG4EmModelConfiguration::TG4EmModelConfiguration(const TG4EmModelConfiguration& rhs)
  : fElossModelName(rhs.fElossModelName),
    fFluctModelName(rhs.fFluctModelName),
    fParticles(rhs.fParticles)   
{
/// Copy constructor

  G4cout << "TG4EmModelConfiguration::TG4EmModelConfiguration(rhs)" << G4endl;
}

//_____________________________________________________________________________
TG4EmModelConfiguration& 
TG4EmModelConfiguration::operator=(const TG4EmModelConfiguration& rhs)
{
/// Assignment operator
 
  // check assignment to self
  if (this == &rhs) return *this;

  // assignment operator
  fElossModelName = rhs.fElossModelName;
  fFluctModelName = rhs.fFluctModelName;
  fParticles = rhs.fParticles;   

  return *this;
}

//_____________________________________________________________________________
TG4EmModelConfiguration::~TG4EmModelConfiguration() 
{
/// Destructor
}
