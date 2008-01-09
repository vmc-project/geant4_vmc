// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4ExtDecayerPhysics.cxx
/// \brief Implementation of the TG4ExtDecayerPhysics class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ExtDecayerPhysics.h"
#include "TG4ExtDecayer.h"
#include "TG4Globals.h"

#include <TVirtualMCDecayer.h>
#include <TVirtualMC.h>

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4Decay.hh>

//_____________________________________________________________________________
TG4ExtDecayerPhysics::TG4ExtDecayerPhysics(const G4String& name)
  : TG4VPhysicsConstructor(name),
    fDecayProcess(0)
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4ExtDecayerPhysics::TG4ExtDecayerPhysics(G4int verboseLevel,
                                     const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel), 
    fDecayProcess(0)
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4ExtDecayerPhysics::~TG4ExtDecayerPhysics() 
{
/// Destructor

  delete fDecayProcess;
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4ExtDecayerPhysics::ConstructParticle()
{
/// Nothing to be done here
}

//_____________________________________________________________________________
void TG4ExtDecayerPhysics::ConstructProcess()
{
/// Loop over all particles instantiated and add external decayer
/// to all decay processes if External decayer is set

  // Check if VMC decayer is defined
  TVirtualMCDecayer* mcDecayer = gMC->GetDecayer(); 
  if ( ! mcDecayer ) {
    // TG4Globals::Warning(
    //  "TG4ExtDecayerPhysics", "ConstructProcess",
    //  "No VMC external decayer defined.");
    return;
  }    
      
  // Create Geant4 external decayer
  TG4ExtDecayer* tg4Decayer = new TG4ExtDecayer(mcDecayer);
  tg4Decayer->VerboseLevel(VerboseLevel()); 
     // The tg4Decayer is deleted in G4Decay destructor
     // But we may have a problem if there are more than one 
     // instances of G4Decay process

  theParticleIterator->reset();
  while ((*theParticleIterator)())
  {
    if ( VerboseLevel() > 1 ) {
      G4cout << "Setting ext decayer for: " 
             <<  theParticleIterator->value()->GetParticleName() 
             << G4endl;
    }         
  
    G4ProcessVector* processVector 
      = theParticleIterator->value()->GetProcessManager()->GetProcessList();
    for (G4int i=0; i<processVector->length(); i++) {
    
      G4Decay* decay = dynamic_cast<G4Decay*>((*processVector)[i]);
      if ( decay ) decay->SetExtDecayer(tg4Decayer);
    }              
  }

  if ( VerboseLevel() > 0 ) {
    G4cout << "### " << "External decayer physics constructed." << G4endl;
  }  
}
