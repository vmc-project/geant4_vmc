//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4RegionsManager.cxx
/// \brief Implementation of the TG4RegionsManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RegionsManager.h"
#include "TG4G3CutVector.h"
#include "TG4G3PhysicsManager.h"
#include "TG4G3Units.h"
#include "TG4GeometryServices.h"
#include "TG4Globals.h"
#include "TG4Limits.h"
#include "TG4Medium.h"
#include "TG4PhysicsManager.h"
#include "TG4RegionsMessenger.h"

#include <G4Gamma.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4ProductionCuts.hh>
#include <G4RToEConvForElectron.hh>
#include <G4RToEConvForGamma.hh>
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

//_____________________________________________________________________________
TG4RegionsManager::TG4RegionsManager()
 : fMessenger(this)
{}

//
// private methods
//

//_____________________________________________________________________________
G4bool TG4RegionsManager::Iterate(G4double energyCut, G4double& lowerCut,
  G4double& higherCut, G4double defaultRangeCut, G4double lowEdgeEnergy,
  G4double highEdgeEnergy, G4int nbin,
  std::map<G4double, G4double>& energyToRangeMap,
  std::map<G4double, G4double>::const_iterator& it, G4Material* material,
  G4VRangeToEnergyConverter& converter) const
{
  /// Helper function called from ConvertEnergyToRange() in each iteration

  G4double step = (higherCut - lowerCut) / nbin;
  energyToRangeMap.clear();
  G4String indent("     ");
  for (G4int i = 0; i <= nbin; i++) {
    G4double rangeCut = lowerCut + i * step;
    if (rangeCut < defaultRangeCut) continue;
    G4double energy = converter.Convert(rangeCut, material);
    if (VerboseLevel() > 2) {
      G4cout << indent << "For range: " << rangeCut << " mm  got energy "
             << energy / MeV << " MeV" << G4endl;
    }

    if (energy < lowEdgeEnergy) energy = lowEdgeEnergy;
    if (energy > highEdgeEnergy) energy = highEdgeEnergy;

    energyToRangeMap.insert(
      std::pair<G4double, G4double>(energy / MeV, rangeCut));
  }

  // Now find the closest value of energy
  it = energyToRangeMap.lower_bound(energyCut);

  if (it == energyToRangeMap.end()) {
    if (VerboseLevel() > 2) {
      G4cout << indent << "Range  not found " << G4endl;
    }
    return false;
  }
  else {
    if (VerboseLevel() > 2) {
      G4cout << indent << "Found range limit: " << it->second << " mm"
             << G4endl;
    }
    return true;
  }
}

