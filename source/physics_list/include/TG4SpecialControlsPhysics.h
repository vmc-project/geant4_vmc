// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics_list
/// \class TG4SpecialControlsPhysics
/// \brief The builder for special controls process.
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_SPECIAL_CONTROLS_PHYSICS_H
#define TG4_SPECIAL_CONTROLS_PHYSICS_H

#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

class TG4SpecialControls;

class TG4SpecialControlsPhysics: public TG4VPhysicsConstructor
{
  public:
    TG4SpecialControlsPhysics(
      const G4String& name = "Special controls");
    TG4SpecialControlsPhysics(
      G4int verboseLevel,
      const G4String& name = "Special controls");
    virtual ~TG4SpecialControlsPhysics();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    // data members
    TG4SpecialControls*  fSpecialControls;
};

#endif //TG4_SPECIAL_CONTROLS_PHYSICS_H

