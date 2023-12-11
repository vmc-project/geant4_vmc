//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2023 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4RegionsManager2.cxx
/// \brief Implementation of the TG4RegionsManager2 class
///
/// \author I. Hrivnacova; IJCLab Orsay

#include "TG4RegionsManager2.h"
#include "TG4GeometryServices.h"
#include "TG4Globals.h"
#include "TG4Limits.h"
#include "TG4Medium.h"
#include "TG4PhysicsManager.h"

#include <G4LogicalVolumeStore.hh>
#include <G4Material.hh>
#include <G4ProductionCuts.hh>
#include <G4ProductionCutsTable.hh>
#include <G4Region.hh>
#include <G4RegionStore.hh>
#include <G4SystemOfUnits.hh>

//_____________________________________________________________________________
TG4RegionsManager2::TG4RegionsManager2()
 : fMessenger(this)
{}

//
// public methods
//

//_____________________________________________________________________________
void TG4RegionsManager2::DefineRegions()
{
  /// Define regions according to tracking media

  if (VerboseLevel() > 0) {
    G4cout << "Define regions for VMC cuts per materials." << G4endl;
  }

  // Get medium map
  auto mediumMap = TG4GeometryServices::Instance()->GetMediumMap();
  // Get G4 region store
  auto g4RegionStore = G4RegionStore::GetInstance();

  // Get world volume & material
  auto worldLV = TG4GeometryServices::Instance()->GetWorld()->GetLogicalVolume();
  auto worldMaterial = worldLV->GetMaterial();

  // Get default range cut values from physics manager
  auto defaultRangeCutEle =
    TG4PhysicsManager::Instance()->GetCutForElectron();
  auto defaultRangeCutGam = TG4PhysicsManager::Instance()->GetCutForGamma();
  auto defaultRangeCutPositron =
    TG4PhysicsManager::Instance()->GetCutForPositron();
  auto defaultRangeCutProton =
    TG4PhysicsManager::Instance()->GetCutForProton();

  // Define region for each logical volume
  G4int counter = 0;
  auto lvStore = G4LogicalVolumeStore::GetInstance();

  for (std::size_t i = 0; i < lvStore->size(); ++i) {

    auto lv = (*lvStore)[i];
    auto materialName = lv->GetMaterial()->GetName();
    // print debug message
    if (VerboseLevel() > 1) {
      G4cout << "-- Volume = " << i << "  " << lv->GetName()
             << " material = " << materialName << G4endl;
    }

    // skip world
    if (lv == worldLV) {
      if (VerboseLevel() > 1) {
        G4cout << "   " << "skipping worldVolume" << G4endl;
      }
      continue;
    }

    // skip volume if it has already a region assigned
    if (lv->GetRegion() != nullptr &&
        lv->GetRegion()->GetName() == materialName) {
        if (VerboseLevel() > 1) {
           G4cout << "   "
                  << "has already region set, skipping" << G4endl;
        }
        continue;
    }

    // Get region fior this material, if it already exists,
    // and add the logical volume
    auto regionName = materialName;
    auto region = g4RegionStore->GetRegion(regionName, false);
    if (region != nullptr) {
      if (lv->GetRegion() != region) {
        // Add volume to the region per material if its region is different
        // (by default the volume has the DefaultRegionForTheWorld)
        if (VerboseLevel() > 1) {
          G4cout << "   "
                 << "adding volume in region = " << regionName << G4endl;
        }
        region->AddRootLogicalVolume(lv);
      }
      continue;
    }

    // After this line, the region does not exist
    if (VerboseLevel() > 1) {
      G4cout << "   "
             << "creating new region = " << regionName << G4endl;
    }
    region = new G4Region(regionName);
    region->AddRootLogicalVolume(lv);
    ++counter;
  }

  if (fIsCheck) {
    CheckRegionsInGeometry();
  }

  if (VerboseLevel() > 0) {
    G4cout << "Number of added regions: " << counter << G4endl;
  }
}

//_____________________________________________________________________________
void TG4RegionsManager2::UpdateProductionCutsTable()
{
  /// Update production cuts table according to stored region information

  if (VerboseLevel() > 1) {
    G4cout << "Update production cuts table" << G4endl;
  }

  auto mediumMap = TG4GeometryServices::Instance()->GetMediumMap();
  auto g4ProductionCutsTable = G4ProductionCutsTable::GetProductionCutsTable();

  // Global energy cuts
  auto cutEleGlobal = GetGlobalEnergyCut(kCUTELE);
  auto cutGamGlobal = GetGlobalEnergyCut(kCUTGAM);

  // cut vectors for gamma, e-, e+, proton
  std::vector<G4double> gamCuts; 
  std::vector<G4double> eleCuts; 

  // G4cout << "g4RegionStore size: " << G4RegionStore::GetInstance()->size() << G4endl;
  // G4cout << "g4ProductionCutsTable size: " << g4ProductionCutsTable->GetTableSize() << G4endl;

  // update table (create materials cut couples)
  g4ProductionCutsTable->CreateCoupleTables(); 
  // G4cout << "g4ProductionCutsTable size after update: " << g4ProductionCutsTable->GetTableSize() << G4endl;

  for (std::size_t i = 0; i < g4ProductionCutsTable->GetTableSize(); ++i) {
    auto couple = g4ProductionCutsTable->GetMaterialCutsCouple(i);
    auto material = couple->GetMaterial();
    auto medium = mediumMap->GetMedium(material);

    auto limits = (TG4Limits*)medium->GetLimits();
    auto cutEle = GetEnergyCut(limits, kCUTELE, cutEleGlobal);
    auto cutGam = GetEnergyCut(limits, kCUTGAM, cutGamGlobal);

    gamCuts.push_back(cutGam);
    eleCuts.push_back(cutEle);
  }

  g4ProductionCutsTable->SetEnergyCutVector(gamCuts, 0);
  g4ProductionCutsTable->SetEnergyCutVector(eleCuts, 1);
}
