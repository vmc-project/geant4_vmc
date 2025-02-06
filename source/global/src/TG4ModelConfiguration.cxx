//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ModelConfiguration.cxx
/// \brief Implementation of the TG4ModelConfiguration class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ModelConfiguration.h"
#include "TG4Globals.h"

#include <G4AnalysisUtilities.hh>

#include <algorithm>

namespace
{

void PrintNamesVector(const std::vector<G4String> names)
{
  std::vector<G4String>::const_iterator it;
  for (it = names.begin(); it != names.end(); it++) {
    G4cout << (*it) << " ";
  }
}

} // namespace

//_____________________________________________________________________________
TG4ModelConfiguration::TG4ModelConfiguration(const G4String& modelName)
  : fModelName(modelName),
    fParticles(),
    fRegionsMedia(),
    fRegions(),
    fFastSimulationModel(0)
{
  /// Standard constructor
}

//_____________________________________________________________________________
void TG4ModelConfiguration::SetRegionsMedia(const G4String& regionsMedia)
{
  /// Set the list of regions.
  /// Can be used only for region names that do not contain spaces,
  /// the names with spaces must be added individually.
  ///

  std::vector<G4String> mediaVector;
  if (regionsMedia.size()) {
    // use analysis utility to tokenize regions
    G4Analysis::Tokenize(regionsMedia, mediaVector);
  }

  // append the medias
  fRegionsMedia.insert(
    fRegionsMedia.end(), mediaVector.begin(), mediaVector.end());
}

//_____________________________________________________________________________
void TG4ModelConfiguration::SetOneRegionMedium(const G4String& regionMedium)
{
  // append the medium
  fRegionsMedia.push_back(regionMedium);
}

//_____________________________________________________________________________
void TG4ModelConfiguration::SetOneRegion(const G4String& region)
{
  // append the region
  fRegions.push_back(region);
}

//_____________________________________________________________________________
void TG4ModelConfiguration::Print() const
{
  /// Print all data

  G4cout << "Model configuration: " << fModelName << G4endl;
  G4cout << "particles: " << fParticles << G4endl;
  G4cout << "media:     ";
  PrintNamesVector(fRegionsMedia);
  G4cout << G4endl;
  G4cout << "regions:   ";
  PrintNamesVector(fRegions);
  G4cout << G4endl;
}

//_____________________________________________________________________________
G4bool TG4ModelConfiguration::HasParticle(const G4String& particleName)
{
  /// Return true if given particle is in the particles list and is not
  /// in the excluded list

  return
    (fParticles == "all" || TG4Globals::Contains(particleName, fParticles)) &&
    ! (TG4Globals::Contains(particleName, fExcludedParticles));
}

//_____________________________________________________________________________
G4bool TG4ModelConfiguration::HasRegion(const G4String& regionName)
{
  /// Return true if given regionName is in the regions list

  return (
    std::find(fRegions.begin(), fRegions.end(), regionName) != fRegions.end());
}
