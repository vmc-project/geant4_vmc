// $Id: TG4TrackInformation.cxx,v 1.1.1.1 2002/06/16 15:57:34 hristov Exp $
// Category: event
//
// Author: I. Hrivnacova
//
// Class TG4TrackInformation
// -------------------------
// See the class description in the header file.

#include "TG4TrackInformation.h"

G4Allocator<TG4TrackInformation> gAliTrackInfoAllocator;

//_____________________________________________________________________________
TG4TrackInformation::TG4TrackInformation()
  : G4VUserTrackInformation(),
    fTrackParticleID(-1),
    fParentParticleID(-1) 
{
//
}

//_____________________________________________________________________________
TG4TrackInformation::TG4TrackInformation(G4int trackParticleID) 
  : G4VUserTrackInformation(),
    fTrackParticleID(trackParticleID),
    fParentParticleID(-1)
{
//
}    

//_____________________________________________________________________________
TG4TrackInformation::TG4TrackInformation(G4int trackParticleID, 
                                         G4int parentParticleID)
  : G4VUserTrackInformation(),
    fTrackParticleID(trackParticleID),
    fParentParticleID(parentParticleID)
{
//
}    

//_____________________________________________________________________________
TG4TrackInformation::~TG4TrackInformation(){
//
}    

// public methods

//_____________________________________________________________________________
void TG4TrackInformation::Print() const
{
// Prints track information.
// ---

  G4cout << "TrackParticleID: " << fTrackParticleID << "   "
         << "ParentParticleID: " << fParentParticleID << G4endl;
}
