#ifndef TG4_TRACKING_ACTION_H
#define TG4_TRACKING_ACTION_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4TrackingAction.h
/// \brief Definition of the TG4TrackingAction class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4Verbose.h"
#include "TG4TrackingActionMessenger.h"
#include "TG4TrackSaveControl.h"

#include <G4UserTrackingAction.hh>

class TG4TrackInformation;
class TG4TrackManager;
class TG4SpecialControlsV2;

class G4Track;

/// \ingroup event
/// \brief Actions at the beginnig and at the end of track
/// 
/// Class that ensures calling sensitive detector
/// before track starts stepping.
/// It also takes care of setting step status (kVertex)
/// and passing G4Track to TG4StepManager.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4TrackingAction : public G4UserTrackingAction,
                          public TG4Verbose 
{
  public:
    TG4TrackingAction();
    virtual ~TG4TrackingAction();
   
    // static access method
    static TG4TrackingAction* Instance();

    // methods
    virtual void PrepareNewEvent();
    virtual void PreTrackingAction(const G4Track* aTrack);
    virtual void PostTrackingAction(const G4Track* aTrack);
                  // the following methods should not
                  // be overwritten in a derived class
    virtual void PreUserTrackingAction(const G4Track* aTrack);
    virtual void PostUserTrackingAction(const G4Track* aTrack);
    void FinishPrimaryTrack();

    // set methods
    void SetNewVerboseLevel(G4int level);
    void SetNewVerboseTrackID(G4int trackID);
    void SetSpecialControls(TG4SpecialControlsV2* specialControls);

  private:
    /// Not implemented
    TG4TrackingAction(const TG4TrackingAction& right);
    /// Not implemented
    TG4TrackingAction& operator=(const TG4TrackingAction& right);

    // methods
    void UserProcessHits(const G4Track* aTrack);
    void Verbose() const;

    // static data members
    static __thread TG4TrackingAction*   fgInstance; ///< this instance

    //
    // data members
    
    /// messenger
    TG4TrackingActionMessenger  fMessenger; 
    
    /// the special controls manager
    TG4SpecialControlsV2*  fSpecialControls;
    
    /// track manager 
    TG4TrackManager*  fTrackManager;
    
    /// current primary track ID 
    G4int   fPrimaryTrackID;
    
    /// current track ID 
    G4int   fCurrentTrackID; 
    
    /// control of saving secondary tracks
    TG4TrackSaveControl  fTrackSaveControl;

    /// control of saving current track
    G4bool  fOverwriteLastTrack;

    /// new /tracking/verbose level
    G4int   fNewVerboseLevel;
    
    /// track ID for which the new verbose level is applied
    G4int   fNewVerboseTrackID;
};


// inline methods

inline TG4TrackingAction* TG4TrackingAction::Instance() { 
  /// Return this instance.
  return fgInstance; 
}

inline void TG4TrackingAction::PreTrackingAction(const G4Track* /*aTrack*/) {
  /// Dummy pre-tracking action that can be overriden
  /// in a user defined class
}

inline void TG4TrackingAction::PostTrackingAction(const G4Track* /*aTrack*/) {
  /// Dummy post-tracking action that can be overriden
  /// in a user defined class
}

inline void TG4TrackingAction::SetSpecialControls(TG4SpecialControlsV2* specialControls) {
  /// Return special controls manager
  fSpecialControls = specialControls;
}  

#endif //TG4_TRACKING_ACTION_H
