//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ComposedPhysicsList.cxx
/// \brief Implementation of the TG4ComposedPhysicsList class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ComposedPhysicsList.h"
#include "TG4PhysicsManager.h"
#include "TG4G3PhysicsManager.h"

#include <G4VUserPhysicsList.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessTable.hh>
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4Positron.hh>
#include <G4Proton.hh>
#include <G4RegionStore.hh>
#include <G4SystemOfUnits.hh>
#include <G4GammaConversionToMuons.hh>

//_____________________________________________________________________________
TG4ComposedPhysicsList::TG4ComposedPhysicsList()
  : G4VUserPhysicsList(),
    TG4Verbose("composedPhysicsList"),
    fMessenger(this),
    fPhysicsLists(),
    fIsProductionCutsTableEnergyRange(false),
    fProductionCutsTableEnergyMin(0.),
    fProductionCutsTableEnergyMax(0.),
    fGammaToMuonsCrossSectionFactor(-1.)
{
/// Default constructor

  if ( VerboseLevel() >1 )
    G4cout << "TG4ComposedPhysicsList::TG4ComposedPhysicsList" << G4endl;
  
  SetVerboseLevel(TG4VVerbose::VerboseLevel());
}

//_____________________________________________________________________________
TG4ComposedPhysicsList::~TG4ComposedPhysicsList() 
{
/// Destructor

  for ( G4int i=0; i<G4int(fPhysicsLists.size()); i++ )
    delete fPhysicsLists[i];
}

//
// private methods
//

//_____________________________________________________________________________
void TG4ComposedPhysicsList::ApplyGammaToMuonsCrossSectionFactor()
{
/// Apply gamma to muons cross section factor if defined by user
/// if applicaple

  if ( fGammaToMuonsCrossSectionFactor > 0. ) {
    G4ParticleDefinition* gamma = G4Gamma::Gamma();
    G4ProcessManager* processManager = gamma->GetProcessManager();
    G4ProcessVector* processVector = processManager->GetProcessList();
    G4bool done = false;
    // get G4GammaConversionToMuons
    for ( G4int i=0; i< processVector->length(); i++ ) {
      G4GammaConversionToMuons* gammaToMuMu
        = dynamic_cast<G4GammaConversionToMuons*>((*processVector)[i]);
      if ( gammaToMuMu ) {
        if ( VerboseLevel() > 0 ) {
          G4cout << "### Set gamma to muons cross section factor user value: "
                 << fGammaToMuonsCrossSectionFactor
                 << G4endl;
        }
        gammaToMuMu->SetCrossSecFactor(fGammaToMuonsCrossSectionFactor);
        done = true;
        break;
      }
    }
    if ( ! done ) {
      TG4Globals::Warning(
        "TG4ComposedPhysicsList", "ApplyGammaToMuonsCrossSectionFactor",
        "Cannot set gamma to muons cross section factor, GammaToMuons must be activated first.");
    }
  }
}

//
// public methods
//

//_____________________________________________________________________________
void TG4ComposedPhysicsList::AddPhysicsList(G4VUserPhysicsList* physicsList)
{
/// Add physics list in the list

  fPhysicsLists.push_back(physicsList);
}  

