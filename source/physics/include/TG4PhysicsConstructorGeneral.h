// $Id: TG4PhysicsConstructorGeneral.h,v 1.2 2003/12/18 13:27:46 brun Exp $
/// \ingroup physics
//
/// \class TG4PhysicsConstructorGeneral
/// 
/// Constructor of physics for ions.
/// According to ExN04GeneralPhysics, GEANT4 tag Name: geant4-06-00
///
/// Author: I. Hrivnacova

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

