//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2023 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4VRegionsManager.cxx
/// \brief Implementation of the TG4VRegionsManager class
///
/// \author I. Hrivnacova; IJCLab Orsay

#include "TG4VRegionsManager.h"
#include "TG4G3CutVector.h"
#include "TG4G3PhysicsManager.h"
#include "TG4G3Units.h"
#include "TG4GeometryServices.h"
#include "TG4Globals.h"
#include "TG4Limits.h"
#include "TG4Medium.h"
#include "TG4RegionsMessenger.h"

#include <G4LogicalVolumeStore.hh>
#include <G4ProductionCuts.hh>
#include <G4Region.hh>
#include <G4RegionStore.hh>
#include <G4RunManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>
#include <G4VUserPhysicsList.hh>
#include <G4Version.hh>

#include <map>
#include <set>
#include <fstream>

TG4VRegionsManager* TG4VRegionsManager::fgInstance = nullptr;

const G4String TG4VRegionsManager::fgkDefaultRegionName =
  "RegionWithDefaultCuts";
const G4String TG4VRegionsManager::fgkDefaultFileName = "regions.dat";

//_____________________________________________________________________________
TG4VRegionsManager::TG4VRegionsManager()
  : TG4Verbose("regionsManager")
{
  /// Default constructor

  fgInstance = this;
}

//_____________________________________________________________________________
TG4VRegionsManager::~TG4VRegionsManager()
{
  /// Destructor

  fgInstance = nullptr;
}

//
// protected methods
//

//_____________________________________________________________________________
G4double TG4VRegionsManager::GetGlobalEnergyCut(TG4G3Cut cutType) const
{
  /// Return global cut in energy of given cutType.
  /// Return DBL_MAX if cut value is not defined

  TG4boolVector* isCutVector =
    TG4G3PhysicsManager::Instance()->GetIsCutVector();
  TG4G3CutVector* cutVector = TG4G3PhysicsManager::Instance()->GetCutVector();

  G4double cutValue = DBL_MAX;
  if ((*isCutVector)[cutType] && (*cutVector)[cutType] > DBL_MIN) {
    cutValue = (*cutVector)[cutType];
  }

  return cutValue;
}

//_____________________________________________________________________________
G4double TG4VRegionsManager::GetEnergyCut(
  TG4Limits* limits, TG4G3Cut cutType, G4double globalCutValue) const
{
  /// Return cut in energy defined in limits of given cutType.
  /// Return DBL_MAX if cut value is not defined.

  G4double cut = DBL_MAX;
  if (limits->GetCutVector() && (*limits->GetCutVector())[cutType] > DBL_MIN) {
    cut = (*limits->GetCutVector())[cutType];
  }
  else {
    cut = globalCutValue;
  }

  return cut;
}

//_____________________________________________________________________________
G4bool TG4VRegionsManager::IsCoupleUsedInTheRegion(
  const G4MaterialCutsCouple* couple, const G4Region* region) const
{
  /// Reimplemented G4ProductionCutsTable::IsCoupleUsedInTheRegion
  /// which is declared private

  G4ProductionCuts* productionCuts = region->GetProductionCuts();
  std::vector<G4Material*>::const_iterator itm = region->GetMaterialIterator();
  size_t nofMaterials = region->GetNumberOfMaterials();
  for (size_t i = 0; i < nofMaterials; i++, itm++) {
    if (couple->GetMaterial() == (*itm) &&
        couple->GetProductionCuts() == productionCuts) {
      return true;
    }
  }

  return false;
}

///_____________________________________________________________________________
void TG4VRegionsManager::CheckRegionsInGeometry() const
{
  /// Loop over all logical volumes and check if the region to which
  /// the volume belongs correspond to its material

  if (VerboseLevel() > 0) {
    G4cout << ".. Checking regions materials " << G4endl;
  }

  Bool_t good = true;
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  for (G4int i = 0; i < G4int(lvStore->size()); i++) {

    G4LogicalVolume* lv = (*lvStore)[i];

    // skip world volume
    if (lv == TG4GeometryServices::Instance()->GetWorld()->GetLogicalVolume())
      continue;

    // skip volume without medium
    TG4Medium* medium =
      TG4GeometryServices::Instance()->GetMediumMap()->GetMedium(lv, false);
    if (!medium) continue;

    if (lv->GetRegion()->GetName() != lv->GetMaterial()->GetName() &&
        lv->GetRegion()->GetName() != fgkDefaultRegionName) {

      G4cout << "The region name " << lv->GetRegion()->GetName()
             << " for LV = " << lv->GetName()
             << " does not match its material name "
             << lv->GetMaterial()->GetName() << G4endl;

      good = false;
    }
  }
  if (good) {
    G4cout << ".. Regions are consistent with materials." << G4endl;
  }
  else {
    G4cout << ".. Found inconsistencies between regions and materials."
           << G4endl;
  }
}

//_____________________________________________________________________________
void TG4VRegionsManager::PrintLegend(std::ostream& output) const
{
  /// Print the range data legend

  //clang-format off
  output << std::setw(30) << std::left << "# material name"
         << "  rangeGam[mm]"
         << "  rangeEle[mm]"
         << "   cutGam[GeV]"
         << "   cutEle[GeV]"
         << " vmcCutGam[GeV]"
         << " vmcCutEle[GeV]" << G4endl;
  //clang-format on
}

