// $Id: TG4PhysicsConstructorSpecialCuts.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
/// \ingroup physics
//
/// \class TG4PhysicsConstructorSpecialCuts
/// 
/// Constructor of special cuts.
///
/// Author: I. Hrivnacova

#ifndef TG4_PHYSICS_CONSTRUCTOR_SPECIAL_CUTS_H
#define TG4_PHYSICS_CONSTRUCTOR_SPECIAL_CUTS_H

#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

class TG4PhysicsConstructorSpecialCuts: public TG4VPhysicsConstructor
{
  public:
    TG4PhysicsConstructorSpecialCuts(const G4String& name = "Special cuts");
    TG4PhysicsConstructorSpecialCuts(G4int verboseLevel,
                                     const G4String& name = "Special cuts");
    virtual ~TG4PhysicsConstructorSpecialCuts();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();
};

#endif //TG4_PHYSICS_CONSTRUCTOR_SPECIAL_CUTS_H

