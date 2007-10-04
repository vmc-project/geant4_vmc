// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: event
// Class TG4TrackInformation
// -------------------------
// See the class description in the header file.

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
