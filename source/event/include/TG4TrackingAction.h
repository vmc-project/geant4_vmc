// $Id: TG4TrackingAction.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: event
//
// Author: I.Hrivnacova
//
// Class TG4TrackingAction
// -----------------------
// Class that ensures calling sensitive detector
// before track starts stepping.
// It also takes care of setting step status (kVertex)
// and passing G4Track to TG4StepManager.

#ifndef TG4_TRACKING_ACTION_H
#define TG4_TRACKING_ACTION_H

#include "TG4Verbose.h"
#include "TG4TrackingActionMessenger.h"

#include <G4UserTrackingAction.hh>

class TG4TrackInformation;

class G4Track;

class TG4TrackingAction : public G4UserTrackingAction,
                          public TG4Verbose 
{
  public:
    TG4TrackingAction();
    // --> protected
    // TG4TrackingAction(const TG4TrackingAction& right);
    virtual ~TG4TrackingAction();
   
    // static access method
    static TG4TrackingAction* Instance();

    // methods
    virtual void PrepareNewEvent();
    virtual void PreTrackingAction(const G4Track* aTrack) {;}
    virtual void PostTrackingAction(const G4Track* aTrack) {;}
                  // the following methods should not
		  // be overwritten in a derived class
    virtual void PreUserTrackingAction(const G4Track* aTrack);
    virtual void PostUserTrackingAction(const G4Track* aTrack);
    void FinishPrimaryTrack();
    void TrackToStack(const G4Track* track);

    // set methods
    void SetNewVerboseLevel(G4int level);
    void SetNewVerboseTrackID(G4int trackID);
    void SetSavePrimaries(G4bool savePrimaries);
    void SetSaveSecondaries(G4bool saveSecondaries);

    // get methods
    G4bool GetSavePrimaries() const;
    G4bool GetSaveSecondaries() const;
    G4int GetNofTracks() const;

  protected:
    TG4TrackingAction(const TG4TrackingAction& right);

    // operators
    TG4TrackingAction& operator=(const TG4TrackingAction& right);

  private:
    // methods
    void SetTrackInformation(const G4Track* aTrack);
    void SetParentToTrackInformation(const G4Track* aTrack);
    TG4TrackInformation* GetTrackInformation(const G4Track* track,
                                             const G4String& method) const;
    void UserProcessHits(const G4Track* aTrack);
    void Verbose() const;

    // static data members
    static TG4TrackingAction*   fgInstance; //this instance

    // data members
    TG4TrackingActionMessenger  fMessenger; //messenger
    G4int   fPrimaryTrackID;    //current primary track ID 
    G4bool  fSavePrimaries;     //control of saving primaries
    G4bool  fSaveSecondaries;   //control of saving primaries
    G4int   fNewVerboseLevel;   //new /tracking/verbose level
    G4int   fNewVerboseTrackID; //track ID for which new /tracking/verbose level
                                // is applied
    G4int   fTrackCounter;      //tracks counter
};


// inline methods

inline TG4TrackingAction* TG4TrackingAction::Instance() 
{ return fgInstance; }

inline void TG4TrackingAction::SetSavePrimaries(G4bool savePrimaries)
{ fSavePrimaries = savePrimaries; }

inline void TG4TrackingAction::SetSaveSecondaries(G4bool saveSecondaries)
{ fSaveSecondaries = saveSecondaries; }

inline G4bool TG4TrackingAction::GetSavePrimaries() const
{ return fSavePrimaries; }

inline G4bool TG4TrackingAction::GetSaveSecondaries() const
{ return fSaveSecondaries; }

inline G4int TG4TrackingAction::GetNofTracks() const
{ return fTrackCounter; }

#endif //TG4_TRACKING_ACTION_H
