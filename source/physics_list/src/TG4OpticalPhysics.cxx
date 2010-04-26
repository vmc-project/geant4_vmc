// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4OpticalPhysics.cxx
/// \brief Implementation of the TG4OpticalPhysics class 
///
/// According to ExN06PhysicsList (geant4 6.0)
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4OpticalPhysics.h"
#include "TG4Globals.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4Cerenkov.hh>
#include <G4Scintillation.hh>
#include <G4OpAbsorption.hh>
#include <G4OpRayleigh.hh>
#include <G4OpBoundaryProcess.hh>

const G4int    TG4OpticalPhysics::fgkDefaultVerboseLevel = 0;
const G4int    TG4OpticalPhysics::fgkDefaultMaxNumPhotons = 20;
const G4double TG4OpticalPhysics::fgkDefaultMaxBetaChange = 1.0;    
const G4double TG4OpticalPhysics::fgkDefaultYieldFactor = 1.0;
const G4OpticalSurfaceModel TG4OpticalPhysics::fgkDefaultSurfaceModel = unified;

//_____________________________________________________________________________
TG4OpticalPhysics::TG4OpticalPhysics(const G4String& name)
  : TG4VPhysicsConstructor(name),
    fMessenger(this),
    fProcesses(),
    fProcessActivation(),
    fProcessVerbose(),
    fMaxNumPhotons(fgkDefaultMaxNumPhotons),
    fMaxBetaChange(fgkDefaultMaxBetaChange),
    fYieldFactor(fgkDefaultYieldFactor),
    fSurfaceModel(fgkDefaultSurfaceModel),
    fTrackSecondariesFirst(false)
{
/// Standard constructor

  for ( G4int i=0; i<kNoProcess; i++ ) {
    fProcesses.push_back(0);
    fProcessActivation.push_back(true);
    fProcessVerbose.push_back(fgkDefaultVerboseLevel);
  }  
}

//_____________________________________________________________________________
TG4OpticalPhysics::TG4OpticalPhysics(G4int verboseLevel,
                                     const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel),
    fMessenger(this),
    fProcesses(),
    fProcessActivation(),
    fProcessVerbose(),
    fMaxNumPhotons(fgkDefaultMaxNumPhotons), 
    fMaxBetaChange(fgkDefaultMaxBetaChange),
    fYieldFactor(fgkDefaultYieldFactor),
    fSurfaceModel(fgkDefaultSurfaceModel),
    fTrackSecondariesFirst(false)
{
/// Standard constructor

  for ( G4int i=0; i<kNoProcess; i++ ) {
    fProcesses.push_back(0);
    fProcessActivation.push_back(true);
    fProcessVerbose.push_back(fgkDefaultVerboseLevel);
  }  
}

//_____________________________________________________________________________
TG4OpticalPhysics::~TG4OpticalPhysics() 
{
/// Destructor
}

//
// private methods
//

