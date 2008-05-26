#ifndef TG4_STEP_LIMITER_PHYSICS_H
#define TG4_STEP_LIMITER_PHYSICS_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4StepLimiterPhysics.h
/// \brief Definition of the TG4StepLimiterPhysics class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

class G4StepLimiter;

/// \ingroup physics_list
/// \brief The builder for step limiter process.
///
/// \author I. Hrivnacova; IPN Orsay

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
    G4StepLimiter* fStepLimiterProcess; ///< step limiter process

  private:
    /// Not implemented
    TG4StepLimiterPhysics(const TG4StepLimiterPhysics& right);
    /// Not implemented
    TG4StepLimiterPhysics& operator=(const TG4StepLimiterPhysics& right);  
};

#endif //TG4_STEP_LIMITER_PHYSICS_H

