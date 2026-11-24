//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2026 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// Regression test for multiple tracking media sharing one material.
///
/// Run from the configured Geant4 VMC build directory:
///   cmake . -DGeant4VMC_BUILD_TESTS=ON
///   cmake --build . --target test_material_media -j 4
///   ctest -R '^material_media$' --output-on-failure
/// Replace --output-on-failure with --verbose to also show passing test output.

#include "TG4G3ControlVector.h"
#include "TG4G3CutVector.h"
#include "TG4G3PhysicsManager.h"
#include "TG4GeometryServices.h"
#include "TG4Limits.h"
#include "TG4Medium.h"
#include "TG4MediumMap.h"
#include "TG4RegionsManager2.h"
#include <G4Box.hh>
#include <G4Electron.hh>
#include <G4Gamma.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4MaterialCutsCouple.hh>
#include <G4PVPlacement.hh>
#include <G4Positron.hh>
#include <G4ProductionCuts.hh>
#include <G4ProductionCutsTable.hh>
#include <G4Proton.hh>
#include <G4Region.hh>
#include <G4SystemOfUnits.hh>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace
{
void Check(bool condition, const std::string& message)
{
  if (!condition)
    throw std::runtime_error(message);
}
} // namespace

int main()
{
  G4Gamma::Definition();
  G4Electron::Definition();
  G4Positron::Definition();
  G4Proton::Definition();
  TG4GeometryServices geometry;
  TG4G3PhysicsManager physics;
  physics.SetG3DefaultCuts();
  // The medium without limits supplies the lowest gamma cut via the global
  // fallback; the lowest electron and proton cuts come from different media.
  physics.SetCut(kCUTGAM, 5.);
  physics.SetCut(kCUTELE, 20.);
  physics.SetCut(kCUTHAD, 30.);
  auto map = geometry.GetMediumMap();
  auto material = new G4Material("shared", 1., 1.01 * g / mole, 1. * g / cm3);
  auto solid = new G4Box("box", 10., 10., 10.);
  auto world = new G4LogicalVolume(solid, material, "world");
  auto pv = new G4PVPlacement(nullptr, {}, world, "world", nullptr, false, 0);
  geometry.SetWorld(pv);
  TG4G3ControlVector controls;
  TG4G3CutVector cuts;
  TG4Limits a(cuts, controls), b(cuts, controls), c(cuts, controls);
  a.SetG3Cut(kCUTGAM, 8.);
  a.SetG3Cut(kCUTELE, 60.);
  a.SetG3Cut(kCUTHAD, 90.);
  b.SetG3Cut(kCUTGAM, 40.);
  b.SetG3Cut(kCUTELE, 12.);
  b.SetG3Cut(kCUTHAD, 80.);
  c.SetG3Cut(kCUTGAM, 50.);
  c.SetG3Cut(kCUTELE, 50.);
  c.SetG3Cut(kCUTHAD, 25.);
  TG4Limits* limits[] = { &a, &b, &c, nullptr };
  for (int i = 0; i < 4; ++i) {
    auto medium = map->AddMedium(i + 1);
    medium->SetMaterial(material);
    medium->SetLimits(limits[i]);
    auto lv = new G4LogicalVolume(solid, material, "shared-volume");
    map->MapMedium(lv, i + 1);
    map->MapMedium(lv, i + 1);
  }
  map->MapMedium(world, 1);
  std::vector<TG4Medium*> media;
  map->GetMedia(material, media);
  Check(media.size() == 4, "Repeated volume mappings must not duplicate media");
  Check(media.front() == map->GetMedium(1),
        "The first mapped medium must be retained");

  auto single = new G4Material("single", 2., 4. * g / mole, 1. * g / cm3);
  auto singleLV = new G4LogicalVolume(solid, single, "single-volume");
  auto singleMedium = map->AddMedium(5);
  singleMedium->SetMaterial(single);
  singleMedium->SetLimits(&a);
  map->MapMedium(singleLV, 5);
  auto region = new G4Region("test");
  region->AddRootLogicalVolume(world);
  region->AddRootLogicalVolume(singleLV);
  region->UsedInMassGeometry(true);
  region->SetProductionCuts(new G4ProductionCuts());
  TG4RegionsManager2 manager;
  std::ostringstream warnings;
  auto previousBuffer = G4cerr.rdbuf(warnings.rdbuf());
  manager.UpdateProductionCutsTable();
  G4cerr.rdbuf(previousBuffer);
  const auto warning = warnings.str();
  const auto warningStart = warning.find("More than one tracking medium");
  Check(warningStart != std::string::npos,
        "Missing warning for shared material");
  Check(warning.find("More than one tracking medium", warningStart + 1) ==
          std::string::npos,
        "Unexpected warning for single-medium material");
  Check(warning.find("material shared") != std::string::npos,
        "Warning must identify the shared material");
  Check(warning.find("production thresholds may be lower") != std::string::npos,
        "Warning must explain the possible difference from tracking cuts. "
        "Captured warning:\n" + warning);
  auto table = G4ProductionCutsTable::GetProductionCutsTable();
  table->UpdateCoupleTable(pv);
  Check(table->GetTableSize() == 2,
        "Expected one material-cuts couple per material");
  for (std::size_t i = 0; i < table->GetTableSize(); ++i) {
    const bool shared =
      table->GetMaterialCutsCouple(i)->GetMaterial() == material;
    Check(table->GetEnergyCutsVector(0)->at(i) == (shared ? 5. : 8.),
          "Incorrect gamma threshold");
    Check(table->GetEnergyCutsVector(1)->at(i) == (shared ? 12. : 60.),
          "Incorrect electron threshold");
    Check(table->GetEnergyCutsVector(2)->at(i) == (shared ? 12. : 60.),
          "Incorrect positron threshold");
    Check(table->GetEnergyCutsVector(3)->at(i) == (shared ? 25. : 90.),
          "Incorrect proton threshold");
  }
  std::ostringstream printed;
  manager.PrintRegions(printed);
  std::istringstream rows(printed.str());
  std::string line;
  int checked = 0;
  while (std::getline(rows, line)) {
    std::istringstream row(line);
    std::string name;
    double rangeGam, rangeEle, cutGam, cutEle, vmcGam, vmcEle;
    if (!(row >> name >> rangeGam >> rangeEle >> cutGam >> cutEle >> vmcGam >>
          vmcEle))
      continue;
    if (name != "'shared';" && name != "'single';")
      continue;
    Check(vmcGam == cutGam,
          "Printed VMC gamma cut must match the production threshold");
    Check(vmcEle == cutEle,
          "Printed VMC electron cut must match the production threshold");
    ++checked;
  }
  Check(checked == 2, "Expected printed cuts for both materials");
  std::cout << "PASS: distinct media, duplicate mappings, per-particle minima, "
               "global fallback, single medium\n";
}
