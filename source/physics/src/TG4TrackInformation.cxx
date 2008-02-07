// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4TrackInformation.cxx
/// \brief Implementation of the TG4TrackInformation class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4TrackInformation.h"

/// Geant4 allocator for TG4TrackInformation objects
G4Allocator<TG4TrackInformation> gTrackInfoAllocator;

//_____________________________________________________________________________
TG4TrackInformation::TG4TrackInformation()
  : G4VUserTrackInformation(),
    fTrackParticleID(-1),
    fParentParticleID(-1),
    fPDGLifetime(-1.0),
    fIsUserTrack(false)
{
/// Default constructor
}

//_____________________________________________________________________________
TG4TrackInformation::TG4TrackInformation(G4int trackParticleID) 
  : G4VUserTrackInformation(),
    fTrackParticleID(trackParticleID),
    fParentParticleID(-1),
    fPDGLifetime(-1.0), 
    fIsUserTrack(false)
{
/// Standard constructor
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
TG4TrackInformation::~TG4TrackInformation()
{
/// Destructor
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
