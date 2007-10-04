// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics_list
/// \class TG4OpticalPhysics
/// \brief Optical physics builder.
///
/// According to ExN06PhysicsList (geant4 6.0)
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_OPTICAL_PHYSICS_H
#define TG4_OPTICAL_PHYSICS_H

#include "TG4VPhysicsConstructor.h"

#include "TG4OpticalPhysicsMessenger.h"

#include <globals.hh>

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpBoundaryProcess;

class TG4OpticalPhysics: public TG4VPhysicsConstructor
{
  public:
    TG4OpticalPhysics(const G4String& name = "Optical");
    TG4OpticalPhysics(G4int verboseLevel,
                      const G4String& name = "Optical");
    virtual ~TG4OpticalPhysics();

    // set emthods
    void SetMaxNumPhotonsPerStep(G4int maxNofPhotons);

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    TG4OpticalPhysics(const TG4OpticalPhysics& right);
    TG4OpticalPhysics& operator=(const TG4OpticalPhysics& right);

    // static data members
    static  const G4int  fgkDefaultMaxNumPhotonsPerStep;
          
    // data members
    TG4OpticalPhysicsMessenger  fMessenger;          // messenger
    G4Cerenkov*          fCerenkovProcess;           // Cerenkov 
    G4Scintillation*     fScintillationProcess;      // scintillation
    G4OpAbsorption*      fAbsorptionProcess;         // absorption
    G4OpRayleigh*        fRayleighScatteringProcess; // Rayleigh
    G4OpBoundaryProcess* fBoundaryProcess;           // boundary process
    G4int                fMaxNumPhotonsPerStep;      // max number of Cerenkov
                                                     // photons per step
};

#endif //TG4_OPTICAL_PHYSICS_H

