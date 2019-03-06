#ifndef TG4_GEO_TRAK_MANAGER_H
#define TG4_GEO_TRAK_MANAGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4GeoTrackManager.h
/// \brief Definition of the TG4GeoTrackManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4Verbose.h"

#include <globals.hh>

class G4Step;

class TVirtualGeoTrack;

/// \ingroup event
/// \brief The manager class for collecting TGeo tracks for visualization
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4GeoTrackManager : public TG4Verbose
{
  public:
    TG4GeoTrackManager();
    virtual ~TG4GeoTrackManager();

    // methods
    void UpdateRootTrack(const G4Step* step);

  private:
    /// Not implemented
    TG4GeoTrackManager(const TG4GeoTrackManager& right);
    /// Not implemented
    TG4GeoTrackManager& operator=(const TG4GeoTrackManager& right);

    // static data members
    /// minimum point distance to store a point in TGeo track
    static const G4double  fgkMinPointDistance;

    //
    // data members

    /// control to collect Root tracks
    G4bool fCollectTracks;

    /// current Root track
    TVirtualGeoTrack* fCurrentTGeoTrack;

    /// parent of the current Root track
    TVirtualGeoTrack* fParentTGeoTrack;
};

#endif //TG4_GEO_TRAK_MANAGER_H
