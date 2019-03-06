#ifndef TG4_STACK_POPPER_H
#define TG4_STACK_POPPER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4StackPopper.h
/// \brief Definition of the TG4StackPopper class
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4VProcess.hh>

class TVirtualMCStack;

class G4Track;

/// \ingroup physics
/// \brief The process which pops particles defined by user from
///        the VMC stack and passes them to tracking
///
/// \author I. Hrivnacova; IPN Orsay

class TG4StackPopper: public G4VProcess
{
  public:
    TG4StackPopper(const G4String& processName = "stackPopper");
    virtual ~TG4StackPopper();

    // static access method
    static TG4StackPopper* Instance();

    // methods
    virtual G4bool IsApplicable(const G4ParticleDefinition& /*particleDefinition*/);

    virtual G4double PostStepGetPhysicalInteractionLength(
                           const G4Track& track,
                           G4double previousStepSize,
                           G4ForceCondition* condition);

    virtual G4VParticleChange* PostStepDoIt(
                                   const G4Track& track,
                                   const G4Step& step);

    // No operation in AlongStepDoIt and AtRestDoIt

    virtual G4double AlongStepGetPhysicalInteractionLength(
                           const G4Track& /*track*/,
                           G4double  /*previousStepSize*/,
                           G4double  /*currentMinimumStep*/,
                           G4double& /*proposedSafety*/,
                           G4GPILSelection* /*selection*/)  { return -1.0; }

    virtual G4double AtRestGetPhysicalInteractionLength(
                           const G4Track& /*track*/,
                           G4ForceCondition* /*condition*/) { return -1.0; }

    virtual G4VParticleChange* AlongStepDoIt(
                                   const G4Track& /*track*/,
                                   const G4Step& /*step*/) { return 0; }

    virtual G4VParticleChange* AtRestDoIt(
                                   const G4Track& /*track*/,
                                   const G4Step& /*step*/) { return 0; }

    void Notify();
    void Reset();
    void SetMCStack(TVirtualMCStack*  mcStack);
    void SetDoExclusiveStep(G4TrackStatus trackStatus);

    G4bool HasPoppedTracks() const;

  private:
    /// Not implemented
    TG4StackPopper(const TG4StackPopper& right);
    /// Not implemented
    TG4StackPopper& operator = (const TG4StackPopper& right);

    /// this instance
    static G4ThreadLocal TG4StackPopper*  fgInstance;

    /// Cached pointer to thread-local VMC stack
    TVirtualMCStack*  fMCStack;

    /// the counter for popped tracks
    G4int  fNofDoneTracks;

    /// The indication for performing exclusive step
    ///
    /// It is set in stepping action when a track is not alive and there are
    /// user tracks in the VMC stack
    G4bool fDoExclusiveStep;

    /// The track status to be restored after performing exclusive step
    G4TrackStatus fTrackStatus;
};

// inline methods

inline TG4StackPopper* TG4StackPopper::Instance() {
  /// Return this instance.
  return fgInstance;
}

inline G4bool TG4StackPopper::IsApplicable(
                   const G4ParticleDefinition& /*particleDefinition*/) {
  /// Applicable to any particles
  return true;
}

inline void TG4StackPopper::SetMCStack(TVirtualMCStack*  mcStack) {
  /// Set  cached pointer to thread-local VMC stack
  fMCStack = mcStack;
}

#endif //TG4_STACK_POPPER_H
