#ifndef TG4_SPECIAL_CONTROLS_PHYSICS_H
#define TG4_SPECIAL_CONTROLS_PHYSICS_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4SpecialControlsPhysics.h
/// \brief Definition of the TG4SpecialControlsPhysics class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

class TG4SpecialControls;

/// \ingroup physics_list
/// \brief The builder for special controls process.
///
/// \author I. Hrivnacova; IPN Orsay

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
    TG4SpecialControls*  fSpecialControls; ///< the special controls process
};

#endif //TG4_SPECIAL_CONTROLS_PHYSICS_H