//_____________________________________________________________________________
std::pair<G4double,G4double>
TG4RegionsManager::ConvertEnergyToRange(G4double energyCut,
  G4Material* material, G4VRangeToEnergyConverter& converter,
  G4double defaultRangeCut) const
{
  /// Estimate cut in range for given cut in energy.
  /// Return negative value if range cut is not fount.

  G4double lowEdgeEnergy = converter.GetLowEdgeEnergy();
  G4double highEdgeEnergy = converter.GetHighEdgeEnergy();

  // First compute default energy cut
  std::map<G4double, G4double> energyToRangeMap;
  G4double energy = converter.Convert(defaultRangeCut, material);
  if (energy < lowEdgeEnergy) energy = lowEdgeEnergy;
  if (energy > highEdgeEnergy) energy = highEdgeEnergy;
  energyToRangeMap.insert(
    std::pair<G4double, G4double>(energy / MeV, defaultRangeCut));

  // Compute the range order where we will start iterating
  // withing the orders in between fgkMinRangeOrder and fgkMaxRangeOrder
  // (1e-03mm, 1e-02, 1e-01, 1mm, 1cm, 10 cm, 1m, 10m, 100m, 1000m )
  G4String indent("     ");
  for (G4int i = fgkMinRangeOrder; i <= fgkMaxRangeOrder; i++) {
    G4double rangeCut = pow(10., i);
    if (rangeCut < defaultRangeCut) continue;
    energy = converter.Convert(rangeCut, material);
    if (VerboseLevel() > 2) {
      G4cout << indent << "For range: " << std::setw(5) << rangeCut
             << " mm  got energy " << energy / MeV << " MeV" << G4endl;
    }

    if (energy < lowEdgeEnergy) energy = lowEdgeEnergy;
    if (energy > highEdgeEnergy) energy = highEdgeEnergy;
    energyToRangeMap.insert(
      std::pair<G4double, G4double>(energy / MeV, rangeCut));
  }

  // If energyCut is above the maximum range order return the last
  // computed value
  if (energyToRangeMap.rbegin()->first < energyCut) {
    if (VerboseLevel() > 2) {
      G4cout << indent << "Outside range, return the highest value " << G4endl;
    }
    return *(energyToRangeMap.rbegin());
  }

  // Now find the closest value of energy which is
  // equal or higher than given energyCut
  std::map<G4double, G4double>::const_iterator it =
    energyToRangeMap.lower_bound(energyCut);

  if (it == energyToRangeMap.end()) {
    if (VerboseLevel() > 2) {
      G4cout << indent << "Range not found " << G4endl;
    }
    return {-1.0, -1.0};
  }

  if (VerboseLevel() > 2) {
    G4cout << indent << "Found range limit: " << it->second << " mm" << G4endl;
  }

  // Now iterate up to desired precision of range
  G4int iteration = 0;
  while (iteration < fRangePrecision) {
    G4int nbin = fgkNofBins;
    if (iteration == 0) --nbin;

    auto higherCutIt = it;
    auto higherCut = it->second;
    // G4cout << "higherCut = " << higherCut << G4endl;
    if (it == energyToRangeMap.begin()) return *higherCutIt;

    --it;
    auto lowerCutIt = it;
    auto lowerCut = it->second;
    // G4cout << "lowerCut = " << lowerCut << G4endl;
    G4bool found =
      Iterate(energyCut, lowerCut, higherCut, defaultRangeCut, lowEdgeEnergy,
        highEdgeEnergy, nbin, energyToRangeMap, it, material, converter);

    if (!found) {
      // Now we have to go one step back to get below the user cut value
      // unless we are at the beginning of the map
      if (it == energyToRangeMap.begin()) return *higherCutIt;
      --it;
      return *it;
      ;
    }
    ++iteration;
  }

  // Now we have to go one step back to get below the user cut value
  // unless we are at the beginning of the map
  // if (it == energyToRangeMap.begin()) return it->second;
  if (it == energyToRangeMap.begin()) return *it;
  --it;
  return *it;
  ;
}

//_____________________________________________________________________________
std::pair<G4double,G4double>
TG4RegionsManager::GetRangeCut(G4double energyCut,
  G4Material* material, G4VRangeToEnergyConverter& converter,
  G4double defaultRangeCut) const
{
  /// Convert energy cuts in range cuts;
  /// return defaultRangeCut, if found value is smaller than default

  if (energyCut == DBL_MAX) {
    if (VerboseLevel() > 1) {
      G4cout << "   " << converter.GetParticleType()->GetParticleName()
             << " energy cut not defined, keeping default range" << G4endl;
    }
    return {converter.GetLowEdgeEnergy(), defaultRangeCut};
  }

  if (VerboseLevel() > 1) {
    G4cout << "   " << converter.GetParticleType()->GetParticleName()
           << ": energy cut = " << energyCut << " MeV" << G4endl;
  }

  if (fIsLoad) {
    auto it = fRegionData.find(material->GetName());
    if (it != fRegionData.end()) {
      auto [rangeCut, energyCut] =
        (converter.GetParticleType() == G4Gamma::Definition()) ?
          std::pair(it->second[fgkRangeGamIdx], it->second[fgkCutGamIdx]) :
          std::pair(it->second[fgkRangeEleIdx], it->second[fgkCutEleIdx]);

      if (VerboseLevel() > 1) {
        G4cout << "  " << converter.GetParticleType()->GetParticleName()
               << " loaded range, cut values: "
               << rangeCut << ", " << energyCut << G4endl;
      }
      return {energyCut, rangeCut};
    }
    else {
      if (VerboseLevel() > 1) {
        G4cout << "  " << converter.GetParticleType()->GetParticleName()
               << " rangeCut not found, keeping default range" << G4endl;
      }
      return {converter.GetLowEdgeEnergy(), defaultRangeCut};
    }
  }

  auto [calcEnergyCut, rangeCut]  =
    ConvertEnergyToRange(energyCut, material, converter, defaultRangeCut);

  if (rangeCut < 0.) {
    if (VerboseLevel() > 1) {
      G4cout << "  " << converter.GetParticleType()->GetParticleName()
             << " rangeCut not found, keeping default range" << G4endl;
    }
    return {converter.GetLowEdgeEnergy(), defaultRangeCut};
  }

  if (rangeCut < defaultRangeCut) {
    if (VerboseLevel() > 1) {
      G4cout << "  " << converter.GetParticleType()->GetParticleName()
             << " rangeCut found " << rangeCut << " mm "
             << " is below default value, it will be ignored" << G4endl;
    }
    return {converter.GetLowEdgeEnergy(), defaultRangeCut};
  }

  return {calcEnergyCut, rangeCut};
}

