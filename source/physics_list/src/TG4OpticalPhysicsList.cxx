// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics_lists
// Class TG4OpticalPhysicsList
// ---------------------------
// See the class description in the header file.

#include "TG4OpticalPhysicsList.h"
#include "TG4OpticalPhysics.h"

//_____________________________________________________________________________
TG4OpticalPhysicsList::TG4OpticalPhysicsList()
  : G4VModularPhysicsList(),
    TG4Verbose("opticalPhysicsList")
 {
//
  Configure();

  SetVerboseLevel(TG4VVerbose::VerboseLevel());
}

//_____________________________________________________________________________
TG4OpticalPhysicsList::~TG4OpticalPhysicsList() {
//
  //delete fExtDecayer;
       // fExtDecayer is deleted in G4Decay destructor
}

//
// private methods
//

//_____________________________________________________________________________
void TG4OpticalPhysicsList::Configure()
{
/// Create the selected physics constructors
/// and registeres them in the modular physics list.

  // electromagnetic physics
  RegisterPhysics(new TG4OpticalPhysics());
}    

//
// public methods
//

//_____________________________________________________________________________
void TG4OpticalPhysicsList::ConstructProcess()
{
/// Construct all processes.

  // Create processes for registered physics
  // To avoid call AddTransportation twice we do not call directly
  // G4VModularPhysicsList::ConstructProcess();
  // but call registered processes ourselves:
  G4PhysConstVector::iterator itr;
  for (itr = physicsVector->begin(); itr!= physicsVector->end(); ++itr) {
    (*itr)->ConstructProcess();
  }
}

//_____________________________________________________________________________
G4int TG4OpticalPhysicsList::VerboseLevel() const 
{
/// Return verbose level (via TG4VVerbose)

  return TG4VVerbose::VerboseLevel();
}

//_____________________________________________________________________________
void TG4OpticalPhysicsList::VerboseLevel(G4int level) 
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
