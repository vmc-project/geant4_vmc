// $Id: TG4PhysicsConstructorMuon.cxx,v 1.2 2003/12/18 13:27:46 brun Exp $
// Category: physics
//
// Class TG4PhysicsConstructorMuon
// -----------------------------
// See the class description in the header file.
// According to ExN04MuonPhysics.cc,v 1.2.2.1 2001/06/28 19:07:37 gunter Exp 
// GEANT4 tag Name: geant4-06-00
//
// Author: I. Hrivnacova

#include "TG4PhysicsConstructorMuon.h"
#include "TG4ProcessControlMap.h"
#include "TG4ProcessMCMap.h"
#include "TG4G3Control.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4MuonPlus.hh>
#include <G4MuonMinus.hh>
#include <G4TauMinus.hh>
#include <G4TauPlus.hh>
#include <G4NeutrinoTau.hh>
#include <G4AntiNeutrinoTau.hh>
#include <G4NeutrinoMu.hh>
#include <G4AntiNeutrinoMu.hh>

//TBR
#include <G4MuIonisation.hh>
#include <G4MuBremsstrahlung.hh>
#include <G4MuPairProduction.hh>
#include <G4hIonisation.hh>

//_____________________________________________________________________________
TG4PhysicsConstructorMuon::TG4PhysicsConstructorMuon(const G4String& name)
  : TG4VPhysicsConstructor(name) {
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorMuon::TG4PhysicsConstructorMuon(G4int verboseLevel,
                                                     const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel) {
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorMuon::~TG4PhysicsConstructorMuon() {
//
}

//
// private methods
//

//_____________________________________________________________________________
void TG4PhysicsConstructorMuon::ConstructProcessForMuonPlus()
{
/// Constructs electromagnetic processes for mu+.
  
  // add processes
  G4ProcessManager* pManager = G4MuonPlus::MuonPlus()->GetProcessManager();
  pManager->AddProcess(&fMuPlusMultipleScattering,-1,  1, 1);
  pManager->AddProcess(&fMuPlusIonisation,        -1,  2, 2);
  pManager->AddProcess(&fMuPlusBremsstrahlung,    -1,  3, 3);
  pManager->AddProcess(&fMuPlusPairProduction,    -1,  4, 4);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fMuPlusMultipleScattering, kMULS); 
  controlMap->Add(&fMuPlusIonisation, kLOSS); 
  controlMap->Add(&fMuPlusBremsstrahlung, kBREM); 
  controlMap->Add(&fMuPlusPairProduction, kPAIR); 


  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fMuPlusMultipleScattering, kPMultipleScattering); 
  mcMap->Add(&fMuPlusIonisation, kPEnergyLoss); 
  mcMap->Add(&fMuPlusBremsstrahlung, kPBrem); 
  mcMap->Add(&fMuPlusPairProduction, kPPair); 
}  

//_____________________________________________________________________________
void TG4PhysicsConstructorMuon::ConstructProcessForMuonMinus()
{
/// Constructs electromagnetic processes for mu-.
  
  // add processes & set ordering
  G4ProcessManager* pManager = G4MuonMinus::MuonMinus()->GetProcessManager();
  pManager->AddProcess(&fMuMinusMultipleScattering,-1,  1, 1);
  pManager->AddProcess(&fMuMinusIonisation,        -1,  2, 2);
  pManager->AddProcess(&fMuMinusBremsstrahlung,    -1,  3, 3);
  pManager->AddProcess(&fMuMinusPairProduction,    -1,  4, 4);

  pManager->AddRestProcess(&fMuMinusCaptureAtRest);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fMuMinusMultipleScattering, kMULS); 
  controlMap->Add(&fMuMinusIonisation, kLOSS); 
  controlMap->Add(&fMuMinusBremsstrahlung, kBREM); 
  controlMap->Add(&fMuMinusPairProduction, kPAIR); 
  controlMap->Add(&fMuMinusCaptureAtRest, kMUNU); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fMuMinusMultipleScattering, kPMultipleScattering); 
  mcMap->Add(&fMuMinusIonisation, kPEnergyLoss); 
  mcMap->Add(&fMuMinusBremsstrahlung, kPBrem); 
  mcMap->Add(&fMuMinusPairProduction, kPPair); 
  mcMap->Add(&fMuMinusCaptureAtRest, kPMuonNuclear); 
}  

//_____________________________________________________________________________
void TG4PhysicsConstructorMuon::ConstructProcessForTauPlus()
{
/// Constructs electromagnetic processes for tau+.
  
  // add processes
  G4ProcessManager* pManager = G4TauPlus::TauPlus()->GetProcessManager();
  pManager->AddProcess(&fTauPlusMultipleScattering, -1, 1, 1);
  pManager->AddProcess(&fTauPlusIonisation,         -1, 2, 2);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fTauPlusMultipleScattering, kMULS); 
  controlMap->Add(&fTauPlusIonisation, kLOSS); 


  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fTauPlusMultipleScattering, kPMultipleScattering); 
  mcMap->Add(&fTauPlusIonisation, kPEnergyLoss); 
}  

//_____________________________________________________________________________
void TG4PhysicsConstructorMuon::ConstructProcessForTauMinus()
{
/// Constructs electromagnetic processes for tau-.
  
  // add processes
  G4ProcessManager* pManager = G4TauMinus::TauMinus()->GetProcessManager();
  pManager->AddProcess(&fTauMinusMultipleScattering, -1, 1, 1);
  pManager->AddProcess(&fTauMinusIonisation,         -1, 2, 2);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fTauMinusMultipleScattering, kMULS); 
  controlMap->Add(&fTauMinusIonisation, kLOSS); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fTauMinusMultipleScattering, kPMultipleScattering); 
  mcMap->Add(&fTauMinusIonisation, kPEnergyLoss); 
}  

//
// protected methods
//

//_____________________________________________________________________________
void TG4PhysicsConstructorMuon::ConstructParticle()
{
/// Instantiate particles.

  // Mu
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();

  // Tau
  G4TauMinus::TauMinusDefinition();
  G4TauPlus::TauPlusDefinition();
  G4NeutrinoTau::NeutrinoTauDefinition();
  G4AntiNeutrinoTau::AntiNeutrinoTauDefinition();
}

//_____________________________________________________________________________
void TG4PhysicsConstructorMuon::ConstructProcess()
{
/// Construct electromagnetic processes for muons.

  ConstructProcessForMuonPlus();
  ConstructProcessForMuonMinus();
  ConstructProcessForTauPlus();
  ConstructProcessForTauMinus();

  if (VerboseLevel() >0 ) {
    G4cout << "### Muon physics constructed." << G4endl;
  }  
}
