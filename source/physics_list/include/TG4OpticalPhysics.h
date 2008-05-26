#ifndef TG4_OPTICAL_PHYSICS_H
#define TG4_OPTICAL_PHYSICS_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4OpticalPhysics.h
/// \brief Definition of the TG4OpticalPhysics class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4VPhysicsConstructor.h"

#include "TG4OpticalPhysicsMessenger.h"

#include <globals.hh>

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpBoundaryProcess;

/// \ingroup physics_list
/// \brief Optical physics builder.
///
/// According to ExN06PhysicsList (geant4 6.0)
///
/// \author I. Hrivnacova; IPN Orsay

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
    /// Not implemented
    TG4OpticalPhysics(const TG4OpticalPhysics& right);
    /// Not implemented
    TG4OpticalPhysics& operator=(const TG4OpticalPhysics& right);

    //
    // static data members
    
    /// Default value for maximum number of Cerenkov photons per step
    static const G4int  fgkDefaultMaxNumPhotonsPerStep;
          
    //
    // data members
    
    /// messenger
    TG4OpticalPhysicsMessenger  fMessenger;
    
    /// Cerenkov process
    G4Cerenkov*          fCerenkovProcess;
    
    /// scintillation process
    G4Scintillation*     fScintillationProcess;
    
    /// optical absorption process
    G4OpAbsorption*      fAbsorptionProcess;
    
    /// Rayleigh scattering process
    G4OpRayleigh*        fRayleighScatteringProcess;
    
    /// optical boundary process
    G4OpBoundaryProcess* fBoundaryProcess;
    
    /// max number of Cerenkov photons per step
    G4int                fMaxNumPhotonsPerStep;   
};

#endif //TG4_OPTICAL_PHYSICS_H

