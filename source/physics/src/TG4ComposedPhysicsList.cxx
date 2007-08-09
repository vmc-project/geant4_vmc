// $Id: TG4ComposedPhysicsList.cxx,v 1.12 2007/05/31 10:24:33 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics
// Class TG4ComposedPhysicsList
// ----------------------------
// See the class description in the header file.

#include "TG4ComposedPhysicsList.h"
#include "TG4G3PhysicsManager.h"
#include "TG4G3ControlVector.h"
#include "TG4ProcessControlMap.h"

#include <G4VUserPhysicsList.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessTable.hh>

//_____________________________________________________________________________
TG4ComposedPhysicsList::TG4ComposedPhysicsList()
  : G4VUserPhysicsList(),
    TG4Verbose("physicsListBuilder")
 {
//
  SetVerboseLevel(TG4VVerbose::VerboseLevel());
}

//_____________________________________________________________________________
TG4ComposedPhysicsList::~TG4ComposedPhysicsList() {
//

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

  for (G4int i=0; i<G4int(fPhysicsLists.size()); i++ )
    fPhysicsLists[i]->ConstructParticle();
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::ConstructProcess()
{
/// Construct all processes.

  for (G4int i=0; i<G4int(fPhysicsLists.size()); i++ )
    fPhysicsLists[i]->ConstructProcess();
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCuts()
{
/// Set the default cut value for all particle types.

  for (G4int i=0; i<G4int(fPhysicsLists.size()); i++ )
    fPhysicsLists[i]->SetCuts();
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::PrintAllProcesses() const
{
/// Print all processes.

  G4cout << "TG4ComposedPhysicsList processes: " << G4endl;
  G4cout << "================================ " << G4endl;
 
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

  G4cout << "TG4ComposedPhysicsList particles and processes: " << G4endl;
  G4cout << "============================================== " << G4endl;
 
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
