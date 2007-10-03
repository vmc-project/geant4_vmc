// $Id: TG4StepLimiterPhysics.h,v 1.5 2007/05/31 10:24:32 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics_list
/// \class TG4StepLimiterPhysics
/// \brief The builder for step limiter process.
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_STEP_LIMITER_PHYSICS_H
#define TG4_STEP_LIMITER_PHYSICS_H

#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

class G4StepLimiter;

class TG4StepLimiterPhysics: public TG4VPhysicsConstructor
{
  public:
    TG4StepLimiterPhysics(const G4String& name = "StepLimiter");
    TG4StepLimiterPhysics(G4int verboseLevel,
                          const G4String& name = "StepLimiter");
    virtual ~TG4StepLimiterPhysics();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    // data members
    G4StepLimiter* fStepLimiterProcess; // step limiter process

  private:
    TG4StepLimiterPhysics(const TG4StepLimiterPhysics& right);
    TG4StepLimiterPhysics& operator=(const TG4StepLimiterPhysics& right);  
};

#endif //TG4_STEP_LIMITER_PHYSICS_H

