// $Id: TG4SpecialCutsForGamma.cxx,v 1.1.1.1 2002/06/16 15:57:35 hristov Exp $
// Category: physics
//
// See the class description in the header file.

#include "TG4SpecialCutsForGamma.h"
#include "TG4Limits.h"


//_____________________________________________________________________________
TG4SpecialCutsForGamma::TG4SpecialCutsForGamma(const G4String& processName)
  : TG4VSpecialCuts(processName) {
//
}

//_____________________________________________________________________________
TG4SpecialCutsForGamma::TG4SpecialCutsForGamma() {
//
}

//_____________________________________________________________________________
TG4SpecialCutsForGamma::~TG4SpecialCutsForGamma() {
//
}

// public methods

//_____________________________________________________________________________
G4double TG4SpecialCutsForGamma::GetMinEkine(const TG4Limits& limits,
                                             const G4Track& track) const
{					     
// Returns the min kinetic energy cut from limits.
// --- 

  return limits.GetMinEkineForGamma(track);
}  

