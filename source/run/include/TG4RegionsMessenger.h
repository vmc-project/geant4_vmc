#ifndef TG4_REGIONS_MESSENGER_H
#define TG4_REGIONS_MESSENGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4RegionsMessenger.h
/// \brief Definition of the TG4RegionsMessenger class
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4RegionsManager;
class TG4RegionsManager2;

class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

/// \ingroup run
/// \brief Messenger class that defines commands for TG4RegionsManager[2]
///      
///
/// Implements commands:
/// - /mcRegions/check [true|false]
/// - /mcRegions/print [true|false]
/// - /mcRegions/save [true|false]
/// - /mcRegions/setFileName fileName
///
/// Commands working only with old region manager:
/// - /mcRegions/dump lvName
/// - /mcRegions/setRangePrecision value
/// - /mcRegions/setEnergyTolerance value
/// - /mcRegions/applyForGamma true|false
/// - /mcRegions/applyForElectron true|false
/// - /mcRegions/applyForPositron true|false
/// - /mcRegions/applyForProton true|false
/// - /mcRegions/load [true|false]
/// - /mcRegions/fromG4Table [true|false]
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4RegionsMessenger : public G4UImessenger
{
 public:
  TG4RegionsMessenger(TG4RegionsManager* regionsManager);
  TG4RegionsMessenger(TG4RegionsManager2* regionsManager);
  virtual ~TG4RegionsMessenger();

  // methods
  virtual void SetNewValue(G4UIcommand* command, G4String string);

 private:
  TG4RegionsMessenger() = delete;
  TG4RegionsMessenger(const TG4RegionsMessenger& right) = delete;
  TG4RegionsMessenger& operator=(const TG4RegionsMessenger& right) = delete;

  // methods
  void CreateCommands();

  // data members
  TG4RegionsManager* fRegionsManager = nullptr;  ///< associated class
  TG4RegionsManager2* fRegionsManager2 = nullptr; ///< associated class
  G4UIdirectory* fDirectory = nullptr;          ///< command directory

  /// command: /mcRegions/check [true|false]
  G4UIcmdWithABool* fSetCheckCmd = nullptr;
  /// command: /mcRegions/print [true|false]
  G4UIcmdWithABool* fSetPrintCmd = nullptr;
  /// command: /mcRegions/save [true|false]
  G4UIcmdWithABool* fSetSaveCmd = nullptr;
  /// command: /mcRegions/setFileName fileName
  G4UIcmdWithAString* fSetFileNameCmd = nullptr;

  // commands working only with old regions messenger
  /// command: /mcRegions/dump [lvName]
  G4UIcmdWithAString* fDumpRegionCmd = nullptr;
  /// command: /mcRegions/setRangePrecision value
  G4UIcmdWithAnInteger* fSetRangePrecisionCmd = nullptr;
  /// command: /mcRegions/setEnergyTolerance value
  G4UIcmdWithADouble* fSetEnergyToleranceCmd = nullptr;
  /// command: /mcRegions/applyForGamma true|false
  G4UIcmdWithABool* fApplyForGammaCmd = nullptr;
  /// command: /mcRegions/applyForElectron true|false
  G4UIcmdWithABool* fApplyForElectronCmd = nullptr;
  /// command: /mcRegions/applyForPositron true|false
  G4UIcmdWithABool* fApplyForPositronCmd = nullptr;
  /// command: /mcRegions/applyForProton true|false
  G4UIcmdWithABool* fApplyForProtonCmd = nullptr;
  /// command: /mcRegions/load [true|false]
  G4UIcmdWithABool* fSetLoadCmd = nullptr;
  /// command: /mcRegions/fromG4Table [true|false]
  G4UIcmdWithABool* fSetFromG4TableCmd = nullptr;
};

#endif // TG4_RUN_MESSENGER_H
