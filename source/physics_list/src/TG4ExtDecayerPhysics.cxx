//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
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
    fMessenger(this),
    fDecayProcess(0),
    fSelection(),
    fSkipNeutrino(false)
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4ExtDecayerPhysics::TG4ExtDecayerPhysics(G4int theVerboseLevel,
                                           const G4String& name)
  : TG4VPhysicsConstructor(name, theVerboseLevel),
    fMessenger(this),
    fDecayProcess(0),
    fSelection(),
    fSkipNeutrino(false)
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
  tg4Decayer->SetSkipNeutrino(fSkipNeutrino);
     // The tg4Decayer is deleted in G4Decay destructor
     // But we may have a problem if there are more than one
     // instances of G4Decay process

  auto aParticleIterator = GetParticleIterator();
  aParticleIterator->reset();
  while ((*aParticleIterator)())
  {
    G4ParticleDefinition* particle = aParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    // skip particles which do not have process manager
    if ( ! pmanager ) continue;

    if ( fSelection.find(particle->GetParticleName()) != std::string::npos ) {

      if ( VerboseLevel() > 1 ) {
        G4cout << "Switching off Geant4 decay table for: "
               <<  particle->GetParticleName()
               << G4endl;
      }

      // Unset the decay table for particles in a selection;
      // for the particles in selection, the external decayer
      // will have a priority over Geant4 decay table
      particle->SetDecayTable(0);
    }

    if ( VerboseLevel() > 1 ) {
      G4cout << "Setting ext decayer for: "
             <<  aParticleIterator->value()->GetParticleName()
             << G4endl;
    }

    G4ProcessVector* processVector = pmanager->GetProcessList();
    for (G4int i=0; i<processVector->length(); i++) {

      G4Decay* decay = dynamic_cast<G4Decay*>((*processVector)[i]);
      if ( decay ) decay->SetExtDecayer(tg4Decayer);
    }
  }

  if ( VerboseLevel() > 0 ) {
    G4cout << "### " << "External decayer physics constructed." << G4endl;
  }
}