//_____________________________________________________________________________
void TG4RegionsManager::CheckRegionsRanges() const
{
  /// Loop over region data map and check if the calculated cuts
  /// are consistent with the energy cuts defined in limits

  if (VerboseLevel() > 0) {
    G4cout << ".. Checking energyCuts approximation, tolerance: "
          << fEnergyTolerance << G4endl;
  }

  auto counter = 0;
  for (auto& [regionName, values] : fRegionData) {
    // check values for gamma and electron
    for (size_t idx = fgkCutGamIdx; idx < fgkVmcCutGamIdx; ++idx ) {
      auto vmcIdx = idx + 2;
      auto factor = (idx == fgkCutGamIdx) ? 10. : 1.;
      auto cutType = (idx == fgkCutGamIdx) ? "gam" : "ele";
      if (fabs(values[idx] - values[vmcIdx]) >
          (values[vmcIdx] * fEnergyTolerance * factor)) {

        G4cout << std::setw(25) << std::left << regionName << "  " << cutType
               << " cut from range = " << std::scientific << values[idx] / MeV
               << " from limits = " << std::scientific<< values[vmcIdx] / MeV
               << " MeV" << " delta: "
               << fabs(values[idx] - values[vmcIdx]) / values[vmcIdx] * 100.
               << " %" << G4endl;
        ++counter;
      }
    }
  }

  if (counter == 0) {
    G4cout << ".. Cuts from ranges in regions are consistent with energy cuts."
           << G4endl << "   (precision fgkEnergyTolerance)" << G4endl;
  }
  else {
    G4cout << ".. Found " << counter
      << " inconsistencies between cut from ranges energy cuts." << G4endl;
  }
}

//_____________________________________________________________________________
void TG4RegionsManager::PrintFromMap(std::ostream& output) const
{
  /// Loop over regions data map and print all regions ranges,
  /// calculated energy cuts and the VMC energy cuts.

  if (fRegionData.empty()) {
    G4cout << "No regions data in the map." << G4endl;
  }

  PrintLegend(output);

  for (auto [regionName, values] : fRegionData) {
    PrintRegionData(output, regionName, values);
  }
}


//
// public methods
//

