// $Id: TG4VVerbose.cxx,v 1.2 2004/11/10 11:39:28 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: global
// Class TG4Verbose
// -------------------
// See the class description in the header file.

#include "TG4VVerbose.h"
#include "TG4Globals.h"

#include <math.h>

// static data members
const G4int TG4VVerbose::fgkDefaultVerboseLevel = 1;

//_____________________________________________________________________________
TG4VVerbose::TG4VVerbose()
  : fVerboseLevel(fgkDefaultVerboseLevel) {
//
}
  
//_____________________________________________________________________________
TG4VVerbose::TG4VVerbose(G4int verboseLevel) 
  : fVerboseLevel(verboseLevel) {
// 
}

//_____________________________________________________________________________
TG4VVerbose::~TG4VVerbose() {
//
}

