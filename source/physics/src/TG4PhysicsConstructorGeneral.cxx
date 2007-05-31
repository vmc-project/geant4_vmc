// $Id: TG4PhysicsConstructorGeneral.cxx,v 1.7 2006/12/12 16:21:16 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics
// Class TG4PhysicsConstructorGeneral
// ----------------------------------
// See the class description in the header file.

#include "TG4PhysicsConstructorGeneral.h"
#include "TG4ProcessControlMap.h"
#include "TG4ProcessMCMap.h"
#include "TG4ExtDecayer.h"

#include <TVirtualMCDecayer.h>
#include <TVirtualMC.h>

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4ChargedGeantino.hh>
#include <G4Geantino.hh>

#include <G4Decay.hh>

//_____________________________________________________________________________
TG4PhysicsConstructorGeneral::TG4PhysicsConstructorGeneral(const G4String& name)
  : TG4VPhysicsConstructor(name),
    fDecayProcess(0)
{
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorGeneral::TG4PhysicsConstructorGeneral(
                                                   G4int verboseLevel,
                                                   const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel), 
    fDecayProcess(0)
{
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorGeneral::~TG4PhysicsConstructorGeneral() 
{
//
  delete fDecayProcess;
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

  // Create process
  fDecayProcess = new G4Decay();

  // Set external decayer
  TVirtualMCDecayer* mcDecayer = gMC->GetDecayer(); 
  if (mcDecayer) {
    TG4ExtDecayer* tg4Decayer = new TG4ExtDecayer(mcDecayer);
       // the tg4Decayer is deleted in G4Decay destructor
    tg4Decayer->VerboseLevel(VerboseLevel());   
    fDecayProcess->SetExtDecayer(tg4Decayer);
    
    if (VerboseLevel() > 0) { 
      G4cout << "### External decayer is set" << G4endl;
    }  
  } 

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (fDecayProcess->IsApplicable(*particle)) { 
      pmanager ->AddProcess(fDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(fDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(fDecayProcess, idxAtRest);
    }
  }
  
  // map to G3 controls
  TG4ProcessControlMap* processMap = TG4ProcessControlMap::Instance();
  processMap->Add(fDecayProcess, kDCAY); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fDecayProcess, kPDecay); 
  mcMap->Add("Transportation", kPTransportation); 

  if (VerboseLevel() > 0) {
    G4cout << "### General physics constructed." << G4endl;
  }  
}
