// $Id: TG4SpecialCutsForEplus.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: physics
//
// Class TG4SpecialCutsForEplus
// ----------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4SpecialCutsForEplus.h"
#include "TG4Limits.h"


//_____________________________________________________________________________
TG4SpecialCutsForEplus::TG4SpecialCutsForEplus(const G4String& processName)
  : TG4VSpecialCuts(processName) {
//
}

//_____________________________________________________________________________
TG4SpecialCutsForEplus::TG4SpecialCutsForEplus() {
//
}

//_____________________________________________________________________________
TG4SpecialCutsForEplus::~TG4SpecialCutsForEplus() {
//
}

//
// public methods
//

//_____________________________________________________________________________
G4double TG4SpecialCutsForEplus::GetMinEkine(const TG4Limits& limits,
                                             const G4Track& track) const
{					     
/// Return the min kinetic energy cut from limits.
// --- 

  return limits.GetMinEkineForEplus(track);
}  

