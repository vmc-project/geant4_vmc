// $Id: TG4SpecialCutsForNeutralHadron.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: physics
//
// Class TG4SpecialCutsForNeutralHadron
// ------------------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4SpecialCutsForNeutralHadron.h"
#include "TG4Limits.h"


//_____________________________________________________________________________
TG4SpecialCutsForNeutralHadron::TG4SpecialCutsForNeutralHadron(
                                              const G4String& processName)
  : TG4VSpecialCuts(processName) {
//
}

//_____________________________________________________________________________
TG4SpecialCutsForNeutralHadron::TG4SpecialCutsForNeutralHadron() {
//
}

//_____________________________________________________________________________
TG4SpecialCutsForNeutralHadron::~TG4SpecialCutsForNeutralHadron() {
//
}

//
// public methods
//

//_____________________________________________________________________________
G4double TG4SpecialCutsForNeutralHadron::GetMinEkine(const TG4Limits& limits,
                                                     const G4Track& track) const
{					     
/// Return the min kinetic energy cut from limits.
// --- 

  return limits.GetMinEkineForNeutralHadron(track);
}  

