#ifndef TG4_REGIONS_MANAGER2_H
#define TG4_REGIONS_MANAGER2_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2023 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4RegionsManager2.h
/// \brief Definition of the TG4RegionsManager2 class
///
/// \author I. Hrivnacova; IJCLab Orsay

#include "TG4VRegionsManager.h"

#include <globals.hh>

class TG4Limits;

class G4Region;
class G4Material;
class G4MaterialCutsCouple;

/// \ingroup run
/// \brief Manager class for setting VMC cuts in energy in G4 regions
///
/// The VMC cuts in energy can be defined via the following TVirtulaMC
/// functions:
/// - TVirtualMC::SetCut(..)
/// - TVirtualMC::Gstpar(..)
///
/// The TG4RegionsManager2 class defines regions with production
/// cuts corresponding to the VMC energy cuts set by user in two
/// steps:
/// - DefineRegions: regions per material are created and applied 
///   to all logical volumes
/// - UpdateProductionCutsTable: create material cuts couples in
///   G4ProductionCutsTable and construct energy cuts vectors in the order 
///   of created material cuts couples and set them to the table
///
/// User can select several levels of verbosity; the following information
/// is then printed:
/// - 0  no output
/// - 1  number of regions added via VMC
/// - 2  the list of all volumes and their associated regions
///
/// \author I. Hrivnacova; IJCLab Orsay

class TG4RegionsManager2 : public TG4VRegionsManager
{
 public:
  TG4RegionsManager2();
  ~TG4RegionsManager2() override = default;

  // methods
  void DefineRegions() override;
  void UpdateProductionCutsTable() override;

 private:
  TG4RegionsManager2(const TG4RegionsManager2& right) = delete;
  TG4RegionsManager2& operator=(const TG4RegionsManager2& right) = delete;

  /// messenger
  TG4RegionsMessenger fMessenger;
};

#endif // TG4_REGIONS_MANAGER2_H
