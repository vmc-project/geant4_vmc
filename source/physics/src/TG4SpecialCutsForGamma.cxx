// $Id: TG4SpecialCutsForGamma.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: physics
//
// Class TG4SpecialCutsForGamma
// -----------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

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

//
// public methods
//

//_____________________________________________________________________________
G4double TG4SpecialCutsForGamma::GetMinEkine(const TG4Limits& limits,
                                             const G4Track& track) const
{					     
/// Return the min kinetic energy cut from limits.
// --- 

  return limits.GetMinEkineForGamma(track);
}  

