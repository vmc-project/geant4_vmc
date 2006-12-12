// $Id: TG4PhysicsConstructorSpecialCuts.cxx,v 1.5 2006/01/13 16:59:39 brun Exp $
// Category: physics
//
// Class TG4PhysicsConstructorSpecialCuts
// --------------------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4PhysicsConstructorSpecialCuts.h"
#include "TG4G3PhysicsManager.h"
#include "TG4G3PhysicsManager.h"
#include "TG4ProcessMCMap.h"
#include "TG4SpecialCuts.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4VProcess.hh>

#include <TMCProcess.h>


//_____________________________________________________________________________
TG4PhysicsConstructorSpecialCuts::TG4PhysicsConstructorSpecialCuts(
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
TG4PhysicsConstructorSpecialCuts::TG4PhysicsConstructorSpecialCuts(
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
TG4PhysicsConstructorSpecialCuts::~TG4PhysicsConstructorSpecialCuts() 
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
void TG4PhysicsConstructorSpecialCuts::ConstructParticle()
{
/// The particles are constructed in the physics list.
}

//_____________________________________________________________________________
void TG4PhysicsConstructorSpecialCuts::ConstructProcess()
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

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fSpecialCutsForGamma, kPStop);
  mcMap->Add(fSpecialCutsForElectron, kPStop);
  mcMap->Add(fSpecialCutsForEplus, kPStop);
  mcMap->Add(fSpecialCutsForChargedHadron, kPStop);
  mcMap->Add(fSpecialCutsForNeutralHadron, kPStop);
  mcMap->Add(fSpecialCutsForMuon, kPStop);
  mcMap->Add(fSpecialCutsForOther, kPStop);

  if (VerboseLevel() > 0) 
    G4cout << "### Special Cuts constructed. " << G4endl;
}


