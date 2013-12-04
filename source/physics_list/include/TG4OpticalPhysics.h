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
#include "TG4OpticalProcess.h"

#include <G4OpticalSurface.hh>
#include <globals.hh>

#include <vector>

class G4VProcess;

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
    TG4OpticalPhysics(G4int theVerboseLevel,
                      const G4String& name = "Optical");
    virtual ~TG4OpticalPhysics();

    // set methods
    void SetProcessActivation(TG4OpticalProcess process, G4bool isActive);
    void SetProcessVerbose(TG4OpticalProcess process, G4int theVerboseLevel);

    void SetMaxNumPhotonsPerStep(G4int maxNofPhotons);
    void SetMaxBetaChangePerStep(G4double maxBetaChange);
    void SetScintillationYieldFactor(G4double yieldFactor);
    void SetOpticalSurfaceModel(G4OpticalSurfaceModel model);
    void SetTrackSecondariesFirst(G4bool trackSecondariesFirst);

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
    
    // methods
    void PrintStatistics() const;

    //
    // static data members
    
    /// Default value for processes verbose level
    static const G4int  fgkDefaultVerboseLevel;

    /// Default value for maximum number of Cerenkov photons per step
    static const G4int  fgkDefaultMaxNumPhotons;

    /// Default value for max change of beta per step
    static const G4double  fgkDefaultMaxBetaChange;
          
    /// Default value for scintillation yield factor
    static const G4double  fgkDefaultYieldFactor;
          
    /// Default value for optical surface model
    static const G4OpticalSurfaceModel  fgkDefaultSurfaceModel;

    //
    // data members
    
    /// messenger
    TG4OpticalPhysicsMessenger  fMessenger;
    
    /// The vector of optical processes
    std::vector<G4VProcess*>    fProcesses;

    /// The vector of processes activations
    std::vector<G4bool>         fProcessActivation; 
    
    /// The vector of processes verbose level
    std::vector<G4int>          fProcessVerbose; 
    
    /// max number of Cerenkov photons per step
    G4int                       fMaxNumPhotons;   

    /// max change of beta per step
    G4double                    fMaxBetaChange;   

    /// scintillation yield factor
    G4double                    fYieldFactor;   

    /// the optical surface model (glisur or unified)
    G4OpticalSurfaceModel       fSurfaceModel;   

    /// option to track secondaries before finishing their parent track
    G4bool                      fTrackSecondariesFirst;   
};

#endif //TG4_OPTICAL_PHYSICS_H

