// $Id: TG4VVerbose.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: global
//
// Class TG4Verbose
// -------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

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

