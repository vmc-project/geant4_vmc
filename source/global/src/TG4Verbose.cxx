// $Id: TG4Verbose.cxx,v 1.3 2005/01/28 12:29:38 brun Exp $
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
G4int                TG4Verbose::fgCounter = 0;
TG4VerboseMessenger* TG4Verbose::fgMessenger = 0;

//_____________________________________________________________________________
TG4Verbose::TG4Verbose(const G4String& cmdName)
  : TG4VVerbose(),
    fCommand(0) 
{
//
  CreateMessenger();
  
  fCommand = fgMessenger->AddCommand(this, cmdName);
  
  fgCounter++;  
}
  
//_____________________________________________________________________________
TG4Verbose::TG4Verbose(const G4String& cmdName, G4int verboseLevel) 
  : TG4VVerbose(verboseLevel),
    fCommand(0) 
{
// 
  CreateMessenger();

  fCommand = fgMessenger->AddCommand(this, cmdName);  
}

//_____________________________________________________________________________
TG4Verbose::~TG4Verbose() {
//
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
    
