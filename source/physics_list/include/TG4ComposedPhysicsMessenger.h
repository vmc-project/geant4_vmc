#ifndef TG4_COMPOSED_PHYSICS_MESSENGER_H
#define TG4_COMPOSED_PHYSICS_MESSENGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ComposedPhysicsMessenger.h
/// \brief Definition of the TG4ComposedPhysicsMessenger class
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4ComposedPhysicsList;

class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

/// \ingroup physics_list
/// \brief Messenger class that defines commands for Geant4 VMC composed
///        physics list and related classes
///
/// Implements commands:
/// - /mcPhysics/rangeCutForElectron  value [unit]
/// - /mcPhysics/rangeCutForPositron  value [unit]
/// - /mcPhysics/rangeCutForGamma     value [unit]
/// - /mcPhysics/rangeCutForProton    value [unit]
/// - /mcPhysics/rangeCuts            value [unit]
/// - /mcPhysics/productionCutsTableEnergyRange  minValue maxValue [unit]
/// - /mcPhysics/setGammaToMuonsCrossSectionFactor  value
/// - /mcPhysics/printProcessMCMap
/// - /mcPhysics/printProcessControlMap
/// - /mcPhysics/printVolumeLimits [volName]
/// - /mcPhysics/printGlobalCuts
/// - /mcPhysics/printGlobalControls
/// - /mcPhysics/g4NeutronHPVerbose
/// - /mcPhysics/g4HadronicProcessStoreVerbose
/// - /mcPhysics/useLowLooperThresholds
/// - /mcPhysics/useHighLooperThresholds
///
/// \author I. Hrivnacova; IPN Orsay

class TG4ComposedPhysicsMessenger : public G4UImessenger
{
 public:
  TG4ComposedPhysicsMessenger(TG4ComposedPhysicsList* physicsList);
  virtual ~TG4ComposedPhysicsMessenger();

  // methods
  virtual void SetNewValue(G4UIcommand* command, G4String string);

 private:
  /// Not implemented
  TG4ComposedPhysicsMessenger();
  /// Not implemented
  TG4ComposedPhysicsMessenger(const TG4ComposedPhysicsMessenger& right);
  /// Not implemented
  TG4ComposedPhysicsMessenger& operator=(
    const TG4ComposedPhysicsMessenger& right);

  // methods
  void CreateProductionCutsTableEnergyRangeCmd();

  //
  // data members

  /// associated class
  TG4ComposedPhysicsList* fPhysicsList;

  /// command directory
  G4UIdirectory* fDirectory;

  /// rangeCutForGamma command
  G4UIcmdWithADoubleAndUnit* fRangeGammaCutCmd;

  /// rangeCutForElectron command
  G4UIcmdWithADoubleAndUnit* fRangeElectronCutCmd;

  /// rangeCutForPositron command
  G4UIcmdWithADoubleAndUnit* fRangePositronCutCmd;

  /// rangeCutForProton command
  G4UIcmdWithADoubleAndUnit* fRangeProtonCutCmd;

  /// rangeCuts command
  G4UIcmdWithADoubleAndUnit* fRangeAllCutCmd;

  /// productionCutsTableEnergyRange command
  G4UIcommand* fProductionCutsTableEnergyRangeCmd;

  /// setGammaToMuonsCrossSectionFactor
  G4UIcmdWithADouble* fSetGammaToMuonsCrossSectionFactorCmd;

  /// printAllProcess command
  G4UIcmdWithoutParameter* fPrintAllProcessesCmd;

  /// dumpAllProcess command
  G4UIcmdWithoutParameter* fDumpAllProcessesCmd;

  /// printProcessMCMap command
  G4UIcmdWithoutParameter* fPrintProcessMCMapCmd;

  /// printProcessControlsMap command
  G4UIcmdWithoutParameter* fPrintProcessControlMapCmd;

  /// printVolumeLimits command
  G4UIcmdWithAString* fPrintVolumeLimitsCmd;

  /// printGeneralCuts command
  G4UIcmdWithoutParameter* fPrintGlobalCutsCmd;

  /// printGeneralControls command
  G4UIcmdWithoutParameter* fPrintGlobalControlsCmd;

  /// g4NeutronHPVerbose command
  G4UIcmdWithAnInteger* fG4NeutronHPVerboseCmd;

  /// g4HadronicProcessStoreVerbose command
  G4UIcmdWithAnInteger* fG4HadronicProcessStoreVerboseCmd;

  /// useLowerLooperThresholds command
  G4UIcmdWithoutParameter* fUseLowLooperThresholdsCmd;

  /// useHigherLooperThresholds command
  G4UIcmdWithoutParameter* fUseHighLooperThresholdsCmd;
};

#endif // TG4_COMPOSED_PHYSICS_MESSENGER_H
