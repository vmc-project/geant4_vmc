// $Id: TG4PhysicsConstructorStepLimiter.h,v 1.3 2006/01/13 16:59:38 brun Exp $
/// \ingroup physics
//
/// \class TG4PhysicsConstructorStepLimiter
/// \brief Constructor of step limiter physics.
///
/// Author: I. Hrivnacova

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

