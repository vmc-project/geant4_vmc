// $Id: TG4UserParticlesPhysics.h 295 2007-10-04 23:17:33Z rdm $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics_list
/// \class TG4UserParticlesPhysics
/// \brief The builder for user defined particles.
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_USER_PARTICLES_PHYSICS_H
#define TG4_USER_PARTICLES_PHYSICS_H

#include "TG4VPhysicsConstructor.h"
#include <globals.hh>

class TG4UserParticlesPhysics: public TG4VPhysicsConstructor
{
  public:
    TG4UserParticlesPhysics(const G4String& name = "userParticles");
    TG4UserParticlesPhysics(G4int verboseLevel,
                         const G4String& name = "userParticles");
    virtual ~TG4UserParticlesPhysics();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    TG4UserParticlesPhysics(const TG4UserParticlesPhysics& right);
    TG4UserParticlesPhysics& operator=(const TG4UserParticlesPhysics& right);
};

#endif //TG4_USER_PARTICLES_PHYSICS_H

