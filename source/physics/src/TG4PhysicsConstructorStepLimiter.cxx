// $Id: $
// Category: physics
//
// Class TG4PhysicsConstructorStepLimiter
// --------------------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4PhysicsConstructorStepLimiter.h"
#include "TG4ProcessControlMap.h"
#include "TG4ProcessMCMap.h"

#include <TMCProcess.h>

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>

//_____________________________________________________________________________
TG4PhysicsConstructorStepLimiter::TG4PhysicsConstructorStepLimiter(
                                                   const G4String& name)
  : TG4VPhysicsConstructor(name) {
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorStepLimiter::TG4PhysicsConstructorStepLimiter(
						   G4int verboseLevel,
                                                   const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel) {
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorStepLimiter::~TG4PhysicsConstructorStepLimiter() {
//
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

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){

    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    pmanager ->AddProcess(&fStepLimiterProcess, -1, -1, 6);
  }
  
  // no G3 control for this process

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add("StepLimiter", kStepMax); 

  if (VerboseLevel() > 0) {
    G4cout << "### Step limiter physics constructed." << G4endl;
  }  
}
