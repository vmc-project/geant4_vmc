// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
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
#include "TG4G3PhysicsManager.h"
#include "TG4G3ControlVector.h"
#include "TG4ProcessControlMap.h"

#include <G4VUserPhysicsList.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessTable.hh>
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4Positron.hh>


const G4double TG4ComposedPhysicsList::fgkDefautCut = 1.0*mm;  

//_____________________________________________________________________________
TG4ComposedPhysicsList::TG4ComposedPhysicsList()
  : G4VUserPhysicsList(),
    TG4Verbose("composedPhysicsList"),
    fMessenger(this),
    fPhysicsLists(),
    fCutForGamma(fgkDefautCut),
    fCutForElectron(fgkDefautCut),
    fCutForPositron(fgkDefautCut)
{
/// Default constructor

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

  for (G4int i=0; i<G4int(fPhysicsLists.size()); i++ )
    fPhysicsLists[i]->ConstructParticle();
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::ConstructProcess()
{
/// Construct all processes.

  // lock physics manager
  TG4G3PhysicsManager* g3PhysicsManager = TG4G3PhysicsManager::Instance();
  g3PhysicsManager->Lock();  

  for (G4int i=0; i<G4int(fPhysicsLists.size()); i++ )
    fPhysicsLists[i]->ConstructProcess();
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCuts()
{
/// Set cut values

  // Set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(fCutForGamma, "gamma");
  SetCutValue(fCutForElectron, "e-");
  SetCutValue(fCutForPositron, "e+");

  if ( VerboseLevel() > 1 ) DumpCutValuesTable();
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCutForGamma(G4double cut)
{
/// Set cut value for gamma

  fCutForGamma = cut;
  SetParticleCuts(fCutForGamma, G4Gamma::Gamma());
}

//_____________________________________________________________________________
void TG4ComposedPhysicsList::SetCutForElectron(G4double cut)
{
/// Set cut value for electron

  fCutForElectron = cut;
  SetParticleCuts(fCutForElectron, G4Electron::Electron());
}

//_____________________________________________________________________________

void TG4ComposedPhysicsList::SetCutForPositron(G4double cut)
{
/// Set cut value for positron

  fCutForPositron = cut;
  SetParticleCuts(fCutForPositron, G4Positron::Positron());
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
  
  for (G4int i=0; i<G4int(fPhysicsLists.size()); i++ )
    fPhysicsLists[i]->SetVerboseLevel(level);
}