//_____________________________________________________________________________
void TG4VRegionsManager::PrintRegionData(std::ostream& output,
  const G4String& matName, const TG4RegionData& values) const
{
  /// Print one region data.
  /// The regions names are printed within '' and separated from the following date
  /// with ';' to facilitate reading data back. The procedure then works only with
  /// material names that do not contain these two special characters.

  auto name = "\'" + matName + "\';";

  // Print all data
  //clang-format off
  output << std::setw(30) << std::left << name << "  "
         << std::scientific << values[fgkRangeGamIdx] << "  "
         << std::scientific << values[fgkRangeEleIdx] << "  "
         << std::scientific << values[fgkCutGamIdx] * TG4G3Units::InverseEnergy() << "  "
         << std::scientific << values[fgkCutEleIdx] * TG4G3Units::InverseEnergy() << "  "
         << std::scientific << values[fgkVmcCutGamIdx] * TG4G3Units::InverseEnergy() << "  "
         << std::scientific << values[fgkVmcCutEleIdx] * TG4G3Units::InverseEnergy() << G4endl;
  //clang-format on
}

//_____________________________________________________________________________
void TG4VRegionsManager::PrintFromG4Table(std::ostream& output) const
{
  /// Loop over the production cuts table and print the production ranges
  /// and cuts from the table and the VMC cuts from the TG4Limits
  /// associated with the production cuts material.

  G4ProductionCutsTable* productionCutsTable =
    G4ProductionCutsTable::GetProductionCutsTable();

  if (productionCutsTable->GetTableSize() == 0) {
    G4cout << "No production cuts defined." << G4endl;
    return;
  }

  PrintLegend(output);

  for (G4int i = 0; i < G4int(productionCutsTable->GetTableSize()); i++) {
    const G4MaterialCutsCouple* couple =
      productionCutsTable->GetMaterialCutsCouple(i);

    const G4Material* material = couple->GetMaterial();
    G4ProductionCuts* cuts = couple->GetProductionCuts();

    G4double rangeGam = cuts->GetProductionCut(0);
    G4double rangeEle = cuts->GetProductionCut(1);
    // if ( couple->IsRecalcNeeded() ) {
    //  TG4Globals::Warning("TG4VRegionsManager", "PrintProductionCuts",
    //     "Recalculation is needed - the energy cuts may be wrong");
    // }

    const std::vector<G4double>* energyCutsGam =
      productionCutsTable->GetEnergyCutsVector(0);
    const std::vector<G4double>* energyCutsEle =
      productionCutsTable->GetEnergyCutsVector(1);

    G4double cutGam = (*energyCutsGam)[couple->GetIndex()];
    G4double cutEle = (*energyCutsEle)[couple->GetIndex()];

    // Get limits via material
    TG4Limits* limits =
      TG4GeometryServices::Instance()->FindLimits(material, true);

    G4double cutGamLimits = DBL_MAX;
    G4double cutEleLimits = DBL_MAX;
    if (!limits) {
      TG4Globals::Warning("TG4VRegionsManager", "CheckRegions",
        "Limits for material " + TString(material->GetName()) + " not found. " +
          TG4Globals::Endl());
    }
    else {
      cutGamLimits = GetEnergyCut(limits, kCUTGAM, DBL_MAX);
      cutEleLimits = GetEnergyCut(limits, kCUTELE, DBL_MAX);
    }

    G4String matName = material->GetName();
    TG4RegionData values =
      {rangeGam, rangeEle, cutGam, cutEle, cutGamLimits, cutEleLimits};

    PrintRegionData(output, matName, values);
  }
}

//
// public methods
//

//_____________________________________________________________________________
void TG4VRegionsManager::PrintRegions(std::ostream& output) const
{
  /// Print regions from the production cuts table.

  PrintFromG4Table(output);
}

//_____________________________________________________________________________
void TG4VRegionsManager::CheckRegions() const
{
  /// Check if the region to which the volume belongs correspond to its material

  CheckRegionsInGeometry();
}

//_____________________________________________________________________________
void TG4VRegionsManager::SaveRegions()
{
  /// Dump all regions data in a file

  // Open file
  auto fileName = fFileName.empty() ? fgkDefaultFileName : fFileName;
  std::ofstream fileOutput;
  fileOutput.open(fileName, std::ios::out);
  if (! fileOutput.is_open()) {
    TG4Globals::Warning("TG4VRegionsManager", "SaveRegions",
      "Saving regions in file " + TString(fileName.data()) + " has failed.");
    return;
  }

  if (VerboseLevel() > 0) {
    G4cout << "Saving regions from production cuts table in file: " << fileName << G4endl;
  }

  PrintRegions(fileOutput);
  fileOutput.close();
}

//_____________________________________________________________________________
void TG4VRegionsManager::DumpRegionStore() const
{
  /// Dump all region properties:
  /// production cuts, volumes list and material list.

  auto regionStore = G4RegionStore::GetInstance();

  G4cout << "========= Region Store Dump ======================================"
         << G4endl;

  for (auto region : *regionStore ) {
    G4cout << region->GetName() << ":" << G4endl;

    auto cuts = region->GetProductionCuts();
    if (cuts != nullptr) {
      auto rangeGam = cuts->GetProductionCut(0);
      auto rangeEle = cuts->GetProductionCut(1);
      G4cout << "  ProductionCuts: rangeGam=" << rangeGam << "  rangeEle=" << rangeEle << G4endl;
    }

    size_t lvCounter = 0;
    auto lvIt = region->GetRootLogicalVolumeIterator();
    G4cout << "  RootVolumes: ";
    while (lvCounter < region->GetNumberOfRootVolumes()) {
      G4cout << " " << (*lvIt++)->GetName() << "; ";
      ++lvCounter;
    }
    G4cout << G4endl;;

    size_t matCounter = 0;
    auto matIt = region->GetMaterialIterator();
    G4cout << "  Materials: ";
    while (matCounter < region->GetNumberOfMaterials()) {
      G4cout << " " << (*matIt)->GetName() << "; ";;
      ++matCounter;
    }
    G4cout << G4endl;;
  }

  G4cout << "========= End Region Store Dump =================================="
         << G4endl;
}
