#ifndef TG4_USER_PARTICLES_PHYSICS_H
#define TG4_USER_PARTICLES_PHYSICS_H

// $Id: TG4UserParticlesPhysics.h 295 2007-10-04 23:17:33Z rdm $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4UserParticlesPhysics.h
/// \brief Definition of the TG4UserParticlesPhysics class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4VPhysicsConstructor.h"
#include <globals.hh>

/// \ingroup physics_list
/// \brief The builder for user defined particles.
///
/// \author I. Hrivnacova; IPN Orsay

class TG4UserParticlesPhysics: public TG4VPhysicsConstructor
{
  public:
    TG4UserParticlesPhysics(const G4String& name = "UserParticles");
    TG4UserParticlesPhysics(G4int verboseLevel,
                         const G4String& name = "UserParticles");
    virtual ~TG4UserParticlesPhysics();

  protected:
    // methods
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    /// Not implemented
    TG4UserParticlesPhysics(const TG4UserParticlesPhysics& right);
    /// Not implemented
    TG4UserParticlesPhysics& operator=(const TG4UserParticlesPhysics& right);
};

#endif //TG4_USER_PARTICLES_PHYSICS_H

