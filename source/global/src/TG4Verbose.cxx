// $Id: TG4Verbose.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: global
//
// Class TG4Verbose
// -------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4Verbose.h"
#include "TG4Globals.h"

#include <math.h>

// static data members
const G4String       TG4Verbose::fgkDirectoryName = "/mcVerbose/";
TG4VerboseMessenger* TG4Verbose::fgMessenger = 0;

//_____________________________________________________________________________
TG4Verbose::TG4Verbose(const G4String& cmdName)
  : TG4VVerbose() {
//
  CreateMessenger();
  
  fgMessenger->AddCommand(this, cmdName);  
}
  
//_____________________________________________________________________________
TG4Verbose::TG4Verbose(const G4String& cmdName, G4int verboseLevel) 
  : TG4VVerbose(verboseLevel) {
// 
  CreateMessenger();

  fgMessenger->AddCommand(this, cmdName);  
}

//_____________________________________________________________________________
TG4Verbose::TG4Verbose()
  : TG4VVerbose() {
//
}
  
//_____________________________________________________________________________
TG4Verbose::~TG4Verbose() {
//
  if (fgMessenger) {
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
    
