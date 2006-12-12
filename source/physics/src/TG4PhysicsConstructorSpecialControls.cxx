// $Id: TG4PhysicsConstructorSpecialControls.cxx,v 1.5 2006/01/13 16:59:39 brun Exp $
// Category: physics
//
// Class TG4PhysicsConstructorSpecialControls
// ------------------------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4PhysicsConstructorSpecialControls.h"
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
TG4PhysicsConstructorSpecialControls::TG4PhysicsConstructorSpecialControls(
                                     const G4String& name)
  : TG4VPhysicsConstructor(name),
    fSpecialControls(0) 
{
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorSpecialControls::TG4PhysicsConstructorSpecialControls(
                                     G4int verboseLevel, 
                                     const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel),
    fSpecialControls(0) 
{
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorSpecialControls::~TG4PhysicsConstructorSpecialControls() 
{
//
  delete fSpecialControls;
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4PhysicsConstructorSpecialControls::ConstructParticle()
{
/// The particles are constructed in the physics list.
}

//_____________________________________________________________________________
void TG4PhysicsConstructorSpecialControls::ConstructProcess()
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

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fSpecialControls, kPNull);

  if (VerboseLevel() > 0) {
    G4cout << "### Special Controls constructed. " << G4endl;
  }  
}

