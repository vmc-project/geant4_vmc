#ifndef TG4_HEPEM_TRACKING_PHYSICS_H
#define TG4_HEPEM_TRACKING_PHYSICS_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2025 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4HepEmTrackingPhysics.h
/// \brief Definition of the TG4HepEmTrackingPhysics class

#include <G4VPhysicsConstructor.hh>
#include <globals.hh>

/// \ingroup physics_list
/// \brief EM physics with G4HepEm for e-, e+ and gamma
///
/// The G4HepEm tracking manager transports e-, e+ and gamma; the other
/// particles get the EM processes of G4EmStandardPhysics_option1 (EMV).

class TG4HepEmTrackingPhysics : public G4VPhysicsConstructor
{
 public:
  TG4HepEmTrackingPhysics(G4int verbose = 1);
  ~TG4HepEmTrackingPhysics() override = default;

  void ConstructParticle() override;
  void ConstructProcess() override;
};

#endif // TG4_HEPEM_TRACKING_PHYSICS_H
