#ifndef TG4_COMPOSED_PHYSICS_LIST_H
#define TG4_COMPOSED_PHYSICS_LIST_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ComposedPhysicsList.h
/// \brief Definition of the TG4ComposedPhysicsList class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4ComposedPhysicsMessenger.h"
#include "TG4Verbose.h"

#include <G4VUserPhysicsList.hh>
#include <G4HadronicProcessType.hh>
#include <globals.hh>

#include <vector>
#include <tuple>

/// \ingroup physics_list
/// \brief The Geant4 VMC physics list builder
///
/// The helper class which allows to combine more physics list
/// together.
///
/// \author I. Hrivnacova; IPN Orsay

class TG4ComposedPhysicsList : public G4VUserPhysicsList, public TG4Verbose
{
 public:
  TG4ComposedPhysicsList();
  virtual ~TG4ComposedPhysicsList();

  // methods
  void AddPhysicsList(G4VUserPhysicsList* physicsList);

  virtual void ConstructParticle();
  virtual void ConstructProcess();
  virtual void SetCuts();

  void SetCutForGamma(G4double cut);
  void SetCutForElectron(G4double cut);
  void SetCutForPositron(G4double cut);
  void SetCutForProton(G4double cut);

  void SetProductionCutsTableEnergyRange(G4double min, G4double max);
  void SetGammaToMuonsCrossSectionFactor(G4double value);
  void SetCrossSectionFactor(
         const G4String& particleName, const G4String& processDef,
         G4double value, G4bool isProcessName);

  void SetLooperThresholdsLevel(G4int level);

  void PrintAllProcesses() const;
  void DumpAllProcesses() const;

  virtual G4int VerboseLevel() const;
  virtual void VerboseLevel(G4int level);

 private:
  /// Not implemented
  TG4ComposedPhysicsList(const TG4ComposedPhysicsList& right);
  /// Not implemented
  TG4ComposedPhysicsList& operator=(const TG4ComposedPhysicsList& right);

  // methods
  void ApplyGammaToMuonsCrossSectionFactor();
  void ApplyCrossSectionFactor(
         const G4String& particleName, const G4String& processDef,
         G4double value, G4bool isProcessName);
  void ApplyCrossSectionFactors();

  void SetLooperThresholds();

  // static data members
  static const G4double
    fgkDefautLooperThresholdsLevel; ///< the default cut value

  // data members
  TG4ComposedPhysicsMessenger fMessenger;         ///< messenger
  std::vector<G4VUserPhysicsList*> fPhysicsLists; ///< physics lists

  /// Info if the production cuts table energy range is redefined by user
  G4bool fIsProductionCutsTableEnergyRange;

  /// The production cuts table energy range minimum redefined by user
  G4double fProductionCutsTableEnergyMin;

  /// The production cuts table energy range maximum redefined by user
  G4double fProductionCutsTableEnergyMax;

  /// Gamma to muons cross section factor
  G4double fGammaToMuonsCrossSectionFactor;

  /// Cross section factors by process type or name
  std::vector<std::tuple<G4String, G4String, G4double, G4bool>> fCrossSectionFactors;

  /// Looper threshold level (can have valuee 0,1,2)
  G4int fLooperThresholdsLevel;
};

// inline methods

inline void TG4ComposedPhysicsList::SetGammaToMuonsCrossSectionFactor(
  G4double value)
{
  /// Set gamma to muons cross section factor
  fGammaToMuonsCrossSectionFactor = value;
}

inline void TG4ComposedPhysicsList::SetLooperThresholdsLevel(G4int level)
{
  /// Set gamma to muons cross section factor
  fLooperThresholdsLevel = level;
}

#endif // TG4_COMPOSED_MODULAR_PHYSICS_H
