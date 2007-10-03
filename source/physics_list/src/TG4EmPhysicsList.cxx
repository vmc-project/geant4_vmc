// $Id: TG4EmPhysicsList.cxx,v 1.13 2007/08/09 13:43:34 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics_lists
// Class TG4EmPhysicsList
// ---------------------------
// See the class description in the header file.

#include "TG4EmPhysicsList.h"

#include <G4EmStandardPhysics.hh>
#include <G4DecayPhysics.hh>

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessTable.hh>

const G4double TG4EmPhysicsList::fgkDefaultCutValue = 1.0 * mm;

//_____________________________________________________________________________
TG4EmPhysicsList::TG4EmPhysicsList()
  : G4VModularPhysicsList(),
    TG4Verbose("emPhysicsList")
 {
//
  Configure();

  defaultCutValue = fgkDefaultCutValue;

  SetVerboseLevel(TG4VVerbose::VerboseLevel());
}

//_____________________________________________________________________________
TG4EmPhysicsList::~TG4EmPhysicsList() {
//
  //delete fExtDecayer;
       // fExtDecayer is deleted in G4Decay destructor
}

//
// private methods
//

//_____________________________________________________________________________
void TG4EmPhysicsList::Configure()
{
/// Create the selected physics constructors
/// and registeres them in the modular physics list.

  // electromagnetic physics
  RegisterPhysics(new G4EmStandardPhysics());

  // decay physics
  RegisterPhysics(new G4DecayPhysics());
}    

//
// public methods
//

//_____________________________________________________________________________
void TG4EmPhysicsList::ConstructProcess()
{
/// Call ase class method + add verbose info

  // create processes for registered physics
  G4VModularPhysicsList::ConstructProcess();

  if (VerboseLevel() > 0) 
    G4cout << "### EM physics constructed. " << G4endl;
}

//_____________________________________________________________________________
G4int TG4EmPhysicsList::VerboseLevel() const 
{
/// Return verbose level (via TG4VVerbose)

  return TG4VVerbose::VerboseLevel();
}


//_____________________________________________________________________________
void TG4EmPhysicsList::VerboseLevel(G4int level) 
{
/// Set the specified level to both TG4Verbose and 
/// G4VModularPhysicsList.
/// The verbose level is also propagated to registered physics contructors.

  TG4VVerbose::VerboseLevel(level);
  SetVerboseLevel(level);
  
  G4PhysConstVector::iterator it;
  for ( it = physicsVector->begin(); it != physicsVector->end(); ++it ) {
    TG4Verbose* verbose = dynamic_cast<TG4Verbose*>(*it);
    if ( verbose )
      verbose->VerboseLevel(level);
    else
      (*it)->SetVerboseLevel(level);  
  }
}

//_____________________________________________________________________________
void TG4EmPhysicsList::SetRangeCut(G4double value)
{
/// Reset the default cut to a given value.                                 \n
/// !!! Should be used only in PreInit phase,
/// use SetDefaultCutValue() method of base class to reset
/// the cut value in later phases.

  defaultCutValue = value;
}  