//_____________________________________________________________________________
void TG4OpticalPhysics::PrintStatistics() const
{
/// Print all processes activation and their parameters

  for ( G4int i=0; i<kNoProcess; i++ ) {
    G4cout << "  " << TG4OpticalProcessName(i) << " process:  ";
    if ( ! fProcessActivation[i] ) {
      G4cout << "inactive" << G4endl;
    }  
    else {
      G4cout << "active" << G4endl;
      if ( i == kCerenkov ) {
        G4cout << "    Max number of photons per step: " << fMaxNumPhotons << G4endl;
        G4cout << "    Max beta change per step:       " << fMaxBetaChange << G4endl;
      }  
      if ( i == kScintillation ) {
        G4cout << "    Yield factor: "  << fYieldFactor << G4endl;
      }  
      if ( i == kBoundary ) {
        G4cout << "    OpticalSurfaceModel:  ";
        if ( fSurfaceModel == glisur )  G4cout << "glisur" << G4endl;
        if ( fSurfaceModel == unified ) G4cout << "unified" << G4endl;
      }
    }    
  }  
  if ( fTrackSecondariesFirst ) {
    G4cout << "  Track secondaries first:  activated" << G4endl;
  } 
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

  fProcesses[kCerenkov]      = new G4Cerenkov("Cerenkov");
  fProcesses[kScintillation] = new G4Scintillation("Scintillation");
  fProcesses[kAbsorption]    = new G4OpAbsorption();
  fProcesses[kRayleigh]      = new G4OpRayleigh();
  fProcesses[kBoundary]      = new G4OpBoundaryProcess();

  //fProcesses[kCerenkov]->DumpPhysicsTable();
  //fAbsorptionProcess->DumpPhysicsTable();
  //fRayleighScatteringProcess->DumpPhysicsTable();

  // Add verbose 
  for ( G4int i=0; i<kNoProcess; i++ ) {
    fProcesses[i]->SetVerboseLevel(fProcessVerbose[i]);
  }  

  G4Cerenkov* cerenkov 
    = static_cast<G4Cerenkov*>(fProcesses[kCerenkov]);
  cerenkov->SetMaxNumPhotonsPerStep(fMaxNumPhotons);
  cerenkov->SetMaxBetaChangePerStep(fMaxBetaChange);
  cerenkov->SetTrackSecondariesFirst(fTrackSecondariesFirst);

  G4Scintillation* scintillation 
    = static_cast<G4Scintillation*>(fProcesses[kScintillation]);
  scintillation->SetScintillationYieldFactor(fYieldFactor);
  scintillation->SetTrackSecondariesFirst(fTrackSecondariesFirst);

  G4OpticalSurfaceModel themodel = fSurfaceModel;   
  // model from GEANT3
  // G4OpticalSurfaceModel themodel = glisur;
  static_cast<G4OpBoundaryProcess*>(fProcesses[kBoundary])->SetModel(themodel);

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    
    // skip geantinos
    if ( particle->GetParticleName() == "geantino" ||
         particle->GetParticleName() == "chargedgeantino" ) continue;
    
    G4ProcessManager* processManager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    if ( fProcesses[kCerenkov]->IsApplicable(*particle) &&
         fProcessActivation[kCerenkov] ) {
      processManager->AddProcess(fProcesses[kCerenkov]);
      processManager->SetProcessOrdering(fProcesses[kCerenkov],idxPostStep);
    }
    if ( fProcesses[kScintillation]->IsApplicable(*particle) &&
         fProcessActivation[kScintillation] ) {
      processManager->AddProcess(fProcesses[kScintillation]);
      processManager->SetProcessOrderingToLast(fProcesses[kScintillation], idxAtRest);
      processManager->SetProcessOrderingToLast(fProcesses[kScintillation], idxPostStep);
    }
    if ( particleName == "opticalphoton" ) {
      for ( G4int i=kAbsorption; i<=kBoundary; i++ ) {  
        // Absorption, Raileigh, Boundary    
        if ( fProcessActivation[i] )
          processManager->AddDiscreteProcess(fProcesses[i]);
      }  
    }
  }  

  if ( VerboseLevel() > 1 ) PrintStatistics(); 

  if ( VerboseLevel() > 0 ) {
    G4cout << "### " << namePhysics << " physics constructed." << G4endl;
  }
  
}

