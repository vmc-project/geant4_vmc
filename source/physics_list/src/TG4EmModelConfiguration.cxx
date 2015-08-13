//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
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
TG4EmModelConfiguration::TG4EmModelConfiguration(const G4String& modelName)
  : fModelName(modelName),
    fParticles(),
    fRegions()
{
/// Standard constructor

  G4cout << "TG4EmModelConfiguration::TG4EmModelConfiguration " << modelName  << G4endl;
}

//_____________________________________________________________________________
TG4EmModelConfiguration::TG4EmModelConfiguration(const TG4EmModelConfiguration& rhs)
  : fModelName(rhs.fModelName),
    fParticles(rhs.fParticles),
    fRegions(rhs.fRegions)
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
  fModelName = rhs.fModelName;
  fParticles = rhs.fParticles;
  fRegions = rhs.fRegions;

  return *this;
}

//_____________________________________________________________________________
TG4EmModelConfiguration::~TG4EmModelConfiguration() 
{
/// Destructor
}
