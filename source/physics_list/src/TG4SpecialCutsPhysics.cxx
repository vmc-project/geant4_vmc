//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4SpecialCutsPhysics.cxx
/// \brief Implementation of the TG4SpecialCutsPhysics class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SpecialCutsPhysics.h"
#include "TG4G3PhysicsManager.h"
#include "TG4ProcessMCMap.h"
#include "TG4SpecialCuts.h"

#include <G4Neutron.hh>
#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4VProcess.hh>

#include <TMCProcess.h>

//_____________________________________________________________________________
TG4SpecialCutsPhysics::TG4SpecialCutsPhysics(const G4String& name)
  : TG4VPhysicsConstructor(name),
    fSpecialCutsForGamma(0),
    fSpecialCutsForElectron(0),
    fSpecialCutsForChargedHadron(0),
    fSpecialCutsForNeutralHadron(0),
    fSpecialCutsForNeutron(0),
    fSpecialCutsForMuon(0)
{
  /// Standard constructor
}

//_____________________________________________________________________________
TG4SpecialCutsPhysics::TG4SpecialCutsPhysics(
  G4int theVerboseLevel, const G4String& name)
  : TG4VPhysicsConstructor(name, theVerboseLevel),
    fSpecialCutsForGamma(0),
    fSpecialCutsForElectron(0),
    fSpecialCutsForChargedHadron(0),
    fSpecialCutsForNeutralHadron(0),
    fSpecialCutsForNeutron(0),
    fSpecialCutsForMuon(0)
{
  /// Standard constructor
}

//_____________________________________________________________________________
TG4SpecialCutsPhysics::~TG4SpecialCutsPhysics()
{
  /// Destructor

  delete fSpecialCutsForGamma;
  delete fSpecialCutsForElectron;
  delete fSpecialCutsForChargedHadron;
  delete fSpecialCutsForNeutralHadron;
  delete fSpecialCutsForNeutron;
  delete fSpecialCutsForMuon;
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4SpecialCutsPhysics::ConstructParticle()
{
  /// The particles are constructed in the physics list.
}

//_____________________________________________________________________________
void TG4SpecialCutsPhysics::ConstructProcess()
{
  /// Add TG4SpecialCuts "process" that activates
  /// the kinetic energy cuts defined in
  /// the vector of cuts (PhysicsManager::fCutVector) or in TG4Limits.

  // create processes
  fSpecialCutsForGamma = new TG4SpecialCutsForGamma();
  fSpecialCutsForElectron = new TG4SpecialCutsForElectron();
  fSpecialCutsForChargedHadron = new TG4SpecialCutsForChargedHadron();
  fSpecialCutsForNeutralHadron = new TG4SpecialCutsForNeutralHadron();
  fSpecialCutsForNeutron = new TG4SpecialCutsForNeutron();
  fSpecialCutsForMuon = new TG4SpecialCutsForMuon();

  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();

  auto aParticleIterator = GetParticleIterator();
  aParticleIterator->reset();
  while ((*aParticleIterator)()) {
    G4ParticleDefinition* particle = aParticleIterator->value();

    // skip particles which do not have process manager
    if (!particle->GetProcessManager()) continue;

    TG4G3ParticleWSP particleWSP = g3PhysicsManager->GetG3ParticleWSP(particle);
    G4String name = g3PhysicsManager->GetG3ParticleWSPName(particleWSP);

    // uncomment this to see all particles "WSP"
    // G4cout << "Iterating particle: "
    //       << particle->GetParticleName() << " " << particleWSP << " "
    //       << name << G4endl;

    if ((particleWSP != kNofParticlesWSP)) {
      // special process is created in any case

      G4ProcessManager* pManager = particle->GetProcessManager();
      switch (particleWSP) {
        case kGamma:
          pManager->AddDiscreteProcess(fSpecialCutsForGamma);
          break;
        case kElectron:
          pManager->AddDiscreteProcess(fSpecialCutsForElectron);
          break;
        case kChargedHadron:
          pManager->AddDiscreteProcess(fSpecialCutsForChargedHadron);
          break;
        case kNeutralHadron: {
          G4bool isNeutronWithKiller = false;
          if (particle == G4Neutron::Definition()) {
            G4ProcessVector* processes = pManager->GetProcessList();
            for (size_t i = 0; i < processes->size(); ++i) {
              if ((*processes)[i]->GetProcessName() == "nKiller") {
                isNeutronWithKiller = true;
                break;
              }
            }
          }
          if (isNeutronWithKiller) {
            pManager->AddDiscreteProcess(fSpecialCutsForNeutron);
          }
          else {
            pManager->AddDiscreteProcess(fSpecialCutsForNeutralHadron);
          }
          break;
        }
        case kMuon:
          pManager->AddDiscreteProcess(fSpecialCutsForMuon);
          break;
        case kEplus:
        case kAny:
        case kNofParticlesWSP:
          // nothing to be done
          break;
      }
    }
  }

  if (VerboseLevel() > 0) G4cout << "### Special Cuts constructed. " << G4endl;
}
