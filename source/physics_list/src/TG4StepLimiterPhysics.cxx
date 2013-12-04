// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4StepLimiterPhysics.cxx
/// \brief Implementation of the TG4StepLimiterPhysics class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4StepLimiterPhysics.h"
#include "TG4ProcessControlMap.h"
#include "TG4ProcessMCMap.h"

#include <TMCProcess.h>

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4StepLimiter.hh>

//_____________________________________________________________________________
TG4StepLimiterPhysics::TG4StepLimiterPhysics(const G4String& name)
  : TG4VPhysicsConstructor(name),
    fStepLimiterProcess(0) 
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4StepLimiterPhysics::TG4StepLimiterPhysics(G4int theVerboseLevel,
                                             const G4String& name)
  : TG4VPhysicsConstructor(name, theVerboseLevel), 
    fStepLimiterProcess(0) 
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4StepLimiterPhysics::~TG4StepLimiterPhysics() 
{
/// Destructor

  delete fStepLimiterProcess;
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4StepLimiterPhysics::ConstructParticle()
{
/// No particles instatiated 

}

//_____________________________________________________________________________
void TG4StepLimiterPhysics::ConstructProcess()
{
/// Set step limiter process to all particles

  fStepLimiterProcess = new G4StepLimiter();

  aParticleIterator->reset();
  while( (*aParticleIterator)() ){

    G4ParticleDefinition* particle = aParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    pmanager ->AddProcess(fStepLimiterProcess, -1, -1, 6);
  }
  
  if (VerboseLevel() > 0) {
    G4cout << "### Step limiter physics constructed." << G4endl;
  }  
}