//_____________________________________________________________________________
void TG4RegionsManager::DefineRegions()
{
  /// Apply cuts defined in tracking media

  // Do nothing if no option is activated
  if (!(fApplyForGamma || fApplyForElectron || fApplyForPositron ||
        fApplyForProton)) {
    return;
  }

  if (VerboseLevel() > 0) {
    G4cout << "Converting VMC cuts in regions" << G4endl;
  }

  if (fIsLoad) {
    LoadRegions();
  }

  // Create G4 range to energy converters
#if (G4VERSION_NUMBER == 1100 || G4VERSION_NUMBER == 1101 )
  // Temporary work-around for a bug in G4VRangeToEnergyConverter
  auto g4ConverterElePtr = new G4RToEConvForElectron();
  auto g4ConverterGamPtr = new G4RToEConvForGamma();
  auto& g4ConverterEle = *g4ConverterElePtr;
  auto& g4ConverterGam = *g4ConverterGamPtr;
#else
  G4RToEConvForElectron g4ConverterEle;
  G4RToEConvForGamma g4ConverterGam;
#endif

  // Get default range cut values from physics manager
  G4double defaultRangeCutEle =
    TG4PhysicsManager::Instance()->GetCutForElectron();
  G4double defaultRangeCutGam = TG4PhysicsManager::Instance()->GetCutForGamma();
  G4double defaultRangeCutPositron =
    TG4PhysicsManager::Instance()->GetCutForPositron();
  G4double defaultRangeCutProton =
    TG4PhysicsManager::Instance()->GetCutForProton();

  // Create a new region with default cuts
  G4Region* defaultRegion = new G4Region(fgkDefaultRegionName);
  G4ProductionCuts* dcuts = new G4ProductionCuts();
  dcuts->SetProductionCut(defaultRangeCutGam, 0);
  dcuts->SetProductionCut(defaultRangeCutEle, 1);
  dcuts->SetProductionCut(defaultRangeCutPositron, 2);
  dcuts->SetProductionCut(defaultRangeCutProton, 3);
  defaultRegion->SetProductionCuts(dcuts);

  // Get world default region
  G4LogicalVolume* worldLV =
    TG4GeometryServices::Instance()->GetWorld()->GetLogicalVolume();
  G4Region* worldRegion = worldLV->GetRegion();

  // Get global energy cuts
  G4double cutEleGlobal = GetGlobalEnergyCut(kCUTELE);
  G4double cutGamGlobal = GetGlobalEnergyCut(kCUTGAM);
  if (VerboseLevel() > 2) {
    G4cout << "Global cut values: "
           << "  CUTELE = " << cutEleGlobal << " MeV"
           << "  CUTGAM = " << cutGamGlobal << " MeV" << G4endl;
  }

  G4int counter = 0;
  std::set<G4Material*> processedMaterials;
  std::set<G4Material*> processedMaterials2;

  // Define region for each logical volume
  //

  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  for (G4int i = 0; i < G4int(lvStore->size()); i++) {

    G4LogicalVolume* lv = (*lvStore)[i];
    G4bool isWorld = (lv == worldLV);

    TG4Medium* medium =
      TG4GeometryServices::Instance()->GetMediumMap()->GetMedium(lv, false);
    if (!medium) continue;

    G4Material* material = lv->GetMaterial();
    if (VerboseLevel() > 1) {
      G4cout << "-- Volume = " << i << "  " << lv << "  " << lv->GetName()
             << " material = " << material->GetName() << G4endl;
    }

    // Get region, if it already exists, and add the logical volume
    G4String regionName = material->GetName();
    G4Region* region =
      G4RegionStore::GetInstance()->GetRegion(regionName, false);
    if (region && !isWorld) {
      if (VerboseLevel() > 1) {
        G4cout << "   "
               << "adding volume in region = " << regionName << G4endl;
      }
      if (lv->GetRegion() != region) region->AddRootLogicalVolume(lv);
      // skip evaluation of cuts if this material was already processed
      if (processedMaterials2.find(material) != processedMaterials2.end()) {
        if (VerboseLevel() > 1) {
          G4cout << "   "
                 << "skipping cuts evaluation in region = " << regionName << G4endl;
        }
        continue;
      }
    }

    // After this line, region either does not exist, or we deal with the
    // world reagion

    // If this material was already processed and did not result
    // in a new region: add the logical volume to the default region
    if (processedMaterials.find(material) != processedMaterials.end() &&
        !isWorld) {
      if (VerboseLevel() > 1) {
        G4cout << "   "
               << "adding volume in the default region" << G4endl;
      }
      defaultRegion->AddRootLogicalVolume(lv);
      continue;
    }

    // Get energy cuts defined in limits
    TG4Limits* limits = (TG4Limits*)lv->GetUserLimits();
    G4double cutEle = GetEnergyCut(limits, kCUTELE, cutEleGlobal);
    G4double cutGam = GetEnergyCut(limits, kCUTGAM, cutGamGlobal);

    // Convert energy cuts defined in limits in range cuts
    auto [calcCutEle, rangeEle] =
      GetRangeCut(cutEle, material, g4ConverterEle, defaultRangeCutEle);
    auto [calcCutGam, rangeGam] =
      GetRangeCut(cutGam, material, g4ConverterGam, defaultRangeCutGam);
    if (VerboseLevel() > 1) {
      G4cout << ".. converted in e- rangeCut = " << rangeEle << " mm  "
             << "gamma rangeCut = " << rangeGam << " mm" << G4endl;
    }

    if (fabs(rangeEle - defaultRangeCutEle) < 1e-03 &&
        fabs(rangeGam - defaultRangeCutGam) < 1e-03 && !region) {
      // Do not create a new region if range cuts do not differ
      // from those in the default region and region was not defined
      // before
      if (!isWorld) {
        if (VerboseLevel() > 1) {
          G4cout << "   "
                 << "adding volume in the default region" << G4endl;
        }
        defaultRegion->AddRootLogicalVolume(lv);
      }
      // Save the material in the processed materials not resulting
      // in new region
      processedMaterials.insert(material);
    }
    else {
      // Create new production cuts
      G4ProductionCuts* cuts = new G4ProductionCuts();

      if (fApplyForGamma) {
        cuts->SetProductionCut(rangeGam, 0);
      }
      else {
        cuts->SetProductionCut(defaultRangeCutGam, 0);
      }

      if (fApplyForElectron) {
        cuts->SetProductionCut(rangeEle, 1);
      }
      else {
        cuts->SetProductionCut(defaultRangeCutEle, 1);
      }

      if (fApplyForPositron) {
        cuts->SetProductionCut(rangeEle, 2);
      }
      else {
        cuts->SetProductionCut(defaultRangeCutPositron, 2);
      }

      if (fApplyForProton) {
        cuts->SetProductionCut(rangeEle, 3);
      }
      else {
        cuts->SetProductionCut(defaultRangeCutProton, 2);
      }

      // save computed ranges in a map
      if (! fIsLoad) {
        fRegionData[regionName] =
          {rangeGam, rangeEle, calcCutGam, calcCutEle, cutGam, cutEle};
      }

      if (isWorld) {
        // set new production cuts to the world
        worldRegion->SetProductionCuts(cuts);
        worldRegion->RegionModified(true);
        if (VerboseLevel() > 1) {
          G4cout << "   "
                 << "setting new production cuts to the world region" << G4endl;
        }
        continue;
      }

      G4String which;
      // Create new region if it does not yet exist
      if (region == nullptr) {
        region = new G4Region(regionName);
        ++counter;
        which = "new ";
      }
      if (VerboseLevel() > 1) {
        G4cout << "   "
               << "adding volume in " << which << "region " << regionName << G4endl;
      }
      // Set computed production cuts
      region->SetProductionCuts(cuts);
      region->AddRootLogicalVolume(lv);

      // Save the material in the processed materials
      processedMaterials2.insert(material);
    }
  }

  processedMaterials.clear();
  processedMaterials.clear();

  if (VerboseLevel() > 0) {
    G4cout << "Number of added regions: " << counter << G4endl;
  }
}

