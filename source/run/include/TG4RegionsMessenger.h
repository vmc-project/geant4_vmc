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

class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

/// \ingroup run
/// \brief Messenger class that defines commands for TG4RegionsManager
///
/// Implements commands:
/// - /mcRegions/dump [lvName]
/// - /mcRegions/setRangePrecision value
/// - /mcRegions/setEnergyTolerance value
/// - /mcRegions/applyForGamma true|false
/// - /mcRegions/applyForElectron true|false
/// - /mcRegions/applyForPositron true|false
/// - /mcRegions/applyForProton true|false
/// - /mcRegions/check [true|false]
/// - /mcRegions/print [true|false]
/// - /mcRegions/save [true|false]
/// - /mcRegions/load [true|false]
/// - /mcRegions/fromG4Table [true|false]
/// - /mcRegions/setFileName fileName
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4RegionsMessenger : public G4UImessenger
{
 public:
  TG4RegionsMessenger(TG4RegionsManager* runManager);
  virtual ~TG4RegionsMessenger();

  // methods
  virtual void SetNewValue(G4UIcommand* command, G4String string);

 private:
  /// Not implemented
  TG4RegionsMessenger();
  /// Not implemented
  TG4RegionsMessenger(const TG4RegionsMessenger& right);
  /// Not implemented
  TG4RegionsMessenger& operator=(const TG4RegionsMessenger& right);

  // data members
  TG4RegionsManager* fRegionsManager; ///< associated class
  G4UIdirectory* fDirectory;          ///< command directory

  /// command: /mcRegions/dump [lvName]
  G4UIcmdWithAString* fDumpRegionCmd;
  /// command: /mcRegions/setRangePrecision value
  G4UIcmdWithAnInteger* fSetRangePrecisionCmd;
  /// command: /mcRegions/setEnergyTolerance value
  G4UIcmdWithADouble* fSetEnergyToleranceCmd;
  /// command: /mcRegions/applyForGamma true|false
  G4UIcmdWithABool* fApplyForGammaCmd;
  /// command: /mcRegions/applyForElectron true|false
  G4UIcmdWithABool* fApplyForElectronCmd;
  /// command: /mcRegions/applyForPositron true|false
  G4UIcmdWithABool* fApplyForPositronCmd;
  /// command: /mcRegions/applyForProton true|false
  G4UIcmdWithABool* fApplyForProtonCmd;
  /// command: /mcRegions/check [true|false]
  G4UIcmdWithABool* fSetCheckCmd;
  /// command: /mcRegions/print [true|false]
  G4UIcmdWithABool* fSetPrintCmd;
  /// command: /mcRegions/save [true|false]
  G4UIcmdWithABool* fSetSaveCmd;
  /// command: /mcRegions/load [true|false]
  G4UIcmdWithABool* fSetLoadCmd;
  /// command: /mcRegions/fromG4Table [true|false]
  G4UIcmdWithABool* fSetFromG4TableCmd;
  /// command: /mcRegions/setFileName fileName
  G4UIcmdWithAString* fSetFileNameCmd;
  /// option to activate print/save from production cuts table
  G4bool fIsFromG4Table;
};

#endif // TG4_RUN_MESSENGER_H
