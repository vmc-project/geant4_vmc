// $Id: TG4PhysicsConstructorSpecialControls.h,v 1.5 2006/01/13 16:59:38 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4PhysicsConstructorSpecialControls
/// \brief Constructor of special controls of physics processes.
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_PHYSICS_CONSTRUCTOR_SPECIAL_CONTROLS_H
#define TG4_PHYSICS_CONSTRUCTOR_SPECIAL_CONTROLS_H

#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

class TG4SpecialControls;

class TG4PhysicsConstructorSpecialControls: public TG4VPhysicsConstructor
{
  public:
    TG4PhysicsConstructorSpecialControls(
      const G4String& name = "Special controls");
    TG4PhysicsConstructorSpecialControls(
      G4int verboseLevel,
      const G4String& name = "Special controls");
    virtual ~TG4PhysicsConstructorSpecialControls();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    // data members
    TG4SpecialControls*  fSpecialControls;
};

#endif //TG4_PHYSICS_CONSTRUCTOR_SPECIAL_CONTROLS_H

