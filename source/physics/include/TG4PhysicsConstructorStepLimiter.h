// $Id: TG4PhysicsConstructorStepLimiter.h,v 1.4 2006/12/12 16:21:16 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4PhysicsConstructorStepLimiter
/// \brief Constructor of step limiter physics.
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_PHYSICS_CONSTRUCTOR_STEP_LIMITER_H
#define TG4_PHYSICS_CONSTRUCTOR_STEP_LIMITER_H

#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

class G4StepLimiter;

class TG4PhysicsConstructorStepLimiter: public TG4VPhysicsConstructor
{
  public:
    TG4PhysicsConstructorStepLimiter(const G4String& name = "StepLimiter");
    TG4PhysicsConstructorStepLimiter(G4int verboseLevel,
                                     const G4String& name = "StepLimiter");
    virtual ~TG4PhysicsConstructorStepLimiter();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    // data members
    G4StepLimiter* fStepLimiterProcess; // step limiter process

  private:
    TG4PhysicsConstructorStepLimiter(const TG4PhysicsConstructorStepLimiter& right);
    TG4PhysicsConstructorStepLimiter& operator=(const TG4PhysicsConstructorStepLimiter& right);  
};

#endif //TG4_PHYSICS_CONSTRUCTOR_STEP_LIMITER_H