//_____________________________________________________________________________
void TG4RegionsManager::PrintRegions(std::ostream& output) const
{
  /// Print regions from data map or the production cuts table.

  if (fIsG4Table) {
    G4cout << "going to print/write from G4 Table" << G4endl;
    PrintFromG4Table(output);
  } else {
    G4cout << "going to print/write from map" << G4endl;
    PrintFromMap(output);
  }
}

//_____________________________________________________________________________
void TG4RegionsManager::CheckRegions() const
{
  /// Perform two checks:
  /// -  if calculated energy cuts are consistent with energy cuts defined in limits
  /// -  if the region to which the volume belongs correspond to its material

  CheckRegionsRanges();
  CheckRegionsInGeometry();
}

//_____________________________________________________________________________
void TG4RegionsManager::LoadRegions()
{
  /// Load all regions ranges from the input file.

  auto fileName = fFileName.empty() ? fgkDefaultFileName : fFileName;

  std::ifstream input;
  input.open(fileName, std::ios::in);

  if (! input.is_open()) {
    TG4Globals::Warning("TG4RegionsManager", "LoadRegions",
      "Open input file  " + TString(fileName.data()) + " has failed.");
    fIsLoad = false;
    return;
  }

  if (VerboseLevel() > 0) {
    G4cout << "Loading regions from file: " << fileName << G4endl;
  }

  auto worldMaterialName = TG4GeometryServices::Instance()->GetWorld()->
    GetLogicalVolume()->GetMaterial()->GetName();

  G4String skipLine;
  G4String regionName;
  G4int counter;
  TG4RegionData regionValues;

  // skip comments
  std::getline(input, skipLine);

  // read data
  while (! input.eof()) {
    std::getline(input, regionName, ';');
    for (auto& value : regionValues) {
      input >> value;
    }

    if (input.good()) {
      auto startPos = regionName.find('\'') + 1;
      auto length = regionName.find_last_of('\'') - startPos;
      regionName = regionName.substr(startPos, length);
      if (VerboseLevel() > 0) {
        G4cout << "Loading " << regionName << G4endl;
      }

      // update units
      for (auto idx = fgkCutGamIdx; idx <= fgkVmcCutEleIdx; ++ idx) {
        regionValues[idx] *= TG4G3Units::Energy();
      }

      auto it = fRegionData.find(regionName);
      if (it == fRegionData.end()) {
        fRegionData[regionName] = regionValues;
        ++counter;
      }
      else {
        // skip "fake" material couples originated from assemblies
        // that are added in the regions in addition to the real material
        if (regionName == worldMaterialName) {
          if (VerboseLevel() > 0) {
            G4cout << "Skipping " << regionName << " already in map." << G4endl;
          }
        }
        else {
          // print warning
          TG4Globals::Warning("TG4RegionsManager", "LoadRegions",
            "Duplicated region data: " + TString(regionName.data()) +
            "  will be skipped !!!.");

          if (VerboseLevel() > 0) {
            auto valuesInMap = it->second;
            G4cout << "   Already in map !!!" << G4endl << "   data in map: ";
            PrintRegionData(G4cout, regionName, valuesInMap);
            G4cout << "   loaded data: ";
            PrintRegionData(G4cout, regionName, regionValues);
          }
        }
      }
    }
  }

  if (VerboseLevel() > 0) {
    G4cout << "Loaded " << counter << " regions from file: " << fileName << G4endl;
  }
}

