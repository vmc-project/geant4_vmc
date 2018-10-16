#ifndef TG4_TRACK_INFORMATION_H
#define TG4_TRACK_INFORMATION_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4TrackInformation.h
/// \brief Definition of the TG4TrackInformation class
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4Allocator.hh>
#include <G4VUserTrackInformation.hh>
#include <globals.hh>

#include "TMCParticleStatus.h"

class G4ParticleDefinition;

/// \ingroup physics
/// \brief Defines additional track information.
///
/// \author I. Hrivnacova; IPN Orsay

class TG4TrackInformation : public G4VUserTrackInformation
{
 public:
  TG4TrackInformation();
  TG4TrackInformation(G4int trackParticleID);
  // TG4TrackInformation(G4int trackParticleID, G4int parentParticleID);
  virtual ~TG4TrackInformation();

  // operators required by G4

  /// Override \em new operator for G4Allocator
  inline void* operator new(size_t);
  /// Override \em delete operator for G4Allocator
  inline void operator delete(void* trackInformation);

  // methods
  virtual void Print() const;

  // set methods
  void SetTrackParticleID(G4int trackParticleID);
  void SetParentParticleID(G4int parentParticleID);
  void SetPDGLifetime(G4double pdgLifetime);
  void SetPDGEncoding(G4int pdgEncoding);
  void SetInitialTrackStatus(TMCParticleStatus* status);
  void SetIsUserTrack(G4bool isUserTrack);
  void SetStop(G4bool stop);
  void SetInterrupt(G4bool interrupt);

  // get methods
  G4int GetTrackParticleID() const;
  G4int GetParentParticleID() const;
  G4double GetPDGLifetime() const;
  G4int GetPDGEncoding() const;
  const TMCParticleStatus* GetInitialTrackStatus() const;
  G4bool IsUserTrack() const;
  G4bool IsStop() const;
  G4bool IsInterrupt() const;

 private:
  // data members

  G4int fTrackParticleID;  ///< the index of track particle in VMC stack
  G4int fParentParticleID; ///< the index of parent track in VMC stack
  G4double fPDGLifetime;   ///< the original particle PDG lifetime
  G4double fPDGEncoding;   ///< the particle PDG encoding
  G4bool fIsUserTrack;     ///< true if defined by user and not primary track
  G4bool fStop;            ///< true if track should be stopped
  G4bool fInterrupt;       ///< true if track should be interrupted
  /// Pointer holds some additional information on the particle status e.g. for
  /// the case the track has already been transported up to the current point in
  /// another engine.
  TMCParticleStatus* fInitialTrackStatus;
};

// inline methods
#include "TG4TrackInformation.icc"

#endif // TG4_TRACK_INFORMATION_H
