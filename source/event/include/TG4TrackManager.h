// $Id: $
/// \ingroup event
//
/// \class TG4TrackManager
/// \brief Actions at the beginnig and at the end of track
/// 
/// Class that ensures calling sensitive detector
/// before track starts stepping.
/// It also takes care of setting step status (kVertex)
/// and passing G4Track to TG4StepManager.
///
/// Author: I.Hrivnacova

#ifndef TG4_TRACK_MANAGER_H
#define TG4_TRACK_MANAGER_H

#include "TG4Verbose.h"

#include <G4UserTrackingAction.hh>
#include <G4TrackVector.hh>

class TG4TrackInformation;

class G4Track;
class G4PrimaryVertex;
class G4PrimaryParticle;

class TG4TrackManager : public TG4Verbose 
{
  public:
    TG4TrackManager();
    virtual ~TG4TrackManager();
   
    // static access method
    static TG4TrackManager* Instance();

    // methods
    G4int SetTrackInformation(const G4Track* aTrack);
    void  SetParentToTrackInformation(const G4Track* aTrack);

    void  TrackToStack(const G4Track* track);
    void  PrimaryToStack(const G4PrimaryVertex* vertex,
                       const G4PrimaryParticle* particle);

    void  SaveSecondaries(const G4Track* track, G4TrackVector* secondaries);

    // set methods
    void SetSaveSecondaries(G4bool saveSecondaries, G4bool inStep = false);
    void SetNofTracks(G4int nofTracks);
    void SetG4TrackingManager(G4TrackingManager* trackingManager);

    // get methods
    G4bool GetSaveSecondaries() const;
    G4int  GetNofTracks() const;
    G4bool IsUserTrack(const G4Track* track) const;

  private:
    TG4TrackManager(const TG4TrackManager& right);
    TG4TrackManager& operator=(const TG4TrackManager& right);

    // methods
    TG4TrackInformation* GetTrackInformation(const G4Track* track) const;

    // static data members
    static TG4TrackManager*   fgInstance; //this instance

    // data members
    G4TrackingManager*   fG4TrackingManager;  // G4 tracking manager
    //TG4TrackManagerMessenger  fMessenger; //messenger
    G4bool  fSaveSecondaries;   //control of saving secondaries
    G4bool  fSaveSecondariesInStep;   //control of saving secondaries in step
    G4int   fTrackCounter;      //tracks counter
    G4int   fCurrentTrackID;     //current track ID
    G4int   fNofSavedSecondaries;//number of secondaries already saved
};


// inline methods

inline TG4TrackManager* TG4TrackManager::Instance() { 
  /// Return this instance.
  return fgInstance; 
}

inline void TG4TrackManager::SetNofTracks(G4int nofTracks) {
  /// Set number of tracks
  fTrackCounter = nofTracks;
}  

inline void TG4TrackManager::SetG4TrackingManager(G4TrackingManager* trackingManager) {
  /// Set G4 tracking manager
  fG4TrackingManager = trackingManager;
}  

inline G4bool TG4TrackManager::GetSaveSecondaries() const { 
  /// Return control for saving secondaries in the VMC stack
  return fSaveSecondaries || fSaveSecondariesInStep; 
}

inline G4int TG4TrackManager::GetNofTracks() const { 
  /// Return track counter = current number of tracks (in event)  
  return fTrackCounter; 
}

#endif //TG4_TRACK_MANAGER_H
