#ifndef TG4_REGIONS_MANAGER_H
#define TG4_REGIONS_MANAGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4RegionsManager.h
/// \brief Definition of the TG4RegionsManager class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4VRegionsManager.h"

#include <globals.hh>

#include <map>

class TG4Limits;

class G4Region;
class G4Material;
class G4VRangeToEnergyConverter;
class G4MaterialCutsCouple;

/// \ingroup run
/// \brief Manager class for converting VMC cuts in energy in G4 regions
///
/// The VMC cuts in energy can be defined via the following TVirtulaMC
/// functions:
/// - TVirtualMC::SetCut(..)
/// - TVirtualMC::Gstpar(..)
///
/// The TG4RegionsManager class defined regions with cuts in range
/// corresponding to the VMC energy cuts set by user following
/// these rules:
/// - If the VMC energy cut defined by user results in a range cut
///   smaller than the default range cut value defined in user
///   physics list, the VMC cut is ignored and the default range cut
///   is used
/// - The range cut is first evaluated within the range 1e-03mm to 1m;
///   when the range cut order is found, it is refined up to given
///   precision (2 orders of magnitude by default) within 10 values
///   of each order and the range value with the closest energy still
///   smaller than VMC cut is chosen. \n
///   It may happen that value cannot be refined up to given precision,
///   then the best found value is returned.
/// - The regions are defined only in case when the VMC cuts
///   result in range cuts different from the range cuts in default
///   region; then the region includes all logical volumes
///   with a given material
/// - As the range cuts do not match precisely to user defined
///   energy cuts, the specialCuts process applies the energy cuts
///   as tracking cuts when a particle with energy cut below threshold
///   is generated.
/// 
/// By default, the computed range cut for e- is applied also to e+ and proton.
/// This feature can be switched off by the UI commands:
/// - /mcRegions/applyForPositron false
/// - /mcRegions/applyForProton false
///
/// User can select several levels of verbosity; the following information
/// is then printed:
/// - 0  no output
/// - 1  number of regions added via VMC
/// - 2  the list of all volumes, the cuts in energy and calculated cuts
///      in range
/// - 3  all evaluated energy values
///
/// \author I. Hrivnacova; IPN Orsay

class TG4RegionsManager : public TG4VRegionsManager
{
 public:
  TG4RegionsManager();
  ~TG4RegionsManager() override = default;

  // methods
  void DefineRegions() override;;
  void UpdateProductionCutsTable() override {}

  void CheckRegions() const override;
  void PrintRegions(std::ostream& output) const override;
  void LoadRegions();
  void DumpRegion(const G4String& volName) const;

  // set methods
  void SetRangePrecision(G4int precision);
  void SetEnergyTolerance(G4double tolerance);
  void SetApplyForGamma(G4bool applyForGamma);
  void SetApplyForElectron(G4bool applyForElectron);
  void SetApplyForPositron(G4bool applyForPositron);
  void SetApplyForProton(G4bool applyForProton);
  void SetLoad(G4bool isLoad);
  void SetFromG4Table(G4bool isG4Table);

  // get methods
  G4int GetRangePrecision() const;
  G4double GetEnergyTolerance() const;
  G4bool GetApplyForGamma() const;
  G4bool GetApplyForElectron() const;
  G4bool GetApplyForPositron() const;
  G4bool GetApplyForProton() const;
  G4String GetFileName() const;
  G4bool IsG4Table() const;
  G4bool IsLoad() const;

 private:
  using TG4RegionData = std::array<G4double, fgkValuesSize>;

  TG4RegionsManager(const TG4RegionsManager& right) = delete;
  TG4RegionsManager& operator=(const TG4RegionsManager& right) = delete;

  // methods
  G4bool Iterate(G4double energyCut, G4double& lowerCut, G4double& higherCut,
    G4double defaultRangeCut, G4double lowEdgeEnergy, G4double highEdgeEnergy,
    G4int nbin, std::map<G4double, G4double>& map,
    std::map<G4double, G4double>::const_iterator& it, G4Material* material,
    G4VRangeToEnergyConverter& converter) const;

  std::pair<G4double,G4double>
    ConvertEnergyToRange(G4double energyCut, G4Material* material,
    G4VRangeToEnergyConverter& converter, G4double defaultRangeValue) const;

  std::pair<G4double,G4double>
    GetRangeCut(G4double energyCut, G4Material* material,
    G4VRangeToEnergyConverter& converter, G4double defaultRangeValue) const;

