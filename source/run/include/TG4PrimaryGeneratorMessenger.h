#ifndef TG4_PRIMARY_GENERATOR_MESSENGER_H
#define TG4_PRIMARY_GENERATOR_MESSENGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2019 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4PrimaryGeneratorMessenger.h
/// \brief Definition of the TG4PrimaryGeneratorMessenger class
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4PrimaryGeneratorAction;

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

/// \ingroup run
/// \brief Messenger class that defines commands for TG4PrimaryGeneratorAction
///
/// Implements commands:
/// - /mcPrimaryGenerator/skipUnknownParticles true|false
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4PrimaryGeneratorMessenger : public G4UImessenger
{
 public:
  TG4PrimaryGeneratorMessenger(TG4PrimaryGeneratorAction* action);
  virtual ~TG4PrimaryGeneratorMessenger();

  // methods
  virtual void SetNewValue(G4UIcommand* command, G4String string);

 private:
  /// Not implemented
  TG4PrimaryGeneratorMessenger();
  /// Not implemented
  TG4PrimaryGeneratorMessenger(const TG4PrimaryGeneratorMessenger& right);
  /// Not implemented
  TG4PrimaryGeneratorMessenger& operator=(
    const TG4PrimaryGeneratorMessenger& right);

  // data members
  TG4PrimaryGeneratorAction* fPrimaryGeneratorAction; ///< associated class
  G4UIdirectory* fDirectory;                          ///< command directory

  /// command: /mcPrimaryGenerator/skipUnknownParticles
  G4UIcmdWithABool* fSkipUnknownParticlesCmd;
  /// command: /mcRegions/applyForElectron true|false
};

#endif // TG4_PRIMARY_GENERATOR_MESSENGER