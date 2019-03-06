//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4VPhysicsConstructor.cxx
/// \brief Implementation of the TG4VPhysicsConstructor class
///
/// According to ExN04IonPhysics.cc,v 1.1.2.1 2001/06/28 19:07:37 gunter Exp
/// GEANT4 tag Name: geant4-06-00
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4VPhysicsConstructor.h"
#include "TG4Globals.h"

//_____________________________________________________________________________
TG4VPhysicsConstructor::TG4VPhysicsConstructor(const G4String& name)
  : G4VPhysicsConstructor(name),
    TG4Verbose(G4String("physics" + name))
{
/// Standard constructor

  VerboseLevel(1);
}

//_____________________________________________________________________________
TG4VPhysicsConstructor::TG4VPhysicsConstructor(const G4String& name,
                                               G4int theVerboseLevel)
  : G4VPhysicsConstructor(name),
    TG4Verbose(G4String("physics" + name))
{
/// Standard constructor

  VerboseLevel(theVerboseLevel);
}

//_____________________________________________________________________________
TG4VPhysicsConstructor::~TG4VPhysicsConstructor()
{
/// Destructor
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
