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

#include "TG4Verbose.h"
#include "TG4G3Cut.h"
#include "TG4RegionsMessenger.h"

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
/// User can select several levels of verbosity; the following information
/// is then printed:
/// - 0  no output
/// - 1  number of regions added via VMC
/// - 2  the list of all volumes, the cuts in energy and calculated cuts
///      in range
/// - 3  all evaluated energy values
///
/// \author I. Hrivnacova; IPN Orsay

class TG4RegionsManager : public TG4Verbose
{
  public:
    TG4RegionsManager();
    virtual ~TG4RegionsManager();

    static TG4RegionsManager* Instance();

    // methods
    void  DefineRegions();
    void  CheckRegions() const;
    void  PrintRegions() const;
    void  DumpRegion(const G4String& volName) const;

    // set methods
    void   SetRangePrecision(G4int precision);
    void   SetApplyForGamma(G4bool applyForGamma);
    void   SetApplyForElectron(G4bool applyForElectron);
    void   SetApplyForPositron(G4bool applyForPositron);
    void   SetApplyForProton(G4bool applyForProton);
    void   SetCheck(G4bool isCheck);
    void   SetPrint(G4bool isPrint);

    // get methods
    G4int  GetRangePrecision() const;
    G4bool GetApplyForGamma() const;
    G4bool GetApplyForElectron() const;
    G4bool GetApplyForPositron() const;
    G4bool GetApplyForProton() const;
    G4bool IsCheck() const;
    G4bool IsPrint() const;

  private:
    /// Not implemented
    TG4RegionsManager(const TG4RegionsManager& right);
    /// Not implemented
    TG4RegionsManager& operator=(const TG4RegionsManager& right);

    // methods
    G4double  GetGlobalEnergyCut(
                    TG4G3Cut cutType) const;

    G4double  GetEnergyCut(
                    TG4Limits* limits,
                    TG4G3Cut cutType,
                    G4double globalCutValue) const;

    G4bool   Iterate(G4double  energyCut,
                     G4double& lowerCut,
                     G4double& higherCut,
                     G4double  defaultRangeCut,
                     G4double lowEdgeEnergy,
                     G4double highEdgeEnergy,
                     G4int nbin,
                     std::map<G4double, G4double>& map,
                     std::map<G4double, G4double>::const_iterator& it,
                     G4Material* material,
                     G4VRangeToEnergyConverter& converter) const;

    G4double  ConvertEnergyToRange(
                    G4double energyCut,
                    G4Material* material,
                    G4VRangeToEnergyConverter& converter,
                    G4double defaultRangeValue) const;

    G4double  GetRangeCut(
                    G4double energyCut,
                    G4Material* material,
                    G4VRangeToEnergyConverter& converter,
                    G4double defaultRangeValue) const;

    G4bool    IsCoupleUsedInTheRegion(
                    const G4MaterialCutsCouple* couple,
                    const G4Region* region) const;

    G4bool    CheckCut(TG4Limits* limits,
                       TG4G3Cut cut, const G4String& particleName,
                       G4double energy, G4double range) const;

    void CheckRegionsRanges() const;
    void CheckRegionsInGeometry() const;

    //
    // static data members

    /// the singleton instance
    static TG4RegionsManager* fgInstance;
    /// the default precision for calculating ranges
    static const G4int    fgkDefaultRangePrecision;
    /// the tolerance (absolute) for comparing range values
    static const G4double fgkRangeTolerance;
    /// the tolerance (relative) for comparing energy cut values
    static const G4double fgkEnergyTolerance;
    /// the number of bins for search range iteration
    static const G4int    fgkNofBins;
    /// the minimum range order
    static const G4int    fgkMinRangeOrder;
    /// the maximum range order
    static const G4int    fgkMaxRangeOrder;
    /// the name of the region with default cuts
    static const G4String fgkDefaultRegionName;

    //
    // data members

    /// messenger
    TG4RegionsMessenger  fMessenger;
    /// the precision for calculating ranges
    G4int  fRangePrecision;
    /// option to apply range cuts for gamma (default is true)
    G4bool fApplyForGamma;
    /// option to apply range cuts for e- (default is true)
    G4bool fApplyForElectron;
    /// option to apply range cuts for e+ (default is true)
    G4bool fApplyForPositron;
    /// option to apply range cuts for proton (default is true)
    G4bool fApplyForProton;
    /// option to perform consistency check (by default false)
    G4bool fIsCheck;
    /// option to print all regions
    G4bool fIsPrint;
};

/// Return the singleton instance
inline TG4RegionsManager* TG4RegionsManager::Instance()
{  return fgInstance; }

/// Set the precision for calculating ranges
inline void   TG4RegionsManager::SetRangePrecision(G4int precision)
{ G4cout << "### New precision: " << precision << G4endl;
  fRangePrecision = precision; }

/// Set the option to apply range cuts for gamma (default is true)
inline void TG4RegionsManager::SetApplyForGamma(G4bool applyForGamma)
{ fApplyForGamma = applyForGamma; }

/// Set the option to apply range cuts for e- (default is true)
inline void TG4RegionsManager::SetApplyForElectron(G4bool applyForElectron)
{ fApplyForElectron = applyForElectron; }

/// Set the option to apply range cuts for e+ (default is true)
inline void TG4RegionsManager::SetApplyForPositron(G4bool applyForPositron)
{ fApplyForPositron = applyForPositron; }

/// Set the option to apply range cuts for proton (default is true)
inline void TG4RegionsManager::SetApplyForProton(G4bool applyForProton)
{ fApplyForProton = applyForProton; }

/// Set the option to perform consistency check
inline void  TG4RegionsManager::SetCheck(G4bool isCheck)
{  fIsCheck = isCheck; }

/// Set the option to print all regions
inline void  TG4RegionsManager::SetPrint(G4bool isPrint)
{  fIsPrint = isPrint; }

/// Return the precision for calculating ranges
inline G4int TG4RegionsManager::GetRangePrecision() const
{  return fRangePrecision; }

/// Return the option to apply range cuts for gamma
inline G4bool TG4RegionsManager::GetApplyForGamma() const
{  return fApplyForGamma; }

/// Return the option to apply range cuts for e+
inline G4bool TG4RegionsManager::GetApplyForElectron() const
{  return fApplyForElectron; }

/// Return the option to apply range cuts for positron
inline G4bool TG4RegionsManager::GetApplyForPositron() const
{  return fApplyForPositron; }

/// Return the option to apply range cuts for proton
inline G4bool TG4RegionsManager::GetApplyForProton() const
{  return fApplyForProton; }

/// Return the option to perform consistency check
inline G4bool TG4RegionsManager::IsCheck() const
{ return fIsCheck; }

/// Return the option to print all regions
inline G4bool TG4RegionsManager::IsPrint() const
{ return fIsPrint; }

#endif //TG4_REGIONS_MANAGER_H

