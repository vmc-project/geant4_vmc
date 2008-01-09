// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4SpecialControlsPhysics.cxx
/// \brief Implementation of the TG4SpecialControlsPhysics class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SpecialControlsPhysics.h"
#include "TG4SpecialControls.h"
#include "TG4G3PhysicsManager.h"
#include "TG4G3ControlVector.h"
#include "TG4ProcessMCMap.h"
#include "TG4SpecialControls.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4VProcess.hh>

#include <TMCProcess.h>

//_____________________________________________________________________________
TG4SpecialControlsPhysics::TG4SpecialControlsPhysics(
                                     const G4String& name)
  : TG4VPhysicsConstructor(name),
    fSpecialControls(0) 
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4SpecialControlsPhysics::TG4SpecialControlsPhysics(
                                     G4int verboseLevel, 
                                     const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel),
    fSpecialControls(0) 
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4SpecialControlsPhysics::~TG4SpecialControlsPhysics() 
{
/// Destructor

  delete fSpecialControls;
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4SpecialControlsPhysics::ConstructParticle()
{
/// The particles are constructed in the physics list.
}

//_____________________________________________________________________________
void TG4SpecialControlsPhysics::ConstructProcess()
{
/// Add TG4SpecialControls "process" that activates
/// the control process controls defined in TG4Limits.

 fSpecialControls = new TG4SpecialControls("specialControls"); 

  TG4G3PhysicsManager* g3PhysicsManager 
    = TG4G3PhysicsManager::Instance();

  theParticleIterator->reset();
  while ((*theParticleIterator)())
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    TG4G3ParticleWSP particleWSP 
      = g3PhysicsManager->GetG3ParticleWSP(particle);

    if ( particleWSP != kNofParticlesWSP ) {
        // special process is created in any case
        particle->GetProcessManager()->AddDiscreteProcess(fSpecialControls);
    }
  }  

  if (VerboseLevel() > 0) {
    G4cout << "### Special Controls constructed. " << G4endl;
  }  
}