  void CheckRegionsRanges() const;
  void PrintFromMap(std::ostream& output) const;

  //
  // static data members

  /// the default precision for calculating ranges
  static constexpr G4int fgkDefaultRangePrecision = 5;
  /// the default tolerance (relative) for comparing energy cut values
  static constexpr G4double fgkDefaultEnergyTolerance = 0.01;
  /// the number of bins for search range iteration
  static constexpr G4int fgkNofBins = 10;
  /// the minimum range order
  static constexpr G4int fgkMinRangeOrder = -3;
  /// the maximum range order
  static constexpr G4int fgkMaxRangeOrder = 6;

  //
  // data members

  /// messenger
  TG4RegionsMessenger fMessenger;
  /// the precision for calculating ranges
  G4int fRangePrecision = fgkDefaultRangePrecision;
  /// the tolerance (relative) for comparing energy cut values
  G4double fEnergyTolerance = fgkDefaultEnergyTolerance;
  /// option to apply range cuts for gamma (default is true)
  G4bool fApplyForGamma = true;
  /// option to apply range cuts for e- (default is true)
  G4bool fApplyForElectron = true;
  /// option to apply range cuts for e+ (default is true)
  G4bool fApplyForPositron = true;
  /// option to apply range cuts for proton (default is true)
  G4bool fApplyForProton = true;
  /// option to print or save regions from G4 production cuts table
  G4bool fIsG4Table = false;
  /// option to load regions ranges from a file
  G4bool fIsLoad = false;
  /// map for computed or loaded regions data
  std::map<G4String, TG4RegionData> fRegionData;
};

/// Set the precision for calculating ranges
inline void TG4RegionsManager::SetRangePrecision(G4int precision)
{
  G4cout << "### New precision: " << precision << G4endl;
  fRangePrecision = precision;
}

/// Set the tolerance (relative) for comparing energy cut values
inline void TG4RegionsManager::SetEnergyTolerance(G4double tolerance)
{
  G4cout << "### New tolerance: " << tolerance << G4endl;
  fEnergyTolerance = tolerance;
}

/// Set the option to apply range cuts for gamma (default is true)
inline void TG4RegionsManager::SetApplyForGamma(G4bool applyForGamma)
{
  fApplyForGamma = applyForGamma;
}

/// Set the option to apply range cuts for e- (default is true)
inline void TG4RegionsManager::SetApplyForElectron(G4bool applyForElectron)
{
  fApplyForElectron = applyForElectron;
}

/// Set the option to apply range cuts for e+ (default is true)
inline void TG4RegionsManager::SetApplyForPositron(G4bool applyForPositron)
{
  fApplyForPositron = applyForPositron;
}

/// Set the option to apply range cuts for proton (default is true)
inline void TG4RegionsManager::SetApplyForProton(G4bool applyForProton)
{
  fApplyForProton = applyForProton;
}

/// Set the option to print/save cuts from G4 table
inline void TG4RegionsManager::SetFromG4Table(G4bool isG4Table)
{
  fIsG4Table = isG4Table;
}

/// Return the precision for calculating ranges
inline G4int TG4RegionsManager::GetRangePrecision() const
{
  return fRangePrecision;
}

/// Return the tolerance (relative) for comparing energy cut values
inline G4double TG4RegionsManager::GetEnergyTolerance() const
{
  return fEnergyTolerance;
}

/// Return the option to apply range cuts for gamma
inline G4bool TG4RegionsManager::GetApplyForGamma() const
{
  return fApplyForGamma;
}

/// Return the option to apply range cuts for e+
inline G4bool TG4RegionsManager::GetApplyForElectron() const
{
  return fApplyForElectron;
}

/// Return the option to apply range cuts for positron
inline G4bool TG4RegionsManager::GetApplyForPositron() const
{
  return fApplyForPositron;
}

/// Return the option to apply range cuts for proton
inline G4bool TG4RegionsManager::GetApplyForProton() const
{
  return fApplyForProton;
}

/// Return the option to print or save regions from production cuts table
inline G4bool TG4RegionsManager::IsG4Table() const
{ return fIsG4Table; }

/// Return the option to load regions ranges from a file
inline G4bool TG4RegionsManager::IsLoad() const { return fIsLoad; }

#endif // TG4_REGIONS_MANAGER_H
