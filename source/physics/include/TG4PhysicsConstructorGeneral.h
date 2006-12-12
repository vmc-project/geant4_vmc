// $Id: TG4PhysicsConstructorGeneral.h,v 1.6 2006/01/13 16:59:38 brun Exp $
/// \ingroup physics
//
/// \class TG4PhysicsConstructorGeneral
/// \brief Constructor of general physics.
///
/// According to ExN04GeneralPhysics, GEANT4 tag Name: geant4-06-00
///
/// Author: I. Hrivnacova

#ifndef TG4_PHYSICS_CONSTRUCTOR_GENERAL_H
#define TG4_PHYSICS_CONSTRUCTOR_GENERAL_H

#include "TG4VPhysicsConstructor.h"
#include <globals.hh>

class G4Decay;

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
    G4Decay* fDecayProcess; // decay process

  private:
    TG4PhysicsConstructorGeneral(const TG4PhysicsConstructorGeneral& right);
    TG4PhysicsConstructorGeneral& operator=(const TG4PhysicsConstructorGeneral& right);
};

#endif //TG4_PHYSICS_CONSTRUCTOR_GENERAL_H

