// $Id: TG4PhysicsConstructorGeneral.cxx,v 1.3 2003/12/18 13:27:46 brun Exp $
// Category: physics
//
// Class TG4PhysicsConstructorGeneral
// ------------------------------
// See the class description in the header file.
// According to ExN04IonPhysics.cc,v 1.1.2.1 2001/06/28 19:07:37 gunter Exp 
// GEANT4 tag Name: geant4-06-00
//
// Author: I. Hrivnacova

#include "TG4PhysicsConstructorGeneral.h"
#include "TG4ProcessControlMap.h"
#include "TG4ProcessMCMap.h"
#include "TG4G3Control.h"
#include "TG4ExtDecayer.h"

#include <TVirtualMCDecayer.h>
#include <TVirtualMC.h>

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4ChargedGeantino.hh>
#include <G4Geantino.hh>

//_____________________________________________________________________________
TG4PhysicsConstructorGeneral::TG4PhysicsConstructorGeneral(const G4String& name)
  : TG4VPhysicsConstructor(name) {
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorGeneral::TG4PhysicsConstructorGeneral(
						   G4int verboseLevel,
                                                   const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel) {
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorGeneral::~TG4PhysicsConstructorGeneral() {
//
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4PhysicsConstructorGeneral::ConstructParticle()
{
/// Instantiate particles.

  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();  
}

//_____________________________________________________________________________
void TG4PhysicsConstructorGeneral::ConstructProcess()
{
/// Construct electromagnetic processes for e+.

  // Set external decayer
  TVirtualMCDecayer* mcDecayer = gMC->GetDecayer(); 
  if (mcDecayer) {
    TG4ExtDecayer* tg4Decayer = new TG4ExtDecayer(mcDecayer);
       // the tg4Decayer is deleted in G4Decay destructor
    tg4Decayer->VerboseLevel(VerboseLevel());   
    fDecayProcess.SetExtDecayer(tg4Decayer);
    
    if (VerboseLevel() > 0) { 
      G4cout << "### External decayer is set" << G4endl;
    }  
  } 

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (fDecayProcess.IsApplicable(*particle)) { 
      pmanager ->AddProcess(&fDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(&fDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(&fDecayProcess, idxAtRest);
    }
  }
  
  // map to G3 controls
  TG4ProcessControlMap* processMap = TG4ProcessControlMap::Instance();
  processMap->Add(&fDecayProcess, kDCAY); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add("Decay", kPDecay); 
  mcMap->Add("Transportation", kPTransportation); 

  if (VerboseLevel() > 0) {
    G4cout << "### General physics constructed." << G4endl;
  }  
}
