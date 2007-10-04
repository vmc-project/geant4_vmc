// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics
// Class TG4VPhysicsConstructor
// ------------------------------
// See the class description in the header file.
// According to ExN04IonPhysics.cc,v 1.1.2.1 2001/06/28 19:07:37 gunter Exp 
// GEANT4 tag Name: geant4-06-00

#include "TG4VPhysicsConstructor.h"
#include "TG4Globals.h"

//_____________________________________________________________________________
TG4VPhysicsConstructor::TG4VPhysicsConstructor(const G4String& name)
  : G4VPhysicsConstructor(name),
    TG4Verbose(G4String("physics" + name))
{
//
  VerboseLevel(1);
}

//_____________________________________________________________________________
TG4VPhysicsConstructor::TG4VPhysicsConstructor(const G4String& name,
                                               G4int verboseLevel)
  : G4VPhysicsConstructor(name),
    TG4Verbose(G4String("physics" + name))
{
//
  VerboseLevel(verboseLevel);
}

//_____________________________________________________________________________
TG4VPhysicsConstructor::~TG4VPhysicsConstructor() {
//
}

//
// public methods
//

//_____________________________________________________________________________
void TG4VPhysicsConstructor::VerboseLevel(G4int level)
{
/// Set the same value to G4VPhysicsConstructor verbose
/// level and TG4Verbose level.

   TG4Verbose::VerboseLevel(level);
   
   // verbose in G4VPhysicsConstructor 
   SetVerboseLevel(level);
}


//_____________________________________________________________________________
G4int TG4VPhysicsConstructor::VerboseLevel() const
{
/// Return TG4Verbose level value.

   return TG4Verbose::VerboseLevel();
}
