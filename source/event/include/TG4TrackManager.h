#ifndef TG4_TRACK_MANAGER_H
#define TG4_TRACK_MANAGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4TrackManager.h
/// \brief Definition of the TG4TrackManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4Verbose.h"
#include "TG4TrackSaveControl.h"

#include <G4UserTrackingAction.hh>
#include <G4TrackVector.hh>

class TG4TrackInformation;
class TG4StackPopper;

class TVirtualMCStack;

class G4Track;
class G4PrimaryVertex;
class G4PrimaryParticle;

/// \ingroup event
/// \brief The class for storing G4 tracks in VMC sack
///
/// It provides methods for storing G4 primary particles
/// and secondary tracks and utility functions for updating
/// TG4TrackInformation, which hold the info about
/// correspondence between Geant4 and VMC stack numbering
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4TrackManager : public TG4Verbose
{
  public:
    TG4TrackManager();
    virtual ~TG4TrackManager();

    // static access method
    static TG4TrackManager* Instance();

    // methods
    void  LateInitialize();
    void  AddPrimaryParticleId(G4int id);
    G4int SetTrackInformation(const G4Track* aTrack, G4bool overWrite = false);
    void  SetParentToTrackInformation(const G4Track* aTrack);
    void  SetBackPDGLifetime(const G4Track* aTrack);

    void  TrackToStack(const G4Track* track, G4bool overWrite = false);
    void  PrimaryToStack(const G4PrimaryVertex* vertex,
                       const G4PrimaryParticle* particle);

    void  SaveSecondaries(const G4Track* track, const G4TrackVector* secondaries);

    // set methods
    void SetMCStack(TVirtualMCStack*  mcStack);
    void SetTrackSaveControl(TG4TrackSaveControl control);
    void SetSaveDynamicCharge(G4bool saveDynamicCharge);
    void SetNofTracks(G4int nofTracks);
    void SetG4TrackingManager(G4TrackingManager* trackingManager);
    void ResetPrimaryParticleIds();

    // get methods
    TG4TrackInformation* GetTrackInformation(const G4Track* track) const;
    TG4TrackSaveControl  GetTrackSaveControl() const;
    G4bool GetSaveDynamicCharge() const;
    G4int  GetNofTracks() const;
    G4bool IsUserTrack(const G4Track* track) const;

  private:
    /// Not implemented
    TG4TrackManager(const TG4TrackManager& right);
    /// Not implemented
    TG4TrackManager& operator=(const TG4TrackManager& right);

    // static data members
    static G4ThreadLocal TG4TrackManager*   fgInstance; ///< this instance

    // data members
    G4TrackingManager*  fG4TrackingManager;  ///< G4 tracking manager
    std::vector<G4int>  fPrimaryParticleIds; ///< The VMC stack primary particle Ids
    TG4TrackSaveControl fTrackSaveControl;   ///< control of saving secondaries

    /// Cached pointer to thread-local VMC stack
    TVirtualMCStack*  fMCStack;

    /// Cached pointer to thread-local stack popper
    TG4StackPopper* fStackPopper;

    G4bool  fSaveDynamicCharge;     ///< control of saving dynamic charge of secondaries
    G4int   fTrackCounter;          ///< tracks counter
    G4int   fCurrentTrackID;        ///< current track ID
    G4int   fNofSavedSecondaries;   ///< number of secondaries already saved
};


// inline methods

inline TG4TrackManager* TG4TrackManager::Instance() {
  /// Return this instance.
  return fgInstance;
}

inline void TG4TrackManager::SetMCStack(TVirtualMCStack* mcStack) {
  /// Set  cached pointer to thread-local VMC stack
  fMCStack = mcStack;
}

inline void TG4TrackManager::SetTrackSaveControl(TG4TrackSaveControl control) {
  /// Set control for saving secondaries in the VMC stack
  fTrackSaveControl = control;
}

inline void TG4TrackManager::SetSaveDynamicCharge(G4bool saveDynamicCharge) {
  /// Set control of saving dynamic charge of secondaries (in TParticle::fStatus)
  fSaveDynamicCharge = saveDynamicCharge;
}

inline void TG4TrackManager::SetNofTracks(G4int nofTracks) {
  /// Set number of tracks
  fTrackCounter = nofTracks;
}

inline void TG4TrackManager::SetG4TrackingManager(
                       G4TrackingManager* trackingManager) {
  /// Set G4 tracking manager
  fG4TrackingManager = trackingManager;
}

inline TG4TrackSaveControl  TG4TrackManager::GetTrackSaveControl() const
{
  /// Return control of saving secondaries
  return fTrackSaveControl;
}

inline G4bool  TG4TrackManager::GetSaveDynamicCharge() const
{
  /// Return the control of saving dynamic charge of secondaries
  return fSaveDynamicCharge;
}

inline G4int TG4TrackManager::GetNofTracks() const {
  /// Return track counter = current number of tracks (in event)
  return fTrackCounter;
}

#endif //TG4_TRACK_MANAGER_H
