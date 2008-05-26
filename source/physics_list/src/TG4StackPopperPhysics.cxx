// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4StackPopperPhysics.cxx
/// \brief Implementation of the TG4StackPopperPhysics class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4StackPopperPhysics.h"
#include "TG4StackPopper.h"

#include <G4ProcessManager.hh>


//_____________________________________________________________________________
TG4StackPopperPhysics::TG4StackPopperPhysics(const G4String& name)
  : TG4VPhysicsConstructor(name),
    fMessenger(this),
    fStackPopperProcess(0),
    fSelection() 
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4StackPopperPhysics::TG4StackPopperPhysics(G4int verboseLevel,
                                             const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel), 
    fMessenger(this),
    fStackPopperProcess(0), 
    fSelection() 
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4StackPopperPhysics::~TG4StackPopperPhysics() 
{
/// Destructor

  delete fStackPopperProcess;
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4StackPopperPhysics::ConstructParticle()
{
/// No particles instatiated 

}

//_____________________________________________________________________________
void TG4StackPopperPhysics::ConstructProcess()
{
/// Set step limiter process to all particles

  fStackPopperProcess = new TG4StackPopper();

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){

    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    // add this as an option
    if ( fSelection.size() == 0 ||
         fSelection.find(particle->GetParticleName()) != std::string::npos )
         
      if (VerboseLevel() > 1) {
        G4cout << "Adding StackPopper process to " 
               <<  particle->GetParticleName() << G4endl;
      }         
             
      pmanager ->AddContinuousProcess(fStackPopperProcess);
  }
  
  if (VerboseLevel() > 0) {
    G4cout << "### Stack popper physics constructed." << G4endl;
  }  
}
