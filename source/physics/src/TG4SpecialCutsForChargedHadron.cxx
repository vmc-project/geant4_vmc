// $Id: TG4SpecialCutsForChargedHadron.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: physics
//
// Class TG4SpecialCutsForChargedHadron
// ------------------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4SpecialCutsForChargedHadron.h"
#include "TG4Limits.h"


//_____________________________________________________________________________
TG4SpecialCutsForChargedHadron::TG4SpecialCutsForChargedHadron(
					  const G4String& processName)
  : TG4VSpecialCuts(processName) {
//
}

//_____________________________________________________________________________
TG4SpecialCutsForChargedHadron::TG4SpecialCutsForChargedHadron() {
//
}

//_____________________________________________________________________________
TG4SpecialCutsForChargedHadron::~TG4SpecialCutsForChargedHadron() {
//
}

//
// public methods
//

//_____________________________________________________________________________
G4double TG4SpecialCutsForChargedHadron::GetMinEkine(const TG4Limits& limits,
                                                     const G4Track& track) const
{					     
/// Return the min kinetic energy cut from limits.
// --- 

  return limits.GetMinEkineForChargedHadron(track);
}  

