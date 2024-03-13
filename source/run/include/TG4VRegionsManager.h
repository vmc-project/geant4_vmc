#ifndef TG4_V_REGIONS_MANAGER_H
#define TG4_V_REGIONS_MANAGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2023 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4VRegionsManager.h
/// \brief Definition of the TG4VRegionsManager class
///
/// \author I. Hrivnacova; IJCLab Orsay

#include "TG4G3Cut.h"
#include "TG4RegionsMessenger.h"
#include "TG4Verbose.h"

#include <globals.hh>

#include <map>

class TG4Limits;

class G4Region;
class G4Material;
class G4VRangeToEnergyConverter;
class G4MaterialCutsCouple;

/// \ingroup run
/// \brief Base class for mangers for converting VMC cuts in energy in G4 regions
///
/// \author I. Hrivnacova; IPN Orsay

class TG4VRegionsManager : public TG4Verbose
{
 public:
  TG4VRegionsManager();
  virtual ~TG4VRegionsManager();

  static TG4VRegionsManager* Instance();

  // methods
  virtual void DefineRegions() = 0;
  virtual void UpdateProductionCutsTable() = 0;

  virtual void CheckRegions() const;
  virtual void PrintRegions(std::ostream& output) const;
  virtual void SaveRegions();
  virtual void DumpRegionStore() const;

  // set methods
  void SetFileName(const G4String& fileName);
  void SetApplyForGamma(G4bool applyForGamma);
  void SetApplyForElectron(G4bool applyForElectron);
  void SetApplyForPositron(G4bool applyForPositron);
  void SetApplyForProton(G4bool applyForProton);
  void SetCheck(G4bool isCheck);
  void SetPrint(G4bool isPrint);
  void SetSave(G4bool isSave);

  // get methods
  G4String GetFileName() const;
  G4bool GetApplyForGamma() const;
  G4bool GetApplyForElectron() const;
  G4bool GetApplyForPositron() const;
  G4bool GetApplyForProton() const;
  G4bool IsCheck() const;
  G4bool IsPrint() const;
  G4bool IsSave() const;

 protected:
  // constants
  static constexpr size_t fgkRangeGamIdx = 0;
  static constexpr size_t fgkRangeEleIdx = 1;
  static constexpr size_t fgkCutGamIdx = 2;
  static constexpr size_t fgkCutEleIdx = 3;
  static constexpr size_t fgkVmcCutGamIdx = 4;
  static constexpr size_t fgkVmcCutEleIdx = 5;
  static constexpr size_t fgkValuesSize = 6;

  using TG4RegionData = std::array<G4double, fgkValuesSize>;

  TG4VRegionsManager(const TG4VRegionsManager& right) = delete;
  TG4VRegionsManager& operator=(const TG4VRegionsManager& right) = delete;

  // methods
  G4double GetGlobalEnergyCut(TG4G3Cut cutType) const;

  G4double GetEnergyCut(
    TG4Limits* limits, TG4G3Cut cutType, G4double globalCutValue) const;

  G4bool IsCoupleUsedInTheRegion(
    const G4MaterialCutsCouple* couple, const G4Region* region) const;

  void CheckRegionsInGeometry() const;
  void PrintLegend(std::ostream& output) const;
  void PrintRegionData(std::ostream& output, const G4String& matName,
    const TG4RegionData& values) const;
  void PrintFromG4Table(std::ostream& output) const;

  //
  // static data members

  /// the singleton instance
  static TG4VRegionsManager* fgInstance;
  /// the name of the region with default cuts
  static const G4String fgkDefaultRegionName;
  /// the name of the region with default cuts
  static const G4String fgkDefaultFileName;

  //
  // data members

  /// file name for regions output
  G4String fFileName;
  /// option to apply cuts for gamma (default is true)
  G4bool fApplyForGamma = true;
  /// option to apply cuts for e- (default is true)
  G4bool fApplyForElectron = true;
  /// option to apply cuts for e+ (default is true)
  G4bool fApplyForPositron = true;
  /// option to apply cuts for proton (default is true)
  G4bool fApplyForProton = true;
  /// option to perform consistency check (by default false)
  G4bool fIsCheck = false;
  /// option to print all regions
  G4bool fIsPrint = false;
  /// option to save all regions in a file
  G4bool fIsSave = false;
};

/// Return the singleton instance
inline TG4VRegionsManager* TG4VRegionsManager::Instance() { return fgInstance; }

/// Set the file name for regions output
inline void TG4VRegionsManager::SetFileName(const G4String& fileName)
{ fFileName = fileName; }

/// Set the option to apply range cuts for gamma (default is true)
inline void TG4VRegionsManager::SetApplyForGamma(G4bool applyForGamma)
{
  fApplyForGamma = applyForGamma;
}

/// Set the option to apply range cuts for e- (default is true)
inline void TG4VRegionsManager::SetApplyForElectron(G4bool applyForElectron)
{
  fApplyForElectron = applyForElectron;
}

/// Set the option to apply range cuts for e+ (default is true)
inline void TG4VRegionsManager::SetApplyForPositron(G4bool applyForPositron)
{
  fApplyForPositron = applyForPositron;
}

/// Set the option to apply range cuts for proton (default is true)
inline void TG4VRegionsManager::SetApplyForProton(G4bool applyForProton)
{
  fApplyForProton = applyForProton;
}

/// Set the option to perform consistency check (by default false)
inline void TG4VRegionsManager::SetCheck(G4bool isCheck)
{ fIsCheck = isCheck; }

/// Set the option to print all regions
inline void TG4VRegionsManager::SetPrint(G4bool isPrint)
{ fIsPrint = isPrint; }

/// Set the option to save all regions in a file
inline void TG4VRegionsManager::SetSave(G4bool isSave)
{ fIsSave = isSave; }

/// Return the file name for regions output
inline G4String TG4VRegionsManager::GetFileName() const
{ return fFileName; }

/// Return the option to apply range cuts for gamma
inline G4bool TG4VRegionsManager::GetApplyForGamma() const
{
  return fApplyForGamma;
}

/// Return the option to apply range cuts for e+
inline G4bool TG4VRegionsManager::GetApplyForElectron() const
{
  return fApplyForElectron;
}

/// Return the option to apply range cuts for positron
inline G4bool TG4VRegionsManager::GetApplyForPositron() const
{
  return fApplyForPositron;
}

/// Return the option to apply range cuts for proton
inline G4bool TG4VRegionsManager::GetApplyForProton() const
{
  return fApplyForProton;
}

/// Return the option to perform consistency check
inline G4bool TG4VRegionsManager::IsCheck() const { return fIsCheck; }

/// Return the option to print all regions
inline G4bool TG4VRegionsManager::IsPrint() const { return fIsPrint; }

/// Return option to save all regions in a file
inline G4bool TG4VRegionsManager::IsSave() const { return fIsSave; }

#endif // TG4_V_REGIONS_MANAGER_H
