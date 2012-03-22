// $Id: TG4UserParticlesPhysics.cxx 296 2007-10-18 09:36:49Z brun $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4UserParticlesPhysics.cxx
/// \brief Implementation of the TG4UserParticlesPhysics class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4UserParticlesPhysics.h"
#include "TG4UserParticle.h"
#include "TG4ParticlesManager.h"
#include "TG4StateManager.h"
#include "TG4Globals.h"
#include "TGeant4.h"

#include <G4ProcessManager.hh>

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include <G4ionIonisation.hh>

#include <G4OpAbsorption.hh>
#include <G4OpRayleigh.hh>
#include <G4OpBoundaryProcess.hh>

#include <TVirtualMCApplication.h>


//_____________________________________________________________________________
TG4UserParticlesPhysics::TG4UserParticlesPhysics(const G4String& name)
  : TG4VPhysicsConstructor(name)
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4UserParticlesPhysics::TG4UserParticlesPhysics(G4int verboseLevel,
                                     const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel)
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4UserParticlesPhysics::~TG4UserParticlesPhysics() 
{
/// Destructor
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4UserParticlesPhysics::ConstructParticle()
{
/// Add user defined particles

  TG4StateManager::Instance()->SetNewState(kAddParticles);
  TVirtualMCApplication::Instance()->AddParticles();
  TG4StateManager::Instance()->SetNewState(kPreInit);
}

//_____________________________________________________________________________
void TG4UserParticlesPhysics::ConstructProcess()
{
/// Add physics processes (ionization, multiple scattering) to
/// user defined particles according to the particle type

  TG4ParticlesManager* particlesManager = TG4ParticlesManager::Instance();
  

  for ( G4int i=0; i<particlesManager->GetNofUserParticles(); ++i ) {
  
    TG4UserParticle* userParticle = particlesManager->GetUserParticle(i);
    G4ProcessManager* processManager = userParticle->GetProcessManager();
    
    switch ( userParticle->GetMCType() ) {
    
      case kPTGamma:
        // From G4EmStandardPhysics
        processManager->AddDiscreteProcess(new G4PhotoElectricEffect);
        processManager->AddDiscreteProcess(new G4ComptonScattering);
        processManager->AddDiscreteProcess(new G4GammaConversion);
        break;

      case kPTElectron: 
        // From G4EmStandardPhysics
        processManager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
        processManager->AddProcess(new G4eIonisation,        -1, 2, 2);
        processManager->AddProcess(new G4eBremsstrahlung(),  -1, 3, 3);
        break;
       
      case kPTNeutron:
        TG4Globals::Warning(
          "TG4UserParticlesPhysics", "ConstructProcess",
          "No hadronic processes are built for user defined particles."); 
         break;

      case kPTHadron:
        // From G4EmStandardPhysics
        processManager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
        processManager->AddProcess(new G4hIonisation,         -1, 2, 2);
        TG4Globals::Warning(
          "TG4UserParticlesPhysics", "ConstructProcess",
          "No hadronic processes are built for user defined particles."); 
        break;
          
      case kPTMuon: 
        // From G4EmStandardPhysics
        processManager->AddProcess(new G4hMultipleScattering,-1, 1, 1);
        processManager->AddProcess(new G4MuIonisation,       -1, 2, 2);
        processManager->AddProcess(new G4MuBremsstrahlung,   -1, 3, 3);
        processManager->AddProcess(new G4MuPairProduction,   -1, 4, 4);
        break;
       
      case kPTGeantino: 
        break;      
     
      case kPTChargedGeantino: 
        break; 
      
      case kPTOpticalPhoton:
        // From TG4OpticalPhysics
        processManager->AddDiscreteProcess(new G4OpAbsorption());
        processManager->AddDiscreteProcess(new G4OpRayleigh());
        processManager->AddDiscreteProcess(new G4OpBoundaryProcess());
        break; 
   
      case kPTIon:
        // From G4EmStandardPhysics
        processManager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
        processManager->AddProcess(new G4ionIonisation,       -1, 2, 2);
        TG4Globals::Warning(
          "TG4UserParticlesPhysics", "ConstructProcess",
          "No hadronic processes are built for user defined particles."); 
        break;

      case kPTUndefined: 
        break;
    }   
  }                    

  if (VerboseLevel() > 0) 
    G4cout << "### User particles physics constructed. " << G4endl;
}
