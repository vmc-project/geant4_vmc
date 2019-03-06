//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4Verbose.cxx
/// \brief Implementation of the TG4Verbose class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4Verbose.h"

// static data members
              const G4String       TG4Verbose::fgkDirectoryName = "/mcVerbose/";
G4ThreadLocal G4int                TG4Verbose::fgCounter = 0;
G4ThreadLocal TG4VerboseMessenger* TG4Verbose::fgMessenger = 0;

//_____________________________________________________________________________
TG4Verbose::TG4Verbose(const G4String& cmdName)
  : TG4VVerbose(),
    fCommand(0)
{
/// Standard constructor with specified \em name

  CreateMessenger();

  fCommand = fgMessenger->AddCommand(this, cmdName);

  fgCounter++;
}

//_____________________________________________________________________________
TG4Verbose::TG4Verbose(const G4String& cmdName, G4int verboseLevel)
  : TG4VVerbose(verboseLevel),
    fCommand(0)
{
/// Standard constructor with specified \em name and \em verboseLevel

  CreateMessenger();

  fCommand = fgMessenger->AddCommand(this, cmdName);
}

//_____________________________________________________________________________
TG4Verbose::~TG4Verbose()
{
/// Destructor

  //G4cout << "TG4Verbose::~TG4Verbose " << fgCounter << " " << this << G4endl;

  fgCounter--;

  if (!fgMessenger) return;

  fgMessenger->RemoveCommand(this, fCommand);
     // fCommand is deleted by fgMessenger

  if (fgCounter==0) {
    delete fgMessenger;
    fgMessenger = 0;
  }
}

//
// private methods
//

//_____________________________________________________________________________
TG4VerboseMessenger* TG4Verbose::CreateMessenger()
{
/// Create static messenger if it does not yet exists.

  if (!fgMessenger)
    fgMessenger = new TG4VerboseMessenger(fgkDirectoryName);

  return fgMessenger;
}

