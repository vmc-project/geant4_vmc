//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
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
TG4StackPopperPhysics::TG4StackPopperPhysics(G4int theVerboseLevel,
                                             const G4String& name)
  : TG4VPhysicsConstructor(name, theVerboseLevel),
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
/// Set stack popper process to selected particles or all particles
/// if no particles were selected

  fStackPopperProcess = new TG4StackPopper();

  auto aParticleIterator = GetParticleIterator();
  aParticleIterator->reset();
  while ( (*aParticleIterator)() ) {

    G4ParticleDefinition* particle = aParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    // skip particles which do not have process manager
    if ( ! pmanager ) continue;

    // add this as an option
    if ( fSelection.size() == 0 ||
         fSelection.find(particle->GetParticleName()) != std::string::npos ) {

      if (VerboseLevel() > 1) {
        G4cout << "Adding StackPopper process to "
               <<  particle->GetParticleName() << G4endl;
      }

      pmanager->AddProcess(fStackPopperProcess);
      pmanager->SetProcessOrdering(fStackPopperProcess,idxPostStep);
    }
  }

  if (VerboseLevel() > 0) {
    G4cout << "### Stack popper physics constructed." << G4endl;
  }
}
