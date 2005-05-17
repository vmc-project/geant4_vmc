// $Id: TG4PhysicsConstructorOptical.h,v 1.3 2004/11/10 11:39:28 brun Exp $
/// \ingroup physics
//
/// \class TG4PhysicsConstructorOptical
/// 
/// Constructor of optical physics.
/// According to ExN06PhysicsList (geant4 6.0)
///
/// Author: I. Hrivnacova

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

    // set emthods
    void SetMaxNumPhotonsPerStep(G4int maxNofPhotons);

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    static  const G4int  fgkDefaultMaxNumPhotonsPerStep;
          
  
    G4Cerenkov*          fCerenkovProcess;           // Cerenkov 
    G4Scintillation*     fScintillationProcess;      // scintillation
    G4OpAbsorption*      fAbsorptionProcess;         // absorption
    G4OpRayleigh*        fRayleighScatteringProcess; // Rayleigh
    G4OpBoundaryProcess* fBoundaryProcess;           // boundary process
    G4int                fMaxNumPhotonsPerStep;      // max number of Cerenkov
                                                     // photons per step
};

#endif //TG4_PHYSICS_CONSTRUCTOR_OPTICAL_H

