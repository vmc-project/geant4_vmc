// $Id: TG4PhysicsConstructorGeneral.h,v 1.1 2002/06/20 11:57:45 hristov Exp $
// Category: physics
//
// Author: I. Hrivnacova
//
// Class TG4PhysicsConstructorGeneral
// -----------------------------
// Constructor of physics for ions.
// According to ExN04IonPhysics.hh, GEANT4 tag Name: geant4-03-02

#ifndef TG4_PHYSICS_CONSTRUCTOR_GENERAL_H
#define TG4_PHYSICS_CONSTRUCTOR_GENERAL_H

#include "TG4VPhysicsConstructor.h"

#include <G4Decay.hh>
#include <globals.hh>

class TG4PhysicsConstructorGeneral: public TG4VPhysicsConstructor
{
  public:
    TG4PhysicsConstructorGeneral(const G4String& name = "General");
    TG4PhysicsConstructorGeneral(G4int verboseLevel,
                                 const G4String& name = "General");
    virtual ~TG4PhysicsConstructorGeneral();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    // data members
    G4Decay fDecayProcess; // decay process
};

#endif //TG4_PHYSICS_CONSTRUCTOR_GENERAL_H

