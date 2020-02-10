#ifndef TG4_EXT_DECAYR_MESSENGER_H
#define TG4_EXT_DECAYR_MESSENGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ExtDecayerMessenger.h
/// \brief Definition of the TG4ExtDecayerMessenger class
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4ExtDecayerPhysics;

class G4UIcmdWithAString;
class G4UIcmdWithABool;

/// \ingroup physics_list
/// \brief Messenger class that defines commands for the stack popper
///        special process
///
/// Implements commands:
/// - /mcPhysics/setExtDecayerSelection [particleName1 particleName2 ...]
/// - /mcPhysics/skipExtDecayerNeutrino true|false
///
/// \author I. Hrivnacova; IPN Orsay

class TG4ExtDecayerMessenger : public G4UImessenger
{
 public:
  TG4ExtDecayerMessenger(TG4ExtDecayerPhysics* extDecayerPhysics);
  virtual ~TG4ExtDecayerMessenger();

  // methods
  virtual void SetNewValue(G4UIcommand* command, G4String string);

 private:
  /// Not implemented
  TG4ExtDecayerMessenger();
  /// Not implemented
  TG4ExtDecayerMessenger(const TG4ExtDecayerMessenger& right);
  /// Not implemented
  TG4ExtDecayerMessenger& operator=(const TG4ExtDecayerMessenger& right);

  //
  // data members

  /// associated class
  TG4ExtDecayerPhysics* fExtDecayerPhysics;

  /// setExtDecayerSelection command
  G4UIcmdWithAString* fSetSelectionCmd;

  /// skipExtDecayerNeutrino command
  G4UIcmdWithABool* fSkipNeutrinoCmd;
};

#endif // TG4_EXT_DECAYER_MESSENGER_H