//_____________________________________________________________________________
void TG4RegionsManager::DumpRegion(const G4String& volName) const
{
  /// Dump region properties for volume with given name

  // Get volume
  G4LogicalVolume* lv =
    TG4GeometryServices::Instance()->FindLogicalVolume(volName);
  if (!lv) return;

  // Get region
  G4Region* region = lv->GetRegion();
  if (!region) {
    TG4Globals::Warning("TG4RegionsManager", "DumpRegion",
      "Region for volume " + TString(volName) + "not found.");
    return;
  }

  G4ProductionCuts* cuts = region->GetProductionCuts();

  // Range cuts
  G4cout << G4endl;
  G4cout << " Region name: " << region->GetName() << G4endl;
  G4cout << " Material : " << lv->GetMaterial()->GetName() << G4endl;
  G4cout << " Range cuts        : "
         << " gamma " << G4BestUnit(cuts->GetProductionCut("gamma"), "Length")
         << "    e- " << G4BestUnit(cuts->GetProductionCut("e-"), "Length")
         << "    e+ " << G4BestUnit(cuts->GetProductionCut("e+"), "Length")
         << " proton " << G4BestUnit(cuts->GetProductionCut("proton"), "Length")
         << G4endl;

  // Energy cuts
  G4cout << " Energy thresholds : ";

  G4ProductionCutsTable* productionCutsTable =
    G4ProductionCutsTable::GetProductionCutsTable();

  const G4MaterialCutsCouple* couple =
    productionCutsTable->GetMaterialCutsCouple(lv->GetMaterial(), cuts);
  // if ( ! couple || couple->IsRecalcNeeded() ) {
  if (!couple) {
    G4cout << " not ready to print" << G4endl;
    return;
  }

  const std::vector<G4double>* energyCutsGam =
    productionCutsTable->GetEnergyCutsVector(0);
  const std::vector<G4double>* energyCutsEle =
    productionCutsTable->GetEnergyCutsVector(1);
  const std::vector<G4double>* energyCutsPos =
    productionCutsTable->GetEnergyCutsVector(2);
  const std::vector<G4double>* energyCutsPro =
    productionCutsTable->GetEnergyCutsVector(3);

  G4double cutGam = (*energyCutsGam)[couple->GetIndex()];
  G4double cutEle = (*energyCutsEle)[couple->GetIndex()];
  G4double cutPos = (*energyCutsPos)[couple->GetIndex()];
  G4double cutPro = (*energyCutsPro)[couple->GetIndex()];

  G4cout << " gamma " << G4BestUnit(cutGam, "Energy") << "    e- "
         << G4BestUnit(cutEle, "Energy") << "    e+ "
         << G4BestUnit(cutPos, "Energy") << " proton "
         << G4BestUnit(cutPro, "Energy");
  G4cout << G4endl;

  G4RegionStore* regionStore = G4RegionStore::GetInstance();
  if (couple->IsUsed()) {
    G4cout << " Region(s) which use this couple : ";
    std::vector<G4Region*>::iterator it;
    for (it = regionStore->begin(); it != regionStore->end(); it++) {
      if (IsCoupleUsedInTheRegion(couple, *it)) {
        G4cout << "    " << (*it)->GetName();
      }
    }
    G4cout << G4endl;
  }
}

//_____________________________________________________________________________
void TG4RegionsManager::SetLoad(G4bool isLoad)
{
/// Set the option to load regions ranges from a file.
//  The options fIsSave and fIsLoad cannot be activated both
//  in the same run.

  if (fIsSave) {
    TG4Globals::Warning("TG4RegionsManager", "SetLoad",
      "\"Save\" option is active. The input file " + fFileName +
      " will be overwritten.");
  }

  fIsLoad = isLoad;
}
