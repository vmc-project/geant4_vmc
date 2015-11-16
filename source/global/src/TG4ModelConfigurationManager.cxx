//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ModelConfigurationManager.cxx
/// \brief Implementation of the TG4ModelConfigurationManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ModelConfigurationManager.h"
#include "TG4ModelConfigurationMessenger.h"
#include "TG4ModelConfiguration.h"
#include "TG4PhysicsManager.h"
#include "TG4GeometryServices.h"
#include "TG4MediumMap.h"
#include "TG4Medium.h"
#include "TG4Limits.h"
#include "TG4G3ControlVector.h"

#include <G4TransportationManager.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4LogicalVolume.hh>
#include <G4RegionStore.hh>
#include <G4Region.hh>
#include <G4ProductionCuts.hh>
#include <G4Material.hh>

//_____________________________________________________________________________
TG4ModelConfigurationManager::TG4ModelConfigurationManager(const G4String& name,
                                                const G4String& availableModels)
  : TG4Verbose("modelConfigurationManager"),
    fMessenger(0),
    fName(name),
    fAvailableModels(availableModels),
    fVector(),
    fCreateRegionsDone(false)
{
/// Standard constructor

  if ( VerboseLevel() > 1 ) {
    G4cout << "TG4ModelConfigurationManager::TG4ModelConfigurationManager" << G4endl;
  }

  fMessenger = new TG4ModelConfigurationMessenger(this, availableModels);
}

//_____________________________________________________________________________
TG4ModelConfigurationManager::~TG4ModelConfigurationManager()
{
/// Destructor

  delete fMessenger;

  ModelConfigurationVector::iterator it;
  for ( it = fVector.begin(); it != fVector.end(); it++ ) {
    delete *it;
  }
}

//
// public methods
//

//_____________________________________________________________________________
void TG4ModelConfigurationManager::CreateRegions()
{
/// Create regions for all registered models
  
  if ( VerboseLevel() > 1 ) {
    G4cout << "TG4ModelConfigurationManager::ConstructRegions" << G4endl;
  }

  // Return if regions were already created
  if ( fCreateRegionsDone ) return;

  // Loop over logical volumes
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  for (G4int i=0; i<G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];

    TG4Medium* medium
      = TG4GeometryServices::Instance()->GetMediumMap()->GetMedium(lv, false);
    if ( ! medium ) continue;

    G4String mediumName = medium->GetName();
    G4String materialName = lv->GetMaterial()->GetName();

    if ( VerboseLevel() > 2 ) {
      G4cout << "Processing volume " << lv->GetName()
             << ", medium " << mediumName
             << ", material " << materialName << G4endl;
    }

    // Skip volumes with media which are not in the regions list
    G4bool isModelRegion = false;
    ModelConfigurationVector::const_iterator it;
    for ( it = fVector.begin(); it != fVector.end(); it++ ) {
      if ( (*it)->HasRegion(mediumName) ) isModelRegion = true;
    }

    if ( ! isModelRegion ) {
      if ( VerboseLevel() > 2 ) {
        G4cout << "Medium " << mediumName << " is not in selection" << G4endl;
      }
      continue;
    }

    // If region already exists, only add the logical volume
    // and continue the loop 
    G4Region* region
      = G4RegionStore::GetInstance()->GetRegion(materialName, false );

    if ( ! region ) {
      region = new G4Region(materialName);

      if ( VerboseLevel() > 1 ) {
        G4cout << "Created region " << region->GetName() << region << G4endl;
      }
    }

    if ( VerboseLevel() > 1 ) {
      G4cout << "Adding region " << region->GetName() << " to lv " << lv->GetName() << G4endl;
    }
    region->AddRootLogicalVolume(lv);
    lv->SetRegion(region);
  }

  fCreateRegionsDone = true;
}  

//_____________________________________________________________________________
void TG4ModelConfigurationManager::SetModel(const G4String& modelName)
{
/// Set an extra EM model with the given name.

  if ( ! GetModelConfiguration(modelName, false) ) {
    fVector.push_back(new TG4ModelConfiguration(modelName));
  }
  else {
    TString text = "Cannot create model ";
    text +=  modelName.data();
    text += " twice.";
    TG4Globals::Warning(
      "TG4ModelConfigurationManager", "SetModel", text);
  }
}

//_____________________________________________________________________________
void TG4ModelConfigurationManager::SetModelParticles(const G4String& modelName,
                                       const G4String& particles)
{
/// Set particles for the physics model for given medium.

  TG4ModelConfiguration* modelConfiguration
    = GetModelConfiguration(modelName, "SetModelParticles");

  if ( ! modelConfiguration ) {
    TString text = "The model configuration ";
    text +=  modelName.data();
    text += " is not defined.";
    TG4Globals::Warning(
      "TG4ModelConfigurationManager", "SetModelParticles", 
      text + TG4Globals::Endl()
      + TString("Setting will be ignored."));
    return;
  }

  modelConfiguration->SetParticles(particles);
}

//_____________________________________________________________________________
void TG4ModelConfigurationManager::SetModelRegions(const G4String& modelName,
                                     const G4String& regions)
{
/// Set regions for the physics model for given medium.

  TG4ModelConfiguration* modelConfiguration
    = GetModelConfiguration(modelName, "SetModelRegions");

  if ( ! modelConfiguration ) {
    TString text = "The model configuration ";
    text +=  modelName.data();
    text += " is not defined.";
    TG4Globals::Warning(
      "TG4ModelConfigurationManager", "SetModelRegions", 
      text + TG4Globals::Endl()
      + TString("Setting will be ignored."));
    return;
  }

  modelConfiguration->SetRegions(regions);
}

//_____________________________________________________________________________
TG4ModelConfiguration* 
TG4ModelConfigurationManager::GetModelConfiguration(const G4String& modelName, 
                                                    G4bool warn) const
{
/// Return the model configuration via specified model name;
/// print warning if a configuration is not found

  ModelConfigurationVector::const_iterator it;
  for ( it = fVector.begin(); it != fVector.end(); it++ ) {
    if ( (*it)->GetModelName() == modelName ) return *it;
  }

  if ( warn ) {
    TString text = "Model configuration ";
    text +=  modelName.data();
    text += " does not exist";
    TG4Globals::Warning(
      "TG4ModelConfigurationManager", "GetModelConfiguration", text);
  }

  return 0;
}
