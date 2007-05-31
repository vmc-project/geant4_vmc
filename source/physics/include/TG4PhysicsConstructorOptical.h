// $Id: TG4PhysicsConstructorOptical.h,v 1.6 2006/12/12 16:21:16 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4PhysicsConstructorOptical
/// \brief Constructor of optical physics.
///
/// According to ExN06PhysicsList (geant4 6.0)
///
/// \author I. Hrivnacova; IPN Orsay

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
    TG4PhysicsConstructorOptical(const TG4PhysicsConstructorOptical& right);
    TG4PhysicsConstructorOptical& operator=(const TG4PhysicsConstructorOptical& right);

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

