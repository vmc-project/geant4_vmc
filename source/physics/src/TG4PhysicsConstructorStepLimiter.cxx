// $Id: TG4PhysicsConstructorStepLimiter.cxx,v 1.3 2006/12/12 16:21:16 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics
// Class TG4PhysicsConstructorStepLimiter
// --------------------------------------
// See the class description in the header file.

#include "TG4PhysicsConstructorStepLimiter.h"
#include "TG4ProcessControlMap.h"
#include "TG4ProcessMCMap.h"

#include <TMCProcess.h>

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4StepLimiter.hh>

//_____________________________________________________________________________
TG4PhysicsConstructorStepLimiter::TG4PhysicsConstructorStepLimiter(
                                                   const G4String& name)
  : TG4VPhysicsConstructor(name),
    fStepLimiterProcess(0) 
{
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorStepLimiter::TG4PhysicsConstructorStepLimiter(
                                                   G4int verboseLevel,
                                                   const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel), 
    fStepLimiterProcess(0) 
{
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorStepLimiter::~TG4PhysicsConstructorStepLimiter() 
{
//
  delete fStepLimiterProcess;
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4PhysicsConstructorStepLimiter::ConstructParticle()
{
/// No particles instatiated 

}

//_____________________________________________________________________________
void TG4PhysicsConstructorStepLimiter::ConstructProcess()
{
/// Set step limiter process to all particles

  fStepLimiterProcess = new G4StepLimiter();

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){

    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    pmanager ->AddProcess(fStepLimiterProcess, -1, -1, 6);
  }
  
  // no G3 control for this process

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add("StepLimiter", kStepMax); 

  if (VerboseLevel() > 0) {
    G4cout << "### Step limiter physics constructed." << G4endl;
  }  
}
