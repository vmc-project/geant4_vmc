// $Id: TG4TrackingAction.h,v 1.5 2006/12/12 16:21:15 brun Exp $
/// \ingroup event
//
/// \class TG4TrackingAction
/// \brief Actions at the beginnig and at the end of track
/// 
/// Class that ensures calling sensitive detector
/// before track starts stepping.
/// It also takes care of setting step status (kVertex)
/// and passing G4Track to TG4StepManager.
///
/// Author: I.Hrivnacova

#ifndef TG4_TRACKING_ACTION_H
#define TG4_TRACKING_ACTION_H

#include "TG4Verbose.h"
#include "TG4TrackingActionMessenger.h"

#include <G4UserTrackingAction.hh>

class TG4TrackInformation;
class TG4TrackManager;

class G4Track;

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
    void SetSaveSecondaries(G4bool saveSecondaries);

    // get methods
    G4bool GetSaveSecondaries() const;

  private:
    TG4TrackingAction(const TG4TrackingAction& right);
    TG4TrackingAction& operator=(const TG4TrackingAction& right);

    // methods
    void UserProcessHits(const G4Track* aTrack);
    void Verbose() const;

    // static data members
    static TG4TrackingAction*   fgInstance; //this instance

    // data members
    TG4TrackingActionMessenger  fMessenger;   //messenger
    TG4TrackManager*            fTrackManager;//track manager 
    G4int   fPrimaryTrackID;    //current primary track ID 
    G4bool  fSaveSecondaries;   //control of saving secondaries
    G4int   fNewVerboseLevel;   //new /tracking/verbose level
    G4int   fNewVerboseTrackID; //track ID for which new /tracking/verbose level
                                // is applied
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

inline G4bool TG4TrackingAction::GetSaveSecondaries() const { 
  /// Return control for saving secondaries in the VMC stack
  return fSaveSecondaries; 
}

#endif //TG4_TRACKING_ACTION_H
