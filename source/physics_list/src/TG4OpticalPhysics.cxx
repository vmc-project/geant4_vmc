// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics_lists
// Class TG4OpticalPhysics
// ------------------------
// See the class description in the header file.
// According to ExN06PhysicsList (geant4 6.0)

#include "TG4OpticalPhysics.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4Cerenkov.hh>
#include <G4Scintillation.hh>
#include <G4OpAbsorption.hh>
#include <G4OpRayleigh.hh>
#include <G4OpBoundaryProcess.hh>

const G4int  TG4OpticalPhysics::fgkDefaultMaxNumPhotonsPerStep = 300;

//_____________________________________________________________________________
TG4OpticalPhysics::TG4OpticalPhysics(const G4String& name)
  : TG4VPhysicsConstructor(name),
    fMessenger(this),
    fCerenkovProcess(0),
    fScintillationProcess(0),
    fAbsorptionProcess(0),
    fRayleighScatteringProcess(0),
    fBoundaryProcess(0),
    fMaxNumPhotonsPerStep(fgkDefaultMaxNumPhotonsPerStep) 
{
//
}

//_____________________________________________________________________________
TG4OpticalPhysics::TG4OpticalPhysics(G4int verboseLevel,
                                     const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel),
    fMessenger(this),
    fCerenkovProcess(0),
    fScintillationProcess(0),
    fAbsorptionProcess(0),
    fRayleighScatteringProcess(0),
    fBoundaryProcess(0), 
    fMaxNumPhotonsPerStep(fgkDefaultMaxNumPhotonsPerStep) 
{
//
}

//_____________________________________________________________________________
TG4OpticalPhysics::~TG4OpticalPhysics() {
//
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4OpticalPhysics::ConstructParticle()
{
/// Instantiate particles.

  // optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();
}

//_____________________________________________________________________________
void TG4OpticalPhysics::ConstructProcess()
{
/// Construct optical processes.
/// According to ExN06PhysicsList.cc.
/// (geant4 1.1)

  fCerenkovProcess = new G4Cerenkov("Cerenkov");
  fScintillationProcess = new G4Scintillation("Scintillation");
  fAbsorptionProcess = new G4OpAbsorption();
  fRayleighScatteringProcess = new G4OpRayleigh();
  fBoundaryProcess = new G4OpBoundaryProcess();

  //fCerenkovProcess->DumpPhysicsTable();
  //fAbsorptionProcess->DumpPhysicsTable();
  //fRayleighScatteringProcess->DumpPhysicsTable();

  // add verbose 
  fCerenkovProcess->SetVerboseLevel(VerboseLevel()-1);
  fScintillationProcess->SetVerboseLevel(VerboseLevel()-1);
  fAbsorptionProcess->SetVerboseLevel(VerboseLevel()-1);
  fRayleighScatteringProcess->SetVerboseLevel(VerboseLevel()-1);
  fBoundaryProcess->SetVerboseLevel(VerboseLevel()-1);

  fCerenkovProcess->SetMaxNumPhotonsPerStep(fMaxNumPhotonsPerStep);
  fCerenkovProcess->SetTrackSecondariesFirst(true);

  fScintillationProcess->SetScintillationYieldFactor(1.);
  fScintillationProcess->SetTrackSecondariesFirst(true);

  G4OpticalSurfaceModel themodel = unified;   
  // model from GEANT3
  // G4OpticalSurfaceModel themodel = glisur;
  fBoundaryProcess->SetModel(themodel);

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    
    // skip geantinos
    if ( particle->GetParticleName() == "geantino" ||
         particle->GetParticleName() == "chargedgeantino" ) continue;
    
    G4ProcessManager* processManager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    if (fCerenkovProcess->IsApplicable(*particle)) {
      processManager->AddContinuousProcess(fCerenkovProcess);
    }
    if (fScintillationProcess->IsApplicable(*particle)) {
      processManager->AddProcess(fScintillationProcess);
      processManager->SetProcessOrderingToLast(fScintillationProcess, idxAtRest);
      processManager->SetProcessOrderingToLast(fScintillationProcess, idxPostStep);
    }
    if (particleName == "opticalphoton") {
      processManager->AddDiscreteProcess(fAbsorptionProcess);
      processManager->AddDiscreteProcess(fRayleighScatteringProcess);
      processManager->AddDiscreteProcess(fBoundaryProcess);
    }
  }

  if (VerboseLevel() > 0) {
    G4cout << "### " << namePhysics << " physics constructed." << G4endl;
  }  
}

//_____________________________________________________________________________
void TG4OpticalPhysics::SetMaxNumPhotonsPerStep(G4int maxNumPhotons)
{
/// Limit step to the specified maximum number of Cherenkov photons

  fMaxNumPhotonsPerStep = maxNumPhotons;

  if ( fCerenkovProcess ) 
    fCerenkovProcess->SetMaxNumPhotonsPerStep(maxNumPhotons);
}
