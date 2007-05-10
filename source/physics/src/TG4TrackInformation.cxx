// $Id: TG4TrackInformation.cxx,v 1.2 2004/11/10 11:39:27 brun Exp $
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
    fParentParticleID(-1),
    fIsUserTrack(false) 
{
//
}

//_____________________________________________________________________________
TG4TrackInformation::TG4TrackInformation(G4int trackParticleID) 
  : G4VUserTrackInformation(),
    fTrackParticleID(trackParticleID),
    fParentParticleID(-1),
    fIsUserTrack(false) 
{
//
}    
/*
//_____________________________________________________________________________
TG4TrackInformation::TG4TrackInformation(G4int  trackParticleID, 
                                         G4int  parentParticleID)
  : G4VUserTrackInformation(),
    fTrackParticleID(trackParticleID),
    fParentParticleID(parentParticleID)
{
//
}    
*/
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
         << "ParentParticleID: " << fParentParticleID;

  if ( fIsUserTrack ) G4cout << "  userTrack";

  G4cout << G4endl;
}