//_____________________________________________________________________________
void TG4OpticalPhysics::SetProcessActivation(TG4OpticalProcess process, 
                                             G4bool isActive)
{
/// Activate/inactivate selected process.
/// - If this method is called in PreInit> phase (before execution of ConstructProcess),
/// the process is not added to particles process manager
/// and so it cannot be re-activated later in Idle> phase.
/// - If this method is called in Idle> phase (after execution of ConstructProcess),
/// the existing process is activated or inactivated for all particles.
/// (It is equivalent to geant4 commands /process/(in)activate.) 
  
  if ( process == kNoProcess ) {
    TG4Globals::Warning("TG4OpticalPhysics", "SetProcessActivation",
      "No process is selected");
    return;
  }    

  if ( fProcessActivation[process] == isActive ) return;
  
  fProcessActivation[process] = isActive;
  
  if ( fProcesses[process] ) {
    theParticleIterator->reset();
    while( (*theParticleIterator)() ){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* processManager = particle->GetProcessManager(); 
      G4ProcessVector* processVector = processManager->GetProcessList();
    
      // activate or inactivate processes according to 
      for (G4int i=0; i<processVector->length(); i++) {
        if ( (*processVector)[i] != fProcesses[process] ) continue;
        G4cout << "Setting process activation: "
               << fProcesses[process]->GetProcessName() << "  "
               << isActive << G4endl;
        processManager->SetProcessActivation(fProcesses[process], isActive);
      }
    }    
  }
}  

//_____________________________________________________________________________
void TG4OpticalPhysics::SetProcessVerbose(TG4OpticalProcess process, 
                                          G4int verboseLevel)
{
/// Set new verbose level to a selected process

  if ( process == kNoProcess ) {
    TG4Globals::Warning("TG4OpticalPhysics", "SetProcessVerbose",
      "No process is selected");
    return;
  }    

  if ( fProcessVerbose[process] == verboseLevel ) return;
  
  fProcessVerbose[process] = verboseLevel;
  
  if ( fProcesses[process] ) {
    fProcesses[process]->SetVerboseLevel(verboseLevel);
  }
}  


//_____________________________________________________________________________
void TG4OpticalPhysics::SetMaxNumPhotonsPerStep(G4int maxNumPhotons)
{
/// Limit step to the specified maximum number of Cherenkov photons

  fMaxNumPhotons = maxNumPhotons;

  if ( fProcesses[kCerenkov] ) {
    static_cast<G4Cerenkov*>(fProcesses[kCerenkov])
      ->SetMaxNumPhotonsPerStep(maxNumPhotons);
  }  
}

//_____________________________________________________________________________
void TG4OpticalPhysics::SetMaxBetaChangePerStep(G4double maxBetaChange)
{
/// Limit step to the specified maximum change of beta of the parent particle

  fMaxBetaChange = maxBetaChange;

  if ( fProcesses[kCerenkov] ) {
    static_cast<G4Cerenkov*>(fProcesses[kCerenkov])
      ->SetMaxBetaChangePerStep(maxBetaChange);
  }  
}

//_____________________________________________________________________________
void TG4OpticalPhysics::SetScintillationYieldFactor(G4double yieldFactor)
{
/// Set the scintillation yield factor

  fYieldFactor = yieldFactor;

  if ( fProcesses[kScintillation] ) {
    static_cast<G4Scintillation*>(fProcesses[kScintillation])
      ->SetScintillationYieldFactor(yieldFactor);
  } 
}   

//_____________________________________________________________________________
void TG4OpticalPhysics::SetOpticalSurfaceModel(G4OpticalSurfaceModel model)
{
/// Set optical surface model (glisur or unified)

  fSurfaceModel = model;

  if ( fProcesses[kBoundary] ) {
    static_cast<G4OpBoundaryProcess*>(fProcesses[kBoundary])->SetModel(model);
  }  
}

//_____________________________________________________________________________
void TG4OpticalPhysics::SetTrackSecondariesFirst(G4bool trackSecondariesFirst)
{
/// Limit step to the specified maximum change of beta of the parent particle

  fTrackSecondariesFirst = trackSecondariesFirst;

  if ( fProcesses[kCerenkov] ) {
    static_cast<G4Cerenkov*>(fProcesses[kCerenkov])
      ->SetTrackSecondariesFirst(trackSecondariesFirst);
  }  

  if ( fProcesses[kScintillation] ) {
    static_cast<G4Scintillation*>(fProcesses[kScintillation])
      ->SetTrackSecondariesFirst(trackSecondariesFirst);
  }  
}
