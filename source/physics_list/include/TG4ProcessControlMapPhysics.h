#ifndef TG4_PROCESS_MAP_PHYSICS_H
#define TG4_PROCESS_MAP_PHYSICS_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ProcessControlMapPhysics.h
/// \brief Definition of the TG4ProcessControlMapPhysics class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4VPhysicsConstructor.h"
#include <globals.hh>

class G4Decay;

/// \ingroup physics_list
/// \brief Special builder which fills the VMC process control map
///
/// \author I. Hrivnacova; IPN Orsay

class TG4ProcessControlMapPhysics : public TG4VPhysicsConstructor
{
 public:
  TG4ProcessControlMapPhysics(
    G4bool isBiasing, const G4String& name = "ProcessControlMap");
  TG4ProcessControlMapPhysics(
    G4bool isBiasing, G4int theVerboseLevel, const G4String& name = "ProcessControlMap");
  virtual ~TG4ProcessControlMapPhysics();

 protected:
  // methods
  // construct particle and physics
  virtual void ConstructParticle();
  virtual void ConstructProcess();

 private:
  /// Not implemented
  TG4ProcessControlMapPhysics(const TG4ProcessControlMapPhysics& right);
  /// Not implemented
  TG4ProcessControlMapPhysics& operator=(
    const TG4ProcessControlMapPhysics& right);

  void FillMap(G4bool isBiasing);
};

#endif // TG4_PROCESS_MAP_PHYSICS_H
