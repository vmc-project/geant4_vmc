#ifndef TG4_PROCESS_MAP_PHYSICS_H
#define TG4_PROCESS_MAP_PHYSICS_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2022 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ProcessMapPhysics.h
/// \brief Definition of the TG4ProcessMapPhysics class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4VPhysicsConstructor.h"
#include <globals.hh>

/// \ingroup physics_list
/// \brief Extension of Geant4 hadronic process type
///
/// \author I. Hrivnacova; IJCLab, Orsay

enum TG4HadronicProcessType
{
  fElectronNuclear = 171,
  fPositronNuclear = 172,
  fMuonNuclear     = 173,
  fPhotoNuclear    = 174
};

/// \ingroup physics_list
/// \brief Definition of Geant4 biasing process type
///
/// \author I. Hrivnacova; IJCLab, Orsay

enum TG4BiasingProcessType
{
  fBiasing = 501
};

/// \ingroup physics_list
/// \brief The special builder which fills the VMC process map
///
/// \author I. Hrivnacova; IJCLab, Orsay

class TG4ProcessMapPhysics : public TG4VPhysicsConstructor
{
 public:
  TG4ProcessMapPhysics(const G4String& name = "ProcessMap");
  TG4ProcessMapPhysics(G4int theVerboseLevel,
    const G4String& name = "ProcessMap");
  TG4ProcessMapPhysics(const TG4ProcessMapPhysics& right) = delete;
  TG4ProcessMapPhysics& operator=(const TG4ProcessMapPhysics& right) = delete;
  ~TG4ProcessMapPhysics() override;

 protected:
  // methods
  // construct particle and physics
  void ConstructParticle() override;
  void ConstructProcess() override;

 private:
  void FillMap();
  void UpdateHadronicProcessSubType(G4VProcess* process);
  G4bool UpdateOrSkipBiasingProcess(G4VProcess* process);
};

#endif // TG4_PROCESS_MAP_PHYSICS_H
