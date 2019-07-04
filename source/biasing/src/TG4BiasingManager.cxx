//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2019 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4BiasingManager.cxx
/// \brief Implementation of the TG4BiasingManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4BiasingManager.h"
#include "TG4BiasingOperator.h"
#include "TG4ModelConfiguration.h"

#include <G4AnalysisUtilities.hh>
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4Material.hh>

//_____________________________________________________________________________
TG4BiasingManager::TG4BiasingManager(
  const G4String& name, const G4String& availableModels)
  : TG4ModelConfigurationManager(name, availableModels)
{
  /// Standard constructor

  if (VerboseLevel() > 1) {
    G4cout << "TG4BiasingManager::TG4BiasingManager"
           << G4endl;
  }
}

//_____________________________________________________________________________
TG4BiasingManager::~TG4BiasingManager()
{
  /// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void TG4BiasingManager::CreateBiasingOperator()
{
  /// Create biasing operator

  if (VerboseLevel() > 1) {
    G4cout << "TG4BiasingManager::CreateBiasingOperator" << G4endl;
  }

  // Return if no models are registered
  if (!GetVector().size()) return;

  // Only one "model" is currently supported
  if (GetVector().size() > 1) {
    // Add warning
    return;
  }

  // Generate new regions names based on material names
  SetRegionsNames();

  // Get biasing "model" configuration
  // (only one "model" is currently supported)
  TG4ModelConfiguration* modelConfiguration = GetVector().at(0);

  // Get particles as a vector
  std::vector<G4String> particlesVector;
  if (modelConfiguration->GetParticles().size()) {
    // use analysis utility to tokenize regions
    G4Analysis::Tokenize(modelConfiguration->GetParticles(), particlesVector);
  }

  // Create biasingOperator
  TG4BiasingOperator* biasingOperator = new TG4BiasingOperator();

  // Add particles
  for (auto it = particlesVector.begin(); it != particlesVector.end(); it++) {  
    biasingOperator->AddParticle((*it));
  }

  // Loop over logical volumes
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  for (G4int i = 0; i < G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];
    G4String materialName = lv->GetMaterial()->GetName();

    if (VerboseLevel() > 2) {
      G4cout << "Biasing manager: processing volume " << lv->GetName() << ", material "
             << materialName << G4endl;
    }

    // Skip volumes with materials which are not in the regions list
    if (!modelConfiguration->HasRegion(materialName)) { 
      if (VerboseLevel() > 2) {
        G4cout << "   Material " << materialName << " is not in selection"
               << G4endl;
      }
      continue;
    }

    // Attach biasing operator to the logical volume
    biasingOperator->AttachTo(lv);

    if (VerboseLevel() > 1) {
      G4cout << "Biasing operator attached to lv " << lv->GetName() << G4endl;
    }
  }
}
