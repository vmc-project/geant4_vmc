//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4GeoTrackManager.cxx
/// \brief Implementation of the TG4GeoTrackManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4GeoTrackManager.h"
#include "TG4Globals.h"

#include <G4Track.hh>

#include <TVirtualMC.h>
#include <TGeoManager.h>
#include <TVirtualGeoTrack.h>
#include <TParticlePDG.h>
#include <TDatabasePDG.h>

// static data members
const G4double  TG4GeoTrackManager::fgkMinPointDistance = 0.01;

//_____________________________________________________________________________
TG4GeoTrackManager::TG4GeoTrackManager()
  : TG4Verbose("geoTrackManager"),
    fCollectTracks(kFALSE),
    fCurrentTGeoTrack(0),
    fParentTGeoTrack(0)
{
/// Default constructor
}

//_____________________________________________________________________________
TG4GeoTrackManager::~TG4GeoTrackManager()
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void TG4GeoTrackManager::UpdateRootTrack(const G4Step* step)
{
/// Update the TGeo track with a current step point

   G4Track* track = step->GetTrack();
   G4int stepNumber = track->GetCurrentStepNumber();

   if ( stepNumber == 1 ) {
     // Find and update parent track if it exists
     Int_t trackNumber
       = gMC->GetStack()->GetCurrentTrackNumber();
     Int_t parentTrackNumber
       = gMC->GetStack()->GetCurrentParentTrackNumber();
     Int_t pdg = gMC->TrackPid();
     if ( parentTrackNumber >= 0 ) {
       fParentTGeoTrack = gGeoManager->FindTrackWithId(parentTrackNumber);
       if ( ! fParentTGeoTrack ) {
         TString text = "No parent track with id=";
         text += parentTrackNumber;
         TG4Globals::Exception("TG4GeoTrackManager", "UpdateRootTrack", text);
       }
       fCurrentTGeoTrack = fParentTGeoTrack->AddDaughter(trackNumber, pdg);
       gGeoManager->SetCurrentTrack(fCurrentTGeoTrack);

       if ( VerboseLevel() > 1 ) {
         G4cout << "New TGeo track with id=" << trackNumber
                << "  pdg=" << pdg
                << "  parent=" << parentTrackNumber << G4endl;
       }
     }
     else {
       Int_t itrack = gGeoManager->AddTrack(trackNumber, pdg);
       gGeoManager->SetCurrentTrack(itrack);
       fCurrentTGeoTrack = gGeoManager->GetCurrentTrack();
       if ( VerboseLevel() > 1 ) {
         G4cout << "New primary TGeo track with id=" << trackNumber
                << "  pdg=" << pdg << G4endl;
       }
     }
     TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle(pdg);
     if ( particle ) {
       fCurrentTGeoTrack->SetName(particle->GetName());
       fCurrentTGeoTrack->SetParticle(particle);
     }
   }

   // Skip point if its distance from the previous one is smaller than the limit
   Double_t x, y, z;
   gMC->TrackPosition(x, y, z);
   Bool_t skipPoint = kFALSE;
   if ( fCurrentTGeoTrack->HasPoints() ) {
      Double_t xo,yo,zo,to;
      fCurrentTGeoTrack->GetLastPoint(xo,yo,zo,to);
      Double_t rdist = TMath::Sqrt((xo-x)*(xo-x)+(yo-y)*(yo-y)+(zo-z)*(zo-z));
      if ( rdist < fgkMinPointDistance ) skipPoint=kTRUE;
   }
   if ( skipPoint ) return;

   // Add point to the track
   G4double time = gMC->TrackTime();
   fCurrentTGeoTrack->AddPoint(x, y, z, time);
   if ( VerboseLevel() > 2 ) {
     G4cout << "Added point (x,y,z,t)="
            << x << ", " << y << ", " << z << ", " << time << G4endl;
   }
}
