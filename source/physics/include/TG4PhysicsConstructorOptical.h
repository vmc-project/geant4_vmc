// $Id: TG4PhysicsConstructorOptical.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: physics
//
// Author: I. Hrivnacova
//
// Class TG4PhysicsConstructorOptical
// ----------------------------------
// Constructor of optical physics.
// According to ExN06PhysicsList (geant4 6.0)

#ifndef TG4_PHYSICS_CONSTRUCTOR_OPTICAL_H
#define TG4_PHYSICS_CONSTRUCTOR_OPTICAL_H

#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpBoundaryProcess;

class TG4PhysicsConstructorOptical: public TG4VPhysicsConstructor
{
  public:
    TG4PhysicsConstructorOptical(const G4String& name = "Optical");
    TG4PhysicsConstructorOptical(G4int verboseLevel,
                                 const G4String& name = "Optical");
    virtual ~TG4PhysicsConstructorOptical();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    G4Cerenkov*          fCerenkovProcess;           // Cerenkov 
    G4Scintillation*     fScintillationProcess;      // scintillation
    G4OpAbsorption*      fAbsorptionProcess;         // absorption
    G4OpRayleigh*        fRayleighScatteringProcess; // Rayleigh
    G4OpBoundaryProcess* fBoundaryProcess;           // boundary process
};

#endif //TG4_PHYSICS_CONSTRUCTOR_OPTICAL_H

