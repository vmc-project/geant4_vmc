#ifndef TG4_PROCESS_MC_MAP_PHYSICS_H
#define TG4_PROCESS_MC_MAP_PHYSICS_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ProcessMCMapPhysics.h
/// \brief Definition of the TG4ProcessMCMapPhysics class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4VPhysicsConstructor.h"
#include <globals.hh>

class G4Decay;

/// \ingroup physics_list
/// \brief The special builder which fills the VMC process maps
///
/// \author I. Hrivnacova; IPN Orsay

class TG4ProcessMCMapPhysics : public TG4VPhysicsConstructor
{
 public:
  TG4ProcessMCMapPhysics(
    G4bool isBiasing, const G4String& name = "ProcessMCMap");
  TG4ProcessMCMapPhysics(
    G4bool isBiasing, G4int theVerboseLevel, const G4String& name = "ProcessMCMap");
  virtual ~TG4ProcessMCMapPhysics();

 protected:
  // methods
  // construct particle and physics
  virtual void ConstructParticle();
  virtual void ConstructProcess();

 private:
  /// Not implemented
  TG4ProcessMCMapPhysics(const TG4ProcessMCMapPhysics& right);
  /// Not implemented
  TG4ProcessMCMapPhysics& operator=(const TG4ProcessMCMapPhysics& right);

  void FillMap(G4bool isBiasing);
};

#endif // TG4_PROCESS_MAP_PHYSICS_H
