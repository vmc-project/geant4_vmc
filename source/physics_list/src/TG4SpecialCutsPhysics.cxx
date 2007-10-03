// $Id: TG4SpecialCutsPhysics.cxx,v 1.7 2007/05/31 10:24:33 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics_lists
// Class TG4SpecialCutsPhysics
// ----------------------------
// See the class description in the header file.

#include "TG4SpecialCutsPhysics.h"
#include "TG4G3PhysicsManager.h"
#include "TG4G3PhysicsManager.h"
#include "TG4ProcessMCMap.h"
#include "TG4SpecialCuts.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4VProcess.hh>

#include <TMCProcess.h>


//_____________________________________________________________________________
TG4SpecialCutsPhysics::TG4SpecialCutsPhysics(
                                     const G4String& name)
  : TG4VPhysicsConstructor(name),
    fSpecialCutsForGamma(0),
    fSpecialCutsForElectron(0),
    fSpecialCutsForEplus(0),
    fSpecialCutsForChargedHadron(0),
    fSpecialCutsForNeutralHadron(0),
    fSpecialCutsForMuon(0),
    fSpecialCutsForOther(0)
{
//
}

//_____________________________________________________________________________
TG4SpecialCutsPhysics::TG4SpecialCutsPhysics(
                                     G4int verboseLevel,
                                     const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel),
    fSpecialCutsForGamma(0),
    fSpecialCutsForElectron(0),
    fSpecialCutsForEplus(0),
    fSpecialCutsForChargedHadron(0),
    fSpecialCutsForNeutralHadron(0),
    fSpecialCutsForMuon(0),
    fSpecialCutsForOther(0)
{
//
}

//_____________________________________________________________________________
TG4SpecialCutsPhysics::~TG4SpecialCutsPhysics() 
{
//
  delete fSpecialCutsForGamma;
  delete fSpecialCutsForElectron;
  delete fSpecialCutsForEplus;
  delete fSpecialCutsForChargedHadron;
  delete fSpecialCutsForNeutralHadron;
  delete fSpecialCutsForMuon;
  delete fSpecialCutsForOther;
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
  fSpecialCutsForEplus = new TG4SpecialCutsForEplus();
  fSpecialCutsForChargedHadron = new TG4SpecialCutsForChargedHadron();
  fSpecialCutsForNeutralHadron = new TG4SpecialCutsForNeutralHadron();
  fSpecialCutsForMuon = new TG4SpecialCutsForMuon();
  fSpecialCutsForOther = new TG4SpecialCutsForOther();

  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();

  theParticleIterator->reset();
  while ((*theParticleIterator)())
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    TG4G3ParticleWSP particleWSP 
      = g3PhysicsManager->GetG3ParticleWSP(particle);
    G4String name =
      g3PhysicsManager->GetG3ParticleWSPName(particleWSP);
      
    // uncomment this to see all particles "WSP"
    //G4cout << "Iterating particle: " 
    //       << particle->GetParticleName() << " " << particleWSP << " "
    //       << name << G4endl;

    if ( (particleWSP !=kNofParticlesWSP) ) {
      // special process is created in any case
   
      G4ProcessManager* pManager = particle->GetProcessManager();
      switch (particleWSP) {
        case kGamma:
          pManager->AddDiscreteProcess(fSpecialCutsForGamma);
              break;
        case kElectron:
          pManager->AddDiscreteProcess(fSpecialCutsForElectron);
              break;
        case kEplus:  
          pManager->AddDiscreteProcess(fSpecialCutsForEplus);
              break;
        case kChargedHadron:  
          pManager->AddDiscreteProcess(fSpecialCutsForChargedHadron);
              break;
        case kNeutralHadron:  
          pManager->AddDiscreteProcess(fSpecialCutsForNeutralHadron);
              break;
        case kMuon:  
          pManager->AddDiscreteProcess(fSpecialCutsForMuon);
              break;
        case kAny:
          pManager->AddDiscreteProcess(fSpecialCutsForOther);
              break;
        case kNofParticlesWSP:
              // cannot happen
              break;
      }
    }    
  }

  if (VerboseLevel() > 0) 
    G4cout << "### Special Cuts constructed. " << G4endl;
}


