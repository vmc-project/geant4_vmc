// $Id: TG4VPhysicsConstructor.cxx,v 1.3 2004/11/10 11:39:28 brun Exp $
// Category: physics
//
// Class TG4VPhysicsConstructor
// ------------------------------
// See the class description in the header file.
// According to ExN04IonPhysics.cc,v 1.1.2.1 2001/06/28 19:07:37 gunter Exp 
// GEANT4 tag Name: geant4-06-00
//
// Author: I. Hrivnacova

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
