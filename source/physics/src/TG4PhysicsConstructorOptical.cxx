// $Id: TG4PhysicsConstructorOptical.cxx,v 1.4 2005/05/17 13:43:57 brun Exp $
// Category: physics
//
// Class TG4PhysicsConstructorOptical
// ----------------------------------
// See the class description in the header file.
// According to ExN06PhysicsList (geant4 6.0)
//
// Author: I. Hrivnacova

#include "TG4PhysicsConstructorOptical.h"
#include "TG4ProcessControlMap.h"
#include "TG4ProcessMCMap.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4Cerenkov.hh>
#include <G4Scintillation.hh>
#include <G4OpAbsorption.hh>
#include <G4OpRayleigh.hh>
#include <G4OpBoundaryProcess.hh>

const G4int  TG4PhysicsConstructorOptical::fgkDefaultMaxNumPhotonsPerStep = 300;

//_____________________________________________________________________________
TG4PhysicsConstructorOptical::TG4PhysicsConstructorOptical(const G4String& name)
  : TG4VPhysicsConstructor(name),
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
TG4PhysicsConstructorOptical::TG4PhysicsConstructorOptical(G4int verboseLevel,
                                                           const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel),
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
TG4PhysicsConstructorOptical::~TG4PhysicsConstructorOptical() {
//
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4PhysicsConstructorOptical::ConstructParticle()
{
/// Instantiate particles.

  // optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();
}

//_____________________________________________________________________________
void TG4PhysicsConstructorOptical::ConstructProcess()
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
  fCerenkovProcess->SetVerboseLevel(VerboseLevel());
  fScintillationProcess->SetVerboseLevel(VerboseLevel());
  fAbsorptionProcess->SetVerboseLevel(VerboseLevel());
  fRayleighScatteringProcess->SetVerboseLevel(VerboseLevel());
  fBoundaryProcess->SetVerboseLevel(VerboseLevel());

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
      G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
      processManager->AddDiscreteProcess(fAbsorptionProcess);
      processManager->AddDiscreteProcess(fRayleighScatteringProcess);
      processManager->AddDiscreteProcess(fBoundaryProcess);
    }
  }

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fCerenkovProcess, kCKOV); 
  controlMap->Add(fScintillationProcess, kNoG3Controls); 
  controlMap->Add(fAbsorptionProcess, kLABS); 
  controlMap->Add(fRayleighScatteringProcess, kRAYL); 
  controlMap->Add(fBoundaryProcess, kLABS); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fCerenkovProcess, kPCerenkov); 
  //mcMap->Add(fScintillationProcess, kPScintillation); 
  mcMap->Add(fScintillationProcess, kPNoProcess); 
  mcMap->Add(fAbsorptionProcess, kPLightAbsorption); 
  mcMap->Add(fRayleighScatteringProcess, kPRayleigh); 
  mcMap->Add(fBoundaryProcess, kPLightScattering); 

  if (VerboseLevel() > 0) {
    G4cout << "### " << namePhysics << " physics constructed." << G4endl;
  }  
}

//_____________________________________________________________________________
void TG4PhysicsConstructorOptical::SetMaxNumPhotonsPerStep(G4int maxNumPhotons)
{
/// Limit step to the specified maximum number of Cherenkov photons

  fMaxNumPhotonsPerStep = maxNumPhotons;

  if ( fCerenkovProcess ) 
    fCerenkovProcess->SetMaxNumPhotonsPerStep(maxNumPhotons);
}
