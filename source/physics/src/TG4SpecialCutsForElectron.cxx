// $Id: TG4SpecialCutsForElectron.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: physics
//
// Class TG4SpecialCutsForElectron
// -------------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4SpecialCutsForElectron.h"
#include "TG4Limits.h"


//_____________________________________________________________________________
TG4SpecialCutsForElectron::TG4SpecialCutsForElectron(
                                            const G4String& processName)
  : TG4VSpecialCuts(processName) {
//
}

//_____________________________________________________________________________
TG4SpecialCutsForElectron::TG4SpecialCutsForElectron() {
//
}

//_____________________________________________________________________________
TG4SpecialCutsForElectron::~TG4SpecialCutsForElectron() {
//
}

//
// public methods
//

//_____________________________________________________________________________
G4double TG4SpecialCutsForElectron::GetMinEkine(const TG4Limits& limits,
                                             const G4Track& track) const
{					     
/// Return the min kinetic energy cut from limits.
// --- 

  return limits.GetMinEkineForElectron(track);
}  

