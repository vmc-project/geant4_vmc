// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4ComposedPhysicsList.cxx
/// \brief Implementation of the TG4ComposedPhysicsList class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ComposedPhysicsList.h"
#include "TG4PhysicsManager.h"
#include "TG4G3PhysicsManager.h"
#include "TG4G3ControlVector.h"
#include "TG4ProcessControlMap.h"

#include <G4VUserPhysicsList.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessTable.hh>
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4Positron.hh>


//_____________________________________________________________________________
TG4ComposedPhysicsList::TG4ComposedPhysicsList()
  : G4VUserPhysicsList(),
    TG4Verbose("composedPhysicsList"),
    fMessenger(this),
    fPhysicsLists()
{
/// Default constructor

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

  G4cout << "TG4ComposedPhysicsList::ConstructParticle" << G4endl;

  for (G4int i=0; i<G4int(fPhysicsLists.size()); i++ )
    fPhysicsLists[i]->ConstructParticle();

  G4cout << "TG4ComposedPhysicsList::ConstructParticle done" << G4endl;
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::ConstructProcess()
{
/// Construct all processes.

  G4cout << "TG4ComposedPhysicsList::ConstructProcess" << G4endl;

  // lock physics manager
  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();
  g3PhysicsManager->Lock();  

  for (G4int i=0; i<G4int(fPhysicsLists.size()); i++ ) {
    G4cout << "fPhysicsLists[i]->ConstructProcess: " <<  fPhysicsLists[i] << G4endl;
    fPhysicsLists[i]->ConstructProcess();
  }  

  G4cout << "TG4ComposedPhysicsList::ConstructProcess done" << G4endl;
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCuts()
{
/// Set cut values

  // Set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(TG4PhysicsManager::Instance()->GetCutForGamma(), "gamma");
  SetCutValue(TG4PhysicsManager::Instance()->GetCutForElectron(), "e-");
  SetCutValue(TG4PhysicsManager::Instance()->GetCutForPositron(), "e+");

  if ( VerboseLevel() > 1 ) DumpCutValuesTable();
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
