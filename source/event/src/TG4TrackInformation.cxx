// $Id: TG4TrackInformation.cxx,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: event
//
// Class TG4TrackInformation
// -------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

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

//
// public methods
//

//_____________________________________________________________________________
void TG4TrackInformation::Print() const
{
/// Print track information.

  G4cout << "TrackParticleID: " << fTrackParticleID << "   "
         << "ParentParticleID: " << fParentParticleID << G4endl;
}