//_____________________________________________________________________________
void TG4ComposedPhysicsList::ConstructParticle()
{
/// Construct all particles.

  if ( VerboseLevel() >1 )
    G4cout << "TG4ComposedPhysicsList::ConstructParticle" << G4endl;

  for (G4int i=0; i<G4int(fPhysicsLists.size()); i++ )
    fPhysicsLists[i]->ConstructParticle();

  if ( VerboseLevel() >1 )
    G4cout << "TG4ComposedPhysicsList::ConstructParticle done" << G4endl;
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::ConstructProcess()
{
/// Construct all processes.

  if ( VerboseLevel() >1 )
    G4cout << "TG4ComposedPhysicsList::ConstructProcess" << G4endl;

  // lock physics manager
  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();
  g3PhysicsManager->Lock();  

  for (G4int i=0; i<G4int(fPhysicsLists.size()); i++ ) {
    fPhysicsLists[i]->ConstructProcess();
  }

  if ( fGammaToMuonsCrossSectionFactor > 0. ) {
    ApplyGammaToMuonsCrossSectionFactor();
  }

  if ( VerboseLevel() >1 )
    G4cout << "TG4ComposedPhysicsList::ConstructProcess done" << G4endl;
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCuts()
{
/// Set cut values

  if ( VerboseLevel() >1 )
    G4cout << "TG4ComposedPhysicsList::SetCuts" << G4endl;

  if ( fIsProductionCutsTableEnergyRange ) {
    G4ProductionCutsTable::GetProductionCutsTable()
      ->SetEnergyRange(fProductionCutsTableEnergyMin, fProductionCutsTableEnergyMax);
  }

  // Get default range cut values from physics manager
  G4double rangeCutGam = TG4PhysicsManager::Instance()->GetCutForGamma();
  G4double rangeCutEle = TG4PhysicsManager::Instance()->GetCutForElectron();
  G4double rangeCutPos = TG4PhysicsManager::Instance()->GetCutForPositron();
  G4double rangeCutPro = TG4PhysicsManager::Instance()->GetCutForProton();

  // Set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(rangeCutGam, "gamma");
  SetCutValue(rangeCutEle, "e-");
  SetCutValue(rangeCutPos, "e+");
  SetCutValue(rangeCutPro, "proton");

  // Set the default production cuts to all already created regions.
  // The cuts can be then further customized via VMC cuts or
  // via Geant4 UI commands called after G4RunManager initialization
  // (in VMC examples in g4config2.in macros)

  // Loop over regions
  G4RegionStore* regionStore = G4RegionStore::GetInstance();
  for (G4int i=0; i<G4int(regionStore->size()); i++) {
    G4Region* region = (*regionStore)[i];

    // skip regions which already have production cuts defined
    if ( region->GetProductionCuts() ) continue;

    G4ProductionCuts* cuts = new G4ProductionCuts();
    cuts->SetProductionCut(rangeCutGam, 0);
    cuts->SetProductionCut(rangeCutEle, 1);
    cuts->SetProductionCut(rangeCutPos, 2);
    cuts->SetProductionCut(rangeCutPro, 3);
    region->SetProductionCuts(cuts);
  }

  if ( VerboseLevel() > 0 ) DumpCutValuesTable();
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCutForGamma(G4double cut)
{
/// Set cut value for gamma

  SetParticleCuts(cut, G4Gamma::Gamma());
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCutForElectron(G4double cut)
{
/// Set cut value for electron

  SetParticleCuts(cut, G4Electron::Electron());
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCutForPositron(G4double cut)
{
/// Set cut value for positron

  SetParticleCuts(cut, G4Positron::Positron());
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCutForProton(G4double cut)
{
/// Set cut value for positron

  SetParticleCuts(cut, G4Proton::Proton());
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetProductionCutsTableEnergyRange(G4double min, G4double max)
{
/// Set the production cuts table energy range

  fProductionCutsTableEnergyMin = min;
  fProductionCutsTableEnergyMax = max;
  fIsProductionCutsTableEnergyRange = true;
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::PrintAllProcesses() const
{
/// Print all processes.

  G4cout << "Instantiated processes: " << G4endl;
  G4cout << "======================= " << G4endl;
 
  G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();
  G4ProcessTable::G4ProcNameVector* processNameList 
    = processTable->GetNameList();

  for (G4int i=0; i<G4int(processNameList->size()); i++){
    G4cout << "   " << (*processNameList)[i] << G4endl;
  }  
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::DumpAllProcesses() const
{
/// Dump all particles and their processes.

  G4cout << "Instantiated particles and processes: " << G4endl;
  G4cout << "===================================== " << G4endl;
 
  auto theParticleIterator=GetParticleIterator();
  theParticleIterator->reset();
  while ((*theParticleIterator)())
  {
    // print particle name
    G4cout << "Particle: " 
           << theParticleIterator->value()->GetParticleName()
           << G4endl;

    // dump particle processes
    G4ProcessVector* processVector 
      = theParticleIterator->value()->GetProcessManager()->GetProcessList();
    for (G4int i=0; i<processVector->length(); i++)
      (*processVector)[i]->DumpInfo();
      
    G4cout << G4endl;  
  }  
}


//_____________________________________________________________________________
G4int TG4ComposedPhysicsList::VerboseLevel() const 
{
/// Return verbose level (via TG4VVerbose)

  return TG4VVerbose::VerboseLevel();
}


//_____________________________________________________________________________
void TG4ComposedPhysicsList::VerboseLevel(G4int level) 
{
/// Set the specified level to both TG4Verbose and 
/// G4VUserPhysicsList.
/// The verbose level is also propagated to registered physics lists.

  TG4VVerbose::VerboseLevel(level);
  SetVerboseLevel(level);
  
  for (G4int i=0; i<G4int(fPhysicsLists.size()); i++ ) {
    TG4Verbose* verbose = dynamic_cast<TG4Verbose*>(fPhysicsLists[i]);
    if ( verbose ) 
      verbose->VerboseLevel(level);
    else  
      fPhysicsLists[i]->SetVerboseLevel(level);
  }  
}
