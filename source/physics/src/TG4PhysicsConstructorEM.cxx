// $Id: TG4PhysicsConstructorEM.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: physics
//
// Author: I. Hrivnacova
//
// Class TG4PhysicsConstructorEM
// -----------------------------
// See the class description in the header file.
// According to ExN04EMPhysics.cc,v 1.1.2.1 2001/06/28 19:07:37 gunter Exp 
// GEANT4 tag Name: geant4-06-00

#include "TG4PhysicsConstructorEM.h"
#include "TG4ProcessControlMap.h"
#include "TG4ProcessMCMap.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4Positron.hh>
#include <G4NeutrinoE.hh>
#include <G4AntiNeutrinoE.hh>

//_____________________________________________________________________________
TG4PhysicsConstructorEM::TG4PhysicsConstructorEM(const G4String& name)
  : TG4VPhysicsConstructor(name) {
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorEM::TG4PhysicsConstructorEM(G4int verboseLevel,
                                                 const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel) {
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorEM::~TG4PhysicsConstructorEM() {
//
}

// private methods

//_____________________________________________________________________________
void TG4PhysicsConstructorEM::ConstructProcessForGamma()
{
// Constructs electromagnetic processes for gamma.
// ---
  
  // add processes
  G4ProcessManager* pManager = G4Gamma::Gamma()->GetProcessManager();
  pManager->AddDiscreteProcess(&fPhotoEffect);
  pManager->AddDiscreteProcess(&fComptonEffect);
  pManager->AddDiscreteProcess(&fPairProduction);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fPhotoEffect, kPHOT); 
  controlMap->Add(&fComptonEffect, kCOMP); 
  controlMap->Add(&fPairProduction, kPAIR); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fPhotoEffect, kPPhotoelectric); 
  mcMap->Add(&fComptonEffect, kPCompton); 
  mcMap->Add(&fPairProduction, kPPair); 
}  


//_____________________________________________________________________________
void TG4PhysicsConstructorEM::ConstructProcessForElectron()
{
// Constructs electromagnetic processes for e-.
// ---

  // add process
  G4ProcessManager* pManager = G4Electron::Electron()->GetProcessManager();
  pManager->AddProcess(&fElectronMultipleScattering, -1, 1, 1);
  pManager->AddProcess(&fElectronIonisation,         -1, 2, 2);
  pManager->AddProcess(&fElectronBremsStrahlung,     -1, 3, 3);  

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fElectronMultipleScattering, kMULS); 
  controlMap->Add(&fElectronIonisation, kLOSS); 
  controlMap->Add(&fElectronBremsStrahlung, kBREM); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fElectronMultipleScattering, kPMultipleScattering); 
  mcMap->Add(&fElectronIonisation, kPEnergyLoss); 
  mcMap->Add(&fElectronBremsStrahlung, kPBrem); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorEM::ConstructProcessForPositron()
{
// Constructs electromagnetic processes for e+.
// ---
  
  // add processes
  G4ProcessManager * pManager = G4Positron::Positron()->GetProcessManager();
  pManager->AddProcess(&fPositronMultipleScattering, -1, 1, 1);
  pManager->AddProcess(&fPositronIonisation,         -1, 2, 2);
  pManager->AddProcess(&fPositronBremsStrahlung,     -1, 3, 3);  
  pManager->AddProcess(&fAnnihilation,                0,-1, 4);  

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fPositronMultipleScattering, kMULS); 
  controlMap->Add(&fPositronIonisation, kLOSS); 
  controlMap->Add(&fPositronBremsStrahlung, kBREM); 
  controlMap->Add(&fAnnihilation, kANNI); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fPositronMultipleScattering, kPMultipleScattering); 
  mcMap->Add(&fPositronIonisation, kPEnergyLoss); 
  mcMap->Add(&fPositronBremsStrahlung, kPBrem); 
  mcMap->Add(&fAnnihilation, kPAnnihilation); 
}

// protected methods

//_____________________________________________________________________________
void TG4PhysicsConstructorEM::ConstructParticle()
{
// Instantiates particles.
// ---

  // gamma
  G4Gamma::GammaDefinition();
 
  // electron
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
}

//_____________________________________________________________________________
void TG4PhysicsConstructorEM::ConstructProcess()
{
// Constructs electromagnetic processes for e+.
// ---

  ConstructProcessForGamma();
  ConstructProcessForElectron();
  ConstructProcessForPositron();

  if (VerboseLevel() > 0) {
    G4cout << "### Electromagnetic physics constructed." << G4endl;
  }  
}
